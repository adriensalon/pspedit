#include <limits>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <editor/core/project.hpp>
#include <editor/core/log.hpp>

namespace pspedit {
namespace {

    static void _push_bytes(std::vector<u8>& out, const void* data, const std::size_t size)
    {
        const u8* _data = reinterpret_cast<const u8*>(data);
        out.insert(out.end(), _data, _data + size);
    }

    [[nodiscard]] static u8 _f32_to_u8_normalized(const f32 value)
    {
        return static_cast<u8>(std::clamp(value, 0.f, 1.f) * 255.f + 0.5f);
    }

    [[nodiscard]] static u32 _storage_size(const vertex_storage storage)
    {
        switch (storage) {
        case vertex_storage::normalized_u8:
            return 1;
        case vertex_storage::u16:
            return 2;
        case vertex_storage::i16:
            return 2;
        case vertex_storage::f32:
            return 4;
        }
        return 0;
    }

    [[nodiscard]] static vertex_attribute _build_vertex_attribute(u16& offset, const vertex_semantic semantic, const vertex_storage storage, const u8 components)
    {
        vertex_attribute _attribute = {};

        _attribute.semantic = semantic;
        _attribute.storage = storage;
        _attribute.components = components;
        _attribute.offset = offset;

        offset += static_cast<u16>(_storage_size(storage) * components);
        return _attribute;
    }

    [[nodiscard]] static vertex_descriptor _build_vertex_descriptor(const aiMesh* mesh)
    {
        vertex_descriptor _descriptor = {};
        u16 _offset = 0;

        if (mesh->HasPositions()) {
            _descriptor.attributes[_descriptor.attributes_count++] = _build_vertex_attribute(_offset, vertex_semantic::position, vertex_storage::f32, 3);
        }
        if (mesh->HasNormals()) {
            _descriptor.attributes[_descriptor.attributes_count++] = _build_vertex_attribute(_offset, vertex_semantic::normal, vertex_storage::f32, 3);
        }
        if (mesh->HasVertexColors(0)) {
            _descriptor.attributes[_descriptor.attributes_count++] = _build_vertex_attribute(_offset, vertex_semantic::color, vertex_storage::normalized_u8, 4);
        }
        if (mesh->HasTextureCoords(0)) {
            _descriptor.attributes[_descriptor.attributes_count++] = _build_vertex_attribute(_offset, vertex_semantic::texcoord, vertex_storage::f32, 2);
        }
        // if (mesh->HasBones()) {
        //     _descriptor.skin_weights = 4;
        //     _descriptor.attributes[_descriptor.attributes_count++] = _build_vertex_attribute(_offset, vertex_semantic::weights, vertex_storage::f32, 4);
        //     _descriptor.attributes[_descriptor.attributes_count++] = _build_vertex_attribute(_offset, vertex_semantic::indices, vertex_storage::u16, 4);
        // }

        _descriptor.morph_targets = 1;
        _descriptor.stride = _offset;
        return _descriptor;
    }

    [[nodiscard]] static std::vector<u8> _build_vertex_buffer(const aiMesh* mesh, const vertex_descriptor& descriptor)
    {
        std::vector<u8> _vertex_data;
        _vertex_data.reserve(static_cast<std::size_t>(mesh->mNumVertices) * descriptor.stride);

        // std::vector<skin_data> skin;
        // if (mesh->HasBones()) {
        //     skin = extract_skinning(mesh);
        // }

        for (u32 _vertex_index = 0; _vertex_index < mesh->mNumVertices; ++_vertex_index) {
            if (mesh->HasPositions()) {
                const f32 _position[3] = { mesh->mVertices[_vertex_index].x, mesh->mVertices[_vertex_index].y, mesh->mVertices[_vertex_index].z };
                _push_bytes(_vertex_data, _position, sizeof(_position));
            }

            if (mesh->HasNormals()) {
                const f32 _normal[3] = { mesh->mNormals[_vertex_index].x, mesh->mNormals[_vertex_index].y, mesh->mNormals[_vertex_index].z };
                _push_bytes(_vertex_data, _normal, sizeof(_normal));
            }

            if (mesh->HasVertexColors(0)) {
                const aiColor4D& _assimp_color = mesh->mColors[0][_vertex_index];
                const u8 _color[4] = { _f32_to_u8_normalized(_assimp_color.r), _f32_to_u8_normalized(_assimp_color.g), _f32_to_u8_normalized(_assimp_color.b), _f32_to_u8_normalized(_assimp_color.a) };
                _push_bytes(_vertex_data, _color, sizeof(_color));
            }

            if (mesh->HasTextureCoords(0)) {
                const f32 _uv0[2] = { mesh->mTextureCoords[0][_vertex_index].x, mesh->mTextureCoords[0][_vertex_index].y };
                _push_bytes(_vertex_data, _uv0, sizeof(_uv0));
            }

            // if (mesh->HasBones()) {
            //     _push_bytes(_vertex_data, skin[v].weights.data(), sizeof(float) * 4);
            //     _push_bytes(_vertex_data, skin[v].indices.data(), sizeof(u16) * 4);
            // }
        }

        return _vertex_data;
    }

    [[nodiscard]] static std::vector<u8> _build_index_buffer(const aiMesh* mesh, index_format& out_format)
    {
        const bool use_u16 = mesh->mNumVertices <= std::numeric_limits<u16>::max();
        out_format = use_u16 ? index_format::u16 : index_format::u32;

        std::vector<u8> data;
        data.reserve(static_cast<size_t>(mesh->mNumFaces) * 3 * (use_u16 ? sizeof(u16) : sizeof(u32)));

        for (u32 f = 0; f < mesh->mNumFaces; ++f) {
            const aiFace& face = mesh->mFaces[f];
            if (face.mNumIndices != 3) {
                continue;
            }

            if (use_u16) {
                const u16 tri[3] = { static_cast<u16>(face.mIndices[0]), static_cast<u16>(face.mIndices[1]), static_cast<u16>(face.mIndices[2]) };
                _push_bytes(data, tri, sizeof(tri));
            } else {
                const u32 tri[3] = { face.mIndices[0], face.mIndices[1], face.mIndices[2] };
                _push_bytes(data, tri, sizeof(tri));
            }
        }

        return data;
    }

}

void import_assimp(const std::filesystem::path& import_path)
{
    Assimp::Importer _importer;
    const aiScene* _scene = _importer.ReadFile(import_path.string(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    if (!_scene) {
        log_error("Editor", std::string("Impossible to load mesh with ") + std::string(_importer.GetErrorString()));
        return;
    }
    if (_scene->mNumMeshes == 0) {
        log_error("Editor", "No mesh found in file " + import_path.string());
        return;
    }

    for (u32 _mesh_index = 0; _mesh_index < _scene->mNumMeshes; ++_mesh_index) {
        const aiMesh* _mesh = _scene->mMeshes[_mesh_index];
        const vertex_descriptor _vertex_descriptor = _build_vertex_descriptor(_mesh);
        index_format _index_format = index_format::u16;

        project_imported_asset<mesh_asset> _import = {};
        _import.editor_name = import_path.filename().replace_extension().string();
        _import.import_path = import_path;
        _import.asset.version = 1;
        _import.asset.vertices = _build_vertex_buffer(_mesh, _vertex_descriptor);
        _import.asset.indices = _build_index_buffer(_mesh, _index_format);
        _import.asset.vertex_buffer.vertex = _vertex_descriptor;
        _import.asset.vertex_buffer.usage = buffer_usage::static_draw;
        _import.asset.vertex_buffer.count = _mesh->mNumVertices;
        _import.asset.index_buffer.usage = buffer_usage::static_draw;
        _import.asset.index_buffer.format = _index_format;
        _import.asset.index_buffer.count = _mesh->mNumFaces * 3;
		current_project->meshes.emplace(mesh_id(), std::move(_import));
		// TODO generate new id
    }
}

}