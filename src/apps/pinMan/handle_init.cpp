#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_init(void) {
    if (!freshen_from_remote())
        return usage("Could not freshen remote pin data");

    CPinnedChunkArray remotePins;
    pinlib_readPinList(remotePins, true, false);
    for (auto pin : remotePins) {
        string_q outFile = "blooms/" + pin.fileName + ".bloom";
        if (!fileExists(getIndexPath(outFile))) {
            string_q zipFile = outFile + ".gz";
            if (!fileExists(getIndexPath(zipFile))) {
                // download from ipfs gateway
                ostringstream cmd;
                cmd << "curl -o " << getIndexPath(zipFile) << " \"http://gateway.ipfs.io/ipfs/" << pin.bloomHash << "\" ";
                // LOG_INFO("Downloading " + zipFile);
                LOG_INFO("Downloading " + cmd.str());
                doCommand(cmd.str());
            }
            if (!fileExists(getIndexPath(zipFile))) {
                LOG_WARN("Could download bloom file zip ", zipFile);
            } else {
                ostringstream cmd;
                cmd << "cd " << getIndexPath("") << " && gunzip --keep " << zipFile << " && cd -";
                doCommand(cmd.str());
                if (!fileExists(getIndexPath(outFile))) {
                    LOG_WARN("Could not create bloom file ", outFile);
                }
            }
        } else {
            LOG_INFO("Bloom file ", outFile, " exists.");
        }
    }
    return true;  // do not continue
}
