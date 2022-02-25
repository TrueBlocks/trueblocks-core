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
#include <string>
#include <iostream>
#include <sstream>
#include "utillib.h"

//----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    LOG_INFO("Starting the application..");
    for (size_t i = 0; i < 10; i++) {
        verbose = i;
        LOG0("The value of 'i' is ", i, ". ", 10 - i - 1, " more iterations left ");
        SEP1("seperator");
        LOG1("The value of 'i' is ", i, ". ", 10 - i - 1, " more iterations left ");
        SEP2("seperator");
        LOG2("The value of 'i' is ", i, ". ", 10 - i - 1, " more iterations left ");
        SEP3("seperator");
        LOG3("The value of 'i' is ", i, ". ", 10 - i - 1, " more iterations left ");
        SEP4("seperator");
        LOG4("The value of 'i' is ", i, ". ", 10 - i - 1, " more iterations left ");
        SEP8("seperator");
        LOG8("The value of 'i' is ", i, ". ", 10 - i - 1, " more iterations left ");
        LOG_PROGRESS(UPDATE, i, 2, "\n");
    }
    LOG_WARN("Loop over");
    LOG_ERR("All good things come to an end.. :(");
    LOG_FATAL("Fatal error.. :(");
    return 0;
}
