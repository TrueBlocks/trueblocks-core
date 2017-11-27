/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "options_base.h"
#include "database.h"
#include "sfstring.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    void COptionsTransList::Init(void) {
        queries = "";
    }

    //--------------------------------------------------------------------------------
    SFString COptionsTransList::parseTransList(const SFString& argIn) {

        SFString arg = argIn;
        if (arg.startsWith("0x")) {

            if (arg.Contains(".")) {

                SFString hash = nextTokenClear(arg, '.');
                if (!isHash(hash) || !isNumeral(arg))
                    return "The argument '" + argIn + "' is not properly formatted.";
                queries += (hash + "." + arg + "|");  // blockHash.transID

            } else if (isHash(arg)) {
                queries += (arg + "|");  // transHash

            } else {
                SFString ret = "The argument '" + argIn + "' is not properly formatted.";
                if (arg.length() != 66)
                    ret += " Is it the right length?";
                return ret;
            }

        } else if (arg.Contains(".")) {
            SFString blockNum = nextTokenClear(arg, '.');
            if (!isNumeral(blockNum) || !isNumeral(arg))
                return "The argument '" + argIn + "' is not properly formatted.";
            queries += (argIn + "|");  // blockNum.transID

        } else {
            return "The argument '" + argIn + "' is not properly formatted.";

        }

        return "";
    }

    //--------------------------------------------------------------------------------
    COptionsTransList::COptionsTransList(void) {
        Init();
    }

    //--------------------------------------------------------------------------------
    SFString COptionsTransList::toString(void) const {
        return queries;
    }

}  // namespace qblocks
