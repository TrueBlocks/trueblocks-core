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
        //TODO(tjayrush): global data
        arguments.clear();
        cntParams = nP;
        pParams = pP;
    }

    //--------------------------------------------------------------------------------
    //TODO(tjayrush): global data - but okay, a program only has one name
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
    bool COptionsBase::prepareArguments(int argc, const char *argv[]) {

        string_q env = getEnvStr("NO_COLOR");
        if (string_q(env) == "true")
            colorsOff();

        setProgName(basename((char*)argv[0]));  // NOLINT
        if (isTestMode()) {
            // we present the data once for clarity...
            cout << getProgName() << " argc: " << argc << " ";
            for (int i = 1 ; i < argc ; i++) {
                string_q str = argv[i];
                cout << "[" << i << ":" << trim(str) << "] ";
            }
            cout << "\n";
            // ... and once to use as a command line for copy/paste
            cout << getProgName() << " ";
            for (int i = 1 ; i < argc ; i++) {
                string_q str = argv[i];
                cout << trim(str) << " ";
            }
            cout << "\n";
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
            } else if (startsWith(arg, "-v") || startsWith(arg, "--verbose")) {
                verbose = true;
                arg = substitute(substitute(substitute(arg, "-v", ""), "--verbose", ""), ":", "");
                if (!arg.empty()) {
                    if (!isUnsigned(arg))
                        return usage("Invalid verbose level '" + arg + "'. Quitting...");
                    verbose = str_2_Uint(arg);
                }
            }
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
            if (combine && i < (nArgs-1)) {
                args[curArg++] = arg + ":" + args[i+1];
                i++;

            } else {
                args[curArg++] = arg;
            }
        }
        nArgs = curArg;

        // If we have a command file, we will use it, if not we will creat one and pretend we have one.
        string_q commandList = "";
        for (uint64_t i = 0 ; i < nArgs ; i++) {
            string_q a = args[i];
            if (!contains(a, "--file"))
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

        if (contains(cmdLine, "--noop ")) {
            // do nothing
            replaceAll(cmdLine, "--noop ", "");
        }

        if (contains(cmdLine, "--version ")) {
            cerr << getProgName() << " (quickBlocks) " << getVersionStr() << "\n";
            exit(0);
        }

        if (contains(cmdLine, "--nocolor ")) {
            replaceAll(cmdLine, "--nocolor ", "");
            colorsOff();
        }

        if (contains(cmdLine, "--qblocks ")) {
            replaceAll(cmdLine, "--qblocks ", "");
            CNameValue toolName;
            findToolNickname(toolName, getProgName());
            setProgName("qblocks " + toolName.second);

        }

        if (isEnabled(OPT_HELP) && (contains(cmdLine, "-h ") || contains(cmdLine, "--help "))) {
            usage();
            exit(0);

        }

#ifdef PROVING
        if (isEnabled(OPT_PROVE) && contains(cmdLine, "--prove ")) {
            replaceAll(cmdLine, "--prove ", "");
            expContext().proving = true;
        }

        if (isEnabled(OPT_VERIFY) && contains(cmdLine, "--verify ")) {
            replaceAll(cmdLine, "--verify ", "");
            expContext().verifying = true;
        }
#endif

        if (isEnabled(OPT_ETHER) && contains(cmdLine, "--ether " )) {
            replaceAll(cmdLine, "--ether ", "");
            expContext().asEther = true;
            expContext().asDollars = false;
            expContext().asWei = false;
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
            expContext().spcs = 4;
            expContext().hexNums = true;
            expContext().quoteNums = true;
            expContext().isParity = true;
            for (int i = 0 ; i < 5 ; i++)
                if (sorts[i])
                    sorts[i]->sortFieldList();
        }

        // A final set of checks
        if (isEnabled(OPT_RUNONCE)) {
            if (commandLines.size() > 1)
                return usage("You may not use the --file with this application. Quitting...");
            // protect ourselves from running twice over
            string_q cmd = "ps -ef | grep -i " + getProgName() + " | grep -v grep | grep -v \"sh -c \" | wc -l";
            uint64_t cnt = str_2_Uint(doCommand(cmd));
            if (cnt > 1)  // it is running itself, so if two or more, quit
                return usage("Warning: the command " + getProgName() + " is already running. Quitting...");
        }

        cmdLine = trim(cmdLine);
        return true;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::builtInCmd(const string_q& arg) {
        if (isEnabled(OPT_HELP) && (arg == "-h" || arg == "--help"))
            return true;
        if (isEnabled(OPT_VERBOSE) && (arg == "-v" || startsWith(arg, "-v:") || startsWith(arg, "--verbose")))
            return true;
#ifdef PROVING
        if (isEnabled(OPT_PROVE) && arg == "--prove")
            return true;
        if (isEnabled(OPT_VERIFY) && arg == "--verify")
            return true;
#endif
        if (isEnabled(OPT_ETHER) && arg == "--ether")
            return true;
        if (isEnabled(OPT_WEI) && arg == "--wei")
            return true;
        if (isEnabled(OPT_DOLLARS) && arg == "--dollars")
            return true;
        if (isEnabled(OPT_PARITY) && (arg == "--parity"))
            return true;
        if (arg == "--version")
            return true;
        if (arg == "--qblocks")
            return true;
        if (arg == "--nocolor")
            return true;
        if (arg == "--noop")
            return true;
        return false;
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

        description = descr;
        if (nameIn.empty())
            return;

        hidden      = startsWith(nameIn, "@");
        mode        = startsWith(nameIn, "~");
        optional    = contains  (nameIn, "!");

        shortName   = nameIn;
        replaceAll(shortName, "-", "");
        replaceAll(shortName, "~", "");
        replaceAll(shortName, "@", "");
        replaceAll(shortName, "!", "");

        if (!mode) {
            longName = "--" + shortName;
            shortName = "-" + extract(shortName, 0, 1);
        } else {
            longName = shortName;
        }

        if (contains(longName, ":")) {
            permitted = longName;
            longName = nextTokenClear(permitted, ':');
            replaceAny(permitted, "<>", "");
            if (permitted != "range" && permitted != "list" && permitted != "fn" && permitted != "mode")
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
    bool COptionsBase::usage(const string_q& errMsg) const {
        cerr << usageStr(errMsg);
        return false;
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::usageStr(const string_q& errMsg) const {

        ostringstream os;
        if (isReadme) {
            colorsOff();
            os << "#### Usage\n";
        }

        os << "\n";
        if (!errMsg.empty())
            os << cRed << "  " << errMsg << "\n\n";
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
            if (pParams[i].mode) {
                required += (" " + pParams[i].longName);

            } else if (pParams[i].hidden) {
                // invisible option

            } else if (!pParams[i].shortName.empty()) {
                os << pParams[i].shortName << "|";

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

        string_q ret = postProcess("options", os.str().c_str());
        if (isReadme)
            ret = substitute(substitute(ret, "<", "&lt;"), ">", "&gt;");
        return ret;
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::purpose(void) const {
        string_q purpose;
        for (uint64_t i = 0 ; i < cntParams ; i++)
            if (pParams[i].shortName.empty())
                purpose += ("\n           " + pParams[i].description);

        ostringstream os;
        if (!purpose.empty()) {
            replace(purpose, "\n           ", "");
            string_q xxx;
            xxx = substitute(purpose, "\n", "\n           ");
            os << hiUp1 << "Purpose:" << hiDown << "  ";
            os << xxx;
            os << "  \n";
        }
        return postProcess("purpose", os.str().c_str());
    }

    //--------------------------------------------------------------------------------
const char *STR_ONE_LINE = "| {S} | {L} | {D} |\n";

    string_q COptionsBase::oneDescription(const string_q& sN, const string_q& lN, const string_q& d, bool isMode, bool required) const {
        ostringstream os;
        if (isReadme) {

            // When we are writing the readme file...
            string_q line = STR_ONE_LINE;
            replace(line, "{S}", sN);
            replace(line, "{L}", lN);
            replace(line, "{D}", substitute(d, "|", "&#124;"));
            os << line;

        } else {

            // When we are writing to the command line...
            string_q line = "\t" + substitute(substitute(string_q(STR_ONE_LINE), " ", ""), "|", "");
            replace(line, "{S}", (isMode ? "" : padRight(sN, 3)));
            if (isMode) {
                replace(line, "{L}", padRight(lN , 22));
            } else {
                replace(line, "{L}", padRight((lN.empty() ? "" : " (" + lN + ")") , 19));
            }
            replace(line, "{D}", d + (required && isMode ? " (required)" : ""));
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
            bool isMode = pParams[i].mode;
            if (!pParams[i].hidden && !sName.empty()) {
                bool isReq = !pParams[i].optional;
                sName = (isMode ? "" : sName);
                lName = substitute(substitute((isMode ? substitute(lName, "-", "") : lName), "!", ""), "~", "");
                os << oneDescription(sName, lName, descr, isMode, isReq);
            }
            if (pParams[i].hidden)
                nHidden++;
        }

        // For testing purposes, we show the hidden options
        if (isTestMode() && nHidden) {
            os << "\n#### Hidden options (shown during testing only)\n";
            for (uint64_t i = 0 ; i < cntParams ; i++) {
                string_q sName = pParams[i].shortName;
                string_q lName = pParams[i].longName;
                string_q descr = trim(pParams[i].description);
                bool isMode = pParams[i].mode;
                if (pParams[i].hidden && !sName.empty()) {
                    bool isReq = !pParams[i].optional;
                    lName = substitute(substitute((isMode ? substitute(lName, "-", "") : lName), "!", ""), "~", "");
                    lName = substitute(lName, "@-", "");
                    sName = (isMode ? "" : pParams[i].shortName);
                    os << oneDescription(sName, lName, descr, isMode, isReq);
                }
            }
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
        string_q editor = toml.getConfigStr("settings", "editor", "<NOT_SET>");
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
        string_q cmd = editor + " \"" + fileName + "\"";
        if (isTestMode()) {
            cout << "Testing editFile: " << substitute(cmd, "nano", "open") << "\n";
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
    void COptionsBase::loadToolNames(void) {
        const CToml *toml = getGlobalConfig();
        tools.clear();

        string_q toolStr = toml->getConfigStr("tools", "list", "<not_set>");
        if (toolStr == "<not_set>") {
            CToml lToml(configPath("quickBlocks.toml"));
            lToml.setConfigArray("tools", "list", STR_DEFAULT_TOOLNAMES);
            lToml.writeFile();
            toml = getGlobalConfig("reload");
            toolStr = toml->getConfigStr("tools", "list", "");
        }
        CKeyValuePair keyVal;
        while (keyVal.parseJson3(toolStr)) {
            CNameValue pair = make_pair(keyVal.jsonrpc, keyVal.result);
            tools.push_back(pair);
            keyVal = CKeyValuePair();  // reset
        }
        return;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::findToolName(CNameValue& pair, const string_q& nickname) const {
        if (tools.size() == 0)
            ((COptionsBase*)this)->loadToolNames();  // NOLINT
        for (auto tool : tools) {
            if (nickname == tool.second) {
                pair = tool;
                return true;
            }
        }
        return false;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::findToolNickname(CNameValue& pair, const string_q& name) const {
        if (tools.size() == 0)
            ((COptionsBase*)this)->loadToolNames();  // NOLINT
        for (auto tool : tools) {
            if (name == tool.first) {
                pair = tool;
                return true;
            }
        }
        return false;
    }

    //--------------------------------------------------------------------------------
    string_q COptionsBase::toolNicknames(void) const {
        if (tools.size() == 0)
            ((COptionsBase*)this)->loadToolNames();  // NOLINT
        string_q ret;
        for (auto tool : tools)
            ret += tool.second + "|";
        return ret;
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
    //TODO(tjayrush): global data
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
        needsOption = false;
        enableBits = OPT_DEFAULT;
        for (int i = 0 ; i < 5 ; i++)
            sorts[i] = NULL;
        hiUp1  = (isTestMode() ? "" : cYellow) + "  ";
        hiUp2  = (isTestMode() ? "" : cTeal);
        hiDown = (isTestMode() ? "" : cOff);
    }

#define TEXT_NAMES
    //-----------------------------------------------------------------------
    bool COptionsBase::getNamedAccount(CAccountName& acct, const string_q& addr) const {
        if (namedAccounts.size() == 0) {
            uint64_t save = verbose;
            verbose = false;
#ifdef TEXT_NAMES
            if (!contains(namesFile.getFullPath(), "names.txt"))
                ((COptionsBase*)this)->namesFile = CFilename(configPath("names/names.txt"));
#else
            if (!contains(namesFile.getFullPath(), "names.json"))
                ((COptionsBase*)this)->namesFile = CFilename(configPath("names/names.json"));
#endif
            ((COptionsBase*)this)->loadNames();  // NOLINT
            verbose = save;
        }

        for (size_t i = 0 ; i < namedAccounts.size() ; i++) {
            if (namedAccounts[i].addr % addr) {
                acct = namedAccounts[i];
                return true;
            }
        }
        return false;
    }

    //-----------------------------------------------------------------------
    bool COptionsBase::loadNames(void) {

        // If we're already loaded or editing, return
        if (namedAccounts.size() > 0)
            return true;

        string_q textFile = namesFile.getFullPath();
#ifdef TEXT_NAMES
        string_q binFile  = substitute(textFile, ".txt", ".bin");
#else
        string_q binFile  = substitute(textFile, ".json", ".bin");
#endif

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

#ifdef TEXT_NAMES
        if (verbose && !isTestMode())
            cout << "Reading from text database\n";
#else
        if (verbose && !isTestMode())
            cout << "Reading from the json database\n";
#endif
        // Read the data from the names database and clean it up if needed
        string_q contents;
        asciiFileToString(textFile, contents);
#ifdef TEXT_NAMES
        contents = trimWhitespace(contents);
        replaceAll(contents, "\t\t", "\t");
        if (!endsWith(contents, "\n"))
            contents += "\n";

        // Parse out the data....
        while (!contents.empty()) {
            string_q line = nextTokenClear(contents, '\n');
            if (!startsWith(line, "#") && !line.empty()) {
                if (countOf(line, '\t') < 2) {
                    cerr << "Line " << line << " does not contain two tabs.\n";

                } else {
                    CAccountName account(line);
                    namedAccounts.push_back(account);
                }
            }
        }
#else
        CAccountName item;
        while (item.parseJson3(contents)) {
            namedAccounts.push_back(item);
            item = CAccountName();  // reset
        }
#endif

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
    const char *STR_DEFAULT_NAMEDATA =
    "#---------------------------------------------------------------------------------------------------\n"
#ifdef TEXT_NAMES
    "#  This is the ethName database. Format records as tab separated lines with the following format:\n"
    "#\n"
    "#      Optional Symbol <tab> Name <tab> Address <tab> Source of the data <tab> Description <newline>\n"
    "#\n"
    "#---------------------------------------------------------------------------------------------------------------\n"
    "# Sym    Name                Address                        Source        Description\n"
    "#---------------------------------------------------------------------------------------------------------------\n"
    "#\n"
    "#Add your names here or remove this file and run \"ethName --list\"\n";
#else
    "# Add JSON data to this file (remove these comments). QB will use them in the ethName tool and\n"
    "# to name addresses during export.\n"
    "#\n"
    "#  [\n"
    "#    {\n"
    "#        \"addr\": \"An Ethereum address (leading '0x')\",\n"
    "#        \"symbol\": \"The token symbol (if applicable) found at this address\",\n"
    "#        \"source\": \"An optional source for this information\",\n"
    "#        \"name\": \"The name you wish to give to this address\",\n"
    "#        \"logo\": \"A logo to display for this address\",\n"
    "#        \"description\": \"A description of this address\",\n"
    "#        \"visible\": 'true' if you wish to display this address name in the export, 'false' otherwise\n"
    "#    }\n"
    "#  ]\n"
    "#---------------------------------------------------------------------------------------------------\n";
#endif

    //-----------------------------------------------------------------------
    const char *STR_DEFAULT_TOOLNAMES =
    "[\n"
    "    { name = \"getBlock\",       value = \"block\"       },\n"
    "    { name = \"getTrans\",       value = \"trans\"       },\n"
    "    { name = \"getReceipt\",     value = \"receipt\"     },\n"
    "    { name = \"getLogs\",        value = \"logs\"        },\n"
    "    { name = \"getTrace\",       value = \"trace\"       },\n"
    "    { name = \"getBloom\",       value = \"bloom\"       },\n"
    "    { name = \"getAccounts\",    value = \"accounts\"    },\n"
    "    { name = \"getBalance\",     value = \"balance\"     },\n"
    "    { name = \"getTokenInfo\",   value = \"tokeninfo\"   },\n"
    "    { name = \"getAccountInfo\", value = \"accountinfo\" },\n"
    "    { name = \"ethslurp\",       value = \"slurp\"       },\n"
    "    { name = \"grabABI\",        value = \"grab\"        },\n"
    "    { name = \"ethprice\",       value = \"price\"       },\n"
    "    { name = \"ethName\",        value = \"name\"        },\n"
    "    { name = \"whenBlock\",      value = \"when\"        },\n"
    "    { name = \"whereBlock\",     value = \"where\"       },\n"
    "]";
    //   Tools not added included for searching: makeClass, acctExport, acctScrape,
    //   blockMan, bloomMan, cacheMan, blockScrape, chifra, miniBlocks

    const char *STR_DEFAULT_WHENBLOCKS =
    "[\n"
    "    { name : \"first\",          value : \"0\"       },\n"
    "    { name : \"firstTrans\",     value : \"46147\"   },\n"
    "    { name : \"iceage\",         value : \"200000\"  },\n"
    "    { name : \"devcon1\",        value : \"543626\"  },\n"
    "    { name : \"homestead\",      value : \"1150000\" },\n"
    "    { name : \"daofund\",        value : \"1428756\" },\n"
    "    { name : \"daohack\",        value : \"1718497\" },\n"
    "    { name : \"daofork\",        value : \"1920000\" },\n"
    "    { name : \"devcon2\",        value : \"2286910\" },\n"
    "    { name : \"tangerine\",      value : \"2463000\" },\n"
    "    { name : \"spurious\",       value : \"2675000\" },\n"
    "    { name : \"stateclear\",     value : \"2717576\" },\n"
    "    { name : \"eea\",            value : \"3265360\" },\n"
    "    { name : \"ens2\",           value : \"3327417\" },\n"
    "    { name : \"parityhack1\",    value : \"4041179\" },\n"
    "    { name : \"byzantium\",      value : \"4370000\" },\n"
    "    { name : \"devcon3\",        value : \"4469339\" },\n"
    "    { name : \"parityhack2\",    value : \"4501969\" },\n"
    "    { name : \"kitties\",        value : \"4605167\" },\n"
    "    { name : \"devcon4\",        value : \"6610279\" },\n"
    "    { name : \"constantinople\", value : \"7280000\" },\n"
    "    { name : \"latest\",         value : \"\"        }\n"
    "]";
}  // namespace qblocks
