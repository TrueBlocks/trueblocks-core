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
#include <algorithm>
#include "accountwatch.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountWatch, CAccountName);

//---------------------------------------------------------------------------
static string_q nextAccountwatchChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAccountwatchChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAccountWatch::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["accountwatch_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAccountwatchChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAccountwatchChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAccountWatch*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAccountWatch::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAccountwatchChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "abi_spec") {
                if (abi_spec == CAbi())
                    return "";
                expContext().noFrst = true;
                return abi_spec.Format();
            }
            break;
        case 'c':
            if (fieldName % "curBalance") {
                return wei_2_Str(curBalance);
            }
            break;
        case 'e':
            if (fieldName % "enabled") {
                return bool_2_Str_t(enabled);
            }
            break;
        case 'f':
            if (fieldName % "fm_mode") {
                return uint_2_Str(fm_mode);
            }
            break;
        case 's':
            if (fieldName % "statement") {
                if (statement == CIncomeStatement())
                    return "";
                expContext().noFrst = true;
                return statement.Format();
            }
            if (fieldName % "stateHistory" || fieldName % "stateHistoryCnt") {
                size_t cnt = stateHistory.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += stateHistory[i].Format();
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
    s = toUpper(string_q("abi_spec")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = abi_spec.getValueByName(f);
        return f;
    }

    s = toUpper(string_q("statement")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = statement.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CAccountName::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "statement") {
        string_q str = fieldValue;
        return statement.parseJson3(str);
    }
    if (fieldName % "balance") {
        statement.endBal = statement.begBal = str_2_Wei(fieldValue);
        return true;
    }
    if (fieldName % "address") {
        if (getCurlContext()->nodeRequired)
            bloom = makeBloom(fieldValue);
    }
    // EXISTING_CODE

    if (CAccountName::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "abi_spec") {
                return abi_spec.parseJson3(fieldValue);
            }
            break;
        case 'c':
            if (fieldName % "curBalance") {
                curBalance = str_2_Wei(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "enabled") {
                enabled = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 'f':
            if (fieldName % "fm_mode") {
                fm_mode = str_2_Enum(freshen_e, fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "statement") {
                return statement.parseJson3(fieldValue);
            }
            if (fieldName % "stateHistory") {
                CEthState item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    stateHistory.push_back(item);
                    item = CEthState();  // reset
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
void CAccountWatch::finishParse() {
    // EXISTING_CODE
    if (getCurlContext()->nodeRequired)
        bloom = makeBloom(address);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CAccountName::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> abi_spec;
    archive >> statement;
    archive >> stateHistory;
    archive >> curBalance;
    archive >> enabled;
    // archive >> fm_mode;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountWatch::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CAccountName::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << abi_spec;
    archive << statement;
    archive << stateHistory;
    archive << curBalance;
    archive << enabled;
    // archive << fm_mode;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAccountWatchArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAccountWatchArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAccountWatch::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAccountWatch, "schema"))
        return;

    CAccountName::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CAccountWatch, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccountWatch, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "abi_spec", T_OBJECT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "statement", T_OBJECT, ++fieldNum);
    ADD_FIELD(CAccountWatch, "stateHistory", T_OBJECT | TS_ARRAY, ++fieldNum);
    ADD_FIELD(CAccountWatch, "curBalance", T_WEI, ++fieldNum);
    ADD_FIELD(CAccountWatch, "enabled", T_BOOL, ++fieldNum);
    ADD_FIELD(CAccountWatch, "fm_mode", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CAccountWatch, "fm_mode");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountWatch, "schema");
    HIDE_FIELD(CAccountWatch, "deleted");
    HIDE_FIELD(CAccountWatch, "showing");
    HIDE_FIELD(CAccountWatch, "cname");

    builtIns.push_back(_biCAccountWatch);

    // EXISTING_CODE
    ADD_FIELD(CAccountWatch, "curEther", T_ETHER, ++fieldNum);
    HIDE_FIELD(CAccountWatch, "curEther");
    ADD_FIELD(CAccountWatch, "curDollars", T_ETHER, ++fieldNum);
    HIDE_FIELD(CAccountWatch, "curDollars");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAccountwatchChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAccountWatch* acc = reinterpret_cast<const CAccountWatch*>(dataPtr);
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'c':
                if (startsWith(fieldIn, "cur") && acc->curBalance == str_2_Wei(uint_2_Str(NOPOS)))
                    return "\"n/a\"";
                if (fieldIn % "curEther")
                    return wei_2_Ether(wei_2_Str(acc->curBalance));
                if (fieldIn % "curDollars")
                    return "not-implemented";
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, acc);
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
bool CAccountWatch::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccountWatch& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CAccountWatch::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "abi_spec")
        return &abi_spec;
    if (fieldName % "statement")
        return &statement;
    if (fieldName % "stateHistory" && index < stateHistory.size())
        return &stateHistory[index];
    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ACCOUNTWATCH = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
// This assumes there are valid watches. Caller is expected to check
void loadWatchList(const CToml& toml, CAccountWatchArray& monitors, const string_q& key) {
    string_q watchStr = toml.getConfigJson("watches", key, "");
    CAccountWatch watch;
    while (watch.parseJson3(watchStr)) {
        // cleanup and add to list of watches
        watch.address = str_2_Addr(toLower(watch.address));
        watch.color = convertColor(watch.color);
        monitors.push_back(watch);
        watch = CAccountWatch();  // reset
    }
    return;
}

//-------------------------------------------------------------------------
bool CAccountWatch::openCacheFile1(void) {
    if (tx_cache != NULL)
        return true;
    tx_cache = new CArchive(WRITING_ARCHIVE);
    if (tx_cache == NULL)
        return false;
    return tx_cache->Lock(getMonitorPath(address, fm_mode), modeWriteAppend, LOCK_WAIT);
}

//-------------------------------------------------------------------------
void CAccountWatch::writeLastBlock(blknum_t bn) {
    stringToAsciiFile(getMonitorLast(address, fm_mode), uint_2_Str(bn) + "\n");
}

//-------------------------------------------------------------------------
void CAccountWatch::writeLastExport(blknum_t bn) {
    stringToAsciiFile(getMonitorExpt(address, fm_mode), uint_2_Str(bn) + "\n");
}

//-------------------------------------------------------------------------
void CAccountWatch::writeARecord(blknum_t bn, blknum_t tx_id) {
    if (tx_cache == NULL)
        return;
    *tx_cache << bn << tx_id;
    tx_cache->flush();
}

//-------------------------------------------------------------------------
void CAccountWatch::writeAnArray(const CAppearanceArray_base& items) {
    if (tx_cache == NULL)
        return;
    for (auto item : items)
        *tx_cache << item.blk << item.txid;
    tx_cache->flush();
}
// EXISTING_CODE
}  // namespace qblocks
