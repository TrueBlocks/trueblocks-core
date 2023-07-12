/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "utillib.h"
#include "options.h"
#include "testcase.h"
#include "measure.h"

ostringstream perf;
ostringstream slow;
CStringArray fails;
extern const char* STR_SCREEN_REPORT;
string_q perf_fmt;

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    loadEnvironmentPaths();
    CTestCase::registerClass();

    establishFolder(cacheFolder_tmp);
    CMeasure total("all", "all", "all");
    cerr.rdbuf(cout.rdbuf());

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return EXIT_FAILURE;

    bool runLocal = getGlobalConfig("testRunner")->getConfigBool("settings", "run_local", false);
    if (runLocal) {
        cerr << "Running local tests. Hit enter to continue." << endl;
        getchar();
    }

    total.git_hash = "git_" + string_q(GIT_COMMIT_HASH).substr(0, 10);
    string_q testFolder = getSourcePath3();
    uint32_t testID = 0;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return EXIT_FAILURE;

        for (auto testName : options.tests) {
            string_q path = nextTokenClear(testName, '/');
            LOG1("Processing file: ", path);
            if (options.full_test) {
                // only clean if we're testing all
                options.cleanTest(path, testName);
                if (options.modes & API)
                    options.cleanTest(path, testName + "/api_tests");
            }

            string_q testFile = testFolder + path + "/" + testName + ".csv";
            if (!fileExists(testFile))
                return !options.usage("Cannot find test file " + testFile + ".");

            string_q contents = asciiFileToString(testFile) + "\n";
            replaceAll(contents, "\n\n", "\n");

            CStringArray lines;
            explode(lines, contents, '\n');

            map<string_q, CTestCase> testMap;
            string_q ttOnly = getEnvStr("TEST_TEST_ONLY");
            bool ttOnlyB = !ttOnly.empty();
            for (auto line : lines) {
                if (ttOnlyB) {
                    runLocal = false;
                    if (startsWith(line, "test")) {
                        replace(line, "test", "on");
                    } else if (startsWith(line, ttOnly)) {
                        replace(line, ttOnly, "on");
                    } else if (startsWith(line, "on")) {
                        replace(line, "on", "local");
                    }
                }
                if (startsWith(line, "erigon"))
                    replace(line, "erigon", "local");
                if (runLocal && startsWith(line, "local"))
                    replace(line, "local", "on");
                bool ignore1 = startsWith(line, "#");
                bool ignore2 = !startsWith(line, "on") && !options.ignoreOff;
                bool ignore3 = startsWith(line, "enabled");
                bool ignore4 = false;
                if (!ignore3 && !options.filter.empty()) {
                    if (contains(line, " all,")) {
                        printf("%s", "");  // do nothing - do not remove cruft - squelches compiler warning
                    } else if (options.filter == "fast") {
                        ignore4 = !contains(line, "fast,");
                    } else if (options.filter == "slow") {
                        ignore4 = !contains(line, "slow,");
                    } else if (options.filter == "medi") {
                        ignore4 = !contains(line, "medi,");
                    }
                }

                if (line.empty() || ignore1 || ignore2 || ignore3 || ignore4) {
                    if (ignore2 && !options.ignoreOff) {
                        if (trim(line).substr(0, 120).length() > 0) {
                            cerr << iBlue << "   # " << line.substr(0, 120) << cOff << endl;
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
            perf_fmt = substitute(cleanFmt(STR_DISPLAY_MEASURE), "\"", "");
            options.doTests(total, testArray, path, testName, API, !ttOnlyB);
            options.doTests(total, testArray, path, testName, CMD, !ttOnlyB);
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
    total.allPassed = total.nTests == total.nPassed;
    ::sleep(1);
    total.date = Now().Format(FMT_EXPORT);
    if (options.report && options.skip == 1) {
        // Write performance data to a file and results to the screen
        perf << total.Format(perf_fmt) << endl;
        cerr << "    " << substitute(perf.str(), "\n", "\n    ") << endl;
        if (options.full_test && options.report) {
            string_q perfFile =
                rootConfigs + string_q("perf/performance") + (total.allPassed ? "" : "_failed") + ".csv";
            appendToAsciiFile(perfFile, perf.str());
            appendToAsciiFile(rootConfigs + "perf/performance_slow.csv", slow.str());
        } else {
            LOG_WARN(cRed, "Performance results not written because not full test", cOff);
        }
    }

    // If configured, copy the data out to the folder our performance measurement tool knows about
    string_q copyPath = getGlobalConfig("testRunner")->getConfigStr("settings", "copy_path", "<not_set>");
    if (folderExists(copyPath)) {
        CStringArray files = {"perf/performance.csv", "perf/performance_failed.csv", "perf/performance_slow.csv",
                              "perf/performance_scraper.csv"};
        for (auto file : files) {
            if (fileExists(rootConfigs + file)) {
                ostringstream copyCmd;
                copyCmd << "cp -f \"";
                copyCmd << rootConfigs + file << "\" \"" << copyPath << "\"";
                // clang-format off
                if (system(copyCmd.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
                // clang-format on
            }
        }
    }

    cerr << total.Format(STR_SCREEN_REPORT) << endl;
    for (auto fail : fails)
        cerr << fail;
    cerr << endl;

    return total.allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}

//-----------------------------------------------------------------------
void COptions::doTests(CMeasure& total, CTestCaseArray& testArray, const string_q& testPath, const string_q& testName,
                       int whichTest, bool doRemove) {
    if (!(modes & whichTest))
        return;

    resetClock();
    bool cmdTests = whichTest & CMD;

    CMeasure measure(testPath, testName, (cmdTests ? "cmd" : "api"));
    cerr << measure.Format("Testing [{COMMAND}] ([{TYPE}] mode):") << endl;

    for (auto test : testArray) {
        if (skip != 1 && nRun++ % skip)
            continue;
        if (verbose)
            cerr << string_q(120, '=') << endl << test << endl << string_q(120, '=') << endl;
        test.prepareTest(cmdTests, skip > 0 && doRemove);
        if ((!cmdTests && test.mode == "cmd") || (cmdTests && test.mode == "api")) {
            // do nothing - wrong mode

        } else if (!folderExists(test.goldPath)) {
            LOG_WARN("Folder ", test.goldPath, " not found.");
            return;

        } else if (!folderExists(test.workPath)) {
            LOG_WARN("Folder " + test.workPath + " not found.");
            return;

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
                    for (auto line : fileLines)
                        prepender << "Env: " << line << endl;
                    envLines.push_back(f);
                }
            }

            if (cmdTests) {
                string_q envs = substitute(substitute(linesToString(envLines, '|'), " ", ""), "|", " ");
                string_q env = "env " + envs + " TEST_MODE=true NO_COLOR=true REDIR_CERR=true ";

                string_q exe;
                if (contains(test.path, "libs")) {
                    exe = "test/" + test.tool;
                    if (test.isCmd)
                        exe = getPathToCommands(exe);

                } else {
                    exe = test.tool;
                    if (test.isCmd)
                        exe = "chifra " + test.route;
                }

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
                bool has_options = (!test.builtin && !test.options.empty());
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
                cmd << apiProvider;
                cmd << test.route;
                cmd << (has_options ? ("?" + test.options) : "");
                cmd << "\"";
                cmd << (has_post ? (" | " + test.post + " ") : "");
                cmd << " >" << test.workPath + test.fileName;
                prepender << test.route << "?" << test.options << endl;
            }

            // To run the test, we cd into the gold path (so we find the test files), but we send results to working
            // folder
            string_q goldApiPath = substitute(test.goldPath, "/api_tests", "");
            string_q outputFile = test.getOutputFile(whichTest == API, goldApiPath);
            string_q theCmd = "cd \"" + goldApiPath + "\" ; " + cmd.str();
            if (test.builtin)
                theCmd = "cd \"" + goldApiPath + "\" ; " + test.options;
            LOG4(theCmd);

            string_q customized =
                substitute(substitute(test.workPath, "working", "custom_config") + test.tool + "_" + test.name + "/",
                           "/api_tests", "");
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", saveAndCopy, NULL);
            if (test.mode == "both" || contains(test.tool, "lib"))
                measure.nTests++;
            // clang-format off
            if (system(theCmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
            // clang-format on
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", replaceFile, NULL);
            forEveryFileInFolder(test.goldPath + "*", postCleanup, NULL);

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
                // } else if (!outputFile.empty()) {
                //     cerr << "Output file not seen: " << outputFile << endl;
                //     exit(1);
            }

            replaceAll(contents, "3735928559", "\"0xdeadbeef\"");
            stringToAsciiFile(test.workPath + test.fileName, contents);

            if (test.builtin) {
                if (test.mode == "both" || contains(test.tool, "lib"))
                    measure.nPassed++;
                continue;
            }

            double thisTime = str_2_Double(TIC());
            if (test.mode == "both" || contains(test.tool, "lib"))
                measure.totSecs += thisTime;
            // clang-format off
            string_q timeRep = (thisTime > tooSlow       ? cRed
                                : thisTime <= fastEnough ? cGreen
                                                         : "") +
                               double_2_Str(thisTime, 5) + cOff;
            // clang-format on

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

            string_q result = greenCheck;
            if (!newText.empty() && newText == oldText) {
                if (test.mode == "both" || contains(test.tool, "lib"))
                    measure.nPassed++;

            } else {
                ostringstream os;
                os << cRed << "\tFailed: " << cTeal << (endsWith(test.path, "lib") ? test.tool : measure.cmd) << " ";
                os << test.name << ".txt " << cOff << "(" << (test.builtin ? "" : measure.cmd) << " "
                   << trim(test.options) << ")" << cRed;
                os << cOff << endl;
                fails.push_back(os.str());
                result = redX;
            }

            if (!contains(test.origLine, " all,")) {
                // Prepare for presentation to the screen...
                reverse(test.name);
                test.name = substitute(padLeft(test.name, 30).substr(0, 30), " ", ".");
                reverse(test.name);
                cerr << "   " << timeRep << " - "
                     << (endsWith(test.path, "lib") ? padRight(test.tool, 16) : measure.cmd) << " ";
                cerr << trim(test.name) << " " << result << "  " << trim(test.options).substr(0, 90) << endl;
                if (thisTime > verySlow) {
                    slow << "   " << double_2_Str(thisTime) << " - "
                         << (endsWith(test.path, "lib") ? padRight(test.tool, 16) : measure.cmd) << " ";
                    slow << trim(test.name) << " " << trim(test.options).substr(0, 90) << endl;
                }
            } else {
                // we ignore ethQuote testing since it deprecated
            }

            usleep(500);
            if (shouldQuit()) {
                LOG4("Quitting because of shouldQuit");
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
        measure.allPassed = measure.nTests == measure.nPassed;
        cerr << measure.Format(STR_SCREEN_REPORT) << endl;
        perf << measure.Format(perf_fmt) << endl;
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
bool postCleanup(const string_q& path, void* data) {
    if (contains(path, "makeClass") && (endsWith(path, ".cpp") || endsWith(path, ".h"))) {
        ::remove(path.c_str());
    }
    return true;
}

//-----------------------------------------------------------------------
double verySlow = .6;
double tooSlow = .4;
double fastEnough = .2;

//-----------------------------------------------------------------------
const char* STR_SCREEN_REPORT =
    "   [{CMD}] ([{TYPE}][,{FILTER}]): [{NTESTS}] tests [{NPASSED}] passed [{CHECK}] [{FAILED} failed] in "
    "[{TOTSECS}] "
    "seconds [{AVGSECS}] avg.";
