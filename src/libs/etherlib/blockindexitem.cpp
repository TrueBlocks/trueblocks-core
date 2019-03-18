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
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include <algorithm>
#include "blockindexitem.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBlockIndexItem, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextBlockindexitemChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextBlockindexitemChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CBlockIndexItem::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["blockindexitem_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBlockindexitemChunk, this);
}

//---------------------------------------------------------------------------
string_q nextBlockindexitemChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CBlockIndexItem *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CBlockIndexItem::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "bn" ) { bn = (uint32_t)str_2_Uint(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "cnt" ) { cnt = (uint32_t)str_2_Uint(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "ts" ) { ts = (uint32_t)str_2_Uint(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBlockIndexItem::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBlockIndexItem::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> bn;
    archive >> ts;
    archive >> cnt;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBlockIndexItem::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << bn;
    archive << ts;
    archive << cnt;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBlockIndexItemArray& array) {
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
CArchive& operator<<(CArchive& archive, const CBlockIndexItemArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CBlockIndexItem::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CBlockIndexItem, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CBlockIndexItem, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlockIndexItem, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlockIndexItem, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CBlockIndexItem, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CBlockIndexItem, "bn", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlockIndexItem, "ts", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBlockIndexItem, "cnt", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBlockIndexItem, "schema");
    HIDE_FIELD(CBlockIndexItem, "deleted");
    HIDE_FIELD(CBlockIndexItem, "showing");
    HIDE_FIELD(CBlockIndexItem, "cname");

    builtIns.push_back(_biCBlockIndexItem);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBlockindexitemChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CBlockIndexItem *blo = reinterpret_cast<const CBlockIndexItem *>(dataPtr);
    if (blo) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, blo);
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
bool CBlockIndexItem::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CBlockIndexItem::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextBlockindexitemChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "bn" ) return uint_2_Str(bn);
            break;
        case 'c':
            if ( fieldName % "cnt" ) return uint_2_Str(cnt);
            break;
        case 't':
            if ( fieldName % "ts" ) return uint_2_Str(ts);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBlockIndexItem& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

