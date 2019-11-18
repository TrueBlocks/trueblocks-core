/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "testcase.h"
#include "measure.h"

ostringstream perf;
ostringstream slow;
CMeasure total("all", "all", "all");
CStringArray fails;
extern const char* STR_SCREEN_REPORT;

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    CTestCase::registerClass();

    cerr.rdbuf(cout.rdbuf());

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    total.git_hash = "git_" + string_q(GIT_COMMIT_HASH).substr(0, 10);
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (auto testName : options.tests) {
            string_q path = nextTokenClear(testName, '/');
            LOG1("Processing file: ", path);
            options.cleanTest(path, testName);
            if (options.modes & API)
                options.cleanTest(path, testName + "/api_tests");

            string_q testFile = getCWD() + "../../../../src/other/testCases/" + path + "/" + testName + ".csv";
            if (!fileExists(testFile))
                return options.usage("Cannot find test file " + testFile + ". Quitting.");

            string_q contents = asciiFileToString(testFile) + "\n";
            replaceAll(contents, "\n\n", "\n");

            CStringArray lines;
            explode(lines, contents, '\n');

            CTestCaseArray testArray;
            for (auto line : lines) {
                bool ignore1 = startsWith(line, "#");
                bool ignore2 = startsWith(line, "off") && !options.ignoreOff;
                bool ignore3 = contains(line, ", route,");
                bool ignore4 = false;
                if (!ignore3 && !options.filter.empty()) {
                    if (contains(line, " all,")) {
                        int ret;
                        ret = 0;  // do not remove - squelches compiler warning
                    } else if (options.filter == "fast") {
                        ignore4 = !contains(line, "fast,");
                    } else if (options.filter == "slow") {
                        ignore4 = !contains(line, "slow,");
                    } else if (options.filter == "medi") {
                        ignore4 = !contains(line, "medi,");
                    }
                }

                if (line.empty() || ignore1 || ignore2 || ignore3 || ignore4) {
                    if (ignore2 && !options.ignoreOff)
                        cerr << iBlue << "   # " << line << cOff << endl;
                    // do nothing

                } else {
                    CTestCase test(line);
                    testArray.push_back(test);
                }
            }

            if (!options.doTests(testArray, path, testName, API))
                return 1;

            if (!options.doTests(testArray, path, testName, CMD))
                return 1;

            if (shouldQuit())
                break;
        }
    }

    if (total.nTests == total.nPassed) {
        perf << total.Format(options.perf_format) << endl;
        cerr << "    " << substitute(perf.str(), "\n", "\n    ") << endl;
        if (options.full_test && options.report)
            appendToAsciiFile(configPath("performance.csv"), perf.str());
        appendToAsciiFile(configPath("performance_slow.csv"), slow.str());
    }

    cerr << total.Format(STR_SCREEN_REPORT) << endl;
    for (auto fail : fails)
        cerr << fail;
    cerr << endl;

    return 0;
}

//-----------------------------------------------------------------------
bool COptions::doTests(CTestCaseArray& testArray, const string_q& testPath, const string_q& testName, int whichTest) {
    if (!(modes & whichTest))
        return true;

    resetClock();
    bool cmdTests = whichTest & CMD;

    CMeasure measure(testPath, testName, (cmdTests ? "cmd" : "api"));
    cerr << measure.Format("Testing [{COMMAND}] ([{TYPE}] mode):") << endl;

    for (auto test : testArray) {
        test.prepareTest(cmdTests);
        if ((!cmdTests && test.mode == "cmd") || (cmdTests && test.mode == "api")) {
            // do nothing - wrong mode

        } else if (!folderExists(test.goldPath)) {
            return usage("Folder " + test.goldPath + " not found. Quitting...");

        } else if (!folderExists(test.workPath)) {
            return usage("Folder " + test.workPath + " not found. Quitting...");

        } else {
            ostringstream cmd;
            if (cmdTests) {
                string_q c = test.tool + test.options + " >" + test.workPath + test.fileName + " 2>&1";
                string_q e = "env " + test.extra + " TEST_MODE=true NO_COLOR=true REDIR_CERR=true ";
                cmd << e << c;
            } else {
                cmd << "curl -s \"http://localhost:8080/" << test.route;
                if (!test.builtin && !test.options.empty())
                    cmd << "?" << test.options;
                cmd << "\"";
                if (!no_post && !test.post.empty())
                    cmd << " | " << test.post << " ";
                cmd << " >" << test.workPath + test.fileName;
            }

            string_q theCmd = "cd " + substitute(test.goldPath, "/api_tests", "") + " ; " + cmd.str();
            if (test.builtin)
                theCmd = "cd " + substitute(test.goldPath, "/api_tests", "") + " ; " + test.options;
            LOG4(theCmd);

            string_q customized = substitute(
                substitute(test.workPath, "working", "custom_config") + test.tool + "_" + test.filename + "/",
                "/api_tests", "");
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", saveAndCopy, NULL);
            if (test.mode == "both" || contains(test.tool, "lib"))
                measure.nTests++;
            int ret = system(theCmd.c_str());
            {
                if (ret) {
                    printf("%s", "");
                }
            }  // do not remove, squelches warning
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", replaceFile, NULL);

            if (test.builtin) {
                if (test.mode == "both" || contains(test.tool, "lib"))
                    measure.nPassed++;
                continue;
            }

            double thisTime = str_2_Double(TIC());
            if (test.mode == "both" || contains(test.tool, "lib"))
                measure.totSecs += thisTime;
            string_q timeRep =
                (thisTime > tooSlow ? cRed : thisTime <= fastEnough ? cGreen : "") + double_2_Str(thisTime, 5) + cOff;

            if (endsWith(test.path, "lib"))
                replace(test.workPath, "../", "");

            string_q newFn = test.goldPath + test.fileName;
            string_q newText = asciiFileToString(newFn);

            string_q oldFn = test.workPath + test.fileName;
            string_q oldText = asciiFileToString(oldFn);

            string_q result = greenCheck;
            if (!newText.empty() && newText == oldText) {
                if (test.mode == "both" || contains(test.tool, "lib"))
                    measure.nPassed++;

            } else {
                ostringstream os;
                os << cRed << "\tFailed: " << cTeal << (endsWith(test.path, "lib") ? test.tool : measure.cmd) << " ";
                os << test.filename << ".txt " << cOff << "(" << (test.builtin ? "" : measure.cmd) << " "
                   << trim(test.options) << ")" << cRed;
                //                if (newText.empty())    os << " working file is empty ";
                //                if (ret)                os << " system call returned non-zero ";
                //                if (newText != oldText) {
                //                    os << " files differ " << endl;
                //                    os << "newFile: " << newFn << ": " << fileExists(newFn) << ": " << newText.size()
                //                    << endl;
                ////                    os << cYellow << newText << endl;
                //                    os << "oldFile: " << oldFn << ": " << fileExists(oldFn) << ": " << oldText.size()
                //                    << endl;
                ////                    os << cBlue << oldText;
                //                }
                os << cOff << endl;
                fails.push_back(os.str());
                result = redX;
            }

            if (!contains(test.origLine, " all,")) {
                reverse(test.filename);
                test.filename = substitute(padLeft(test.filename, 30).substr(0, 30), " ", ".");
                reverse(test.filename);
                cerr << "   " << timeRep << " - "
                     << (endsWith(test.path, "lib") ? padRight(test.tool, 16) : measure.cmd) << " ";
                cerr << trim(test.filename) << " " << result << "  " << trim(test.options).substr(0, 90) << endl;
                if (thisTime > verySlow) {
                    slow << "   " << double_2_Str(thisTime) << " - "
                         << (endsWith(test.path, "lib") ? padRight(test.tool, 16) : measure.cmd) << " ";
                    slow << trim(test.filename) << " " << trim(test.options).substr(0, 90) << endl;
                }
            }

            if (!no_quit && (result == redX))
                return false;

            usleep(1000);

            if (shouldQuit()) {
                LOG4("Quitting because of shouldQuit");
                break;
            }
        }
    }

    total += measure;
    if (measure.nTests) {
        cerr << measure.Format(STR_SCREEN_REPORT) << endl;
        perf << measure.Format(perf_format) << endl;
    }

    return true;
}

//-----------------------------------------------------------------------
bool saveAndCopy(const string_q& customFile, void* data) {
    CStringArray parts;
    explode(parts, customFile, '/');
    string_q destFile = configPath(parts[parts.size() - 1]);
    string_q saveFile = getCachePath("tmp/" + parts[parts.size() - 1] + ".save");
    copyFile(destFile, saveFile);
    copyFile(customFile, destFile);
    return true;
}

//-----------------------------------------------------------------------
bool replaceFile(const string_q& customFile, void* data) {
    CStringArray parts;
    explode(parts, customFile, '/');
    string_q destFile = configPath(parts[parts.size() - 1]);
    string_q saveFile = getCachePath("tmp/" + parts[parts.size() - 1] + ".save");
    copyFile(saveFile, destFile);
    ::remove(saveFile.c_str());
    return true;
}

//-----------------------------------------------------------------------
double verySlow = .6;
double tooSlow = .4;
double fastEnough = .2;

//-----------------------------------------------------------------------
const char* STR_SCREEN_REPORT =
    "   [{CMD}] ([{TYPE}][,{FILTER}]): [{NTESTS}] tests [{NPASSED}] passed [{CHECK}] [{FAILED} failed] in [{TOTSECS}] "
    "seconds [{AVGSECS}] avg.";
