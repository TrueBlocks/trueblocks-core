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
    void COptionsBase::registerOptions(size_t nP, COption const *pP) {
        // TODO(tjayrush): global data
        arguments.clear();
        cntParams = nP;
        pParams = pP;
    }

    //--------------------------------------------------------------------------------
    // TODO(tjayrush): global data - but okay, a program only has one name
    string_q COptionsBase::g_progName = "quickBlocks";

    //--------------------------------------------------------------------------------
    void COptionsBase::setProgName(const string_q& name) {
        g_progName = name;
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::getProgName(void) const {
        return g_progName;
    }

    //--------------------------------------------------------------------------------
    bool prepareEnv(int argc, const char *argv[]) {
        string_q env = getEnvStr("NO_COLOR");
        if (string_q(env) == "true")
            colorsOff();
        if (argc > 0)
            COptionsBase::g_progName = basename((char*)argv[0]);  // NOLINT
        return true;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::prepareArguments(int argc, const char *argv[]) {

        prepareEnv(argc, argv);
        if (getEnvStr("REDIR_CERR") == "true")
            cerr.rdbuf( cout.rdbuf() );

        // send out the environment, if any non-default
        if (isTestMode() || isLevelOn(sev_debug4)) {
            size_t save = verbose;
            verbose = 10;
            if (isApiMode())                        { LOG4("API_MODE=",    getEnvStr("API_MODE")); }
            if (getEnvStr("DOCKER_MODE") == "true") { LOG4("DOCKER_MODE=", getEnvStr("DOCKER_MODE")); }
//            if (!getEnvStr("IPFS_PATH").empty())    { LOG4("IPFS_PATH=",   getEnvStr("IPFS_PATH")); }
            verbose = save;
        }

        if (isTestMode()) {
            ostringstream argos1;
            ostringstream argos2;
            for (int i = 1 ; i < argc ; i++) {
                string_q str = argv[i];
                argos1 << "[" << i << ":" << trim(str) << "] ";
                argos2 << trim(str) << " ";
            }

            const char* STR_TESTLOAD_TXT = "[{PROGRAM}] argc: [{ARGC}] [{ARGS}]\n";
            const char* STR_CMD_TXT = "[{PROGRAM}] [{ARGS}]\n";

            string_q out = STR_TESTLOAD_TXT;
            replace(out, "[{PROGRAM}]", getProgName());
            replace(out, "[{ARGC}]", int_2_Str(argc));
            replace(out, "[{ARGS}]", argos1.str());
            cerr << out;

            out = STR_CMD_TXT;
            replace(out, "[{PROGRAM}]", getProgName());
            replace(out, "[{ARGS}]", argos2.str());
            cerr << out;
        }

        if ((uint64_t)argc <= minArgs)  // the first arg is the program's name
            return usage("Not enough arguments presented.");

        int nChars = 0;
        for (int i = 0 ; i < argc ; i++) {
            nChars += string_q(argv[i]).length();
        }
        size_t nArgs = 0;
        string_q *args = new string_q[argc + nChars + 2];

        bool hasStdIn = false;
        for (int i = 1 ; i < argc ; i++) {
            string_q str = argv[i];
            string_q arg = trim(str);
            replace(arg, "--verbose", "-v");
            while (!arg.empty()) {
                string_q opt = expandOption(arg);  // handles case of -rf for example
                if (isReadme) {
                    if (args)
                        delete [] args;
                    return usage();
                }
                if (opt == "-")
                    hasStdIn = true;
                else if (!opt.empty())
                    args[nArgs++] = opt;
            }
        }

        string_q stdInCmds;
        if (hasStdIn) {
            // reading from stdin, expect only a list of addresses, one per line.
            char c = static_cast<char>(getchar());
            while (c != EOF) {
                stdInCmds += c;
                c = static_cast<char>(getchar());
            }
            if (!endsWith(stdInCmds, "\n"))
                stdInCmds += "\n";
        }

        //-----------------------------------------------------------------------------------
        // Collapse commands that have 'permitted' sub options (i.e. colon ":" args)
        //-----------------------------------------------------------------------------------
        size_t curArg = 0;
        for (size_t i = 0 ; i < nArgs ; i++) {
            string_q arg = args[i];
            bool combine = false;
            for (size_t j = 0 ; j < cntParams && !combine ; j++) {
                if (!pParams[j].permitted.empty()) {
                    string_q shortName = pParams[j].shortName;
                    string_q longName  = pParams[j].longName;
                    if (shortName == arg || startsWith(longName, arg)) {
                        // We want to pull the next parameter into this one since it's a ':' param
                        combine = true;
                    }
                }
            }

            if (!combine && (arg == "-v" || arg == "-verbose" || arg == "--verbose")) {
                if (i < nArgs-1) {
                    uint64_t n = str_2_Uint(args[i+1]);
                    if (n > 0 && n <= 10) {
                        // We want to pull the next parameter into this one since it's a ':' param
                        combine = true;
                    }
                }
                if (!combine) {
                    arg = "--verbose:1";
                }
            }

            if (!combine && arg == "--fmt") {
                if (i < nArgs-1)
                    combine = true;
            }

            if (combine && i < (nArgs-1)) {
                args[curArg++] = arg + ":" + args[i+1];
                i++;
            } else {
                args[curArg++] = arg;
            }
        }
        nArgs = curArg;

        //-----------------------------------------------------------------------------------
        // We now have 'nArgs' command line arguments stored in the array 'args.'  We spin
        // through them doing one of two things
        //
        // (1) handle any arguments common to all programs and remove them from the array
        // (2) identify any --file arguments and store them for later use
        //-----------------------------------------------------------------------------------
        string_q cmdFileName = "";
        for (uint64_t i = 0 ; i < nArgs ; i++) {
            string_q arg = args[i];
            if (startsWith(arg, "--file:")) {
                cmdFileName = substitute(arg, "--file:", "");
                replace(cmdFileName, "~/", getHomeFolder());
                if (!fileExists(cmdFileName)) {
                    if (args) delete [] args;
                    return usage("--file: '" + cmdFileName + "' not found. Quitting.");
                }

            } else if (startsWith(arg, "-v:") || startsWith(arg, "--verbose:")) {
                verbose = true;
                arg = substitute(substitute(arg, "-v:", ""), "--verbose:", "");
                if (!arg.empty()) {
                    if (!isUnsigned(arg))
                        return usage("Invalid verbose level '" + arg + "'. Quitting...");
                    verbose = str_2_Uint(arg);
                }

            } else if (startsWith(arg, "--fmt:")) {
                arg = substitute(arg, "--fmt:", "");
                     if ( arg == "txt" ) { exportFmt = TXT1;  }
                else if ( arg == "csv" ) { exportFmt = CSV1; }  // NOLINT
                else if ( arg == "json") { exportFmt = JSON1; }  // NOLINT
                else if ( arg == "api" ) { exportFmt = API1; }  // NOLINT
                else return usage("Export format (" + arg +  // NOLINT
                    ") must be one of [ json | txt | csv | api ]. Quitting...");
                args[i] = "";
            }
        }

        // remove empty arguments
        curArg = 0;
        for (size_t i = 0 ; i < nArgs ; i++) {
            if (!args[i].empty())
                args[curArg++] = args[i];
        }
        nArgs = curArg;

        // If we have a command file, we will use it, if not we will creat one and pretend we have one.
        string_q commandList = "";
        for (uint64_t i = 0 ; i < nArgs ; i++) {
            string_q a = args[i];
            if (!contains(a, "--file:"))
                commandList += (a + " ");
        }
        commandList += '\n';

        if (!cmdFileName.empty()) {
            string_q toAll;
            if (!commandList.empty())
                toAll = (" " + substitute(commandList, "\n", ""));
            commandList = "";
            // The command line also has a --file in it, so add these commands as well
            string_q contents;
            asciiFileToString(cmdFileName, contents);
            cleanString(contents, false);
            if (contents.empty())
                return usage("Command file '" + cmdFileName + "' is empty. Quitting...");
            if (startsWith(contents, "NOPARSE\n")) {
                commandList = contents;
                nextTokenClear(commandList, '\n');
                commandList += toAll;
            } else {
                CStringArray lines;
                explode(lines, contents, '\n');
                for (auto command : lines) {
                    if (!command.empty() && !startsWith(command, ";")) {  // ignore comments
                        commandList += (command + toAll + "\n");
                    }
                }
            }
        }
        commandList += stdInCmds;
        explode(commandLines, commandList, '\n');
        for (auto& item : commandLines)
            item = trim(item);
        if (commandLines.empty())
            commandLines.push_back("--noop");

        if (args) delete [] args;

        return 1;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::standardOptions(string_q& cmdLine) {

        // Note: check each item individual in case more than one appears on the command line
        cmdLine += " ";
        replace(cmdLine, "--output ", "--output:");
        replace(cmdLine, "--fmt ", "--fmt:");

        if (contains(cmdLine, "--noop ")) {
            // do nothing
            replaceAll(cmdLine, "--noop ", "");
        }

        if (contains(cmdLine, "--version ")) {
            cerr << getProgName() << " " << getVersionStr() << "\n";
            exit(0);
        }

        if (contains(cmdLine, "--nocolor ")) {
            replaceAll(cmdLine, "--nocolor ", "");
            colorsOff();
        }

        if (contains(cmdLine, "--no_header ")) {
            replaceAll(cmdLine, "--no_header ", "");
            isNoHeader = true;
        }

        if (isEnabled(OPT_HELP) && (contains(cmdLine, "-h ") || contains(cmdLine, "--help "))) {
            usage();
            exit(0);

        }

        if (isEnabled(OPT_ETHER) && contains(cmdLine, "--ether " )) {
            replaceAll(cmdLine, "--ether ", "");
            expContext().asEther = true;
            expContext().asDollars = false;
            expContext().asWei = false;
        }

        if (isEnabled(OPT_RAW) && contains(cmdLine, "--very_raw ")) {
            replaceAll(cmdLine, "--very_raw ", "");
            isRaw = true;
            isVeryRaw = true;
        }

        if (isEnabled(OPT_RAW) && contains(cmdLine, "--raw ")) {
            replaceAll(cmdLine, "--raw ", "");
            isRaw = true;
        }

        if (isEnabled(OPT_OUTPUT) && contains(cmdLine, "--output:")) {
            redirFilename = substitute(cmdLine, "--output:", "|");
            nextTokenClear(redirFilename, '|');
            redirFilename = nextTokenClear(redirFilename, ' ');
            if (redirFilename.empty())
                return usage("Please provide a filename for the --output option. Quitting...");
            if (!isTestMode() && !startsWith(redirFilename, '/'))
                return usage("Output file (" + redirFilename + ") must be a fully qualified path. Quitting...");
            establishFolder(redirFilename);
            ASSERT(!folderExists(outputFn));
            redirStream.open(redirFilename.c_str());
            if ( redirStream.is_open() ) {
                coutBackup = cout.rdbuf();     // back up cout's streambuf
                cout.rdbuf( redirStream.rdbuf() );         // assign streambuf to cout
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
            expContext().isParity = true;
            for (int i = 0 ; i < 5 ; i++)
                if (sorts[i])
                    sorts[i]->sortFieldList();
        }

        // A final set of options that do not have command line options
        if (isEnabled(OPT_PREFUND)) {
            CStringArray lines;
            asciiFileToLines(configPath("prefunds.txt"), lines);
            for (auto line : lines) {
                CStringArray parts;
                explode(parts, line, '\t');
                prefundWeiMap[toLower(parts[0])] = str_2_Wei(parts[1]);
            }
        }

        if (isEnabled(OPT_RUNONCE)) {
            if (commandLines.size() > 1)
                return usage("You may not use the --file with this application. Quitting...");
            // protect ourselves from running twice over
            if (isRunning(getProgName(), true))
                return usage("Warning: the command " + getProgName() + " is already running. Quitting...");
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

        if (isEnabled(OPT_ETHER) && arg == "--ether")
            return true;
        if (isEnabled(OPT_RAW) && arg == "--raw")
            return true;
        if (isEnabled(OPT_OUTPUT) && startsWith(arg, "--output:"))
            return true;
        if (isEnabled(OPT_RAW) && arg == "--very_raw")
            return true;
        if (isEnabled(OPT_WEI) && arg == "--wei")
            return true;
        if (isEnabled(OPT_DOLLARS) && arg == "--dollars")
            return true;
        if (isEnabled(OPT_PARITY) && (arg == "--parity"))
            return true;
        if (isEnabled(OPT_FMT) && startsWith(arg, "--fmt:"))
            return true;
        if (arg == "--version")
            return true;
        if (arg == "--nocolor")
            return true;
        if (arg == "--noop")
            return true;
        return false;
    }

    //---------------------------------------------------------------------------------------------------
    bool COptionsBase::confirmUint(const string_q&name, uint64_t& value, const string_q& argIn) const {

        value = NOPOS;

        const COption *param = findParam(name);
        if (!param)
            return usage("Unknown parameter `" + name + "'. Quitting...");
        if (!contains(param->type, "uint") && !contains(param->type, "blknum"))
            return true;

        string_q arg = argIn;
        replace(arg, param->shortName + ":", "");
        replace(arg, name + ":", "");
        replaceAll(arg, "-", "");

        if (!isNumeral(arg))
            return usage("Value to --" + name + " parameter (" + arg +
                ") must be a valid unsigned integer. Quitting...");
        value = str_2_Uint(arg);
        return true;
    }

    //---------------------------------------------------------------------------------------------------
    bool COptionsBase::confirmBlockNum(const string_q&name, blknum_t& value,
                                        const string_q& argIn, blknum_t latest) const {

        if (!confirmUint(name, value, argIn))
            return false;
        if (value > latest)
            return usage("Block number (" + argIn + ") is greater than the latest block. Quitting...");
        return true;
    }

    //---------------------------------------------------------------------------------------------------
    bool COptionsBase::confirmEnum(const string_q&name, string_q& value, const string_q& argIn) const {
        const COption *param = findParam(name);
        if (!param)
            return usage("Unknown parameter `" + name + "'. Quitting...");
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
        replace(arg, param->shortName + ":", "");
        replace(arg, name + ":", "");
        replaceAll(arg, "-", "");

        if (!contains(type, "|" + arg + "|")) {
            string_q desc = substitute(substitute(param->description, ", one ", "| One "), "*", "");
            nextTokenClear(desc, '|');
            return usage("Invalid option '" + arg + "' for '" + name + "'." + desc + " required. Quitting...");
        }

        value = arg;
        return true;
    }

    //---------------------------------------------------------------------------------------------------
    const COption *COptionsBase::findParam(const string_q& name) const {
        for (size_t i = 0 ; i < cntParams ; i++)
            if (startsWith(pParams[i].longName, "--" + name))
                return &pParams[i];
        return NULL;
    }

    //--------------------------------------------------------------------------------
    // If nameIn starts with (modes are required -- unless noted. --options are optional):
    //      -    ==> regular option
    //      @    ==> hidden option
    //      ~    ==> mode (no leading --option needed)
    //      ~!   ==> non-required mode
    //      empty nameIn means description is for the whole program (not the option)
    //--------------------------------------------------------------------------------
    COption::COption(const string_q& nameIn, const string_q& descr) {

        longName      = nameIn;
        shortName     = nameIn;
        is_hidden     = startsWith(nameIn, "@");
        is_positional = startsWith(nameIn, "~");
        is_optional   = contains  (nameIn, "!");
        description   = substitute(descr, "&#44;", ",");
        if (nameIn.empty())
            return;

        replaceAll(shortName, "-", "");
        replaceAll(shortName, "~", "");
        replaceAll(shortName, "@", "");
        replaceAll(shortName, "!", "");

        if (!is_positional) {
            longName = "--" + shortName;
            shortName = "-" + (contains(shortName, "fmt") ? "x" : extract(shortName, 0, 1));
        } else {
            longName = shortName;
        }

        if (contains(longName, ":")) {
            permitted = longName;
            longName = nextTokenClear(permitted, ':');
            type = permitted;
            replaceAny(permitted, "<>", "");
            if (permitted != "range" &&
                permitted != "list" &&
                permitted != "fn" &&
                permitted != "mode" &&
                permitted != "on/off")
                permitted = "val";
            longName += (" " + permitted);
        }

        if (contains(longName, "(") && contains(longName, ")")) {
            string_q hotKey = longName;
            nextTokenClear(hotKey, '(');
            hotKey = nextTokenClear(hotKey, ')');
            replaceAny(longName, "()", "");
            shortName = "-" + hotKey;
        }
    }

    //--------------------------------------------------------------------------------
    COption::COption(const string_q& ln, const string_q& sn, const string_q& t, size_t opts, const string_q& d) {
        description = substitute(d, "&#44;", ",");
        if (ln.empty())
            return;

        is_positional = (opts & OPT_POSITIONAL);
        is_hidden = (opts & OPT_HIDDEN);
        is_optional = !(opts & OPT_REQUIRED);

        type = t;
        permitted = t;
        permitted = substitute(permitted, "<uint32>", "<num>");
        permitted = substitute(permitted, "<uint64>", "<num>");
        permitted = substitute(permitted, "<blknum>", "<num>");
        permitted = substitute(permitted, "<string>", "<str>");
        if (contains(type, "enum")) {
            description += ", one [X] of " + substitute(substitute(substitute(type, "list<", ""), ">", ""), "enum", "");
            replace(description, " [X]", (contains(type, "list") ? " or more" : ""));
            permitted = "<val>";
        }

        longName = "--" + ln + (permitted.empty() ? "" : " " + permitted);
        shortName = (sn.empty() ? "" : "-" + sn);
        if (is_positional)
            longName = shortName = ln;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::usage(const string_q& errMsg) const {
        cerr << usageStr(errMsg);
        return false;
    }

const char *STR_ERROR_JSON =
"{ \"errors\": [ \"[ERRORS]\" ] }\n";

    //--------------------------------------------------------------------------------
    string_q COptionsBase::usageStr(const string_q& errMsg) const {

        if (isApiMode())
            cout << substitute(STR_ERROR_JSON, "[ERRORS]", getProgName() + " - " + errMsg);

        ostringstream os;
        if (isReadme) {
            colorsOff();
            os << "#### Usage\n";
        }

        os << "\n";
        if (!errMsg.empty())
            os << cRed << "  " << errMsg << cOff << "\n\n";
        os << hiUp1 << "Usage:" << hiDown << "    " << getProgName() << " " << options() << "  \n";
        os << purpose();
        os << descriptions() << "\n";
        os << notes();
        if (!isReadme) {
            os << bBlue << "  Powered by QBlocks";
            os << (isTestMode() ? "" : " (" + getVersionStr() + ")") << "\n" << cOff;
        }
        string_q ret = os.str().c_str();
        return postProcess("usage", ret);
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::options(void) const {
        string_q required;

        ostringstream os;
        if (!needsOption)
            os << "[";
        for (uint64_t i = 0 ; i < cntParams ; i++) {
            if (pParams[i].is_positional) {
                required += (pParams[i].longName.empty() ? "" : (" " + pParams[i].longName));

            } else if (pParams[i].is_hidden) {
                // invisible option

            } else if (!pParams[i].shortName.empty()) {
                os << pParams[i].shortName << "|";
            }
        }
        if (isEnabled(OPT_VERBOSE))
            os << "-v|";
        if (isEnabled(OPT_HELP))
            os << "-h";
        if (!needsOption)
            os << "]";
        os << required;

        string_q ret = postProcess("options", os.str());
        if (isReadme)
            ret = substitute(substitute(ret, "<", "&lt;"), ">", "&gt;");
        return ret;
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::purpose(void) const {
        ostringstream os;
        os << hiUp1 << "Purpose:" << hiDown << "  ";
        string_q purpose;
        for (size_t p = 0 ; p < cntParams ; p++)
            if (pParams[p].longName.empty())  // program description
                purpose = pParams[p].description;
        os << substitute(purpose, "\n", "\n        ") << "\n";
        if (!endsWith(purpose, "\n"))
            os << "\n";
        return postProcess("purpose", os.str());
    }

    //--------------------------------------------------------------------------------
const char *STR_ONE_LINE = "| {S} | {L} | {D} |\n";

    string_q COptionsBase::oneDescription(const string_q& sN, const string_q& lN, const string_q& d,
                                            bool isPositional, bool required) const {
        ostringstream os;
        if (isReadme) {

            // When we are writing the readme file...
            string_q line = STR_ONE_LINE;
            replace(line, "{S}", sN);
            replace(line, "{L}", lN);
            replace(line, "{D}", substitute((d + (required && isPositional ? " (required)" : "")), "|", "&#124;"));
            os << line;

        } else {

            // When we are writing to the command line...
            string_q line = "\t" + substitute(substitute(string_q(STR_ONE_LINE), " ", ""), "|", "");
            replace(line, "{S}", (isPositional ? "" : padRight(sN, 3)));
            if (isPositional) {
                replace(line, "{L}", padRight(lN , 22));
            } else {
                replace(line, "{L}", padRight((lN.empty() ? "" : " (" + lN + ")") , 19));
            }
            replace(line, "{D}", d + (required && isPositional ? " (required)" : ""));
            os << line;
        }
        return postProcess("oneDescription", os.str().c_str());
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::notes(void) const {

        ostringstream os;
        string_q ret = postProcess("notes", "");
        if (!ret.empty()) {
            string_q tick = "- ";
            string_q lead = "\t";
            string_q trail = "\n";
            if (isReadme) {
                lead = "";
                trail = "\n";
            }
            replaceAll(ret, "[{", hiUp2);
            replaceAll(ret, "}]", hiDown);

            os << hiUp1 << "Notes:" << hiDown << "\n";
            os << (isReadme ? "\n" : "");
            while (!ret.empty()) {
                string_q line = substitute(nextTokenClear(ret, '\n'), "|", "\n" + lead + "  ");
                os << lead << tick << line << "\n";
            }
            os << "\n";
            ret = os.str().c_str();
            replaceAll(ret, "-   ", "  - ");
        }
        return ret;
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::descriptions(void) const {

        ostringstream os;
        os << hiUp1 << "Where:" << hiDown << "  \n";
        if (isReadme) {
            os << "\n";
            os << "| Short Cut | Option | Description |\n";
            os << "| -------: | :------- | :------- |\n";
        }

        size_t nHidden = 0;
        for (uint64_t i = 0 ; i < cntParams ; i++) {
            string_q sName = pParams[i].shortName;
            string_q lName = pParams[i].longName;
            string_q descr = trim(pParams[i].description);
            bool isPositional = pParams[i].is_positional;
            if (!pParams[i].is_hidden && !sName.empty()) {
                bool isReq = !pParams[i].is_optional;
                sName = (isPositional ? "" : sName);
                lName = substitute(substitute((isPositional ? substitute(lName, "-", "") : lName), "!", ""), "~", "");
                os << oneDescription(sName, lName, descr, isPositional, isReq);
            }
            if (pParams[i].is_hidden)
                nHidden++;
            if (isTestMode() && isEnabled(OPT_FMT))
                nHidden++;
        }

        // For testing purposes, we show the hidden options
        if (isTestMode() && nHidden) {
            os << "\n#### Hidden options (shown during testing only)\n";
            for (uint64_t i = 0 ; i < cntParams ; i++) {
                string_q sName = pParams[i].shortName;
                string_q lName = pParams[i].longName;
                string_q descr = trim(pParams[i].description);
                bool isPositional = pParams[i].is_positional;
                if (pParams[i].is_hidden && !sName.empty()) {
                    bool isReq = !pParams[i].is_optional;
                    lName = substitute(substitute((isPositional ? substitute(lName, "-", "")
                                 : lName), "!", ""), "~", "");
                    lName = substitute(lName, "@-", "");
                    sName = (isPositional ? "" : pParams[i].shortName);
                    os << oneDescription(sName, lName, descr, isPositional, isReq);
                }
            }
            if (isEnabled(OPT_FMT))
                os << oneDescription(" ", "--fmt <val>",
                    "export format, one of [none|json*|txt|csv|api]", false, false);
            os << "#### Hidden options (shown during testing only)\n\n";
        }

        if (isEnabled(OPT_VERBOSE))
            os << oneDescription("-v", "--verbose",
                "set verbose level. Either -v, --verbose or -v:n where 'n' is level", false, false);
        if (isEnabled(OPT_HELP))
            os << oneDescription("-h", "--help", "display this help screen", false, false);
        return postProcess("description", os.str().c_str());
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::expandOption(string_q& arg) {

        string_q ret = arg;

        // Check that we don't have a regular command with a single dash, which
        // should report an error in client code
        for (uint64_t i = 0 ; i < cntParams ; i++) {
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
    int sortParams(const void *c1, const void *c2) {
        const COption *p1 = reinterpret_cast<const COption*>(c1);
        const COption *p2 = reinterpret_cast<const COption*>(c2);
        if (p1->shortName == "-h")
            return 1;
        else if (p2->shortName == "-h")
            return -1;
        return p1->shortName.compare(p2->shortName);
    }

    //--------------------------------------------------------------------------------
    uint64_t verbose = false;

    //---------------------------------------------------------------------------------------------------
    string_q configPath(const string_q& part) {
        return getHomeFolder() + ".quickBlocks/" + part;
    }

    //------------------------------------------------------------------
    void editFile(const string_q& fileName) {
        CToml toml(configPath("quickBlocks.toml"));
        string_q editor = toml.getConfigStr("dev", "editor", "<NOT_SET>");
        if (!isTestMode() && editor == "<NOT_SET>") {
            editor = getEnvStr("EDITOR");
            if (editor.empty()) {
                cerr << endl;
                cerr << cTeal << "\tWarning: " << cOff;
                cerr << "$EDITOR environment setting not found. Either export it or\n";
                cerr << "\tadd an \"[settings] editor=\" value in the config file." << endl << endl;
                return;
            }
        }

        CFilename fn(fileName);
        string_q cmd = "cd " + fn.getPath() + " ; " + editor + " \"" + fn.getFilename() + "\"";
        if (isTestMode()) {
            cerr << "Testing editFile: " << fn.getFilename() << "\n";
            string_q contents;
            asciiFileToString(fileName, contents);
            cout << contents << "\n";
        } else {
            if (system(cmd.c_str())) { }  // Don't remove. Silences compiler warnings
        }
    }

    //-------------------------------------------------------------------------
    bool COptionsBase::isEnabled(uint32_t q) const {
        return (enableBits & q);
    }

    void COptionsBase::optionOff(uint32_t q) {
        enableBits &= (~q);
    }

    void COptionsBase::optionOn (uint32_t q) {
        enableBits |= q;
    }

    //--------------------------------------------------------------------------------
    int sortByBlockNum(const void *v1, const void *v2) {
        const CNameValue *b1 = reinterpret_cast<const CNameValue *>(v1);
        const CNameValue *b2 = reinterpret_cast<const CNameValue *>(v2);
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
        return (int)(str_2_Uint(b1->second) - str_2_Uint(b2->second));  // NOLINT
    }

    //-----------------------------------------------------------------------
    const CToml *getGlobalConfig(const string_q& name) {
        static CToml *toml = NULL;
        static string_q components = "quickBlocks|";

        if (name == "reload" && toml) {
            toml->clear();
            toml = NULL;
        }

        if (!toml) {
            static CToml theToml("");

            // Forces a reload
            theToml.clear();
            theToml.setFilename(configPath("quickBlocks.toml"));
            theToml.readFile(configPath("quickBlocks.toml"));
            toml = &theToml;

            // Always load the program's custom config if it exists
            string_q fileName = configPath(COptionsBase::g_progName + ".toml");
            if (fileExists(fileName) && !contains(components, COptionsBase::g_progName + "|")) {
                components += COptionsBase::g_progName + "|";
                CToml custom(fileName);
                toml->mergeFile(&custom);
            }
        }

        // If we're told explicitly to load another config, do that here
        if (!name.empty() && name != "reload") {
            string_q fileName = configPath(name+".toml");
            if (fileExists(fileName) && !contains(components, name+"|")) {
                components += name+"|";
                CToml custom(fileName);
                toml->mergeFile(&custom);
            }
        }

        return toml;
    }

    //-----------------------------------------------------------------------
    static bool sortByValue(const CNameValue& p1, const CNameValue& p2) {
        blknum_t b1 = str_2_Uint(p1.second);
        blknum_t b2 = str_2_Uint(p2.second);
        if (b1 == 0) {
            if (p1.first == "latest") b1 = NOPOS;
            if (p1.first == "constantinople") b1 = (NOPOS-2);
        }
        if (b2 == 0) {
            if (p2.first == "latest") b2 = NOPOS;
            if (p2.first == "constantinople") b2 = (NOPOS-2);
        }
        return b1 < b2;
    }

    //-----------------------------------------------------------------------
    // TODO(tjayrush): global data
    CNameValueArray COptionsBase::specials;

    //-----------------------------------------------------------------------
    void COptionsBase::loadSpecials(void) {

        const CToml *toml = getGlobalConfig("whenBlock");
        specials.clear();
        string_q specialsStr = toml->getConfigStr("specials", "list", STR_DEFAULT_WHENBLOCKS);
        CKeyValuePair keyVal;
        while (keyVal.parseJson3(specialsStr)) {
            CNameValue pair = make_pair(keyVal.jsonrpc, keyVal.result);
            specials.push_back(pair);
            keyVal = CKeyValuePair();  // reset
        }
        sort(specials.begin(), specials.end(), sortByValue);
        return;
    }

    //--------------------------------------------------------------------------------
    bool  COptionsBase::forEverySpecialBlock(NAMEVALFUNC func, void *data) {
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

    //---------------------------------------------------------------------------------------------------
    COptionsBase::COptionsBase(void) : namesFile("") {
        minArgs = 1;
        isReadme = false;
        isRaw = false;
        isVeryRaw = false;
        isNoHeader = false;
        exportFmt = (isApiMode() ? API1 : TXT1);
        needsOption = false;
        enableBits = OPT_DEFAULT;
        scanRange = make_pair(0, NOPOS);
        for (int i = 0 ; i < 5 ; i++)
            sorts[i] = NULL;
        hiUp1  = (isTestMode() ? "" : cYellow) + "  ";
        hiUp2  = (isTestMode() ? "" : cTeal);
        hiDown = (isTestMode() ? "" : cOff);
        arguments.clear();
        commandLines.clear();
        namedAccounts.clear();
        pParams = NULL;
        cntParams = 0;
        coutBackup = NULL;
        redirFilename = "";
        // redirStream
    }

    //--------------------------------------------------------------------------------
    COptionsBase::~COptionsBase(void) {
        closeRedirect();
    }

    //-----------------------------------------------------------------------
    void COptionsBase::setSorts(CRuntimeClass *c1, CRuntimeClass *c2, CRuntimeClass *c3) {
        sorts[0] = c1;
        sorts[1] = c2;
        sorts[2] = c3;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::isRedirected(void) const {
        return (coutBackup != NULL);
    }

    //--------------------------------------------------------------------------------
    void COptionsBase::closeRedirect(void) {
        if (coutBackup != NULL) {
            cout.rdbuf( coutBackup );  // restore cout's original streambuf
            redirStream.close();
            coutBackup = NULL;
            cout << redirFilename;
            redirFilename = "";
        }
    }

    //-----------------------------------------------------------------------
    bool COptionsBase::getNamedAccount(CAccountName& acct, const string_q& addr) const {
        if (namedAccounts.size() == 0) {
            uint64_t save = verbose;
            verbose = false;
            if (!contains(namesFile.getFullPath(), "names.txt"))
                ((COptionsBase*)this)->namesFile = CFilename(configPath("names/names.txt"));  // NOLINT
            ((COptionsBase*)this)->loadNames();  // NOLINT
            verbose = save;
        }

        for (size_t i = 0 ; i < namedAccounts.size() ; i++) {
            if (namedAccounts[i].address % addr) {
                acct = namedAccounts[i];
                return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    string_q COptionsBase::getNamedAccount(const string_q& addr) const {
        CAccountName item;
        item.address = addr;
        getNamedAccount(item, item.address);
        return substitute(substitute(item.name, "(", ""), ")", "");
    }

    //-----------------------------------------------------------------------
    bool COptionsBase::loadNames(void) {

        // If we're already loaded or editing, return
        if (namedAccounts.size() > 0)
            return true;

        string_q textFile = namesFile.getFullPath();
        string_q binFile  = substitute(textFile, ".txt", ".bin");

        time_q txtDate = fileLastModifyDate(textFile);
        time_q binDate = fileLastModifyDate(binFile);
        if (verbose && !isTestMode())
            cout << "txtDate: " << txtDate << " binDate: " << binDate << "\n";

        if (binDate > txtDate) {
            CArchive nameCache(READING_ARCHIVE);
            if (nameCache.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
                if (verbose && !isTestMode())
                    cout << "Reading from binary cache\n";
                nameCache >> namedAccounts;
                nameCache.Release();
                return true;
            }
        }

        if (verbose && !isTestMode())
            cout << "Reading from text database\n";

        // Read the data from the names database and clean it up if needed
        CStringArray lines;
        asciiFileToLines(textFile, lines);
        for (auto line : lines) {
            if (!startsWith(line, '#') && contains(line, "0x")) {
                CAccountName account(line);
                if (isAddress(account.address))
                    namedAccounts.push_back(account);
            }
        }
        if (lines.size() == 0) {
            cerr << "Something went wrong loading names file. Quitting...";
            return false;
        }

        CArchive nameCache(WRITING_ARCHIVE);
        if (nameCache.Lock(binFile, modeWriteCreate, LOCK_CREATE)) {
            if (verbose && !isTestMode())
                cout << "Writing binary cache\n";
            nameCache << namedAccounts;
            nameCache.Release();
        }

        return true;
    }

    //-----------------------------------------------------------------------
    string_q cleanFmt(const string_q& str, format_t fmt) {
        string_q ret = (substitute(substitute(substitute(str, "\n", ""), "\\n", "\n"), "\\t", "\t"));
        if (fmt == CSV1)
            ret = "\"" + substitute(ret, "\t", "\",\"") + "\"";
        return ret;
    }

    const char *STR_DEFAULT_WHENBLOCKS =
    "[ { name = \"first\", value = 0 },"
    "{ name = \"firstTrans\", value = 46147 },"
    "{ name = \"iceage\", value = 200000 },"
    "{ name = \"devcon1\", value = 543626 },"
    "{ name = \"homestead\", value = 1150000 },"
    "{ name = \"daofund\", value = 1428756 },"
    "{ name = \"daohack\", value = 1718497 },"
    "{ name = \"daofork\", value = 1920000 },"
    "{ name = \"devcon2\", value = 2286910 },"
    "{ name = \"tangerine\", value = 2463000 },"
    "{ name = \"spurious\", value = 2675000 },"
    "{ name = \"stateclear\", value = 2717576 },"
    "{ name = \"eea\", value = 3265360 },"
    "{ name = \"ens2\", value = 3327417 },"
    "{ name = \"parityhack1\", value = 4041179 },"
    "{ name = \"byzantium\", value = 4370000 },"
    "{ name = \"devcon3\", value = 4469339 },"
    "{ name = \"parityhack2\", value = 4501969 },"
    "{ name = \"kitties\", value = 4605167 },"
    "{ name = \"devcon4\", value = 6610279 },"
    "{ name = \"constantinople\", value = 7280000 },"
    "{ name = \"latest\", value = \"\" }"
    "]";
}  // namespace qblocks
