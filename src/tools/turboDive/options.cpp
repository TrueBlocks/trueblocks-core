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
 * Parts of this file were generated with makeClass. Edit only those parts of the code
 * outside of the BEG_CODE/END_CODE sections
 */
#include "options.h"
#include "silkworm/db/chaindb.hpp"
#include "silkworm/db/tables.hpp"
#include "tableentry.h"

using namespace silkworm;
using namespace silkworm::db::table;

extern map<string_q, string_q> nameMap;
extern const char* STR_DISPLAY_TURBO;
//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("mode", "m", "enum[tables*|dump]", OPT_FLAG, "the command to run"),
    COption("datadir", "d", "<string>", OPT_FLAG, "folder containing TurboGeth data file (data.mdb)"),
    COption("name", "n", "<string>", OPT_FLAG, "for 'dump' command only, the name of the table to dump"),
    COption("goerli", "g", "", OPT_SWITCH, "run against the goerli testnet"),
    COption("no_zero", "z", "", OPT_SWITCH, "suppress the display of tables with zero records"),
    COption("", "", "", OPT_DESCRIPTION, "Dive deeply into the turboGeth database."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, "-m:") || startsWith(arg, "--mode:")) {
            if (!confirmEnum("mode", mode, arg))
                return false;

        } else if (startsWith(arg, "-d:") || startsWith(arg, "--datadir:")) {
            datadir = substitute(substitute(arg, "-d:", ""), "--datadir:", "");

        } else if (startsWith(arg, "-n:") || startsWith(arg, "--name:")) {
            name = substitute(substitute(arg, "-n:", ""), "--name:", "");

        } else if (arg == "-g" || arg == "--goerli") {
            goerli = true;

        } else if (arg == "-z" || arg == "--no_zero") {
            no_zero = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

            // END_CODE_AUTO
        }
    }

    // Data verification
    if (goerli) {
        datadir = substitute(datadir, "/TurboGeth/tg/chaindata/", "/TurboGeth/goerli/tg/chaindata/");
    }

    if (mode.empty())
        return usage("You must provide a mode of operation. Quitting...");

    if (!folderExists(datadir))
        return usage("Folder '" + datadir + "' does not exist. Quitting...");

    if (mode == "dump" && name.empty())
        return usage("You must provide a table name for the dump command. Quitting...");

    if (!loadTables())
        return false;

    // Display formatting
    configureDisplay("turboDive", "CTableEntry", STR_DISPLAY_TURBO);
    if (expContext().exportFmt != NONE1)
        configureDisplay("turboDive", "CTableEntry", substitute(STR_DISPLAY_TABLEENTRY, "[{DESCRIPTION}]\t", ""));
    else
        expContext().exportFmt = TXT1;

    if (mode == "dump") {
        CTableEntry table = tableMap[name];
        if (table.name != name)
            return usage("Table name '" + name + "' not found. Quitting...");
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);
    optionOff(OPT_DENOM);

    // BEG_CODE_INIT
    mode = "";
    datadir = "";
    name = "";
    goerli = false;
    no_zero = false;
    // END_CODE_INIT
    datadir = "/Users/jrush/Library/TurboGeth/tg/chaindata/";
    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    setSorts(GETRUNTIME_CLASS(CBlock), GETRUNTIME_CLASS(CTransaction), GETRUNTIME_CLASS(CReceipt));
    Init();
    // BEG_CODE_NOTES
    // clang-format off
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_MSG
    // END_ERROR_MSG

    expContext().exportFmt = NONE1;

    CTableEntry::registerClass();
    CLmdbStat::registerClass();
    CLmdbRecord::registerClass();
    CLmdbVal::registerClass();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
bool COptions::loadTables(void) {
    map<string_q, uint32_t> silkyMap;
    uint64_t nTables = (sizeof(kTables) / sizeof(silkworm::lmdb::TableConfig));
    for (size_t i{0}; i < nTables; i++)
        silkyMap[kTables[i].name] = 1;

    size_t page_size = 4096;

    // Try to load the database
    lmdb::options opts{};
    opts.map_size = ((fileSize(datadir + "data.mdb") + page_size - 1) / page_size) * page_size;
    opts.read_only = true;
    lmdb_env = lmdb::get_env(datadir.c_str(), opts);
    lmdb_txn = lmdb_env->begin_ro_transaction();

    MDB_stat stat;

    auto unnamed = lmdb_txn->open(lmdb::FREE_DBI);
    unnamed->get_stat(&stat);

    CTableEntry e;
    e.id = unnamed->get_dbi();
    e.name = unnamed->get_name();
    e.description = nameMap[e.name];
    e.longName = nextTokenClear(e.description, '|');
    e.silky = silkyMap[e.name];
    e.stat.psize = stat.ms_psize;
    e.stat.depth = stat.ms_depth;
    e.stat.branch_pages = stat.ms_branch_pages;
    e.stat.leaf_pages = stat.ms_leaf_pages;
    e.stat.overflow_pages = stat.ms_overflow_pages;
    e.stat.entries = stat.ms_entries;
    tables12.push_back(e);
    tableMap[e.name] = e;

    unnamed.reset();
    unnamed = lmdb_txn->open(lmdb::MAIN_DBI);
    unnamed->get_stat(&stat);

    e = CTableEntry();
    e.id = unnamed->get_dbi();
    e.name = unnamed->get_name();
    e.description = nameMap[e.name];
    e.longName = nextTokenClear(e.description, '|');
    e.silky = silkyMap[e.name];
    e.stat.psize = stat.ms_psize;
    e.stat.depth = stat.ms_depth;
    e.stat.branch_pages = stat.ms_branch_pages;
    e.stat.leaf_pages = stat.ms_leaf_pages;
    e.stat.overflow_pages = stat.ms_overflow_pages;
    e.stat.entries = stat.ms_entries;
    tables12.push_back(e);
    tableMap[e.name] = e;

    if (!stat.ms_entries)
        return usage("No tables found in " + datadir + "data.mdb. Quitting...");

    MDB_val key, value;
    int rc = unnamed->get_first(&key, &value);
    if (rc != MDB_SUCCESS) {
        LOG_WARN("Something went wrong reading values from database: ", rc);
        return usage("Quitting...");
    }

    while (rc != MDB_NOTFOUND) {
        if (value.mv_size < sizeof(size_t)) {
            LOG_WARN("Sizes disagree when reading database: ", value.mv_size, sizeof(size_t), MDB_INVALID);
            return usage("Quitting...");
        }

        // auto p_data = static_cast<uint8_t*>(value.mv_data);
        auto named = lmdb_txn->open({(const char*)key.mv_data});
        named->get_stat(&stat);

        if (!no_zero || stat.ms_entries) {
            e = CTableEntry();
            e.id = named->get_dbi();
            e.name = named->get_name();
            e.description = nameMap[e.name];
            e.longName = nextTokenClear(e.description, '|');
            e.silky = silkyMap[e.name];
            e.stat.psize = stat.ms_psize;
            e.stat.depth = stat.ms_depth;
            e.stat.branch_pages = stat.ms_branch_pages;
            e.stat.leaf_pages = stat.ms_leaf_pages;
            e.stat.overflow_pages = stat.ms_overflow_pages;
            e.stat.entries = stat.ms_entries;
            tables12.push_back(e);
            tableMap[e.name] = e;
        }

        rc = unnamed->get_next(&key, &value);
        if (rc != MDB_NOTFOUND)
            lmdb::err_handler(rc);
    }
    lmdb_txn.reset();
    lmdb_env.reset();

    return true;
}

//-----------------------------------------------------------------------
const char* STR_MIGRATIONS =
    "Migrations|Serialized SyncStageProgress and SyncStageUnwind buckets, it stores stages progress to understand "
    "in which context was executed migration in case of bug-report developer can ask content of this bucket";
const char* STR_CONTRACTCODEBUCKET = "ContractCodeBucket|key - addressHash+incarnation, value - code hash";
const char* STR_PLAINCONTRACTCODEBUCKET =
    "PlainContractCodeBucket|Plain State, key - address+incarnation, value - code hash";
const char* STR_CURRENTSTATEBUCKET =
    "CurrentStateBucket|Contains Accounts:,key - address hash,value - account encoded for storage,Contains Storage:,"
    "key - address hash + incarnation + storage key hash,value - storage value(common.hash),"
    "{Flags:DupSort,AutoDupSortKeysConversion:true,DupFromLen:72,DupToLen:40,}";
const char* STR_CURRENTSTATEBUCKET_OLD =
    "CurrentStateBucketOld1 (deprecated)|Contains Accounts:,key - address hash,value - account encoded for "
    "storage,Contains Storage:,"
    "key - address hash + incarnation + storage key hash,value - storage value(common.hash)";
const char* STR_CODEBUCKET = "CodeBucket|key - contract code hash, value - contract code";
const char* STR_PLAINACCOUNTCHANGESETBUCKET =
    "PlainAccountChangeSetBucket|Keeps changesets of accounts (\"plain state\"), key - encoded timestamp(block number),"
    "value - encoded ChangeSet{k - address v - account(encoded).";
const char* STR_INTERMEDIATETRIEHASHBUCKET =
    "IntermediateTrieHashBucket|some_prefix_of(hash_of_address_of_account) => hash_of_subtrie,{Flags: DupSort, "
    "CustomDupComparator: DupCmpSuffix32,}";
const char* STR_INTERMEDIATETRIEHASHBUCKET_OLD =
    "IntermediateTrieHashBucketOld1 (deprecated)|some_prefix_of(hash_of_address_of_account) => hash_of_subtrie";
const char* STR_INODESBUCKET = "InodesBucket|This bucket stored in separated database.";
const char* STR_STORAGEHISTORYBUCKET =
    "StorageHistoryBucket|key - address hash,value - list of block where it's changed";
const char* STR_FASTTRIEPROGRESSKEY =
    "FastTrieProgressKey|fastTrieProgressKey tracks the number of trie entries imported during fast sync.";
const char* STR_HEADBLOCKKEY = "HeadBlockKey|headBlockKey tracks the latest know full block's hash.";
const char* STR_HEADFASTBLOCKKEY =
    "HeadFastBlockKey|headFastBlockKey tracks the latest known incomplete block's hash during fast sync.";
const char* STR_SYNCSTAGEPROGRESS = "SyncStageProgress|Progress of sync stages: stageName -> stageData";
const char* STR_SYNCSTAGEPROGRESSOLD1 =
    "SyncStageProgressOld1 (deprecated)|Progress of sync stages: stageName -> stageData";
const char* STR_SYNCSTAGEUNWIND = "SyncStageUnwind|Position to where to unwind sync stages: stageName -> stageData";
const char* STR_SYNCSTAGEUNWINDOLD1 =
    "SyncStageUnwindOld1 (deprecated)|Position to where to unwind sync stages: stageName -> stageData";
const char* STR_CLIQUEBUCKET = "CliqueBucket|No description";
const char* STR_SENDERS = "Senders|Transaction senders - stored separately from the block bodies";
const char* STR_ACCOUNTCHANGESETBUCKET =
    "AccountChangeSetBucket|keeps changesets of accounts,key - encoded timestamp(block number),value - encoded "
    "ChangeSet{k - addrHash v - account(encoded).";
const char* STR_STORAGECHANGESETBUCKET =
    "StorageChangeSetBucket|keeps changesets of storage, key - encoded timestamp(block number), value - encoded "
    "ChangeSet{k - compositeKey(for storage) v - originalValue(common.Hash)}.";
const char* STR_PLAINSTORAGECHANGESETBUCKET =
    "PlainStorageChangeSetBucket|keeps changesets of storage (\"plain state\"),key - encoded timestamp(block "
    "number),value - encoded ChangeSet{k - plainCompositeKey(for storage) v - originalValue(common.Hash)}.";
const char* STR_ACCOUNTSHISTORYBUCKET =
    "AccountHistoryBucket|key - address hash,value - list of block where it's changed";
const char* STR_DATABASEVERISIONKEY =
    "databaseVerisionKey|tracks the current database version.|Data item prefixes (use single byte to avoid mixing "
    "data types, avoid `i`, used for indexes).|headerPrefix + num (uint64 big endian) + hash -> header";
const char* STR_LOGTOPICINDEX =
    "LogTopicIndex|Stores bitmap indices - in which block numbers saw logs of given 'address' or 'topic'|[addr or "
    "topic] + [2 bytes inverted shard number] -> bitmap(blockN)|indices are sharded - because some bitmaps are >1Mb "
    "and when new incoming blocks process it|updates ~300 of bitmaps - by append small amount new values. It cause "
    "much big writes (LMDB does copy-on-write).|if last existing shard size merge it with delta|if serialized size of "
    "delta > ShardLimit - break down to multiple shards|shard number - it's biggest value in bitmap";
const char* STR_LOGADDRESSINDEX =
    "LogAddressIndex|Stores bitmap indices - in which block numbers saw logs of given 'address' or 'topic'|[addr or "
    "topic] + [2 bytes inverted shard number] -> bitmap(blockN)|indices are sharded - because some bitmaps are >1Mb "
    "and when new incoming blocks process it|updates ~300 of bitmaps - by append small amount new values. It cause "
    "much big writes (LMDB does copy-on-write).|if last existing shard size merge it with delta|if serialized size of "
    "delta > ShardLimit - break down to multiple shards|shard number - it's biggest value in bitmap";
const char* STR_CALLFROMINDEX =
    "CallFromIndex|Indices for call traces - have the same format as LogTopicIndex and LogAddressIndex|Store bitmap "
    "indices - in which block number we saw calls from (CallFromIndex) or to (CallToIndex) some addresses";
const char* STR_CALLTOINDEX =
    "CallToIndex|Indices for call traces - have the same format as LogTopicIndex and LogAddressIndex|Store bitmap "
    "indices - in which block number we saw calls from (CallFromIndex) or to (CallToIndex) some addresses";
const char* STR_TXLOOKUPPREFIX = "TxLookupPrefix|txLookupPrefix + hash -> transaction/receipt lookup metadata";
const char* STR_PREIMAGEPREFIX = "PreImagePrefix|preimagePrefix + hash -> preimage";
const char* STR_CONFIGPREFIX = "ConfigPrefix|config prefix for the db";
const char* STR_BLOOMBITSINDEXPREFIX =
    "BloomBitsIndexprefix|Chain index prefixes (use `i` + single byte to avoid mixing data "
    "types).|BloomBitsIndexPrefix is the data table of a chain indexer to track its progress";
const char* STR_SNAPSHOTINFOBUCKET = "SnapshotInfoBucket|is used to store information about data layout.";
const char* STR_DATABASEINFOBUCKET = "DatabaseInfoBucket|is used to store information about data layout.";
const char* STR_INCARNATIONMAPBUCKET =
    "IncarnationMapBucket|Incarnations for deleted accounts,key - address,value - incarnation of account when it was "
    "last deleted";
const char* STR_HEADERNUMBERPREFIX =
    "HeaderNumberPrefix|Data item prefixes (use single byte to avoid mixing data types, avoid `i`, used for "
    "indexes).|headerNumberPrefix + hash -> num (uint64 big endian)";
const char* STR_BLOCKBODYPREFIX = "BlockBodyPrefix|blockBodyPrefix + num (uint64 big endian) + hash -> block body";
const char* STR_BLOCKRECEIPTSPREFIX =
    "BlockReceiptsPrefix|blockReceiptsPrefix + num (uint64 big endian) + hash -> block receipts";
const char* STR_HEADERPREFIX = "HeaderPrefix|headerPrefix + num (uint64 big endian) + hash -> header";
const char* STR_BLOOMBITSPREFIX =
    "BloomBitsPrefix|bloomBitsPrefix + bit (uint16 big endian) + section (uint64 big endian) + hash -> bloom bits";
const char* STR_PLAINSTATEBUCKET =
    "PlainStateBucket|Plain State. The same as CurrentStateBucket, but the keys arent' hashed.|Logical "
    "layout:|Contains Accounts:|key - address (unhashed)|value - account encoded for storage|Contains Storage:|key - "
    "address (unhashed) + incarnation + storage key (unhashed)|value - storage value(common.hash)||Physical "
    "layout:|PlainStateBucket and CurrentStateBucket utilises DupSort feature of LMDB (store multiple values inside 1 "
    "key).|-------------------------------------------------------------|key              |            "
    "value|-------------------------------------------------------------|[acc_hash]              | "
    "[acc_value]|[acc_hash]+[inc]        | [storage1_hash]+[storage1_value]| [storage2_hash]+[storage2_value] // this "
    "value has no own key. it's 2nd value of [acc_hash]+[inc] key.| [storage3_hash]+[storage3_value]| "
    "...|[acc_hash]+[old_inc]    [storage1_hash]+[storage1_value]| ...|[acc2_hash]             | "
    "[acc2_value]|PlainStateBucket: {|Flags:                     DupSort,|AutoDupSortKeysConversion: true,|DupFromLen: "
    "               60,|DupToLen:                  28,";
const char* STR_PLAINSTATEBUCKETOLD1 =
    "PlainStateBucketOld1|Plain State. The same as CurrentStateBucket, but the keys arent' hashed.|Logical "
    "layout:|Contains Accounts:|key - address (unhashed)|value - account encoded for storage|Contains Storage:|key - "
    "address (unhashed) + incarnation + storage key (unhashed)|value - storage value(common.hash)||Physical "
    "layout:|PlainStateBucket and CurrentStateBucket utilises DupSort feature of LMDB (store multiple values inside 1 "
    "key).|-------------------------------------------------------------|key              |            "
    "value|-------------------------------------------------------------|[acc_hash]              | "
    "[acc_value]|[acc_hash]+[inc]        | [storage1_hash]+[storage1_value]| [storage2_hash]+[storage2_value] // this "
    "value has no own key. it's 2nd value of [acc_hash]+[inc] key.| [storage3_hash]+[storage3_value]| "
    "...|[acc_hash]+[old_inc]    [storage1_hash]+[storage1_value]| ...|[acc2_hash]             | "
    "[acc2_value]|PlainStateBucket: {|Flags:                     DupSort,|AutoDupSortKeysConversion: true,|DupFromLen: "
    "               60,|DupToLen:                  28,";
const char* STR_LOG = "Log|blockReceiptsPrefix + num (uint64 big endian) + hash -> block receipts";
const char* STR_HEADHEADERKEY = "HeadHeaderKey|no description";
const char* STR_MAINDBI = "Main DBI|[MAIN_DBI]";
const char* STR_FREEDBI = "Free DBI|[FREE_DBI]";

//-----------------------------------------------------------------------
map<string_q, string_q> nameMap = {
    {"[FREE_DBI]", STR_FREEDBI},
    {"[MAIN_DBI]", STR_MAINDBI},
    {"migrations", STR_MIGRATIONS},
    {"contractCode", STR_CONTRACTCODEBUCKET},
    {"PLAIN-contractCode", STR_PLAINCONTRACTCODEBUCKET},
    {"CST2", STR_CURRENTSTATEBUCKET},
    {"CST", STR_CURRENTSTATEBUCKET_OLD},
    {"CODE", STR_CODEBUCKET},
    {"PLAIN-ACS", STR_PLAINACCOUNTCHANGESETBUCKET},
    {"iTh2", STR_INTERMEDIATETRIEHASHBUCKET},
    {"iTh", STR_INTERMEDIATETRIEHASHBUCKET_OLD},
    {"inodes", STR_INODESBUCKET},
    {"hST", STR_STORAGEHISTORYBUCKET},
    {"TrieSync", STR_FASTTRIEPROGRESSKEY},
    {"LastBlock", STR_HEADBLOCKKEY},
    {"LastFast", STR_HEADFASTBLOCKKEY},
    {"SSP2", STR_SYNCSTAGEPROGRESS},
    {"SSP", STR_SYNCSTAGEPROGRESSOLD1},
    {"SSU2", STR_SYNCSTAGEUNWIND},
    {"SSU", STR_SYNCSTAGEUNWINDOLD1},
    {"clique-", STR_CLIQUEBUCKET},
    {"txSenders", STR_SENDERS},
    {"ACS", STR_ACCOUNTCHANGESETBUCKET},
    {"SCS", STR_STORAGECHANGESETBUCKET},
    {"DatabaseVersion", STR_DATABASEVERISIONKEY},
    {"b", STR_BLOCKBODYPREFIX},
    {"r", STR_BLOCKRECEIPTSPREFIX},
    {"log_topic_index", STR_LOGTOPICINDEX},
    {"log_address_index", STR_LOGADDRESSINDEX},
    {"call_from_index", STR_CALLFROMINDEX},
    {"call_to_index", STR_CALLTOINDEX},
    {"l", STR_TXLOOKUPPREFIX},
    {"secure-key-", STR_PREIMAGEPREFIX},
    {"ethereum-config-", STR_CONFIGPREFIX},
    {"iB", STR_BLOOMBITSINDEXPREFIX},
    {"PLAIN-SCS", STR_PLAINSTORAGECHANGESETBUCKET},
    {"hAT", STR_ACCOUNTSHISTORYBUCKET},
    {"SNINFO", STR_SNAPSHOTINFOBUCKET},
    {"DBINFO", STR_DATABASEINFOBUCKET},
    {"incarnationMap", STR_INCARNATIONMAPBUCKET},
    {"H", STR_HEADERNUMBERPREFIX},
    {"h", STR_HEADERPREFIX},
    {"B", STR_BLOOMBITSPREFIX},
    {"PLAIN-CST2", STR_PLAINSTATEBUCKET},
    {"PLAIN-CST", STR_PLAINSTATEBUCKETOLD1},
    {"log", STR_LOG},
    {"LastHeader", STR_HEADHEADERKEY},
};

//-----------------------------------------------------------------------
const char* STR_DISPLAY_TURBO =
    "[{ID}]-[{SILKY}]\t"
    "[{w:25:LONGNAME}]\t"
    "[{w:20:NAME}]\t"
    "[{r:5:STAT::DEPTH}]\t"
    "[{r:15:STAT::ENTRIES}]\t"
    "[{r:15:STAT::TOTAL}]\t"
    "[{r:15:STAT::AVG}]";
