/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "options_base.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    SFString COptionsBlockList::parseBlockList(const SFString& argIn, blknum_t lastBlock) {
        SFString arg = argIn;
        if (arg.Contains("-")) {

            // If we already have a range, bail
            if (start != stop)
                return "Specify only a single block range at a time.";

            SFString arg1 = nextTokenClear(arg, '-');
            if (arg1 == "latest")
                return "Cannot start range with 'latest'";

            start = toUnsigned(arg1);
            stop  = toUnsigned(arg);
            if (arg == "latest")
                stop = lastBlock;
            if (stop <= start)
                return "'stop' must be strictly larger than 'start'";
            if (stop - start + 1 > MAX_BLOCK_RANGE)
                return "The range you specified (" + argIn + ") is too broad. Ranges may be at "
                        "most " + asStringU(MAX_BLOCK_RANGE) + " blocks long. Quitting...";

        } else {

            blknum_t num = toUnsigned(arg);
            CNameValue spec;
            if (pOptions && pOptions->findSpecial(spec, arg)) {
                if (spec.getName() == "latest") {
                    num = lastBlock;
                } else {
                    num = toUnsigned(spec.getValue());
                }
            }

            if (num == 0 && arg != "first" && !arg.startsWith("0") && !isNumeral(arg))
                return "'" + arg + "' does not appear to be a valid block. Quitting...";

            if (num > lastBlock) {
                SFString lateStr = (isTestMode() ? "--" : asStringU(lastBlock));
                return "Block " + arg + " is later than the last valid block " + lateStr + ". Quitting...\n";
            }

            if (nNums >= MAX_BLOCK_LIST)
                return "Too many blocks in list. Max is " + asString(MAX_BLOCK_LIST);

            nums[nNums++] = num;
        }
        latest = lastBlock;
        return "";
    }

    //--------------------------------------------------------------------------------
    void COptionsBlockList::Init(void) {
        nums[0]    = NOPOS;
        nNums      = 0;  // we will set this to '1' later if user supplies no values
        start = stop = 0;
    }

    //--------------------------------------------------------------------------------
    COptionsBlockList::COptionsBlockList(void) {
        Init();
    }

    //--------------------------------------------------------------------------------
    SFString COptionsBlockList::toString(void) const {
        SFString ret;
        for (SFUint32 i = start ; i < stop ; i++)
            ret += (asStringU(i) + "|");
        for (SFUint32 i = 0 ; i < nNums ; i++)
            ret += (asStringU(nums[i]) + "|");
        return ret;
    }

}  // namespace qblocks
