#include "utillib.h"
#include "main.h"

//-----------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    COptions options;
    options.init();

    for (auto doRemove : options.locations) {
        string_q path = nextTokenClear(doRemove, '/');
        string_q cppName = nextTokenClear(doRemove, '/');
        string route = nextTokenClear(doRemove, '/');
        if (doRemove == "true") {
            rmWorkingTests(path, cppName);
            rmWorkingTests(path, cppName + "/api_tests");
        }

        CStringArray testLines;
        asciiFileToLines(options.sourceFolder + route + ".csv", testLines);

        vector<CTestCase> testArray;
        for (auto line : testLines) {
            if (!startsWith(line, "#") && !startsWith(line, "enabled") && !line.empty()) {
                CTestCase test(line);
                if (startsWith(line, "on")) {
                    testArray.push_back(test);
                } else {
                    test.copyBack();
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
    cerr << endl;

    bool allPassed = options.totalTests == options.totalPassed;
    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}

//-----------------------------------------------------------------------------
void COptions::doTests(vector<CTestCase>& testArray, const string_q& route, bool isCmd) {
    uint64_t nTests = 0;
    uint64_t nPassed = 0;
    for (auto test : testArray) {
        if (!test.shouldRun(isCmd)) {
            continue;
        }

        test.prepareTest(isCmd);
        string_q goldFn = test.goldPath + test.fileName;
        string_q workFn = test.workPath + test.fileName;
        string_q curlFilePath = test.testRoot + test.name + "_curl.txt";
        string_q redirFile = test.testRoot + test.name + ".redir";
        string_q envs = substitute(substitute(linesToString(test.envLines, '|'), " ", ""), "|", " ");

        ostringstream theCmd;
        theCmd << "cd \"" + test.testRoot + "\" ; ";
        if (isCmd) {
            if (fileExists(redirFile)) {
                test.origLine += (fileExists(redirFile) ? " & output = " + test.name + "_out.file" : "");
            }

            ostringstream os;
            os << "chifra" << (contains(test.tool, "chifra") ? "" : " " + test.route) << " " << test.options;
            if (fileExists(redirFile)) {
                os << (fileExists(redirFile) ? " --output " + test.name + "_out.file" : "");
            }

            theCmd << "echo \"" << substitute(os.str(), getHomeFolder(), "$HOME/") << "\" >" << workFn + " && ";
            theCmd << "env " << envs << " TEST_MODE=true NO_COLOR=true ";
            theCmd << os.str();
            theCmd << " >>" << workFn << " 2>&1";

        } else {
            theCmd << "curl -s";
            theCmd << (fileSize(curlFilePath) > 0 ? (" --config " + curlFilePath) : "");
            theCmd << " -H \"User-Agent: testRunner\"";
            theCmd << " \"" << test.apiUrl() << "\"";
            theCmd << (test.post.empty() ? "" : (" | " + test.post + " "));
            theCmd << " >" << workFn;
        }

        if (system(theCmd.str().c_str())) {
        }

        test.cleanWorkFile(isCmd, workFn);

        nTests++;
        string_q goldText = asciiFileToString(goldFn);
        string_q workText = asciiFileToString(workFn);
        nPassed += goldText == workText;

        // reportOneTest();
        ostringstream os;
        os << "    " << (goldText == workText ? (cGreen + "ok    ") : (cRed + "failed")) << cOff << " ";
        os << padRight(test.route, 15, true, '.') << padRight(test.name, 30, true, '.') << " ";
        os << test.options;
        os << cOff;
        cerr << padRight(os.str(), 135, false, ' ') << "      ";
        cerr << ((goldText == workText && !isRemoteTesting) ? "\r" : "\n");
        cerr.flush();
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
        os << " passed, " << nFailed << " failed.";
        os << cOff;
        cerr << padRight(os.str(), 135, false, ' ') << "      ";
        cerr << endl;
    }

    return;
}

//-----------------------------------------------------------------------------
void COptions::init(void) {
    ::setenv("NO_USERQUERY", "true", 1);
    isRemoteTesting = getEnvStr("TB_REMOTE_TESTING") == "true";
    cleanFolder(getCachePath() + "tmp/");
    sourceFolder = getCWD() + string_q("../../../../src/dev_tools/testRunner/testCases/");

    if (getEnvStr("TEST_SLURPS") == "true") {
        locations.push_back("tools/ethslurp/slurp");
    }
    locations.push_back("tools/ethNames/names/true");
    locations.push_back("tools/getBlocks/blocks/true");
    locations.push_back("tools/getLogs/logs/true");
    locations.push_back("tools/getReceipts/receipts/true");
    locations.push_back("tools/getState/state/true");
    locations.push_back("tools/getTokens/tokens/true");
    locations.push_back("tools/getTraces/traces/true");
    locations.push_back("tools/getTrans/transactions/true");
    locations.push_back("tools/grabABI/abis/true");
    locations.push_back("tools/whenBlock/when/true");
    locations.push_back("apps/acctExport/list/true");
    locations.push_back("apps/acctExport/monitors/false");
    locations.push_back("apps/acctExport/export/false");
    locations.push_back("apps/blockScrape/scrape/true");
    locations.push_back("apps/cacheStatus/status/true");
    locations.push_back("apps/chunkMan/chunks/true");
    locations.push_back("apps/chifra/chifra/true");
    locations.push_back("apps/config/config/true");
    locations.push_back("apps/fireStorm/explore/true");
    locations.push_back("apps/init/init/true");
    locations.push_back("apps/daemon/daemon/true");
}

//-----------------------------------------------------------------------------
bool rmWorkingTests(const string_q& path, const string_q& cppName) {
    ostringstream os;
    os << "find ../../../working/" << path << "/" << cppName << "/";
    os << " -name \"" << cppName << "_*.txt\"";
    os << " -maxdepth 1";
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
bool waitForCreate(const string_q& filename) {
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

//-----------------------------------------------------------------------------
CTestCase::CTestCase(const string_q& line) {
    origLine = line;

    CStringArray parts;
    explode(parts, line, ',');
    onOff = parts.size() > 0 ? trim(parts[0]) : "";
    mode = parts.size() > 1 ? trim(parts[1]) : "";
    speed = parts.size() > 2 ? trim(parts[2]) : "";
    route = parts.size() > 3 ? trim(parts[3]) : "";
    tool = parts.size() > 4 ? trim(parts[4]) : "";
    name = parts.size() > 5 ? trim(parts[5]) : "";
    post = parts.size() > 6 ? (trim(parts[6]) == "y" ? "jq ." : "") : "";
    options = parts.size() > 7 ? trim(parts[7]) : "";

    CStringArray chars = {"=", "&", "@"};
    for (auto ch : chars) {
        replaceAll(options, " " + ch, ch);
        replaceAll(options, ch + " ", ch);
    }
    replaceAll(options, "&#44;", ",");

    path = nextTokenClear(tool, '/');
    fileName = tool + "_" + name + ".txt";

    string_q tr = "/test/gold/dev_tools/testRunner/";
    goldPath = substitute(getCWD(), tr, "/test/gold/" + path + "/" + tool + "/");
    workPath = substitute(getCWD(), tr, "/test/working/" + path + "/" + tool + "/");
    testRoot = substitute(goldPath, "/api_tests", "");
}

//-----------------------------------------------------------------------------
void CTestCase::prepareTest(bool isCmd) {
    if (isCmd) {
        CStringArray opts = {"val",   "addrs",     "blocks", "files", "dates",  "transactions",
                             "terms", "functions", "modes",  "mode",  "topics", "fourbytes"};
        options = "&" + options;
        for (auto opt : opts)
            replaceAll(options, "&" + opt + "=", " ");
        replaceAll(options, "%20", " ");
        replaceAll(options, "@", " -");
        replaceAll(options, "&", " --");
        replaceAll(options, "=", " ");
        if (trim(options) == "--" || startsWith(trim(options), "-- "))
            replace(options, "--", "");

    } else {
        if (tool == "chifra")
            nextTokenClear(options, '&');
        CStringArray parts;
        explode(parts, options, '&');
        ostringstream os;
        for (auto part : parts) {
            string_q key = nextTokenClear(part, '=');
            if (!os.str().empty())
                os << "&";
            os << toCamelCase(key) << (part.empty() ? "" : "=" + part);
        }
        options = os.str();
        replaceAll(options, "@", "");
        replaceAll(options, " ", "%20");
        goldPath += "api_tests/";
        workPath += "api_tests/";
    }

    string_q envFile = testRoot + name + ".env";
    if (fileExists(envFile)) {
        CStringArray fileLines;
        asciiFileToLines(envFile, fileLines);
        for (auto f : fileLines) {
            if (!startsWith(f, "#")) {
                envLines.push_back(f);
            }
        }
    }
}

//-----------------------------------------------------------------------------
string_q CTestCase::outputFileContents(void) {
    if (contains(origLine, "output")) {
        string_q line = substitute(substitute(origLine, "&", "|"), "=", "|");
        CStringArray parts;
        explode(parts, line, '|');
        bool next = false;
        for (auto part : parts) {
            part = trim(part);
            if (next && outputFile.empty()) {
                outputFile = testRoot + part;
                break;
            }
            if (part == "output") {
                next = true;
            }
        }

        string_q contents = asciiFileToString(outputFile);
        if (fileExists(outputFile) && !contents.empty()) {
            ostringstream os;
            os << "----" << endl;
            os << "Results in " << substitute(outputFile, testRoot, "./") << endl;
            os << contents << endl;
            return os.str();
        }
    }

    return "";
}

//-----------------------------------------------------------------------------
bool CTestCase::shouldRun(bool isCmd) const {
    bool shouldRun1 = isCmd && mode != "api";
    bool shouldRun2 = !isCmd && mode != "cmd";
    return shouldRun1 || shouldRun2;
}

//-----------------------------------------------------------------------------
void CTestCase::cleanWorkFile(bool isCmd, const string_q& workFn) {
    string_q workContents = asciiFileToString(workFn);

    ostringstream prepender;
    for (auto e : envLines) {
        prepender << "Env: " << e << endl;
    }

    if (isCmd) {
        workContents += outputFileContents();
    } else {
        string_q curlFilePath = testRoot + name + "_curl.txt";
        if (fileSize(curlFilePath) > 0) {
            CStringArray curlFileLines;
            asciiFileToLines(curlFilePath, curlFileLines);
            for (auto curlArgument : curlFileLines) {
                prepender << "CURL: " << curlArgument << endl;
            }
        }
        prepender << route << "?" << options << endl;
    }

    if (!prepender.str().empty()) {
        workContents = prepender.str() + workContents;
    }

    stringToAsciiFile(workFn, substitute(workContents, "3735928559", "\"0xdeadbeef\""));
}

//-----------------------------------------------------------------------------
string_q CTestCase::apiUrl(void) const {
    string_q apiOptions = (!options.empty() ? ("?" + options) : "");
    string_q apiPort = getEnvStr("TB_TEST_API_SERVER").empty() ? "8080" : getEnvStr("TB_TEST_API_SERVER");
    return "http://localhost:" + apiPort + "/" + route + apiOptions;
}
