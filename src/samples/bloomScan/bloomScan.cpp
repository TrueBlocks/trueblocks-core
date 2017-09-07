/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    // Tell the system where the blocks are and which version to use
    etherlib_init("infura");

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    //while (!options.commandList.empty())
    {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;
        if (getSource() != "binary")
            forEveryBlock(displayBloom, null, options.start, options.stop - options.start, 100); //options.skip);
        else
            forEveryNonEmptyBlockOnDisc(displayBloom, null, options.start, options.stop - options.start, 1); //options.skip);
    }

    return 0;
}

//-------------------------------------------------------------
bool displayBloom(CBlock& block, void *data)
{
#if 0
    SFString b = fromBloom(block.logsBloom);
    b.ReplaceAny("0"," ");
//    b.ReplaceAny("1248",".");
//    b.ReplaceAny("3569ac","-");
    b.ReplaceAny("1248","░");
    b.ReplaceAny("3569ac","▒");
    b.ReplaceAny("7bdef","▓");
    cout << block.blockNumber << " " << b << "\n";
#endif
    if (block.transactions.getCount()) {
        cout << block.blockNumber << "\r";
        cout.flush();
    }
    return true;
}


