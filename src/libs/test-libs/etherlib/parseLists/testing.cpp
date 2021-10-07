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
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions : public COptionsBase {
  public:
    COptions(void) {
    }
    ~COptions(void) {
    }
    bool parseArguments(string_q& command) override {
        return true;
    };
    void Init(void) override{};
};

//--------------------------------------------------------------
void testParseStringList(void) {
    COptions options;
    cout << "Testing parseStringList2" << endl;
    CStringArray items;
    parseStringList2(&options, items, "argIn");
    cout << "the list:" << endl;
    for (auto item : items)
        cout << "\t" << item << endl;
}

//--------------------------------------------------------------
void testParseFourbyteList(void) {
    COptions options;
    cout << "Testing parseFourbyteList" << endl;
    CStringArray strings = {"0xnotafour", "0xdeadbeef", "0x", "0x0000000000"};
    CFourbyteArray items;
    for (auto str : strings)
        parseFourbyteList(&options, items, str);
    cout << "the list:" << endl;
    for (auto item : items)
        cout << "\t" << item << endl;
}

//--------------------------------------------------------------
void testParseTopicList(void) {
    COptions options;
    cout << "Testing parseTopicList" << endl;
    CStringArray strings = {"0xnotatopic", "0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef", "0x",
                            "0x0000000000000000000000000000000000000000000000000000000000000000"};
    CTopicArray items;
    for (auto str : strings)
        parseTopicList2(&options, items, str);
    cout << "the list:" << endl;
    for (auto item : items)
        cout << "\t" << item << endl;
}

//--------------------------------------------------------------
void testParseAddressList(void) {
    COptions options;
    cout << "Testing parseAddressList" << endl;
    CStringArray strings = {"0xnotanaddress", "0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef", "0x0",
                            "0x0000000000000000000000000000000000000000000000000000000000000000",
                            "0xdeadbeefdeadbeefdeadbeefdeadbeefdeadbeef"};
    CAddressArray items;
    for (auto str : strings)
        parseAddressList(&options, items, str);
    cout << "the list:" << endl;
    for (auto item : items)
        cout << "\t" << item << endl;
}

//--------------------------------------------------------------
void testParseTransactionList(void) {
    COptions options;
    cout << "Testing parseTransactionList" << endl;
    CStringArray strings = {
        "0xnotatxid",
        "2002001.*",
        "2002001.1",
        "0x7ec79e64cc8f3301b98ca2dab208af3b340f171b1ee1c6a6115354df2e311134",
        "0xf161f6657d145bd12bd70bd930d28506b347e08a69b3fb78499a36cca398ef12.0",
        "0xf161f6657d145bd12bd70bd930d28506b347e08a69b3fb78499a36cca398ef12.111",
        "first",
        // "latest",
        "ab.cd",
        "ab.cd.dd.ee",
        "2002001.3.next",
        "2002001.5.next",
        "2002001.1.prev",
        "2002001.0.prev",
        "2002001.5.next.next",
        "2002001.5.next.next.prev.prev",
    };
    COptionsTransList items;
    for (auto str : strings)
        parseTransList2(&options, items, str);
    cout << "the list:" << endl;
    for (size_t i = 0; i < items.size(); i++)
        cout << "\t" << items[i] << endl;
}

//--------------------------------------------------------------
void testParseBlockList(void) {
    COptions options;
    cout << "Testing parseBlockList" << endl;
    CStringArray strings = {
        "10000.next", "10000.prev", "first",
        // latest
        "1-10000:glimp", "1-4000000:annually", "1-1400000:quarterly", "1-15000:monthly", "1-400000:weekly",
        "1-15000:daily", "1-4000:hourly", "1-10000:1000", "1+10", "100-99",
        "0x49e93db18c70c64d8cba2248e15f07fc3e885ffba74b12027d561504ff61a71d", "12",
        //        "2020-10-01T01",
    };
    for (auto str : strings) {
        COptionsBlockList items;
        parseBlockList2(&options, items, str, 13000000);
        cout << string_q(22, '-') << " " << str << " " << string_q(22, '-') << endl;
        for (size_t i = 0; i < items.size(); i++)
            cout << "\t" << items[i] << endl;
    }
}

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    colorsOff();
    testParseStringList();
    testParseFourbyteList();
    testParseTopicList();
    testParseAddressList();
    testParseTransactionList();
    testParseBlockList();
    return 0;
}
