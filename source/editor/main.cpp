
#include <editor/toolchain/toolchain.hpp>


int main()
{
    pspedit::toolchain_configuration tcc;
    tcc.cache_directory = "";
    pspedit::toolchain_manager tcm(tcc);
    std::vector<pspedit::tool_diagnostic> tcd;
    std::vector<pspedit::editor_action> tca = tcm.detect_actions(tcd);
    pspedit::tool_result _result;

    if (tca.size() > 0) {
        _result = pspedit::execute_action(tca[0]);
    }

    // pspedit::docker_manager _dcm("docker");
    // pspedit::docker_info _dinf = _dcm.probe_info();
    return 0;
}