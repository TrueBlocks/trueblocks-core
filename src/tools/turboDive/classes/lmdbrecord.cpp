#if 0
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
#include "lmdbrecord.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLmdbRecord, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextLmdbrecordChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextLmdbrecordChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CLmdbRecord::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["lmdbrecord_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextLmdbrecordChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLmdbrecordChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CLmdbRecord*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CLmdbRecord::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextLmdbrecordChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "key") {
                if (key == CLmdbVal())
                    return "{}";
                return key.Format();
            }
            break;
        case 'v':
            if (fieldName % "value") {
                if (value == CLmdbVal())
                    return "{}";
                return value.Format();
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    string_q s;
    s = toUpper(string_q("key")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = key.getValueByName(f);
        return f;
    }

    s = toUpper(string_q("value")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = value.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CLmdbRecord::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'k':
            if (fieldName % "key") {
                return key.parseJson3(fieldValue);
            }
            break;
        case 'v':
            if (fieldName % "value") {
                return value.parseJson3(fieldValue);
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CLmdbRecord::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLmdbRecord::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> key;
    archive >> value;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLmdbRecord::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << key;
    archive << value;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLmdbRecordArray& array) {
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
CArchive& operator<<(CArchive& archive, const CLmdbRecordArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLmdbRecord::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CLmdbRecord, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CLmdbRecord, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CLmdbRecord, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CLmdbRecord, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CLmdbRecord, "cname", T_TEXT, ++fieldNum);
    ADD_OBJECT(CLmdbRecord, "key", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CLmdbVal));
    ADD_OBJECT(CLmdbRecord, "value", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CLmdbVal));

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLmdbRecord, "schema");
    HIDE_FIELD(CLmdbRecord, "deleted");
    HIDE_FIELD(CLmdbRecord, "showing");
    HIDE_FIELD(CLmdbRecord, "cname");

    builtIns.push_back(_biCLmdbRecord);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLmdbrecordChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CLmdbRecord* lmd = reinterpret_cast<const CLmdbRecord*>(dataPtr);
    if (lmd) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, lmd);
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
bool CLmdbRecord::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CLmdbRecord& lmd) {
    lmd.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLmdbRecord& lmd) {
    lmd.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLmdbRecord& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CLmdbRecord::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "key")
        return &key;

    if (fieldName % "value")
        return &value;

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_LMDBRECORD =
    "[{KEY}]\t"
    "[{VALUE}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
CLmdbRecord::CLmdbRecord(const MDB_val& k, const MDB_val& v) {
    key.size = k.mv_size;
    key.data = (const_void_ptr)k.mv_data;
    value.size = v.mv_size;
    value.data = (const_void_ptr)v.mv_data;
}
// EXISTING_CODE
}  // namespace qblocks
#endif
