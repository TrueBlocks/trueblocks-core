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
#include "page.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPage, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPageChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPageChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPage::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["page_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextPageChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPageChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPage*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPage::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPageChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cmdUrl") {
                return cmdUrl;
            }
            break;
        case 'd':
            if (fieldName % "dest_path") {
                return dest_path;
            }
            if (fieldName % "defaultTable") {
                return defaultTable;
            }
            if (fieldName % "defaultSearch") {
                return defaultSearch;
            }
            if (fieldName % "defaultSort") {
                return defaultSort;
            }
            if (fieldName % "defaultTags") {
                return defaultTags;
            }
            if (fieldName % "dataUrl") {
                return dataUrl;
            }
            if (fieldName % "dataQuery") {
                return dataQuery;
            }
            break;
        case 'i':
            if (fieldName % "imports") {
                return imports;
            }
            break;
        case 'l':
            if (fieldName % "longName") {
                return longName;
            }
            break;
        case 'n':
            if (fieldName % "noPage") {
                return bool_2_Str(noPage);
            }
            break;
        case 'p':
            if (fieldName % "properName") {
                return properName;
            }
            break;
        case 'r':
            if (fieldName % "recordIcons") {
                return recordIcons;
            }
            break;
        case 's':
            if (fieldName % "sevenName") {
                return sevenName;
            }
            if (fieldName % "singular") {
                return singular;
            }
            if (fieldName % "schema") {
                return schema;
            }
            if (fieldName % "subpages" || fieldName % "subpagesCnt") {
                size_t cnt = subpages.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += subpages[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 't':
            if (fieldName % "twoName") {
                return twoName;
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
bool CPage::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cmdUrl") {
                cmdUrl = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "dest_path") {
                dest_path = fieldValue;
                return true;
            }
            if (fieldName % "defaultTable") {
                defaultTable = fieldValue;
                return true;
            }
            if (fieldName % "defaultSearch") {
                defaultSearch = fieldValue;
                return true;
            }
            if (fieldName % "defaultSort") {
                defaultSort = fieldValue;
                return true;
            }
            if (fieldName % "defaultTags") {
                defaultTags = fieldValue;
                return true;
            }
            if (fieldName % "dataUrl") {
                dataUrl = fieldValue;
                return true;
            }
            if (fieldName % "dataQuery") {
                dataQuery = fieldValue;
                return true;
            }
            break;
        case 'i':
            if (fieldName % "imports") {
                imports = fieldValue;
                return true;
            }
            break;
        case 'l':
            if (fieldName % "longName") {
                longName = fieldValue;
                return true;
            }
            break;
        case 'n':
            if (fieldName % "noPage") {
                noPage = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "properName") {
                properName = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "recordIcons") {
                recordIcons = fieldValue;
                return true;
            }
            break;
        case 's':
            if (fieldName % "sevenName") {
                sevenName = fieldValue;
                return true;
            }
            if (fieldName % "singular") {
                singular = fieldValue;
                return true;
            }
            if (fieldName % "schema") {
                schema = fieldValue;
                return true;
            }
            if (fieldName % "subpages") {
                CSubpage obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    subpages.push_back(obj);
                    obj = CSubpage();  // reset
                }
                return true;
            }
            break;
        case 't':
            if (fieldName % "twoName") {
                twoName = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPage::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPage::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> longName;
    archive >> properName;
    archive >> twoName;
    archive >> sevenName;
    archive >> singular;
    archive >> noPage;
    archive >> dest_path;
    archive >> recordIcons;
    archive >> imports;
    archive >> defaultTable;
    archive >> defaultSearch;
    archive >> defaultSort;
    archive >> defaultTags;
    archive >> dataUrl;
    archive >> dataQuery;
    archive >> cmdUrl;
    archive >> schema;
    archive >> subpages;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPage::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << longName;
    archive << properName;
    archive << twoName;
    archive << sevenName;
    archive << singular;
    archive << noPage;
    archive << dest_path;
    archive << recordIcons;
    archive << imports;
    archive << defaultTable;
    archive << defaultSearch;
    archive << defaultSort;
    archive << defaultTags;
    archive << dataUrl;
    archive << dataQuery;
    archive << cmdUrl;
    archive << schema;
    archive << subpages;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPage::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CPage copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPageArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPageArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPage::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPage, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPage, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPage, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPage, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPage, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPage, "longName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "properName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "twoName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "sevenName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "singular", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "noPage", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "dest_path", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "recordIcons", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "imports", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "defaultTable", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "defaultSearch", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "defaultSort", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "defaultTags", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "dataUrl", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "dataQuery", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "cmdUrl", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "schema", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPage, "subpages", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPage, "schema");
    HIDE_FIELD(CPage, "deleted");
    HIDE_FIELD(CPage, "showing");
    HIDE_FIELD(CPage, "cname");

    builtIns.push_back(_biCPage);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPageChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPage* pag = reinterpret_cast<const CPage*>(dataPtr);
    if (pag) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, pag);
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
bool CPage::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPage& pag) {
    pag.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPage& pag) {
    pag.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPage& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CPage::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "subpages") {
        if (index == NOPOS) {
            CSubpage empty;
            ((CPage*)this)->subpages.push_back(empty);  // NOLINT
            index = subpages.size() - 1;
        }
        if (index < subpages.size())
            return &subpages[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PAGE = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
