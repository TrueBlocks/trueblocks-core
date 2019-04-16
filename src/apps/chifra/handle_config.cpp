/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_config(void) {

    // config node does not require a running node
    nodeNotRequired();

    if (addrs.empty())
        return usage("This function requires an address. Quitting...");

    string_q cmd = toLower(nextTokenClear(tool_flags, ' '));
    if (cmd != "edit" && cmd != "show")
        return usage("chifra config 'mode' must be either 'edit' or 'show'. Quitting...");

    for (auto addr : addrs) {
        string_q path = getMonitorPath(addr + ".toml");
        if (!fileExists(path)) {
            // If it does not exist and the user wants to edit it, create it, otherwise error out
            if (cmd == "show")
                return usage("File '" + path + "' not found. Quitting...");
            createConfigFile(addr);
        }
        ASSERT(fileExists(path));

        if (cmd == "edit") {
            editFile(path);

        } else {
            ASSERT(cmd == "show");
            ostringstream os;
            os << "cat " << path;
            if (isTestMode())
                cout << substitute(os.str(), getCachePath(""), "$BLOCK_CACHE/") << endl;
            else
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        }
    }

    return true;
}

//--------------------------------------------------------------------------------
string_q colors[] = {
    "green_c", "blue_c", "red_c", "magenta_c", "yellow_c", "teal_c", "white_b",
    "green_b", "blue_b", "red_b", "magenta_b", "yellow_b", "teal_b", "black_b",
};
uint64_t nColors = sizeof(colors) / sizeof(string_q);

//----------------------------------------------------------------
const char* STR_WATCH =
"    { address = \"{ADDR}\", name = \"{NAME}\", firstBlock = {FB}, color = \"{COLOR}\" },\n";

//----------------------------------------------------------------
bool COptions::createConfigFile(const address_t& addr) {

    string_q fileName = getMonitorPath(addr + ".toml");
    cerr << cTeal << "Creating configuration file: " << fileName << endl;

    string_q config;
    asciiFileToString(configPath("chifra/config.template"), config);

    string_q name = getNamedAccount(addr);
    replace(config, "[{NAME}]", (name.empty() ? addr : name));

    cout << cTeal << "\tAdding monitor for address '" << addr << "'..." << cOff << endl;
    string_q watch = STR_WATCH;
    replaceAll(watch, "{ADDR}",  addr);
    replaceAll(watch, "{NAME}",  name);
    replaceAll(watch, "{FB}",    uint_2_Str(0));
    time_q now = Now();
    replaceAll(watch, "{COLOR}", colors[((uint64_t)date_2_Ts(now)) % nColors]);

    replace(config, "[{JSON_WATCH}]", "list = [\n" + watch + "]\n");
    if (isTestMode()) {
        cout << fileName << endl;
        cout << config;
    } else {
        stringToAsciiFile(fileName, config);
        if (verbose > 1)
            cout << config << endl;
    }

    return true;
}
