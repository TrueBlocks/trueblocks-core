#include "pinlib.h"

extern bool addPinToArray(CPinnedItem& item, void *data);
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    pinlib_init(quickQuitHandler);
    verbose = 5;

    CPinReport report;
    report.publishTs = date_2_Ts(Now());
    report.fileName = "ipfs-hashes/pin-report.json";
    report.indexFormat = "Qmart6XP9XjL43p72PGR93QKytbK8jWWcMguhFgxATTya2";
    report.bloomFormat = "QmNhPk39DUFoEdhUmtGARqiFECUHeghyeryxZM9kyRxzHD";
    report.newBlockRange = "000000000-010156360";
    forEveryPin(addPinToArray, &report.newPins);
    report.prevHash = "QmbFMke1KXqnYyBBWxB74N4c5SBnJMVAiMNRcGu6x1AwQH";
    report.prevBlockRange = "";
    report.prevPins.clear();
    report.doExport(cout);
    pinlib_cleanup();

    return 1;
}

//----------------------------------------------------------------
bool addPinToArray(CPinnedItem& pin, void *data) {
    CPinnedItemArray *array = (CPinnedItemArray*)data;
    array->push_back(pin);
    return true;
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

