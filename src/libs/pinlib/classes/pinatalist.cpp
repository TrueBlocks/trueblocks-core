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
#include "pinatalist.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinataPinlist, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPinatapinlistChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinatapinlistChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinataPinlist::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinatapinlist_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextPinatapinlistChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinatapinlistChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinataPinlist*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinataPinlist::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinatapinlistChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "count") {
                return count;
            }
            break;
        case 'r':
            if (fieldName % "rows" || fieldName % "rowsCnt") {
                size_t cnt = rows.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += rows[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
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
bool CPinataPinlist::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "count") {
                count = fieldValue;
                return true;
            }
            break;
        case 'r':
            if (fieldName % "rows") {
                CPinataPin obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    rows.push_back(obj);
                    obj = CPinataPin();  // reset
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CPinataPinlist::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinataPinlist::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> count;
    archive >> rows;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinataPinlist::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << count;
    archive << rows;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinataPinlistArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPinataPinlistArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinataPinlist::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinataPinlist, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinataPinlist, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinataPinlist, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataPinlist, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinataPinlist, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinataPinlist, "count", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinataPinlist, "rows", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinataPinlist, "schema");
    HIDE_FIELD(CPinataPinlist, "deleted");
    HIDE_FIELD(CPinataPinlist, "showing");
    HIDE_FIELD(CPinataPinlist, "cname");

    builtIns.push_back(_biCPinataPinlist);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinatapinlistChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinataPinlist* pin = reinterpret_cast<const CPinataPinlist*>(dataPtr);
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
bool CPinataPinlist::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinataPinlist& pin) {
    pin.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinataPinlist& pin) {
    pin.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinataPinlist& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CPinataPinlist::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "rows") {
        if (index == NOPOS) {
            CPinataPin empty;
            ((CPinataPinlist*)this)->rows.push_back(empty);  // NOLINT
            index = rows.size() - 1;
        }
        if (index < rows.size())
            return &rows[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINATAPINLIST = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
