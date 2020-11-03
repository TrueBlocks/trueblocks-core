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
#include "silkworm/chain/config.hpp"
#include "silkworm/db/chaindb.hpp"
#include "silkworm/db/tables.hpp"
#include "silkworm/db/util.hpp"
#include "silkworm/types/block.hpp"
#include "tableentry.h"

using namespace silkworm;
using namespace silkworm::db::table;

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    options.first = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        CRuntimeClass* pClass = GETRUNTIME_CLASS(CTableEntry);

        if (options.first)
            cout << exportPreamble(expContext().fmtMap["header"], pClass);

        if (options.mode == "tables") {
            options.handle_tables();

        } else {
            ASSERT(options.mode == "dump");
            options.handle_dump();
        }

        cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    }

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
const char* STR_DISPLAY_MDBSTAT_VERBOSE =
    "\n"
    "  Id:              [{ID}]\n"
    "  Name:            [{NAME}]\n"
    "  Long Name:       [{LONGNAME}]\n"
//    "  Description:     [{DESCRIPTION}]\n"
    "  Page Size:       [{STAT::PSIZE}]\n"
    "  Depth:           [{STAT::DEPTH}]\n"
    "  Branch Pages:    [{STAT::BRANCH_PAGES} pages][, {STAT::BRANCH_BYTES} bytes][, {STAT::BRANCH_PCT}%]\n"
    "  Leaf Pages:      [{STAT::LEAF_PAGES} pages][, {STAT::LEAF_BYTES} bytes][, {STAT::LEAF_PCT}%]\n"
    "  Overflow Pages:  [{STAT::OVERFLOW_PAGES} pages][, {STAT::OVERFLOW_BYTES} bytes][, {STAT::OVERFLOW_PCT}%]\n"
    "  Total:           [{STAT::TOTAL_PAGES} pages][, {STAT::TOTAL_BYTES} bytes][, {STAT::TOTAL_PCT}%]\n"
    "  nRecords:        [{STAT::ENTRIES}]\n"
    "  Avg Record Size: [{STAT::AVG}]\n";

//-----------------------------------------------------------------------
bool COptions::handle_tables(void) {
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    for (auto table : tables12) {
        if (name.empty() || table.name == name) {
            if (isText) {
                if (verbose) {
                    cout << bBlue << table.Format(STR_DISPLAY_MDBSTAT_VERBOSE);

                } else {
                    ostringstream os;
                    os << "\"" << Now().Format(FMT_EXPORT) << "\",\""; // "[" << Now().Format(FMT_EXPORT) << "{TRUE}]\t";
                    os << getLatestBlock_client() << "\",\""; //"[" << uint_2_Str(getLatestBlock_client()) << "{TRUE}]\t";
                    os << expContext().fmtMap["format"];
                    cout << bBlue << table.Format(os.str()) << endl;
                }

            } else {
                if (!first)
                    cout << "," << endl;
                cout << "  ";
                indent();
                table.doExport(cout);
                unindent();
                first = false;
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_LMDBRECORD_VERBOSE =
    "[{w:12:KEY::SIZE}][{w:12:KEY::DATA}][{w:12:VALUE::SIZE}][{w:12:VALUE::DATA}]";

//-----------------------------------------------------------------------
bool COptions::handle_dump(void) {
    size_t page_size = 4096;

    // Try to load the database
    lmdb::options opts{};
    opts.map_size = ((fileSize(datadir + "data.mdb") + page_size - 1) / page_size) * page_size;
    opts.read_only = true;
    lmdb_env = lmdb::get_env(datadir.c_str(), opts);
    lmdb_txn = lmdb_env->begin_ro_transaction();

    CTableEntry table = tableMap[name];

    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    size_t cnt = 0;
    auto named = lmdb_txn->open({table.name.c_str()});
    if (named->is_opened()) {
        MDB_val key, value;
        int rc = named->get_first(&key, &value);
        while (rc != MDB_NOTFOUND) {
            CLmdbRecord record(key, value);
            if (isText) {
                if (verbose) {
                    cout << bBlue << record.Format(STR_DISPLAY_LMDBRECORD_VERBOSE) << endl;

                } else {
                    cout << bBlue << record.Format(STR_DISPLAY_LMDBRECORD_VERBOSE) << endl;
                }
            } else {
                if (!first)
                    cout << "," << endl;
                cout << "  ";
                indent();
                record.doExport(cout);
                unindent();
                first = false;
            }
            rc = named->get_next(&key, &value);
            if (++cnt == 10)
                rc = MDB_NOTFOUND;
        }
    }

    //    cout << stat <<
    //    e = CTableEntry();
    //    e.id = named->get_dbi();
    //    e.name = named->get_name();
    //    e.description = nameMap[e.name];
    //    e.longName = nextTokenClear(e.description, '|');
    //    e.stat.psize = stat.ms_psize;
    //    e.stat.depth = stat.ms_depth;
    //    e.stat.branch_pages = stat.ms_branch_pages;
    //    e.stat.leaf_pages = stat.ms_leaf_pages;
    //    e.stat.overflow_pages = stat.ms_overflow_pages;
    //    e.stat.entries = stat.ms_entries;
    //    tables12.push_back(e);
    //    tableMap[e.name] = e;
    //
    //    rc = unnamed->get_next(&key, &value);
    //    if (rc != MDB_NOTFOUND)
    //        lmdb::err_handler(rc);

    lmdb_txn.reset();
    lmdb_env.reset();

    //    // bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    //    manageFields("CTableEntry:all", true);

    return true;
}
