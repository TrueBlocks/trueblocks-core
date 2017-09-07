/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
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

    // TODO(jayrush): extend this to allow for other APIs

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
