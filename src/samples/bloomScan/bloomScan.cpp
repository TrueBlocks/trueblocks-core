/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern bool visitBloom(const SFString& path, void *data);
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
bool visitBloom(const SFString& path, void *data) {

    if (path.endsWith("/")) {
        forAllFiles(path + "*", visitBloom, data);

    } else {
extern bool displayBloom(blknum_t bn, const SFBloom& bloom, void *data);
        if (path.endsWith(".bin")) {
            SFBloom bloom;
            SFArchive archive(READING_ARCHIVE);
            if (archive.Lock(path, binaryReadOnly, LOCK_NOWAIT)) {
                SFBloomArray blooms;
                archive >> blooms;
                archive.Release();
                for (uint32_t i = 0 ; i < blooms.getCount() ; i++) {
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
    SFString s = bloom2Bytes(bloom);
    COptions *opt = (COptions*)data;
    if (opt->mode == "short") {
        size_t len = s.length();
        s.Replace   ("0x",     "");
        s.ReplaceAny("0",      "");
        s.ReplaceAny("1248",   ".");
        s.ReplaceAny("3569ac", ".");
        s.ReplaceAny("7bde",   "+");
        s.ReplaceAny("f",      "@");
        size_t rem = len - s.length();
        s = s + SFString(' ', rem) + "|";
    } else {
        s.Replace("0x", cOff);
        s.ReplaceAll("0",  " ");
        s.ReplaceAll("1",  "[.");
        s.ReplaceAll("2",  "[.");
        s.ReplaceAll("4",  "[.");
        s.ReplaceAll("8",  "[.");
        s.ReplaceAll("3",  "+.");
        s.ReplaceAll("5",  "+.");
        s.ReplaceAll("6",  "+.");
        s.ReplaceAll("9",  "+.");
        s.ReplaceAll("a",  "+.");
        s.ReplaceAll("c",  "+.");
        s.ReplaceAll("7",  "-+");
        s.ReplaceAll("b",  "-+");
        s.ReplaceAll("d",  "-+");
        s.ReplaceAll("e",  "-+");
        s.ReplaceAll("f",  "%@");
//        s.ReplaceAll(" ",cOff);
        s.ReplaceAll("[",cOff+cWhite);
        s.ReplaceAll("+",cOff+cYellow);
        s.ReplaceAll("-",cOff+cMagenta);
        s.ReplaceAll("%",cOff+bBlue);
    }
    cout << cOff << bn << s << "\n";
    cout.flush();
    return true;
}
