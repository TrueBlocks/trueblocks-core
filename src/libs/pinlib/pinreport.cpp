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
#include "pinreport.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CPinReport, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextPinreportChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextPinreportChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CPinReport::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["pinreport_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextPinreportChunk, this);
}

//---------------------------------------------------------------------------
string_q nextPinreportChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CPinReport*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CPinReport::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextPinreportChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomFormat") {
                return bloomFormat;
            }
            break;
        case 'f':
            if (fieldName % "fileName") {
                return fileName;
            }
            break;
        case 'i':
            if (fieldName % "indexFormat") {
                return indexFormat;
            }
            break;
        case 'n':
            if (fieldName % "newBlockRange") {
                return newBlockRange;
            }
            if (fieldName % "newPins" || fieldName % "newPinsCnt") {
                size_t cnt = newPins.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += newPins[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'p':
            if (fieldName % "prevHash") {
                return hash_2_Str(prevHash);
            }
            if (fieldName % "prevBlockRange") {
                return prevBlockRange;
            }
            if (fieldName % "prevPins" || fieldName % "prevPinsCnt") {
                size_t cnt = prevPins.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += prevPins[i].Format();
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
bool CPinReport::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "bloomFormat") {
                bloomFormat = fieldValue;
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
            if (fieldName % "indexFormat") {
                indexFormat = fieldValue;
                return true;
            }
            break;
        case 'n':
            if (fieldName % "newBlockRange") {
                newBlockRange = fieldValue;
                return true;
            }
            if (fieldName % "newPins") {
                CPinnedItem obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    newPins.push_back(obj);
                    obj = CPinnedItem();  // reset
                }
                return true;
            }
            break;
        case 'p':
            if (fieldName % "prevHash") {
                prevHash = str_2_Hash(fieldValue);
                return true;
            }
            if (fieldName % "prevBlockRange") {
                prevBlockRange = fieldValue;
                return true;
            }
            if (fieldName % "prevPins") {
                CPinnedItem obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    prevPins.push_back(obj);
                    obj = CPinnedItem();  // reset
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
void CPinReport::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CPinReport::Serialize(CArchive& archive) {
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
    archive >> indexFormat;
    archive >> bloomFormat;
    archive >> prevHash;
    archive >> newBlockRange;
    archive >> newPins;
    archive >> prevBlockRange;
    archive >> prevPins;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CPinReport::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << fileName;
    archive << indexFormat;
    archive << bloomFormat;
    archive << prevHash;
    archive << newBlockRange;
    archive << newPins;
    archive << prevBlockRange;
    archive << prevPins;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinReportArray& array) {
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
CArchive& operator<<(CArchive& archive, const CPinReportArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CPinReport::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CPinReport, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CPinReport, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CPinReport, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinReport, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CPinReport, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CPinReport, "fileName", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinReport, "indexFormat", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinReport, "bloomFormat", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinReport, "prevHash", T_HASH | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinReport, "newBlockRange", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinReport, "newPins", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinReport, "prevBlockRange", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CPinReport, "prevPins", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CPinReport, "schema");
    HIDE_FIELD(CPinReport, "deleted");
    HIDE_FIELD(CPinReport, "showing");
    HIDE_FIELD(CPinReport, "cname");

    builtIns.push_back(_biCPinReport);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextPinreportChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CPinReport* pin = reinterpret_cast<const CPinReport*>(dataPtr);
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
bool CPinReport::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CPinReport& pin) {
    pin.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CPinReport& pin) {
    pin.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CPinReport& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CPinReport::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "newPins") {
        if (index == NOPOS) {
            CPinnedItem empty;
            ((CPinReport*)this)->newPins.push_back(empty);  // NOLINT
            index = newPins.size() - 1;
        }
        if (index < newPins.size())
            return &newPins[index];
    }

    if (fieldName % "prevPins") {
        if (index == NOPOS) {
            CPinnedItem empty;
            ((CPinReport*)this)->prevPins.push_back(empty);  // NOLINT
            index = prevPins.size() - 1;
        }
        if (index < prevPins.size())
            return &prevPins[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PINREPORT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
