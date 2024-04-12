// Allow for running either a single test (by route) or a group (by group)
// string_q localFile = getCWD() + "addresses.tsv";
// replace(localFile, "test/gold/dev_tools/testRunner", "build");
// if (fileExists(localFile)) {
//     LOG_ERR(localFile, " found in local folder. Chifra monitor tests will fail.");
//     exit(0);
// }
// if (g etGlobalConfig("")->getConfigBool("dev", "debug_curl", false))
//     return u sage("[dev]debug_curl is set in config file. All tests will fail.");
// bool hasKey = g etGlobalConfig("")->getConfigStr("keys.etherscan", "apiKey", "<not_set>") != "<not_set>";
// bool wantsTest = getEnvStr("TEST_SLURPS") == "true";
// bool runSlurps = (hasKey && wantsTest);
// if (runSlurps) {
//     tests.push_back("tools/ethslurp");
// }
#include "options.h"

bool COptions::parseArguments(const string_q& command) {
    ::setenv("NO_USERQUERY", "true", 1);

    Init();
    tests.push_back("tools/ethNames");
    tests.push_back("tools/getBlocks");
    tests.push_back("tools/getLogs");
    tests.push_back("tools/getReceipts");
    tests.push_back("tools/getState");
    tests.push_back("tools/getTokens");
    tests.push_back("tools/getTraces");
    tests.push_back("tools/getTrans");
    tests.push_back("tools/grabABI");
    tests.push_back("tools/whenBlock");
    tests.push_back("apps/acctExport");
    tests.push_back("apps/blockScrape");
    tests.push_back("apps/cacheStatus");
    tests.push_back("apps/chunkMan");
    tests.push_back("apps/chifra");
    tests.push_back("apps/config");
    tests.push_back("apps/fireStorm");
    tests.push_back("apps/init");
    tests.push_back("apps/daemon");

    cerr << "Using `jq .` for post processing." << endl;
    doCommand("chifra blocks --uniq 0 2>/dev/null");

    cerr << "Cleaning monitor caches..." << endl;
    doCommand("chifra monitors --decache 0xf503017d7baf7fbc0fff7492b751025c6a78179b 2>/dev/null");
    doCommand("chifra monitors --decache 0x9531c059098e3d194ff87febb587ab07b30b1306 2>/dev/null");
    doCommand("chifra monitors --decache 0x5deda52dc2b3a565d77e10f0f8d4bd738401d7d3 2>/dev/null");
    doCommand("chifra monitors --decache 0xd0b3462481c33f63a288cd1923e2a261ee65b4ff 2>/dev/null");

    cerr << "Cleaning abi caches..." << endl;
    doCommand("chifra abis --decache 2>/dev/null");
    doCommand("chifra abis --decache 0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51 2>/dev/null");
    doCommand("chifra abis --decache 0xd7edd2f2bcccdb24afe9a4ab538264b0bbb31373 2>/dev/null");
    doCommand("chifra abis --decache 0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359 2>/dev/null");
    doCommand("chifra abis --decache 0x226159d592e2b063810a10ebf6dcbada94ed68b8 2>/dev/null");
    doCommand("chifra abis --decache 0x17996cbddd23c2a912de8477c37d43a1b79770b8 2>/dev/null");
    doCommand("chifra abis --decache 0x0000000000004946c0e9f43f4dee607b0ef1fa1c 2>/dev/null");
    doCommand("chifra abis --decache 0x7c66550c9c730b6fdd4c03bc2e73c5462c5f7acc 2>/dev/null");
    doCommand("chifra abis --decache 0xa478c2975ab1ea89e8196811f51a7b7ade33eb11 2>/dev/null");
    doCommand("chifra abis --decache 0x7d655c57f71464b6f83811c55d84009cd9f5221c 2>/dev/null");
    doCommand("chifra abis --decache 0x0000000000004946c0e9f43f4dee607b0ef1fa1c 2>/dev/null");
    doCommand("chifra abis --decache 0x30f938fed5de6e06a9a7cd2ac3517131c317b1e7 2>/dev/null");
    doCommand("chifra abis --decache 0xb9da44c051c6cc9e04b7e0f95e95d69c6a6d8031 2>/dev/null");
    doCommand("chifra abis --decache 0x6d903f6003cca6255d85cca4d3b5e5146dc33925 2>/dev/null");
    doCommand("chifra abis --decache 0x9ba00d6856a4edf4665bca2c2309936572473b7e 2>/dev/null");
    doCommand("chifra abis --decache 0x1a9c8182c09f50c8318d769245bea52c32be35bc 2>/dev/null");
    doCommand("chifra abis --decache 0x729d19f657bd0614b4985cf1d82531c67569197b 2>/dev/null");
    doCommand("chifra abis --decache 0x81f7564e413586f1f99fde55740ac52b43ca99c9 2>/dev/null");
    doCommand("chifra abis --decache 0x8d12a197cb00d4747a1fe03395095ce2a5cc6819 2>/dev/null");
    doCommand("chifra abis --decache 0xdbd27635a534a3d3169ef0498beb56fb9c937489 2>/dev/null");

    cerr << "Downloading abi files..." << endl;
    doCommand("chifra abis 0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51");
    doCommand("chifra abis 0xd7edd2f2bcccdb24afe9a4ab538264b0bbb31373");
    doCommand("chifra abis 0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359");
    doCommand("chifra abis 0x226159d592e2b063810a10ebf6dcbada94ed68b8");
    doCommand("chifra abis 0x17996cbddd23c2a912de8477c37d43a1b79770b8");
    doCommand("chifra abis 0x0000000000004946c0e9f43f4dee607b0ef1fa1c");
    doCommand("chifra abis 0x7c66550c9c730b6fdd4c03bc2e73c5462c5f7acc");
    doCommand("chifra abis 0xa478c2975ab1ea89e8196811f51a7b7ade33eb11");
    doCommand("chifra abis 0x7d655c57f71464b6f83811c55d84009cd9f5221c");
    doCommand("chifra abis 0x0000000000004946c0e9f43f4dee607b0ef1fa1c");
    doCommand("chifra abis 0x30f938fed5de6e06a9a7cd2ac3517131c317b1e7");
    doCommand("chifra abis 0xb9da44c051c6cc9e04b7e0f95e95d69c6a6d8031");
    doCommand("chifra abis 0x6d903f6003cca6255d85cca4d3b5e5146dc33925");
    doCommand("chifra abis 0x9ba00d6856a4edf4665bca2c2309936572473b7e");
    doCommand("chifra abis 0x1a9c8182c09f50c8318d769245bea52c32be35bc");
    doCommand("chifra abis 0x729d19f657bd0614b4985cf1d82531c67569197b");
    doCommand("chifra abis 0x81f7564e413586f1f99fde55740ac52b43ca99c9");
    doCommand("chifra abis 0x8d12a197cb00d4747a1fe03395095ce2a5cc6819");
    doCommand("chifra abis 0xdbd27635a534a3d3169ef0498beb56fb9c937489");

    return true;
}

void COptions::Init(void) {
}

COptions::COptions(void) {
    Init();
}

COptions::~COptions(void) {
}

const CToml* COptions::getGlobalConfig(const string_q& mergeIn) {
    static CToml* toml = NULL;
    static string_q components = "trueBlocks|";

    if (!toml) {
        string_q configFile = configPath + "trueBlocks.toml";
        static CToml theToml(configFile);
        toml = &theToml;
        string_q name = progName;
        string_q fileName = chainConfigPath + name + ".toml";
        if (name == "makeClass" || name == "testRunner")
            fileName = configPath + name + ".toml";
        if (fileExists(fileName) && !contains(components, name + "|")) {
            components += name + "|";
            CToml custom(fileName);
            toml->mergeFile(&custom);
        }
    }

    // If we're told explicitly to load another config, do that as well
    if (!mergeIn.empty()) {
        string_q name = mergeIn;
        string_q fileName = chainConfigPath + name + ".toml";
        if (name == "makeClass" || name == "testRunner")
            fileName = configPath + name + ".toml";
        if (fileExists(fileName) && !contains(components, name + "|")) {
            components += name + "|";
            CToml custom(fileName);
            toml->mergeFile(&custom);
        }
    }

    return toml;
}

inline bool waitForCreate(const string_q& filename) {
    size_t mx = 1000;
    size_t cnt = 0;
    while (cnt < mx && !fileExists(filename))
        cnt++;

    return fileExists(filename);
}

template <class T>
T RandomValue(T a, T b) {
    T range = (a > b ? a - b : b - a);
    if (range == 0)
        return a;
    return min(a, b) + (((T)rand()) % range);
}

string_q int_2_Strxx(int64_t i) {
    ostringstream os;
    os << i;
    return os.str();
}
string_q doCommand(const string_q& cmd, bool readStderr) {
    string_q tmpPath = "/tmp/";
    string_q filename = tmpPath + makeValidName("qb_" + int_2_Strxx(RandomValue(1, 10000)));
    string_q theCommand = (cmd + " >" + filename);
    if (readStderr) {
        theCommand = (cmd + " >/dev/null 2>" + filename);
    }
    if (system(theCommand.c_str())) {
    }  // Don't remove cruft. Silences compiler warnings

    // Check twice for existence since the previous command creates the file but may take some time
    waitForCreate(filename);
    string_q ret;
    asciiFileToString(filename, ret);
    ::remove(filename.c_str());
    return trim(ret, '\n');
}
