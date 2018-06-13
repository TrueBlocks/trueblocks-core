/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "namevalue.h"
#include "accountname.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    static uint32_t nP = 0;
    static CParams ps[] = { };
    static CDefaultOptions defOpts;

    //--------------------------------------------------------------------------------
    uint32_t& nParamsRef = nP;
    CParams *paramsPtr  = &ps[0];
    COptionsBase *pOptions = &defOpts;

    //--------------------------------------------------------------------------------
    static string_q programName = "quickBlocks";

    //--------------------------------------------------------------------------------
    bool COptionsBase::prepareArguments(int argc, const char *argv[]) {

        string_q env = getEnvStr("NO_COLOR");
        if (string_q(env) == "true")
            colorsOff();

        programName = basename((char*)argv[0]);
        if (isTestMode()) {
            // we present the data once for clarity...
            cout << programName << " argc: " << argc << " ";
            for (int i=1;i<argc;i++) {
                string_q str = argv[i];
                cout << "[" << i << ":" << trim(str) << "] ";
            }
            cout << "\n";
            // ... and once to use as a command line for copy/paste
            cout << programName << " ";
            for (int i=1;i<argc;i++) {
                string_q str = argv[i];
                cout << trim(str) << " ";
            }
            cout << "\n";
        }

        if ((uint64_t)argc <= minArgs)  // the first arg is the program's name
            return usage("Not enough arguments presented.");

        int nChars = 0;
        for (int i=0; i<argc; i++) {
            nChars += string_q(argv[i]).length();
        }
        uint32_t nArgs = 0;
        string_q *args = new string_q[argc+nChars+2];

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
                    verbose = toUnsigned(arg);
                }
            }
        }

        //-----------------------------------------------------------------------------------
        // Collapse commands that have 'permitted' sub options (i.e. colon ":" args)
        //-----------------------------------------------------------------------------------
        uint32_t curArg = 0;
        for (uint32_t i = 0 ; i < nArgs ; i++) {
            string_q arg = args[i];
            bool combine = false;
            for (uint32_t j = 0 ; j < nParamsRef && !combine ; j++) {
                if (!paramsPtr[j].permitted.empty()) {
                    string_q shortName = paramsPtr[j].shortName;
                    string_q longName  = "-"+paramsPtr[j].longName;
                    if (shortName == arg ||
                        startsWith(longName, arg))
                    {
                        // we want to pull the next parameter into this one since it's a ':' param
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
        commandList = "";
        fromFile = false;
        if (cmdFileName.empty()) {
            for (uint64_t i = 0 ; i < nArgs ; i++)
                commandList += (args[i] + " ");
            commandList += '\n';

        } else {
            fromFile = true;
            string_q contents =  asciiFileToString(cmdFileName);
            replaceAll(contents, "\t", " ");
            replaceAll(contents, "-v", "");
            replaceAll(contents, "-h", "");
            replaceAll(contents, "  ", " ");
            replaceAll(contents, "\\\n", "");
            if (contents.empty()) {
                return usage("Command file '" + cmdFileName + "' is empty. Quitting...");
            }
            if (startsWith(contents, "NOPARSE\n")) {
                commandList = contents;
                nextTokenClear(commandList,'\n');
            } else {
                while (!contents.empty()) {
                    string_q command = trimWhitespace(nextTokenClear(contents, '\n'));
                    if (!command.empty() && !startsWith(command, ";"))  // ignore comments
                        commandList += (command+"\n");
                }
            }
        }
        commandList += stdInCmds;
        replaceAll(commandList, " \n", "\n");
        commandList = trim(commandList, '\n');

        if (args) delete [] args;
        return 1;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::standardOptions(string_q& cmdLine) {
        cmdLine += " ";
        if (contains(cmdLine, "--version ")) {
            cerr << programName << " (quickBlocks) " << getVersionStr() << "\n";
            exit(0);

        } else if (contains(cmdLine, "-h ") || contains(cmdLine, "--help ")) {
            usage();
            exit(0);

        } else if (contains(cmdLine, "--nocolor ")) {
            replaceAll(cmdLine, "--nocolor ","");
            colorsOff();

        } else if (isEnabled(OPT_DENOM) && contains(cmdLine, "--ether " )) {
            replaceAll(cmdLine, "--ether ","");
            expContext().asEther = true;
            expContext().asDollars = false;
            expContext().asWei = false;

        } else if (isEnabled(OPT_DENOM) && contains(cmdLine, "--wei ")) {
            replaceAll(cmdLine, "--wei ","");
            expContext().asEther = false;
            expContext().asDollars = false;
            expContext().asWei = true;

        } else if (isEnabled(OPT_DENOM) && contains(cmdLine, "--dollars ")) {
            replaceAll(cmdLine, "--dollars ","");
            expContext().asEther = false;
            expContext().asDollars = true;
            expContext().asWei = false;

        } else if (isEnabled(OPT_PARITY) && contains(cmdLine, "--parity ")) {
            replaceAll(cmdLine, "--parity ","");
            expContext().spcs = 4;
            expContext().hexNums = true;
            expContext().quoteNums = true;
            expContext().isParity = true;
            for (int i=0;i<5;i++)
                if (sorts[i])
                    sorts[i]->sortFieldList();
        }
        cmdLine = trim(cmdLine);
        return true;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::builtInCmd(const string_q& arg) {
        if (isEnabled(OPT_VERBOSE) && (arg == "-v" || startsWith(arg, "-v:") || startsWith(arg, "--verbose")))
            return true;
        if (isEnabled(OPT_DENOM) && (arg == "--ether" || arg == "--wei" || arg == "--dollars"))
            return true;
        if (isEnabled(OPT_PARITY) && (arg == "--parity"))
            return true;
        if (arg == "-h" || arg == "--help")
            return true;
        if (arg == "--version")
            return true;
        if (arg == "--nocolor")
            return true;
        if (arg == "null")
            return true;
        return false;
    }

    //--------------------------------------------------------------------------------
    CParams::CParams(const string_q& nameIn, const string_q& descr) {
        string_q name = nameIn;

        description = descr;
        string_q dummy;
        if (contains(name, ":<") || contains(name, ":[")) {
            permitted = name;
            name = nextTokenClear(permitted,':');
            // order matters
            if (permitted == "<range>")
                dummy = " range";
            else if (permitted == "<list>")
                dummy = " list";
            else if (permitted == "<fn>")
                dummy = " fn";
            else if (permitted == "<mode>")
                dummy = " mode";
            else if (!permitted.empty())
                dummy = " val";
        }
        if (!name.empty()) {
            shortName = name.substr(0,2);
            if (name.length() > 2)
                longName = name + dummy;

            if (contains(name, "{")) {
                replace(name, "{", "|{");
                nextTokenClear(name, '|');
                shortName += name;

            } else if (contains(name, ":")) {
                nextTokenClear(name, ':');
                shortName += name[0];
                longName = "-" + name + dummy;
            }

            if (contains(longName, "(") && contains(longName, ")")) {
                hotKey = longName;
                nextTokenClear(hotKey,'(');
                hotKey = nextTokenClear(hotKey, ')');
                replaceAny(longName, "()","");
                string_q ss;
                ss = shortName[0];
                shortName = ss + hotKey;
            }
        }
    }

    static string_q sep = "  ";
    static string_q sep2 = "";

    //--------------------------------------------------------------------------------
    int usage(const string_q& errMsg) {
        cerr << usageStr(errMsg);
        return false;
    }

    string_q usageStr(const string_q& errMsg) {

        ostringstream os;
        if (COptionsBase::isReadme) {
            sep = sep2 = "`";
            colorsOff();
            os << "#### Usage\n";
        }

        os << "\n";
        if (!errMsg.empty())
            os << cRed << "  " << errMsg << "\n\n";
        os << bYellow << sep << "Usage:" << sep2 << "    " << cOff << programName << " " << options() << "  \n";
        os << purpose();
        os << descriptions() << "\n";
        os << notes();
        if (!COptionsBase::isReadme)
            os << bBlue << "  Powered by QuickBlocks" << (isTestMode() ? "" : " (" + getVersionStr() + ")") << "\n" << cOff;
        string_q ret = os.str().c_str();
        ASSERT(pOptions);
        return pOptions->postProcess("usage", ret);
    }

    //--------------------------------------------------------------------------------
    string_q options(void) {
        string_q required;

        CStringExportContext ctx;
        if (!COptionsBase::needsOption)
            ctx << "[";
        for (uint64_t i = 0 ; i < nParamsRef ; i++) {
            if (startsWith(paramsPtr[i].shortName, '~')) {
                required += (" " + substitute(paramsPtr[i].longName.substr(1), "!", ""));

            } else if (startsWith(paramsPtr[i].shortName, '@')) {
                // invisible option

            } else if (!paramsPtr[i].shortName.empty()) {
                ctx << paramsPtr[i].shortName << "|";

            } else if (!paramsPtr[i].shortName.empty()) {
                ctx << paramsPtr[i].shortName << "|";
            }
        }
        if (isEnabled(OPT_VERBOSE))
            ctx << "-v|";
        ctx << "-h";
        if (!COptionsBase::needsOption)
            ctx << "]";
        ctx << required;

        ASSERT(pOptions);
        string_q ret = pOptions->postProcess("options", ctx.str);
        if (COptionsBase::isReadme)
            ret = substitute(substitute(ret, "<", "&lt;"), ">", "&gt;");
        return ret;
    }

    //--------------------------------------------------------------------------------
    string_q purpose(void) {
        string_q purpose;
        for (uint64_t i = 0 ; i < nParamsRef ; i++)
            if (paramsPtr[i].shortName.empty())
                purpose += ("\n           " + paramsPtr[i].description);

        ostringstream os;
        if (!purpose.empty()) {
            replace(purpose, "\n           ", "");
            string_q xxx;
            xxx = substitute(purpose, "\n", "\n           ");
            os << bYellow;
            os << sep;
            os << "Purpose:";
            os << sep2;
            os << "  ";
            os << cOff;
            os << xxx;
            os << "  \n";
        }
        ASSERT(pOptions);
#ifdef NATIVE
        return pOptions->postProcess("purpose", os.str());
#else
        return pOptions->postProcess("purpose", os.str().c_str());
#endif
    }

    //--------------------------------------------------------------------------------
const char *STR_ONE_LINE = "| {S} | {L} | {D} |\n";

    string_q oneDescription(const string_q& sN, const string_q& lN, const string_q& d, bool isMode, bool required) {
        ostringstream os;
        if (COptionsBase::isReadme) {

            // When we are writing the readme file...
            string_q line = STR_ONE_LINE;
            replace(line, "{S}", sN);
            replace(line, "{L}", (isMode ? "" : "-") + lN);
            replace(line, "{D}", substitute(d, "|", "&#124;"));
            os << line;

        } else {

            // When we are writing to the command line...
            string_q line = "\t" + substitute(substitute(string_q(STR_ONE_LINE), " ",""), "|","");
            replace(line, "{S}", (isMode ? "" : padRight(sN, 3)));
            if (isMode)
                replace(line, "{L}", padRight(lN , 22));
            else {
                replace(line, "{L}", padRight((lN.empty() ? "" : " (-" + lN + ")") , 19));
            }
            replace(line, "{D}", d + (required ? " (required)" : ""));
            os << line;
        }
        ASSERT(pOptions);
#ifdef NATIVE
        return pOptions->postProcess("oneDescription", os.str());
#else
        return pOptions->postProcess("oneDescription", os.str().c_str());
#endif
    }

    //--------------------------------------------------------------------------------
    string_q notes(void) {
        CStringExportContext ctx;
        ASSERT(pOptions);
        string_q ret = pOptions->postProcess("notes", "");
        if (!ret.empty()) {
            string_q tick = "- ";
            string_q lead = "\t";
            string_q trail = "\n";
            string_q sepy1 = cTeal, sepy2 = cOff;
            if (COptionsBase::isReadme) {
                sepy1 = sepy2 = "`";
                lead = "";
                trail = "\n";
            }
            replaceAll(ret, "[{", sepy1);
            replaceAll(ret, "}]", sepy2);

            ctx << bYellow << sep << "Notes:" << sep << cOff << "\n";
            ctx << (COptionsBase::isReadme ? "\n" : "");
            while (!ret.empty()) {
                string_q line = substitute(nextTokenClear(ret,'\n'), "|","\n" + lead + "  ");
                ctx << lead << tick << line << "\n";
            }
            ctx << "\n";
            replaceAll(ctx.str, "-   ","  - ");
        }
        return ctx.str;
    }

    //--------------------------------------------------------------------------------
    string_q descriptions(void) {
        string_q required;
        CStringExportContext ctx;
        ctx << bYellow << sep << "Where:" << sep << cOff << "  \n";
        if (COptionsBase::isReadme) {
            ctx << "\n";
            ctx << "| Short Cut | Option | Description |\n";
            ctx << "| -------: | :------- | :------- |\n";
        }

        bool showHidden = (getEnvStr("SHOW_HIDDEN_OPTIONS") == "true");
        for (uint64_t i = 0 ; i < nParamsRef ; i++) {
            string_q sName = paramsPtr[i].shortName;
            string_q lName = paramsPtr[i].longName;
            string_q descr = trim(paramsPtr[i].description);
            if (startsWith(sName, '@') && !showHidden) {
                // invisible option

            } else if (!sName.empty()) {
                bool isMode = startsWith(sName, '~');
                // ~ makes the option a required mode, ! makes it not required
                bool isReq = isMode && !contains(lName, '!');
                sName = (isMode ? "" : sName);
                lName = substitute(substitute((isMode ? substitute(lName, "-", "") : lName), "!", ""), "~", "");
                ctx << oneDescription(sName, lName, descr, isMode, isReq);
            }
        }

        if (isEnabled(OPT_VERBOSE))
            ctx << oneDescription("-v", "-verbose", "set verbose level. Either -v, --verbose or -v:n where 'n' is level", false, false);
        ctx << oneDescription("-h", "-help", "display this help screen", false, false);
        ASSERT(pOptions);
        return pOptions->postProcess("description", ctx.str);
    }

    //--------------------------------------------------------------------------------
    string_q expandOption(string_q& arg) {

        string_q ret = arg;

        // Check that we don't have a regular command with a single dash, which
        // should report an error in client code
        for (uint64_t i = 0 ; i < nParamsRef ; i++) {
            if (paramsPtr[i].longName == arg) {
                arg = "";
                return ret;
            }
        }

        CStringExportContext ctx;

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
            COptionsBase::isReadme = true;
            arg = "";
            replaceAll(ret, "-th","");
            replaceAll(ret, "-ht","");
            return ret;
        }

        // This may be a command with two -a -b (or more) single options
        if (arg.length()>2 && arg[2] == ' ') {
            ret = arg.substr(0,2);
            arg = arg.substr(3);
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
        ret = arg.substr(0,2);
        arg = "-"+arg.substr(2);
        return ret;
    }

    //--------------------------------------------------------------------------------
    int sortParams(const void *c1, const void *c2) {
        const CParams *p1 = reinterpret_cast<const CParams*>(c1);
        const CParams *p2 = reinterpret_cast<const CParams*>(c2);
        if (p1->shortName == "-h")
            return 1;
        else if (p2->shortName == "-h")
            return -1;
        return p1->shortName.compare(p2->shortName);
    }

    //--------------------------------------------------------------------------------
    uint32_t COptionsBase::enableBits = OPT_DEFAULT;
    bool COptionsBase::isReadme = false;
    bool COptionsBase::needsOption = false;

    //--------------------------------------------------------------------------------
    uint64_t verbose = false;

    //---------------------------------------------------------------------------------------------------
    string_q configPath(const string_q& part) {
        return getHomeFolder() + ".quickBlocks/" + part;
    }

    //------------------------------------------------------------------
    void editFile(const string_q& fileName) {
        CToml toml(configPath("quickBlocks.toml"));
        string_q editor = toml.getConfigStr("settings", "editor", "open ");
        string_q cmd = editor + " \"" + fileName + "\"";
        if (isTestMode()) {
            cout << "Testing editFile: " << cmd << "\n";
            cout << asciiFileToString(fileName) << "\n";
        } else {
            if (system(cmd.c_str())) {}  // do not remove. Silences compiler warnings
        }
    }

    //-------------------------------------------------------------------------
    bool isEnabled(uint32_t q) {
        return COptionsBase::enableBits & q;
    }

    void optionOff(uint32_t q) {
        COptionsBase::enableBits &= (~q);
    }

    void optionOn (uint32_t q) {
        COptionsBase::enableBits |= q;
    }

    //--------------------------------------------------------------------------------
    int sortByBlockNum(const void *v1, const void *v2) {
        CNameValue *b1 = (CNameValue *)v1;  // NOLINT
        CNameValue *b2 = (CNameValue *)v2;  // NOLINT
        if (b1->getName() == "latest")
            return 1;
        if (b2->getName() == "latest")
            return -1;
        if (contains(b1->getValue(), "tbd") && contains(b1->getValue(), "tbd"))
            return b1->getValue().compare(b2->getValue());
        if (contains(b1->getValue(), "tbd"))
            return 1;
        if (contains(b2->getValue(), "tbd"))
            return -1;
        return (int)(b1->getValueU() - b2->getValueU());
    }

    //-----------------------------------------------------------------------
    const CToml *getGlobalConfig(const string_q& name) {
        static CToml *toml = NULL;
        static string_q components = "quickBlocks|";
        if (!toml) {
            static CToml theToml(configPath("quickBlocks.toml"));
            toml = &theToml;
            // Always load the program's custom config if it exists
            string_q fileName = configPath(programName+".toml");
            if (fileExists(fileName) && !contains(components, programName+"|")) {
                components += programName+"|";
                CToml custom(fileName);
                toml->mergeFile(&custom);
            }
        }

        // If we're told explicitly to load another config, do that here
        if (!name.empty()) {
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
    void COptionsBase::loadSpecials(void) {

        const CToml *toml = getGlobalConfig("whenBlock");

        specials.Clear();
        string_q specialsStr = toml->getConfigStr("specials", "list", "");
        char *p = cleanUpJson((char *)specialsStr.c_str());
        while (p && *p) {
            CNameValue pair;
            uint32_t nFields = 0;
            p = pair.parseJson(p, nFields);
            if (nFields) {
                specials[specials.getCount()] = pair;
            }
        }
        specials.Sort(sortByBlockNum);
        return;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::findSpecial(CNameValue& pair, const string_q& arg) const {
        if (specials.getCount() == 0)
            ((COptionsBase*)this)->loadSpecials();
        for (uint32_t i = 0 ; i < specials.getCount() ; i++) {
            if (arg == specials[i].getName()) {
                pair = specials[i];
                return true;
            }
        }
        return false;
    }

    //---------------------------------------------------------------------------------------------------
    COptionsBase::COptionsBase(void) : namesFile("") {
        fromFile = false;
        minArgs = 1;
        isReadme = false;
        needsOption = false;
        for (int i=0;i<5;i++)
            sorts[i] = NULL;
    }

    //-----------------------------------------------------------------------
    bool COptionsBase::getNamedAccount(CAccountName& acct, const string_q& addr) const {
        if (namedAccounts.getCount() == 0) {
            uint64_t save = verbose;
            verbose = false;
            ((COptionsBase*)this)->loadNames();
            verbose = save;
        }

        for (uint32_t i = 0 ; i < namedAccounts.getCount() ; i++) {
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
        if (namedAccounts.getCount() > 0)
            return true;

        string_q textFile = namesFile.getFullPath();
        string_q binFile  = substitute(textFile, ".txt", ".bin");

        SFTime txtDate = fileLastModifyDate(textFile);
        SFTime binDate = fileLastModifyDate(binFile);

        if (verbose && !isTestMode())
            cout << "txtDate: " << txtDate << " binDate: " << binDate << "\n";

        if (binDate > txtDate) {
            SFArchive nameCache(READING_ARCHIVE);
            if (nameCache.Lock(binFile, binaryReadOnly, LOCK_NOWAIT)) {
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
        string_q contents = trimWhitespace(asciiFileToString(textFile));
        replaceAll(contents, "\t\t", "\t");
        if (!endsWith(contents, "\n"))
            contents += "\n";

        // Parse out the data....
        while (!contents.empty()) {
            string_q line = nextTokenClear(contents, '\n');
            if (!startsWith(line, "#")) {
                if (countOf(line, '\t') < 2) {
                    cerr << "Line " << line << " does not contain two tabs.\n";

                } else {
                    CAccountName account(line);
                    namedAccounts[namedAccounts.getCount()] = account;
                }
            }
        }

        SFArchive nameCache(WRITING_ARCHIVE);
        if (nameCache.Lock(binFile, binaryWriteCreate, LOCK_CREATE)) {
            if (verbose && !isTestMode())
                cout << "Writing binary cache\n";
            nameCache << namedAccounts;
            nameCache.Release();
        }

        return true;
    }

    const char *STR_DEFAULT_DATA =
    "#---------------------------------------------------------------------------------------------------\n"
    "#  This is the ethName database. Format records as tab separated lines with the following format:\n"
    "#\n"
    "#      Optional Symbol <tab> Name <tab> Address <tab> Source of the data <tab> Description <newline>\n"
    "#\n"
    "#---------------------------------------------------------------------------------------------------------------\n"
    "# Sym    Name                Address                        Source        Description\n"
    "#---------------------------------------------------------------------------------------------------------------\n"
    "#\n"
    "#Add your names here or remove this file and run \"ethName --list\"\n";

}  // namespace qblocks
