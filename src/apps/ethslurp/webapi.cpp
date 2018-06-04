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
#include "webapi.h"

namespace qblocks {

//--------------------------------------------------------------------------------
CWebAPI::CWebAPI(void) {
}

//--------------------------------------------------------------------------------
CWebAPI::~CWebAPI(void) {
}

//--------------------------------------------------------------------------------
bool CWebAPI::checkKey(CToml& toml) {
    key      = toml.getConfigStr("settings", "api_key",      EMPTY);
    provider = toml.getConfigStr("settings", "api_provider", "EtherScan");
    url      = toml.getConfigStr("settings", "api_url",      "http://etherscan.io/apis");

    if (!key.empty() && key != "<NOT_SET>")
        return true;

    // Most web APIs require an API key. You will have to get one of these yourself. The
    // program will ask for an api key until it gets one. You only need to provide it once.
    char buffer[256];
    bzero(buffer, sizeof(buffer));

    cerr
    << cRed << "\n  ***Warning***" << cOff << "\n"
    << "  " << cYellow
        << "This program" << cOff << " needs an api_key from " + provider + " in order to work. You may get one at\n"
    << "  " + url + ". See our online help file for more information.\n"
    << "  Please provide an API key or type 'exit'\n"
    << "  > ";
    cerr.flush();

    if (!isTestMode())
        cin >> buffer;
    key = buffer;
    if (key % "exit" || key % "quit" || key.empty())
        exit(0);

    // save the key for later
    toml.setConfigStr("settings", "api_key",      key);
    toml.setConfigStr("settings", "api_provider", "EtherScan");
    toml.setConfigStr("settings", "api_url",      "http://etherscan.io/apis");
    toml.writeFile();

    return true;
}

//--------------------------------------------------------------------------------
SFString CWebAPI::getKey(void) const {
    return key;
}
}  // namespace qblocks
