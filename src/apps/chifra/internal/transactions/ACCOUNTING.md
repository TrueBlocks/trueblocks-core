# How to Do Perfect 18-Decimal Place Accurate Accounting for Any EVM-Based Address at Near-Zero Cost

## Definitions

### Address
- Any ethereum address including both smart contracts and externally owned accounts (EOA).
- *Suggestion:* Stop calling them EOAs. It means nothing to anyoen other than blockchain heads.
- We should call them "user's accounts" or just "accounts" and we do so in the following document.

### Appearance
- For a given address, an appearance is a `<bn.txid>` where `bn` is the block number and `txid` is the transaction index of where in the block the address appears. Any given address may "appears" many times on the blockchain.
- Appearances happen in obvious places such as the `from` and `to` addresses of a transaction, or in much more esoteric places such as inside of `logs` and even deep inside a trace 15 layers down the call stack.
- If an address appears more than once in a single transaction we only record the first such appearance. This is encapsulated in the idea of a unique appearance.
- Appearances are transactions, but they are not what people normally think of as transactions.
- This definition is purposefuly as inclusive as possible. This inclusivness is needed to ensure an account's history reconciles.
- The Unchained Index is an index (i.e. a list) of every appearance of every address anywhere on the chain.

### Account History
- An account history is the list of unique appearances for any given address.
- Without a complete and accurate account history, it is not possible to deliver a reconciled accounting of balance (or any state) changes.

### Asset
- An asset is a smart contract that, during any point in its history, responds with a non-zero value when queried for `totalSupply` or `getBalance`.
- An asset may or may not be an ERC20 (or any derivative) token.
- Smart contracts that are not ERC20 complient may still be labeled an asset. This is purposeful in order to rather more inclusive than less.
- Any smart contract complient with the ERC20 standard (or any derivitive) is automatically included.
- Note that we purosefully do not denote an address as an asset by virtue of generating Transfer logs as this may not correspond to a balance transfer. Only smart contracts that exhibit true balance transfers count as assets.

### Block
- A list of transactions include in the canonical fork of a given blockchain.
- Blocks that contain no transactions are valid blocks.
- A block may or may not include a block reward.
- The block reward for a given blockchain may change over time.
- A block may or may not (depending on the blockchain) contain a `miner` or `proposer` address.
- Blocks may or may not contain miners of `uncle` blocks.

### Transaction
- A transaction must be included in a "finalized" block to be called a transaction. (This purposefully excludes pending transactions or transactions in unripe blocks.)
- Every transaction contains at least one (and frequently many more) *traces*
- Many transactions (but no-where near all) contain one or more logs.
- Transactions will always contain at least one unique address appearance. This is because for any transaction to be initiated, it must be initiated from an EOA (User's account). This address will appear in the `from` field of the transaction.

### Reciept
- Every transaction has exactly one reciept.
- Receipts store certain information including gas usage and the creation of new smart contracts and also a list of logs (or events) generated during the execution of the transaction.
- A transaction that has not yet been included in a block (a pending transaction) will have no events.

### Logs (Events)
- A log (or event) is the representation of an "event" that occured during the execution of a transaction.
- Logs appear as an array in the transaction's receipt.
- There may be any number of logs in a given receipt (including zero).
- Logs may be generated at any level of the execution trace of a single transaction including many levels deep in the deepest trace.
- When doing a reconciliation, not every log in a given transaction will be `relevant` to the account being reconciled. For example, during an airdrop, balences change for many addresses, but only the log denoting the balance change for the address currently being reconciled is `relevant`.

### Log Topic
- Every log has at least one and possibly more "topics." Topic0 (topic zero) is a representation of the Event Signature of the log. For example, for the event signature `Transfer(address,address,uint256)` the topic is `0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef`.
- Topics may be used to filter logs which may be massive otherwise.

### Trace
- A trace is the unit of processing at which we can do reconcilations.
- Traces appear as an array of one or more items in each transaction.
- The first trace in the list is identical (modulo a few small gas details) to the transaction itself.
- A trace consists of three parts. The `Trace` itself. A `TraceAction` (which records the "intention" of the action) and a `TraceResult` that records the result of the calculation.
- Asset transfers happend during the execution of a trace (but may be signaled in an log).
- Traces are expensive to query and should be avoided at all costs. Logs are much faster to query, but may be inadequate to fully reconcile a given transaction.
- Traces should only be queried when absolutely necessary.

### Transfer
- A transfer is a *trace* or *log* that indicates a change in balance of asset for a given address.
- Transfers may happen between any two addresses.
- There are three types of transfers our system accounts for.
  - Direct address to address transfers of the ETH token, 
  - ERC20 transfers,
  - Systemic transfers, and
  - Implied transfers as detailed below.

### Direct Address to Address Transfers of ETH
- In this case, ETH tokens are transfers from the account balance of one address to another. This is recorded in the `value` or `amount` feild of the `TraceAction` (and because the first trace IS the transaction, equivently in the transaction).
- An event may be, but very frequently is not, generated to note such a transfer. For example, when an EOA sends ETH to another EOA, no events are generated.
- ETH transfers are accurate to 18-decimals places (called WEI).

### ERC20 Transfers
- ERC20 transfers are recorded in logs with the topic for for the event signature of `Transfer(address,address,uint256)` (see above).
- If a log has this topic, and the address being reconciled is involved in this log (and there is an actual balance transfer) then this log will signal an `ERC20 Transfer`).

### Systemic Transfers
- There are multiple special case occurences in the history of the chain where balances change due to the operation of the system. For example:
  - Mining rewards
  - Uncle rewards
  - Slashing / staking rewards
  - Reception of value from self-descrtucts
  - Token transfers without a corresponding event generation
    - Many token mint and airdrop occureneces
    - Accumulation of compounded interest in certain smart contracts
  
### Implied Transfers
- Because of the Turing Complete nature of the blockchain programming environment, it is not possible to enforce rules on smart contracts that preclude them from behaving in any way they wish. This is in the nature of programming languages.
- Sometimes a transaction will modify the ETH or ERC20 token balance

### Beginning Balance
- The balance of a given asset for a given address prior to the processing in a given block.
- The RPC provides balances only at the block level, not at the transaction level, therefore inter-block transactions become important.
- Note that balances are only available from the chain per block.

### Ending Balance
- The balance of a given asset for a given address after the processing in a block completes.
- Note that balances are only available from the chain per block.

### Next Begining Balance
- The balance of the account at the start of the next block (identical to *Ending Balance* for this block)
- Useful only for explanation.

### Ending Balance at Last Appearance
- Distinqished from ending balance (which is the balance at the end of the previous block relative to an appearance), this is the balance of the asset for the given address at the end of that address's last known appearance.
- Note that the ending balance at the last appearance may be many blocks in the past or even a transaction within the same block as the given appearance.
- This value is used when creating a reconciliation in date-ascending order. The *begining balance* for a given appearance better be the same as the *ending balance at last appearance*, otherwise the history does not reconcile.

### Beginning Balance at Next Appearance
- Distinquished from *begining balance* which is the asset balance of an account prior to the execution of the block, this value is the balance of an account at before the start of the next appearance.
- It is useful when producing a reconciled history in date-reversed order.
- It is also useful in date-ascending order to simulate intra-block balance histories.

### Intra-Block Balance Histories
- Because balances are only available at the end of a block, one must "simulate" account balances for an account that has mulitple transactions in the same block.
- There are four cases:
  - The transaction is alone in the block,
    - *Ending Balance* and *Next Beginning Balance*
  - There are more than one transactions in a block and the transaction is the first in the block,
    - *Ending Balance* and *Beginning Balance at Next Appearance*
  - There are more than one transactions in a block and the transaction is the last in the block,
    - *Ending Balance at Last Appearance* and *Next Beginning Balance*
  - There are more than one transactions in a block and the transaction is neither first nor last
    - *Ending Balance at Last Appearance* and *Beginnging Balance at Next Appearance*

### Statement
- A profit and loss statement for a single address and a single asset at a single transaction

### Per-transaction reconcoliation
- a group of one or more statements for a given address and asset at a given transaction

### Reconcilation
- one or more per-transaction reconciliations

## Concepts:

- The once reconciled, always reconciled principle

## How to Reconcile an Account History:

for each transaction in the address's history:
  if the item has already been reconciled and cached, return it
	// readReconciliationFromCache:
	clear all statements in the transaction
	if the cache item does not exist, return false
	else

  else

//--------------------------------------------------------------
bool CLedgerManager::getStatements(CTransaction& trans) {
    if (trans.readReconsFromCache(accountedFor)) {
//==============================================================================
    statements.clear();
    if (isTestMode()) {
        return false;
    }

    string_q path = getReconcilationPath(accountedFor);
    if (!fileExists(path)) {
        return false;
    }

    CArchive archive(READING_ARCHIVE);
    if (archive.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        archive >> statements;
        archive.Release();
        for (auto& statement : statements) {
            // Prior to 0.45.0, we sometimes cached unreconciled statements, clean them
            bool isReconciled = statement.reconciled();
            bool isBackLevel = statement.m_schema < getVersionNum(0, 43, 0);
            if (isBackLevel || !isReconciled) {
                // This is an old version of the reconciliation, delete it and return false
                LOG_WARN("Back-level cache for statements found. Removing....");
                statements.clear();
                ::remove(path.c_str());
                return false;
            }

            // Freshen in case user has changed the names database since putting the statement in the cache
            CName tokenName;
            if (findToken(statement.assetAddr, tokenName)) {
                statement.assetSymbol = tokenName.symbol;
                statement.decimals = tokenName.decimals;
            }

            statement.pTransaction = this;
        }
        return !shouldQuit();
    }

    return false;//==============================================================================

        searchOp = READ;
        for (auto& statement : trans.statements) {
            string_q key = statementKey(statement.accountedFor, statement.assetAddr);
            ledgers[key] = statement;
        }
        return !shouldQuit();
    }

    searchOp = RECONCILE;
    getTransfers(trans);

bool CLedgerManager::getTransfers(const CTransaction& trans) {
    timestamp_t ts = str_2_Ts(trans.Format("[{TIMESTAMP}]"));
    string_q encoding = trans.input.substr(0, 10);
    time_q dt = ts_2_Date(ts);

    CTransferArray tmp;
    for (auto& log : trans.receipt.logs) {
        CName tokenName;
        findToken(log.address, tokenName);
        if (tokenName.address.empty()) {
            tokenName.address = log.address;
            tokenName.petname = addr_2_Petname(tokenName.address, '-');
        }

        CTransfer transfer;
        if (isTokenTransfer(log.topics[0]) && log.topics.size() > 2) {
            transfer.sender = topic_2_Addr(log.topics[1]);
            transfer.recipient = topic_2_Addr(log.topics[2]);
            transfer.amount = str_2_Wei(log.data);

            // TODO: Would enabling these make more transfers reconcile?
            // } else if (log.topics.size() > 1) {
            //     if (isWithdrawalTransfer(log.topics[0])) {
            //         transfer.sender = topic_2_Addr(log.topics[1]);
            //         transfer.recipient = topic_2_Addr(trans.from);
            //         transfer.amount = str_2_Wei(log.data);
            //     } else if (isDepositTransfer(log.topics[0]) || isMintTransfer(log.topics[0]) ||
            //                isBurnTransfer(log.topics[0]) || isStakedTransfer(log.topics[0])) {
            //         transfer.sender = topic_2_Addr(trans.from);
            //         transfer.recipient = topic_2_Addr(log.topics[1]);
            //         transfer.amount = str_2_Wei(log.data);
            //     }

            bool nonZero = transfer.amount != 0;
            bool ofInterest = (transfer.sender == accountedFor || transfer.recipient == accountedFor);
            if (nonZero && ofInterest) {
                transfer.blockNumber = trans.blockNumber;
                transfer.transactionIndex = trans.transactionIndex;
                transfer.logIndex = log.logIndex;
                transfer.timestamp = ts;
                transfer.date = dt;
                transfer.transactionHash = trans.hash;
                transfer.encoding = encoding;
                transfer.assetAddr = log.address;
                transfer.log = (CLog*)&log;  // TODO: for debugging only, can be removed

                transfer.assetSymbol = tokenName.symbol;
                if (transfer.assetSymbol.empty()) {
                    transfer.assetSymbol = getTokenSymbol(transfer.assetAddr, trans.blockNumber);
                    if (transfer.assetSymbol.empty()) {
                        transfer.assetSymbol = transfer.assetAddr.substr(0, 4);
                    }
                }

                transfer.decimals = tokenName.decimals;
                if (transfer.decimals == 0) {
                    transfer.decimals = getTokenDecimals(transfer.assetAddr, trans.blockNumber);
                    if (transfer.decimals == 0) {
                        transfer.decimals = 18;
                    }
                }

                tmp.push_back(transfer);
            }
        }
    }

    // sorts the transfers (other than the top level) by assetAddr, blockNumber, transactionIndex, logIndex
    sort(tmp.begin(), tmp.end());

    transfers.clear();
    transfers.reserve(tmp.size() + 2);

    // Adds the top level transaction itself to the front of the array
    transfers.push_back(trans.toTransfer());
    transfers.insert(transfers.end(), tmp.begin(), tmp.end());

    return transfers.size() > 0;
}
}












    bool hasBogus = false;
    for (size_t i = 0; i < transfers.size(); i++) {
        CTransfer& transfer = transfers[i];
        if (filterByAsset(transfer.assetAddr)) {
//--------------------
    bool filterByAsset(const address_t& asset) {
        return (!isFilterOn() || assetFilter[asset]);
    }
//--------------------
            string tokenKey = statementKey(accountedFor, transfer.assetAddr);
            if (ledgers[tokenKey] == CLedgerEntry()) {
                CReconciliation prevStatement(accountedFor, transfer.assetAddr, &trans);
                if (trans.blockNumber > 0) {
                    prevStatement.blockNumber = max(trans.blockNumber, blknum_t(1)) - 1;
                    prevStatement.endBal = trans.blockNumber == 0 ? 0
                                                                  : getTokenBalanceAt(transfer.assetAddr, accountedFor,
                                                                                      prevStatement.blockNumber);
                }
                ledgers[tokenKey] = prevStatement;
            }

            CReconciliation statement(accountedFor, transfer.assetAddr, &trans);
            statement.logIndex = transfer.logIndex;
            statement.assetSymbol = transfer.assetSymbol;
            statement.decimals = transfer.decimals;
            if (!statement.reconcileFlows(transfer)) {
                statement.reconcileFlows_traces();
//==========================================================================
//-----------------------------------------------------------------------
bool CReconciliation::reconcileFlows(const CTransfer& transfer) {
    sender = transfer.sender;
    recipient = transfer.recipient;

    bool isEth = isEtherAddr(assetAddr);
    if (isEth) {
        if (pTransaction->from == accountedFor) {
            gasOut = str_2_BigInt(pTransaction->getValueByName("gasCost"));
        }

        // Do not collapse. Both may be true.
        if (sender == accountedFor) {
            amountOut = pTransaction->isError ? 0 : pTransaction->value;
        }

        // Do not collapse. Both may be true.
        if (recipient == accountedFor) {
            if (sender == "0xPrefund") {
                prefundIn = pTransaction->value;

            } else if (sender == "0xBlockReward") {
                minerBaseRewardIn = pTransaction->value;
                minerNephewRewardIn = pTransaction->extraValue1;
                minerTxFeeIn = pTransaction->extraValue2;

            } else if (sender == "0xUncleReward") {
                minerUncleRewardIn = pTransaction->value;

            } else {
                amountIn = pTransaction->isError ? 0 : pTransaction->value;
            }
        }

    } else {
        // Do not collapse. Both may be true.
        if (sender == accountedFor) {
            amountOut = transfer.amount;
        }

        // Do not collapse. Both may be true.
        if (recipient == accountedFor) {
            amountIn = transfer.amount;
        }

        if (sender != accountedFor && recipient != accountedFor) {
            LOG_ERR("Invalid state in reconcileFlows. Quitting...");
            exit(0);
        }
    }

    // For trialBalance only
    begBal = blockNumber == 0 ? 0 : getTokenBalanceAt(assetAddr, accountedFor, blockNumber - 1);
    endBal = getTokenBalanceAt(assetAddr, accountedFor, blockNumber);

    ostringstream os;
    LOG_TRIAL_BALANCE(isEth ? "flows-top" : "flows-token");
    if (trialBalance()) {
        return true;
    }

    return reconcileFlows_traces();
}

//-----------------------------------------------------------------------
bool CReconciliation::reconcileFlows_traces(void) {
    bool isEth = isEtherAddr(assetAddr);
    if (!isEth) {
        return true;
    }

    amountIn = 0;
    internalIn = selfDestructIn = prefundIn = 0;
    minerBaseRewardIn = minerNephewRewardIn = minerTxFeeIn + minerUncleRewardIn = 0;
    amountOut = 0;
    internalOut = selfDestructOut = 0;  // don't reset gasOut as it's always top level

    if (pTransaction->blockNumber == 0) {
        sender = pTransaction->from;
        recipient = pTransaction->to;
        begBal = 0;
        prefundIn = pTransaction->value;

    } else {
        reconciliationType = "trace-";
        bool tracesAllocated = false;
        if (pTransaction->traces.size() == 0) {
            blknum_t bn = pTransaction->blockNumber;
            blknum_t txid = pTransaction->transactionIndex;
            loadTraces(*((CTransaction*)pTransaction), bn, txid, false, false);  // NOLINT
            tracesAllocated = true;
        }

        size_t index = 0;
        for (auto trace : pTransaction->traces) {
            if (!trace.action.selfDestructed.empty()) {
                // do not collapse, both may happen
                if (trace.action.refundAddress == accountedFor) {
                    sender = trace.action.from;
                    recipient = trace.action.to;
                    selfDestructIn += trace.action.balance;
                }

                // do not collapse, both may happen
                if (trace.action.selfDestructed == accountedFor) {
                    sender = trace.action.from;
                    recipient = trace.action.to;
                    selfDestructOut += trace.action.balance;
                }

            } else {
                if (trace.action.from == accountedFor && !trace.isDelegateCall()) {
                    // Sometimes, EOAs appear here, but there is no way
                    // that a trace can initiate an expenditure on an EOA
                    // TODO(tjayrush): unless it's the first trace?
                    // unless the EOA initiated the top level tx. I think
                    // this might be a bug in a smart contract or something.
                    if (isContractAt(accountedFor, blockNumber) || pTransaction->from == accountedFor) {
                        sender = trace.action.from;
                        recipient = trace.action.to;
                        if (index == 0) {
                            amountOut += pTransaction->isError ? 0 : trace.action.value;
                        } else {
                            internalOut += pTransaction->isError ? 0 : trace.action.value;
                        }
                    }
                }

                if (trace.action.to == accountedFor && !trace.isDelegateCall()) {
                    sender = trace.action.from;
                    recipient = trace.action.to;
                    if (pTransaction->from == "0xPrefund") {
                        prefundIn = pTransaction->value;

                    } else if (pTransaction->from == "0xBlockReward") {
                        minerBaseRewardIn = pTransaction->value;
                        minerNephewRewardIn = pTransaction->extraValue1;
                        minerTxFeeIn = pTransaction->extraValue2;

                    } else if (pTransaction->from == "0xUncleReward") {
                        minerUncleRewardIn = pTransaction->value;

                    } else {
                        if (index == 0) {
                            amountIn += pTransaction->isError ? 0 : trace.action.value;
                        } else {
                            internalIn += pTransaction->isError ? 0 : trace.action.value;
                        }
                    }
                }
            }
            index++;
        }

        if (tracesAllocated) {
            ((CTransaction*)pTransaction)->traces.clear();
        }
    }

    ostringstream os;
    LOG_TRIAL_BALANCE("traces");
    return trialBalance();
}
//==========================================================================
            }

            bool isPrevDiff = transfer.blockNumber == 0 || (ledgers[tokenKey].blockNumber != transfer.blockNumber);
            bool isNextDiff = nextBlock != transfer.blockNumber && i == transfers.size() - 1;
            if (isTestMode()) {
                LOG_INFO("isPrevDiff: ", isPrevDiff, " isNextDiff: ", isNextDiff);
                LOG_INFO("ledger.blockNumber: ", ledgers[tokenKey].blockNumber,
                         " prevBlock: ", ledgers[tokenKey].blockNumber, " transfer.blockNumber: ", transfer.blockNumber,
                         " nextBlock: ", nextBlock);
            }

            statement.prevAppBlk = ledgers[tokenKey].blockNumber;
            statement.prevBal = ledgers[tokenKey].balance;
            bigint_t begBal, endBal;
            bool isBogus = false;
            if (!statement.reconcileBalances(isPrevDiff, isNextDiff, begBal, endBal)) {
//==========================================================================
//-----------------------------------------------------------------------
bool CReconciliation::reconcileBalances(bool prevDifferent, bool nextDifferent, bigint_t& begBalOut,
                                        bigint_t& endBalOut) {
    bigint_t balEOLB = blockNumber == 0 ? 0 : getTokenBalanceAt(assetAddr, accountedFor, blockNumber - 1);
    bigint_t balEOB = getTokenBalanceAt(assetAddr, accountedFor, blockNumber);
    begBalOut = balEOLB;
    endBalOut = balEOB;

    if (blockNumber == 0) {
        // balances are zero

    } else if (prevDifferent && nextDifferent) {
        // The trace reconcile may have changed values
        begBal = balEOLB;
        endBal = balEOB;

    } else if (prevDifferent) {
        // This tx has a tx after it in the same block but none before it
        begBal = balEOLB;
        endBal = endBalCalc();

    } else if (nextDifferent) {
        // This tx has a tx before it in the block but none after it
        begBal = prevBal;
        endBal = balEOB;

    } else {
        // this tx has both a tx before it and one after it in the same block
        begBal = prevBal;
        endBal = endBalCalc();
    }

    if (pTransaction->blockNumber == 0) {
        reconciliationType = "genesis";

    } else {
        if (prevDifferent && nextDifferent) {
            reconciliationType += "regular";

        } else if (prevDifferent) {
            reconciliationType += "prevDiff-same";

        } else if (nextDifferent) {
            reconciliationType += "same-nextDiff";

        } else {
            reconciliationType += "same-same";
        }
    }
    reconciliationType += isEtherAddr(assetAddr) ? "-eth" : "-token";

    ostringstream os;
    os << "[" << blockNumber << "] " << prevDifferent << " " << nextDifferent;
    LOG_TRIAL_BALANCE(isEtherAddr(assetAddr) ? "balances-top" : "balances-token");
    if (isTestMode()) {
        LOG_INFO("");
        LOG_INFO("");
    }

    return trialBalance();
}
//==========================================================================
                // This fixes a lot of unreconciled transactions.
                //
                // We need to do this because sometimes hackers generate Transfer events but do not change
                // token balances. We need to be able to detect this and skip these events. We do this by
                // looking for a large number of events and no balance change. If we find this, we call it
                // a 'bogus' transfer and skip it. A better method would have been to query the number of
                // state changes in the transaction, but that's not available in the logs. So, we have to
                // do this.
                bool lotsOfLogs = trans.receipt.logs.size() > 10;
                bool mayBeAirdrop = isZeroAddr(transfer.sender) || transfer.sender == trans.to;
                bool noBalanceChange = (begBal == endBal);
                if ((lotsOfLogs || mayBeAirdrop) && noBalanceChange) {
                    // Sometimes, people generate events but do not change balances. Call FakeFishing on Etherscan
                    // LOG_WARN("Probably bogus transfer skipped: ", trans.hash);
                    statement.reconciliationType = "skipped";
                    statement.internalOut = statement.amountNet();
                    isBogus = true;
                }
            }
            if (statement.amountNet() != 0 || isBogus) {
                trans.statements.push_back(statement);
                ledgers[tokenKey] = statement;
            }
            hasBogus |= isBogus;
        }
    }

    for (size_t s = 0; s < trans.statements.size(); s++) {
        CReconciliation* st = &trans.statements[s];
        st->spotPrice = getPriceInUsd(st->assetAddr, st->priceSource, st->blockNumber);
    }

    if (hasBogus) {
        // We've determined that this transaction has bogus transfers, but we cache it anyway since
        // it's probably all bogus.
        trans.cacheConditional(accountedFor);
    }

    return !shouldQuit();
}

//-----------------------------------------------------------------------
static bool isTokenTransfer(const string_q& logNeedle) {
    return logNeedle == str_2_Topic("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
}

// //-----------------------------------------------------------------------
// static bool isDepositTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0xe1fffcc4923d04b559f4d29a8bfc6cda04eb5b0d3c460751c2402c5c5cc9109c");
// }

// //-----------------------------------------------------------------------
// static bool isWithdrawalTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x7fcf532c15f0a6db0bd6d0e038bea71d30d808c7d98cb3bf7268a95bf5081b65");
// }

// //--------------------------------------------------------------
// static bool isMintTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x0f6798a560793a54c3bcfe86a93cde1e73087d944c0ea20544137d4121396885");
// }

// //--------------------------------------------------------------
// static bool isBurnTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0xcc16f5dbb4873280815c1ee09dbd06736cffcc184412cf7a71a0fdb75d397ca5");
// }

// //--------------------------------------------------------------
// static bool isStakedTransfer(const string_q& logNeedle) {
//     return logNeedle == str_2_Topic("0x9e71bc8eea02a63969f509818f2dafb9254532904319f9dbda79b67bd34a5f3d");
// }

//--------------------------------------------------------------
bool CLedgerManager::getTransfers(const CTransaction& trans) {
}

//--------------------------------------------------------------------------------
void CLedgerManager::getPrevNext(size_t index, const CTransaction& trans) {
    nextBlock = NOPOS;
    if (index < appArray.size() - 1) {
        nextBlock = appArray[index + 1].blk;
    }
}

*/
