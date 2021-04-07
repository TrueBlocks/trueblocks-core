/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
//#include "stake.h"

// extern bool build_cap_table(COptions& options, int argc, const char* argv[]);
//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
#if 0
    etherlib_init("binary", quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        cerr << bGreen << "Building cap table for ..."
             << "\n"
             << cOff;
        if (!build_cap_table(options, argc, argv))
            cerr << "\tThe tool ended with an error.";
        cerr << bGreen << "...done\n" << cOff;
    }

    etherlib_cleanup();
#endif
    return 0;
}

#if 0
//--------------------------------------------------------------
bool build_cap_table(COptions& options, int argc, const char* argv[]) {
    // We need to be able to open the cache
    CArchive txCache(READING_ARCHIVE);
    if (!txCache.Lock(options.cacheFile, modeReadOnly, LOCK_NOWAIT)) {
        cout << "Could not open file: " << options.cacheFile << ".";
        return EXIT_FAILURE;
    }

    size_t counter = 0;
    do {
        uint32_t transID, blockNum;
        txCache >> blockNum >> transID;
        if (blockNum >= options.start) {
            CTransaction trans;
            getTransaction(trans, blockNum, transID);
            getReceipt(trans.receipt, trans.hash);
            if (true) {  // }!trans.is_error) { // ignore errors
                CBlock block;
                getBlock(block, blockNum);
                trans.pBlock = &block;
                options.abi_spec.articulateTransaction(&trans);
                if (!verbose && !(counter % 13)) {
                    cerr << cTeal << "Processing: " << cOff;
                    cerr << blockNum << " : ";
                    cerr << ts_2_Date(block.timestamp) << " ";
                }

                for (auto log : trans.receipt.logs) {
                    CFunction art = log.articulatedLog;

                    if (log.articulatedLog.name == "CreatedToken" || log.articulatedLog.name == "Mint") {
                        // event Mint        (address _to, uint256 _value)
                        // event CreatedToken(address _to, uint256 _value)
                        address_t receiver = art.inputs[0].value;
                        wei_t prevBal = options.getTokenBalance(receiver, blockNum - 1);
                        wei_t curBal = options.getTokenBalance(receiver, blockNum);
                        wei_t amount = str_2_Wei(art.inputs[1].value);

                        options.updateHolder(trans, receiver, curBal);

                        if (verbose) {
                            string_q result = trans.Format(substitute(fmtOut, "{CHNG}", "{CHNG}+"));
                            replace(result, "{DATE}",
                                    substitute(substitute(trans.Format("[{DATE}]"), " UTC", ""), " ", "T"));
                            replace(result, "{TYPE}", log.articulatedLog.name);
                            replace(result, "{HOLDER}", receiver);
                            replace(result, "{BAL1}", pad(prevBal));
                            replace(result, "{CHNG}", pad(amount));
                            replace(result, "{BAL2}", pad(curBal));
                            cout << result << endl;
                        }

                    } else if (log.articulatedLog.name == "Transfer") {
                        // event Transfer(address _from, address _to, uint256 _value)
                        address_t sender = art.inputs[0].value;
                        wei_t prevBalS = options.getTokenBalance(sender, blockNum - 1);
                        wei_t curBalS = options.getTokenBalance(sender, blockNum);

                        address_t receiver = art.inputs[1].value;
                        wei_t prevBalR = options.getTokenBalance(receiver, blockNum - 1);
                        wei_t curBalR = options.getTokenBalance(receiver, blockNum);

                        wei_t amount = str_2_Wei(art.inputs[2].value);

                        options.updateHolder(trans, sender, curBalS);
                        options.updateHolder(trans, receiver, curBalR);

                        if (verbose) {
                            string_q result = trans.Format(substitute(fmtOut, "{CHNG}", "{CHNG}-"));
                            replace(result, "{DATE}",
                                    substitute(substitute(trans.Format("[{DATE}]"), " UTC", ""), " ", "T"));
                            replace(result, "{TYPE}", log.articulatedLog.name);
                            replace(result, "{HOLDER}", sender);
                            replace(result, "{BAL1}", pad(prevBalS));
                            replace(result, "{CHNG}", pad(amount));
                            replace(result, "{BAL2}", pad(curBalS));
                            cout << result << endl;

                            result = trans.Format(substitute(fmtOut, "{CHNG}", "{CHNG}+"));
                            replace(result, "{DATE}",
                                    substitute(substitute(trans.Format("[{DATE}]"), " UTC", ""), " ", "T"));
                            replace(result, "{TYPE}", log.articulatedLog.name);
                            replace(result, "{HOLDER}", receiver);
                            replace(result, "{BAL1}", pad(prevBalR));
                            replace(result, "{CHNG}", pad(amount));
                            replace(result, "{BAL2}", pad(curBalR));
                            cout << result << endl;

                            cout << endl;
                        }
                    }
                    if (!verbose && !(counter % 13))
                        cerr << log.articulatedLog.name << "-";
                }
                if (!verbose && !(counter % 13)) {
                    cerr << "                              \r";
                    cerr.flush();
                }
                counter++;
            }
        }
    } while (!txCache.Eof());

    txCache.Release();
    return true;
}

// event Destruction(uint256 _amount)
// event Issuance   (uint256 _amount)
// event OwnerUpdate(address _prevOwner,address _newOwner)
//-------------------------------------------------------------------------
string_q COptions::getTotalSupply(blknum_t blockNum) {
#ifdef DEBUGGING
    return "1000000000000";
#else
    string_q encoding = "0x18160ddd";
    string_q cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"[CMD]\"}, \"[BLOCK]\"]";
    replace(cmd, "[CMD]", encoding);
    replace(cmd, "[TOKEN]", token);
    replace(cmd, "[BLOCK]", uint_2_Hex(blockNum));

    CFunction ret;
    if (!abi_spec.articulateOutputs(encoding, callRPC("eth_call", cmd, false), ret))
        return "";
    return ret.outputs[0].value;
#endif
}

//-------------------------------------------------------------------------
wei_t COptions::getTokenBalance(const address_t& holder, blknum_t blockNum) {
#ifdef DEBUGGING
    wei_t w = str_2_Wei(holder);
    w = (w >> 22);
    return w;
#else
    string_q encoding = "0x70a08231";
    string_q cmd = "[{\"to\": \"[TOKEN]\", \"data\": \"[CMD][HOLDER]\"}, \"[BLOCK]\"]";
    replace(cmd, "[CMD]", encoding);
    replace(cmd, "[TOKEN]", token);
    replace(cmd, "[HOLDER]", padLeft(extract(holder, 2), 64, '0'));  // encoded data (may be empty)
    replace(cmd, "[BLOCK]", uint_2_Hex(blockNum));

    CFunction ret;
    if (!abi_spec.articulateOutputs(encoding, callRPC("eth_call", cmd, false), ret))
        return 0;
    return str_2_Wei(ret.outputs[0].value);
#endif
}

//-------------------------------------------------------------------------
string_q pad(wei_t in) {
    return padLeft(wei_2_Str(in), 28);
}

//-------------------------------------------------------------------------
const string_q fmtOut =
    "{TYPE}\t[{TIMESTAMP}]\t[{BLOCKNUMBER}].[{TRANSACTIONINDEX}]\t[{IS_ERROR}]\t{HOLDER}\t[{r:28:VALUE}]\t{BAL1}\t{"
    "CHNG}\t{BAL2}";

//-------------------------------------------------------------------------
void COptions::updateHolder(const CTransaction& trans, const address_t& addr, const wei_t& amt) {
    ASSERT(trans.pBlock);
    blknum_t bucket = (trans.pBlock->blockNumber / bucketSize) * bucketSize + 1;

    if (bucket != capTable.pOptions->curBucket) {
        // create a new table since we're at a new block
        blknum_t prev = capTable.pOptions->curBucket;
        capTable.pOptions->totSupply = getTotalSupply(capTable.pOptions->curBucket);
        capTable.pOptions->curBucket = bucket;
        capTable.pOptions->ts = trans.pBlock->timestamp;
        if (prev != NOPOS)
            cout << capTable.pOptions->report() << endl;
    }

    CStake stake;
    stake.address = addr;
    stake.stake = amt;

    CCapTable::iterator it = capTable.find(addr);
    if (it == capTable.end()) {
        capTable.insert(make_pair(addr, stake));
    } else {
        it->second = stake;
    }
}

//-------------------------------------------------------------------------
uint64_t COptions::countNonZero(void) {
    uint64_t ret = 0;
    for (CCapTable::iterator it = capTable.begin(); it != capTable.end(); it++)
        if (it->second.stake > 0)
            ret++;
    return ret;
}

#include "stake.h"
//-------------------------------------------------------------------------
string_q COptions::report(void) {
    CStakeArray stakes;
    for (CCapTable::iterator it = capTable.begin(); it != capTable.end(); it++) {
        if (it->second.stake > 0) {
            CStake stake;
            stake.address = it->first;
            stake.stake = it->second.stake;
            stake.pMapItem = &(it->second);
            stakes.push_back(stake);
        }
    }
    if (reverse)
        sort(stakes.begin(), stakes.end(), reverseSort);
    else
        sort(stakes.begin(), stakes.end());

// #define EXPORT
#ifdef EXPORT
    ostringstream os;
    for (auto stake : stakes)
        os << stake.address << "\t" << stake.stake << endl;
    string_q fileName = "./export_" + uint_2_Str(curBucket - 1) + ".txt";
    stringToAsciiFile(fileName, os.str());
    return "Exported " + fileName + "                              ";
#else
    wei_t total = str_2_Wei(totSupply);

    string_q fmt =
        "Block:   [{BN}]  Date:   [{DATE}]\n"
        "Bucket:  [{BK}]  nRows:  [{ROWS}]\n"
        "Holders: [{HL}]  Supply: [{SUPPLY}]\n";
    string_q ret = fmt;
    replaceAll(ret, "[{", cYellow);
    replaceAll(ret, "}]", cOff);
    replace(ret, "BN", padNum9T(curBucket - 1));
    replace(ret, "DATE", ts_2_Date(ts).Format(FMT_EXPORT));
    replace(ret, "BK", padNum9T(bucketSize));
    replace(ret, "ROWS", uint_2_Str(nRows));
    replace(ret, "HL", padNum9T(countNonZero()));
    replace(ret, "SUPPLY", wei_2_Str(total));

    ostringstream os;
    os << string_q(90, '=') << endl;
    os << ret << endl;
    os << padCenter("holder", 42) << "\t" << padCenter("stake", 28) << endl;
    os << string_q(120, '-') << endl;
    uint64_t cnt = 0;
    for (auto& stake : stakes) {
        cnt++;
        if (cnt < nRows) {
            os << stake.display(cnt) << endl;
        } else {
            stake.pMapItem->lastPos = cnt;
            stake.lastPos = cnt;
        }
    }
    os << string_q(120, '-') << endl;
    os << ret << endl;
    return os.str();
#endif
}
#endif
