/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "bloom_blocks.h"

//-------------------------------------------------------------------------
IMPLEMENT_NODE(CBloomReceipt, CBaseNode);
CBloomReceipt::CBloomReceipt(void) {
    static bool been_here = false;
    if (!been_here) {
        ADD_FIELD(CBloomReceipt, "logsBloom", TS_STRING, 1);
        been_here = true;
    }
}
bool CBloomReceipt::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    if (fieldName == "logsBloom"   ) { logsBloom = fieldValue; return true; }
    return true;
}
string_q CBloomReceipt::getValueByName(const string_q& fieldName) const {
    if (fieldName == "logsBloom"   ) return logsBloom;
    return "";
}
void CBloomReceipt::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    ctx << toJson();
    return;
}

//-------------------------------------------------------------------------
IMPLEMENT_NODE(CBloomTrans, CBaseNode);
CBloomTrans::CBloomTrans(void) {
    static bool been_here = false;
    if (!been_here) {
        ADD_FIELD(CBloomTrans, "hash", TS_STRING, 1);
        ADD_FIELD(CBloomTrans, "receipt", T_OBJECT, 2);
        ADD_FIELD(CBloomTrans, "transactionIndex", TS_STRING, 3);
        been_here = true;
    }
}
bool CBloomTrans::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    if (fieldName == "transactionIndex"   ) { transactionIndex = str_2_Uint(fieldValue); return true; }
    if (fieldName == "hash"   ) { hash = fieldValue; return true; }
    return true;
}
string_q CBloomTrans::getValueByName(const string_q& fieldName) const {
    if (fieldName == "hash"   ) return hash;
    if (fieldName == "receipt") return receipt.Format();
    if (fieldName == "transactionIndex"   ) return uint_2_Str(transactionIndex);
    return "";
}
void CBloomTrans::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    ctx << toJson();
    return;
}
const CBaseNode *CBloomTrans::getObjectAt(const string_q& name, size_t i) const {
    if (name == "receipt")
        return &receipt;
    return NULL;
}

//-------------------------------------------------------------------------
IMPLEMENT_NODE(CBloomBlock, CBaseNode);
CBloomBlock::CBloomBlock(void) {
    static bool been_here = false;
    if (!been_here) {
        ADD_FIELD(CBloomBlock, "logsBloom", TS_STRING, 1);
        ADD_FIELD(CBloomBlock, "number", TS_STRING, 2);
        ADD_FIELD(CBloomBlock, "transactions", T_OBJECT|TS_ARRAY, 3);
        been_here = true;
    }
}
bool CBloomBlock::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    if (fieldName == "logsBloom"   ) { logsBloom    = fieldValue; return true; }
    if (fieldName == "number"      ) { number       = str_2_Uint(fieldValue); return true; }
    if (fieldName == "transactions") {
        string_q str = fieldValue;
        CBloomTrans item;
        while (item.parseJson3(str)) {
            string_q result;
            queryRawReceipt(result, item.hash);
            CRPCResult generic;
            generic.parseJson3(result);
            item.receipt.parseJson3(generic.result);
            transactions.push_back(item);
            item = CBloomTrans();  // reset
        }
        return true;
    }
    return true;
}
string_q CBloomBlock::getValueByName(const string_q& fieldName) const {
    if (fieldName == "logsBloom"   ) return logsBloom;
    if (fieldName == "number"      ) return uint_2_Str(number);
    if (fieldName == "transactionsCnt") return uint_2_Str(transactions.size());
    if (fieldName == "transactions") {
        if (GETRUNTIME_CLASS(CBloomBlock)->isFieldHidden("transactions"))
            return "";
        string_q ret;
        for (size_t i = 0 ; i < transactions.size(); i++)
            ret += transactions[i].Format();
        return ret;
    }
    return "";
}
void CBloomBlock::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    ctx << toJson();
    return;
}
const CBaseNode *CBloomBlock::getObjectAt(const string_q& name, size_t i) const {
    if (name == "transactions" && i < transactions.size())
        return &transactions[i];
    return NULL;
}
