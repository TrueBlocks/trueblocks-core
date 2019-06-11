
#if 0
    string_q fields =
        "CBlock:blockHash,blockNumber|"
        "CTransaction:to,from,blockHash,blockNumber|"
        "CReceipt:to,from,blockHash,blockNumber,transactionHash,transactionIndex,cumulativeGasUsed,logsBloom,root|"
        "CLogEntry:blockHash,blockNumber,transactionHash,transactionIndex,transactionLogIndex,removed,type";
    manageFields(fields, true);
    string_q result;
    queryRawLogs(result, trans.blockNumber, trans.blockNumber);
    if (opt->isVeryRaw) {
        opt->rawItems.push_back(result);
        return true;
    }
    CRPCResult generic;
    generic.parseJson3(result);
    CBlock bl;
    CTransaction tt; tt.pBlock = &bl;
    CReceipt receipt; receipt.pTrans = &tt;
    CLogEntry log; log.pReceipt = &receipt;
    while (log.parseJson3(generic.result)) {
        if (log.getValueByName("transactionIndex") == uint_2_Str(trans.transactionIndex)) {
            opt->rawItems.push_back(log.Format());
        }
        log = CLogEntry();
        log.pReceipt = &receipt;
    }
    if (opt->rawItems.size() == 0) {
        CReceipt rr;
        receipt.pTrans = &trans;
        CLogEntry ll;
        ll.pReceipt = &rr;
        ll.address = "not_an_real_log";
        opt->rawItems.push_back(ll.Format());
    }
#endif

