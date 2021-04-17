#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_init(void) {
    if (!freshen_from_remote())
        return usage("Could not freshen remote pin data");
    CPinnedChunkArray remotePins;
    pinlib_readPinList(remotePins, true, false);
    for (auto pin : remotePins) {
        // string_q outFile = getIndexPath("blooms/" + pin.fileName + ".bloom.gz ");
        string_q outFile = "blooms/" + pin.fileName + ".bloom";
        string_q zipFile = outFile + ".gz";
        if (!fileExists(outFile) && !fileExists(zipFile)) {
            cout << "curl -o " << zipFile << " \"http://gateway.ipfs.io/ipfs/" << pin.bloomHash << "\"" << endl;
        }
    }
    return true;  // do not continue
}
