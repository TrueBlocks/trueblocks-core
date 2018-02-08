/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
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
void [{PREFIX}]_init(void)
{
    [{BLKPATH}]
[{REGISTERS}][{CHAINLIB}]
}

//-----------------------------------------------------------------------
[{FUNC_DECLS}]

//-----------------------------------------------------------------------
const CTransaction *promoteTo[{PPREFIX}](const CTransaction *p)
{
    if (p && (p->input.length()>=10 || p->input == "0x"))
    {
[{INIT_CODE}]        // EXISTING_CODE
        // EXISTING_CODE

[{FACTORY1}]
        }
        // falls through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // returns NULL if not promoted
    return promoteToToken(p);
}

//-----------------------------------------------------------------------
[{EVENT_DECLS}]

//-----------------------------------------------------------------------
const CLogEntry *promoteTo[{PPREFIX}]Event(const CLogEntry *p)
{
    if (!p)
        return NULL;

    uint32_t nTops = p->topics.getCount();
    if (nTops>0) // the '0'th topic is the event signature
    {
        SFString data = p->data.substr(2);
        // EXISTING_CODE
        // EXISTING_CODE

[{FACTORY2}]
        }
        // fall through
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // returns NULL if not promoted
    return promoteToTokenEvent(p);
}
[{CODE_SIGS}]
/*
 [{ABI}]*/
