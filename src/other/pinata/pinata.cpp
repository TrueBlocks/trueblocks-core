#include "pinlib.h"

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

    LOG4("Starting");
    cout << "ascii: " << fileLastModifyDate(configPath("ipfs-hashes/pins.json")) << endl;
    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
    cout << "Should be found: ";
    CPinnedItem item;
    if (findChunk(argv[1], item)) {
        cout << item << endl;
    } else {
        cout << "Not found" << endl;
    }

    LOG4("unpinning");
    item = CPinnedItem();
    unpinChunk(argv[1], item);

    cout << "ascii: " << fileLastModifyDate(configPath("ipfs-hashes/pins.json")) << endl;
    cout << "binary: " << fileLastModifyDate(getCachePath("tmp/pins.bin")) << endl;
    cout << "Removed: " << item << endl;

    cout << "Should not be found: ";
    if (findChunk(argv[1], item)) {
        cout << item << endl;
    } else {
        cout << "Not found" << endl;
    }

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
