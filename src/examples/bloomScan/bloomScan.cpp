/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool visitBloom(const string_q& path, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    COptions opt;
    if (opt.parseArguments(argc, argv)) {
        forEveryBloomFile( visitBloom, &opt, opt.start, opt.nBlocks, opt.skip );
    }

    return 0;
}

//-----------------------------------------------------------------------
bool visitBloom(const string_q& path, void *data) {

    if (endsWith(path, "/")) {
        forAllFiles(path + "*", visitBloom, data);

    } else {
extern bool displayBloom(blknum_t bn, const SFBloom& bloom, void *data);
        if (endsWith(path, ".bin")) {
            SFBloom bloom;
            SFArchive archive(READING_ARCHIVE);
            if (archive.Lock(path, binaryReadOnly, LOCK_NOWAIT)) {
                SFBloomArray blooms;
                archive >> blooms;
                archive.Release();
                for (size_t i = 0 ; i < blooms.size() ; i++) {
                    bloom |= blooms[i];
                }
            }
            COptions *options = (COptions*)data;
            if (options->asData)
                cout << bnFromPath(path) << "," << fileSize(path) << "," << bitsTwiddled(bloom) << "\n";
            else
                displayBloom(bnFromPath(path), bloom, data);
        }
    }
    return true;
}

//-------------------------------------------------------------
bool displayBloom(blknum_t bn, const SFBloom& bloom, void *data) {
    string_q s = bloom2Bytes(bloom);
    COptions *opt = (COptions*)data;
    if (opt->mode == "short") {
        size_t len = s.length();
        replace(s,    "0x",     "");
        replaceAny(s, "0",      "");
        replaceAny(s, "1248",   ".");
        replaceAny(s, "3569ac", ".");
        replaceAny(s, "7bde",   "+");
        replaceAny(s, "f",      "@");
        size_t rem = len - s.length();
        s = s + string_q(rem, ' ') + "|";
    } else {
        replace(s,    "0x", cOff);
        replaceAll(s, "0",  " ");
        replaceAll(s, "1",  "[.");
        replaceAll(s, "2",  "[.");
        replaceAll(s, "4",  "[.");
        replaceAll(s, "8",  "[.");
        replaceAll(s, "3",  "+.");
        replaceAll(s, "5",  "+.");
        replaceAll(s, "6",  "+.");
        replaceAll(s, "9",  "+.");
        replaceAll(s, "a",  "+.");
        replaceAll(s, "c",  "+.");
        replaceAll(s, "7",  "-+");
        replaceAll(s, "b",  "-+");
        replaceAll(s, "d",  "-+");
        replaceAll(s, "e",  "-+");
        replaceAll(s, "f",  "%@");
//        replaceAll(s, " ",cOff);
        replaceAll(s, "[",cOff+cWhite);
        replaceAll(s, "+",cOff+cYellow);
        replaceAll(s, "-",cOff+cMagenta);
        replaceAll(s, "%",cOff+bBlue);
    }
    cout << cOff << bn << s << "\n";
    cout.flush();
    return true;
}
