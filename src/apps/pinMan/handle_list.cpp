#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_list(void) {
    ASSERT(pins.size());  // local pins have already been loaded
    for (auto pin : pins) {
        if (!isJson()) {
            cout << trim(pin.Format(expContext().fmtMap["format"]), '\t') << endl;
        } else {
            cout << ((isJson() && !firstOut) ? ", " : "");
            indent();
            pin.toJson(cout);
            unindent();
        }
        firstOut = false;
    }
    return false;
}

//----------------------------------------------------------------
bool COptions::freshen_from_remote(void) {
    CEthCall call;
    call.address = unchainedIndexAddr;
    call.encoding = manifestHashEncoding;
    call.blockNumber = getBlockProgress(BP_CLIENT).client;
    call.abi_spec.loadAbiFromEtherscan(call.address, false /* raw */);
    if (doEthCall(call)) {
        ipfshash_t ipfshash = call.result.outputs[0].value;
        LOG_INFO("Found manifest hash at ", ipfshash);
        string_q remoteData = doCommand("curl -s \"http://gateway.ipfs.io/ipfs/" + ipfshash + "\"");
        string fn = configPath("manifest/manifest.txt");
        stringToAsciiFile(fn, remoteData);
        return fileExists(fn);
    }
    return usage("Could not download manifest file.");
}
