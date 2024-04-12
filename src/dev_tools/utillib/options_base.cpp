/*
 */

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
#include "options_base.h"
#include "filenames.h"
#include "configenv.h"

namespace qblocks {

//--------------------------------------------------------------------------------
// void COptionsBase::registerOptions(size_t nP, COption const* pP, uint32_t on, uint32_t off) {
//     arguments.clear();
//     if (parameters.empty()) {
//         for (size_t i = 0; i < nP; i++)
//             parameters.push_back(pP[i]);
//         if (off != NOOPT)
//             optionOff(off);
//         if (on != NOOPT)
//             optionOn(on);
//     }
// }

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

static const char* CHR_VALID_NAME = "\t\n\r()<>[]{}`|;'!$^*~@?&#+%,:=\"";
//---------------------------------------------------------------------------------------
bool isValidName(const string_q& fn) {
    if (fn.empty() || isdigit(fn[0]))
        return false;
    string_q test = fn;
    replaceAny(test, CHR_VALID_NAME, "");
    return test == fn;
}

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

    if (isEnabled(OPT_OUTPUT) && (startsWith(arg, "--output:") || startsWith(arg, "--append")))
        return true;
    if (isEnabled(OPT_RAW) && arg == "--raw")
        return true;
    if (arg == "--version")
        return true;
    if (arg == "--nocolor")
        return true;
    if (arg == "--noop")
        return true;
    return false;
}

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

}  // namespace qblocks
