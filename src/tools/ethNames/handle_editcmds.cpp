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
#define LOGGING_LEVEL_TEST
#include "options.h"

//-----------------------------------------------------------------------
void pushToOutput(CAccountNameArray& out, const CAccountName& name, bool to_custom) {
    if (to_custom && !name.is_custom)
        return;
    if (!to_custom && name.is_custom)
        return;
    out.push_back(name);
}

//-----------------------------------------------------------------------
bool COptions::handle_editcmds(CStringArray& terms, bool to_custom, bool autoname) {
    ENTER("handle_editcmds");

    string_q crud = crudCommands[0];
    if (!contains("create|update|delete|undelete|remove", crud))
        EXIT_USAGE("Invalid edit command '" + crud + "'.");

    CAccountName target;
    target.address = toLower(trim(getEnvStr("TB_NAME_ADDRESS"), '\"'));
    ASSERT(!terms.empty());
    if (target.address.empty())
        target.address = terms[0];
    target.name = trim(getEnvStr("TB_NAME_NAME"), '\"');
    target.tags = trim(getEnvStr("TB_NAME_TAG"), '\"');
    target.source = trim(getEnvStr("TB_NAME_SOURCE"), '\"');
    target.symbol = trim(getEnvStr("TB_NAME_SYMBOL"), '\"');
    target.decimals = str_2_Uint(trim(getEnvStr("TB_NAME_DECIMALS"), '\"'));
    target.description = trim(getEnvStr("TB_NAME_DESCR"), '\"');
    target.is_custom = str_2_Bool(trim(getEnvStr("TB_NAME_CUSTOM"), '\"')) || to_custom;
    finishClean(target);

    LOG_TEST_STR(string_q(45, '-'));
    LOG_TEST_OBJ(target);
    LOG_TEST_STR(string_q(45, '-'));

    bool isEdit = crud == "create" || crud == "update";
    string_q fmt = isEdit ? "tags\taddress\tname\tsymbol\tsource\tdescription\tdecimals\tdeleted\tis_custom"
                            "\tis_prefund\tis_contract\tis_erc20\tis_erc721"
                          : "address";
    CStringArray fields;
    explode(fields, fmt, '\t');

    CAccountNameArray outArray;
    outArray.reserve(namesMap.size() + 2);

    bool edited = false;
    for (auto mapItem : namesMap) {
        CAccountName name = mapItem.second;
        if (name.address == target.address) {
            if (crud == "remove") {
                // do nothing (i.e. skip this name)
                LOG4("Removing ", name.address);
            } else if (crud == "delete") {
                name.m_deleted = true;
                pushToOutput(outArray, name, to_custom);
                LOG4("Deleting ", name.address);
            } else if (crud == "undelete") {
                name.m_deleted = false;
                pushToOutput(outArray, name, to_custom);
                LOG4("Undeleting ", name.address);
            } else {
                name = target;
                pushToOutput(outArray, name, to_custom);
                LOG4("Editing ", name.address);
                terms.clear();
                terms.push_back(target.address);  // we only need the address for the search
            }
            edited = true;
        } else {
            pushToOutput(outArray, name, to_custom);
        }
    }

    if (crud == "create" && !edited) {
        pushToOutput(outArray, target, to_custom);
        LOG4("Creating ", target.address);
        terms.clear();
        terms.push_back(target.address);  // we only need the address for the search
    }

    sort(outArray.begin(), outArray.end());
    LOG4("Finished sorting...");

    fmt = STR_DISPLAY_ACCOUNTNAME;
    ostringstream dataStream2;
    string_q fieldStr = toLower(substitute(substitute(fmt, "[{", ""), "}]", ""));
    string_q testMode = getEnvStr("TEST_MODE");
    setenv("TEST_MODE", "false", true);
    dataStream2 << fieldStr << endl;
    for (auto name : outArray) {
        if (!isZeroAddr(name.address) && !name.is_prefund)
            dataStream2 << name.Format(fmt) << endl;
    }
    setenv("TEST_MODE", testMode.c_str(), true);

    if (!autoname || target.name != target.address) {
        // We don't want to write this 'not found on chain' fact to the database
        string_q dest = to_custom ? getConfigPath("names/names_custom.tab") : getConfigPath("names/names.tab");
        stringToAsciiFile(dest, dataStream2.str());
        namesMap.clear();
        ::remove(getCachePath("names/names.bin").c_str());
        ::remove(getCachePath("names/names.db").c_str());
        LOG4("Finished writing...");

        string_q copyBack = getGlobalConfig("ethNames")->getConfigStr("settings", "source", "<NOTSET>");
        if (!isTestMode() && !to_custom && fileExists(copyBack)) {
            copyFile(dest, copyBack);
        }
    }

    EXIT_NOMSG(true);
}
