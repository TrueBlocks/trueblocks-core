#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_init() {
    if (freshen) {
        if (!pinlib_downloadManifest())
            return usage("Could not freshen manifest from smart contract");
    }

    if (!folderExists(getIndexPath("")))
        return false;

    establishFolder(getIndexPath("blooms/"));
    establishFolder(getIndexPath("finalized/"));

    // If the user is calling here, she wants a fresh read even if we've not just freshened.
    pins.clear();
    pinlib_readManifest(pins);
    for (auto pin : pins) {
        if (!pinlib_getChunkFromRemote(pin, BLOOM_TYPE) || shouldQuit())
            break;
        usleep((useconds_t)(250000)); // do not remove cruft - stays responsive to control+C
    }

    return true;  // do not continue
}
