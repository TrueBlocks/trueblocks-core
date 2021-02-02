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
            cout << exportPreamble(expContext().fmtMap["header"], (options.license ? "CPinataLicense" : "CPinReport"));

        if (options.license) {
            if (expContext().exportFmt == TXT1 || expContext().exportFmt == CSV1) {
                cout << trim(options.lic.Format(expContext().fmtMap["format"]), '\t') << endl;
            } else {
                indent();
                options.lic.toJson(cout);
                unindent();
            }

        } else {
            options.handle_status();
        }

        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    pinlib_cleanup();

    return 0;
}

void COptions::handle_status(void) {
    string_q provider = getGlobalConfig()->getConfigStr("settings", "rpcProvider", "");
    LOG_INFO("rpcProvider:\t", cGreen, provider, cOff);
    LOG_INFO("latestBlock:\t", cGreen, getLatestBlock_client(), cOff);
    LOG_INFO("unchainedIndexAddr:\t", cGreen, unchainedIndexAddr, cOff);

    // forEveryPin(removeFromPinata, NULL);
    string_q pins;
    pinataListOfPins(pins);
    CPinataList pinList;
    pinList.parseJson3(pins);
    while (str_2_Uint(pinList.count) != 0) {
        cout << pinList.count << endl;
        for (auto pin : pinList.rows) {
            cout << "Unpinning " << pin.metadata.name << ": " << pin.ipfs_pin_hash << " ";
            unpinChunkByHash(pin.ipfs_pin_hash);
        }
        usleep(300000);
        pins = "";
        pins.clear();
        pinataListOfPins(pins);
        pinList = CPinataList();
        pinList.parseJson3(pins);
    }
    //        pinataListOfPins(pins);
    //    }
    // freshenBloomFilters(true);
    //    //publishManifest(cout); //, prevHash);
    //    cout << getFileContentsByHash("QmcvjroTiE95LWeiP8HHq1YA3ysRchLuVx8HLQui8WcSBV")<< endl;
}
#if 0
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
//    cout << "ascii: " << fileLastModifyDate(configPath("ipfs-hashes/pins.json")) << endl;
//    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
//    cout << "Should be found: ";
    CPinnedItem item;
//    if (findChunk(argv[1], item)) {
//        cout << item << endl;
//    } else {
//        cout << "Not found" << endl;
//    }
//
//    LOG4("unpinning");
//    item = CPinnedItem();
//    unpinChunk(argv[1], item);
//
//    cout << "ascii: " << fileLastModifyDate(configPath("ipfs-hashes/pins.json")) << endl;
//    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
//    cout << "Removed: " << item << endl;
//
//    cout << "Should not be found: ";
//    if (findChunk(argv[1], item)) {
//        cout << item << endl;
//    } else {
//        cout << "Not found" << endl;
//    }

    LOG4("pinning");
    item = CPinnedItem();
    pinChunk(argv[1], item);

    cout << "ascii: " << fileLastModifyDate(configPath("ipfs-hashes/pins.json")) << endl;
    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
    cout << "Pinned: " << item << endl;

    cout << "Should be found: ";
    if (findChunk(argv[1], item)) {
        cout << item << endl;
    } else {
        cout << "Not found" << endl;
    }

    LOG4("Done");
    pinlib_cleanup();
    return 1;
}
#endif
