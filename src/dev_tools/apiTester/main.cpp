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

        for (auto test : options.tests) {
            if (shouldQuit())
                continue;
            if (options.cleanTests)  options.cleanTest(test);
            if (options.which & CMD) options.doTest(test, true);
            if (options.which & API) options.doTest(test, false);
        }
    }
    return 0;
}

extern bool saveAndCopy(const string_q& path, void *data);
extern bool replaceFile(const string_q& path, void *data);
double tooSlow = .4;
double fastEnough = .2;
//-----------------------------------------------------------------------
bool COptions::doTest(const string_q& testName, bool cmdTests) {

    cerr << "Testing " << testName << " (" << (cmdTests ? "cmd" : "api") << " mode):" << endl;

    resetClock();
    double totalTime = 0;

    string_q testFile = "./" + testName + ".csv";
    if (!fileExists(testFile))
        testFile = getCWD() + "../../../../src/other/build_assets/testCases/" + testName + ".csv";
    if (!fileExists(testFile))
        return usage("Cannot find test file " + testFile + ". Quitting.");

    string_q contents = asciiFileToString(testFile) + "\n";
    replaceAll(contents, "\n\n", "\n");

    CStringArray lines;
    explode(lines, contents, '\n');

    for (auto line : lines) {
        if (shouldQuit())
            continue;

        string_q testMode = (cmdTests ? "cmd" : "api");

        bool ignore1 = startsWith(line, "#");
        bool ignore2 = !ignoreOff && startsWith(line, "off");
        bool ignore3 = contains(line, ", route,");
        bool ignore4 = false;
        if (!ignore3 && !speed_filter.empty()) {
                 if (contains(line, " all,")) { /* do nothing */ }
            else if (speed_filter == "fast" ) ignore4 = !contains(line, "fast,");
            else if (speed_filter == "slow" ) ignore4 = !contains(line, "slow,");
            else if (speed_filter == "medi" ) ignore4 = !contains(line, "medi,");
        }

        if (ignore2 || ignore4) {
            if (testMode == "cmd")
                cerr << iBlue << "   # " << line << cOff << endl;
            cerr.flush();

        } else if (ignore1 || ignore3 || line.empty()) {
            // do nothing it's a comment

        } else {
            CStringArray parts;
            explode(parts, line, ',');
            string_q onOff    = trim(parts[0]);
            string_q mode     = trim(parts[1]);
            string_q speed    = trim(parts[2]);
            string_q route    = trim(parts[3]);
            string_q tool     = trim(parts[4]);
            string_q filename = trim(parts[5]);
            string_q post     = parts.size() > 6 ? trim(parts[6]) : "";
            string_q options  = parts.size() > 7 ? trim(parts[7]) : "";
            string_q optTool  = parts.size() > 8 ? trim(parts[8]) : "";
            string_q path     = nextTokenClear(tool, '/');
            if ((!cmdTests && mode == "cmd") || (cmdTests && mode == "api"))
                continue;

            string_q goldPath = "../../../gold/" + path + "/" + tool + (!cmdTests ? "/api_tests/" : "/");
            string_q filePath = substitute(goldPath, "/gold/", "/working/");
            string_q fileName = tool + "_" + filename + ".txt";
            string_q removePath = filePath + fileName;
            if (fileExists(removePath))
                ::remove(removePath.c_str());

            if (!optTool.empty())
                tool = optTool;

            ostringstream cmd;

            replaceAll(options, " = ", "=");
            replaceAll(options, " & ", "&");
            replaceAll(options, " @ ", "@");
            replaceAll(post, "n", "");
            replaceAll(post, "y", "jq");
            if (!cmdTests) {
                replaceAll(options, "@", "");
                replaceAll(options, " ", "%20");
                cmd << "curl -s \"http:/""/localhost:8080/" << route;
                if (!options.empty())
                    cmd << "?" << options;
                cmd << "\"";
                if (!post.empty())
                    cmd << " | " <<  post << " ";
                cmd << " >" << filePath + fileName;

            } else {
                options = "&" + options;
                replaceAll(options, "&addr_list=", " ");
                replaceAll(options, "&block_list=", " ");
                replaceAll(options, "&date_list=", " ");
                replaceAll(options, "&trans_list=", " ");
                replaceAll(options, "&term_list=", " ");
                replaceAll(options, "%20", " ");
                replaceAll(options, "@", " -");
                replaceAll(options, "&", " --");
                replaceAll(options, "\\*", " \"*\"");
                replaceAll(options, "=", " ");
                if (trim(options) == "--" || startsWith(trim(options), "-- "))
                    replace(options, "--", "");
                string_q c = tool + options + " >" + filePath + fileName + " 2>&1";
                cmd << "TEST_MODE=true NO_COLOR=true REDIR_CERR=true " << c;
            }

            string_q theCmd = "cd ../../../gold/" + path + "/" + tool + " ; " + cmd.str();
            // cerr << "Calling " << theCmd << endl;

            string_q customized = substitute(substitute(filePath, "working", "custom_config") + tool + "_" + filename + "/", "/api_tests", "");
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
                string_q newText = asciiFileToString(filePath + fileName);
                string_q oldText = asciiFileToString(goldPath + fileName);
                if (newText.empty() || newText != oldText)
                    result = redX;
            } else {
                result = redX;
            }

            if (!contains(line, " all,")) {
                reverse(filename);
                filename = substitute(padLeft(filename, 30).substr(0,30), " ", ".");
                reverse(filename);
                cerr << "   " << timeRep << " - " << testName << " ";
                cerr << trim(filename) << " " << trim(options).substr(0,90) << " " << result << endl;
            }

            if (quit_on_error && (result == redX))
                return false;

            usleep(1000);
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
bool COptions::cleanTest(const string_q& testName) {
    ostringstream os;
    os << "find ../../../working/tools/" << testName << "/ -depth 1 -name \"get*.txt\" -exec rm '{}' ';' ; ";
    os << "find ../../../working/tools/" << testName << "/ -depth 1 -name \"eth*.txt\" -exec rm '{}' ';' ; ";
    os << "find ../../../working/tools/" << testName << "/ -depth 1 -name \"grab*.txt\" -exec rm '{}' ';' ; ";
    os << "find ../../../working/tools/" << testName << "/ -depth 1 -name \"*Block*.txt\" -exec rm '{}' ';' ; ";
    system(os.str().c_str());
    return true;
}
