/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"

//----------------------------------------------------------------
int main(int argc, char *argv[])
{
    etherlib_init("binary", quickQuitHandler);
    forEveryFileInFolder("tests/", visitFile, NULL);
    return 1;
}

//----------------------------------------------------------------
extern bool showOldGroup(const void *group);
extern bool showNewGroup(const void *group);
//----------------------------------------------------------------
bool visitFile(const string_q& path, void *data) {

    if (endsWith(path, '/')) {
        forEveryFileInFolder(path + "*", visitFile, data);

    } else {
        if (endsWith(path, "array-empty.toml")) return true;
        if (contains(path, "/invalid/")) return true;

        CToml oldToml(path);
        if (oldToml.isBackLevel()) {
            cout << string_q(80, '-') << endl << "Old toml: " << path << endl;
            cout << oldToml << endl;
            cout << oldToml.getConfigStr("", "version", "<NOT_SET>") << endl;
            CStringArray strs = { "watches", "tools", "exclusions" };
            for (auto str : strs) {
                string_q val = oldToml.getConfigJson(str, "list", "");
                if (!val.empty())
                    cout << val << endl;
            }
            if (contains(path, "ethslurp")) {
                cout << oldToml.getConfigStr("display", "fmt_csv_field", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_csv_file", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_csv_record", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_custom_field", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_custom_file", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_custom_record", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_ethscan_field", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_ethscan_fieldList", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_ethscan_file", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_ethscan_record", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_fieldList", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_html_field", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_html_file", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_html_record", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_json_field", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_json_file", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_json_record", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_txt_field", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_txt_file", "") << endl;
                cout << oldToml.getConfigStr("display", "fmt_txt_record", "") << endl;
            }
            oldToml.forEveryGroup(showOldGroup);
        }

#if 0
        try {
            CNewToml newToml(path);
            if (newToml.isBackLevel()) {
                cout << string_q(80, '-') << endl << "New toml: " << path << endl;
                cout << newToml;
                cout << newToml.getConfigStr("", "version", "<NOT_SET>") << endl;
                cout << newToml.getConfigJson("watches", "list", "<NOT_SET>") << endl;
                cout << newToml.getConfigStr("tools", "list", "<NOT_SET>") << endl;
                cout << newToml.getConfigStr("exclusions", "list", "<NOT_SET>") << endl;
                cout << "back level file" << endl;
                if (contains(path, "ethslurp")) {
                    cout << newToml.getConfigStr("display", "fmt_csv_field", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_csv_file", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_csv_record", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_custom_field", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_custom_file", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_custom_record", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_ethscan_field", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_ethscan_fieldList", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_ethscan_file", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_ethscan_record", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_fieldList", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_html_field", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_html_file", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_html_record", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_json_field", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_json_file", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_json_record", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_txt_field", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_txt_file", "") << endl;
                    cout << newToml.getConfigStr("display", "fmt_txt_record", "") << endl;
                }
                newToml.forEveryGroup(showNewGroup);
            }
        } catch (qblocks::parse_exception e) {
            //cout << e << endl;
        }
#endif
    }
    return true;
}

bool old = false;
//----------------------------------------------------------------
bool showOldGroup(const void *group) {
#ifndef NEW_TOML
    CToml::CTomlGroup *grp = (CToml::CTomlGroup *)group;
    cout << "oldGroup:" << endl;
    cout << *grp << endl;
#else
    old = true;
    showNewGroup(group);
    old = false;
#endif
    return true;
}

#ifndef NEW_TOML
//----------------------------------------------------------------
bool CToml::forEveryGroup(TOMLGROUPFUNC func) const {
    if (!func)
        return false;
    for (auto group : groups) {
        bool ret = (*func)((void*)&group);
        if (!ret)
            return false;
    }
    return true;
}

//----------------------------------------------------------------
#endif

//----------------------------------------------------------------
bool showNewGroup(const void *group) {
    tableptr_t *toml = (tableptr_t*)group;
    if (old)
        cout << "oldGroup:\n";
    else
        cout << "newGroup:\n";
    cout << *((tableptr_t)*toml) << endl;
    return true;
}

//----------------------------------------------------------------
bool CNewToml::forEveryGroup(TOMLGROUPFUNC func) const {
    if (!func)
        return false;
    return (*func)(&toml);
}

//----------------------------------------------------------------
bool CNewToml::isBackLevel(void) const {
    string_q cur = getVersionStr();
    string_q vers = getConfigStr("", "version", "NOT_SET");
    if (vers == "NOT_SET")
        vers = getConfigStr("version", "current", "NOT_SET");
    return vers != cur;
}
