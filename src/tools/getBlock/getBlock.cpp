/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

extern SFString doOneBlock(SFUint32 num, const COptions& opt);
extern SFString checkOneBlock(SFUint32 num, const COptions& opt);
extern SFString normalizeBlock(const SFString& inIn, bool remove);
//------------------------------------------------------------
int main(int argc, const char * argv[]) {

    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

#if 0
    if (options.isCheck) {
        HIDE_FIELD(CTransaction, "gasLimit");
        HIDE_FIELD(CTransaction, "cumulativeGasUsed");
        HIDE_FIELD(CTransaction, "timestamp");
        HIDE_FIELD(CTransaction, "receipt");
        HIDE_FIELD(CTransaction, "gasUsed");
        HIDE_FIELD(CReceipt,     "contractAddress");
        HIDE_FIELD(CReceipt,     "logsBloom");
        HIDE_FIELD(CReceipt,     "logs");
    }
#endif

    SFString checkResults;
    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // There can be more than one thing to do...
        if (!options.quiet)
            cout << (options.isMulti() ? "[" : "");
        if (options.isRange) {
            for (SFUint32 i = options.start ; i < options.stop ; i++) {
                if (options.isCheck) {
                    checkResults += checkOneBlock(i, options);

                } else {
                    SFString result = doOneBlock(i, options);
                    if (options.normalize) {
                        if (verbose)
                            cout << i << "\n";
                        result = normalizeBlock(result, false);
                    }
                    if (!options.quiet) {
                        cout << result;
                        if (i < options.stop-1)
                            cout << ",";
                        cout << "\n";

                    } else if (!(i%150)) {
                        cerr << ".";
                        cerr.flush();

                    } else if (!(i%1000)) {
                        cerr << "+";
                        cerr.flush();
                    }
                }
            }
        } else {
            for (SFUint32 i = 0 ; i < options.nNums ; i++) {
                if (options.isCheck) {
                    checkResults += checkOneBlock(options.nums[i], options);

                } else {
                    SFString result = doOneBlock(options.nums[i], options);
                    if (options.normalize) {
                        if (verbose)
                            cout << options.nums[i] << "\n";
                        result = normalizeBlock(result, false);
                    }
                    if (!options.quiet) {
                        cout << result;
                        if (i < options.nNums - 1)
                            cout << ",";
                        cout << "\n";
                        if (options.isCheck) {
                            // TODO(any): read 'raw' and compare result with gold
                        }

                    } else if (!(i%150)) {
                        cerr << ".";
                        cerr.flush();

                    } else if (!(i%1000)) {
                        cerr << "+";
                        cerr.flush();
                    }
                }
            }
        }
        if (!options.quiet)
            cout << (options.isMulti() ? "]" : "");

        if (options.isCheck)
            cout << checkResults << "\n";
    }

    return 0;
}

//------------------------------------------------------------
SFString doOneBlock(SFUint32 num, const COptions& opt) {

    CBlock gold;
    SFString result;
    SFString numStr = asStringU(num);
    if (opt.isRaw) {

        if (!queryRawBlock(result, numStr, true, opt.terse)) {
            result = "Could not query raw block " + numStr + ". Is an Ethereum node running?";
        }

    } else {
        // queryBlock returns false if there are no transactions, so ignore the return value
        queryBlock(gold, numStr, true);
        if (opt.force) { // turn this on to force a write of the block to the disc
            SFString fileName = getBinaryFilename1(gold.blockNumber);
            writeToBinary(gold, fileName);
        }

        if (curSource().Contains("Only")) {
            // --source::cache mode doesn't include timestamp in transactions
            for (txnum_t t = 0 ; t < gold.transactions.getCount() ; t++) {
                gold.transactions[t].timestamp = (SFUint32)gold.timestamp;
            }

        }
        result = gold.Format();
    }
    return result;
}

//------------------------------------------------------------
SFString diffStr(const SFString& str1, const SFString& str2) {
    SFString diff = str1;
    SFString junk = str2;
    while (!junk.empty()) {
        SFString line = nextTokenClear(junk,'\n');
        diff.Replace(line,"\n");
    }
    SFString ret = Strip(diff.Substitute("\n\n","\n"), '\n');
    if (!ret.empty())
        ret += "\n";
    return ret;
}

//------------------------------------------------------------
SFString checkOneBlock(SFUint32 num, const COptions& opt) {

    SFString numStr = asStringU(num);

    // Get the block raw from the node...
    SFString fromNode;
    queryRawBlock(fromNode, numStr, true, false);
    fromNode.Replace("\"hash\":","\"blockHash\":");
    if (verbose)
        cout << num << "\n";
    fromNode = normalizeBlock(fromNode, true);

    // Now get the same block from quickBlocks
    SFString fromQblocks;
    CBlock qBlocks;
    queryBlock(qBlocks, numStr, true);
    for (uint32_t i = 0 ; i < qBlocks.transactions.getCount() ; i++) {
        // quickBlocks pulls the receipt for each transaction, but the RPC does
        // not. Therefore, we must set the transactions' gasUsed and logsBloom
        // to be the same as the block's (even though they are not) so they
        // are removed as duplicates. Otherwise, the blocks won't match
        qBlocks.transactions[i].receipt.gasUsed = qBlocks.gasUsed;
        qBlocks.transactions[i].receipt.logsBloom = qBlocks.logsBloom;
    }
    if (verbose)
        cout << num << "\n";
    fromQblocks = normalizeBlock(qBlocks.Format(), true);

    SFString result = "The strings are "; result += ((fromNode != fromQblocks) ? "different\n" : "the same\n");
    SFString diffA  = "In fromNode but not fromQblocks:\n" + diffStr(fromNode, fromQblocks);
    SFString diffB  = "In fromQblocks but not fromNode:\n" + diffStr(fromQblocks, fromNode);

    // return the results
    SFString head = "\n" + SFString('-',80) + "\n";
    return head + "from Node:\n" + fromNode +
            head + "from Quickblocks:\n" + fromQblocks +
            head + result +
            head + diffA +
            head + diffB;
}

//------------------------------------------------------------
SFString removeField(const SFString& strIn, const SFString& field) {

    SFString search = "\"" + field + "\":";
    if (!strIn.Contains(search))
        return strIn;

    SFString ret;
    SFString str = strIn;
    //while (str.Contains(search))
    {
        size_t start = str.find(search);
        SFString before = str.substr(0,start);
        SFString rest   = str.substr(start);
#if 0
        cout << before << "\n";
        cout << rest << "\n";
#endif

        size_t end = rest.find(",")+1; // first comma
        if (rest.startsWith(search+"[")) // unless it's an array, then end of array
            end = rest.find("]")+1;
        SFString during = rest.substr(0,end);
        SFString after  = rest.substr(end);

        before = Strip(before, '\n');
        during = Strip(during, '\n');
        after  = Strip(after,  '\n');
        ret += (before + after);
        str = after;

#if 0
        cout << "Removing field " << field << ": (" << str.length() << ")\n";
        cout << "\t" << SFString('+',50) << "\n\t" << before.Right(50).Substitute("\n","\n\t") << "\n";
        cout << "\t" << SFString('+',50) << "\n\t" << during.Substitute("\n","\n\t") << "\n";
        cout << "\t" << SFString('+',50) << "\n\t" << after.Left(50).Substitute("\n","\n\t") << "\n";
        cout << "\t" << SFString('+',50) << "\n";
#endif
    }

    return ret;
}

//------------------------------------------------------------
SFString cleanAll(const SFString& str, bool remove) {

    static const SFString removes[] = {
        // fields in RPC but not in QuickBlocks
        "author", "condition", "creates", "difficulty", "extraData", "miner", "mixHash", "networkId",
        "nonce", "nonce", // NOT A DUP--IT NEEDS TO BE HERE BECAUSE THERE ARE TWO DIFFERENT NONCES
        "publicKey", "r", "raw", "receiptsRoot", "s", "sealFields", "sha3Uncles", "size", "standardV", "stateRoot",
        "totalDifficulty", "transactionsRoot", "uncles", "v",
        // fields in QuickBlocks but not in RPC
        "contractAddress", "cumulativeGasUsed", "receipt", "address", "data", "logIndex", "topics",
    };
    uint32_t nRemoved = sizeof(removes) / sizeof(SFString);
    if (!remove)
        nRemoved = 0;

    SFString orig = str;
    orig.ReplaceAny("\t\r {}","");
    orig.ReplaceAll(",",",\n"); // put everything on its own line
    orig.Replace("\"jsonrpc\":\"2.0\",", ""); // known junk
    orig.Replace("\"id\":1", "");
    for (uint32_t i = 0 ; i < nRemoved ; i++) {
        SFString search = "\"" + removes[i] + "\":";
        while (orig.Contains(search)) {
            orig = removeField(orig, removes[i]);
        }
    }
    orig = orig.Substitute("}]","");
    orig.ReplaceAll(",", "");
    orig.ReplaceAll("[", "");
    orig.ReplaceAll("]", "");
    orig = StripAny(orig, "\t\n ");
    orig = orig.Substitute("\"result\":","").Substitute("\"transactions\":","").Substitute("\"logs\":","");
    orig = orig.Substitute("0x" + SFString('0',512), "0x0"); // minimize bloom filters
    orig.ReplaceAll("\n\n","\n");
    SFString ret = Strip(orig.Substitute("\"\"","\"\n\"").Substitute("\n\n","\n"),'\n');
    return ret;
}

//------------------------------------------------------------
SFString sorted(const SFString& inIn) {
    SFStringArray inList, outList;
    SFString in = inIn;
    while (!in.empty()) {
        inList[inList.getCount()] = nextTokenClear(in,'\n');
    }
    inList.Sort(sortByStringValue);
    for (uint32_t i = 0 ; i < inList.getCount()-1 ; i++) {
        if (inList[i] != inList[i+1] && inList[i].Contains(":"))
            outList[outList.getCount()] = inList[i];
    }
    // add the last one of it's not already there
    if (inList[inList.getCount()-1] != outList[outList.getCount()-1])
        outList[outList.getCount()] = inList[inList.getCount()-1];

    SFString ret;
    for (uint32_t i = 0 ; i < outList.getCount() ; i++)
        ret += (outList[i] + "\n");
    return ret;
}

//------------------------------------------------------------
SFString normalizeBlock(const SFString& inIn, bool remove) {
    return sorted(cleanAll(inIn, remove));
}
