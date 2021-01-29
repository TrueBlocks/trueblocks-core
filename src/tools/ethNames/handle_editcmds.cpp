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
bool COptions::processEditCommand(CStringArray& terms, bool to_custom) {
    ENTER("processEditCommand");

    if (!contains("create|update|delete|undelete|remove", crudCommand))
        EXIT_USAGE("Invalid edit command '" + crudCommand + "'. Quitting...");

    CAccountName target;
    target.address = toLower(trim(getEnvStr("TB_NAME_ADDRESS"), '\"'));
    if (target.address.empty()) {
        target.address = terms[0];
    }
    target.name = trim(getEnvStr("TB_NAME_NAME"), '\"');
    target.tags = trim(getEnvStr("TB_NAME_TAG"), '\"');
    target.source = trim(getEnvStr("TB_NAME_SOURCE"), '\"');
    target.symbol = trim(getEnvStr("TB_NAME_SYMBOL"), '\"');
    target.decimals = str_2_Uint(trim(getEnvStr("TB_NAME_DECIMALS"), '\"'));
    target.description = trim(getEnvStr("TB_NAME_DESCR"), '\"');
    target.is_custom = str_2_Bool(trim(getEnvStr("TB_NAME_CUSTOM"), '\"')) || to_custom;
    finishClean(target);

    if (!isApiMode() && isTestMode()) {
        cout << string_q(45, '-') << endl;
        cout << target << endl;
        cout << string_q(45, '-') << endl;
    }

    bool isEdit = crudCommand == "create" || crudCommand == "update";
    string_q fmt = isEdit ? "tags\taddress\tname\tsymbol\tsource\tdescription\tdecimals\tdeleted\tis_custom"
                            "\tis_prefund\tis_contract\tis_erc20\tis_erc721"
                          : "address";
    CStringArray fields;
    explode(fields, fmt, '\t');

    CAccountNameArray outArray;
    outArray.reserve(namedAccounts.size() + 2);

    bool edited = false;
    for (auto name : namedAccounts) {
        if (name.address == target.address) {
            if (crudCommand == "remove") {
                // do nothing (i.e. skip this name)
                LOG4("Removing ", name.address);
            } else if (crudCommand == "delete") {
                name.m_deleted = true;
                pushToOutput(outArray, name, to_custom);
                LOG4("Deleting ", name.address);
            } else if (crudCommand == "undelete") {
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

    if (crudCommand == "create" && !edited) {
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
        if (!name.is_prefund)  // if the user has customized a prefund, save it as a custom as well
            dataStream2 << name.Format(fmt) << endl;
    }
    setenv("TEST_MODE", testMode.c_str(), true);

    string_q dest = to_custom ? configPath("names/names_custom.tab") : configPath("names/names.tab");
    stringToAsciiFile(dest, dataStream2.str());
    namedAccounts.clear();
    ::remove(getCachePath("names/names.bin").c_str());
    LOG4("Finished writing...");

    EXIT_NOMSG(true);
}
