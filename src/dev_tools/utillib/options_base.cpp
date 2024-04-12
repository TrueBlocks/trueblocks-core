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
#include "basetypes.h"
#include "database.h"
#include "exportcontext.h"
#include "options_base.h"
#include "filenames.h"
#include "exportcontext.h"
#include "configenv.h"

namespace qblocks {

//--------------------------------------------------------------------------------
void COptionsBase::registerOptions(size_t nP, COption const* pP, uint32_t on, uint32_t off) {
    arguments.clear();
    if (parameters.empty()) {
        for (size_t i = 0; i < nP; i++)
            parameters.push_back(pP[i]);
        if (off != NOOPT)
            optionOff(off);
        if (on != NOOPT)
            optionOn(on);
    }
}

//--------------------------------------------------------------------------------
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

    bool isRedir = getEnvStr("REDIR_CERR") == "true";
    if (isRedir)
        cerr.rdbuf(cout.rdbuf());

    // We allow users to add 'true' or 'false' to boolean options, but the following code works by the
    // presence or absence of the boolean key, so here we spin through, removing 'true' and 'false' and
    // removing the key if we find 'false'
    CStringArray cleaned_;
    for (int i = 1; i < argCountIn; i++) {
        string_q arg = argvIn[i];
        if (toLower(arg) == "true") {
            // don't put this in, but leave the key
        } else if (toLower(arg) == "false") {
            // remove the key
            if (cleaned_.size())
                cleaned_.pop_back();
        } else if (arg == "--readme") {
            cleaned_.push_back(arg);
        } else {
            // add this arg
            cleaned_.push_back(arg);
        }
    }

    for (auto item : cleaned_) {
        CStringArray parts;
        string_q arg = substitute(substituteAny(item, "\r\n\t", " "), ",", " ");
        explode(parts, arg, ' ');
        for (auto part : parts) {
            if (!part.empty()) {
                separatedArgs_.push_back(part);
            }
        }
    }

    return true;
}

//--------------------------------------------------------------------------------
bool COptionsBase::isBadSingleDash(const string_q& arg) const {
    for (const auto& option : parameters) {
        string_q cmd = substitute(arg, "-", "");
        if (cmd == option.longName)
            return true;
    }

    CStringArray builtInCmds = {"verbose", "fmt", "ether",   "output",  "append",
                                "raw",     "wei", "version", "nocolor", "noop"};

    for (auto bi : builtInCmds) {
        if (arg == ("-" + bi))
            return true;
    }

    return false;
}

//--------------------------------------------------------------------------------
bool COptionsBase::prepareArguments(int argCountIn, const char* argvIn[]) {
    CStringArray separatedArgs_;
    if (!prePrepareArguments(separatedArgs_, argCountIn, argvIn))  // returns false if readme is requested
        return usage();

    CStringArray argumentsIn;
    for (auto arg : separatedArgs_) {
        replace(arg, "--verbose", "-v");
        while (!arg.empty()) {
            if (startsWith(arg, "-") && !contains(arg, "--") && isBadSingleDash(arg))
                return invalid_option(arg + ". Did you mean -" + arg + "?");
            string_q opt = expandOption(arg);  // handles case of -rf for example
            if (opt == "-") {
                return usage("Raw '-' not supported.");
            } else if (!opt.empty()) {
                argumentsIn.push_back(opt);  // args[nArgs++] = opt;
            }
        }
    }
    if (argumentsIn.size() < minArgs)  // the first arg is the program's name, so we use <=
        return (argumentsIn.size() == 0 && !isApiMode()) ? usage("") : usage("Not enough arguments presented.");

    //-----------------------------------------------------------------------------------
    // Collapse commands that have 'permitted' sub options (i.e. colon ":" args)
    //-----------------------------------------------------------------------------------
    CStringArray argumentsOut;
    for (size_t i = 0; i < argumentsIn.size(); i++) {
        string_q arg = argumentsIn[i];
        bool combine = false;
        for (const auto& option : parameters) {
            if (!option.permitted.empty()) {
                string_q hotKey = option.hotKey;
                string_q longName = option.longName;
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
    // We now have 'nArgs' command line arguments stored in the array 'args.'  We spin through
    // them in order to handle any arguments common to all programs and remove them from the array
    //-----------------------------------------------------------------------------------
    for (uint64_t i = 0; i < argumentsOut.size(); i++) {
        string_q arg = argumentsOut[i];
        if (startsWith(arg, "-v:") || startsWith(arg, "--verbose:")) {
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
            } else if (arg == "json") {
                expContext().exportFmt = JSON1;
            } else {
                return usage("The --fmt option (" + arg + ") must be one of [ json | txt | csv ].");
            }
            argumentsOut[i] = "";
        }
    }

    // remove empty arguments
    CStringArray argumentsOut3;
    for (auto arg : argumentsOut)
        if (!arg.empty())
            argumentsOut3.push_back(arg);

    // If we have a command file, we will use it, if not we will create one and pretend we have one.
    string_q commandList = "";
    for (auto arg : argumentsOut3) {
        commandList += (arg + " ");
    }
    commandList += '\n';

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
    bool append = false;
    if (contains(cmdLine, "--append")) {
        append = true;
        replaceAll(cmdLine, "--append", "");
    }

    // Note: check each item individually in case more than one appears on the command line
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
        return false;
    }

    if (contains(cmdLine, "--nocolor ")) {
        replaceAll(cmdLine, "--nocolor ", "");
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
        expContext().asWei = false;
    }

    if (isEnabled(OPT_RAW) && contains(cmdLine, "--raw ")) {
        replaceAll(cmdLine, "--raw ", "");
        isRaw = true;
    }

    if (isEnabled(OPT_OUTPUT) && contains(cmdLine, "--output:")) {
        closeRedirect();  // close the current one in case it's open
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
        outputStream.open(rd_outputFilename.c_str(), (append ? ios::out | ios::app : ios::out | ios::trunc));
        if (outputStream.is_open()) {
            coutSaved = cout.rdbuf();          // back up cout's streambuf
            cout.rdbuf(outputStream.rdbuf());  // assign streambuf to cout
        } else {
            return usage("Could not open output stream at '" + rd_outputFilename + ".");
        }
        CStringArray parts;
        explode(parts, temp, '.');
        if (parts.size() > 0) {
            string_q last = parts[parts.size() - 1];
            if (last == "txt") {
                expContext().exportFmt = TXT1;
            } else if (last == "csv") {
                expContext().exportFmt = CSV1;
            } else if (last == "json") {
                expContext().exportFmt = JSON1;
            }
        }
    }

    if (isEnabled(OPT_WEI) && contains(cmdLine, "--wei ")) {
        replaceAll(cmdLine, "--wei ", "");
        expContext().asEther = false;
        expContext().asWei = true;
    }

    cmdLine = substitute(trim(cmdLine), "  ", " ");
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
    if (isEnabled(OPT_OUTPUT) && (startsWith(arg, "--output:") || startsWith(arg, "--append")))
        return true;
    if (isEnabled(OPT_RAW) && arg == "--raw")
        return true;
    if (isEnabled(OPT_WEI) && arg == "--wei")
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
            format = defFormat;
            manageFields(dataType + ":" + cleanFmt((format.empty() ? defFormat : format)));
            break;
        case JSON1:
            format = "";
            break;
    }
    if (expContext().asEther)
        format = substitute(format, "{BALANCE}", "{ETHER}");
    expContext().fmtMap["meta"] = meta;
    expContext().fmtMap["format"] = cleanFmt(format);
    expContext().fmtMap["header"] = cleanFmt(format);
    if (noHeader)
        expContext().fmtMap["header"] = "";
}

//--------------------------------------------------------------------------------
bool COptionsBase::invalid_option(const string_q& arg) const {
    if (startsWith(arg, "-") && !contains(arg, "--") && isBadSingleDash(arg))
        return invalid_option(arg + ". Did you mean -" + arg + "?");
    return usage("Invalid option: " + arg);
}

//--------------------------------------------------------------------------------
string_q COptionsBase::expandOption(string_q& arg) {
    string_q ret = arg;

    // Check that we don't have a regular command with a single dash, which
    // should report an error in client code
    for (const auto& option : parameters) {
        if (option.longName == arg) {
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

//-------------------------------------------------------------------------
bool COptionsBase::isEnabled(uint32_t q) const {
    return (enableBits & q);
}

//-------------------------------------------------------------------------
void COptionsBase::optionOff(uint32_t q) {
    enableBits &= (~q);
}

//-------------------------------------------------------------------------
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
const CToml* getGlobalConfig(const string_q& mergeIn) {
    static CToml* toml = NULL;
    static string_q components = "trueBlocks|";

    if (!toml) {
        string_q configFile = getConfigEnv()->configPath + "trueBlocks.toml";
        static CToml theToml(configFile);
        toml = &theToml;
        string_q name = COptionsBase::g_progName;
        string_q fileName = getConfigEnv()->chainConfigPath + name + ".toml";
        if (name == "makeClass" || name == "testRunner")
            fileName = getConfigEnv()->configPath + name + ".toml";
        if (fileExists(fileName) && !contains(components, name + "|")) {
            components += name + "|";
            CToml custom(fileName);
            toml->mergeFile(&custom);
        }
    }

    // If we're told explicitly to load another config, do that as well
    if (!mergeIn.empty()) {
        string_q name = mergeIn;
        string_q fileName = getConfigEnv()->chainConfigPath + name + ".toml";
        if (name == "makeClass" || name == "testRunner")
            fileName = getConfigEnv()->configPath + name + ".toml";
        if (fileExists(fileName) && !contains(components, name + "|")) {
            components += name + "|";
            CToml custom(fileName);
            toml->mergeFile(&custom);
        }
    }

    return toml;
}

COptionsBase::COptionsBase(void) {
    minArgs = 1;
    isRaw = false;
    firstOut = true;
    noHeader = false;
    enableBits = OPT_DEFAULT;
    arguments.clear();
    notes.clear();
    commandLines.clear();
    coutSaved = NULL;
    rd_outputFilename = "";
    rd_zipOnClose = false;
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
            case CSV1:
                // only report if the user isn't in API mode
                break;
            default:
                cerr << "{ \"outputFilename\": \"" << outFn << "\" }";
                break;
        }

        // Zip the file if we're told to do so
        if (!isTestMode() && rd_zipOnClose) {
            ostringstream os;
            os << "gzip -fv " << substitute(outFn, ".gz", "");
            if (system(os.str().c_str())) {
            }  // Don't remove. Silences warnings
        }

        rd_zipOnClose = false;
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

}  // namespace qblocks
