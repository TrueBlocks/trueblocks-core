/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include <algorithm>
#include "receipt.h"
#include "etherlib.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CReceipt, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextReceiptChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextReceiptChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CReceipt::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextReceiptChunk, this);
}

//---------------------------------------------------------------------------
string_q nextReceiptChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CReceipt *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CReceipt::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    if (fieldName == "contractAddress" && fieldValue == "null") {
        *((string_q*)&fieldValue) = "0";  // NOLINT
    } else if (fieldName == "status" && (fieldValue == "null" || fieldValue == "0x")) {
        *((string_q*)&fieldValue) = toStringU(NO_STATUS);  // NOLINT
    }

    if (pTrans)
        if (((CTransaction*)pTrans)->setValueByName(fieldName, fieldValue))  // NOLINT
            return true;
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "contractAddress" ) { contractAddress = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasUsed" ) { gasUsed = str_2_Gas(fieldValue); return true; }
            break;
        case 'l':
            if ( fieldName % "logs" ) {
                char *p = (char *)fieldValue.c_str();  // NOLINT
                while (p && *p) {
                    CLogEntry item;
                    size_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        logs.push_back(item);
                }
                return true;
            }
            break;
        case 's':
            if ( fieldName % "status" ) { status = (uint32_t)str_2_Uint(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CReceipt::finishParse() {
    // EXISTING_CODE
    for (size_t i = 0 ; i < logs.size() ; i++) {
        logs.at(i).pReceipt = this;  // taking a non-const reference of an element that already exists
    }
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CReceipt::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> contractAddress;
    archive >> gasUsed;
    archive >> logs;
    archive >> status;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CReceipt::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << contractAddress;
    archive << gasUsed;
    archive << logs;
    archive << status;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CReceiptArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CReceiptArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CReceipt::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CReceipt, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CReceipt, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CReceipt, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CReceipt, "contractAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CReceipt, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CReceipt, "logs", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CReceipt, "status", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CReceipt, "schema");
    HIDE_FIELD(CReceipt, "deleted");
    HIDE_FIELD(CReceipt, "showing");

    builtIns.push_back(_biCReceipt);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextReceiptChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CReceipt *rec = (const CReceipt *)dataPtr;  // NOLINT
    if (rec) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 's':
                if ( fieldIn % "status" ) {
                    if (rec->status == NO_STATUS) {
                        return "null";
                    } else if (rec->pTrans &&
                               rec->pTrans->pBlock &&
                               rec->pTrans->pBlock->blockNumber < byzantiumBlock) {
                        return "null";
                    }
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, rec);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CReceipt::readBackLevel(SFArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    SFBloom removed;
    if (m_schema < getVersionNum(0, 2, 0)) {
        archive >> contractAddress;
        archive >> gasUsed;
        archive >> logs;
        archive >> removed;  // was logsBloom
        // The 'status' field will be corrected in CBlock::finishParse() once we have a block
        // number. We set status here to NO_STATUS assuming pre-byzantium. After byzantium, we
        // have to pick up the value (0 or 1) from the node
        status = NO_STATUS;
        finishParse();
        done = true;

    } else if (m_schema <= getVersionNum(0, 3, 0)) {
        archive >> contractAddress;
        archive >> gasUsed;
        archive >> logs;
        archive >> removed;  // was logsBloom
        archive >> status;
        finishParse();
        done = true;

    }
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CReceipt& rec) {
    rec.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CReceipt& rec) {
    rec.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CReceipt::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextReceiptChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "contractAddress" ) return fromAddress(contractAddress);
            break;
        case 'g':
            if ( fieldName % "gasUsed" ) return fromGas(gasUsed);
            break;
        case 'l':
            if ( fieldName % "logs" || fieldName % "logsCnt" ) {
                size_t cnt = logs.size();
                if (endsWith(fieldName, "Cnt"))
                    return toStringU(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += logs[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 's':
            if ( fieldName % "status" ) return toStringU(status);
            break;
    }

    // EXISTING_CODE
    // See if this field belongs to the item's container
    ret = nextTransactionChunk(fieldName, pTrans);
    if (contains(ret, "Field not found"))
        ret = "";
    if (!ret.empty())
        return ret;
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CReceipt& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CReceipt::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "logs" && index < logs.size() )
        return &logs[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

