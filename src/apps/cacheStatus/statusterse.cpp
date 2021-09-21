/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "statusterse.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CStatusTerse, CStatus);

//---------------------------------------------------------------------------
extern string_q nextStatusterseChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextStatusterseChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CStatusTerse::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["statusterse_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextStatusterseChunk, this);
}

//---------------------------------------------------------------------------
string_q nextStatusterseChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CStatusTerse*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CStatusTerse::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextStatusterseChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'm':
            if (fieldName % "modes1") {
                return modes1;
            }
            if (fieldName % "modes2") {
                return modes2;
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CStatus::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CStatusTerse::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CStatus::setValueByName(fieldName, fieldValue))
        return true;
    switch (tolower(fieldName[0])) {
        case 'm':
            if (fieldName % "modes1") {
                modes1 = fieldValue;
                return true;
            }
            if (fieldName % "modes2") {
                modes2 = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CStatusTerse::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CStatusTerse::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CStatus::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> modes1;
    archive >> modes2;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CStatusTerse::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CStatus::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << modes1;
    archive << modes2;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CStatusTerse::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CStatusTerse copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CStatusTerseArray& array) {
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
CArchive& operator<<(CArchive& archive, const CStatusTerseArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CStatusTerse::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CStatusTerse, "schema"))
        return;

    CStatus::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CStatusTerse, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CStatusTerse, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CStatusTerse, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CStatusTerse, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CStatusTerse, "modes1", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CStatusTerse, "modes2", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CStatusTerse, "schema");
    HIDE_FIELD(CStatusTerse, "deleted");
    HIDE_FIELD(CStatusTerse, "showing");
    HIDE_FIELD(CStatusTerse, "cname");

    builtIns.push_back(_biCStatusTerse);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextStatusterseChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CStatusTerse* sta = reinterpret_cast<const CStatusTerse*>(dataPtr);
    if (sta) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, sta);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CStatusTerse::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CStatusTerse& sta) {
    sta.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CStatusTerse& sta) {
    sta.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CStatusTerse& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_STATUSTERSE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
CStatusTerse::CStatusTerse(const CStatus& it) {
    client_version = it.client_version;
    trueblocks_version = it.trueblocks_version;
    is_testing = it.is_testing;
    is_archive = it.is_archive;
    is_tracing = it.is_tracing;
    is_docker = it.is_docker;
    has_eskey = it.has_eskey;
    has_pinkey = it.has_pinkey;
    config_path = it.config_path;
    cache_path = it.cache_path;
    index_path = it.index_path;
    rpc_provider = it.rpc_provider;
    modes1 += string_q(it.is_testing ? "testing|" : "");
    modes1 += string_q(it.is_archive ? "" : "not ") + "archive|";
    modes1 += string_q(it.is_tracing ? "" : "not ") + "tracing|";
    modes1 += string_q(it.is_docker ? "docker|" : "");
    modes1 = (modes1.empty() ? "" : " (" + substitute(trim(modes1, '|'), "|", ", ") + ")");
    modes2 += string_q(it.has_eskey ? "" : "no ") + "eskey|";
    modes2 += string_q(it.has_pinkey ? "" : "no ") + "pinkey|";
    modes2 = (modes2.empty() ? "" : " (" + substitute(trim(modes2, '|'), "|", ", ") + ")");
}
// EXISTING_CODE
}  // namespace qblocks
