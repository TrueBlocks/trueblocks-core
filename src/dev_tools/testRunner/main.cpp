#include "utillib.h"
#include "main.h"

//-----------------------------------------------------------------------------
void COptions::doTests(vector<CTestCase>& testArray, const string_q& route, bool isCmd) {
    uint64_t nTests = 0;
    uint64_t nPassed = 0;
    for (auto test : testArray) {
        test.prepareTest(isCmd);

        bool shouldRun1 = isCmd && test.mode != "api";
        bool shouldRun2 = !isCmd && test.mode != "cmd";
        if (shouldRun1 || shouldRun2) {
            string_q testRoot = substitute(test.goldPath, "/api_tests", "");

            ostringstream theCmd;
            theCmd << "cd \"" + testRoot + "\" ; ";

            ostringstream prepender;
            for (auto e : test.envLines) {
                prepender << "Env: " << e << endl;
            }

            string_q goldFn = test.goldPath + test.fileName;
            string_q workFn = test.workPath + test.fileName;
            if (isCmd) {
                string_q envs = substitute(substitute(linesToString(test.envLines, '|'), " ", ""), "|", " ");
                string_q env = "env " + envs + " TEST_MODE=true NO_COLOR=true ";
                string_q exe = "chifra" + (contains(test.tool, "chifra") ? "" : " " + test.route);
                string_q fullCmd = exe + " " + test.options;
                string_q debugCmd = substitute(fullCmd, getHomeFolder(), "$HOME/");
                string_q redir = workFn;
                theCmd << "echo \"" << debugCmd << "\" >" << redir + " && ";
                string_q rFile = substitute(test.goldPath, "/api_tests", "") + test.name + ".redir";
                if (fileExists(rFile)) {
                    fullCmd += " --output " + test.name + "_out.file";
                    test.origLine += " & output = " + test.name + "_out.file";
                }
                theCmd << env << fullCmd << " >>" << redir << " 2>&1";

            } else {
                bool has_options = !test.options.empty();
                bool has_post = !test.post.empty();
                bool has_env = (test.envLines.size() > 0);

                // Check if curl configuration file for the given test exists. We can use such
                // files to send requests other than GET, e.g. when testing CRUD endpoints
                CStringArray curlFileLines;
                string_q curlFilePath = substitute(test.goldPath, "/api_tests", "") + test.name + "_curl.txt";
                if (fileExists(curlFilePath))
                    asciiFileToLines(curlFilePath, curlFileLines);

                theCmd << "curl -s ";
                if (curlFileLines.size() > 0) {
                    theCmd << "--config " << curlFilePath << " ";
                    for (auto curlArgument : curlFileLines) {
                        prepender << "CURL: " << curlArgument << endl;
                    }
                }
                theCmd << "-H \"User-Agent: testRunner\" ";
                if (has_env)
                    theCmd << "-H \"X-TestRunner-Env: " << substitute(linesToString(test.envLines, '|'), " ", "")
                           << "\" ";
                theCmd << "\"";
                string_q port = getEnvStr("TB_TEST_API_SERVER");
                string_q apiProvider = "http://localhost:" + (port.empty() ? "8080" : port) + "/";
                theCmd << apiProvider;
                theCmd << test.route;
                theCmd << (has_options ? ("?" + test.options) : "");
                theCmd << "\"";
                theCmd << (has_post ? (" | " + test.post + " ") : "");
                theCmd << " >" << workFn;
                prepender << test.route << "?" << test.options << endl;
            }

            nTests++;
            if (system(theCmd.str().c_str())) {
            }

            string_q workContents = asciiFileToString(workFn);
            if (!prepender.str().empty()) {
                workContents = prepender.str() + workContents;
            }
            replaceAll(workContents, "3735928559", "\"0xdeadbeef\"");

            if (isCmd && contains(test.origLine, "output")) {
                if (fileExists(test.outputFile())) {
                    ostringstream os;
                    os << "----" << endl;
                    os << "Results in " << substitute(test.outputFile(), testRoot, "./") << endl;
                    os << asciiFileToString(test.outputFile()) << endl;
                    workContents += os.str();
                }
            }
            stringToAsciiFile(workFn, workContents);

            string_q goldText = asciiFileToString(goldFn);
            string_q workText = asciiFileToString(workFn);
            bool passes = !goldText.empty() && goldText == workText;
            if (passes) {
                nPassed++;
            } else {
                fails.push_back(test);
            }

            // reportOneTest();
            ostringstream os;
            os << "  " << (passes ? (cGreen + "ok    ") : (cRed + "failed")) << cOff << " ";
            os << padRight(route, 15, true, '.') << padRight(test.name, 30, true, '.') << " ";
            os << test.options;
            os << cOff;
            cerr << padRight(os.str(), 135, false, ' ') << "      ";
            cerr << (passes ? "\r" : "\n");
            cerr.flush();
        }
    }

    totalTests += nTests;
    totalPassed += nPassed;
    if (nTests) {
        uint64_t nFailed = nTests - nPassed;
        ostringstream os;
        os << (nFailed ? cRed : cYellow);
        os << padRight(route + " (" + (isCmd ? "cmd" : "api") + " mode)", 25, false, ' ') + " ==> ";
        os << (!nFailed ? "ok    " : "failed");
        os << ": " << nPassed;
        os << " of " << nTests;
        os << " passed (" << nFailed << " failed)";
        os << cOff;
        cerr << padRight(os.str(), 135, false, ' ') << "      ";
        cerr << endl;
    }

    return;
}

//-----------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    COptions options;
    options.init();

    for (auto route : options.tests) {
        string_q path = nextTokenClear(route, '/');
        rmWorkingTests(path, route);
        rmWorkingTests(path, route + "/api_tests");

        string_q testFile = options.sourceFolder + path + "/" + route + ".csv";

        CStringArray testLines;
        asciiFileToLines(testFile, testLines);

        vector<CTestCase> testArray;
        for (auto line : testLines) {
            if (!startsWith(line, "#") && !startsWith(line, "enabled") && !line.empty()) {
                CTestCase test(line);
                if (!startsWith(line, "on")) {
                    test.copyBack();
                } else {
                    testArray.push_back(test);
                }
            }
        }

        options.doTests(testArray, route, false /* isCmd */);
        options.doTests(testArray, route, true /* isCmd */);
    }

    uint64_t nFailed = options.totalTests - options.totalPassed;

    cerr << string_q(125, '=') << endl;
    cerr << "  nTests:  " << options.totalTests << endl;
    cerr << "  nPassed: " << options.totalPassed << (nFailed ? " ==> not okay" : " ==> ok") << endl;
    cerr << "  nFailed: " << nFailed << endl;
    for (auto fail : options.fails) {
        cerr << "\tFailed: ";
        cerr << fail.route << " " << fail.name << ".txt ";
        cerr << "(" << fail.route << " " << trim(fail.options) << ")";
        cerr << endl;
    }
    cerr << endl;

    bool allPassed = options.totalTests == options.totalPassed;
    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}

//-----------------------------------------------------------------------------
void COptions::init(void) {
    ::setenv("NO_USERQUERY", "true", 1);
    cleanFolder(getCachePath() + "tmp/");
    cerr << "Using `jq .` for post processing." << endl;

    if (getEnvStr("TEST_SLURPS") == "true") {
        tests.push_back("tools/ethslurp");
    }
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

    cerr << "Cleaning monitor caches..." << endl;
    doCommand("chifra monitors --decache 0xf503017d7baf7fbc0fff7492b751025c6a78179b 2>/dev/null");
    doCommand("chifra monitors --decache 0x9531c059098e3d194ff87febb587ab07b30b1306 2>/dev/null");
    doCommand("chifra monitors --decache 0x5deda52dc2b3a565d77e10f0f8d4bd738401d7d3 2>/dev/null");
    doCommand("chifra monitors --decache 0xd0b3462481c33f63a288cd1923e2a261ee65b4ff 2>/dev/null");

    cerr << "Cleaning abi caches..." << endl;
    CStringArray addrs = {
        "0x45f783cce6b7ff23b2ab2d70e416cdb7d6055f51", "0xd7edd2f2bcccdb24afe9a4ab538264b0bbb31373",
        "0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359", "0x226159d592e2b063810a10ebf6dcbada94ed68b8",
        "0x17996cbddd23c2a912de8477c37d43a1b79770b8", "0x0000000000004946c0e9f43f4dee607b0ef1fa1c",
        "0x7c66550c9c730b6fdd4c03bc2e73c5462c5f7acc", "0xa478c2975ab1ea89e8196811f51a7b7ade33eb11",
        "0x7d655c57f71464b6f83811c55d84009cd9f5221c", "0x0000000000004946c0e9f43f4dee607b0ef1fa1c",
        "0x30f938fed5de6e06a9a7cd2ac3517131c317b1e7", "0xb9da44c051c6cc9e04b7e0f95e95d69c6a6d8031",
        "0x6d903f6003cca6255d85cca4d3b5e5146dc33925", "0x9ba00d6856a4edf4665bca2c2309936572473b7e",
        "0x1a9c8182c09f50c8318d769245bea52c32be35bc", "0x729d19f657bd0614b4985cf1d82531c67569197b",
        "0x81f7564e413586f1f99fde55740ac52b43ca99c9", "0x8d12a197cb00d4747a1fe03395095ce2a5cc6819",
        "0xdbd27635a534a3d3169ef0498beb56fb9c937489",
    };
    for (auto addr : addrs) {
        doCommand("chifra abis --decache " + addr + " 2>/dev/null");
        doCommand("chifra abis " + addr);
    }
    doCommand("chifra abis --decache 2>/dev/null");

    sourceFolder = getCWD() + string_q("../../../../src/dev_tools/testRunner/testCases/");
}

//-----------------------------------------------------------------------------
string_q CTestCase::outputFile() const {
    string_q testRoot = substitute(goldPath, "/api_tests", "");
    string_q line = substitute(substitute(origLine, "&", "|"), "=", "|");
    CStringArray parts;
    explode(parts, line, '|');
    bool next = false;
    string_q ret;
    for (auto part : parts) {
        part = trim(part);
        if (next && ret.empty()) {
            ret = testRoot + part;
        }
        if (part == "output") {
            next = true;
        }
    }
    return ret;
}

//-----------------------------------------------------------------------------
bool rmWorkingTests(const string_q& path, const string_q& route) {
    ostringstream os;
    os << "find ../../../working/" << path << "/" << route << "/";
    os << " -name \"" << route << "_*.txt\"";
    os << " -exec rm '{}' ';' 2>/dev/null ; ";
    if (system(os.str().c_str())) {
    }
    return true;
}

//-----------------------------------------------------------------------------
string_q linesToString(const CStringArray& lines, char sep) {
    ostringstream os;
    for (auto line : lines)
        os << line << (sep != 0 ? string_q(1, sep) : "");
    return os.str();
}

//-----------------------------------------------------------------------------
string_q getCachePath(void) {
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    string_q configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    string_q configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#endif
    return configPath + "cache/mainnet/";
}

//-----------------------------------------------------------------------------
string_q padRight(const string_q& str, size_t len, bool bumpPad, char p) {
    string_q result;
    if (len > str.length()) {
        result = str + std::string(len - str.length(), p);
    } else {
        result = str.substr(0, len);
    }
    if (bumpPad && len >= 3) {
        result.replace(len - 3, 3, 3, p);
    }
    return result;
}

//-----------------------------------------------------------------------------
string_q padLeft(const string_q& str, size_t len, char p) {
    if (len > str.length())
        return string_q(len - str.length(), p) + str;
    return str;
}

//-----------------------------------------------------------------------------
string_q getEnvStr(const string_q& name) {
    char* sss = getenv(name.c_str());
    return (sss ? string_q(sss) : string_q(""));
}

//-----------------------------------------------------------------------------
static string_q escapePath(const string_q& nameIn) {
    string_q name = nameIn;
    replaceAll(name, "&", "\\&");
    replaceAll(name, "(", "\\(");
    replaceAll(name, ")", "\\)");
    replaceAll(name, "'", "\\'");
    return name;
}

//-----------------------------------------------------------------------------
int copyFile(const string_q& fromIn, const string_q& toIn) {
    ifstream src(escapePath(fromIn), ios::binary);
    ofstream dst(escapePath(toIn), ios::binary);
    dst << src.rdbuf();
    return static_cast<int>(fileExists(toIn));
}

//-----------------------------------------------------------------------------
namespace qblocks {
typedef bool (*CONSTAPPLYFUNC)(const string_q& path, void* data);
extern bool forAllFiles(const string_q& mask, CONSTAPPLYFUNC func, void* data);
}  // namespace qblocks

//-----------------------------------------------------------------------------
namespace filename_local {
class CFileListState {
  public:
    string_q top;
    CStringArray& list;
    bool recurse;
    CFileListState(const string_q& t, CStringArray& l, bool r) : top(t), list(l), recurse(r) {
    }
};

//-----------------------------------------------------------------------------
bool visitFile(const string_q& path, void* data) {
    CFileListState* state = reinterpret_cast<CFileListState*>(data);
    if (endsWith(path, '/')) {
        if (path == state->top || state->recurse) {
            return forAllFiles(path + "*", visitFile, data);
        } else {
            state->list.push_back(path);
        }
        return true;
    }
    state->list.push_back(path);
    return true;
}
};  // namespace filename_local

//-----------------------------------------------------------------------------
size_t listFilesInFolder(CStringArray& items, const string_q& folder, bool recurse) {
    filename_local::CFileListState state(folder, items, recurse);
    forAllFiles(folder, filename_local::visitFile, &state);
    return items.size();
}

//-----------------------------------------------------------------------------
int cleanFolder(const string_q& path, bool recurse, bool interactive) {
    CStringArray files;
    listFilesInFolder(files, path, true);
    for (auto file : files)
        ::remove(file.c_str());
    return static_cast<int>(files.size());
}

//-----------------------------------------------------------------------------
inline bool waitForCreate(const string_q& filename) {
    size_t mx = 1000;
    size_t cnt = 0;
    while (cnt < mx && !fileExists(filename))
        cnt++;

    return fileExists(filename);
}

//-----------------------------------------------------------------------------
template <class T>
T RandomValue(T a, T b) {
    T range = (a > b ? a - b : b - a);
    if (range == 0)
        return a;
    return min(a, b) + (((T)rand()) % range);
}

//-----------------------------------------------------------------------------
string_q int_2_Strxx(int64_t i) {
    ostringstream os;
    os << i;
    return os.str();
}

//-----------------------------------------------------------------------------
string_q doCommand(const string_q& cmd, bool readStderr) {
    string_q tmpPath = "/tmp/";
    string_q filename = tmpPath + makeValidName("qb_" + int_2_Strxx(RandomValue(1, 10000)));
    string_q theCommand = (cmd + " >" + filename);
    if (readStderr) {
        theCommand = (cmd + " >/dev/null 2>" + filename);
    }
    if (system(theCommand.c_str())) {
    }  // Don't remove cruft. Silences compiler warnings
    waitForCreate(filename);
    string_q ret = asciiFileToString(filename);
    ::remove(filename.c_str());
    return trim(ret, '\n');
}

//-----------------------------------------------------------------------------
static const char* CHR_VALID_NAME =
    "\t\n\r()<>[]{}`\\|; "
    "'!$^*~@"
    "?&#+%"
    ",:/=\"";

//-----------------------------------------------------------------------------
string_q makeValidName(const string_q& inOut) {
    string_q ret = inOut;
    replaceAny(ret, CHR_VALID_NAME, "_");
    if (!ret.empty() && isdigit(ret[0]))
        ret = "_" + ret;
    return ret;
}

// Test that all lines in all tests files has all fields (options may be empty)
// Allow for running either a single test (by route) or a group (by group)
// string_q localFile = getCWD() + "addresses.tsv";
// replace(localFile, "test/gold/dev_tools/testRunner", "build");
// if (fileExists(localFile)) {
//     LOG_ERR(localFile, " found in local folder. Chifra monitor tests will fail.");
//     exit(0);
// }
// if (g etGlobalConfig("")->getC onfigBool("dev", "debug_curl", false))
//     return u sage("[dev]debug_curl is set in config file. All tests will fail.");
// bool hasKey = g etGlobalConfig("")->getConfigStr("keys.etherscan", "apiKey", "<not_set>") != "<not_set>";
// bool wantsTest = g etEnvStr("TEST_SLURPS") == "true";
// bool runSlurps = (hasKey && wantsTest);
// if (runSlurps) {
//     tests.push_back("tools/ethslurp");
// }

// If a test case is not commented out (with #) copy its gold file to working (since
// we've cleaned the working otherwise)
// If configured, copy the data out to the folder our performance measurement tool knows about
// Handle _curl files (X-POST, etc.)
// Clean existing .txt files in both root and api_files folders prior to the test
// Append --output file results to the output file
// Prepend test data to the output file
// Write performance data to a file and results to the screen
// Support --mode both|cmd|api
// Make sure both gold and working folders exist
// Must read .env files if present and put them in the environment
// Check for duplicate tests names within a given folder
