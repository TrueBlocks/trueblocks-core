/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//------------------------------------------------------------------------------------------------
bool COptions::handle_config(void) {
    ENTER8("handle_" + mode);
    nodeNotRequired();

    if (contains(tool_flags, "help")) {
        ostringstream os;
        os << "cacheStatus --help";
        NOTE_CALL(os.str());
        if (system(os.str().c_str())) {
        }  // Don't remove. Silences compiler warnings
        EXIT_NOMSG8(true);
    }

    if (contains(tool_flags, "get") && !contains(tool_flags, "--get"))
        replace(tool_flags, "get", "--get");  // syntactic sugar for command line

    if (contains(tool_flags, "set") && !contains(tool_flags, "--set"))
        replace(tool_flags, "set", "--set");  // syntactic sugar for command line

    replaceAll(tool_flags, "--get", "--get_config");
    replaceAll(tool_flags, "--set", "--set_config");

    if (!startsWith(tool_flags, "--get_config") && !startsWith(tool_flags, "--set_config"))
        EXIT_MSG8("chifra config 'mode' must be either '--get' or '--set'.", false);

    ostringstream os;
    os << "cacheStatus " << tool_flags;

    string_q settings = getEnvStr("CONFIG_SET");
    string_q path = getCachePath("tmp/settings.json");
    establishFolder(path);
    stringToAsciiFile(path, settings);
    if (contains(tool_flags, "set") && (isApiMode() || isTestMode()))
        cerr << "Chifra to cacheStatus:\n" << cYellow << settings << cOff << endl;

    // both testing and non-testing
    NOTE_CALL(os.str());
    if (system(os.str().c_str())) {
    }  // Don't remove. Silences compiler warnings

    EXIT_NOMSG8(true);
}

#if 0
/*
-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------* /
#include "options.h"

#define cleanPath(path_) (isTestMode() ? substitute(path_, getCachePath(""), "$CACHE_PATH/") : path_)
//------------------------------------------------------------------------------------------------
bool COptions::handle_config(void) {

    ENTER8("handle_" + mode);
    nodeNotRequired();

    tool_flags = trim(substitute(substitute(tool_flags, "--addrs", ""), "--mode", ""));
    string_q cmd = nextTokenClear(tool_flags, ' ');
    if (cmd != "edit" && cmd != "show")
        EXIT_USAGE("chifra co nfig 'mode' must be either 'edit' or 'show'.");

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
            else {
                NOTE_CALL(os.str());
                if (system(os.str().c_str())) { }  // Don't remove. Silences compiler warnings
            }
        }
    }
    EXIT_NOMSG8(true);
}

//--------------------------------------------------------------------------------
string_q colors[] = {
    "green_c", "blue_c", "red_c", "magenta_c", "yellow_c", "teal_c", "white_b",
    "green_b", "blue_b", "red_b", "magenta_b", "yellow_b", "teal_b", "black_b",
};
uint64_t nColors = sizeof(colors) / sizeof(string_q);

//----------------------------------------------------------------
const char* STR_WATCH =
"    { address = \"{ADDRESS}\", name = \"{NAME}\", firstBlock = {FB}, color = \"{COLOR}\" },\n";

//----------------------------------------------------------------
bool COptions::createConfigFile(const address_t& addr) {

    ENTER8("createConfigFile:" + addr);

    string_q fileName = getMonitorPath(addr + ".toml");
    LOG_INFO("Creating configuration file: " + cleanPath(fileName));

    string_q config;
    asciiFileToString(configPath("chifra/config.template"), config);

    string_q name = getNamedAccount(addr);
    replace(config, "[{NAME}]", (name.empty() ? addr : name));

    LOG_INFO("\tAdding monitor for address: " + addr);
    string_q watch = STR_WATCH;
    replaceAll(watch, "{ADDRESS}",  addr);
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
    EXIT_NOMSG8(true);
}
*/

#endif
