/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

extern bool process_reconciliation(COptions* options, CTransaction& trans, CReconciliationMap& prev, blknum_t next);

//-----------------------------------------------------------------------
bool acct_Display(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (fourByteFilter(trav->trans.input, opt)) {
        if (opt->accounting) {
            blknum_t next = trav->index < opt->monApps.size() - 1 ? opt->monApps[trav->index + 1].blk : NOPOS;
            process_reconciliation(opt, trav->trans, opt->prevStatements, next);
        }
        cout << ((isJson() && !opt->firstOut) ? ", " : "");
        cout << trav->trans;
        opt->firstOut = false;
    }

    prog_Log(trav, data);
    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool acct_Pre(CTraverser* trav, void* data) {
    COptions* opt = (COptions*)data;

    if (opt->monApps.size() > 0 && opt->first_record != 0) {
        CReconciliation eth;
        eth.blockNumber = opt->monApps[0].blk - 1;
        eth.endBal = getBalanceAt(opt->accountedFor, opt->monApps[0].blk - 1);
        opt->prevStatements[opt->accountedFor + "_eth"] = eth;
    }
    return true;
}

class CReconCache {
  public:
    CTransaction* trans = nullptr;
    CReconCache(void) {
    }
};

//-----------------------------------------------------------------------
bool loadRecon(const string_q& path, void* data) {
    CReconCache* thing = (CReconCache*)data;
    CReconciliation nums(thing->trans->blockNumber, thing->trans->transactionIndex, thing->trans->timestamp);
    readNodeFromBinary(nums, path);
    thing->trans->statements.push_back(nums);
    return true;
}
//-----------------------------------------------------------------------
bool process_reconciliation(COptions* options, CTransaction& trans, CReconciliationMap& prevStatements, blknum_t next) {
    string_q path = getBinaryCachePath(CT_RECONS, options->accountedFor, trans.blockNumber, trans.transactionIndex);
    establishFolder(path);

    if (fileExists(path)) {
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
            archive >> trans.statements;
            for (auto statement : trans.statements)
                prevStatements[options->accountedFor + "_" + toLower(statement.asset)] = statement;
            archive.Release();
            return true;
        }
    }

    CAddressBoolMap done;
    CStringArray corrections;
    CReconciliation theStatement(trans.blockNumber, trans.transactionIndex, trans.timestamp);
    theStatement.reconcileEth(corrections, prevStatements, next, &trans, options->accountedFor);
    trans.statements.clear();
    trans.statements.push_back(theStatement);
    prevStatements[options->accountedFor + "_eth"] = theStatement;
    for (auto log : trans.receipt.logs) {
        CAccountName tokenName;
        bool isToken = options->findToken(tokenName, log.address);
        bool isAirdrop = options->airdropMap[log.address];
        bool isDone = done[log.address];
        if ((isToken || trans.hasToken || isAirdrop) && !isDone) {
            CMonitor m;
            m.address = log.address;
            theStatement.reset();
            theStatement.asset = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
            if (theStatement.asset.empty())
                theStatement.asset = getTokenSymbol(m, trans.blockNumber);
            if (isAirdrop && theStatement.asset.empty()) {
                options->getNamedAccount(tokenName, log.address);
                theStatement.asset = tokenName.symbol.empty() ? tokenName.name.substr(0, 4) : tokenName.symbol;
            }
            if (theStatement.asset.empty()) {
                theStatement.asset = log.address.substr(0, 4) + "...";
            }
            theStatement.decimals = tokenName.decimals != 0 ? tokenName.decimals : 18;
            string key = options->accountedFor + "_" + log.address;
            theStatement.begBal = prevStatements[key].endBal;
            theStatement.endBal = str_2_BigInt(getTokenBalanceOf(m, options->accountedFor, trans.blockNumber));
            if (theStatement.begBal > theStatement.endBal) {
                theStatement.amountOut = (theStatement.begBal - theStatement.endBal);
            } else {
                theStatement.amountIn = (theStatement.endBal - theStatement.begBal);
            }
            theStatement.amountNet = theStatement.amountIn - theStatement.amountOut;
            theStatement.reconciled = true;
            theStatement.reconciliationType = "";
            done[log.address] = true;
            if (theStatement.amountNet != 0)
                trans.statements.push_back(theStatement);
            prevStatements[key] = theStatement;
        }
    }

    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(path, modeWriteCreate, LOCK_WAIT)) {
        archive << trans.statements;
        archive.Release();
        return true;
    }

    return true;
}
