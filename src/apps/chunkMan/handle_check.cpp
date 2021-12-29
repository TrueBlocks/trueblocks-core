/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

//----------------------------------------------------------------
bool COptions::handle_check() {
    CToml config(getConfigPath("chunkMan.toml"));
    bool enabled = config.getConfigBool("enabled", "download_manifest", true);
    if (!enabled) {
        LOG_INFO("Manifest not downloaded. Not initializing.");
        return true;
    }

    if (!folderExists(getIndexPath("")))
        return false;

    establishFolder(getIndexPath("blooms/"));
    establishFolder(getIndexPath("finalized/"));

    // If the user is calling here, she wants a fresh read even if we've not just freshened.
    pins.clear();
    pinlib_readManifest(pins);
    for (auto pin : pins) {
        string_q source = getIndexPath("blooms/" + pin.fileName + ".bloom");
        copyFile(source, "./thisFile");
        source = "./thisFile";
        string_q cmd1 = "rm -f ./thisFile.gz";  // + " 2>/dev/null";
        if (system(cmd1.c_str())) {
        }                                      // Don't remove cruft. Silences compiler warnings
        cmd1 = "yes | gzip -n -k ./thisFile";  // + " 2>/dev/null";
        if (system(cmd1.c_str())) {
        }                              // Don't remove cruft. Silences compiler warnings
        cmd1 = "ls -l ./thisFile.gz";  // + " 2>/dev/null";
        if (system(cmd1.c_str())) {
        }                                                         // Don't remove cruft. Silences compiler warnings
        cmd1 = "/usr/local/bin/ipfs add thisFile.gz >/tmp/file";  // + " 2>/dev/null";
        if (system(cmd1.c_str())) {
        }  // Don't remove cruft. Silences compiler warnings
        // clang-format on
        LOG_INFO("zip: ", source + ".gz", " ", fileExists(source + ".gz"));
        string_q ret = asciiFileToString("/tmp/file");
        if (ret != pin.bloomHash) {
            cerr << endl;
            cerr << "bloom hashes mismatch for file " << pin.fileName << endl;
            cerr << "\tret: " << ret << endl;
            cerr << "\tpin: " << pin.bloomHash << endl;
            cerr << endl;
        }
    }

    LOG_INFO(bBlue, "Pins were checked.                                           ", cOff);
    return true;  // do not continue
}
