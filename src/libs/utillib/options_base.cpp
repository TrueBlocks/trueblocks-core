/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "sfstring.h"
#include "database.h"
#include "exportcontext.h"
#include "options_base.h"
#include "colors.h"
#include "filenames.h"
#include "toml.h"
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
    static SFString programName = "quickBlocks";

    //--------------------------------------------------------------------------------
    bool COptionsBase::prepareArguments(int argc, const char *argv[]) {

        if (SFString(getenv("NO_COLOR")) == "true")
            colorsOff();

        programName = basename((char*)argv[0]);
        if (isTestMode()) {
            // we present the data once for clarity...
            cout << programName << " argc: " << argc << " ";
            for (int i=1;i<argc;i++)
                cout << "[" << i << ":" << Strip(argv[i], ' ') << "] ";
            cout << "\n";
            // ... and once to use as a command line for copy/paste
            cout << programName << " ";
            for (int i=1;i<argc;i++)
                cout << Strip(argv[i], ' ') << " ";
            cout << "\n";
        }

        if ((SFUint32)argc <= minArgs)  // the first arg is the program's name
            return usage("Not enough arguments presented.");

        int nChars = 0;
        for (int i=0; i<argc; i++) {
            nChars += SFString(argv[i]).length();
        }
        uint32_t nArgs = 0;
        SFString *args = new SFString[argc+nChars+2];

        bool hasStdIn = false;
        for (int i = 1 ; i < argc ; i++) {
            SFString arg = Strip(argv[i], ' ');
            arg.Replace("--verbose", "-v");
            while (!arg.empty()) {
                SFString opt = expandOption(arg);  // handles case of -rf for example
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

        SFString stdInCmds;
        if (hasStdIn) {
            // reading from stdin, expect only a list of addresses, one per line.
            char c = static_cast<char>(getchar());
            while (c != EOF) {
                stdInCmds += c;
                c = static_cast<char>(getchar());
            }
            if (!stdInCmds.endsWith("\n"))
                stdInCmds += "\n";
        }

        //-----------------------------------------------------------------------------------
        // We now have 'nArgs' command line arguments stored in the array 'args.'  We spin
        // through them doing one of two things
        //
        // (1) handle any arguments common to all programs and remove them from the array
        // (2) identify any --file arguments and store them for later use
        //-----------------------------------------------------------------------------------
        SFString cmdFileName = "";
        for (SFUint32 i = 0 ; i < nArgs ; i++) {
            SFString arg = args[i];
            if (arg.startsWith("--file:")) {
                cmdFileName = arg.Substitute("--file:", "");
                cmdFileName.Replace("~/", getHomeFolder());
                if (!fileExists(cmdFileName)) {
                    if (args) delete [] args;
                    return usage("--file: '" + cmdFileName + "' not found. Quitting.");
                }
            } else if (arg.startsWith("-v") || arg.startsWith("--verbose")) {
                verbose = true;
                arg = arg.Substitute("-v", "").Substitute("--verbose", "").Substitute(":", "");
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
            SFString arg = args[i];
            bool combine = false;
            for (uint32_t j = 0 ; j < nParamsRef && !combine ; j++) {
                if (!paramsPtr[j].permitted.empty()) {
                    SFString shortName = paramsPtr[j].shortName;
                    SFString longName  = "-"+paramsPtr[j].longName;
                    if (shortName == arg ||
                        longName.startsWith(arg))
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
            for (SFUint32 i = 0 ; i < nArgs ; i++)
                commandList += (args[i] + " ");
            commandList += '\n';

        } else {
            fromFile = true;
            SFString contents =  asciiFileToString(cmdFileName).Substitute("\t", " ").
                                            Substitute("-v", "").Substitute("-h", "").
                                            Substitute("  ", " ").Substitute("\\\n", "");
            if (contents.empty()) {
                return usage("Command file '" + cmdFileName + "' is empty. Quitting...");
            }
            while (!contents.empty()) {
                SFString command = StripAny(nextTokenClear(contents, '\n'), "\t\r\n ");
                if (!command.empty() && !command.startsWith(";"))  // ignore comments
                    commandList += (command+"\n");
            }
        }
        commandList += stdInCmds;
        commandList.ReplaceAll(" \n", "\n");
        commandList = Strip(commandList, '\n');

        if (args) delete [] args;
        return 1;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::standardOptions(SFString& cmdLine) {
        cmdLine += " ";
        if (cmdLine.Contains("--version ")) {
            cerr << programName << " (quickBlocks) " << getVersionStr() << "\n";
            exit(0);

        } else if (cmdLine.Contains("-h ") || cmdLine.Contains("--help ")) {
            usage();
            exit(0);

        } else if (cmdLine.Contains("--nocolors ") || cmdLine.Contains("--nocolor ")) {
            cmdLine.ReplaceAll("--nocolors ","");
            cmdLine.ReplaceAll("--nocolor ","");
            colorsOff();

        } else if (isEnabled(OPT_DENOM) && cmdLine.Contains("--ether " )) {
            cmdLine.ReplaceAll("--ether ","");
            expContext().asEther = true;
            expContext().asDollars = false;
            expContext().asWei = false;

        } else if (isEnabled(OPT_DENOM) && cmdLine.Contains("--wei ")) {
            cmdLine.ReplaceAll("--wei ","");
            expContext().asEther = false;
            expContext().asDollars = false;
            expContext().asWei = true;

        } else if (isEnabled(OPT_DENOM) && cmdLine.Contains("--dollars ")) {
            cmdLine.ReplaceAll("--dollars ","");
            expContext().asEther = false;
            expContext().asDollars = true;
            expContext().asWei = false;
        }
        cmdLine = Strip(cmdLine, ' ');
        return true;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBase::builtInCmd(const SFString& arg) {
        if (isEnabled(OPT_VERBOSE) && (arg == "-v" || arg.startsWith("-v:") || arg.startsWith("--verbose")))
            return true;
        if (isEnabled(OPT_DENOM) && (arg == "--ether" || arg == "--wei" || arg == "--dollars"))
            return true;
        if (arg == "-h" || arg == "--help")
            return true;
        if (arg == "--version")
            return true;
        if (arg == "--nocolors" || arg == "--nocolor")
            return true;
        if (arg == "null")
            return true;
        return false;
    }

    //--------------------------------------------------------------------------------
    CParams::CParams(const SFString& nameIn, const SFString& descr) {
        SFString name = nameIn;

        description = descr;
        SFString dummy;
        if (name.Contains(":<") || name.Contains(":[")) {
            permitted = name;
            name = nextTokenClear(permitted,':');
            // order matters
            if (permitted == "<range>")
                dummy = " range";
            else if (permitted == "<list>")
                dummy = " list";
            else if (permitted == "<fn>")
                dummy = " fn";
            else if (!permitted.empty())
                dummy = " val";
        }
        if (!name.empty()) {
            shortName = name.Left(2);
            if (name.length() > 2)
                longName = name + dummy;

            if (name.Contains("{")) {
                name.Replace("{", "|{");
                nextTokenClear(name, '|');
                shortName += name;

            } else if (name.Contains(":")) {
                nextTokenClear(name, ':');
                shortName += name[0];
                longName = "-" + name + dummy;
            }

            if (longName.Contains("(") && longName.Contains(")")) {
                hotKey = longName;
                nextTokenClear(hotKey,'(');
                hotKey = nextTokenClear(hotKey, ')');
                longName.ReplaceAny("()","");
                shortName = SFString(shortName[0]) + hotKey;
            }
        }
    }

    static SFString sep = "  ";
    static SFString sep2 = "";

    //--------------------------------------------------------------------------------
    int usage(const SFString& errMsg) {
        cerr << usageStr(errMsg);
        return false;
    }

    SFString usageStr(const SFString& errMsg) {

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
        SFString ret = os.str().c_str();
        ASSERT(pOptions);
        return pOptions->postProcess("usage", ret);
    }

    //--------------------------------------------------------------------------------
    SFString options(void) {
        SFString required;

        CStringExportContext ctx;
        if (!COptionsBase::needsOption)
            ctx << "[";
        for (SFUint32 i = 0 ; i < nParamsRef ; i++) {
            if (paramsPtr[i].shortName.startsWith('~')) {
                required += (" " + paramsPtr[i].longName.substr(1).Substitute("!", ""));

            } else if (paramsPtr[i].shortName.startsWith('@')) {
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
        SFString ret = pOptions->postProcess("options", ctx.str);
        if (COptionsBase::isReadme)
            ret = ret.Substitute("<", "&lt;").Substitute(">", "&gt;");
        return ret;
    }

    //--------------------------------------------------------------------------------
    SFString purpose(void) {
        SFString purpose;
        for (SFUint32 i = 0 ; i < nParamsRef ; i++)
            if (paramsPtr[i].shortName.empty())
                purpose += ("\n           " + paramsPtr[i].description);

        CStringExportContext ctx;
        if (!purpose.empty()) {
            purpose.Replace("\n           ", "");
            ctx << bYellow << sep << "Purpose:" << sep2 << "  " << cOff
                << purpose.Substitute("\n", "\n           ") << "  \n";
        }
        ASSERT(pOptions);
        return pOptions->postProcess("purpose", ctx.str);
    }

    //--------------------------------------------------------------------------------
const char *STR_ONE_LINE = "| {S} | {L} | {D} |\n";

    SFString oneDescription(const SFString& sN, const SFString& lN, const SFString& d, bool isMode, bool required) {
        CStringExportContext ctx;
        if (COptionsBase::isReadme) {

            // When we are writing the readme file...
            SFString line = STR_ONE_LINE;
            line.Replace("{S}", sN);
            line.Replace("{L}", (isMode ? "" : "-") + lN);
            line.Replace("{D}", d.Substitute("|", "&#124;"));
            ctx << line;

        } else {

            // When we are writing to the command line...
            SFString line = "\t" + SFString(STR_ONE_LINE).Substitute(" ","").Substitute("|","");
            line.Replace("{S}", (isMode ? "" : padRight(sN, 3)));
            if (isMode)
                line.Replace("{L}", padRight(lN , 22));
            else {
                line.Replace("{L}", padRight((lN.empty() ? "" : " (-" + lN + ")") , 19));
            }
            line.Replace("{D}", d + (required ? " (required)" : ""));
            ctx << line;
        }
        ASSERT(pOptions);
        return pOptions->postProcess("oneDescription", ctx.str);
    }

    //--------------------------------------------------------------------------------
    SFString notes(void) {
        CStringExportContext ctx;
        ASSERT(pOptions);
        SFString ret = pOptions->postProcess("notes", "");
        if (!ret.empty()) {
            SFString tick = "- ";
            SFString lead = "\t";
            SFString trail = "\n";
            SFString sep1, sep2;
            if (COptionsBase::isReadme) {
                sep1 = sep2 = "`";
                lead = "";
                trail = "\n";
            }
            ret.ReplaceAll("[{", sep1);
            ret.ReplaceAll("}]", sep2);

            ctx << bYellow << sep << "Notes:" << sep << cOff << "\n";
            ctx << (COptionsBase::isReadme ? "\n" : "");
            while (!ret.empty()) {
                SFString line = nextTokenClear(ret,'\n');
                ctx << lead << tick << line << "\n";
            }
            ctx << "\n";
        }
        return ctx.str;
    }

    //--------------------------------------------------------------------------------
    SFString descriptions(void) {
        SFString required;
        CStringExportContext ctx;
        ctx << bYellow << sep << "Where:" << sep << cOff << "  \n";
        if (COptionsBase::isReadme) {
            ctx << "\n";
            ctx << "| Short Cut | Option | Description |\n";
            ctx << "| -------: | :------- | :------- |\n";
        }

        for (SFUint32 i = 0 ; i < nParamsRef ; i++) {
            SFString sName = paramsPtr[i].shortName;
            SFString lName = paramsPtr[i].longName;
            SFString descr = Strip(paramsPtr[i].description, ' ');
            if (sName.startsWith('@')) {
                // invisible option

            } else if (!sName.empty()) {
                bool isMode = sName.startsWith('~');
                // ~ makes the option a required mode, ! makes it not required
                bool isReq = isMode && !lName.Contains('!');
                sName = (isMode ? "" : sName);
                lName = (isMode ? lName.Substitute('-', "") : lName).Substitute("!", "").Substitute("~", "");
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
    SFString expandOption(SFString& arg) {

        SFString ret = arg;

        // Check that we don't have a regular command with a single dash, which
        // should report an error in client code
        for (SFUint32 i = 0 ; i < nParamsRef ; i++) {
            if (paramsPtr[i].longName == arg) {
                arg = "";
                return ret;
            }
        }

        CStringExportContext ctx;

        // Not an option
        if (!arg.startsWith('-') || arg.startsWith("--")) {
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
            ret.ReplaceAll("-th","");
            ret.ReplaceAll("-ht","");
            return ret;
        }

        // This may be a command with two -a -b (or more) single options
        if (arg.length()>2 && arg[2] == ' ') {
            ret = arg.Left(2);
            arg = arg.substr(3);
            return ret;
        }

        // One of the range commands. These must be alone on
        // the line (this is a bug for -rf:txt for example)
        if (arg.Contains(":") || arg.Contains("=")) {
            arg = "";
            return ret;
        }

        // This is a ganged-up option. We need to pull it apart by returning
        // the first two chars, and saving the rest for later.
        ret = arg.Left(2);
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
    SFUint32 verbose = false;

    //---------------------------------------------------------------------------------------------------
    SFString configPath(const SFString& part) {
        return getHomeFolder() + ".quickBlocks/" + part;
    }

    //------------------------------------------------------------------
    void editFile(const SFString& fileName) {
        CToml toml(configPath("quickBlocks.toml"));
        SFString editor = toml.getConfigStr("settings", "editor", "open ");
        SFString cmd = editor + " \"" + fileName + "\"";
        if (isTestMode()) {
            cout << "Testing editFile: " << cmd << "\n";
            cout << asciiFileToString(fileName) << "\n";
        } else {
            if (system(cmd.c_str())) {}  // do not remove. Silences compiler warnings
        }
    }

    //-------------------------------------------------------------------------
    namespace qbGlobals {
        static SFString source;
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

    //-------------------------------------------------------------------------
    SFString getSource(void) { return qbGlobals::source; }
    void     setSource(const SFString& src) { qbGlobals::source = src; }

    //--------------------------------------------------------------------------------
    SFString COptionsBlockList::parseBlockList(const SFString& argIn, blknum_t lastBlock) {
        SFString arg = argIn;
        if (arg.Contains("-")) {

            SFString arg1 = nextTokenClear(arg, '-');
            if (arg1 == "latest")
                return "Cannot start range with 'latest'";

            start = toUnsigned(arg1);
            stop  = toUnsigned(arg);
            if (arg == "latest")
                stop = lastBlock;
            if (stop <= start)
                return "'stop' must be strictly larger than 'start'";
            if (stop - start + 1 > MAX_BLOCK_LIST)
                return "The range you specified (" + argIn + ") is too broad. Ranges may be at "
                        "most " + asStringU(MAX_BLOCK_LIST) + " blocks long. Quitting...";
            isRange = true;

        } else {

            blknum_t num = toUnsigned(arg);
            CNameValue spec;
            if (pOptions && pOptions->findSpecial(spec, arg)) {
                if (spec.getName() == "latest") {
                    num = lastBlock;
                } else {
                    num = toUnsigned(spec.getValue());
                }
            }

            if (num == 0 && arg != "first" && !arg.startsWith("0") && !isNumeral(arg))
                return "'" + arg + "' does not appear to be a valid block. Quitting...";

            if (num > lastBlock) {
                SFString lateStr = (isTestMode() ? "--" : asStringU(lastBlock));
                return "Block " + arg + " is later than the last valid block " + lateStr + ". Quitting...\n";
            }

            if (nNums >= MAX_BLOCK_LIST)
                return "Too many blocks in list. Max is " + asString(MAX_BLOCK_LIST);

            nums[nNums++] = num;
        }
        latest = lastBlock;
        return "";
    }

    //--------------------------------------------------------------------------------
    void COptionsBlockList::Init(void) {
        isRange    = false;
        nums[0]    = NOPOS;
        nNums      = 0;  // we will set this to '1' later if user supplies no values
        start = stop = 0;
    }

    //--------------------------------------------------------------------------------
    COptionsBlockList::COptionsBlockList(void) {
        Init();
    }

    //--------------------------------------------------------------------------------
    SFString COptionsBlockList::toString(void) const {
        SFString ret;
        if (isRange) {
            for (SFUint32 i = start ; i < stop ; i++)
                ret += (asStringU(i) + "|");
        } else {
            for (SFUint32 i = 0 ; i < nNums ; i++)
                ret += (asStringU(nums[i]) + "|");
        }
        return ret;
    }

    //--------------------------------------------------------------------------------
    int sortByBlockNum(const void *v1, const void *v2) {
        CNameValue *b1 = (CNameValue *)v1;  // NOLINT
        CNameValue *b2 = (CNameValue *)v2;  // NOLINT
        if (b1->getName() == "latest")
            return 1;
        if (b2->getName() == "latest")
            return -1;
        if (b1->getValue().Contains("tbd") && b1->getValue().Contains("tbd"))
            return b1->getValue().compare(b2->getValue());
        if (b1->getValue().Contains("tbd"))
            return 1;
        if (b2->getValue().Contains("tbd"))
            return -1;
        return (int)(b1->getValueU() - b2->getValueU());
    }

    extern const char *STR_DEFAULT_SPECIALS;
    //-----------------------------------------------------------------------
    void COptionsBase::loadSpecials(void) {

        static CToml *toml = NULL;
        if (!toml) {
            static CToml theToml(configPath("quickBlocks.toml"));
            toml = &theToml;
        }
        specials.Clear();

        SFString specialsStr = toml->getConfigArray("specials", "list", "");
        if (specialsStr.empty()) {
            SFString in = asciiFileToString(configPath("quickBlocks.toml"));
            stringToAsciiFile(configPath("quickBlocks.toml"), in + "\n" + STR_DEFAULT_SPECIALS);
            specialsStr = toml->getConfigArray("specials", "list", "");
        }
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
    bool COptionsBase::findSpecial(CNameValue& pair, const SFString& arg) const {
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

    //--------------------------------------------------------------------------------
    const char *STR_DEFAULT_SPECIALS =
    "[[specials]]\n"
    "list = [\n"
    "\t{ name = \"first\",          value = \"0\"           },\n"
    "\t{ name = \"iceage\",         value = \"200000\"      },\n"
    "\t{ name = \"homestead\",      value = \"1150000\"     },\n"
    "\t{ name = \"daofund\",        value = \"1428756\"     },\n"
    "\t{ name = \"daohack\",        value = \"1718497\"     },\n"
    "\t{ name = \"daofork\",        value = \"1920000\"     },\n"
    "\t{ name = \"tangerine\",      value = \"2463000\"     },\n"
    "\t{ name = \"spurious\",       value = \"2675000\"     },\n"
    "\t{ name = \"stateclear\",     value = \"2718436\"     },\n"
    "\t{ name = \"byzantium\",      value = \"4370000 tbd\" },\n"
    "\t{ name = \"constantinople\", value = \"tbd\"         },\n"
    "\t{ name = \"latest\",         value = \"\"            }\n"
    "]\n";

    //---------------------------------------------------------------------------------------------------
    COptionsBase::COptionsBase(void) : namesFile("") {
        fromFile = false;
        minArgs = 1;
        isReadme = false;
        needsOption = false;
    }

    //-----------------------------------------------------------------------
    bool COptionsBase::getNamedAccount(CAccountName& acct, const SFString& addr) const {
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

        SFString textFile = namesFile.getFullPath();
        SFString binFile  = textFile.Substitute(".txt",".bin");

        SFTime txtDate = fileLastModifyDate(textFile);
        SFTime binDate = fileLastModifyDate(binFile);

        if (verbose && !isTestMode())
            cout << "txtDate: " << txtDate << " binDate: " << binDate << "\n";

        if (binDate > txtDate) {
            SFArchive archive(READING_ARCHIVE);
            if (archive.Lock(binFile, binaryReadOnly, LOCK_NOWAIT)) {
                if (verbose && !isTestMode())
                    cout << "Reading from binary cache\n";
                archive >> namedAccounts;
                archive.Release();
                return true;
            }
        }

        if (verbose && !isTestMode())
            cout << "Reading from text database\n";

        // Read the data from the names database and clean it up if needed
        SFString contents = StripAny(asciiFileToString(textFile), "\t\n ");
        contents.ReplaceAll("\t\t", "\t");
        if (!contents.endsWith("\n"))
            contents += "\n";

        // Parse out the data....
        while (!contents.empty()) {
            SFString line = nextTokenClear(contents, '\n');
            if (!line.startsWith("#")) {
                if (countOf('\t', line) < 2) {
                    cerr << "Line " << line << " does not contain two tabs.\n";

                } else {
                    CAccountName account(line);
                    namedAccounts[namedAccounts.getCount()] = account;
                }
            }
        }

        SFArchive archive(WRITING_ARCHIVE);
        if (archive.Lock(binFile, binaryWriteCreate, LOCK_CREATE)) {
            if (verbose && !isTestMode())
                cout << "Writing binary cache\n";
            archive << namedAccounts;
            archive.Release();
        }

        return true;
    }

    const char *STR_DEFAULT_DATA =
    "#---------------------------------------------------------------------------------------------------\n"
    "#  This is the ethName database. Format records as tab seperated lines with the following format:\n"
    "#\n"
    "#      Optional Symbol <tab> Name <tab> Address <tab> Source of the data <tab> Description <newline>\n"
    "#\n"
    "#---------------------------------------------------------------------------------------------------------------\n"
    "# Sym    Name                Address                        Source        Description\n"
    "#---------------------------------------------------------------------------------------------------------------\n"
    "#\n"
    "#Add your names here\n";

}  // namespace qblocks
