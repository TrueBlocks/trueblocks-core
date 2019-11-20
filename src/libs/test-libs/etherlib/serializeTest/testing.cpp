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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "utillib.h"
#include "options.h"
#include "person.h"
#include "status.h"

CPerson leader("Leader", 100);
CPerson* lastAdded = &leader;

int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    CPerson::registerClass();
    CStatus::registerClass();
    CCache::registerClass();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (size_t i = 0; i < 10; i++) {
            CPerson* newPerson = new CPerson("Person " + uint_2_Str(i), i * 2);
            lastAdded->next = newPerson;
            lastAdded = newPerson;
        }

        cout << "Creation" << endl << string_q(80, '-') << endl;
        CPerson* cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << endl;
            cur = cur->next;
        }
        cout.flush();

        CArchive out(WRITING_ARCHIVE);
        if (out.Lock("./file.bin", modeWriteCreate, LOCK_WAIT)) {
            leader.Serialize(out);
            out.Release();
        }
        copyFile("./file.bin", "copy.bin");
        cout << "copied file exists (copy.bin): " << fileExists("copy.bin") << endl;

        cout << endl << "Reset" << endl << string_q(80, '-') << endl;
        leader = CPerson();
        cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << endl;
            cur = cur->next;
        }
        cout.flush();

        CArchive in(READING_ARCHIVE);
        if (in.Lock("./file.bin", modeReadOnly, LOCK_WAIT)) {
            leader.Serialize(in);
            in.Release();
        }

        cout << endl << "Read in data" << endl << string_q(80, '-') << endl;
        cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << endl;
            cur = cur->next;
        }
        cout.flush();

        CPerson copy;
        CArchive cpin(READING_ARCHIVE);
        if (cpin.Lock("./copy.bin", modeReadOnly, LOCK_WAIT)) {
            copy.Serialize(cpin);
            cpin.Release();
        }

        cout << endl << "Read in data from copy" << endl << string_q(80, '-') << endl;
        cur = &copy;
        while (cur) {
            cout << cur->name << " : " << cur->age << endl;
            cur = cur->next;
        }
        cout.flush();

        cout << endl << "Show the leader" << endl << string_q(80, '-') << endl;
        cout << leader << endl;

        cout << endl << "Show the copy" << endl << string_q(80, '-') << endl;
        cout << copy << endl;

        cout << endl << "Show a cache" << endl << string_q(80, '-') << endl;
        CCache cache;  // same with cache
        cache.type = "type";
        cache.path = "path";
        cache.nFiles = 12;
        cache.nFolders = 13;
        cache.sizeInBytes = 14;
        cache.valid_counts = true;
        cout << cache << endl;

        cout << endl << "Show a status" << endl << string_q(80, '-') << endl;
        CStatus status;  // we include 'status' in this test case only to test the code generation of makeClass -ar
        status.client_version = "client_version";
        status.trueblocks_version = "trueblocks_version";
        status.rpc_provider = "rpc_provider";
        status.api_provider = "api_provider";
        status.balance_provider = "balance_provider";
        status.host = "host";
        status.caches.push_back(&cache);
        cout << status << endl;

        ::remove("./file.bin");
        ::remove("./copy.bin");
    }

    return 0;
}
