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
#include "newreceipt.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CNewReceipt, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextNewreceiptChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextNewreceiptChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CNewReceipt::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["newreceipt_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextNewreceiptChunk, this);
}

//---------------------------------------------------------------------------
string_q nextNewreceiptChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CNewReceipt*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CNewReceipt::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextNewreceiptChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "contractAddress") {
                return addr_2_Str(contractAddress);
            }
            break;
        case 'g':
            if (fieldName % "gasUsed") {
                return gas_2_Str(gasUsed);
            }
            break;
        case 'i':
            if (fieldName % "isErr") {
                return bool_2_Str_t(isErr);
            }
            break;
        case 'l':
            if (fieldName % "logs" || fieldName % "logsCnt") {
                size_t cnt = logs.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += logs[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            if (fieldName % "logsBloom") {
                return logsBloom;
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
bool CNewReceipt::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "contractAddress") {
                contractAddress = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'g':
            if (fieldName % "gasUsed") {
                gasUsed = str_2_Gas(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "isErr") {
                isErr = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "logs") {
                CLogEntry obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    logs.push_back(obj);
                    obj = CLogEntry();  // reset
                }
                return true;
            }
            if (fieldName % "logsBloom") {
                logsBloom = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CNewReceipt::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> contractAddress;
    archive >> gasUsed;
    archive >> logs;
    archive >> logsBloom;
    archive >> isErr;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    ((CNewReceipt*)this)->m_schema = getVersionNum();  // NOLINT
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << contractAddress;
    archive << gasUsed;
    archive << logs;
    archive << logsBloom;
    archive << isErr;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CNewReceipt::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CNewReceipt copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CNewReceiptArray& array) {
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
CArchive& operator<<(CArchive& archive, const CNewReceiptArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CNewReceipt::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CNewReceipt, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CNewReceipt, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CNewReceipt, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CNewReceipt, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CNewReceipt, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CNewReceipt, "contractAddress", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CNewReceipt, "gasUsed", T_GAS, ++fieldNum);
    ADD_FIELD(CNewReceipt, "logs", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CNewReceipt, "logsBloom", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CNewReceipt, "isErr", T_BOOL | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CNewReceipt, "schema");
    HIDE_FIELD(CNewReceipt, "deleted");
    HIDE_FIELD(CNewReceipt, "showing");
    HIDE_FIELD(CNewReceipt, "cname");

    builtIns.push_back(_biCNewReceipt);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextNewreceiptChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CNewReceipt* newr = reinterpret_cast<const CNewReceipt*>(dataPtr);
    if (newr) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, newr);
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
bool CNewReceipt::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CNewReceipt& newr) {
    newr.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CNewReceipt& newr) {
    newr.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CNewReceipt& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CNewReceipt::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "logs") {
        if (index == NOPOS) {
            CLogEntry empty;
            ((CNewReceipt*)this)->logs.push_back(empty);  // NOLINT
            index = logs.size() - 1;
        }
        if (index < logs.size())
            return &logs[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_NEWRECEIPT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
