/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "jsonTest.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char * argv[])
{
	CFunction::registerClass();
	CParameter::registerClass();
	CAccount::registerClass();
	CTransaction::registerClass();

    // Tell the system where the blocks are and which version to use
    setStorageRoot(BLOCK_CACHE);
    etherlib_init("binary");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;
	}

#if 0
	CAccount theAccount;

	// This is how many records we currently have
	uint32_t origCount  = theAccount.transactions.getCount();
	uint32_t nNewBlocks = 0;
	uint32_t nextRecord = origCount;

	cerr << "\tSlurping new transactions from blockchain...\n";
	uint32_t  nRead = 0;

extern char *cleanUpJson(char *s);
	// Keep reading until we get less than a full page
	SFString contents = asciiFileToString("test.json");
	contents = cleanUpJson((char *)(const char*)contents);
	SFString message = nextTokenClear(contents, '[');
	uint32_t nRecords = countOf('}',contents)-1;
	nRead += nRecords;
	cerr << "\tDownloaded " << nRead << " potentially new transactions." << (isTesting?"\n":"\r");

	uint32_t minBlock=0,maxBlock=0;
	findBlockRange(contents, minBlock, maxBlock);
	cerr << "\n\tDownload contains blocks from " << minBlock << " to " << maxBlock << "\n";

	// Keep track of which last full page we've read
	theAccount.lastPage = 0;
	theAccount.pageSize = 5000;

	cerr << "\tSearching transactions";
	if (theAccount.lastBlock>0)
	    cerr << " after block " << theAccount.lastBlock;
	cerr << "\n";

	// pre allocate the array
	theAccount.transactions.Grow(nRead);

    CPerformanceClock bigStart = qbNow();
	uint32_t lastBlock=0;
	uint32_t cnt=0;
	char *p = (char *)(const char*)contents;
	while (p && *p)
	{
		CTransaction trans;uint32_t nFields=0;
//double littleStart = qbNow();
		p = trans.parseJson(p,nFields);
//double littleStart2 = qbNow();
		if (nFields)
		{
			uint32_t transBlock = trans.blockNumber;
			if (transBlock > theAccount.lastBlock) // add the new transaction if it's in a new block
			{
				theAccount.transactions[nextRecord++] = trans;
				lastBlock = transBlock;
				if (!(++nNewBlocks%REP_FREQ))
				{
					cerr << "\tFound new transaction at block " << transBlock << ". Importing..." << (isTesting?"\n":"\r");
					cerr.flush();
				}
			}
		}
		cnt++;
//double littleEnd = qbNow();
if (!(nNewBlocks%REP_FREQ))
{
//	cerr << "one record: " << (littleEnd-littleStart) << "\n";
//	cerr << "one parse: " << (littleStart2-littleStart) << "\n";
}
	}
	if (!isTesting && nNewBlocks) { cerr << "\tFound new transaction at block " << lastBlock << ". Importing...\n"; cerr.flush(); }
    CPerformanceClock bigEnd = qbNow();
    cerr << "all records: " << (bigEnd - bigStart) << "\n";
#endif

	return 0;
}

//--------------------------------------------------------------------------------
void findBlockRange(const SFString& json, uint32_t& minBlock, uint32_t& maxBlock)
{
	SFString search = "blockNumber:";
	uint32_t len = search.length();

	minBlock = 0;
	int32_t first = json.find(search);
	if (first != NOPOS)
	{
		SFString str = json.substr(first+len);
		minBlock = toLong(str);
	}

	SFString end = json.substr(json.ReverseFind('{'));//pull off the last transaction
	int32_t last = end.find(search);
	if (last != NOPOS)
	{
		SFString str = end.substr(last+len);
		maxBlock = toLong(str);
	}
}
