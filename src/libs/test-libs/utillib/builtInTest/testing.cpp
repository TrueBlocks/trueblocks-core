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
#include "options.h"

//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
    }

    return 0;
}
// CStringArray builtInCmds = {"verbose", "fmt",     "ether",  "output",  "raw",     "very_raw", "mocked",
//                             "wei",     "dollars", "parity", "version", "nocolor", "noop"};

// for (auto bi : builtInCmds) {
//     if (arg == ("-" + bi))
//         return true;
// }

// return false;
// }

// //--------------------------------------------------------------------------------
// bool COptionsBase::prepareArguments(int argCountIn, const char* argvIn[]) {
//     CStringArray separatedArgs_;
//     if (!prePrepareArguments(separatedArgs_, argCountIn, argvIn))  // returns false if readme is requested
//         return usage();

//     CStringArray argumentsIn;
//     for (auto arg : separatedArgs_) {
//         replace(arg, "--verbose", "-v");
//         while (!arg.empty()) {
//             if (startsWith(arg, "-") && !contains(arg, "--") && isBadSingleDash(arg))
//                 return invalid_option(arg + ". Did you mean -" + arg + "?");
//             string_q opt = expandOption(arg);  // handles case of -rf for example
//             if (opt == "-") {
//                 return usage("Raw '-' not supported.");
//             } else if (!opt.empty()) {
//                 argumentsIn.push_back(opt);  // args[nArgs++] = opt;
//             }
//         }
//     }
//     if (argumentsIn.size() < minArgs)  // the first arg is the program's name, so we use <=
//         return (argumentsIn.size() == 0 && !isApiMode()) ? usage("") : usage("Not enough arguments presented.");

//     //-----------------------------------------------------------------------------------
//     // Collapse commands that have 'permitted' sub options (i.e. colon ":" args)
//     //-----------------------------------------------------------------------------------
//     CStringArray argumentsOut;
//     for (size_t i = 0; i < argumentsIn.size(); i++) {
//         string_q arg = argumentsIn[i];
//         bool combine = false;
//         for (const auto& option : parameters) {
//             if (!option.permitted.empty()) {
//                 string_q hotKey = option.hotKey;
//                 string_q longName = option.longName;
//                 if (hotKey == arg || startsWith(longName, arg)) {
//                     // We want to pull the next parameter into this one since it's a ':' param
//                     combine = true;
//                 }
//             }
//         }

//         if (!combine && (arg == "-v" || arg == "-verbose" || arg == "--verbose")) {
//             if (i < argumentsIn.size() - 1) {
//                 uint64_t n = str_2_Uint(argumentsIn[i + 1]);
//                 if (n > 0 && n <= 10) {
//                     // We want to pull the next parameter into this one since it's a ':' param
//                     combine = true;
//                 }
//             }
//             if (!combine) {
//                 arg = "--verbose:1";
//             }
//         }

//         if (!combine && (arg == "-x" || arg == "--fmt")) {
//             if (i < argumentsIn.size() - 1)
//                 combine = true;
//         }

//         if (combine && i < (argumentsIn.size() - 1)) {
//             argumentsOut.push_back(arg + ":" + argumentsIn[i + 1]);
//             i++;
//         } else {
//             argumentsOut.push_back(arg);
//         }
//     }

//     //-----------------------------------------------------------------------------------
//     // We now have 'nArgs' command line arguments stored in the array 'args.'  We spin
//     // through them doing one of two things
//     //
//     // (1) handle any arguments common to all programs and remove them from the array
//     // (2) identify any --file arguments and store them for later use
//     //-----------------------------------------------------------------------------------
//     string_q cmdFileName = "";
//     for (uint64_t i = 0; i < argumentsOut.size(); i++) {
//         string_q arg = argumentsOut[i];
//         if (startsWith(arg, "--file:")) {
//             cmdFileName = substitute(arg, "--file:", "");
//             replace(cmdFileName, "~/", getHomeFolder());
//             if (!fileExists(cmdFileName)) {
//                 return usage("--file: '" + cmdFileName + "' not found.");
//             }

//         } else if (startsWith(arg, "-v:") || startsWith(arg, "--verbose:")) {
//             verbose = true;
//             arg = substitute(substitute(arg, "-v:", ""), "--verbose:", "");
//             if (!arg.empty()) {
//                 if (!isUnsigned(arg))
//                     return usage("Invalid verbose level '" + arg + "'.");
//                 verbose = str_2_Uint(arg);
//             }

//         } else if (startsWith(arg, "-x:") || startsWith(arg, "--fmt:")) {
//             arg = substitute(substitute(arg, "--fmt:", ""), "-x:", "");
//             if (arg == "txt") {
//                 expContext().exportFmt = TXT1;
//             } else if (arg == "csv") {
//                 expContext().exportFmt = CSV1;
//             } else if (arg == "yaml") {
//                 expContext().exportFmt = YAML1;
//             } else if (arg == "json") {
//                 expContext().exportFmt = JSON1;
//             } else if (arg == "api") {
//                 expContext().exportFmt = API1;
//             } else {
//                 return usage("Export format (" + arg + ") must be one of [ json | txt | csv | api ].");
//             }
//             argumentsOut[i] = "";
//         }
//     }

//     // remove empty arguments
//     CStringArray argumentsOut3;
//     for (auto arg : argumentsOut)
//         if (!arg.empty())
//             argumentsOut3.push_back(arg);

//     // If we have a command file, we will use it, if not we will creat one and pretend we have one.
//     string_q commandList = "";
//     for (auto arg : argumentsOut3) {
//         if (!contains(arg, "--file:"))
//             commandList += (arg + " ");
//     }
//     commandList += '\n';

//     if (!cmdFileName.empty()) {
//         string_q toAll;
//         if (!commandList.empty())
//             toAll = (" " + substitute(commandList, "\n", ""));
//         commandList = "";
//         // The command line also has a --file in it, so add these commands as well
//         string_q contents = substitute(asciiFileToString(cmdFileName), "\t", " ");
//         cleanString(contents, false);
//         if (contents.empty())
//             return usage("Command file '" + cmdFileName + "' is empty.");
//         if (startsWith(contents, "NOPARSE\n")) {
//             commandList = contents;
//             nextTokenClear(commandList, '\n');
//             commandList += toAll;
//         } else {
//             CStringArray lines;
//             explode(lines, contents, '\n');
//             for (auto command : lines) {
//                 while (contains(command, "--fmt  "))
//                     replace(command, "--fmt  ", "--fmt ");
//                 replace(command, "--fmt ", "--fmt:");
//                 if (!command.empty() && !startsWith(command, ";") && !startsWith(command, "#")) {  // ignore comments
//                     commandList += (command + toAll + "\n");
//                     if (isTestMode())
//                         cerr << "Cmd: " << command << toAll << endl;
//                 }
//             }
//         }
//     }
//     //        commandList += stdInCmds;
//     explode(commandLines, commandList, '\n');
//     for (auto& item : commandLines)
//         item = trim(item);
//     if (commandLines.empty())
//         commandLines.push_back("--noop");

//     return true;
// }

// //---------------------------------------------------------------------------------------
// static const char* CHR_VALID_NAME = "\t\n\r()<>[]{}`|;'!$^*~@?&#+%,:=\"";
// //---------------------------------------------------------------------------------------
// bool isValidName(const string_q& fn) {
//     if (fn.empty() || isdigit(fn[0]))
//         return false;
//     string_q test = fn;
//     replaceAny(test, CHR_VALID_NAME, "");
//     return test == fn;
// }

// //--------------------------------------------------------------------------------
// bool COptionsBase::standardOptions(string_q& cmdLine) {
//     if (contains(cmdLine, "--to_file")) {
//         ostringstream rep;
//         rep << "--output:"
//             << "/tmp/" + makeValidName(Now().Format(FMT_EXPORT));
//         rep << (expContext().exportFmt == CSV1    ? ".csv"
//                 : expContext().exportFmt == TXT1  ? ".txt"
//                 : expContext().exportFmt == YAML1 ? ".yaml"
//                                                   : ".json");
//         replaceAll(cmdLine, "--to_file", rep.str());
//     }

//     // Note: check each item individual in case more than one appears on the command line
//     cmdLine += " ";
//     replace(cmdLine, "--output ", "--output:");

//     // noop switch
//     if (contains(cmdLine, "--noop ")) {
//         // do nothing
//         replaceAll(cmdLine, "--noop ", "");
//     }

//     // noop flag
//     while (contains(cmdLine, "--noop:")) {
//         replace(cmdLine, "--noop", "|");
//         string_q start = nextTokenClear(cmdLine, '|');
//         nextTokenClear(cmdLine, ' ');
//         cmdLine = start + trim(cmdLine);
//         // do nothing
//     }
//     if (!endsWith(cmdLine, ' '))
//         cmdLine += " ";

//     if (contains(cmdLine, "--version ")) {
//         cout << getProgName() << " " << getVersionStr() << "\n";
//         return false;
//     }

//     if (contains(cmdLine, "--nocolor ")) {
//         replaceAll(cmdLine, "--nocolor ", "");
//         colorsOff();
//     }

//     if (contains(cmdLine, "--no_header ")) {
//         replaceAll(cmdLine, "--no_header ", "");
//         noHeader = true;
//     }

//     if (isEnabled(OPT_HELP) && (contains(cmdLine, "-h ") || contains(cmdLine, "--help "))) {
//         usage();
//         return false;
//     }

//     if (isEnabled(OPT_ETHER) && contains(cmdLine, "--ether ")) {
//         replaceAll(cmdLine, "--ether ", "");
//         expContext().asEther = true;
//         expContext().asDollars = false;
//         expContext().asWei = false;
//     }

//     if (isEnabled(OPT_RAW) && contains(cmdLine, "--very_raw ")) {
//         replaceAll(cmdLine, "--very_raw ", "");
//         setenv("NO_SCHEMAS", "true", true);
//         isRaw = true;
//         isVeryRaw = true;
//     }

//     if (isEnabled(OPT_RAW) && contains(cmdLine, "--raw ")) {
//         replaceAll(cmdLine, "--raw ", "");
//         setenv("NO_SCHEMAS", "true", true);
//         isRaw = true;
//     }

//     if (isEnabled(OPT_CRUD)) {
//         CStringArray validCruds;
//         validCruds.push_back("--create ");
//         validCruds.push_back("--update ");
//         validCruds.push_back("--delete ");
//         validCruds.push_back("--undelete ");
//         validCruds.push_back("--remove ");
//         for (const string_q& cmd : validCruds) {
//             if (contains(cmdLine, cmd)) {
//                 replaceAll(cmdLine, cmd, "");
//                 crudCommands.push_back(trim(substitute(cmd, "--", "")));
//             }
//         }
//     }

//     if (isEnabled(OPT_MOCKDATA) && contains(cmdLine, "--mocked ")) {
//         replaceAll(cmdLine, "--mocked ", "");
//         mocked = true;
//     }

//     if (isEnabled(OPT_OUTPUT) && contains(cmdLine, "--output:")) {
//         closeRedirect();  // close the current one in case it's open (--file for example)
//         string_q temp = substitute(cmdLine, "--output:", "|");
//         nextTokenClear(temp, '|');
//         temp = nextTokenClear(temp, ' ');
//         if (temp.empty())
//             return usage("Please provide a filename for the --output option.");
//         if (!isValidName(temp))
//             return usage("Please provide a valid filename (" + temp + ") for the --output option.");
//         rd_zipOnClose = endsWith(temp, ".gz");
//         replace(temp, ".gz", "");
//         CFilename fn(temp);
//         establishFolder(fn.getPath());
//         if (!folderExists(fn.getPath()))
//             return usage("Output file path not found and could not be created: '" + fn.getPath() + "'.");
//         rd_outputFilename = fn.getFullPath();
//         outputStream.open(rd_outputFilename.c_str());
//         if (outputStream.is_open()) {
//             coutSaved = cout.rdbuf();          // back up cout's streambuf
//             cout.rdbuf(outputStream.rdbuf());  // assign streambuf to cout
//         } else {
//             return usage("Could not open output stream at '" + rd_outputFilename + ".");
//         }
//     }

//     if (isEnabled(OPT_WEI) && contains(cmdLine, "--wei ")) {
//         replaceAll(cmdLine, "--wei ", "");
//         expContext().asEther = false;
//         expContext().asDollars = false;
//         expContext().asWei = true;
//     }

//     if (isEnabled(OPT_DOLLARS) && contains(cmdLine, "--dollars ")) {
//         replaceAll(cmdLine, "--dollars ", "");
//         expContext().asEther = false;
//         expContext().asDollars = true;
//         expContext().asWei = false;
//     }

//     if (isEnabled(OPT_PARITY) && contains(cmdLine, "--parity ")) {
//         replaceAll(cmdLine, "--parity ", "");
//         expContext().spcs = 2;
//         expContext().hexNums = true;
//         expContext().quoteNums = true;
//         expContext().asParity = true;
//     }

//     cmdLine = trim(cmdLine);
//     return true;
// }

// //--------------------------------------------------------------------------------
// bool COptionsBase::builtInCmd(const string_q& arg) {
//     if (isEnabled(OPT_HELP) && (arg == "-h" || arg == "--help"))
//         return true;

//     if (isEnabled(OPT_VERBOSE)) {
//         if (startsWith(arg, "-v:") || startsWith(arg, "--verbose:"))
//             return true;
//     }

//     if (isEnabled(OPT_FMT)) {
//         if (startsWith(arg, "-x:") || startsWith(arg, "--fmt:"))
//             return true;
//     }

//     if (isEnabled(OPT_ETHER) && arg == "--ether")
//         return true;
//     if (isEnabled(OPT_OUTPUT) && startsWith(arg, "--output:"))
//         return true;
//     if (isEnabled(OPT_RAW) && (arg == "--raw" || arg == "--very_raw"))
//         return true;
//     if (isEnabled(OPT_MOCKDATA) && arg == "--mocked")
//         return true;
//     if (isEnabled(OPT_WEI) && arg == "--wei")
//         return true;
//     if (isEnabled(OPT_DOLLARS) && arg == "--dollars")
//         return true;
//     if (isEnabled(OPT_PARITY) && (arg == "--parity"))
//         return true;
//     if (arg == "--version")
//         return true;
//     if (arg == "--nocolor")
//         return true;
//     if (arg == "--noop")
//         return true;
//     return false;
// }
