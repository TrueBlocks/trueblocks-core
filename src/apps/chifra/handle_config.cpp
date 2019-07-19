/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "question.h"

#define cleanPath(path_) (isTestMode() ? substitute(path_, getCachePath(""), "$CACHE_PATH/") : path_)
//------------------------------------------------------------------------------------------------
bool COptions::handle_config(void) {

    ENTER4("handle_" + mode);
    nodeNotRequired();

    if (addrs.empty())
        EXIT_USAGE("This function requires an address.");

    string_q cmd = toLower(nextTokenClear(tool_flags, ' '));
    if (cmd != "edit" && cmd != "show")
        EXIT_USAGE("chifra config 'mode' must be either 'edit' or 'show'.");

    for (auto addr : addrs) {
        string_q path = getMonitorPath(addr + ".toml");
        if (!fileExists(path)) {
            // If it does not exist and the user wants to edit it, create it, otherwise error out
            if (cmd == "show")
                EXIT_USAGE("File '" + cleanPath(path) + "' not found, cannot show it. Did you mean 'edit'?");
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
                cout << cleanPath(os.str()) << endl;
            else
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
        }
    }
    EXIT_NOMSG4(true);
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

    ENTER4("createConfigFile:" + addr);

    string_q fileName = getMonitorPath(addr + ".toml");
    LOG_INFO("Creating configuration file: " + cleanPath(fileName));

    string_q config;
    asciiFileToString(configPath("chifra/config.template"), config);

    string_q name = getNamedAccount(addr);
    replace(config, "[{NAME}]", (name.empty() ? addr : name));

    LOG_INFO("\tAdding monitor for address: " + addr);
    string_q watch = STR_WATCH;
    replaceAll(watch, "{ADDR}",  addr);
    replaceAll(watch, "{NAME}",  name);
    replaceAll(watch, "{FB}",    uint_2_Str(0));
    time_q now = Now();
    size_t randIdx = (isTestMode() ? 1 : ((size_t)date_2_Ts(now)) % nColors);
    replaceAll(watch, "{COLOR}", colors[randIdx]);

    replace(config, "[{JSON_WATCH}]", "list = [\n" + watch + "]\n");
    if (isTestMode()) {
        cout << cleanPath(fileName) << endl;
        cout << config;
    } else {
        stringToAsciiFile(fileName, config);
        if (verbose > 1)
            cout << config << endl;
    }
    EXIT_NOMSG4(true);
}
