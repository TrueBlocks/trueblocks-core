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
#include "pricecacheitem.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPriceCacheItem, CAccountName);

//---------------------------------------------------------------------------
static string_q nextPricecacheitemChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextPricecacheitemChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CPriceCacheItem::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pricecacheitem_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPricecacheitemChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPricecacheitemChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPriceCacheItem *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CPriceCacheItem::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CAccountName::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'p':
            if ( fieldName % "pair" ) { pair = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "type" ) { type = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPriceCacheItem::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPriceCacheItem::Serialize(CArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CAccountName::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> type;
    archive >> pair;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPriceCacheItem::SerializeC(CArchive& archive) const {

    // Writing always write the latest version of the data
    CAccountName::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << type;
    archive << pair;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPriceCacheItemArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPriceCacheItemArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPriceCacheItem::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPriceCacheItem, "schema")) return;

    CAccountName::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CPriceCacheItem, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CPriceCacheItem, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CPriceCacheItem, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CPriceCacheItem, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CPriceCacheItem, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CPriceCacheItem, "pair", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPriceCacheItem, "schema");
    HIDE_FIELD(CPriceCacheItem, "deleted");
    HIDE_FIELD(CPriceCacheItem, "showing");
    HIDE_FIELD(CPriceCacheItem, "cname");

    builtIns.push_back(_biCPriceCacheItem);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPricecacheitemChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CPriceCacheItem *pri = reinterpret_cast<const CPriceCacheItem *>(dataPtr);
    if (pri) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, pri);
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
bool CPriceCacheItem::readBackLevel(CArchive& archive) {

    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPriceCacheItem& pri) {
    pri.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPriceCacheItem& pri) {
    pri.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CPriceCacheItem::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextPricecacheitemChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'p':
            if ( fieldName % "pair" ) return pair;
            break;
        case 't':
            if ( fieldName % "type" ) return type;
            break;
    }

    // EXISTING_CODE
    if ( fieldName % "firstAppearance" || fieldName % "latestAppearance" )
        return "";
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CAccountName::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPriceCacheItem& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks

