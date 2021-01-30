/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include "pinneditem.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinnedItem, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPinneditemChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinneditemChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinnedItem::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinneditem_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextPinneditemChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinneditemChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinnedItem*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinnedItem::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinneditemChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomHash") {
                return bloomHash;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                return fileName;
            }
            break;
        case 'i':
            if (fieldName % "indexHash") {
                return indexHash;
            }
            break;
        case 'o':
            if (fieldName % "onDisc") {
                return bool_2_Str(onDisc);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CPinnedItem::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomHash") {
                bloomHash = fieldValue;
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                fileName = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "indexHash") {
                indexHash = fieldValue;
                return true;
            }
            break;
        case 'o':
            if (fieldName % "onDisc") {
                onDisc = str_2_Bool(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPinnedItem::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinnedItem::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> fileName;
    archive >> bloomHash;
    archive >> indexHash;
    // archive >> onDisc;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinnedItem::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << fileName;
    archive << bloomHash;
    archive << indexHash;
    // archive << onDisc;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinnedItemArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinnedItemArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinnedItem::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinnedItem, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinnedItem, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinnedItem, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinnedItem, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinnedItem, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinnedItem, "fileName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinnedItem, "bloomHash", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinnedItem, "indexHash", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinnedItem, "onDisc", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CPinnedItem, "onDisc");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinnedItem, "schema");
    HIDE_FIELD(CPinnedItem, "deleted");
    HIDE_FIELD(CPinnedItem, "showing");
    HIDE_FIELD(CPinnedItem, "cname");

    builtIns.push_back(_biCPinnedItem);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinneditemChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinnedItem* pin = reinterpret_cast<const CPinnedItem*>(dataPtr);
    if (pin) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, pin);
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
bool CPinnedItem::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinnedItem& pin) {
    pin.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinnedItem& pin) {
    pin.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinnedItem& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINNEDITEM =
    "[{FILENAME}]\t"
    "[{BLOOMHASH}]\t"
    "[{INDEXHASH}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
