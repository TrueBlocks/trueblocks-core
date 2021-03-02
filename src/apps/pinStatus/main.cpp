#include "pinlib.h"
#include "options.h"

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    pinlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            break;

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], (options.license ? "CPinataLicense" : "CManifest"));

        if (options.license) {
            if (expContext().exportFmt == TXT1 || expContext().exportFmt == CSV1) {
                cout << trim(options.lic.Format(expContext().fmtMap["format"]), '\t') << endl;
            } else {
                indent();
                options.lic.toJson(cout);
                unindent();
            }

        } else {
            LOG_INFO("latestBlock:\t", cGreen,
                     (isTestMode() ? "--latest--" : uint_2_Str(getBlockProgress(BP_CLIENT).client)), cOff);
            LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);
            options.handle_pin();
        }

        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    pinlib_cleanup();

    return 0;
}

//----------------------------------------------------------------
void COptions::handle_pin(void) {
    LOG8("Handle pin: ", pin);

    string_q pins;
    if (!pinataListOfPins(lic, pins)) {
        usageStr(pins + ".");
        return;
    }
    cout << pins << endl;
}

//----------------------------------------------------------------
void COptions::handle_unpin(void) {
    LOG8("Handle unpin: ", unpin);

    string_q pins;
    if (!pinataListOfPins(lic, pins)) {
        usageStr(pins + ".");
        return;
    }

    CPinataPinlist pinList;
    pinList.parseJson3(pins);
    while (str_2_Uint(pinList.count) != 0) {
        cout << pinList.count << endl;
        for (auto thePin : pinList.rows) {
            cout << "Unpinning " << thePin.metadata.name << ": " << thePin.ipfs_pin_hash << " ";
            unpinOneFile(thePin.ipfs_pin_hash);
            usleep(1000000);
        }
        usleep(300000);
        pins = "";
        pins.clear();
        pinataListOfPins(lic, pins);
        pinList = CPinataPinlist();
        pinList.parseJson3(pins);
    }
}

#if 0
/*
 //----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    pinlib_init(quickQuitHandler);
    verbose = 5;

    if (argc != 2) {
        cerr << "pinata <filename>" << endl;
        return 1;
    }

    if (getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_api_key", "<notset>") == "<notset>") {
        cerr << "Pinata key not set.";
        return 1;
    }

    if (getGlobalConfig("blockScrape")->getConfigStr("settings", "pinata_secret_api_key", "<notset>") == "<notset>") {
        cerr << "Pinata secret key not set.";
        return 1;
    }

//    LOG4("Starting");
//    cout << "ascii: " << fileLastModifyDate(getInitialManifest()) << endl;
//    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
//    cout << "Should be found: ";
    CPinnedItem item;
//    if (findChunk(pinList, argv[1], item)) {
//        cout << item << endl;
//    } else {
//        cout << "Not found" << endl;
//    }
//
//    LOG4("unpinning");
//    item = CPinnedItem();
//    unpinChunk(pinList, argv[1], item);
//
//    cout << "ascii: " << fileLastModifyDate(getInitalManifest()) << endl;
//    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
//    cout << "Removed: " << item << endl;
//
//    cout << "Should not be found: ";
//    if (findChunk(pinList, argv[1], item)) {
//        cout << item << endl;
//    } else {
//        cout << "Not found" << endl;
//    }

    LOG4("pinning");
    item = CPinnedItem();
    pinChunk(pinList, argv[1], item);

    cout << "ascii: " << fileLastModifyDate(getInitalManifest()) << endl;
    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
    cout << "Pinned: " << item << endl;

    cout << "Should be found: ";
    if (findChunk(pinList, argv[1], item)) {
        cout << item << endl;
    } else {
        cout << "Not found" << endl;
    }

    LOG4("Done");
    pinlib_cleanup();
    return 1;
}
*/
#endif
