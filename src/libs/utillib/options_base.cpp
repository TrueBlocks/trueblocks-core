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
            ctx << bYellow << sep << "Notes:" << sep << cOff << "\n";
            ctx << (COptionsBase::isReadme ? "\n" : "");
            ctx << ret << "  \n";
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
            ctx << "| Option | Full Command | Description |\n";
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
    SFString COptionsBlockList::parseBlockList(const SFString& argIn, blknum_t latest) {
        SFString arg = argIn;
        if (arg.Contains("-")) {

            SFString arg1 = nextTokenClear(arg, '-');
            if (arg1 == "latest")
                return "Cannot start range with 'latest'";

            start = toUnsigned(arg1);
            stop  = toUnsigned(arg);
            if (arg == "latest")
                stop = latest;
            if (stop <= start)
                return "'stop' must be strictly larger than 'start'";
            isRange = true;

        } else {
            SFUint32 num = toUnsigned(arg);
            if (arg == "latest")
                num = latest;
            if (nNums < MAX_BLOCK_LIST)
                nums[nNums++] = num;
            else
                return "Too many blocks in list. Max is " + asString(MAX_BLOCK_LIST);
        }
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
    int sortByBlockNum(const void *v1, const void *v2) {
        CNameValue *b1 = (CNameValue *)v1;  // NOLINT
        CNameValue *b2 = (CNameValue *)v2;  // NOLINT
        if (b1->getName() == "latest")
            return 1;
        if (b2->getName() == "latest")
            return -1;
        if (b1->getValue().startsWith("tbd") && b1->getValue().startsWith("tbd"))
            return b1->getValue().compare(b2->getValue());
        if (b1->getValue().startsWith("tbd"))
            return 1;
        if (b2->getValue().startsWith("tbd"))
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
                //cout << pair.Format() << "\n";
                if (pair.name == "latest") {
                    pair.value = "[{LATEST}]";
                }
                specials[specials.getCount()] = pair;
            }
        }

        specials.Sort(sortByBlockNum);
        return;
    }

    //--------------------------------------------------------------------------------
    SFString COptionsBase::listSpecials(bool terse) const {
        if (specials.getCount() == 0)
            ((COptionsBase*)this)->loadSpecials();
        ostringstream os;
        if (terse) {
            os << bYellow << "\n  Notes:\n\t" << cOff;
            os << "You may specify any of the following strings to represent 'special' blocks:\n\n\t    ";
        } else {
            os << bYellow << "\n\tSpecial Blocks:" << cOff;
        }

        SFString extra;
        for (uint32_t i = 0 ; i < specials.getCount(); i++) {

            SFString name  = specials[i].getName();
            SFString block = specials[i].getValue();
            if (name == "latest") {
                if (isTestMode()) {
                    block = "";
                } else if (COptionsBase::isReadme) {
                    block = "--";
                } else if (i > 0 && specials[i-1].getValueU() >= specials[i].getValueU()) {
                    extra = iWhite + " (syncing)" + cOff;
                }
            }

            if (terse) {
                os << name;
                os << " (" << cTeal << block << extra << cOff << ")";
                if (i < specials.getCount()-1)
                    os << ", ";
                if (!((i+1)%4))
                    os << "\n\t    ";
            } else {
                os << "\n\t  " << padRight(name, 15) << cTeal << padLeft(block, 10) << cOff << extra ;
            }
        }
        if (terse) {
            if (specials.getCount() % 4)
                os << "\n";
        } else {
            os << "\n";
        }
        return os.str().c_str();
    }

    //--------------------------------------------------------------------------------
    uint32_t COptionsBase::findSpecial(const SFString& arg) const {
        if (specials.getCount() == 0)
            ((COptionsBase*)this)->loadSpecials();
        for (uint32_t i = 0 ; i < specials.getCount() ; i++) {
            if (arg == specials[i].getName())
                return i;
        }
        return (uint32_t)-1;
    }

    //--------------------------------------------------------------------------------
    const char *STR_DEFAULT_SPECIALS =
    "[[specials]]\n"
    "list = [\n"
    "\t{ name = \"first\",          value = \"0\"          },\n"
    "\t{ name = \"iceage\",         value = \"200000\"     },\n"
    "\t{ name = \"homestead\",      value = \"1150000\"    },\n"
    "\t{ name = \"daofund\",        value = \"1428756\"    },\n"
    "\t{ name = \"daohack\",        value = \"1718497\"    },\n"
    "\t{ name = \"daofork\",        value = \"1920000\"    },\n"
    "\t{ name = \"tangerine\",      value = \"2463000\"    },\n"
    "\t{ name = \"spurious\",       value = \"2675000\"    },\n"
    "\t{ name = \"stateclear\",     value = \"2718436\"    },\n"
    "\t{ name = \"byzantium\",      value = \"tbd\"        },\n"
    "\t{ name = \"constantinople\", value = \"tbd\"        },\n"
    "\t{ name = \"latest\",         value = \"\"           }\n"
    "]\n";

    //---------------------------------------------------------------------------------------------------
    COptionsBase::COptionsBase(void) : namesFile("") {
        fromFile = false;
        minArgs = 1;
        isReadme = false;
        needsOption = false;
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
    "DAO    The DAO                0xbb9bc244d798123fde783fcc1c72d3bb8c189413    Etherscan.io    The infamous DAO smart contract\n"
    "QTUM    Qtum                0x9a642d6b3368ddc662CA244bAdf32cDA716005BC    Etherscan.io    Build Decentralized Applications that Simply Work Executable on mobile devices, compatible with major existing blockchain ecosystem\n"
    "OMG    OMGToken            0xd26114cd6EE289AccF82350c8d8487fedB8A0C07    Etherscan.io    A public Ethereum-based financial technology for use in mainstream digital wallets\n"
    "EOS    EOS                0x86Fa049857E0209aa7D9e616F7eb3b3B78ECfdb0    Etherscan.io    Infrastructure for Decentralized Applications\n"
    "PAY    TenX Pay Token            0xB97048628DB6B661D4C2aA833e95Dbe1A905B280    Etherscan.io    TenX connects your blockchain assets for everyday use. TenX debit card and banking licence will allow us to be a hub for the blockchain ecosystem to connect for real-world use cases\n"
    "ICN    ICONOMI                0x888666CA69E0f178DED6D75b5726Cee99A87D698    Etherscan.io    Digital Assets Management platform enables simple access to a variety of digital assets and combined Digital Asset Arrays\n"
    "GNT    Golem Network Token        0xa74476443119A942dE498590Fe1f2454d7D4aC0d    Etherscan.io    Golem is going to create the first decentralized global market for computing power\n"
    "REP    Augur (Reputation)        0xE94327D07Fc17907b4DB788E5aDf2ed424adDff6    Etherscan.io    Augur combines the magic of prediction markets with the power of a decentralized network to create a stunningly accurate forecasting tool\n"
    "MKR    Maker                0xC66eA802717bFb9833400264Dd12c2bCeAa34a6d    Etherscan.io    Maker is a Decentralized Autonomous Organization that creates and insures the dai stablecoin on the Ethereum blockchain\n"
    "SNT    Status Network Token        0x744d70FDBE2Ba4CF95131626614a1763DF805B9E    Etherscan.io    Status is an open source messaging platform and mobile browser to interact with decentralized applications that run on the Ethereum Network\n"
    "CVC    Civic                0x41e5560054824eA6B0732E656E3Ad64E20e94E45    Etherscan.io    Giving businesses and individuals the tools to control and protect identities\n"
    "GNO    Gnosis Token            0x6810e776880C02933D47DB1b9fc05908e5386b96    Etherscan.io    Crowd Sourced Wisdom - The next generation blockchain network. Speculate on anything with an easy-to-use prediction market\n"
    "BAT    Basic Attention Token        0x0D8775F648430679A709E98d2b0Cb6250d2887EF    Etherscan.io    The Basic Attention Token is the new token for the digital advertising industry\n"
    "DGD    Digix Global            0xE0B7927c4aF23765Cb51314A0E0521A9645F0E2A    Etherscan.io    Every asset represents a unique bullion bar sitting in designated securitised custodial vaults\n"
    "BNT    Bancor Network Token        0x1F573D6Fb3F13d689FF844B4cE37794d79a7FF1C    Etherscan.io    Bancor Protocol is a standard for a new generation of cryptocurrencies called Smart Tokens\n"
    "STORJ    StorjToken            0xB64ef51C888972c908CFacf59B47C1AfBC0Ab8aC    Etherscan.io    Blockchain-based, end-to-end encrypted, distributed object storage, where only you have access to your data\n"
    "FUN    FunFair                0x419D0d8BdD9aF5e606Ae2232ed285Aff190E711b    Etherscan.io    FunFair is a decentralised gaming platform powered by Ethereum smart contracts\n"
    "SNGLS    SingularDTV            0xaeC2E87E0A235266D9C5ADc9DEb4b2E29b54D009    Etherscan.io    A Blockchain Entertainment Studio, Smart Contract Rights Management Platform and Video On-Demand Portal\n"
    "DNT    district0x Netwrk Token        0x0AbdAce70D3790235af448C88547603b945604ea    Etherscan.io    A network of decentralized markets and communities. Create, operate, and govern. Powered by Ethereum, Aragon, and IPFS\n"
    "ANT    Aragon Network Token        0x960b236A07cf122663c4303350609A66A7B288C0    Etherscan.io    Create and manage unstoppable organizations. Aragon lets you manage entire organizations using the blockchain. This makes Aragon organizations more efficient than their traditional counterparties\n"
    "EDG    Edgeless            0x08711D3B02C8758F2FB3ab4e80228418a7F8e39c    Etherscan.io    The Ethereum smart contract-based that offers a 0% house edge and solves the transparency question once and for all\n"
    "1ST    FirstBlood Token        0xAf30D2a7E90d7DC361c8C4585e9BB7D2F6f15bc7    Etherscan.io    FirstBlood.io is a decentralized Esports gaming platform that is powered by the blockchain\n"
    "RLC    iEx.ec Network Token        0x607F4C5BB672230e8672085532f7e901544a7375    Etherscan.io    Blockchain Based distributed cloud computing\n"
    "WINGS    Wings                0x667088b212ce3d06a1b553a7221E1fD19000d9aF    Etherscan.io    A decentralized platform to create, join and manage projects\n"
    "BQX    Bitquence            0x5Af2Be193a6ABCa9c8817001F45744777Db30756    Etherscan.io    People-powered cryptocurrency services for the blockchain\n"
    "MLN    Melon Token            0xBEB9eF514a379B997e0798FDcC901Ee474B6D9A1    Etherscan.io    The Melon protocol is a blockchain protocol for digital asset management built on the Ethereum platform\n"
    "ROL    Etheroll (Dice)            0x2e071D2966Aa7D8dECB1005885bA1977D6038A65    Etherscan.io    Provably fair online Ether gaming on the Ethereum Blockchain\n"
    "LUN    Lunyr Token            0xfa05A73FfE78ef8f1a739473e462c54bae6567D9    Etherscan.io    Lunyr is an Ethereum-based decentralized world knowledge base which rewards users with app tokens for peer-reviewing and contributing information\n"
    "    AdToken                0xD0D6D6C5Fe4a677D343cC433536BB717bAe167dD    Etherscan.io    adChain is a browser agnostic Ethereum based solution for digital advertising that seamlessly integrates with pre-existing RTB and programmatic industry standards\n"
    "MCO    Monaco                0xB63B606Ac810a52cCa15e44bB630fd42D8d1d83d    Etherscan.io    Monaco is a cryptocurrency card. Spend and send money globally at interbank exchange rates\n"
    "vSlice    vSlice Token            0x5c543e7AE0A1104f78406C340E9C64FD9fCE5170    Etherscan.io    An Ethereum Gaming Platform Token\n"
    "HMQ    Humaniq                0xcbCC0F036ED4788F63FC0fEE32873d6A7487b908    Etherscan.io    Humaniq aims to be a simple and secure 4th generation mobile bank\n"
    "TAAS    Token-as-a-Service        0xE7775A6e9Bcf904eb39DA2b68c5efb4F9360e08C    Etherscan.io    TaaS is a tokenized closed-end fund designed to reduce the risks and technical barriers of investing in the blockchain space\n"
    "TKN    Monolith TKN            0xaAAf91D9b90dF800Df4F55c205fd6989c977E73a    Etherscan.io    The All Powerful Smart Contract Powered Debit Card\n"
    "TRST    WeTrust (Trustcoin)        0xCb94be6f13A1182E4A4B6140cb7bf2025d28e41B    Etherscan.io    A platform for Trusted Lending Circles, powered by people and blockchain\n"
    "XAUR    Xaurum                0x4DF812F6064def1e5e029f1ca858777CC98D2D81    Etherscan.io    Xaurum is unit of value on the golden blockchain, it represents an increasing amount of gold and can be exchanged for it by melting\n"
    "TIME    ChronoBank (Time Token)        0x6531f133e6DeeBe7F2dcE5A0441aA7ef330B4e53    Etherscan.io    ChronoBank.io is a wide-ranging blockchain project, aimed at disrupting the HR/recruitment/finance industries\n"
    "SAN    SANtiment Network Token        0x7C5A0CE9267ED19B22F8cae653F198e3E8daf098    Etherscan.io    A Better Way to Trade Crypto-Markets - Market Datafeeds, Newswires, and Crowd Sentiment Insights for the Blockchain World\n"
    "NET    Nimiq Network Token        0xcfb98637bcae43C13323EAa1731cED2B716962fD    Etherscan.io    NimiqNetwork World's first Browser-based Blockchain & Ecosystem\n"
    "GUP    Matchpool (Guppy)        0xf7B098298f7C69Fc14610bf71d5e02c60792894C    Etherscan.io    Matchpool is a platform that creates human connections\n"
    "SWT    Swarm City Token        0xB9e7F8568e08d5659f5D29C4997173d84CdF2607    Etherscan.io    Swarm City is a decentralized peer to peer sharing economy\n"
    "BCAP    Blockchain Capital        0xFf3519eeeEA3e76F1F699CCcE5E23ee0bdDa41aC    Etherscan.io    Blockchain Capital is a pioneer and the premier venture capital firm investing in Blockchain enabled technology\n"
    "PLU    Pluton                0xD8912C10681D8B21Fd3742244f44658dBA12264E    Etherscan.io    With Plutus Tap & Pay, you can pay at any NFC-enabled merchant\n"
    "UNI    Unicorns            0x89205A3A3b2A69De6Dbf7f01ED13B2108B2c43e7    Ethereum.org    Ethereum tip jar unicorn tokens\n"
    "EEAR    Eth Early Adoption Registry    0x713b73c3994442b533e6a083ec968e40606810ec    quickBlocks.io    An early, known-dead address useful for testing\n";
}  // namespace qblocks
