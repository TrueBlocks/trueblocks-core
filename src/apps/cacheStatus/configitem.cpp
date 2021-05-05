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
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "configitem.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CConfigItem, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextConfigitemChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextConfigitemChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CConfigItem::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    /* We need to return a boolean if the type is boolean, so we do this crazy shit */
    CRuntimeClass* pClass = getRuntimeClass();
    CFieldData* fld = pClass->findField("value");
    fld->setType(T_TEXT);  // always reset it since an item might have children
    if (pClass && fld && type == "bool")
        fld->setType(T_BOOL);
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["configitem_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    if (pClass && fld && type == "bool")
        fld->setType(T_TEXT);
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextConfigitemChunk, this);
}

//---------------------------------------------------------------------------
string_q nextConfigitemChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CConfigItem*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CConfigItem::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextConfigitemChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'n':
            if (fieldName % "name") {
                return name;
            }
            if (fieldName % "named" || fieldName % "namedCnt") {
                size_t cnt = named.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += named[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'r':
            if (fieldName % "required") {
                return bool_2_Str(required);
            }
            if (fieldName % "read_only") {
                return bool_2_Str(read_only);
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
            }
            if (fieldName % "tip") {
                return tip;
            }
            break;
        case 'v':
            if (fieldName % "value") {
                return value;
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
bool CConfigItem::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'n':
            if (fieldName % "name") {
                name = fieldValue;
                return true;
            }
            if (fieldName % "named") {
                CAccountName obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    named.push_back(obj);
                    obj = CAccountName();  // reset
                }
                return true;
            }
            break;
        case 'r':
            if (fieldName % "required") {
                required = str_2_Bool(fieldValue);
                return true;
            }
            if (fieldName % "read_only") {
                read_only = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            if (fieldName % "tip") {
                tip = fieldValue;
                return true;
            }
            break;
        case 'v':
            if (fieldName % "value") {
                value = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CConfigItem::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CConfigItem::Serialize(CArchive& archive) {
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
    archive >> value;
    archive >> type;
    archive >> tip;
    archive >> required;
    archive >> read_only;
    // archive >> named;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CConfigItem::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << name;
    archive << value;
    archive << type;
    archive << tip;
    archive << required;
    archive << read_only;
    // archive << named;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CConfigItemArray& array) {
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
CArchive& operator<<(CArchive& archive, const CConfigItemArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CConfigItem::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CConfigItem, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CConfigItem, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CConfigItem, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigItem, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CConfigItem, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CConfigItem, "name", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigItem, "value", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigItem, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigItem, "tip", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigItem, "required", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigItem, "read_only", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CConfigItem, "named", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CConfigItem, "named");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CConfigItem, "schema");
    HIDE_FIELD(CConfigItem, "deleted");
    HIDE_FIELD(CConfigItem, "showing");
    HIDE_FIELD(CConfigItem, "cname");

    builtIns.push_back(_biCConfigItem);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextConfigitemChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CConfigItem* con = reinterpret_cast<const CConfigItem*>(dataPtr);
    if (con) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'v':
                if (fieldIn % "value" && con->named.size() > 0) {
                    bool first = true;
                    manageFields("CAccountName:firstAppearance,latestAppearance,nAppearances,sizeInBytes", false);
                    ostringstream os;
                    for (auto name : con->named) {
                        if (!first)
                            os << ", ";
                        os << name << endl;
                        first = false;
                    }
                    return substitute(substitute("[" + os.str() + "]", "\n", "\\n"), "\"", "\\\"");
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, con);
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
bool CConfigItem::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CConfigItem& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CConfigItem::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "named") {
        if (index == NOPOS) {
            CAccountName empty;
            ((CConfigItem*)this)->named.push_back(empty);  // NOLINT
            index = named.size() - 1;
        }
        if (index < named.size())
            return &named[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CONFIGITEM = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
