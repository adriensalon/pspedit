#include <optional>
#include <stdexcept>
#include <string_view>

#include <dwarf.h>
#include <libdwarf.h>

#include <import/dwarf.hpp>

struct dwarf_asset_reference {
    std::filesystem::path path;
    std::string target_type;
};

struct dwarf_component_reference {
    std::string name;
    std::string target_type;
};

struct dwarf_entity_reference {
    std::string name;
};

struct dwarf_scene_reference {
    std::string name;
};

struct dwarf_script {
    std::string qualified_name;
    std::vector<dwarf_asset_reference> asset_references;
    std::vector<dwarf_component_reference> component_references;
    std::vector<dwarf_entity_reference> entity_references;
    std::vector<dwarf_scene_reference> scene_references;
    bool has_on_create = false;
    bool has_on_update = false;
    bool has_on_destroy = false;
};

[[nodiscard]] std::vector<dwarf_script> dwarf_parse(const std::filesystem::path& dwarf_file);

static std::runtime_error dwarf_error(const char* where, Dwarf_Error err)
{
    const char* msg = dwarf_errmsg(err);
    std::string s = std::string(where) + ": " + (msg ? msg : "unknown libdwarf error");
    return std::runtime_error(s);
}

static std::string die_name(Dwarf_Die die)
{
    char* n = nullptr;
    if (dwarf_diename(die, &n, nullptr) == DW_DLV_OK && n)
        return std::string(n);
    return {};
}

static bool die_tag(Dwarf_Die die, Dwarf_Half& out_tag)
{
    return dwarf_tag(die, &out_tag, nullptr) == DW_DLV_OK;
}

static std::optional<std::string> attr_string(Dwarf_Die die, Dwarf_Half attr)
{
    Dwarf_Attribute a = nullptr;
    if (dwarf_attr(die, attr, &a, nullptr) != DW_DLV_OK)
        return std::nullopt;

    char* s = nullptr;
    if (dwarf_formstring(a, &s, nullptr) == DW_DLV_OK && s)
        return std::string(s);

    // DW_FORM_strp etc. are typically handled by dwarf_formstring already.
    return std::nullopt;
}

static std::optional<Dwarf_Unsigned> attr_udata(Dwarf_Die die, Dwarf_Half attr)
{
    Dwarf_Attribute a = nullptr;
    if (dwarf_attr(die, attr, &a, nullptr) != DW_DLV_OK)
        return std::nullopt;

    Dwarf_Unsigned v = 0;
    if (dwarf_formudata(a, &v, nullptr) == DW_DLV_OK)
        return v;
    return std::nullopt;
}

static std::optional<Dwarf_Off> attr_ref_global(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Half attr)
{
    (void)dbg;
    Dwarf_Attribute a = nullptr;
    if (dwarf_attr(die, attr, &a, nullptr) != DW_DLV_OK)
        return std::nullopt;

    Dwarf_Off off = 0;
    if (dwarf_global_formref(a, &off, nullptr) == DW_DLV_OK)
        return off;
    return std::nullopt;
}

static std::optional<Dwarf_Die> off_to_die(Dwarf_Debug dbg, Dwarf_Off off)
{
    Dwarf_Die out = nullptr;
    if (dwarf_offdie_b(dbg, off, /*is_info=*/true, &out, nullptr) == DW_DLV_OK && out)
        return out;
    return std::nullopt;
}

// Unwrap typedef/const/volatile/pointer/reference wrappers to the underlying type DIE.
static std::optional<Dwarf_Die> unwrap_type(Dwarf_Debug dbg, Dwarf_Die type_die)
{
    for (int i = 0; i < 64; ++i) { // prevent cycles
        Dwarf_Half tag = 0;
        if (!die_tag(type_die, tag))
            break;

        const bool is_wrapper = tag == DW_TAG_typedef || tag == DW_TAG_const_type || tag == DW_TAG_volatile_type || tag == DW_TAG_restrict_type || tag == DW_TAG_pointer_type || tag == DW_TAG_reference_type || tag == DW_TAG_rvalue_reference_type;

        if (!is_wrapper)
            return type_die;

        auto ref_off = attr_ref_global(dbg, type_die, DW_AT_type);
        if (!ref_off)
            return type_die;
        auto next = off_to_die(dbg, *ref_off);
        if (!next)
            return type_die;
        type_die = *next;
    }
    return type_die;
}

// For GCC, DW_AT_name of the instantiation is often "pspedit::asset_ref<pspedit::image>".
// Extract base name and template argument string (naive but effective).
struct TypeMatch {
    enum Kind { AssetRef,
        ComponentRef,
        EntityRef,
        SceneRef,
        Unknown } kind
        = Unknown;
    std::string target_type; // T for *_ref<T>
};

static std::string trim(std::string_view sv)
{
    while (!sv.empty() && (sv.front() == ' ' || sv.front() == '\t'))
        sv.remove_prefix(1);
    while (!sv.empty() && (sv.back() == ' ' || sv.back() == '\t'))
        sv.remove_suffix(1);
    return std::string(sv);
}

static std::optional<std::string> extract_template_arg(std::string_view full, std::string_view prefix)
{
    // full contains prefix + "<...>"
    auto p = full.find(prefix);
    if (p == std::string_view::npos)
        return std::nullopt;
    auto lt = full.find('<', p + prefix.size());
    if (lt == std::string_view::npos)
        return std::nullopt;

    // Find matching '>' (handle nested templates superficially by depth count)
    int depth = 0;
    for (size_t i = lt; i < full.size(); ++i) {
        if (full[i] == '<')
            depth++;
        else if (full[i] == '>') {
            depth--;
            if (depth == 0) {
                auto inside = full.substr(lt + 1, i - (lt + 1));
                return trim(inside);
            }
        }
    }
    return std::nullopt;
}

static TypeMatch classify_type_name(std::string_view type_name)
{
    TypeMatch m {};

    // Adjust these strings to your real type spellings in DWARF (namespaces matter).
    constexpr std::string_view ASSET_PREFIX = "pspedit::asset_ref";
    constexpr std::string_view COMP_PREFIX = "pspedit::component_ref";

    if (type_name.find(ASSET_PREFIX) != std::string_view::npos) {
        m.kind = TypeMatch::AssetRef;
        if (auto t = extract_template_arg(type_name, ASSET_PREFIX))
            m.target_type = *t;
        return m;
    }
    if (type_name.find(COMP_PREFIX) != std::string_view::npos) {
        m.kind = TypeMatch::ComponentRef;
        if (auto t = extract_template_arg(type_name, COMP_PREFIX))
            m.target_type = *t;
        return m;
    }

    // Optional non-template refs (rename to whatever you use).
    if (type_name.find("pspedit::entity_ref") != std::string_view::npos) {
        m.kind = TypeMatch::EntityRef;
        return m;
    }
    if (type_name.find("pspedit::scene_ref") != std::string_view::npos) {
        m.kind = TypeMatch::SceneRef;
        return m;
    }

    return m;
}

static std::optional<std::string> type_die_display_name(Dwarf_Debug dbg, Dwarf_Die type_die)
{
    // Prefer DW_AT_name on the underlying type.
    if (auto n = attr_string(type_die, DW_AT_name))
        return n;

    // Some types are unnamed; you can extend this if needed.
    (void)dbg;
    return std::nullopt;
}

static bool in_pspedit_scripts_namespace(const std::vector<std::string>& ns_stack)
{
    // We consider "inside" if the stack ends with ["pspedit","scripts"] (and possibly deeper).
    if (ns_stack.size() < 2)
        return false;
    return ns_stack[0] == "pspedit" && ns_stack[1] == "scripts";
}

static std::string make_qualified(const std::vector<std::string>& ns_stack, const std::string& name)
{
    std::string q;
    for (size_t i = 0; i < ns_stack.size(); ++i) {
        if (i)
            q += "::";
        q += ns_stack[i];
    }
    if (!q.empty())
        q += "::";
    q += name;
    return q;
}

static void scan_script_children(
    Dwarf_Debug dbg,
    Dwarf_Die script_die,
    dwarf_script& out)
{
    Dwarf_Die child = nullptr;
    if (dwarf_child(script_die, &child, nullptr) != DW_DLV_OK || !child)
        return;

    for (Dwarf_Die cur = child; cur;) {
        Dwarf_Half tag = 0;
        if (!die_tag(cur, tag)) {
            Dwarf_Die sib = nullptr;
            if (dwarf_siblingof_b(dbg, cur, true, &sib, nullptr) != DW_DLV_OK)
                break;
            cur = sib;
            continue;
        }

        if (tag == DW_TAG_member) {
            const std::string member_name = die_name(cur);

            // Get member type DIE
            auto type_off = attr_ref_global(dbg, cur, DW_AT_type);
            if (type_off) {
                auto type_die = off_to_die(dbg, *type_off);
                if (type_die) {
                    auto base_die = unwrap_type(dbg, *type_die);
                    if (base_die) {
                        auto disp = type_die_display_name(dbg, *base_die);
                        if (disp) {
                            auto match = classify_type_name(*disp);
                            switch (match.kind) {
                            case TypeMatch::AssetRef: {
                                dwarf_asset_reference r;
                                r.path = std::filesystem::path(member_name); // used as field identifier/label
                                r.target_type = match.target_type;
                                out.asset_references.push_back(std::move(r));
                            } break;
                            case TypeMatch::ComponentRef: {
                                dwarf_component_reference r;
                                r.name = member_name;
                                r.target_type = match.target_type;
                                out.component_references.push_back(std::move(r));
                            } break;
                            case TypeMatch::EntityRef: {
                                dwarf_entity_reference r;
                                r.name = member_name;
                                out.entity_references.push_back(std::move(r));
                            } break;
                            case TypeMatch::SceneRef: {
                                dwarf_scene_reference r;
                                r.name = member_name;
                                out.scene_references.push_back(std::move(r));
                            } break;
                            default:
                                break;
                            }
                        }
                    }
                }
            }
        } else if (tag == DW_TAG_subprogram) {
            // Methods may be represented either directly or via DW_AT_specification.
            std::string fname;

            if (auto n = attr_string(cur, DW_AT_name)) {
                fname = *n;
            } else {
                // Try specification indirection
                auto spec_off = attr_ref_global(dbg, cur, DW_AT_specification);
                if (spec_off) {
                    if (auto spec_die = off_to_die(dbg, *spec_off)) {
                        if (auto sn = attr_string(*spec_die, DW_AT_name))
                            fname = *sn;
                    }
                }
            }

            if (fname == "on_create")
                out.has_on_create = true;
            if (fname == "on_update")
                out.has_on_update = true;
            if (fname == "on_destroy")
                out.has_on_destroy = true;
        }

        Dwarf_Die sib = nullptr;
        if (dwarf_siblingof_b(dbg, cur, true, &sib, nullptr) != DW_DLV_OK)
            break;
        cur = sib;
    }
}

static void walk_die_tree(
    Dwarf_Debug dbg,
    Dwarf_Die die,
    std::vector<std::string>& ns_stack,
    std::vector<dwarf_script>& out_scripts)
{
    Dwarf_Half tag = 0;
    if (!die_tag(die, tag))
        return;

    bool pushed_ns = false;

    if (tag == DW_TAG_namespace) {
        // Enter namespace
        auto n = attr_string(die, DW_AT_name);
        if (n && !n->empty()) {
            ns_stack.push_back(*n);
            pushed_ns = true;
        }
    } else if (tag == DW_TAG_structure_type || tag == DW_TAG_class_type) {
        const std::string name = die_name(die);
        if (!name.empty() && in_pspedit_scripts_namespace(ns_stack)) {
            dwarf_script s;
            s.qualified_name = make_qualified(ns_stack, name);
            scan_script_children(dbg, die, s);
            out_scripts.push_back(std::move(s));
        }
    }

    // Recurse into children
    Dwarf_Die child = nullptr;
    if (dwarf_child(die, &child, nullptr) == DW_DLV_OK && child) {
        for (Dwarf_Die cur = child; cur;) {
            walk_die_tree(dbg, cur, ns_stack, out_scripts);

            Dwarf_Die sib = nullptr;
            if (dwarf_siblingof_b(dbg, cur, true, &sib, nullptr) != DW_DLV_OK)
                break;
            cur = sib;
        }
    }

    if (pushed_ns)
        ns_stack.pop_back();
}

[[nodiscard]] std::vector<dwarf_script> dward_parse(const std::filesystem::path& dwarf_file)
{
    Dwarf_Debug dbg = nullptr;
    Dwarf_Error err = nullptr;

    // Open ELF and initialize libdwarf
    int res = dwarf_init_path(
        dwarf_file.string().c_str(), // path
        nullptr, // true_path_out_buffer
        0, // true_path_bufferlen
        DW_GROUPNUMBER_ANY, // groupnumber
        nullptr, // errhand
        nullptr, // errarg
        &dbg, // dbg
        &err // error
    );

    if (res != DW_DLV_OK || !dbg) {
        throw dwarf_error("dwarf_init_path", err);
    }

    std::vector<dwarf_script> scripts;
    std::vector<std::string> ns_stack;

    // Iterate compilation units
    Dwarf_Unsigned cu_header_length = 0;
    Dwarf_Half version_stamp = 0;
    Dwarf_Off abbrev_offset = 0;
    Dwarf_Half address_size = 0;
    Dwarf_Half length_size = 0;
    Dwarf_Half extension_size = 0;
    Dwarf_Sig8 type_signature {};
    Dwarf_Unsigned typeoffset = 0;
    Dwarf_Unsigned next_cu_header_offset = 0;
    Dwarf_Unsigned header_offset = 0; // <-- matches arg 10 in your prototype
    Dwarf_Half header_cu_type = 0; // <-- matches arg 12 (Dwarf_Half*)

    while (true) {
        int ncu = dwarf_next_cu_header_d(
            dbg,
            /*is_info=*/true,
            &cu_header_length, // 3
            &version_stamp, // 4
            &abbrev_offset, // 5 (Dwarf_Off*)
            &address_size, // 6
            &length_size, // 7
            &extension_size, // 8
            &type_signature, // 9
            &typeoffset, // 10
            &next_cu_header_offset, // 11
            &header_cu_type, // 12  <-- THIS is the fix
            &err // 13
        );

        if (ncu == DW_DLV_NO_ENTRY)
            break;
        if (ncu != DW_DLV_OK)
            throw dwarf_error("dwarf_next_cu_header_d", err);

        // CU DIE root:
        Dwarf_Die cu_die = nullptr;
        if (dwarf_siblingof_b(dbg, nullptr, /*is_info=*/true, &cu_die, &err) == DW_DLV_OK && cu_die) {
            ns_stack.clear();
            walk_die_tree(dbg, cu_die, ns_stack, scripts);
        }
    }

    dwarf_finish(dbg);
    return scripts;
}

void import_dwarf(const std::filesystem::path& dwarf_path)
{

}
