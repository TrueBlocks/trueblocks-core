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
#include "monitor.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CMonitor, CAccountName);

//---------------------------------------------------------------------------
static string_q nextMonitorChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextMonitorChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CMonitor::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["monitor_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextMonitorChunk, this);
}

//---------------------------------------------------------------------------
string_q nextMonitorChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CMonitor*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CMonitor::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextMonitorChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "curBalance") {
                return wei_2_Str(curBalance);
            }
            break;
        case 'e':
            if (fieldName % "enabled") {
                return bool_2_Str(enabled);
            }
            break;
        case 'f':
            if (fieldName % "fm_mode") {
                return uint_2_Str(fm_mode);
            }
            break;
        case 's':
            if (fieldName % "summaryStatement") {
                if (summaryStatement == CReconciliation())
                    return "{}";
                return summaryStatement.Format();
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
    s = toUpper(string_q("summaryStatement")) + "::";
    if (contains(fieldName, s)) {
        string_q f = fieldName;
        replaceAll(f, s, "");
        f = summaryStatement.getValueByName(f);
        return f;
    }

    // Finally, give the parent class a chance
    return CAccountName::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CMonitor::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "summaryStatement") {
        string_q str = fieldValue;
        return summaryStatement.parseJson3(str);
    }
    if (fieldName % "balance") {
        summaryStatement.endBal = summaryStatement.begBal = str_2_Wei(fieldValue);
        return true;
    }
    // EXISTING_CODE

    if (CAccountName::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
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
            if (fieldName % "summaryStatement") {
                return summaryStatement.parseJson3(fieldValue);
            }
            if (fieldName % "stateHistory") {
                CEthState obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    stateHistory.push_back(obj);
                    obj = CEthState();  // reset
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
void CMonitor::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CMonitor::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CAccountName::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> summaryStatement;
    archive >> stateHistory;
    archive >> curBalance;
    archive >> enabled;
    // archive >> fm_mode;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMonitor::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CAccountName::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << summaryStatement;
    archive << stateHistory;
    archive << curBalance;
    archive << enabled;
    // archive << fm_mode;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CMonitorArray& array) {
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
CArchive& operator<<(CArchive& archive, const CMonitorArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CMonitor::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CMonitor, "schema"))
        return;

    CAccountName::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CMonitor, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMonitor, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CMonitor, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CMonitor, "cname", T_TEXT, ++fieldNum);
    ADD_OBJECT(CMonitor, "summaryStatement", T_OBJECT | TS_OMITEMPTY, ++fieldNum, GETRUNTIME_CLASS(CReconciliation));
    ADD_FIELD(CMonitor, "stateHistory", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMonitor, "curBalance", T_WEI, ++fieldNum);
    ADD_FIELD(CMonitor, "enabled", T_BOOL | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMonitor, "fm_mode", T_NUMBER, ++fieldNum);
    HIDE_FIELD(CMonitor, "fm_mode");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CMonitor, "schema");
    HIDE_FIELD(CMonitor, "deleted");
    HIDE_FIELD(CMonitor, "showing");
    HIDE_FIELD(CMonitor, "cname");

    builtIns.push_back(_biCMonitor);

    // EXISTING_CODE
    ADD_FIELD(CMonitor, "curEther", T_ETHER, ++fieldNum);
    HIDE_FIELD(CMonitor, "curEther");
    ADD_FIELD(CMonitor, "curDollars", T_ETHER, ++fieldNum);
    HIDE_FIELD(CMonitor, "curDollars");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextMonitorChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CMonitor* mon = reinterpret_cast<const CMonitor*>(dataPtr);
    if (mon) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'c':
                if (startsWith(fieldIn, "cur") && mon->curBalance == str_2_Wei(uint_2_Str(NOPOS)))
                    return "\"n/a\"";
                if (fieldIn % "curEther")
                    return "\"" + wei_2_Ether(wei_2_Str(mon->curBalance)) + "\"";
                if (fieldIn % "curDollars")
                    return "not-implemented";
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, mon);
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
bool CMonitor::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CMonitor& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CMonitor::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "summaryStatement")
        return &summaryStatement;

    if (fieldName % "stateHistory") {
        if (index == NOPOS) {
            CEthState empty;
            ((CMonitor*)this)->stateHistory.push_back(empty);  // NOLINT
            index = stateHistory.size() - 1;
        }
        if (index < stateHistory.size())
            return &stateHistory[index];
    }

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_MONITOR = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CMonitor::openForWriting(void) {
    if (tx_cache != NULL)
        return true;
    tx_cache = new CArchive(WRITING_ARCHIVE);
    if (tx_cache == NULL)
        return false;
    return tx_cache->Lock(getMonitorPath(address, fm_mode), modeWriteAppend, LOCK_WAIT);
}

//-------------------------------------------------------------------------
void CMonitor::writeMonitorArray(const CAppearanceArray_base& items) {
    if (tx_cache == NULL)
        return;
    for (auto item : items)
        *tx_cache << item.blk << item.txid;
    tx_cache->flush();
}

//-------------------------------------------------------------------------
void CMonitor::writeLastBlock(blknum_t bn) {
    lastVisitedBlock = bn;
    stringToAsciiFile(getMonitorLast(address, fm_mode), uint_2_Str(bn) + "\n");
}

//-------------------------------------------------------------------------
void CMonitor::writeLastExport(blknum_t bn) {
    stringToAsciiFile(getMonitorExpt(address, fm_mode), uint_2_Str(bn) + "\n");
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorPath(const address_t& addr, freshen_e mode) const {
    string_q fn = isAddress(addr) ? addr + ".acct.bin" : addr;
    string_q base = getCachePath("monitors/") + (mode == FM_STAGING ? "staging/" : "");
    if (isTestMode())
        base = configPath("mocked/monitors/") + (mode == FM_STAGING ? "staging/" : "");
    return base + fn;
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorLast(const address_t& addr, freshen_e mode) const {
    return substitute(getMonitorPath(addr, mode), ".acct.bin", ".last.txt");
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorExpt(const address_t& addr, freshen_e mode) const {
    return substitute(getMonitorPath(addr, mode), ".acct.bin", ".expt.txt");
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorDels(const address_t& addr, freshen_e mode) const {
    return getMonitorPath(addr) + ".deleted";
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorCach(const address_t& addr, freshen_e mode) const {
    return getMonitorPath(addr + ".txs.bin");
}

//--------------------------------------------------------------------------------
blknum_t CMonitor::getLastVisited(bool fresh) const {
    if (lastVisitedBlock == NOPOS || fresh) {
        // If the monitor exists, the next block is stored in the database...
        if (fileExists(getMonitorLast(address))) {
            ((CMonitor*)this)->lastVisitedBlock = str_2_Uint(asciiFileToString(getMonitorLast(address)));  // NOLINT

        } else {
            // Accounts can receive ETH counter-factually. By default, we ignore
            // this and start our scan from the account's deploy block (in the case
            // of a contract) or the zero block. User can change this setting.
            if (getGlobalConfig("acctExport")->getConfigBool("settings", "start-when-deployed", true)) {
                blknum_t deployed = getDeployBlock(address);
                ((CMonitor*)this)->lastVisitedBlock = (deployed == NOPOS ? 0 : deployed);  // NOLINT
            }
        }
    }
    return lastVisitedBlock;
}

//-----------------------------------------------------------------------
blknum_t CMonitor::getLastVisitedBlock(void) const {
    return str_2_Uint(asciiFileToString(getMonitorLast(address)));
}

//-----------------------------------------------------------------------
blknum_t CMonitor::getLastExportedBlock(void) const {
    return str_2_Uint(asciiFileToString(getMonitorExpt(address)));
}

//-----------------------------------------------------------------------
bool CMonitor::monitorExists(void) const {
    if (fileExists(getMonitorPath(address)))
        return true;
    if (fileExists(getMonitorLast(address)))
        return true;
    if (fileExists(getMonitorExpt(address)))
        return true;
    if (fileExists(getMonitorDels(address)))
        return true;
    if (fileExists(getMonitorCach(address)))
        return true;
    return false;
}

//--------------------------------------------------------------------------------
#define checkLock(fn, b)                                                                                               \
    if (fileExists((fn) + ".lck")) {                                                                                   \
        msg = ("The " + string_q(b) + " file for monitor " + address + " is locked. Quitting...");                     \
        return true;                                                                                                   \
    }

//--------------------------------------------------------------------------------
bool CMonitor::isMonitorLocked(string_q& msg) const {
    checkLock(getMonitorPath(address), "cache");
    checkLock(getMonitorLast(address), "last block");
    checkLock(getMonitorExpt(address), "last export");
    checkLock(getMonitorDels(address), "marker");
    checkLock(getMonitorCach(address), "cache");
    return false;
}

//--------------------------------------------------------------------------------
bool CMonitor::clearMonitorLocks(void) {
    ::remove((getMonitorPath(address) + ".lck").c_str());
    ::remove((getMonitorLast(address) + ".lck").c_str());
    ::remove((getMonitorExpt(address) + ".lck").c_str());
    ::remove((getMonitorDels(address) + ".lck").c_str());
    ::remove((getMonitorCach(address) + ".lck").c_str());
    return true;
}

//--------------------------------------------------------------------------------
void doMoveFile(const string_q& from, const string_q& to) {
#define CLEAN(a) (cTeal + (isTestMode() ? substitute((a), getCachePath(""), "$CACHE/") : (a)) + cOff)
    LOG4("Moving ", CLEAN(from), " to ", CLEAN(to));
    if (fileExists(from))
        moveFile(from, to);
}

//--------------------------------------------------------------------------------
void CMonitor::moveToProduction(void) {
    if (fm_mode == FM_PRODUCTION)
        return;
    ASSERT(fm_mode == FM_STAGING);

    fm_mode = FM_PRODUCTION;
    if (tx_cache) {
        tx_cache->Release();
        delete tx_cache;
        tx_cache = NULL;
    }
    bool binExists = fileExists(getMonitorPath(address, FM_STAGING));
    bool lastExists = fileExists(getMonitorLast(address, FM_STAGING));
    lockSection();
    if (binExists || lastExists) {
        doMoveFile(getMonitorPath(address, FM_STAGING), getMonitorPath(address));
        doMoveFile(getMonitorLast(address, FM_STAGING), getMonitorLast(address));
        doMoveFile(getMonitorExpt(address, FM_STAGING), getMonitorExpt(address));
        doMoveFile(getMonitorCach(address, FM_STAGING), getMonitorCach(address));
    } else {
        // For some reason (user quit, UI switched to adding a different address to monitor, something went
        // wrong...) the binary cache was not created. Cleanup everything. The user will have to start over.
        ::remove(getMonitorPath(address, FM_STAGING).c_str());
        ::remove(getMonitorLast(address, FM_STAGING).c_str());
        ::remove(getMonitorExpt(address, FM_STAGING).c_str());
        ::remove(getMonitorCach(address, FM_STAGING).c_str());
    }
    unlockSection();
}

//-----------------------------------------------------------------------
bool CMonitor::isDeleted(void) const {
    return fileExists(getMonitorDels(address));
}

//-----------------------------------------------------------------------
void CMonitor::deleteMonitor(void) {
    stringToAsciiFile(getMonitorDels(address), Now().Format(FMT_EXPORT));
}

//-----------------------------------------------------------------------
void CMonitor::undeleteMonitor(void) {
    ::remove(getMonitorDels(address).c_str());
}

//---------------------------------------------------------------------------
void removeFile(const string_q& fn) {
    ::remove(fn.c_str());
    ::remove((fn + ".lck").c_str());
}

//-----------------------------------------------------------------------
void CMonitor::removeMonitor(void) {
    removeFile(getMonitorPath(address));
    removeFile(getMonitorLast(address));
    removeFile(getMonitorExpt(address));
    removeFile(getMonitorDels(address));
    removeFile(getMonitorCach(address));
}

//-----------------------------------------------------------------------
bloom_t CMonitor::getBloom(void) {
    bloom_t not_set;
    if (bloom == not_set) {
        bloom = addr_2_Bloom(address);
    }
    return bloom;
}

//----------------------------------------------------------------
void establishMonitorFolders(void) {
    CMonitor m;
    establishFolder(m.getMonitorPath("", FM_PRODUCTION));
    establishFolder(m.getMonitorPath("", FM_STAGING));
}

//----------------------------------------------------------------
void cleanMonitorStage(void) {
    CMonitor m;
    cleanFolder(m.getMonitorPath("", FM_STAGING));
}

//-------------------------------------------------------------------------
const char* STR_DISPLAY_TOKENBALANCERECORD2 =
    "[{BLOCKNUMBER}]\t"
    "[{HOLDER}]\t"
    "[{ADDRESS}]\t"
    "[{NAME}]\t"
    "[{SYMBOL}]\t"
    "[{DECIMALS}]\t"
    "[{BALANCE}]";

//-------------------------------------------------------------------------
string_q getTokenBalanceOf(const CAbi& abi_spec, const CMonitor& token, const address_t& holder, blknum_t blockNum) {
    map<string_q, string_q> sigMap;
    sigMap["balanceOf"] = "0x70a08231";
    CFunction result;
    if (doEthCall(token.address, sigMap["balanceOf"], padLeft(extract(holder, 2), 64, '0'), blockNum, abi_spec, result))
        return result.outputs[0].value;
    return "";
}

//-------------------------------------------------------------------------
string_q getTokenState(const string_q& what, const CAbi& abi_spec, const CMonitor& token, blknum_t blockNum) {
    map<string_q, string_q> sigMap;
    sigMap["totalSupply"] = "0x18160ddd";
    sigMap["decimals"] = "0x313ce567";
    sigMap["symbol"] = "0x95d89b41";
    sigMap["name"] = "0x06fdde03";
    if (sigMap[what].empty()) {
        return "";
    }

    CFunction result;
    if (!doEthCall(token.address, sigMap[what], "", blockNum, abi_spec, result)) {
        // This may be a proxy contract, so we can try to get its implementation and call back in
        CMonitor proxy = token;
        // sigMap["implementation"] = "0x5c60da1b";
        CFunction proxyResult;
        if (doEthCall(token.address, "0x5c60da1b", "", blockNum, abi_spec, proxyResult)) {
            address_t addr = proxyResult.outputs[0].value;
            if (isZeroAddr(addr))
                return "";
            proxy.address = addr;
            if (doEthCall(proxy.address, sigMap[what], "", blockNum, abi_spec, result))
                return result.outputs[0].value;
            return "";
        }
    }
    return result.outputs.size() ? result.outputs[0].value : "";
}
// EXISTING_CODE
}  // namespace qblocks
