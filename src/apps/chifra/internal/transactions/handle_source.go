package transactionsPkg

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"

func (opts *TransactionsOptions) HandleSeed() (err error) {
	// logger.Warn("Sourcing of funds is currently not implemented. Please come back soon.")
	return validate.Usage("The --seed flag is currently disabled.")
}

/*
handle_source.cpp

#include "options.h"

//--------------------------------------------------------------
static CAddressBoolMap theMap;

//--------------------------------------------------------------
bool CSourceSearch::traceTransfer(CTransfer& transfer, const string_q& id) {
    string_q cacheFn = "./cache/" + id;
    if (false) {  // fileExists(cacheFn)) {
        // if (debugging) {
        // cout << getJigger("traceTransfer", depth) << bWhite << " ----> " << bGreen << "Skipping " << id
        //      << ". Seen this before." << cOff << string_q(60, ' ') << endl;
        // }
        showFromCache(id);
        return true;
    }
    // theMap[id] = true;
    // doCommand("touch " + cacheFn);

    depth++;
    if (depth <= max_depth) {
        showTransfer("traceTransfer", &transfer);
        CTransactionArray senderTxs;
        getSenderTransactions(senderTxs, transfer.sender, transfer.blockNumber);
        for (auto trans : senderTxs) {
            opt->ledgerManager.accountedFor = trans.from;
            // CSourceSearch search(opt);
            CLedgerManager man(opt->ledgerManager.accountedFor);
            man.getTransfers(trans);
            CStringArray topLevels;
            for (size_t i = 0; i < man.transfers.size(); i++) {
                CTransfer* tt = &man.transfers[i];
                string_q ttId = transfer_2_Id(tt);
                ostringstream msg;
                if (!isZeroAddr(tt->sender)) {
                    if (tt->amount > 0) {
                        if (transfer.sender == tt->recipient) {
                            if (isWhiteListed(tt)) {
                                topLevels.push_back(ttId);
                                // cerr << endl << "Adding to topLevels cache: [" << ttId << "]" << endl;
                                traceTransfer(*tt, ttId);
                            } else {
                                msg << "Skipping not whitelisted: " << tt->assetAddr;
                            }
                        } else {
                            msg << "Skipping not the sender:  " << transfer.sender << " != " << tt->recipient;
                        }
                    } else {
                        msg << "Skipping zero amount";
                    }
                } else {
                    msg << "Skipping zero address";
                }
                if (debugging) {
                    LOG_PROG(bBlack, msg.str(), string_q(120 - msg.str().length(), ' '), cOff);  // , "\r");
                }
            }
            // if (debugging) {
            if (topLevels.size() > 0) {
                string_q childrenFn = "./children/" + id;
                // cout << "Writing to cache at ", childrenFn, " ", substitute(os.str(), "\n", "|"));
                linesToAsciiFile(childrenFn, topLevels, '\n');
            }
            // }
        }
    } else {
        if (debugging) {
            cout << getJigger("traceTransfer", depth) << coloredName(transfer.sender) << bTeal
                 << " --> [--max depth reached: " << id << "]" << string_q(50, ' ') << cOff << endl;
        }
    }
    depth--;

    return true;
}

//--------------------------------------------------------------
string_q getJigger(const string_q& msg, uint64_t depth) {
    ostringstream jigger;
    jigger << padNum2T(uint64_t(depth)) << string_q(size_t(depth), '-') << "> ";
    // jigger << "[" << msg.substr(0, 15) << ": " << padNum2T(uint64_t(depth)) << "] " << string_q(size_t(depth), ' ');
    return jigger.str();
}

//--------------------------------------------------------------
void CSourceSearch::showTransfer(const string_q& msg, const CTransfer* transfer) const {
    // if (transfer->amount == 0) {
    //     return;
    // }

    if (!isWhiteListed((CTransfer*)transfer)) {
        LOG_ERR("Asset is not whiteListed -- should not happen: ", transfer->assetAddr);
        exit(0);
    }

    string_q unitValue = padLeft(wei_2_Ether(transfer->amount, transfer->decimals), 25);
    string_q symbol =
        padRight(substitute(transfer->assetSymbol, "WEI", getChain() == "gnosis" ? "xDAI" : "ETH"), 4).substr(0, 4);

    ostringstream fromTo;
    address_t sender = transfer->sender;
    address_t recipient = transfer->recipient;
    fromTo << sender << " --> " << recipient;

    string_q head = getJigger(msg, depth);
    head = head + fromTo.str();
    head = padRight(head, 5 + max_depth + 42 + 5 + 42);
    head = substitute(head, sender, coloredName(sender));
    head = substitute(head, recipient, coloredName(recipient));

    CName assetName;  // whiteList[transfer->assetAddr];
    ostringstream theSend;
    theSend << " sent " << unitValue << " "
            << " " << bBlue << symbol << cOff;
    // theSend << " sent " << unitValue << " " << addr_2_Color(transfer->assetAddr) << assetName.name << " " << bBlue
    //         << symbol << cOff;

    ostringstream id;
    id << " at " << substitute(transfer_2_Id(transfer), transfer->assetAddr + "_", transfer->transactionHash + " ");

    ostringstream os;
    os << head << " " << bMagenta << transfer->date << cOff << " " << theSend.str() << id.str();
    if (purpleList[transfer->sender]) {
        os << endl;
        os << string_q(120, '-') << endl;
        os << "SENDER ON THE PURPLE LIST: " << coloredName(transfer->sender) << endl;
        os << string_q(120, '-') << endl;
        os << substitute(transfer->Format(""), transfer->sender, coloredName(transfer->sender)) << endl;
        os << string_q(120, '-');
    }
    os << endl;
    string_q cacheFn = "./cache/" + transfer_2_Id(transfer);
    stringToAsciiFile(cacheFn, os.str());
    cout << os.str();
}

//--------------------------------------------------------------
string_q addr_2_Color(const address_t& addr) {
    string_q p1 = addr.substr(2, 16);
    string_q p2 = addr.substr(18, 16);
    uint64_t v = (str_2_Uint(p1) + str_2_Uint(p2)) % 15;
    string_q color = bWhite;
    switch (v) {
        case 0:
        case 1:
            color = bRed;
            break;
        case 2:
            color = bGreen;
            break;
        case 3:
            color = bYellow;
            break;
        case 4:
            color = bBlue;
            break;
        case 5:
            color = bMagenta;
            break;
        case 6:
            color = bTeal;
            break;
        case 7:
            color = bWhite;
            break;
        case 8:
            color = bWhite;
            break;
        case 9:
            color = cRed;
            break;
        case 10:
            color = cGreen;
            break;
        case 11:
            color = cYellow;
            break;
        case 12:
            color = cBlue;
            break;
        case 13:
            color = cMagenta;
            break;
        case 14:
            color = cTeal;
            break;
        case 15:
            color = cWhite;
            break;
    }
    return color;
}

//--------------------------------------------------------------
string_q CSourceSearch::addr_2_Name(const address_t& addr) const {
    CName name;
    if (findName(addr, name)) {
        return padRight(name.name.substr(0, 42), 42, ' ');
    }
    return pet_names ? padRight(addr_2_Petname(addr, '-'), 42, ' ') : addr;
}

//--------------------------------------------------------------
address_t CSourceSearch::coloredName(const address_t& addr) const {
    return addr_2_Color(addr) + addr_2_Name(addr) + cOff;
}

//--------------------------------------------------------------
bool CSourceSearch::parseAppearances(CAppearanceArray& apps, const string_q& result) {
    CStringArray txs;
    explode(txs, result, '\n');

    CAppearanceArray_mon appys;
    for (size_t i = 0; i < txs.size(); i++) {
        CUintArray parts;
        explode(parts, txs[i], '\t');
        if (parts.size() > 2) {
            CAppearance_mon app;
            app.blk = uint32_t(parts[1]);
            app.txid = uint32_t(parts[2]);
            appys.push_back(app);
            // LOG_INFO(i, ". Adding ", uint_2_Str(app.blk), ".", uint_2_Str(app.txid), "\r");
        }
    }
    sort(appys.begin(), appys.end(), sortMonitoredAppearanceReverse);

    for (size_t i = 0; i < appys.size(); i++) {
        CAppearance app;
        app.blockNumber = appys[i].blk;
        app.transactionIndex = appys[i].txid;
        LOG_INFO(i, ". Adding again ", uint_2_Str(app.blockNumber), ".", uint_2_Str(app.transactionIndex), "\r");
        apps.push_back(app);
    }

    if (debugging) {
        LOG_INFO("Found ", apps.size(), " appearances.");
    }
    return apps.size() > 0;
}

//---------------------------------------------------------------
bool writeToCache(CTransaction& trans) {
    string_q fn = getBinaryCacheFilename(CT_TXS, trans.blockNumber, trans.transactionIndex);
    if (fileExists(fn)) {
        return true;
    }

    CBlock block;
    getBlockLight(block, trans.blockNumber);
    trans.timestamp = block.timestamp;
    trans.receipt.status = NO_STATUS;
    getFullReceipt(&trans, false);
    writeTransToBinary(trans, fn);
    bool exists = fileExists(fn);
    LOG_INFO(bMagenta, "Wrote to cache ", fn, cOff, " ", exists, string_q(10, ' '));
    return exists;
}

//---------------------------------------------------------------
bool CSourceSearch::getTransactionAndCache(const address_t& sender, CTransaction& trans, const CAppearanceArray& apps,
                                           size_t which) {
    LOG_PROG(bBlack, "Fetching transaction ", which + 1, " of ", apps.size(), " for sender ", sender, " at ",
             apps[which].blockNumber, ".", apps[which].transactionIndex, cOff, "\r");
    getTransaction(trans, apps[which].blockNumber, apps[which].transactionIndex);
    return writeToCache(trans);
}

CAddressBoolMap tooBigMap;
//---------------------------------------------------------------
void CSourceSearch::getSenderTransactions(CTransactionArray& senderTxs, const address_t& sender, blknum_t lastBlock) {
    if (tooBigMap[sender]) {
        if (debugging) {
            LOG_WARN(bRed, "Skipping address in tooBigMap: ", sender, cOff);
        }
        return;
    }

    ostringstream cmd;
    cmd << "chifra list ";
    cmd << sender;
    cmd << " --no_header ";
    cmd << " --chain " + getChain();
    cmd << " --first_block " << firstBlock;
    cmd << " --last_block " << lastBlock;
    uint64_t count = str_2_Uint(doCommand(cmd.str() + " --count | cut -f2"));
    // 2>/dev/null | cut -f2"));

    if (debugging) {
        LOG_INFO("count:          ", count);
    }
    if (count > max_records) {
        tooBigMap[sender] = true;
        if (debugging) {
            LOG_WARN(bRed, "Skipping too big 1: ", sender, " (", count, " records)", cOff);
        }
        return;
    }

    string_q result = doCommand(cmd.str());  //  + " 2>/dev/null");
    // LOG_INFO(result);
    CAppearanceArray senderAppearances;
    parseAppearances(senderAppearances, result);
    if (senderAppearances.size() > max_records) {
        tooBigMap[sender] = true;
        if (debugging) {
            LOG_WARN(bRed, "Skipping too big 2: ", sender, " (", senderAppearances.size(), " records, count: ", count,
                     ")", cOff);
        }
        return;
    }

    for (size_t i = 0; i < senderAppearances.size(); i++) {
        CAppearance app = senderAppearances[i];
        CTransaction senderTx;
        getTransactionAndCache(sender, senderTx, senderAppearances, i);
        CTransfer tmp;
        tmp.assetAddr = senderTx.to;
        if (isWhiteListed(&tmp) || (!isContractAt(senderTx.to, senderTx.blockNumber) && senderTx.value > 0)) {
            senderTxs.push_back(senderTx);
        }
    }
}

void showFromCache(const string_q& id) {
    string_q cacheFn = "./cache/" + id;
    if (fileExists(cacheFn)) {
        // cout << trim(asciiFileToString(cacheFn), '\n') << bRed << " [<--cache]" << cOff << endl;
        cout << trim(asciiFileToString(cacheFn), '\n') << endl;
    }

    string_q childrenFn = "./children/" + id;
    if (fileExists(childrenFn)) {
        CStringArray children;
        asciiFileToLines(childrenFn, children);
        for (auto child : children) {
            showFromCache(child);
        }
    }
}

//---------------------------------------------------------------
CSourceSearch::CSourceSearch(COptions* o) : opt(o) {
    if (!initialized) {
        ASSERT(fileExists("./source.toml"));
        ASSERT(fileExists("./whiteList.csv"));

        C Toml toml("./source.toml");
        if (!toml.getConfigBool("settings", "colors", true)) {
            colorsOff();
        }

        pet_names = toml.getConfigBool("settings", "pet_names", false);
        max_depth = toml.getConfigInt("settings", "max_depth", max_depth);
        max_records = toml.getConfigInt("settings", "max_records", max_records);
        firstBlock = toml.getConfigInt("settings", getChain() + "_first_block", 0);

        if (fileExists("./whiteList.csv")) {
            CStringArray lines, fields;
            asciiFileToLines("./whiteList.csv", lines);
            for (auto line : lines) {
                CName name;
                if (fields.empty()) {
                    explode(fields, line, ',');
                    line = FAKE_ETH_ADDRESS + string_q(",") + get ChainSymbol() + "," + get ChainSymbol() + ",,18," +
                           addr_2_Petname(FAKE_ETH_ADDRESS, '-');
                    name.parseCSV(fields, line);
                    whiteList[FAKE_ETH_ADDRESS] = name;
                } else {
                    if (!contains(line, FAKE_ETH_ADDRESS)) {
                        name.parseCSV(fields, line);
                        whiteList[name.address] = name;
                    }
                }
            }
        }

        if (fileExists("./purpleList.csv")) {
            CStringArray lines, fields;
            asciiFileToLines("./purpleList.csv", lines);
            for (auto line : lines) {
                purpleList[line] = true;
            }
        }

        debugging = get EnvStr("SHOW_TRANSFERS") == "true" || toml.getConfigBool("settings", "debugging", false);
        if (debugging || verbose) {
            cerr << "whiteList:" << endl;
            for (auto name : whiteList) {
                cerr << "\t" << name.second.Format("[{ADDRESS}]\t[{w:30:NAME}]\t[{w:4:SYMBOL}]\t[{DECIMALS}]") << endl;
            }
            cerr << "purpleList:" << endl;
            for (auto addr : purpleList) {
                cerr << "\t" << addr.first << endl;
            }
        }

        initialized = true;
    }
}

CAddressBoolMap CSourceSearch::purpleList;
CAddressNameMap CSourceSearch::whiteList;
uint64_t CSourceSearch::max_depth{4};
uint64_t CSourceSearch::max_records{3000};
blknum_t CSourceSearch::firstBlock{0};
bool CSourceSearch::pet_names{false};
bool CSourceSearch::debugging{false};
bool CSourceSearch::initialized{false};

handle_source.h

#pragma once
#include "etherlib.h"

//-----------------------------------------------------------------------------
class COptions;
class CSourceSearch {
  public:
    COptions* opt;
    uint64_t depth{0};
    static bool pet_names;
    static uint64_t max_depth;
    static uint64_t max_records;
    static blknum_t firstBlock;
    static CNameMap whiteList;
    static CAddressBoolMap purpleList;
    static bool initialized;
    static bool debugging;
    CSourceSearch(COptions* o);
    bool isPurpleList(const address_t& addr) const {
        return purpleList.size() == 0 || purpleList[addr];
    }
    bool isWhiteListed(CTransfer* tt) const {
        if (whiteList.size() == 0) {
            return true;
        }
        CName name = whiteList[tt->assetAddr];
        if (name.address.empty()) {
            return false;
        }
        tt->assetSymbol = name.symbol;
        return true;
    }
    bool traceTransfer(CTransfer& transfer, const string_q& id);
    void showTransfer(const string_q& msg, const CTransfer* transfer) const;
    bool parseAppearances(CAppearanceArray& array, const string_q& cmd);
    void getSenderTransactions(CTransactionArray& senderTxs, const address_t& sender, blknum_t lastBlock);
    bool getTransactionAndCache(const address_t& sender, CTransaction& trans, const CAppearanceArray& apps,
                                size_t which);
    string_q transfer_2_Id(const CTransfer* transfer) const {
        return transfer->assetAddr + "_" + uint_2_Str(transfer->blockNumber) + "." +
               uint_2_Str(transfer->transactionIndex) + "." + uint_2_Str(transfer->logIndex);
    }
    address_t coloredName(const address_t& addr) const;
    string_q addr_2_Name(const address_t& addr) const;
};

//-----------------------------------------------------------------------------
extern string_q getJigger(const string_q& msg, uint64_t depth);
extern string_q addr_2_Color(const address_t& addr);
extern bool writeToCache(CTransaction& trans);
extern void showFromCache(const string_q& id);

options.cpp
    if (source) {
        LOG_WARN("The --source option is unsupported. Use at your own risk.");
        if (!fileExists("./whiteList.csv")) {
            return usage("./whiteList.csv file is required for this option.");
        }
        expContext().exportFmt = CSV1;
        expContext().fmtMap["header"] = "";
    }

main.cpp
    if (opt->source) {
        opt->ledgerManager.accountedFor = trans.from;

        CSourceSearch search(opt);  // order matters
        CLedgerManager man(opt->ledgerManager.accountedFor);

        opt->showTransactionForSource(trans);
        man.getTransfers(trans);
        for (auto t : man.transfers) {
            search.traceTransfer(t, search.transfer_2_Id(&t));
        }
        return true;
    }

//----------------------------------------------------------------
void COptions::showTransactionForSource(const CTransaction& trans) {
    cout << string_q(140, ' ') << endl;
    cout << bBlack << "chifra transactions --chain " << getChain() << " " << trans.hash;
    if (source) {
        cout << " --source --account_for " << addr_2_Color(ledgerManager.accountedFor) << ledgerManager.accountedFor;
    }
    cout << cOff << endl;
}

*/
