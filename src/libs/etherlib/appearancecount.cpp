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
#include "appearancecount.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAppearanceCount, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextAppearancecountChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAppearancecountChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAppearanceCount::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["appearancecount_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAppearancecountChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAppearancecountChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAppearanceCount*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAppearanceCount::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAppearancecountChunk_custom(fieldName, this);
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
        case 'f':
            if (fieldName % "fileSize") {
                return uint_2_Str(fileSize);
            }
            break;
        case 'n':
            if (fieldName % "nRecords") {
                return uint_2_Str(nRecords);
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
bool CAppearanceCount::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
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
        case 'f':
            if (fieldName % "fileSize") {
                fileSize = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "nRecords") {
                nRecords = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAppearanceCount::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAppearanceCount::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> address;
    archive >> nRecords;
    archive >> fileSize;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearanceCount::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << nRecords;
    archive << fileSize;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAppearanceCount::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CAppearanceCount copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearanceCountArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAppearanceCountArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAppearanceCount::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAppearanceCount, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAppearanceCount, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAppearanceCount, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearanceCount, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAppearanceCount, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAppearanceCount, "address", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAppearanceCount, "nRecords", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CAppearanceCount, "fileSize", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAppearanceCount, "schema");
    HIDE_FIELD(CAppearanceCount, "deleted");
    HIDE_FIELD(CAppearanceCount, "showing");
    HIDE_FIELD(CAppearanceCount, "cname");

    builtIns.push_back(_biCAppearanceCount);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAppearancecountChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAppearanceCount* app = reinterpret_cast<const CAppearanceCount*>(dataPtr);
    if (app) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, app);
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
bool CAppearanceCount::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAppearanceCount& app) {
    app.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAppearanceCount& app) {
    app.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAppearanceCount& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_APPEARANCECOUNT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
