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
        programName = SFString(argv[0]);
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
            SFString arg = argv[i];
            while (!arg.empty()) {
                SFString opt = expandOption(arg);  // handles case of -rf for example
                if (opt == "-")
                    hasStdIn = true;
                else
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

        SFString cmdFileName = EMPTY;
        for (SFUint32 i = 0 ; i < nArgs ; i++) {
            SFString arg = args[i];
            if (arg.startsWith("--file:")) {
                cmdFileName = arg.Substitute("--file:", EMPTY);
                cmdFileName.Replace("~/", getHomeFolder());
                if (!fileExists(cmdFileName)) {
                    if (args) delete [] args;
                    return usage("--file: '" + cmdFileName + "' not found. Quitting.");
                }

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
                arg.Replace("--verbose", EMPTY);
                arg.Replace("-v", EMPTY);
                arg.Replace(":", EMPTY);
                if (!arg.empty())
                    verbose = toLong(arg);

            } else if (arg == "-t" || arg == "--test") {

                isTesting = true;
            }
        }

        if (SFString(getenv("NO_COLOR")) == "true")
            colorsOff();
        
        // If we have a command file, we will use it, if not we will creat one and pretend we have one.
        commandList = EMPTY;
        fromFile = false;
        if (cmdFileName.empty()) {
            for (SFUint32 i = 0 ; i < nArgs ; i++)
                commandList += (args[i] + " ");
            commandList += '\n';

        } else {
            fromFile = true;
            SFString contents =  asciiFileToString(cmdFileName).Substitute("\t", " ").
                                            Substitute("-v", "").Substitute("-h", "").
                                            Substitute("-t", "").Substitute("  ", " ").
                                            Substitute("\\ ", "{~}");
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
        if (arg == "-t" || arg == "--test")
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
        if (!name.empty()) {
            shortName = name.Left(2);
            if (name.length() > 2)
                longName = name;
            if (name.Contains("{")) {
                name.Replace("{", "|{");
                nextTokenClear(name, '|');
                shortName += name;

            } else if (name.Contains(":")) {
                nextTokenClear(name, ':');
                shortName += name[0];
                longName = "-" + name;
            }
            if (longName.Contains("(") && longName.Contains(")")) {
                hotKey = longName;
                nextTokenClear(hotKey,'(');
                hotKey = nextTokenClear(hotKey, ')');
                longName.ReplaceAny("()","");
            }
        }
    }

    static SFString sep = "  ";
    static SFString sep2 = "";
    extern const char *STR_FILE_OPTION;

    //--------------------------------------------------------------------------------
    int usage(const SFString& errMsg) {
        cerr << usageStr(errMsg);
        return false;
    }

    SFString usageStr(const SFString& errMsg) {
        ostringstream os;
        if (isTesting) {
            sep = sep2 = "`";
            colorsOff();
            os << "## " << programName << "\n\n";
            os << COptionsBase::header;
            os << "\n#### Usage\n";
        }

        os << "\n";
        if (!errMsg.empty())
            os << cRed << "  " << errMsg << "\n\n";
        os << bYellow << sep << "Usage:" << sep2 << "    " << cOff << programName << " " << options() << "  \n";
        os << purpose();
        os << descriptions() << "\n";
        if (isTesting && !COptionsBase::seeAlso.empty()) {
            SFString note = COptionsBase::seeAlso;
            os << "#### " << nextTokenClear(note, ':') << "\n\n";
            os << Strip(note, ' ') << "\n";
        }
        os << COptionsBase::footer;
        if (isTesting)
            os << STR_FILE_OPTION;
        os << bBlue << (isTesting?"":"  ") << "Powered by QuickBlocks\n" << cOff;
        return os.str().c_str();
    }

    //--------------------------------------------------------------------------------
    const char *STR_FILE_OPTION =
    "##### Other options\n"
    "\n"
    "Enter --version to display the current version of the tool.\n"
    "\n"
    "Enter --nocolors to turn off colored display.\n"
    "\n"
    "Enter --wei (default), --ether, or --dollars to alter the way value is displayed.\n"
    "\n"
    "All `quickBlocks` command-line tools support the `--file:filename` option. Place valid commands, on separate "
    "lines, in a file and include the above option. In some cases, this option may significantly improve "
    "performance. Place a semi-colon at the start of a line to make it a comment.\n"
    "\n";

    //--------------------------------------------------------------------------------
    SFString options(void) {
        SFString required;

        CStringExportContext ctx;
        ctx << "[";
        for (SFUint32 i = 0 ; i < nParamsRef ; i++) {
            if (paramsPtr[i].shortName.startsWith('~')) {
                required += (" " + paramsPtr[i].longName.substr(1).Substitute("!", ""));

            } else if (paramsPtr[i].shortName.startsWith('@')) {
                // invisible option

            } else if (!paramsPtr[i].shortName.empty()) {
                ctx << paramsPtr[i].shortName << "|";
            }
        }
        if (COptionsBase::useTesting)
            ctx << "-t|";
        if (COptionsBase::useVerbose)
            ctx << "-v|";
        ctx << "-h]";
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
            purpose.Replace("\n           ", EMPTY);
            ctx << bYellow << sep << "Purpose:" << sep2 << "  " << cOff
                << purpose.Substitute("\n", "\n           ") << "  \n";
        }
        ctx << bYellow << sep << "Where:" << sep << cOff << "  \n";
        return ctx.str;
    }

    //--------------------------------------------------------------------------------
    SFString description(const SFString& s, const SFString& l, const SFString& d, bool isMode, bool required) {
        CStringExportContext ctx;
        if (isTesting) {
            ctx << "| " << s << " | " << (isMode ? "" : "-") << l << " | " << d.Substitute("|", "&#124;") << " |\n";

        } else {
            ctx << "\t"
            << (isMode ? "" : padRight(s, 3))
            << padRight((l.empty() ? "" : (isMode ? l : " (or -" + l + ")")) , 19 + (isMode ? 3 : 0))
            << d
            << (required ? " (required)" : "")
            << "\n";
        }
        return ctx.str;
    }

    //--------------------------------------------------------------------------------
    SFString descriptions(void) {
        SFString required;
        CStringExportContext ctx;

        if (isTesting) {
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
                ctx << description(sName, lName, descr, isMode, isReq);
            }
        }

        if (COptionsBase::useTesting)
            ctx << description("-t", "-test", "generate intermediary files but do not execute the commands",
                               false, false);
        if (COptionsBase::useVerbose)
            ctx << description("-v", "-verbose", "set verbose level. Follow with a number to set level "
                               "(-v0 for silent)", false, false);
        ctx << description("-h", "-help", "display this help screen", false, false);
        return ctx.str;
    }

    //--------------------------------------------------------------------------------
    SFString expandOption(SFString& arg) {
        SFString ret = arg;

        // Check that we don't have a regular command with a single dash, which
        // should report an error in client code
        for (SFUint32 i = 0 ; i < nParamsRef ; i++) {
            if (paramsPtr[i].longName == arg) {
                arg = EMPTY;
                return ret;
            }
        }

        CStringExportContext ctx;

        // Not an option
        if (!arg.startsWith('-') || arg.startsWith("--")) {
            arg = EMPTY;
            return ret;
        }

        // Stdin case
        if (arg == "-") {
            arg = EMPTY;
            return ret;
        }

        // Single option
        if (arg.length() == 2) {
            arg = EMPTY;
            return ret;
        }

        // One of the range commands. These must be alone on
        // the line (this is a bug for -rf:txt for example)
        if (arg.Contains(":") || arg.Contains("=")) {
            arg = EMPTY;
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
    SFString COptionsBase::header = "";
    SFString COptionsBase::footer = "";
    SFString COptionsBase::seeAlso = "";
    bool COptionsBase::useVerbose = true;
    bool COptionsBase::useTesting = true;

    //--------------------------------------------------------------------------------
    SFUint32 verbose = false;
    bool isTesting = false;

    //---------------------------------------------------------------------------------------------------
    SFString configPath(const SFString& part) {
        return getHomeFolder() + ".quickBlocks" + (isTesting?".test":EMPTY) + "/" + part;
    }
}  // namespace qblocks
