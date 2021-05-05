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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 *  the code inside of 'EXISTING_CODE' tags.
 */
#include "cachedaccount.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CCachedAccount, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextCachedaccountChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextCachedaccountChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CCachedAccount::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["cachedaccount_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    if (handleCustomFormat(ctx, fmt, dataPtr))
        return;
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextCachedaccountChunk, this);
}

//---------------------------------------------------------------------------
string_q nextCachedaccountChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CCachedAccount*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CCachedAccount::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextCachedaccountChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addr") {
                return addr_2_Str(addr);
            }
            break;
        case 'l':
            if (fieldName % "latestPage") {
                return uint_2_Str(latestPage);
            }
            if (fieldName % "latestTx") {
                if (latestTx == CTransaction())
                    return "{}";
                return latestTx.Format();
            }
            break;
        case 't':
            if (fieldName % "transactions" || fieldName % "transactionsCnt") {
                size_t cnt = transactions.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += transactions[i].Format();
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

    string_q s;
    s = toUpper(string_q("latestTx")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = latestTx.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CCachedAccount::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "latestTx") {
        string_q str = fieldValue;
        return latestTx.parseJson3(str);
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "addr") {
                addr = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "latestPage") {
                latestPage = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "latestTx") {
                return latestTx.parseJson3(fieldValue);
            }
            break;
        case 't':
            if (fieldName % "transactions") {
                CTransaction obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    transactions.push_back(obj);
                    obj = CTransaction();  // reset
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
void CCachedAccount::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CCachedAccount::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> addr;
    archive >> latestPage;
    archive >> latestTx;
    archive >> transactions;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CCachedAccount::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << addr;
    archive << latestPage;
    archive << latestTx;
    archive << transactions;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CCachedAccountArray& array) {
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
CArchive& operator<<(CArchive& archive, const CCachedAccountArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CCachedAccount::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CCachedAccount, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CCachedAccount, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CCachedAccount, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CCachedAccount, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CCachedAccount, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CCachedAccount, "addr", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CCachedAccount, "latestPage", T_UNUMBER, ++fieldNum);
    ADD_OBJECT(CCachedAccount, "latestTx", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CTransaction));
    ADD_FIELD(CCachedAccount, "transactions", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CCachedAccount, "schema");
    HIDE_FIELD(CCachedAccount, "deleted");
    HIDE_FIELD(CCachedAccount, "showing");
    HIDE_FIELD(CCachedAccount, "cname");

    builtIns.push_back(_biCCachedAccount);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextCachedaccountChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CCachedAccount* cac = reinterpret_cast<const CCachedAccount*>(dataPtr);
    if (cac) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'n':
                if (fieldIn % "now")
                    return (isTestMode() ? "TESTING_TIME" : Now().Format(FMT_JSON));
                break;
            case 'r':
                if (fieldIn % "records")
                    return (cac->transactions.size() == 0 ? "No records" : "");
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, cac);
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
bool CCachedAccount::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CCachedAccount& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CCachedAccount::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "latestTx")
        return &latestTx;

    if (fieldName % "transactions") {
        if (index == NOPOS) {
            CTransaction empty;
            ((CCachedAccount*)this)->transactions.push_back(empty);  // NOLINT
            index = transactions.size() - 1;
        }
        if (index < transactions.size())
            return &transactions[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_CACHEDACCOUNT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CCachedAccount::handleCustomFormat(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    // Split the format string into three parts: pre, post and records.
    // If no records, just process as normal. We do this because it's so slow
    // copying the records into a string, so we write it directly to the
    // export context. If there is no {RECORDS}, then just send handle it like normal
    if (!contains(fmtIn, "{RECORDS}") || transactions.size() == 0) {
        string_q fmt = fmtIn;

        while (!fmt.empty())
            ctx << getNextChunk(fmt, nextCachedaccountChunk, this);

    } else {
        string_q postFmt = fmtIn;
        replace(postFmt, "{RECORDS}", "|");
        string_q preFmt = nextTokenClear(postFmt, '|');

        // We assume here that the token was properly formed. For the pre-text we
        // have to clear out the start '[', and for the post text we clear out the ']'
        replaceReverse(preFmt, "[", "");
        replace(postFmt, "]", "");

        // We handle the display in three parts: pre, records, and post so as
        // to avoid building the entire record list into an ever-growing and
        // ever-slowing string
        while (!preFmt.empty())
            ctx << getNextChunk(preFmt, nextCachedaccountChunk, this);
        size_t cnt = 0;
        for (size_t i = 0; i < transactions.size(); i++) {
            cnt += transactions[i].m_showing;
            if (cnt && !(cnt % 563)) {
                cerr << "\tExporting record " << cnt << " of " << transactions.size() << " records\r";
                cerr.flush();
            }
            ctx << transactions[i].Format(displayString);
        }
        ctx << "\n";
        while (!postFmt.empty())
            ctx << getNextChunk(postFmt, nextCachedaccountChunk, this);
    }
    return true;
}

//---------------------------------------------------------------------------
void CCachedAccount::markLatest(const CTransaction& trans) {
    if (trans.blockNumber < latestTx.blockNumber)
        return;
    if (trans.blockNumber > latestTx.blockNumber) {
        latestTx.blockNumber = trans.blockNumber;
        latestTx.transactionIndex = trans.transactionIndex;
        return;
    }
    if (trans.transactionIndex > latestTx.transactionIndex) {
        latestTx.blockNumber = trans.blockNumber;
        latestTx.transactionIndex = trans.transactionIndex;
    }
    return;
}
// EXISTING_CODE
}  // namespace qblocks
