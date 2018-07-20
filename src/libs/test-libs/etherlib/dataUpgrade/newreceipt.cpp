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
#include "newreceipt.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNewReceipt, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextNewreceiptChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextNewreceiptChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CNewReceipt::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextNewreceiptChunk, this);
}

//---------------------------------------------------------------------------
string_q nextNewreceiptChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CNewReceipt *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if ( fieldName % "contractAddress" ) { contractAddress = toAddress(fieldValue); return true; }
            break;
        case 'g':
            if ( fieldName % "gasUsed" ) { gasUsed = toGas(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "isError" ) { isError = str2Bool(fieldValue); return true; }
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
            if ( fieldName % "logsBloom" ) { logsBloom = toBloom(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CNewReceipt::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::Serialize(SFArchive& archive) {

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
    archive >> logsBloom;
    archive >> isError;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    ((CNewReceipt*)this)->m_schema = getVersionNum();  // NOLINT
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << contractAddress;
    archive << gasUsed;
    archive << logs;
    archive << logsBloom;
    archive << isError;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CNewReceiptArray& array) {
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
SFArchive& operator<<(SFArchive& archive, const CNewReceiptArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CNewReceipt::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CNewReceipt, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewReceipt, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewReceipt, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CNewReceipt, "contractAddress", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CNewReceipt, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CNewReceipt, "logs", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CNewReceipt, "logsBloom", T_BLOOM, ++fieldNum);
    ADD_FIELD(CNewReceipt, "isError", T_BOOL, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNewReceipt, "schema");
    HIDE_FIELD(CNewReceipt, "deleted");
    HIDE_FIELD(CNewReceipt, "showing");

    builtIns.push_back(_biCNewReceipt);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextNewreceiptChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CNewReceipt *newp = (const CNewReceipt *)dataPtr;  // NOLINT
    if (newp) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, newp);
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
bool CNewReceipt::readBackLevel(SFArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CNewReceipt& newp) {
    newp.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CNewReceipt& newp) {
    newp.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CNewReceipt::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextNewreceiptChunk_custom(fieldName, this);
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
        case 'i':
            if ( fieldName % "isError" ) return toString(isError);
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
            if ( fieldName % "logsBloom" ) return bloom2Bytes(logsBloom);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CNewReceipt& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CNewReceipt::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "logs" && index < logs.size() )
        return &logs[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

