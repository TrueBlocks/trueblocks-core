#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_init() {
    if (freshen && !freshen_from_remote())
        return usage("Could not freshen remote pin data");

    if (!folderExists(getIndexPath("")))
        return false;

    establishFolder(getIndexPath("blooms/"));
    establishFolder(getIndexPath("finalized/"));

    CPinnedChunkArray remotePins;
    pinlib_readPinList(remotePins);
    for (auto pin : remotePins) {
        if (!pinlib_getFileFromIPFS(pin, BLOOM_TYPE) || shouldQuit())
            break;
        usleep(2500);  // do not remove cruft - stays responsive to control+C
    }

    return true;  // do not continue
}
