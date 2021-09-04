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
#include "skin.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CSkin, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextSkinChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextSkinChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CSkin::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["skin_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextSkinChunk, this);
}

//---------------------------------------------------------------------------
string_q nextSkinChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CSkin*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CSkin::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextSkinChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bgPrimary") {
                return bgPrimary;
            }
            if (fieldName % "bgSecondary") {
                return bgSecondary;
            }
            if (fieldName % "borderPrimary") {
                return borderPrimary;
            }
            if (fieldName % "bgHover") {
                return bgHover;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            break;
        case 't':
            if (fieldName % "textPrimary") {
                return textPrimary;
            }
            if (fieldName % "textHover") {
                return textHover;
            }
            if (fieldName % "tableBgPrimary") {
                return tableBgPrimary;
            }
            if (fieldName % "tableBgSecondary") {
                return tableBgSecondary;
            }
            if (fieldName % "tableTextPrimary") {
                return tableTextPrimary;
            }
            if (fieldName % "tableBorderPrimary") {
                return tableBorderPrimary;
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
bool CSkin::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bgPrimary") {
                bgPrimary = fieldValue;
                return true;
            }
            if (fieldName % "bgSecondary") {
                bgSecondary = fieldValue;
                return true;
            }
            if (fieldName % "borderPrimary") {
                borderPrimary = fieldValue;
                return true;
            }
            if (fieldName % "bgHover") {
                bgHover = fieldValue;
                return true;
            }
            break;
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            break;
        case 't':
            if (fieldName % "textPrimary") {
                textPrimary = fieldValue;
                return true;
            }
            if (fieldName % "textHover") {
                textHover = fieldValue;
                return true;
            }
            if (fieldName % "tableBgPrimary") {
                tableBgPrimary = fieldValue;
                return true;
            }
            if (fieldName % "tableBgSecondary") {
                tableBgSecondary = fieldValue;
                return true;
            }
            if (fieldName % "tableTextPrimary") {
                tableTextPrimary = fieldValue;
                return true;
            }
            if (fieldName % "tableBorderPrimary") {
                tableBorderPrimary = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CSkin::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CSkin::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> name;
    archive >> bgPrimary;
    archive >> bgSecondary;
    archive >> textPrimary;
    archive >> borderPrimary;
    archive >> bgHover;
    archive >> textHover;
    archive >> tableBgPrimary;
    archive >> tableBgSecondary;
    archive >> tableTextPrimary;
    archive >> tableBorderPrimary;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSkin::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << bgPrimary;
    archive << bgSecondary;
    archive << textPrimary;
    archive << borderPrimary;
    archive << bgHover;
    archive << textHover;
    archive << tableBgPrimary;
    archive << tableBgSecondary;
    archive << tableTextPrimary;
    archive << tableBorderPrimary;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CSkin::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CSkin copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CSkinArray& array) {
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
CArchive& operator<<(CArchive& archive, const CSkinArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CSkin::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CSkin, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CSkin, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CSkin, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CSkin, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CSkin, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CSkin, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "bgPrimary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "bgSecondary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "textPrimary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "borderPrimary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "bgHover", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "textHover", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "tableBgPrimary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "tableBgSecondary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "tableTextPrimary", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CSkin, "tableBorderPrimary", T_TEXT | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CSkin, "schema");
    HIDE_FIELD(CSkin, "deleted");
    HIDE_FIELD(CSkin, "showing");
    HIDE_FIELD(CSkin, "cname");

    builtIns.push_back(_biCSkin);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextSkinChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CSkin* ski = reinterpret_cast<const CSkin*>(dataPtr);
    if (ski) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, ski);
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
bool CSkin::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CSkin& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_SKIN = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
