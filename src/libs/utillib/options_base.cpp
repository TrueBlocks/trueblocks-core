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
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "basetypes.h"
#include "database.h"
#include "exportcontext.h"
#include "options_base.h"
#include "colors.h"
#include "filenames.h"
#include "accountname.h"
#include "rpcresult.h"

namespace qblocks {

//--------------------------------------------------------------------------------
void COptionsBase::registerOptions(size_t nP, COption const* pP) {
    // TODO(tjayrush): global data
    arguments.clear();
    cntParams = nP;
    pParams = pP;
}

//--------------------------------------------------------------------------------
// TODO(tjayrush): global data - but okay, a program only has one name
string_q COptionsBase::g_progName = "trueBlocks";

//--------------------------------------------------------------------------------
void COptionsBase::setProgName(const string_q& name) {
    g_progName = name;
}

//--------------------------------------------------------------------------------
string_q COptionsBase::getProgName(void) const {
    return g_progName;
}

//--------------------------------------------------------------------------------
bool COptionsBase::prePrepareArguments(CStringArray& separatedArgs_, int argCountIn, const char* argvIn[]) {
    if (argCountIn > 0)  // always is, but check anyway
        COptionsBase::g_progName = CFilename(argvIn[0]).getFilename();
    if (!getEnvStr("PROG_NAME").empty())
        COptionsBase::g_progName = getEnvStr("PROG_NAME");
    if (getEnvStr("NO_COLOR") == "true" || (getProgName() != "testRunner" && !isatty(STDOUT_FILENO)))
        colorsOff();
    if (getEnvStr("REDIR_CERR") == "true")
        cerr.rdbuf(cout.rdbuf());

    // We allow users to add 'true' or 'false' to boolean options, but the following code works by the
    // presence or absense of the boolean key, so here we spin through, removing 'true' and 'false' and
    // removing the key if we find 'false'
    CStringArray cleaned_;
    for (int i = 1; i < argCountIn; i++) {
        if (toLower(argvIn[i]) == "true") {
            // don't put this in, but leave the key
        } else if (toLower(argvIn[i]) == "false") {
            // remove the key
            if (cleaned_.size())
                cleaned_.pop_back();
        } else {
            // add this arg
            cleaned_.push_back(argvIn[i]);
        }
    }

    for (auto item : cleaned_) {
        CStringArray parts;
        string_q arg = substitute(substituteAny(item, "\n\r\t", " "), ",", " ");
        explode(parts, arg, ' ');
        for (auto part : parts) {
            if (!part.empty()) {
                separatedArgs_.push_back(part);
            }
        }
    }

    if (isTestMode()) {
        size_t cnt = 0;
        ostringstream os;
        cerr << getProgName() << " argc: " << (separatedArgs_.size() + 1) << " ";
        for (auto arg : separatedArgs_) {
            cerr << "[" << ++cnt << ":" << trim(arg) << "] ";
            os << trim(arg) << " ";
        }
        cerr << endl;
        cerr << getProgName() << " " << os.str() << endl;
        CStringArray envs = {
            "API_MODE", "DOCKER_MODE", "PROG_NAME", "HIDE_NAMES", "LIVE_TEST",
            // "FRESHEN_FLAG S", "IPFS_PATH",
            "SILENCE", "NO_CACHE", "NO_PROGRESS", "NO_SCHEMAS", "TB_NAME_ADDRESS", "TB_NAME_CUSTOM", "TB_NAME_DECIMALS",
            "TB_NAME_DESCR", "TB_NAME_NAME", "TB_NAME_SOURCE", "TB_NAME_SYMBOL", "TB_NAME_TAG",
            // "TEST_MODE", "NO_COLOR", "REDIR_CERR", "EDITOR",
        };
        for (auto key : envs) {
            string_q val = getEnvStr(key);
            if (!val.empty())
                cerr << key << " = [" << val << "]" << endl;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
bool COptionsBase::isBadSingleDash(const string_q& arg) const {
    for (size_t j = 0; j < cntParams; j++) {
        string_q cmd = substitute(arg, "-", "");
        if (cmd == pParams[j].longName)
            return true;
    }

    CStringArray builtInCmds = {"verbose", "fmt",     "ether",  "output",  "raw",     "very_raw", "mocked",
                                "wei",     "dollars", "parity", "version", "nocolor", "noop"};

    for (auto bi : builtInCmds) {
        if (arg == ("-" + bi))
            return true;
    }

    return false;
}

//--------------------------------------------------------------------------------
bool COptionsBase::prepareArguments(int argCountIn, const char* argvIn[]) {
    CStringArray separatedArgs_;
    prePrepareArguments(separatedArgs_, argCountIn, argvIn);

    CStringArray argumentsIn;
    for (auto arg : separatedArgs_) {
        replace(arg, "--verbose", "-v");
        while (!arg.empty()) {
            if (startsWith(arg, "-") && !contains(arg, "--") && isBadSingleDash(arg))
                return invalid_option(arg + ". Did you mean -" + arg + "?");
            string_q opt = expandOption(arg);  // handles case of -rf for example
            if (isReadme && isEnabled(OPT_HELP))
                return usage();
            if (opt == "-") {
                return usage("Raw '-' not supported.");
            } else if (!opt.empty()) {
                argumentsIn.push_back(opt);  // args[nArgs++] = opt;
            }
        }
    }
    if (argumentsIn.size() < minArgs)  // the first arg is the program's name, so we use <=
        return (argumentsIn.size() == 0 && !isApiMode()) ? usage("") : usage("Not enough arguments presented.");

    //        string_q stdInCmds;
    //        if (hasStdIn) {
    //            // reading from stdin, expect only a list of addresses, one per line.
    //            char c = static_cast<char>(getchar());
    //            while (c != EOF) {
    //                stdInCmds += c;
    //                c = static_cast<char>(getchar());
    //            }
    //            if (!endsWith(stdInCmds, "\n"))
    //                stdInCmds += "\n";
    //        }

    //-----------------------------------------------------------------------------------
    // Collapse commands that have 'permitted' sub options (i.e. colon ":" args)
    //-----------------------------------------------------------------------------------
    CStringArray argumentsOut;
    for (size_t i = 0; i < argumentsIn.size(); i++) {
        string_q arg = argumentsIn[i];
        bool combine = false;
        for (size_t j = 0; j < cntParams && !combine; j++) {
            if (!pParams[j].permitted.empty()) {
                string_q hotKey = pParams[j].hotKey;
                string_q longName = pParams[j].longName;
                if (hotKey == arg || startsWith(longName, arg)) {
                    // We want to pull the next parameter into this one since it's a ':' param
                    combine = true;
                }
            }
        }

        if (!combine && (arg == "-v" || arg == "-verbose" || arg == "--verbose")) {
            if (i < argumentsIn.size() - 1) {
                uint64_t n = str_2_Uint(argumentsIn[i + 1]);
                if (n > 0 && n <= 10) {
                    // We want to pull the next parameter into this one since it's a ':' param
                    combine = true;
                }
            }
            if (!combine) {
                arg = "--verbose:1";
            }
        }

        if (!combine && (arg == "-x" || arg == "--fmt")) {
            if (i < argumentsIn.size() - 1)
                combine = true;
        }

        if (combine && i < (argumentsIn.size() - 1)) {
            argumentsOut.push_back(arg + ":" + argumentsIn[i + 1]);
            i++;
        } else {
            argumentsOut.push_back(arg);
        }
    }

    //-----------------------------------------------------------------------------------
    // We now have 'nArgs' command line arguments stored in the array 'args.'  We spin
    // through them doing one of two things
    //
    // (1) handle any arguments common to all programs and remove them from the array
    // (2) identify any --file arguments and store them for later use
    //-----------------------------------------------------------------------------------
    string_q cmdFileName = "";
    for (uint64_t i = 0; i < argumentsOut.size(); i++) {
        string_q arg = argumentsOut[i];
        if (startsWith(arg, "--file:")) {
            cmdFileName = substitute(arg, "--file:", "");
            replace(cmdFileName, "~/", getHomeFolder());
            if (!fileExists(cmdFileName)) {
                return usage("--file: '" + cmdFileName + "' not found.");
            }

        } else if (startsWith(arg, "-v:") || startsWith(arg, "--verbose:")) {
            verbose = true;
            arg = substitute(substitute(arg, "-v:", ""), "--verbose:", "");
            if (!arg.empty()) {
                if (!isUnsigned(arg))
                    return usage("Invalid verbose level '" + arg + "'.");
                verbose = str_2_Uint(arg);
            }

        } else if (startsWith(arg, "-x:") || startsWith(arg, "--fmt:")) {
            arg = substitute(substitute(arg, "--fmt:", ""), "-x:", "");
            if (arg == "txt") {
                expContext().exportFmt = TXT1;
            } else if (arg == "csv") {
                expContext().exportFmt = CSV1;
            } else if (arg == "yaml") {
                expContext().exportFmt = YAML1;
            } else if (arg == "json") {
                expContext().exportFmt = JSON1;
            } else if (arg == "api") {
                expContext().exportFmt = API1;
            } else {
                return usage("Export format (" + arg + ") must be one of [ json | txt | csv | api ].");
            }
            argumentsOut[i] = "";
        }
    }

    // remove empty arguments
    CStringArray argumentsOut3;
    for (auto arg : argumentsOut)
        if (!arg.empty())
            argumentsOut3.push_back(arg);

    // If we have a command file, we will use it, if not we will creat one and pretend we have one.
    string_q commandList = "";
    for (auto arg : argumentsOut3) {
        if (!contains(arg, "--file:"))
            commandList += (arg + " ");
    }
    commandList += '\n';

    if (!cmdFileName.empty()) {
        string_q toAll;
        if (!commandList.empty())
            toAll = (" " + substitute(commandList, "\n", ""));
        commandList = "";
        // The command line also has a --file in it, so add these commands as well
        string_q contents = substitute(asciiFileToString(cmdFileName), "\t", " ");
        cleanString(contents, false);
        if (contents.empty())
            return usage("Command file '" + cmdFileName + "' is empty.");
        if (startsWith(contents, "NOPARSE\n")) {
            commandList = contents;
            nextTokenClear(commandList, '\n');
            commandList += toAll;
        } else {
            CStringArray lines;
            explode(lines, contents, '\n');
            for (auto command : lines) {
                while (contains(command, "--fmt  "))
                    replace(command, "--fmt  ", "--fmt ");
                replace(command, "--fmt ", "--fmt:");
                if (!command.empty() && !startsWith(command, ";") && !startsWith(command, "#")) {  // ignore comments
                    commandList += (command + toAll + "\n");
                }
            }
        }
    }
    //        commandList += stdInCmds;
    explode(commandLines, commandList, '\n');
    for (auto& item : commandLines)
        item = trim(item);
    if (commandLines.empty())
        commandLines.push_back("--noop");

    return true;
}

//---------------------------------------------------------------------------------------
static const char* CHR_VALID_NAME = "\t\n\r()<>[]{}`|;'!$^*~@?&#+%,:=\"";
//---------------------------------------------------------------------------------------
bool isValidName(const string_q& fn) {
    if (fn.empty() || isdigit(fn[0]))
        return false;
    string_q test = fn;
    replaceAny(test, CHR_VALID_NAME, "");
    return test == fn;
}

//--------------------------------------------------------------------------------
bool COptionsBase::standardOptions(string_q& cmdLine) {
    if (contains(cmdLine, "--to_file")) {
        ostringstream rep;
        rep << "--output:"
            << "/tmp/" + makeValidName(Now().Format(FMT_EXPORT));
        rep << (expContext().exportFmt == CSV1    ? ".csv"
                : expContext().exportFmt == TXT1  ? ".txt"
                : expContext().exportFmt == YAML1 ? ".yaml"
                                                  : ".json");
        replaceAll(cmdLine, "--to_file", rep.str());
    }

    // Note: check each item individual in case more than one appears on the command line
    cmdLine += " ";
    replace(cmdLine, "--output ", "--output:");

    // noop switch
    if (contains(cmdLine, "--noop ")) {
        // do nothing
        replaceAll(cmdLine, "--noop ", "");
    }

    // noop flag
    while (contains(cmdLine, "--noop:")) {
        replace(cmdLine, "--noop", "|");
        string_q start = nextTokenClear(cmdLine, '|');
        nextTokenClear(cmdLine, ' ');
        cmdLine = start + trim(cmdLine);
        // do nothing
    }
    if (!endsWith(cmdLine, ' '))
        cmdLine += " ";

    if (contains(cmdLine, "--version ")) {
        cout << getProgName() << " " << getVersionStr() << "\n";
        return false;
    }

    if (contains(cmdLine, "--nocolor ")) {
        replaceAll(cmdLine, "--nocolor ", "");
        colorsOff();
    }

    if (contains(cmdLine, "--no_header ")) {
        replaceAll(cmdLine, "--no_header ", "");
        noHeader = true;
    }

    if (isEnabled(OPT_HELP) && (contains(cmdLine, "-h ") || contains(cmdLine, "--help "))) {
        usage();
        return false;
    }

    if (isEnabled(OPT_ETHER) && contains(cmdLine, "--ether ")) {
        replaceAll(cmdLine, "--ether ", "");
        expContext().asEther = true;
        expContext().asDollars = false;
        expContext().asWei = false;
    }

    if (isEnabled(OPT_RAW) && contains(cmdLine, "--very_raw ")) {
        replaceAll(cmdLine, "--very_raw ", "");
        setenv("NO_SCHEMAS", "true", true);
        isRaw = true;
        isVeryRaw = true;
    }

    if (isEnabled(OPT_RAW) && contains(cmdLine, "--raw ")) {
        replaceAll(cmdLine, "--raw ", "");
        setenv("NO_SCHEMAS", "true", true);
        isRaw = true;
    }

    if (isEnabled(OPT_CRUD)) {
        CStringArray validCruds;
        validCruds.push_back("--create ");
        validCruds.push_back("--update ");
        validCruds.push_back("--delete ");
        validCruds.push_back("--undelete ");
        validCruds.push_back("--remove ");
        for (const string_q& cmd : validCruds) {
            if (contains(cmdLine, cmd)) {
                replaceAll(cmdLine, cmd, "");
                crudCommands.push_back(trim(substitute(cmd, "--", "")));
            }
        }
    }

    if (isEnabled(OPT_MOCKDATA) && contains(cmdLine, "--mocked ")) {
        replaceAll(cmdLine, "--mocked ", "");
        mocked = true;
    }

    if (isEnabled(OPT_OUTPUT) && contains(cmdLine, "--output:")) {
        closeRedirect();  // close the current one in case it's open (--file for example)
        string_q temp = substitute(cmdLine, "--output:", "|");
        nextTokenClear(temp, '|');
        temp = nextTokenClear(temp, ' ');
        if (temp.empty())
            return usage("Please provide a filename for the --output option.");
        if (!isValidName(temp))
            return usage("Please provide a valid filename (" + temp + ") for the --output option.");
        rd_zipOnClose = endsWith(temp, ".gz");
        replace(temp, ".gz", "");
        CFilename fn(temp);
        establishFolder(fn.getPath());
        if (!folderExists(fn.getPath()))
            return usage("Output file path not found and could not be created: '" + fn.getPath() + "'.");
        rd_outputFilename = fn.getFullPath();
        outputStream.open(rd_outputFilename.c_str());
        if (outputStream.is_open()) {
            coutSaved = cout.rdbuf();          // back up cout's streambuf
            cout.rdbuf(outputStream.rdbuf());  // assign streambuf to cout
        } else {
            return usage("Could not open output stream at '" + rd_outputFilename + ".");
        }
    }

    if (isEnabled(OPT_WEI) && contains(cmdLine, "--wei ")) {
        replaceAll(cmdLine, "--wei ", "");
        expContext().asEther = false;
        expContext().asDollars = false;
        expContext().asWei = true;
    }

    if (isEnabled(OPT_DOLLARS) && contains(cmdLine, "--dollars ")) {
        replaceAll(cmdLine, "--dollars ", "");
        expContext().asEther = false;
        expContext().asDollars = true;
        expContext().asWei = false;
    }

    if (isEnabled(OPT_PARITY) && contains(cmdLine, "--parity ")) {
        replaceAll(cmdLine, "--parity ", "");
        expContext().spcs = 2;
        expContext().hexNums = true;
        expContext().quoteNums = true;
        expContext().asParity = true;
    }

    cmdLine = trim(cmdLine);
    return true;
}

//--------------------------------------------------------------------------------
bool COptionsBase::builtInCmd(const string_q& arg) {
    if (isEnabled(OPT_HELP) && (arg == "-h" || arg == "--help"))
        return true;

    if (isEnabled(OPT_VERBOSE)) {
        if (startsWith(arg, "-v:") || startsWith(arg, "--verbose:"))
            return true;
    }

    if (isEnabled(OPT_FMT)) {
        if (startsWith(arg, "-x:") || startsWith(arg, "--fmt:"))
            return true;
    }

    if (isEnabled(OPT_ETHER) && arg == "--ether")
        return true;
    if (isEnabled(OPT_OUTPUT) && startsWith(arg, "--output:"))
        return true;
    if (isEnabled(OPT_RAW) && (arg == "--raw" || arg == "--very_raw"))
        return true;
    if (isEnabled(OPT_MOCKDATA) && arg == "--mocked")
        return true;
    if (isEnabled(OPT_WEI) && arg == "--wei")
        return true;
    if (isEnabled(OPT_DOLLARS) && arg == "--dollars")
        return true;
    if (isEnabled(OPT_PARITY) && (arg == "--parity"))
        return true;
    if (arg == "--version")
        return true;
    if (arg == "--nocolor")
        return true;
    if (arg == "--noop")
        return true;
    return false;
}

//-----------------------------------------------------------------------
void COptionsBase::configureDisplay(const string_q& tool, const string_q& dataType, const string_q& defFormat,
                                    const string_q& meta) {
    string_q format;
    switch (expContext().exportFmt) {
        case NONE1:
            format = defFormat;
            manageFields(dataType + ":" + cleanFmt(format));
            break;
        case TXT1:
        case CSV1:
            if (isTestMode()) {
                // Just warning the user as if this is set it may break test cases
                string test = getGlobalConfig(tool)->getConfigStr("display", "format", "<not-set>");
                if (test != "<not-set>")
                    LOG_WARN("Tests will fail. Custom display string set to: ", test);
            }
            format = getGlobalConfig(tool)->getConfigStr("display", "format", format.empty() ? defFormat : format);
            manageFields(dataType + ":" + cleanFmt((format.empty() ? defFormat : format)));
            break;
        case YAML1:
        case API1:
        case JSON1:
            format = "";
            break;
    }
    if (expContext().asEther)
        format = substitute(format, "{BALANCE}", "{ETHER}");
    if (expContext().asDollars)
        format = substitute(format, "{BALANCE}", "{DOLLARS}");
    expContext().fmtMap["meta"] = meta;
    expContext().fmtMap["format"] = cleanFmt(format);
    expContext().fmtMap["header"] = cleanFmt(format);
    if (noHeader)
        expContext().fmtMap["header"] = "";
}

//---------------------------------------------------------------------------------------------------
bool COptionsBase::confirmUint(const string_q& name, uint64_t& value, const string_q& argIn) const {
    value = NOPOS;

    const COption* param = findParam(name);
    if (!param)
        return usage("Unknown parameter `" + name + "'.");
    if (!contains(param->type, "uint") && !contains(param->type, "blknum"))
        return true;

    string_q arg = argIn;
    replace(arg, param->hotKey + ":", "");
    replace(arg, name + ":", "");
    replaceAll(arg, "-", "");

    if (!isNumeral(arg))
        return usage("Value to --" + name + " parameter (" + arg + ") must be a valid unsigned integer.");
    value = str_2_Uint(arg);
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptionsBase::confirmUint(const string_q& name, uint32_t& value, const string_q& arg) const {
    value = (uint32_t)NOPOS;
    uint64_t temp;
    if (!confirmUint(name, temp, arg))
        return false;
    value = (uint32_t)temp;
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptionsBase::confirmDouble(const string_q& name, double& value, const string_q& argIn) const {
    value = NOPOS;

    const COption* param = findParam(name);
    if (!param)
        return usage("Unknown parameter `" + name + "'.");
    if (!contains(param->type, "double"))
        return true;

    string_q arg = argIn;
    replace(arg, param->hotKey + ":", "");
    replace(arg, name + ":", "");
    replaceAll(arg, "-", "");

    if (!isDouble(arg))
        return usage("Value to --" + name + " parameter (" + arg + ") must be a valid double.");
    value = str_2_Double(arg);
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptionsBase::confirmBlockNum(const string_q& name, blknum_t& value, const string_q& argIn,
                                   blknum_t latest) const {
    value = NOPOS;

    const COption* param = findParam(name);
    if (!param)
        return usage("Unknown parameter `" + name + "'.");
    if (!contains(param->type, "uint") && !contains(param->type, "blknum"))
        return true;

    string_q arg = argIn;
    replace(arg, param->hotKey + ":", "");
    replace(arg, name + ":", "");
    replaceAll(arg, "-", "");

    if (contains(param->type, "blknum")) {
        if (arg == "first") {
            value = 0;
            return true;
        }
        if (arg == "latest") {
            value = latest;
            return true;
        }
    }

    if (!confirmUint(name, value, argIn))
        return false;

    if (value > latest)
        return usage("Block number (" + argIn + ") is greater than the latest block.");

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptionsBase::confirmEnum(const string_q& name, string_q& value, const string_q& argIn) const {
    const COption* param = findParam(name);
    if (!param)
        return usage("Unknown parameter `" + name + "'.");
    if (param->type.empty() || !contains(param->type, "enum["))
        return true;

    string_q type = param->type;
    replace(type, "*", "");
    replace(type, "enum", "");
    replace(type, "list<", "");
    replace(type, ">", "");
    replace(type, "[", "|");
    replace(type, "]", "|");

    string_q arg = argIn;
    replace(arg, param->hotKey + ":", "");
    replace(arg, name + ":", "");
    replaceAll(arg, "-", "");

    if (!contains(type, "|" + arg + "|")) {
        string_q desc = substitute(substitute(param->description, ", one ", "| One "), "*", "");
        nextTokenClear(desc, '|');
        return usage("Invalid option '" + arg + "' for '" + name + "'." + desc + " required.");
    }

    value = arg;
    return true;
}

//---------------------------------------------------------------------------------------------------
const COption* COptionsBase::findParam(const string_q& name) const {
    for (size_t i = 0; i < cntParams; i++) {
        if (startsWith(pParams[i].longName, "--" + name))  // flags, toggles, switches
            return &pParams[i];
        if (startsWith(pParams[i].longName, name))  // positionals
            return &pParams[i];
    }
    return NULL;
}

//--------------------------------------------------------------------------------
COption::COption(const string_q& ln, const string_q& sn, const string_q& t, size_t opts, const string_q& d) {
    description = substitute(d, "&#44;", ",");
    if (ln.empty())
        return;

    is_positional = (opts & OPT_POSITIONAL);
    is_hidden = (opts & OPT_HIDDEN);
    is_optional = !(opts & OPT_REQUIRED);
    is_deprecated = (opts == OPT_DEPRECATED);

    type = t;
    permitted = t;
    permitted = substitute(permitted, "<uint32>", "<num>");
    permitted = substitute(permitted, "<uint64>", "<num>");
    permitted = substitute(permitted, "<blknum>", "<num>");
    permitted = substitute(permitted, "<string>", "<str>");
    permitted = substitute(permitted, "list<topic>", "<hash>");
    permitted = substitute(permitted, "list<addr>", "<addr>");
    if (contains(type, "enum")) {
        description += ", one [X] of " + substitute(substitute(substitute(type, "list<", ""), ">", ""), "enum", "");
        replace(description, " [X]", (contains(type, "list") ? " or more" : ""));
        permitted = "<val>";
    }

    longName = "--" + ln + (permitted.empty() ? "" : " " + permitted);
    hotKey = (sn.empty() ? "" : "-" + sn);
    if (is_positional)
        longName = hotKey = ln;
}

//--------------------------------------------------------------------------------
bool COptionsBase::usage(const string_q& errMsg) const {
    if (errMsg.empty() || contains(errMsg, "Invalid option:") || isApiMode()) {
        cerr << usageStr(errMsg);
    } else {
        cerr << usageStr(errMsg + " Quitting...");
    }
    return false;
}

//--------------------------------------------------------------------------------
bool COptionsBase::invalid_option(const string_q& arg) const {
    if (startsWith(arg, "-") && !contains(arg, "--") && isBadSingleDash(arg))
        return invalid_option(arg + ". Did you mean -" + arg + "?");
    return usage("Invalid option: " + arg);
}

//--------------------------------------------------------------------------------
bool COptionsBase::flag_required(const string_q& command) const {
    string_q req = "The --[{COMMAND}] option requires a value.";
    return usage(substitute(req, "[{COMMAND}]", command));
}

//--------------------------------------------------------------------------------
void errorMessage(const string_q& msg) {
    if (isApiMode()) {
        const char* STR_ERROR_JSON = "{ \"errors\": [ \"[ERRORS]\" ] }\n";
        string_q message = substitute(msg, "$CONFIG", configPath("trueBlocks.toml"));
        if (!contains(message, "[")) {
            message = substitute(message, "|", " ");
        }
        cout << substitute(substitute(STR_ERROR_JSON, "[ERRORS]", message), "`", "");
    } else {
        string_q message = substitute(substitute(msg, "|", "\n  "), "$CONFIG", configPath("trueBlocks.toml"));
        while (contains(message, '`')) {
            replace(message, "`", bTeal);
            replace(message, "`", cOff);
        }
        cerr << endl
             << cRed << "  Warning: " << cOff << message << (endsWith(msg, '.') ? "" : ".") << " Quitting..." << endl
             << endl;
    }
}

//--------------------------------------------------------------------------------
map<string_q, string_q> progNameMap = {
    // BEG_CODE_CHIFRA_PAIRMAP
    // -- Accounts
    // list
    make_pair("acctExport", "chifra export"),
    // monitors
    make_pair("ethNames", "chifra names"),
    make_pair("grabABI", "chifra abis"),
    // -- Chain Data
    make_pair("getBlocks", "chifra blocks"),
    make_pair("getTrans", "chifra transactions"),
    make_pair("getReceipts", "chifra receipts"),
    make_pair("getLogs", "chifra logs"),
    make_pair("getTraces", "chifra traces"),
    make_pair("whenBlock", "chifra when"),
    // -- Chain State
    make_pair("getState", "chifra state"),
    make_pair("getTokens", "chifra tokens"),
    // -- Admin
    make_pair("cacheStatus", "chifra status"),
    // serve
    make_pair("blockScrape", "chifra scrape"),
    // init
    make_pair("pinMan", "chifra pins"),
    // -- Other
    make_pair("getQuotes", "chifra quotes"),
    make_pair("fireStorm", "chifra explore"),
    make_pair("ethslurp", "chifra slurp"),
    // END_CODE_CHIFRA_PAIRMAP
    //
    make_pair("makeClass", "makeClass"),
    make_pair("testRunner", "testRunner"),
    make_pair("chifra", "chifra"),
};

//--------------------------------------------------------------------------------
string_q COptionsBase::usageStr(const string_q& errMsgIn) const {
    string_q errMsg = errMsgIn;
    while (contains(errMsg, "`")) {
        replace(errMsg, "`", "");  // cTeal);
        replace(errMsg, "`", "");  // cRed);
    }

    if (isApiMode())
        errorMessage(getProgName() + " - " + errMsg);

    ostringstream os;
    if (isReadme) {
        colorsOff();
        os << "### Usage\n";
    }

    os << "\n";
    if (!isReadme && !errMsg.empty())
        os << cRed << "  " << errMsg << cOff << "\n\n";
    string_q progName = getProgName();
    if (isReadme && !contains(progName, "chifra"))
        progName = progNameMap[progName];
    os << hiUp1 << "Usage:" << hiDown << "    " << progName << " " << options() << "  " << endl;
    os << purpose();
    os << descriptions() << "\n";
    os << get_notes();
    os << get_configs();
    if (!isReadme) {
        os << bBlue << "  Powered by TrueBlocks";
        os << (isTestMode() ? "" : " (" + getVersionStr() + ")") << "\n" << cOff;
    }

    return os.str().c_str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::options(void) const {
    string_q positional;

    ostringstream shorts;
    for (uint64_t i = 0; i < cntParams; i++) {
        if (pParams[i].is_positional) {
            positional += (" " + pParams[i].longName);

        } else if (pParams[i].is_hidden || pParams[i].is_deprecated) {
            // invisible option

        } else if (!pParams[i].hotKey.empty()) {
            shorts << pParams[i].hotKey << "|";
        }
    }
    if (isEnabled(OPT_VERBOSE))
        shorts << "-v|";
    if (isEnabled(OPT_HELP))
        shorts << "-h";

    ostringstream os;
    if (!shorts.str().empty())
        os << "[" << shorts.str() << "] ";

    replaceAll(positional, "addrs2 blocks", "<address> <address> [address...] [block...]");
    replaceAll(positional, "addrs blocks", "<address> [address...] [block...]");
    replaceAll(positional, "block_list", "< block | date > [ block... | date... ]");
    replaceAll(positional, "transactions", "<tx_id> [tx_id...]");
    replaceAll(positional, "blocks", "<block> [block...]");
    replaceAll(positional, "addrs topics fourbytes", "<address> [address...] [topics] [fourbytes]");
    replaceAll(positional, "addrs", "<address> [address...]");
    replaceAll(positional, "files", "<file> [file...]");
    replaceAll(positional, "terms", "<term> [term...]");
    replaceAll(positional, "modes", "<mode> [mode...]");
    if (isReadme)
        positional = substitute(substitute(positional, "<", "&lt;"), ">", "&gt;");
    os << trim(positional);

    return trim(os.str());
}

//--------------------------------------------------------------------------------
string_q COptionsBase::purpose(void) const {
    ostringstream os;
    os << hiUp1 << "Purpose:" << hiDown << "  ";
    string_q purpose;
    for (size_t p = 0; p < cntParams; p++)
        if (pParams[p].longName.empty())  // program description
            purpose = substitute(pParams[p].description, "|", "\n            ");
    os << substitute(purpose, "\n", "\n        ") << "\n";
    if (!endsWith(purpose, "\n"))
        os << "\n";

    string_q nn = os.str();
    while (!isReadme && contains(nn, '`')) {
        replace(nn, "`", hiUp2);
        replace(nn, "`", hiDown);
    }
    return nn;
}

//--------------------------------------------------------------------------------
const char* STR_ONE_LINE = "| {S} | {L} | {D} |\n";
string_q COptionsBase::oneDescription(const string_q& sN, const string_q& lN, const string_q& d, bool isPositional,
                                      bool required) const {
    ostringstream os;
    if (isReadme) {
        string_q dd = (d + (required && isPositional ? " (required)" : ""));
        replaceAll(dd, "|", " \\| ");
        replace(dd, "*", "\\*");
        replaceAll(dd, "[", "*[ ");
        replaceAll(dd, "]", " ]*");

        // When we are writing the readme file...
        string_q line = STR_ONE_LINE;
        replace(line, "{S}", sN);
        replace(line, "{L}", substitute(substitute(lN, "<", "&lt;"), ">", "&gt;"));
        replace(line, "{D}", dd);
        os << line;

    } else {
        // When we are writing to the command line...
        string_q line = "\t" + substitute(substitute(string_q(STR_ONE_LINE), " ", ""), "|", "");
        replace(line, "{S}", (isPositional ? "" : padRight(sN, 3)));
        if (isPositional) {
            replace(line, "{L}", padRight(lN, 22));
        } else {
            replace(line, "{L}", padRight((lN.empty() ? "" : " (" + lN + ")"), 19));
        }
        replace(line, "{D}", d + (required && isPositional ? " (required)" : ""));
        os << line;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::format_notes(const CStringArray& strs) const {
    string_q nn;
    for (auto n : strs) {
        string_q s = substitute(n, "[{CONFIG}]", configPathRelative(""));
        if (isTestMode())
            s = substitute(n, "[{CONFIG}]", "$CONFIG/");
        nn += (s + "\n");
    }
    while (!isReadme && contains(nn, '`')) {
        replace(nn, "`", hiUp2);
        replace(nn, "`", hiDown);
    }
    string_q lead = (isReadme ? "" : "\t");
    ostringstream os;
    while (!nn.empty()) {
        string_q line = substitute(substitute(nextTokenClear(nn, '\n'), " |", "|"), "|", "\n" + lead + " ");
        os << lead << "- " << line << "\n";
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_notes(void) const {
    if ((!isReadme && !verbose) || (notes.size() == 0))
        return "";

    ostringstream os;
    os << hiUp1 << "Notes:" << hiDown << "\n" << (isReadme ? "\n" : "");
    os << format_notes(notes);
    os << "\n";

    return substitute(os.str(), "-   ", "  - ");
}

//--------------------------------------------------------------------------------
string_q COptionsBase::format_configs(const CStringArray& strs) const {
    string_q nn;
    for (auto n : strs) {
        string_q s = substitute(n, "[{CONFIG}]", configPathRelative(""));
        if (isTestMode())
            s = substitute(n, "[{CONFIG}]", "$CONFIG/");
        nn += (s + "\n");
    }
    while (!isReadme && contains(nn, '`')) {
        replace(nn, "`", hiUp2);
        replace(nn, "`", hiDown);
    }
    string_q lead = (isReadme ? "" : "\t");
    ostringstream os;
    while (!nn.empty()) {
        string_q line = substitute(substitute(nextTokenClear(nn, '\n'), " |", "|"), "|", "\n" + lead + " ");
        os << lead << line << "\n";
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_configs(void) const {
    if ((!isReadme && !verbose) || (configs.size() == 0))
        return "";

    ostringstream os;
    os << hiUp1 << "Configurable Items:" << hiDown << "\n" << (isReadme ? "\n" : "");
    os << format_configs(configs);
    os << "\n";

    return substitute(os.str(), "-   ", "  - ");
}

//--------------------------------------------------------------------------------
string_q COptionsBase::descriptions(void) const {
    ostringstream os;
    os << hiUp1 << "Where:" << hiDown << "  \n";
    if (!overrideStr.empty()) {
        CStringArray strs;
        strs.push_back(overrideStr);
        string_q ret = format_notes(strs);
        replace(ret, "-", "");
        if (isReadme)
            os << "```" << endl;
        os << ret;
        if (isReadme)
            os << "```" << endl;

    } else {
        if (isReadme) {
            os << "\n";
            os << "| | Option | Description |\n";
            os << "| :----- | :----- | :---------- |\n";
        }

        size_t nHidden = 0;
        for (uint64_t i = 0; i < cntParams; i++) {
            string_q hKey = pParams[i].hotKey;
            string_q lName = substitute(pParams[i].longName, "addrs2", "addrs");
            string_q descr = trim(pParams[i].description);
            bool isPositional = pParams[i].is_positional;
            if (!pParams[i].is_hidden && !pParams[i].is_deprecated && !pParams[i].longName.empty()) {
                bool isReq = !pParams[i].is_optional;
                hKey = (isPositional ? "" : hKey);
                lName = substitute(substitute((isPositional ? substitute(lName, "-", "") : lName), "!", ""), "~", "");
                os << oneDescription(hKey, lName, descr, isPositional, isReq);
            }
            if (pParams[i].is_hidden)
                nHidden++;
        }

        // For testing purposes, we show the hidden options
        if (nHidden && (isTestMode() || (verbose > 1))) {
            if (isReadme) {
                os << "|####|Hidden options||" << endl;
            } else {
                os << endl;
                os << cTeal << italic << "\t#### Hidden options" << cOff << endl;
            }
            for (uint64_t i = 0; i < cntParams; i++) {
                string_q hKey = pParams[i].hotKey;
                string_q lName = pParams[i].longName;
                string_q descr = trim(pParams[i].description);
                bool isPositional = pParams[i].is_positional;
                if (pParams[i].is_hidden && !pParams[i].is_deprecated && !pParams[i].longName.empty()) {
                    bool isReq = !pParams[i].is_optional;
                    lName =
                        substitute(substitute((isPositional ? substitute(lName, "-", "") : lName), "!", ""), "~", "");
                    lName = substitute(lName, "@-", "");
                    hKey = (isPositional ? "" : pParams[i].hotKey);
                    os << oneDescription(hKey, lName, descr, isPositional, isReq);
                }
            }
            if (isReadme) {
                os << "|####|Hidden options||" << endl;
            } else {
                os << cTeal << italic << "\t#### Hidden options" << cOff << endl;
                os << endl;
            }
        }

        if (isEnabled(OPT_FMT) && (verbose || isTestMode()))
            os << oneDescription("-x", "--fmt <val>", "export format, one of [none|json*|txt|csv|api]");
        if (isEnabled(OPT_VERBOSE))
            os << oneDescription("-v", "--verbose", "set verbose level (optional level defaults to 1)");
        if (isEnabled(OPT_HELP))
            os << oneDescription("-h", "--help", "display this help screen");
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::expandOption(string_q& arg) {
    string_q ret = arg;

    // Check that we don't have a regular command with a single dash, which
    // should report an error in client code
    for (uint64_t i = 0; i < cntParams; i++) {
        if (pParams[i].longName == arg) {
            arg = "";
            return ret;
        }
    }

    // Not an option
    if (!startsWith(arg, '-') || startsWith(arg, "--")) {
        arg = "";
        return ret;
    }

    // Stdin case
    if (arg == "-") {
        arg = "";
        return ret;
    }

    // Single option
    if (arg.length() == 2) {
        arg = "";
        return ret;
    }

    // Special case
    if (arg == "-th" || arg == "-ht") {
        isReadme = true;
        hiUp1 = hiUp2 = hiDown = '`';
        arg = "";
        replaceAll(ret, "-th", "");
        replaceAll(ret, "-ht", "");
        if (!isEnabled(OPT_HELP))
            ret = "-h";
        return ret;
    }

    // This may be a command with two -a -b (or more) single options
    if (arg.length() > 2 && arg[2] == ' ') {
        ret = extract(arg, 0, 2);
        arg = extract(arg, 3);
        return ret;
    }

    // One of the range commands. These must be alone on
    // the line (this is a bug for -rf:txt for example)
    if (contains(arg, ":") || contains(arg, "=")) {
        arg = "";
        return ret;
    }

    // This is a ganged-up option. We need to pull it apart by returning
    // the first two chars, and saving the rest for later.
    ret = extract(arg, 0, 2);
    arg = "-" + extract(arg, 2);
    return ret;
}

//--------------------------------------------------------------------------------
int sortParams(const void* c1, const void* c2) {
    const COption* p1 = reinterpret_cast<const COption*>(c1);
    const COption* p2 = reinterpret_cast<const COption*>(c2);
    if (p1->hotKey == "-h")
        return 1;
    else if (p2->hotKey == "-h")
        return -1;
    return p1->hotKey.compare(p2->hotKey);
}

//--------------------------------------------------------------------------------
uint64_t verbose = false;

extern const char* STR_OLD_FOLDER_ERROR;
//---------------------------------------------------------------------------------------------------
string_q configPath(const string_q& part) {
    static bool been_here = false;
    if (!been_here) {
        if (folderExists(getHomeFolder() + ".quickBlocks")) {
            cerr << bBlue << STR_OLD_FOLDER_ERROR << cOff << endl;
            quickQuitHandler(1);
        }
        been_here = true;
    }
#if defined(__linux) || defined(__linux__) || defined(linux)
    return getHomeFolder() + ".local/share/trueblocks/" + part;
#elif defined(__APPLE__)
    return getHomeFolder() + "Library/Application Support/TrueBlocks/" + part;
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
#error-- This source code does not compile on Windows
#endif
}

//---------------------------------------------------------------------------------------------------
string_q configPathRelative(const string_q& part) {
    return substitute(configPath(part), getHomeFolder(), "~/");
}

//------------------------------------------------------------------
void editFile(const string_q& fileName) {
    CToml toml(configPath("trueBlocks.toml"));
    string_q editor = toml.getConfigStr("dev", "editor", "<NOT_SET>");
    if (!isTestMode() && editor == "<NOT_SET>") {
        editor = getEnvStr("EDITOR");
        if (editor.empty()) {
            errorMessage("$EDITOR is not set. Either export it to your environment before calling.");
            return;
        }
    }

    CFilename fn(fileName);
    string_q cmd = "cd \"" + fn.getPath() + "\" ; " + editor + " \"" + fn.getFilename() + "\"";
    if (isTestMode()) {
        cerr << "Testing editFile: " << fn.getFilename() << "\n";
        string_q contents;
        asciiFileToString(fileName, contents);
        cout << contents << "\n";
    } else {
        // clang-format off
        if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
        // clang-format on
    }
}

//-------------------------------------------------------------------------
bool COptionsBase::isEnabled(uint32_t q) const {
    return (enableBits & q);
}

void COptionsBase::optionOff(uint32_t q) {
    enableBits &= (~q);
}

void COptionsBase::optionOn(uint32_t q) {
    enableBits |= q;
}

//--------------------------------------------------------------------------------
int sortByBlockNum(const void* v1, const void* v2) {
    const CNameValue* b1 = reinterpret_cast<const CNameValue*>(v1);
    const CNameValue* b2 = reinterpret_cast<const CNameValue*>(v2);
    if (b1->first == "latest")
        return 1;
    if (b2->first == "latest")
        return -1;
    if (contains(b1->second, "tbd") && contains(b1->second, "tbd"))
        return b1->second.compare(b2->second);
    if (contains(b1->second, "tbd"))
        return 1;
    if (contains(b2->second, "tbd"))
        return -1;
    return static_cast<int>(str_2_Uint(b1->second) - str_2_Uint(b2->second));
}

//-----------------------------------------------------------------------
const CToml* getGlobalConfig(const string_q& name) {
    static CToml* toml = NULL;
    static string_q components = "trueBlocks|";

    if (!toml) {
        static CToml theToml(configPath("trueBlocks.toml"));
        toml = &theToml;
        string_q fileName = configPath(COptionsBase::g_progName + ".toml");
        if (fileExists(fileName) && !contains(components, COptionsBase::g_progName + "|")) {
            components += COptionsBase::g_progName + "|";
            CToml custom(fileName);
            toml->mergeFile(&custom);
        }
    }

    // If we're told explicitly to load another config, do that as well
    if (!name.empty()) {
        string_q fileName = configPath(name + ".toml");
        if (fileExists(fileName) && !contains(components, name + "|")) {
            components += name + "|";
            CToml custom(fileName);
            toml->mergeFile(&custom);
        }
    }

    return toml;
}

//--------------------------------------------------------------------------------
bool COptionsBase::Mocked(const string_q& which) {
    if (!mocked)
        return false;
    string_q path = configPath("mocked/mocks/" + which + ".json");
    if (!fileExists(path))
        return false;
    cout << asciiFileToString(path);
    return true;
}

//-----------------------------------------------------------------------
static bool sortByValue(const CNameValue& p1, const CNameValue& p2) {
    blknum_t b1 = str_2_Uint(p1.second);
    blknum_t b2 = str_2_Uint(p2.second);
    if (b1 == 0) {
        if (p1.first == "latest")
            b1 = NOPOS;
    }
    if (b2 == 0) {
        if (p2.first == "latest")
            b2 = NOPOS;
    }
    return b1 < b2;
}

//-----------------------------------------------------------------------
// TODO(tjayrush): global data
CNameValueArray COptionsBase::specials;

//-----------------------------------------------------------------------
void COptionsBase::loadSpecials(void) {
    specials.clear();
    extern const char* STR_DEFAULT_WHENBLOCKS;
    string_q specialsStr = STR_DEFAULT_WHENBLOCKS;
    CKeyValue keyVal;
    while (keyVal.parseJson3(specialsStr)) {
        CNameValue pair = make_pair(keyVal.jsonrpc, keyVal.result);
        specials.push_back(pair);
        keyVal = CKeyValue();  // reset
    }
    sort(specials.begin(), specials.end(), sortByValue);
    return;
}

//--------------------------------------------------------------------------------
bool COptionsBase::forEverySpecialBlock(NAMEVALFUNC func, void* data) {
    if (!func)
        return false;
    if (specials.size() == 0)
        loadSpecials();
    for (auto special : specials)
        if (!(*func)(special, data))
            return false;
    return true;
}

//--------------------------------------------------------------------------------
bool COptionsBase::findSpecial(CNameValue& pair, const string_q& arg) {
    if (specials.size() == 0)
        loadSpecials();
    for (auto special : specials) {
        if (arg == special.first) {
            pair = special;
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------------------------
string_q getCachePath(const string_q& _part) {
    // TODO(tjayrush): global data
    static string_q g_cachePath;
    if (!g_cachePath.empty())  // leave early if we can
        return substitute((g_cachePath + _part), "//", "/");

    {  // give ourselves a frame - always enters - forces creation in the frame
       // Wait until any other thread is finished filling the value.
        mutex aMutex;
        lock_guard<mutex> lock(aMutex);

        // Another thread may have filled the data while we were waiting
        if (!g_cachePath.empty())
            return substitute((g_cachePath + _part), "//", "/");

        // Otherwise, fill the value
        CToml toml(configPath("trueBlocks.toml"));
        string_q path = toml.getConfigStr("settings", "cachePath", "<NOT_SET>");
        if (path == "<NOT_SET>") {
            path = configPath("cache/");
            toml.setConfigStr("settings", "cachePath", path);
            toml.writeFile();
        }

        CFilename folder(path);
        if (!folderExists(folder.getFullPath()))
            establishFolder(folder.getFullPath());

        g_cachePath = folder.getFullPath();
        if (!folder.isValid()) {
            errorMessage("Invalid cachePath (" + folder.getFullPath() + ") in config file.");
            path = configPath("cache/");
            CFilename fallback(path);
            g_cachePath = fallback.getFullPath();
        }
        if (!endsWith(g_cachePath, "/"))
            g_cachePath += "/";
    }

    return substitute((g_cachePath + _part), "//", "/");
}

//---------------------------------------------------------------------------------------------------
COptionsBase::COptionsBase(void) {
    minArgs = 1;
    isReadme = false;
    isRaw = false;
    isVeryRaw = false;
    mocked = false;
    firstOut = true;
    freshenOnly = false;
    noHeader = false;
    enableBits = OPT_DEFAULT;
    hiUp1 = (isTestMode() ? "" : cYellow) + "  ";
    hiUp2 = (isTestMode() ? "" : cTeal);
    hiDown = (isTestMode() ? "" : cOff);
    arguments.clear();
    usageErrs.clear();
    notes.clear();
    commandLines.clear();

    namesMap.clear();
    tokenMap.clear();
    expContext().prefundMap.clear();
    airdropMap.clear();
    pParams = NULL;
    cntParams = 0;
    coutSaved = NULL;
    rd_outputFilename = "";
    rd_zipOnClose = false;
    overrideStr = "";
    // outputStream
}

//--------------------------------------------------------------------------------
COptionsBase::~COptionsBase(void) {
    closeRedirect();
}

//--------------------------------------------------------------------------------
bool COptionsBase::isRedirected(void) const {
    return (coutSaved != NULL);
}

//--------------------------------------------------------------------------------
void COptionsBase::closeRedirect(void) {
    if (coutSaved != NULL) {
        // restore cout's original streambuf
        cout.rdbuf(coutSaved);
        outputStream.flush();
        outputStream.close();
        coutSaved = NULL;
        // Let the call know where we put the data
        string_q outFn = (isTestMode() ? "--output_filename--" : rd_outputFilename) + (rd_zipOnClose ? ".gz" : "");
        switch (expContext().exportFmt) {
            case TXT1:
                cout << outFn << endl;
                break;
            case CSV1:
            case YAML1:
                cout << "-outputFilename: " << outFn << endl;
                break;
            default:
                cout << "{ \"outputFilename\": \"" << outFn << "\" }";
                break;
        }

        // Zip the file if we're told to do so
        if (!isTestMode() && rd_zipOnClose) {
            ostringstream os;
            os << "gzip -fv " << substitute(outFn, ".gz", "");
            // clang-format off
            if (system(os.str().c_str())) {}  // Don't remove. Silences warnings
            // clang-format on
        }

        rd_zipOnClose = false;
        if (isTestMode()) {
            ::remove(rd_outputFilename.c_str());
        }
        rd_outputFilename = "";
    }
}

//-----------------------------------------------------------------------
string_q cleanFmt(const string_q& str) {
    format_t fmt = expContext().exportFmt;
    string_q ret = (substitute(substitute(substitute(str, "\n", ""), "\\n", "\n"), "\\t", "\t"));
    if (fmt == CSV1)
        ret = "\"" + substitute(ret, "\t", "\",\"") + "\"";
    return ret;
}

//-----------------------------------------------------------------------
const char* STR_DEFAULT_WHENBLOCKS =
    "[ "
    "{ name: \"first\", value: 0 },"
    "{ name: \"firstTrans\", value: 46147 },"
    "{ name: \"firstContract\", value: 50111 },"
    "{ name: \"iceage\", value: 200000 },"
    "{ name: \"devcon1\", value: 543626 },"
    "{ name: \"homestead\", value: 1150000 },"
    "{ name: \"daofund\", value: 1428756 },"
    "{ name: \"daohack\", value: 1718497 },"
    "{ name: \"daofork\", value: 1920000 },"
    "{ name: \"devcon2\", value: 2286910 },"
    "{ name: \"tangerine\", value: 2463000 },"
    "{ name: \"spurious\", value: 2675000 },"
    "{ name: \"stateclear\", value: 2717576 },"
    "{ name: \"eea\", value: 3265360 },"
    "{ name: \"ens2\", value: 3327417 },"
    "{ name: \"parityhack1\", value: 4041179 },"
    "{ name: \"byzantium\", value: 4370000 },"
    "{ name: \"devcon3\", value: 4469339 },"
    "{ name: \"parityhack2\", value: 4501969 },"
    "{ name: \"kitties\", value: 4605167 },"
    "{ name: \"makerdao\", value: 4620855 },"
    "{ name: \"devcon4\", value: 6610517 },"
    "{ name: \"uniswap\", value: 6627917 },"
    "{ name: \"constantinople\", value: 7280000 },"
    "{ name: \"devcon5\", value: 8700401 },"
    "{ name: \"mcdai\", value: 8928158 },"
    "{ name: \"istanbul\", value: 9069000 },"
    "{ name: \"muirglacier\", value: 9200000 },"
    "{ name: \"berlin\", value: 12244000 },"
    "{ name: \"london\", value: 12965000 },"
    "{ name: \"latest\", value:\"\" }"
    "]";

//-----------------------------------------------------------------------
const char* STR_OLD_FOLDER_ERROR =
    "\n"
    "  You must complete the migration process before proceeding:\n\n"
    "      https://github.com/TrueBlocks/trueblocks-core/tree/master/src/other/migrations\n";

}  // namespace qblocks
