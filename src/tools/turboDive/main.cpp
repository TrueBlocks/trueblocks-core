#if 0
/*
 Copyright 2020 The Silkworm Authors

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <time.h>
#include <absl/container/flat_hash_set.h>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/flags/usage.h>
#include <absl/time/time.h>

//#include <boost/filesystem.hpp>
#include <iostream>
#include <silkworm/db/access_layer.hpp>
#include <silkworm/execution/execution.hpp>

using namespace evmc::literals;

// Non-existing accounts only touched by zero-value internal transactions:
// e.g. https://etherscan.io/address/0x000000000000000000636f6e736f6c652e6c6f67
static const absl::flat_hash_set<evmc::address> kPhantomAccounts{
    0x000000000000000000636f6e736f6c652e6c6f67_address,
    0x2386f26fc10000b4e16d0168e52d35cacd2c6185_address,
    0x5a719cf3e02c17c876f6d294adb5cb7c6eb47e2f_address,
};

ABSL_FLAG(std::string, datadir, silkworm::db::default_path(), "chain DB path");
ABSL_FLAG(uint64_t, from, 1, "start from block number (inclusive)");
ABSL_FLAG(uint64_t, to, UINT64_MAX, "check up to block number (exclusive)");

int main(int argc, char* argv[]) {
    absl::SetProgramUsageMessage("Executes Ethereum blocks and compares resulting change sets against DB.");
    absl::ParseCommandLine(argc, argv);

//    namespace fs = boost::filesystem;
//
//    fs::path db_path(absl::GetFlag(FLAGS_datadir));
//    if (!fs::exists(db_path) || !fs::is_directory(db_path) || db_path.empty()) {
//        std::cerr << absl::GetFlag(FLAGS_datadir) << " does not exist.\n";
//        std::cerr << "Use --db flag to point to a Turbo-Geth populated chaindata.\n";
//        return -1;
//    }
    std::string path = "/Volumes/Samsung_T4/turbo_geth/tg/chaindata/"

    absl::Time t1{absl::Now()};
    std::cout << t1 << " Checking change sets in " << absl::GetFlag(FLAGS_datadir) << "\n";

    using namespace silkworm;

    lmdb::options db_opts{};
    db_opts.read_only = true;
    std::shared_ptr<lmdb::Environment> env{lmdb::get_env(absl::GetFlag(FLAGS_datadir).c_str(), db_opts)};

    const uint64_t from{absl::GetFlag(FLAGS_from)};
    const uint64_t to{absl::GetFlag(FLAGS_to)};

    uint64_t block_num{from};
    for (; block_num < to; ++block_num) {
        std::unique_ptr<lmdb::Transaction> txn{env->begin_ro_transaction()};

        std::optional<BlockWithHash> bh{db::read_block(*txn, block_num, /*read_senders=*/true)};
        if (!bh) {
            break;
        }

        db::Buffer buffer{txn.get(), block_num};

        execute_block(bh->block, buffer);

        std::optional<db::AccountChanges> db_account_changes{db::read_account_changes(*txn, block_num)};
        if (buffer.account_changes() != db_account_changes) {
            bool mismatch{false};
            if (db_account_changes) {
                for (const auto& e : *db_account_changes) {
                    if (buffer.account_changes().count(e.first) == 0) {
                        if (!kPhantomAccounts.contains(e.first)) {
                            std::cerr << to_hex(e.first) << " is missing\n";
                            mismatch = true;
                        }
                    } else if (Bytes val{buffer.account_changes().at(e.first)}; val != e.second) {
                        std::cerr << "Value mismatch for " << to_hex(e.first) << ":\n";
                        std::cerr << to_hex(val) << "\n";
                        std::cerr << "vs DB\n";
                        std::cerr << to_hex(e.second) << "\n";
                        mismatch = true;
                    }
                }
                for (const auto& e : buffer.account_changes()) {
                    if (db_account_changes->count(e.first) == 0) {
                        std::cerr << to_hex(e.first) << " is not in DB\n";
                        mismatch = true;
                    }
                }
            } else {
                std::cerr << "Nil DB account changes\n";
                mismatch = true;
            }
            if (mismatch) {
                std::cerr << "Account change mismatch for block " << block_num << " 😲\n";
            }
        }

        Bytes db_storage_changes{db::read_storage_changes(*txn, block_num)};
        Bytes calculated_storage_changes{};
        if (!buffer.storage_changes().empty()) {
            calculated_storage_changes = buffer.storage_changes().encode();
        }
        if (calculated_storage_changes != db_storage_changes) {
            std::cerr << "Storage change mismatch for block " << block_num << " 😲\n";
            std::cerr << to_hex(calculated_storage_changes) << "\n";
            std::cerr << "vs DB\n";
            std::cerr << to_hex(db_storage_changes) << "\n";
        }

        if (block_num % 1000 == 0) {
            absl::Time t2{absl::Now()};
            std::cout << t2 << " Checked blocks ≤ " << block_num << " in " << absl::ToDoubleSeconds(t2 - t1) << " s"
            << std::endl;
            t1 = t2;
        }
    }

    t1 = absl::Now();
    std::cout << t1 << " Blocks [" << from << "; " << block_num << ") have been checked\n";
    return 0;
}
#else
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

const char* STR_DISPLAY_MDBSTAT_VERBOSE =
    "\n"
    "[  {p:ID}:                  ] [{ID}]\n"
    "[  {p:NAME}:                ] [{NAME}]\n"
    "[  {p:LONGNAME}:            ] [{LONGNAME}]\n"
    "[  {p:DESCRIPTION}:         ] [{w:75:DESCRIPTION}]\n"
    "[  {p:STAT::PSIZE}:         ] [{STAT::PSIZE}]\n"
    "[  {p:STAT::DEPTH}:         ] [{STAT::DEPTH}]\n"
    "[  {p:STAT::BRANCH_PAGES}:  ] [{STAT::BRANCH_PAGES} pages][, {STAT::BRANCH_BYTES} bytes]\n"
    "[  {p:STAT::LEAF_PAGES}:    ] [{STAT::LEAF_PAGES} pages][, {STAT::LEAF_BYTES} bytes]\n"
    "[  {p:STAT::OVERFLOW_PAGES}:] [{STAT::OVERFLOW_PAGES} pages][, {STAT::OVERFLOW_BYTES} bytes]\n"
    "[  {p:STAT::ENTRIES}:       ] [{STAT::ENTRIES}]\n";

//-----------------------------------------------------------------------
bool COptions::handle_tables(void) {
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    for (auto table : tables12) {
        if (name.empty() || table.name == name) {
            if (isText) {
                if (verbose) {
                    cout << bBlue << table.Format(STR_DISPLAY_MDBSTAT_VERBOSE);

                } else {
                    cout << bBlue << table.Format(expContext().fmtMap["format"]) << endl;
                }

            } else {
                if (!first)
                    cout << "," << endl;
                cout << "  ";
                incIndent();
                table.doExport(cout);
                decIndent();
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
                incIndent();
                record.doExport(cout);
                decIndent();
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
#endif
