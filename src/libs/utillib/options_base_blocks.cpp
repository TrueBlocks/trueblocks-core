/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "options_base.h"
#include "conversions.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    blknum_t COptionsBlockList::parseBlockOption(SFString& msg, blknum_t lastBlock) const {

        blknum_t ret = NOPOS;

        SFString arg = msg;
        msg = "";

        // if it's a number, return it
        if (isNumeral(arg) || isHexStr(arg)) {
            ret = toUnsigned(arg);

        } else {
            // if it's not a number, it better be a special value, and we better be able to find it
            CNameValue spec;
            if (pOptions && pOptions->findSpecial(spec, arg)) {
                if (spec.getName() == "latest") {
                    ret = lastBlock;
                } else {
                    ret = toUnsigned(spec.getValue());
                }
            } else {
                msg = "The given value '" + arg + "' is not a numeral or a special named block. Quitting...\n";
                return NOPOS;
            }
        }

        if (ret > lastBlock) {
            SFString lateStr = (isTestMode() ? "--" : asStringU(lastBlock));
            msg = "Block " + arg + " is later than the last valid block " + lateStr + ". Quitting...\n";
            return NOPOS;
        }

        return ret;
    }

    //--------------------------------------------------------------------------------
    SFString COptionsBlockList::parseBlockList(const SFString& argIn, blknum_t lastBlock) {
        SFString arg = argIn;
        if (arg.Contains("-")) {

            // If we already have a range, bail
            if (start != stop)
                return "Specify only a single block range at a time.";

            SFString stp = arg;
            SFString strt = nextTokenClear(stp, '-');
            if (strt == "latest")
                return "Cannot start range with 'latest'";

            SFString msg = strt;
            start = parseBlockOption(msg, lastBlock);
            if (!msg.empty())
                return msg;
            msg = stp;
            stop = parseBlockOption(msg, lastBlock);
            if (!msg.empty())
                return msg;

            if (stop <= start)
                return "'stop' must be strictly larger than 'start'";

        } else {

            if (isHash(arg)) {
                hashList[hashList.getCount()] = arg;

            } else {
                SFString msg = arg;
                blknum_t num = parseBlockOption(msg, lastBlock);
                if (!msg.empty())
                    return msg;
                numList[numList.getCount()] = num;
            }
        }

        latest = lastBlock;
        return "";
    }

    //--------------------------------------------------------------------------------
    void COptionsBlockList::Init(void) {
        numList.Clear();
        hashList.Clear();
        start = stop = 0;
        hashFind = NULL;
    }

    //--------------------------------------------------------------------------------
    COptionsBlockList::COptionsBlockList(void) {
        Init();
    }

    //--------------------------------------------------------------------------------
    bool COptionsBlockList::forEveryBlockNumber(UINT64VISITFUNC func, void *data) const {
        if (!func)
            return false;

        for (uint64_t i = start ; i < stop ; i++) {
            if (!(*func)(i, data))
                return false;
        }
        for (uint32_t i = 0 ; i < numList.getCount() ; i++) {
            uint64_t n = numList[i];
            if (!(*func)(n, data))
                return false;
        }
        if (hashFind) {
            for (uint32_t i = 0 ; i < hashList.getCount() ; i++) {
                uint64_t n = (*hashFind)(hashList[i], data);
                if (!(*func)(n, data))
                    return false;
            }
        }
        return true;
    }

    //--------------------------------------------------------------------------------
    bool COptionsBlockList::isInRange(blknum_t bn) const {
        if (start <= bn && bn < stop)
            return true;
        for (uint32_t i = 0 ; i < numList.getCount() ; i++)
            if (bn == numList[i])
                return true;
        return false;
    }

}  // namespace qblocks
