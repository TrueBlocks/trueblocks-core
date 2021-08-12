/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_init() {
    CToml config(configPath("pinMan.toml"));
    bool enabled = config.getConfigBool("enabled", "downloadManifest", true);
    if (!enabled) {
        LOG_INFO("Manifest not downloaded. Not initializing.");
        return true;
    }

    if (!pinlib_downloadManifest())
        return usage("Could not freshen manifest from smart contract");

    if (!folderExists(getIndexPath("")))
        return false;

    establishFolder(getIndexPath("blooms/"));
    establishFolder(getIndexPath("finalized/"));

    // If the user is calling here, she wants a fresh read even if we've not just freshened.
    pins.clear();
    pinlib_readManifest(pins);
    for (auto pin : pins) {
        if (!pinlib_getChunkFromRemote(pin, BLOOM_TYPE, sleep) || shouldQuit())
            break;
        string_q bloomFn = pin.Format(getIndexPath("blooms/[{FILENAME}].bloom.gz"));
        if (pin_locally) {
            ostringstream os;
            os << "ipfs add -Q --pin \"" << bloomFn + "\"";
            string_q newHash = doCommand(os.str());
            LOG_INFO(cGreen, "Re-pinning ", pin.fileName, cOff, " ==> ", newHash, " ",
                     (pin.bloomHash == newHash ? greenCheck : redX));
        }
        ::remove(bloomFn.c_str());
        LOG4(cGreen, "Removed zip file ", bloomFn, cOff);

        if (init_all) {
            if (!pinlib_getChunkFromRemote(pin, CHUNK_TYPE, sleep) || shouldQuit())
                break;
            string_q binFn = pin.Format(getIndexPath("finalized/[{FILENAME}].bin.gz"));
            if (pin_locally) {
                ostringstream os;
                os << "ipfs add -Q --pin \"" << binFn + "\"";
                string_q newHash = doCommand(os.str());
                LOG_INFO(cGreen, "Re-pinning ", pin.fileName, cOff, " ==> ", newHash, " ",
                         (pin.indexHash == newHash ? greenCheck : redX));
                usleep(500000);
            }
            ::remove(binFn.c_str());
            LOG4(cGreen, "Removed zip file ", binFn, cOff);
        }
        // pinlib_pinLocally(pin, pin_locally /* pinBloom */, (pin_locally && init_all) /* pinChunk */);
    }

    LOG_INFO(bBlue, "Pins were (re)initialized.                                      ", cOff);
    return true;  // do not continue
}
