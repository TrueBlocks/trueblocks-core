/*
 */
// If configured, copy the data out to the folder our performance measurement tool knows about
// Write performance data to a file and results to the screen
// Support --mode both|cmd|api
// Make sure both gold and working folders exist
// Must read .env files if present and put them in the environment

#include "utillib.h"
#include "options.h"
#include "testcase.h"
#include "measure.h"

CStringArray fails;
extern const char* STR_SCREEN_REPORT;
extern string_q getOutputFile(const string& orig, const string_q& goldApiPath);

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    loadEnvironmentPaths();

    CMeasure total("all", "all", "all");
    cerr.rdbuf(cout.rdbuf());

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return EXIT_FAILURE;

    string_q testFolder = getCWD() + string_q("../../../../src/dev_tools/testRunner/testCases/");
    uint32_t testID = 0;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return EXIT_FAILURE;

        cleanFolder(cacheFolder_tmp);

        for (auto testName : options.tests) {
            string_q path = nextTokenClear(testName, '/');
            options.cleanTest(path, testName);
            options.cleanTest(path, testName + "/api_tests");

            string_q testFile = testFolder + path + "/" + testName + ".csv";
            if (!fileExists(testFile))
                return !options.usage("Cannot find test file " + testFile + ".");

            string_q contents = asciiFileToString(testFile) + "\n";
            replaceAll(contents, "\n\n", "\n");

            CStringArray lines;
            explode(lines, contents, '\n');

            map<string_q, CTestCase> testMap;
            for (auto line : lines) {
                bool ignore1 = startsWith(line, "#");
                bool ignore2 = !startsWith(line, "on");
                bool ignore3 = startsWith(line, "enabled");
                if (line.empty() || ignore1 || ignore2 || ignore3) {
                    if (ignore2) {
                        if (trim(line).substr(0, 120).length() > 0) {
                            cerr << "   # " << line.substr(0, 120) << endl;
                        }
                        CTestCase test(line, 0);
                        test.goldPath = substitute(getCWD(), "/test/gold/dev_tools/testRunner/",
                                                   "/test/gold/" + test.path + "/" + test.tool + "/" + test.fileName);
                        // if the gold file exists, copy the test case back to working (it may have been removed)
                        if (fileExists(test.goldPath)) {
                            test.workPath =
                                substitute(getCWD(), "/test/gold/dev_tools/testRunner/",
                                           "/test/working/" + test.path + "/" + test.tool + "/" + test.fileName);
                            copyFile(test.goldPath, test.workPath);
                        }
                        replace(test.goldPath, "/" + test.tool + "/", "/" + test.tool + "/api_tests/");
                        if (fileExists(test.goldPath)) {
                            test.workPath = substitute(
                                getCWD(), "/test/gold/dev_tools/testRunner/",
                                "/test/working/" + test.path + "/" + test.tool + "/api_tests/" + test.fileName);
                            copyFile(test.goldPath, test.workPath);
                        }
                    }

                } else {
                    CTestCase test(line, testID++);
                    string_q key = test.Format("[{KEY}]");
                    if (testMap[key] != CTestCase()) {
                        cerr << "Duplicate test names: " << key << ". Quitting..." << endl;
                        return EXIT_FAILURE;
                    }
                    testMap[key] = test;
                }
            }

            CTestCaseArray testArray;
            for (auto t : testMap)
                testArray.push_back(t.second);
            sort(testArray.begin(), testArray.end());

            expContext().exportFmt = CSV1;
            options.doTests(total, testArray, path, testName, API);
            options.doTests(total, testArray, path, testName, CMD);
            if (shouldQuit())
                break;

            if (getEnvStr("WAIT_PER_TEST") == "true") {
                cerr << "Press enter to continue" << endl;
                getchar();
            }
        }
    }

    // We've run through all the tests. We know how many we've run and we know how
    // many have passed, so we know if all of them passed.
    bool allPassed = total.nTests == total.nPassed;

    cerr << string_q(125, '=') << endl;
    cerr << total.Format(STR_SCREEN_REPORT) << endl;
    for (auto fail : fails)
        cerr << fail;
    cerr << endl;

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}

void COptions::doTests(CMeasure& total, CTestCaseArray& testArray, const string_q& testPath, const string_q& testName,
                       int whichTest) {
    bool cmdTests = whichTest & CMD;

    CMeasure measure(testPath, testName, (cmdTests ? "cmd" : "api"));
    cerr << measure.Format("Testing [{COMMAND}] ([{TYPE}] mode):") << endl;

    for (auto test : testArray) {
        test.prepareTest(cmdTests);
        if ((!cmdTests && test.mode == "cmd") || (cmdTests && test.mode == "api")) {
            // do nothing - wrong mode
        } else {
            ostringstream cmd;
            CStringArray fileLines;
            string_q envFile = substitute(test.goldPath, "/api_tests", "") + test.name + ".env";
            if (fileExists(envFile))
                asciiFileToLines(envFile, fileLines);

            ostringstream prepender;
            CStringArray envLines;
            for (auto f : fileLines) {
                if (!startsWith(f, "#")) {
                    prepender << "Env: " << f << endl;
                    envLines.push_back(f);
                }
            }

            if (cmdTests) {
                string_q envs = substitute(substitute(linesToString(envLines, '|'), " ", ""), "|", " ");
                string_q env = "env " + envs + " TEST_MODE=true NO_COLOR=true REDIR_CERR=true ";
                string_q exe = "chifra" + (contains(test.tool, "chifra") ? "" : " " + test.route);
                string_q fullCmd = exe + " " + test.options;
                string_q debugCmd = relativize(fullCmd);
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
                string_q apiProvider =
                    getGlobalConfig("testRunner")->getConfigStr("settings", "api_provider", "http://localhost:8080");
                if (!endsWith(apiProvider, "/"))
                    apiProvider += "/";
                cmd << apiProvider;
                cmd << test.route;
                cmd << (has_options ? ("?" + test.options) : "");
                cmd << "\"";
                cmd << (has_post ? (" | " + test.post + " ") : "");
                cmd << " >" << test.workPath + test.fileName;
                prepender << test.route << "?" << test.options << endl;
            }

            // To run the test, we cd into the gold path (so we find the test
            // files), but we send results to working folder
            string_q goldApiPath = substitute(test.goldPath, "/api_tests", "");
            string_q outputFile = "";
            if (whichTest != API && contains(test.origLine, "output")) {
                outputFile = getOutputFile(test.origLine, goldApiPath);
            }
            string_q theCmd = "cd \"" + goldApiPath + "\" ; " + cmd.str();

            string_q customized =
                substitute(substitute(test.workPath, "working", "custom_config") + test.tool + "_" + test.name + "/",
                           "/api_tests", "");
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", saveAndCopy, NULL);
            measure.nTests++;
            if (system(theCmd.c_str())) {
            }  // Don't remove cruft. Silences compiler warnings
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", replaceFile, NULL);

            string_q contents = asciiFileToString(test.workPath + test.fileName);
            if (!prepender.str().empty()) {
                contents = prepender.str() + contents;
            }

            if (!outputFile.empty() && fileExists(outputFile)) {
                ostringstream os;
                os << "----" << endl;
                os << "Results in " << substitute(outputFile, goldApiPath, "./") << endl;
                os << asciiFileToString(outputFile) << endl;
                contents += os.str();
            }

            replaceAll(contents, "3735928559", "\"0xdeadbeef\"");
            stringToAsciiFile(test.workPath + test.fileName, contents);

            double thisTime = .02;
            string_q timeRep = double_2_Str(thisTime, 5);
            if (endsWith(test.path, "lib"))
                replace(test.workPath, "../", "");

            string_q newFn = test.goldPath + test.fileName;
            string_q newText = asciiFileToString(newFn);

            string_q oldFn = test.workPath + test.fileName;
            string_q oldText = asciiFileToString(oldFn);
            if (contains(oldText, "The XDG_")) {
                // Weird case where we can't turn off timestamp from logs, so we blow away the timing here
                CStringArray lines;
                explode(lines, oldText, '\n');
                ostringstream os;
                for (auto line : lines) {
                    if (contains(line, "The XDG_")) {
                        line = substitute(line, "The XDG_", "|The XDG_");
                        nextTokenClear(line, '|');
                    }
                    os << line << endl;
                }
                oldText = os.str();
                stringToAsciiFile(oldFn, oldText);
            }

            string_q result = "ok";
            if (!newText.empty() && newText == oldText) {
                measure.nPassed++;

            } else {
                ostringstream os;
                os << "\tFailed: " << (endsWith(test.path, "lib") ? test.tool : measure.cmd) << " ";
                os << test.name << ".txt "
                   << "(" << measure.cmd << " " << trim(test.options) << ")";
                os << endl;
                fails.push_back(os.str());
                result = "X";
            }

            if (!contains(test.origLine, " all,")) {
                // Prepare for presentation to the screen...
                reverse(test.name);
                test.name = substitute(padLeft(test.name, 30).substr(0, 30), " ", ".");
                reverse(test.name);
                cerr << "   " << timeRep << " - "
                     << (endsWith(test.path, "lib") ? padRight(test.tool, 16) : measure.cmd) << " ";
                cerr << trim(test.name) << " " << result << "  " << trim(test.options).substr(0, 90) << endl;
            } else {
                // we ignore ethQuote testing since it deprecated
            }

            usleep(500);
            if (shouldQuit()) {
                break;
            }
            envLines.clear();
        }
        // Cleanup files put in the temp folder, if any
        if (fileExists("/tmp/output_test_with_path.json")) {
            ::remove("/tmp/output_test_with_path.json");
        }
    }

    total += measure;
    if (measure.nTests) {
        // measure.allPassed = measure.nTests == measure.nPassed;
        cerr << measure.Format(STR_SCREEN_REPORT) << endl;
    }

    return;
}

//-----------------------------------------------------------------------
bool saveAndCopy(const string_q& customFile, void* data) {
    CStringArray parts;
    explode(parts, customFile, '/');
    string_q destFile = rootConfigs + "configs/mainnet/" + parts[parts.size() - 1];
    string_q saveFile = cacheFolder_tmp + parts[parts.size() - 1] + ".save";
    copyFile(destFile, saveFile);
    copyFile(customFile, destFile);
    return true;
}

//-----------------------------------------------------------------------
bool replaceFile(const string_q& customFile, void* data) {
    CStringArray parts;
    explode(parts, customFile, '/');
    string_q destFile = rootConfigs + "configs/mainnet/" + parts[parts.size() - 1];
    string_q saveFile = cacheFolder_tmp + parts[parts.size() - 1] + ".save";
    copyFile(saveFile, destFile);
    ::remove(saveFile.c_str());
    return true;
}

//-----------------------------------------------------------------------
const char* STR_SCREEN_REPORT =
    "   [{CMD}] ([{TYPE}][,{FILTER}]): [{NTESTS}] tests [{NPASSED}] passed [{CHECK}] [{FAILED} failed] in "
    "[{TOTSECS}].";

//---------------------------------------------------------------------------------------------
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
