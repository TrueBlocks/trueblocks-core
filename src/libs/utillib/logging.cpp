/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "utillib.h"
#include "logging.h"

namespace qblocks {
static logger<file_log> flog("");
static logger<err_log> elog("");
logger<log_policy_i>* dLogger = (logger<log_policy_i>*)&elog;
logger<log_policy_i>* eLogger = (logger<log_policy_i>*)&elog;

static CDefaultOptions g_LocalUseOnly;
bool silenceEnter = true;  // getEnvStr("SILENCE") != "false";
bool silenceExit = silenceEnter;
//----------------------------------------------------------------
string_q _logEnter(const string_q& func) {
    return "Enter(" + g_LocalUseOnly.getProgName() + "," + func + "): ";
}

//----------------------------------------------------------------
string_q _logExit(const string_q& func) {
    return "Exit(" + g_LocalUseOnly.getProgName() + "," + func + "): ";
}

//----------------------------------------------------------------
template <>
string_q logger<log_policy_i>::get_logline_header(void) {
    stringstream header;
// FIX_THIS_CODE
#define LOG_TIMING false
    if (LOG_TIMING) {
        if (isTestMode()) {
            header << "TIME ~ CLOCK - ";
        } else {
            static clock_t last_clock = 0;
            header.fill('0');
            header.width(7);
            clock_t now = clock();
            header << now << " (" << padNum7T(uint64_t(now - last_clock)) << ")- ";
            last_clock = now;
        }
    }
    header << cOff;
    return header.str();
}
}  // namespace qblocks
