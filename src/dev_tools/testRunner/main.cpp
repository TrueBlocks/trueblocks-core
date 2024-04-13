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

#include "utillib.h"
#include "options.h"
#include "testcase.h"

int main(int argc, const char* argv[]) {
    cerr.rdbuf(cout.rdbuf());
    cleanFolder(getCachePath() + "tmp/");

    COptions options;
    options.init();

    for (auto testName : options.tests) {
        string_q path = nextTokenClear(testName, '/');
        cleanTest(path, testName);
        cleanTest(path, testName + "/api_tests");

        string_q testFolder = getCWD() + string_q("../../../../src/dev_tools/testRunner/testCases/");
        string_q testFile = testFolder + path + "/" + testName + ".csv";
        if (!fileExists(testFile)) {
            cerr << "Cannot find test file " + testFile + ".";
            return EXIT_FAILURE;
        }

        CStringArray testLines;
        vector<CTestCase> testArray;
        asciiFileToLines(testFile, testLines);

        for (auto line : testLines) {
            if (startsWith(line, "#") || startsWith(line, "enabled") || line.empty()) {
                continue;
            }

            CStringArray parts;
            explode(parts, line, ',');
            if (parts.size() < 7) {
                cerr << "Wrong size in test line: " << testName << ": " << line << endl;
                exit(1);
            }

            CTestCase test(line);
            if (!startsWith(line, "on")) {
                if (trim(line).substr(0, 120).length() > 0) {
                    cerr << "   # " << line.substr(0, 120) << endl;
                }
                copyBack(test.path, test.tool, test.fileName);

            } else {
                static map<string_q, CTestCase> testMap;
                string_q key = test.route + "-" + test.tool + "-" + test.name;
                if (testMap[key] != CTestCase()) {
                    cerr << "Duplicate test names: " << key << ". Quitting..." << endl;
                    return EXIT_FAILURE;
                }
                testMap[key] = test;
                testArray.push_back(test);
            }
        }

        options.doTests(testArray, testName, API);
        options.doTests(testArray, testName, CMD);
    }

    bool allPassed = options.totalTests == options.totalPassed;
    uint64_t nFailed = options.totalTests - options.totalPassed;

    cerr << string_q(125, '=') << endl;
    cerr << "   all (all): " << options.totalTests << " tests " << options.totalPassed << " passed ";
    cerr << (nFailed ? "" : "ok") << " " << nFailed << " failed." << endl;
    for (auto fail : options.fails)
        cerr << fail;
    cerr << endl;

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}

void COptions::doTests(vector<CTestCase>& testArray, const string_q& testName, int which) {
    bool isCmd = (which & CMD);
    cerr << "Testing " + testName + " " + (isCmd ? "cmd" : "api") + " mode):" << endl;

    uint64_t nTests = 0;
    uint64_t nPassed = 0;
    for (auto test : testArray) {
        test.prepareTest(isCmd);
        if ((isCmd && test.mode != "api") || (!isCmd && test.mode != "cmd")) {
            ostringstream cmd;
            ostringstream prepender;

            CStringArray fileLines;
            string_q envFile = substitute(test.goldPath, "/api_tests", "") + test.name + ".env";
            if (fileExists(envFile))
                asciiFileToLines(envFile, fileLines);

            CStringArray envLines;
            for (auto f : fileLines) {
                if (!startsWith(f, "#")) {
                    prepender << "Env: " << f << endl;
                    envLines.push_back(f);
                }
            }

            if (isCmd) {
                string_q envs = substitute(substitute(linesToString(envLines, '|'), " ", ""), "|", " ");
                string_q env = "env " + envs + " TEST_MODE=true NO_COLOR=true ";
                string_q exe = "chifra" + (contains(test.tool, "chifra") ? "" : " " + test.route);
                string_q fullCmd = exe + " " + test.options;
                string_q debugCmd = substitute(fullCmd, getHomeFolder(), "$HOME/");
                string_q redir = test.workPath + test.fileName;
                cmd << "echo \"" << debugCmd << "\" >" << redir + " && ";
                string_q rFile = substitute(test.goldPath, "/api_tests", "") + test.name + ".redir";
                if (fileExists(rFile)) {
                    fullCmd += " --output " + test.name + "_out.file";
                    test.origLine += " & output = " + test.name + "_out.file";
                }
                cmd << env << fullCmd << " >>" << redir << " 2>&1";

            } else {
                bool has_options = !test.options.empty();
                bool has_post = !test.post.empty();
                bool has_env = (envLines.size() > 0);

                // Check if curl configuration file for the given test exists. We can use such
                // files to send requests other than GET, e.g. when testing CRUD endpoints
                CStringArray curlFileLines;
                string_q curlFilePath = substitute(test.goldPath, "/api_tests", "") + test.name + "_curl.txt";
                if (fileExists(curlFilePath))
                    asciiFileToLines(curlFilePath, curlFileLines);

                cmd << "curl -s ";
                if (curlFileLines.size() > 0) {
                    cmd << "--config " << curlFilePath << " ";
                    for (auto curlArgument : curlFileLines) {
                        // cmd << curlArgument << " ";
                        prepender << "CURL: " << curlArgument << endl;
                    }
                }
                cmd << "-H \"User-Agent: testRunner\" ";
                if (has_env)
                    cmd << "-H \"X-TestRunner-Env: " << substitute(linesToString(envLines, '|'), " ", "") << "\" ";
                cmd << "\"";
                string_q port = getEnvStr("TB_TEST_API_SERVER");
                string_q apiProvider = "http://localhost:" + (port.empty() ? "8080" : port) + "/";
                cmd << apiProvider;
                cmd << test.route;
                cmd << (has_options ? ("?" + test.options) : "");
                cmd << "\"";
                cmd << (has_post ? (" | " + test.post + " ") : "");
                cmd << " >" << test.workPath + test.fileName;
                prepender << test.route << "?" << test.options << endl;
            }

            string_q goldApiPath = substitute(test.goldPath, "/api_tests", "");
            string_q outputFile = "";
            if (which != API && contains(test.origLine, "output")) {
                outputFile = getOutputFile(test.origLine, goldApiPath);
            }
            string_q theCmd = "cd \"" + goldApiPath + "\" ; " + cmd.str();

            nTests++;
            if (system(theCmd.c_str())) {
            }  // Don't remove cruft. Silences compiler warnings

            string_q contents2 = asciiFileToString(test.workPath + test.fileName);
            if (!prepender.str().empty()) {
                contents2 = prepender.str() + contents2;
            }

            if (!outputFile.empty() && fileExists(outputFile)) {
                ostringstream os;
                os << "----" << endl;
                os << "Results in " << substitute(outputFile, goldApiPath, "./") << endl;
                os << asciiFileToString(outputFile) << endl;
                contents2 += os.str();
            }

            replaceAll(contents2, "3735928559", "\"0xdeadbeef\"");
            stringToAsciiFile(test.workPath + test.fileName, contents2);

            if (endsWith(test.path, "lib"))
                replace(test.workPath, "../", "");

            string_q newFn = test.goldPath + test.fileName;
            string_q newText = asciiFileToString(newFn);

            string_q oldFn = test.workPath + test.fileName;
            string_q oldText = asciiFileToString(oldFn);
            string_q result = "ok";
            if (!newText.empty() && newText == oldText) {
                nPassed++;

            } else {
                ostringstream os;
                os << "\tFailed: " << (endsWith(test.path, "lib") ? test.tool : testName) << " ";
                os << test.name << ".txt "
                   << "(" << testName << " " << trim(test.options) << ")";
                os << endl;
                fails.push_back(os.str());
                result = "X";
            }

            if (!contains(test.origLine, " all,")) {
                reverse(test.name);
                test.name = substitute(padLeft(test.name, 30).substr(0, 30), " ", ".");
                reverse(test.name);
                cerr << "   - " << (endsWith(test.path, "lib") ? padRight(test.tool, 16) : testName) << " ";
                cerr << trim(test.name) << " " << result << "  " << trim(test.options).substr(0, 90) << endl;
            } else {
            }

            usleep(500);
            envLines.clear();
        }
        if (fileExists("/tmp/output_test_with_path.json")) {
            ::remove("/tmp/output_test_with_path.json");
        }
    }

    totalTests += nTests;
    totalPassed += nPassed;
    if (nTests) {
        uint64_t nFailed = nTests - nPassed;
        cerr << "   " + testName + " " + (isCmd ? "cmd" : "api") + ": " << nTests << " tests ";
        cerr << nPassed << " passed ";
        cerr << (nFailed ? "X" : "ok") << " " << nFailed << " failed." << endl;
    }

    return;
}

string_q getOutputFile(const string_q& orig, const string_q& goldApiPath) {
    string_q line = substitute(substitute(orig, "&", "|"), "=", "|");
    CStringArray parts;
    explode(parts, line, '|');
    bool next = false;
    string_q outputFile;
    for (auto part : parts) {
        part = trim(part);
        if (next && outputFile.empty()) {
            outputFile = goldApiPath + part;
        }
        if (part == "output") {
            next = true;
        }
    }
    return outputFile;
}

bool cleanTest(const string_q& path, const string_q& testName) {
    ostringstream os;
    os << "find ../../../working/" << path << "/" << testName << "/ -maxdepth 1 -name \"" << testName
       << "_*.txt\" -exec rm '{}' ';' 2>/dev/null ; ";
    os << "find ../../../working/" << path << "/" << testName << "/api_tests/ -maxdepth 1 -name \"" << testName
       << "_*.txt\" -exec rm '{}' ';' 2>/dev/null ; ";
    if (system(os.str().c_str())) {
    }  // Don't remove cruft. Silences compiler warnings
    return true;
}

void copyBack(const string_q& path, const string_q& tool, const string_q& fileName) {
    string_q tr = "/test/gold/dev_tools/testRunner/";
    string_q fn = path + "/" + tool + "/" + fileName;
    string_q fnA = path + "/" + tool + "/api_tests/" + fileName;

    string_q goldPath = substitute(getCWD(), tr, "/test/gold/" + fn);
    string_q workPath = substitute(getCWD(), tr, "/test/working/" + fn);
    if (fileExists(goldPath)) {
        copyFile(goldPath, workPath);
    }

    goldPath = substitute(getCWD(), tr, "/test/gold/" + fnA);
    workPath = substitute(getCWD(), tr, "/test/working/" + fnA);
    if (fileExists(goldPath)) {
        copyFile(goldPath, workPath);
    }
}

string_q linesToString(const CStringArray& lines, char sep) {
    ostringstream os;
    for (auto line : lines)
        os << line << (sep != 0 ? string_q(1, sep) : "");
    return os.str();
}

string_q getCachePath(void) {
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__unix) || defined(__unix__)
    string_q configPath = getHomeFolder() + ".local/share/trueblocks/";
#elif defined(__APPLE__) || defined(__MACH__)
    string_q configPath = getHomeFolder() + "Library/Application Support/TrueBlocks/";
#endif
    return configPath + "cache/mainnet/";
}

string_q padRight(const string_q& str, size_t len, char p) {
    if (len > str.length())
        return str + string_q(len - str.length(), p);
    return str;
}

string_q padLeft(const string_q& str, size_t len, char p) {
    if (len > str.length())
        return string_q(len - str.length(), p) + str;
    return str;
}

string_q getEnvStr(const string_q& name) {
    char* sss = getenv(name.c_str());
    return (sss ? string_q(sss) : string_q(""));
}

static string_q escapePath(const string_q& nameIn) {
    string_q name = nameIn;
    replaceAll(name, "&", "\\&");
    replaceAll(name, "(", "\\(");
    replaceAll(name, ")", "\\)");
    replaceAll(name, "'", "\\'");
    return name;
}

int copyFile(const string_q& fromIn, const string_q& toIn) {
    ifstream src(escapePath(fromIn), ios::binary);
    ofstream dst(escapePath(toIn), ios::binary);
    dst << src.rdbuf();
    return static_cast<int>(fileExists(toIn));
}

namespace qblocks {
typedef bool (*CONSTAPPLYFUNC)(const string_q& path, void* data);
extern bool forAllFiles(const string_q& mask, CONSTAPPLYFUNC func, void* data);
}  // namespace qblocks

namespace filename_local {
class CFileListState {
  public:
    string_q top;
    CStringArray& list;
    bool recurse;
    CFileListState(const string_q& t, CStringArray& l, bool r) : top(t), list(l), recurse(r) {
    }
};
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

size_t listFilesInFolder(CStringArray& items, const string_q& folder, bool recurse) {
    filename_local::CFileListState state(folder, items, recurse);
    forAllFiles(folder, filename_local::visitFile, &state);
    return items.size();
}

int cleanFolder(const string_q& path, bool recurse, bool interactive) {
    CStringArray files;
    listFilesInFolder(files, path, true);
    for (auto file : files)
        ::remove(file.c_str());
    return static_cast<int>(files.size());
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
    waitForCreate(filename);
    string_q ret = asciiFileToString(filename);
    ::remove(filename.c_str());
    return trim(ret, '\n');
}

static const char* CHR_VALID_NAME =
    "\t\n\r()<>[]{}`\\|; "
    "'!$^*~@"
    "?&#+%"
    ",:/=\"";

string_q makeValidName(const string_q& inOut) {
    string_q ret = inOut;
    replaceAny(ret, CHR_VALID_NAME, "_");
    if (!ret.empty() && isdigit(ret[0]))
        ret = "_" + ret;
    return ret;
}
