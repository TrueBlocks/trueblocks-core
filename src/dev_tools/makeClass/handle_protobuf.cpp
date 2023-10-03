#include "utillib.h"
#include "options.h"

bool COptions::handle_protobuf(void) {
    CToml config(rootConfigToml_makeClass);
    bool enabled = config.getConfigBool("enabled", "protobuf", false);
    if (!enabled) {
        LOG_WARN("Skipping protobuf compilation...");
        return true;
    }

    LOG_INFO(cYellow, "handling protobufs...", cOff);

    string_q res = doCommand(
        "protoc"
        " --go_out=."
        " --go_opt=paths=source_relative"
        " --go-grpc_out=."
        " --go-grpc_opt=paths=source_relative"
        " apps/chifra/proto/chifra.proto");
    return res.empty();
}