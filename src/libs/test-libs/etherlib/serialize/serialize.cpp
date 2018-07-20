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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "utillib.h"
#include "options.h"
#include "person.h"

CPerson leader("Leader", 100);
CPerson *lastAdded = &leader;

int main(int argc, const char *argv[]) {

    CPerson::registerClass();

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    // while (!options.commandList.empty())
    {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        for (size_t i = 0 ; i < 10 ; i++) {
            CPerson *newPerson = new CPerson("Person " + toStringU(i), i * 2);
            lastAdded->next = newPerson;
            lastAdded = newPerson;
        }

        cout << "Creation\n" << string_q(80, '-') << "\n";
        CPerson *cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << "\n";
            cur = cur->next;
        }
        cout.flush();

        SFArchive out(WRITING_ARCHIVE);
        if (out.Lock("./file.bin", binaryWriteCreate, LOCK_WAIT)) {
            leader.Serialize(out);
            out.Release();
        }

        cout << "\nReset\n" << string_q(80, '-') << "\n";
        leader = CPerson();
        cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << "\n";
            cur = cur->next;
        }
        cout.flush();

        SFArchive in(READING_ARCHIVE);
        if (in.Lock("./file.bin", binaryReadOnly, LOCK_WAIT)) {
            leader.Serialize(in);
            in.Release();
        }

        cout << "\nRead in data\n" << string_q(80, '-') << "\n";
        cur = &leader;
        while (cur) {
            cout << cur->name << " : " << cur->age << "\n";
            cur = cur->next;
        }
        cout.flush();

        CPerson::registerClass();
        cout << leader.Format() << "\n";

        remove("./file.bin");
    }
    return 0;
}
