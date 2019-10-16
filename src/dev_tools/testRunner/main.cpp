/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
#include "test_case.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    cerr.rdbuf( cout.rdbuf() );

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (auto testName : options.tests) {

            string_q path = nextTokenClear(testName, '/');
            LOG1("Processing file: ", path);
            options.cleanTest(path, testName);

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
                if (!ignore3 && !options.speed_filter.empty()) {
                         if (contains(line, " all,")) { /* do nothing */ }
                    else if (options.speed_filter == "fast" ) ignore4 = !contains(line, "fast,");
                    else if (options.speed_filter == "slow" ) ignore4 = !contains(line, "slow,");
                    else if (options.speed_filter == "medi" ) ignore4 = !contains(line, "medi,");
                }

                if (line.empty() || startsWith(line, "enabled") || ignore1 || ignore2 || ignore3 || ignore4) {
                    if (ignore2 && !options.ignoreOff)
                        cerr << iBlue << "   # " << line << cOff << endl;
                    // do nothing

                } else {
                    CTestCase test(line);
                    testArray.push_back(test);
                }
            }

            if (!options.doTests(testArray, testName, CMD))
                return 1;

            if (!options.doTests(testArray, testName, API))
                return 1;

            if (shouldQuit())
                break;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------
bool COptions::doTests(CTestCaseArray& testArray, const string_q& testName, int whichTest) {

    if (!(modes & whichTest))
        return true;

    resetClock();
    double totalTime = 0;

    bool cmdTests = whichTest & CMD;

    cerr << "Testing " << testName << " (" << (cmdTests ? "cmd" : "api") << " mode):" << endl;

    for (auto test: testArray) {

        test.prepareTest(cmdTests);
        if ((!cmdTests && test.mode == "cmd") || (cmdTests && test.mode == "api")) {
            // do nothing - wrong mode

        } else if (!folderExists(test.goldPath)) {
            return usage("Folder " + test.goldPath + " not found. Quitting...");

        } else if (!folderExists(test.workPath)) {
            return usage("Folder " + test.workPath + " not found. Quitting...");

        } else {

            if (endsWith(test.path, "lib"))
                test.workPath = "../" + test.workPath;

            ostringstream cmd;
            if (cmdTests) {
                string_q c = test.tool + test.options + " >" + test.workPath + test.fileName + " 2>&1";
                cmd << "TEST_MODE=true NO_COLOR=true REDIR_CERR=true " << c;

            } else {
                cmd << "curl -s \"http:/""/localhost:8080/" << test.route;
                if (!test.options.empty())
                    cmd << "?" << test.options;
                cmd << "\"";
                if (!test.post.empty())
                    cmd << " | " <<  test.post << " ";
                cmd << " >" << test.workPath + test.fileName;
            }

            string_q theCmd = "cd " + substitute(test.goldPath, "/api_tests", "") + " ; " + cmd.str();
            LOG4(theCmd);

            string_q customized = substitute(substitute(test.workPath, "working", "custom_config") + test.tool + "_" + test.filename + "/", "/api_tests", "");
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", saveAndCopy, NULL);
            int ret = system(theCmd.c_str());
            if (folderExists(customized))
                forEveryFileInFolder(customized + "/*", replaceFile, NULL);

            double thisTime = str_2_Double(TIC());
            totalTime += thisTime;
            string_q timeRep = (thisTime > tooSlow ? cRed : thisTime <= fastEnough ? cGreen : "") + double_2_Str(thisTime, 5) + cOff;

            string_q result = greenCheck;
            if (!ret) {
                if (endsWith(test.path, "lib"))
                    replace(test.workPath, "../", "");
                string_q newText = asciiFileToString(test.workPath + test.fileName);
                string_q oldText = asciiFileToString(test.goldPath + test.fileName);
                if (newText.empty() || newText != oldText)
                    result = redX;

            } else {
                result = redX;
            }

            if (!contains(test.origLine, " all,")) {
                reverse(test.filename);
                test.filename = substitute(padLeft(test.filename, 30).substr(0,30), " ", ".");
                reverse(test.filename);
                cerr << "   " << timeRep << " - " << (endsWith(test.path, "lib") ? padRight(test.tool, 16) : testName) << " ";
                cerr << trim(test.filename) << " " << result << "  " << trim(test.options).substr(0,90) << endl;
            }

            if (quit_on_error && (result == redX))
                return false;

            usleep(1000);

            if (shouldQuit()) {
                LOG4("Quitting because of shouldQuit");
                break;
            }
        }
    }
    cout << "   " << cTeal << double_2_Str(totalTime, 5) << " " << cOff << testName << " " << (cmdTests ? "cmd" : "api") << " total seconds" << endl << endl;

    return true;
}

//-----------------------------------------------------------------------
bool saveAndCopy(const string_q& customFile, void *data) {
    CStringArray parts;
    explode(parts, customFile, '/');
    string_q destFile = configPath(parts[parts.size()-1]);
    string_q saveFile = getCachePath("tmp/" + parts[parts.size()-1] + ".save");
    copyFile(destFile, saveFile);
    copyFile(customFile, destFile);
    return true;
}

//-----------------------------------------------------------------------
bool replaceFile(const string_q& customFile, void *data) {
    CStringArray parts;
    explode(parts, customFile, '/');
    string_q destFile = configPath(parts[parts.size()-1]);
    string_q saveFile = getCachePath("tmp/" + parts[parts.size()-1] + ".save");
    copyFile(saveFile, destFile);
    ::remove(saveFile.c_str());
    return true;
}

//-----------------------------------------------------------------------
double tooSlow = .4;
double fastEnough = .2;
