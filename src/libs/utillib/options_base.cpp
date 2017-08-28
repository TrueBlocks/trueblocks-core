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

namespace qblocks {

    //--------------------------------------------------------------------------------
    static uint32_t nP = 0;
    static CParams ps[] = { };

    //--------------------------------------------------------------------------------
    uint32_t& nParamsRef = nP;
    CParams *paramsPtr  = &ps[0];

    //--------------------------------------------------------------------------------
    static SFString programName = "quickBlocks";

    //--------------------------------------------------------------------------------
    bool COptionsBase::prepareArguments(int argc, const char *argv[]) {

        if (SFString(getenv("NO_COLOR")) == "true")
            colorsOff();

        programName = basename((char*)argv[0]);
        if ((SFUint32)argc <= minArgs)  // the first arg is the program's name
            return usage("Not enough arguments presented.");

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
                if (isReadme)
                    return usage();
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

            } else if (arg == "--version") {
                cerr << programName << " (quickBlocks) " << getVersionStr() << "\n";
                if (args) delete [] args;
                return false;

            } else if (arg == "-h" || arg == "--help") {
                if (args) delete [] args;
                return usage();

            } else if (arg == "--nocolors" || arg == "--nocolor") {
                colorsOff();

            } else if (arg == "--ether") {
                expContext().asEther = true;
                expContext().asDollars = false;
                expContext().asWei = false;

            } else if (arg == "--wei") {
                expContext().asEther = false;
                expContext().asDollars = false;
                expContext().asWei = true;

            } else if (arg == "--dollars") {
                expContext().asEther = false;
                expContext().asDollars = true;
                expContext().asWei = false;

            } else if (arg.startsWith("-v") || arg.startsWith("--verbose")) {
                verbose = true;
                arg.Replace("--verbose", "");
                arg.Replace("-v", "");
                arg.Replace(":", "");
                if (!arg.empty())
                    verbose = toLongU(arg);
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
                                            Substitute("  ", " ").Substitute("\\ ", "{~}");
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
    bool COptionsBase::builtInCmd(const SFString& arg) {
        if (arg == "-v" || arg.startsWith("-v:") || arg.startsWith("--verbose"))
            return true;
        if (arg == "-h" || arg == "--help")
            return true;
        if (arg == "--version")
            return true;
        if (arg == "--nocolors" || arg == "--nocolor")
            return true;
        if (arg == "--ether" || arg == "--wei" || arg == "--dollars")
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
        if (!COptionsBase::isReadme)
            os << bBlue << "  Powered by QuickBlocks" << (isTestMode() ? "" : " (" + getVersionStr() + ")") << "\n" << cOff;
        return os.str().c_str();
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
        if (COptionsBase::useVerbose)
            ctx << "-v|";
        ctx << "-h";
        if (!COptionsBase::needsOption)
            ctx << "]";
        ctx << required;

        return ctx.str;
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
        ctx << bYellow << sep << "Where:" << sep << cOff << "  \n";
        return ctx.str;
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
        return ctx.str;
    }

    //--------------------------------------------------------------------------------
    SFString descriptions(void) {
        SFString required;
        CStringExportContext ctx;

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

        if (COptionsBase::useVerbose)
            ctx << oneDescription("-v", "-verbose", "set verbose level. Either -v, --verbose or -v:n where 'n' is level", false, false);
        ctx << oneDescription("-h", "-help", "display this help screen", false, false);
        return ctx.str;
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
    bool COptionsBase::useVerbose = true;
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
        SFString configFile = configPath("quickBlocks.toml");
        CToml toml(configFile);
        SFString editor = toml.getConfigStr("settings", "editor", "open ");
        SFString cmd = editor + " \"" + fileName + "\"";
        if (isTestMode()) {
            cout << "Testing editFile: " << cmd << "\n";
            cout << asciiFileToString(fileName) << "\n";
        } else {
            if (system(cmd.c_str())) {}  // do not remove. Silences compiler warnings
        }
    }
}  // namespace qblocks
