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
#include "record.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CRecord, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextRecordChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextRecordChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CRecord::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["record_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextRecordChunk, this);
}

//---------------------------------------------------------------------------
string_q nextRecordChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CRecord*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CRecord::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextRecordChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                return addr_2_Str(address);
            }
            break;
        case 'c':
            if (fieldName % "cnt") {
                return uint_2_Str(cnt);
            }
            break;
        case 'd':
            if (fieldName % "date") {
                return date;
            }
            break;
        case 'g':
            if (fieldName % "grant_id") {
                return uint_2_Str(grant_id);
            }
            break;
        case 'h':
            if (fieldName % "has_data") {
                return bool_2_Str(has_data);
            }
            break;
        case 'k':
            if (fieldName % "key") {
                return uint_2_Str(key);
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 's':
            if (fieldName % "slug") {
                return slug;
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
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
bool CRecord::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                address = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'c':
            if (fieldName % "cnt") {
                cnt = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "date") {
                date = fieldValue;
                return true;
            }
            break;
        case 'g':
            if (fieldName % "grant_id") {
                grant_id = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'h':
            if (fieldName % "has_data") {
                has_data = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'k':
            if (fieldName % "key") {
                key = (uint32_t)str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "slug") {
                slug = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CRecord::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CRecord::Serialize(CArchive& archive) {
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
    archive >> date;
    archive >> type;
    archive >> grant_id;
    archive >> address;
    archive >> name;
    archive >> slug;
    archive >> cnt;
    archive >> has_data;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CRecord::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << key;
    archive << date;
    archive << type;
    archive << grant_id;
    archive << address;
    archive << name;
    archive << slug;
    archive << cnt;
    archive << has_data;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CRecordArray& array) {
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
CArchive& operator<<(CArchive& archive, const CRecordArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CRecord::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CRecord, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CRecord, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CRecord, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CRecord, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CRecord, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CRecord, "key", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CRecord, "date", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRecord, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRecord, "grant_id", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CRecord, "address", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRecord, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRecord, "slug", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CRecord, "cnt", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CRecord, "has_data", T_BOOL | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CRecord, "schema");
    HIDE_FIELD(CRecord, "deleted");
    HIDE_FIELD(CRecord, "showing");
    HIDE_FIELD(CRecord, "cname");

    builtIns.push_back(_biCRecord);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextRecordChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CRecord* rec = reinterpret_cast<const CRecord*>(dataPtr);
    if (rec) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'b':
                if (fieldIn % "bals") {
                    ostringstream os;
                    os << "[";
                    bool first = true;
                    for (auto b : rec->bals) {
                        if (!first)
                            os << ",\n";
                        os << b.second << endl;
                        first = false;
                    }
                    os << "]";
                    return os.str();
                }
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
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
bool CRecord::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CRecord& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_RECORD =
    "[{KEY}]\t"
    "[{DATE}]\t"
    "[{TYPE}]\t"
    "[{ADDRESS}]\t"
    "[{NAME}]\t"
    "[{CNT}]\t"
    "[{CSV}]\t"
    "[{JSON}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
