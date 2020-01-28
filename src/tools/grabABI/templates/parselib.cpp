/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This code was generated automatically from grabABI and makeClass. You may
 * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.
 */
[{HEADERS}]
    //-----------------------------------------------------------------------
    void[{PREFIX}] _init(QUITHANDLER qh){[{BLKPATH}][{REGISTERS}][{CHAINLIB}]}

        //-----------------------------------------------------------------------
        [{FUNC_DECLS}]

    //-----------------------------------------------------------------------
    const CTransaction* promoteTo[{PPREFIX}](const CTransaction* p) {
    if (p && (p->input.length() >= 10 || p->input == "0x")) {
        [{INIT_CODE}]  // EXISTING_CODE
                       // EXISTING_CODE

        [{FACTORY1}]
    }
    // falls through
}

// EXISTING_CODE
// EXISTING_CODE

// never returns NULL
return promoteToToken(p);
}

//-----------------------------------------------------------------------
[{EVENT_DECLS}]

    //-----------------------------------------------------------------------
    const CLogEntry* promoteTo[{PPREFIX}] Event(const CLogEntry* p) {
    size_t nTops = p->topics.size();
    if (nTops > 0) {    // the '0'th topic is the event signature
        [{INIT_CODE2}]  // EXISTING_CODE
                        // EXISTING_CODE
        [{FACTORY2}]
    }
    // fall through
}

// EXISTING_CODE
// EXISTING_CODE

// never returns NULL
return promoteToTokenEvent(p);
}
// EXISTING_CODE
// EXISTING_CODE

[{CODE_SIGS}]
    /*
     [{ABI}]*/
