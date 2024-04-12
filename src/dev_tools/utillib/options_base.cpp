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

string_q COptionsBase::g_progName = "trueBlocks";
void COptionsBase::setProgName(const string_q& name) {
    g_progName = name;
}
string_q COptionsBase::getProgName(void) const {
    return g_progName;
}

uint64_t verbose = false;

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
    arguments.clear();
    commandLines.clear();
    coutSaved = NULL;
    rd_outputFilename = "";
    rd_zipOnClose = false;
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
