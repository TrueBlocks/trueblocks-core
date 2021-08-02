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
#include "profitandloss.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CProfitAndLoss, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextProfitandlossChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextProfitandlossChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CProfitAndLoss::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["profitandloss_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextProfitandlossChunk, this);
}

//---------------------------------------------------------------------------
string_q nextProfitandlossChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CProfitAndLoss*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CProfitAndLoss::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextProfitandlossChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'l':
            if (fieldName % "lastDate") {
                return lastDate.Format(FMT_JSON);
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
bool CProfitAndLoss::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'l':
            if (fieldName % "lastDate") {
                lastDate = str_2_Date(fieldValue);
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
void CProfitAndLoss::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CProfitAndLoss::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> type;
    archive >> lastDate;
    // EXISTING_CODE
    // archive >> currentStatements;
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CProfitAndLoss::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << type;
    archive << lastDate;
    // EXISTING_CODE
    // archive << currentStatements;
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CProfitAndLossArray& array) {
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
CArchive& operator<<(CArchive& archive, const CProfitAndLossArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CProfitAndLoss::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CProfitAndLoss, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CProfitAndLoss, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CProfitAndLoss, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CProfitAndLoss, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CProfitAndLoss, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CProfitAndLoss, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CProfitAndLoss, "lastDate", T_DATE, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CProfitAndLoss, "schema");
    HIDE_FIELD(CProfitAndLoss, "deleted");
    HIDE_FIELD(CProfitAndLoss, "showing");
    HIDE_FIELD(CProfitAndLoss, "cname");

    builtIns.push_back(_biCProfitAndLoss);

    // EXISTING_CODE
    ADD_FIELD(CProfitAndLoss, "currentStatements", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextProfitandlossChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CProfitAndLoss* pro = reinterpret_cast<const CProfitAndLoss*>(dataPtr);
    if (pro) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'c':
                if (fieldIn % "currentStatements" || fieldIn % "currentStatementsCnt") {
                    size_t cnt = pro->currentStatements.size();
                    if (endsWith(toLower(fieldIn), "cnt"))
                        return uint_2_Str(cnt);
                    if (!cnt)
                        return "";
                    string_q retS;
                    bool first = true;
                    for (auto statement : pro->currentStatements) {
                        retS += (first ? "\n" : ",\n") ;
                        retS += statement.second.Format() ;
                    }
                    return retS;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, pro);
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
bool CProfitAndLoss::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CProfitAndLoss& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_PROFITANDLOSS = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
