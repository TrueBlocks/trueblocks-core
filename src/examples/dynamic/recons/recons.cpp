/*-----------------------------------------------------------------------
 * qblocks - fast, fully-decentralized data from blockchains
 *----------------------------------------------------------------------*/
#include "acctlib.h"

const char* STR_DAI_ADDR = "0x6b175474e89094c44da98b954eedeac495271d0f";
const char* STR_WETH_ADDR = "0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2";
class CUniPrice : public CEthCall {
    address_t factory = address_t("0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f");
    string_q getPairSelector = "0xe6a43905";
    string_q getPairBytes;
    string_q getReservesSelector = "0x0902f1ac";
    string_q getReservesBytes = "";

  public:
    CUniPrice(void);
    CUniPrice(const CUniPrice& up);
    CUniPrice& operator=(const CUniPrice& up);

    bool setPair(const address_t& r1, const address_t& r2);
};

CUniPrice::CUniPrice(void) {
}

CUniPrice::CUniPrice(const CUniPrice& up) {
}

CUniPrice& CUniPrice::operator=(const CUniPrice& up) {
    return *this;
}

bool CUniPrice::setPair(const address_t& r1, const address_t& r2) {
    if (!isAddress(r1)) {
        LOG_WARN("r1 is not an address: ", r1);
        return false;
    }
    if (!isAddress(r2)) {
        LOG_WARN("r2 is not an address: ", r2);
        return false;
    }
    this->address = factory;
    this->encoding = getPairSelector;
    this->bytes = padLeft(substitute(r1, "0x", ""), 64, '0') + padLeft(substitute(r2, "0x", ""), 64, '0');
    this->abi_spec.loadAbiFromEtherscan(factory);
    this->blockNumber = getBlockProgress(BP_CLIENT).client;
    if (!doEthCall(*this)) {
        LOG_WARN("Could not find pair for ", r1, " to ", r2);
        return false;
    }
    this->address = getResults();
    this->abi_spec.loadAbiFromEtherscan(this->address);
    this->encoding = getReservesSelector;
    this->bytes = getReservesBytes;
    LOG_INFO(bGreen, "Found USD Pair: ", this->address, " with ", this->abi_spec.interfaces.size(), " endpoints", cOff);
    return true;
}

//-----------------------------------------------------------------------
class CTestTraverser : public CTraverser {
  public:
    CUniPrice uni;
    blknum_t prevBlock;
    CTestTraverser(void) : CTraverser("testing"), prevBlock(NOPOS) {
    }
    wei_t etherPriceFromUniswap(blknum_t bn) const;
};

//-----------------------------------------------------------------------
bool header(CTraverser* trav, void* data) {
    if (!((CTestTraverser*)trav)->uni.setPair(STR_DAI_ADDR, STR_WETH_ADDR))
        return false;

    cout << "hash\t";
    cout << "date\t";
    cout << "bn.txid\t";
    cout << "begBalEth\t";
    cout << "totalInEth\t";
    cout << "totalOutEth\t";
    cout << "endBalEth\t";
    cout << "priceUSD\t";
    cout << "begBalUSD\t";
    cout << "totalInUSD\t";
    cout << "totalOutUSD\t";
    cout << "endBalUSD\t";
    cout << "reconciled" << endl;

    return true;
}

//-----------------------------------------------------------------------
bool display(CTraverser* trav, void* data) {
    CTestTraverser* tt = (CTestTraverser*)trav;
    tt->uni.blockNumber = tt->app->blk;
    tt->trans.timestamp = (timestamp_t)expContext().tsMemMap[(tt->app->blk * 2) + 1];
    tt->block.timestamp = (timestamp_t)expContext().tsMemMap[(tt->app->blk * 2) + 1];

    cerr << tt->readStatus << " ";
    if (doEthCall(tt->uni) && !tt->uni.result.outputs.empty()) {
        CStringArray results;
        if (tt->uni.getResults(results) && results.size() > 1) {
            double reserve1 = str_2_Double(wei_2_Ether(str_2_Wei(results[0]), 18));
            double reserve2 = str_2_Double(wei_2_Ether(str_2_Wei(results[1]), 18));
            double price = reserve1 / reserve2;

            CArchive archive(READING_ARCHIVE);
            string_q path = getBinaryCachePath(CT_RECONS, trav->curMonitor->address, trav->trans.blockNumber,
                                               trav->trans.transactionIndex);
            if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
                archive >> trav->trans.statements;
                for (CReconciliation& statement : trav->trans.statements) {
                    if (statement.assetSymbol == "ETH") {
                        double begBal = str_2_Double(substitute(statement.Format("[{BEGBAL}]"), "\"", ""));
                        double totIn = str_2_Double(substitute(statement.Format("[{TOTALIN}]"), "\"", ""));
                        double totOut = str_2_Double(substitute(statement.Format("[{TOTALOUT}]"), "\"", ""));
                        double endBal = str_2_Double(substitute(statement.Format("[{ENDBAL}]"), "\"", ""));

                        cout << tt->trans.hash << "\t";
                        cout << tt->trans.Format("[{DATE}]") << "\t";
                        cout << tt->app->blk << "." << tt->app->txid << "\t";
                        cout << double_2_Str(begBal, 4) << "\t";
                        cout << double_2_Str(totIn, 4) << "\t";
                        cout << double_2_Str(totOut, 4) << "\t";
                        cout << double_2_Str(endBal, 4) << "\t";
                        cout << double_2_Str(price, 2) << "\t";
                        cout << double_2_Str(begBal * price, 2) << "\t";
                        cout << double_2_Str(totIn * price, 2) << "\t";
                        cout << double_2_Str(totOut * price, 2) << "\t";
                        cout << double_2_Str(endBal * price, 2) << "\t";
                        if (tt->prevBlock == trav->app->blk && !statement.reconciled) {
                            cout << "same block" << endl;
                            LOG_INFO(trav->trans.blockNumber, ".", trav->trans.transactionIndex, "\t",
                                     double_2_Str(endBal * price, 2), "\tsame block");
                        } else {
                            cout << (statement.reconciled ? "true" : "false") << endl;
                            LOG_INFO(trav->trans.blockNumber, ".", trav->trans.transactionIndex, "\t",
                                     double_2_Str(endBal * price, 2), "\t", (statement.reconciled ? "true" : "false"));
                        }
                        usleep(30000);
                    }
                }
            } else {
                LOG_WARN("Cannot find recon: ", path);
            }
        } else {
            cerr << "Invalid results from uni at block " << tt->app->blk << "\r";
            cerr.flush();
        }

    } else {
        cerr << "No result at block " << tt->app->blk << "\r";
        cerr.flush();
    }

    tt->prevBlock = tt->app->blk;
    return true;
}

//-----------------------------------------------------------------------
extern "C" CTraverser* makeTraverser(void) {
    acctlib_init(quickQuitHandler);

    freshenTimestamps(getBlockProgress().client);
    loadTimestamps(&expContext().tsMemMap, expContext().tsCnt);
    expContext().asEther = true;

    CTestTraverser* trav = new CTestTraverser;
    trav->preFunc = header;
    trav->dataFunc = noopFunc;
    trav->displayFunc = display;

    return trav;
}
