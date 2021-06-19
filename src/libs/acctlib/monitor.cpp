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
        case 'f':
            if (fieldName % "firstAppearance") {
                return uint_2_Str(firstAppearance);
            }
            break;
        case 'l':
            if (fieldName % "lastExport") {
                return uint_2_Str(lastExport);
            }
            if (fieldName % "latestAppearance") {
                return uint_2_Str(latestAppearance);
            }
            break;
        case 'n':
            if (fieldName % "nAppearances") {
                return uint_2_Str(nAppearances);
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                return sizeInBytes == 0 ? "" : uint_2_Str(sizeInBytes);
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CAccountName::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CMonitor::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CAccountName::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'f':
            if (fieldName % "firstAppearance") {
                firstAppearance = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "lastExport") {
                lastExport = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "latestAppearance") {
                latestAppearance = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'n':
            if (fieldName % "nAppearances") {
                nAppearances = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "sizeInBytes") {
                sizeInBytes = str_2_Uint(fieldValue);
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
    // archive >> nAppearances;
    // archive >> lastExport;
    // archive >> firstAppearance;
    // archive >> latestAppearance;
    // archive >> sizeInBytes;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMonitor::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CAccountName::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    // archive << nAppearances;
    // archive << lastExport;
    // archive << firstAppearance;
    // archive << latestAppearance;
    // archive << sizeInBytes;

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
    ADD_FIELD(CMonitor, "nAppearances", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CMonitor, "nAppearances");
    ADD_FIELD(CMonitor, "lastExport", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CMonitor, "lastExport");
    ADD_FIELD(CMonitor, "firstAppearance", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CMonitor, "firstAppearance");
    ADD_FIELD(CMonitor, "latestAppearance", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CMonitor, "latestAppearance");
    ADD_FIELD(CMonitor, "sizeInBytes", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMonitor, "sizeInBytes");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CMonitor, "schema");
    HIDE_FIELD(CMonitor, "deleted");
    HIDE_FIELD(CMonitor, "showing");
    HIDE_FIELD(CMonitor, "cname");

    builtIns.push_back(_biCMonitor);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextMonitorChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CMonitor* mon = reinterpret_cast<const CMonitor*>(dataPtr);
    if (mon) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
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
const char* STR_DISPLAY_MONITOR = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
size_t CMonitor::getFileSize(const string_q& path) const {
    return ::fileSize(path);
}

//-------------------------------------------------------------------------
size_t CMonitor::getRecordCnt(const string_q& path) const {
    return ::fileSize(path) / sizeof(CMonitoredAppearance);
}

//-------------------------------------------------------------------------
bool CMonitor::openForWriting(bool staging) {
    if (tx_cache != NULL)
        return true;
    tx_cache = new CArchive(WRITING_ARCHIVE);
    if (tx_cache == NULL)
        return false;
    return tx_cache->Lock(getMonitorPath(address, staging), modeWriteAppend, LOCK_WAIT);
}

//-------------------------------------------------------------------------
void CMonitor::writeMonitorArray(const CMonitoredAppearanceArray& items) {
    if (tx_cache == NULL)
        return;
    for (auto item : items)
        *tx_cache << item.blk << item.txid;
    tx_cache->flush();
}

//-------------------------------------------------------------------------
void CMonitor::writeMonitorLastBlock(blknum_t bn, bool staging) {
    lastVisitedBlock = bn;
    stringToAsciiFile(getMonitorPathLast(address, staging), uint_2_Str(bn) + "\n");
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorPath(const address_t& addr, bool staging) const {
    string_q fn = isAddress(addr) ? addr + ".acct.bin" : addr;
    string_q base = getCachePath("monitors/") + (staging ? "staging/" : "");
    if (isTestMode())
        base = configPath("mocked/monitors/") + (staging ? "staging/" : "");
    return base + fn;
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorPathLast(const address_t& addr, bool staging) const {
    return substitute(getMonitorPath(addr, staging), ".acct.bin", ".last.txt");
}

//---------------------------------------------------------------------------
string_q CMonitor::getMonitorPathDels(const address_t& addr) const {
    return getMonitorPath(addr, false) + ".deleted";
}

//--------------------------------------------------------------------------------
blknum_t CMonitor::getLastVisited(bool fresh) const {
    if (lastVisitedBlock == NOPOS || fresh) {
        // If the monitor exists, the next block is stored in the database...
        if (fileExists(getMonitorPathLast(address, false))) {
            ((CMonitor*)this)->lastVisitedBlock =
                str_2_Uint(asciiFileToString(getMonitorPathLast(address, false)));  // NOLINT

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
blknum_t CMonitor::getLastBlockInMonitor(void) const {
    return str_2_Uint(asciiFileToString(getMonitorPathLast(address, false)));
}

//-----------------------------------------------------------------------
bool CMonitor::monitorExists(void) const {
    if (fileExists(getMonitorPath(address, false)))
        return true;
    if (fileExists(getMonitorPathLast(address, false)))
        return true;
    if (fileExists(getMonitorPathDels(address)))
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
    checkLock(getMonitorPath(address, false), "cache");
    checkLock(getMonitorPathLast(address, false), "last block");
    checkLock(getMonitorPathDels(address), "marker");
    return false;
}

//--------------------------------------------------------------------------------
bool CMonitor::clearMonitorLocks(void) {
    ::remove((getMonitorPath(address, false) + ".lck").c_str());
    ::remove((getMonitorPathLast(address, false) + ".lck").c_str());
    ::remove((getMonitorPathDels(address) + ".lck").c_str());
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
void CMonitor::moveToProduction(bool staging) {
    if (!staging)
        return;
    ASSERT(staging);
    isStaging = false;

    if (tx_cache) {
        tx_cache->Release();
        delete tx_cache;
        tx_cache = NULL;
    }
    bool binExists = fileExists(getMonitorPath(address, true));
    bool lastExists = fileExists(getMonitorPathLast(address, true));
    lockSection();
    if (binExists || lastExists) {
        doMoveFile(getMonitorPath(address, true), getMonitorPath(address, false));
        doMoveFile(getMonitorPathLast(address, true), getMonitorPathLast(address, false));
    } else {
        // For some reason (user quit, UI switched to adding a different address to monitor, something went
        // wrong...) the binary cache was not created. Cleanup everything. The user will have to start over.
        ::remove(getMonitorPath(address, true).c_str());
        ::remove(getMonitorPathLast(address, true).c_str());
    }
    unlockSection();
}

//-----------------------------------------------------------------------
bool CMonitor::isDeleted(void) const {
    return fileExists(getMonitorPathDels(address));
}

//-----------------------------------------------------------------------
void CMonitor::deleteMonitor(void) {
    stringToAsciiFile(getMonitorPathDels(address), Now().Format(FMT_EXPORT));
}

//-----------------------------------------------------------------------
void CMonitor::undeleteMonitor(void) {
    ::remove(getMonitorPathDels(address).c_str());
}

//---------------------------------------------------------------------------
void removeFile(const string_q& fn) {
    ::remove(fn.c_str());
    ::remove((fn + ".lck").c_str());
}

//-----------------------------------------------------------------------
void CMonitor::removeMonitor(void) {
    removeFile(getMonitorPath(address, false));
    removeFile(getMonitorPathLast(address, false));
    removeFile(getMonitorPathDels(address));
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
blknum_t CMonitor::loadAppsFromPath(CMonitoredAppearanceArray& apps, const string_q& pathIn, MAPPFUNC func,
                                    void* data) const {
    string_q path = (pathIn.empty() ? getMonitorPath(address, false) : pathIn);
    blknum_t nRecs = this->getRecordCnt(path);
    if (!nRecs)
        return false;

    CMonitoredAppearance* buffer = new CMonitoredAppearance[nRecs];
    if (!buffer)
        return false;

    bzero((void*)buffer, nRecs * sizeof(CMonitoredAppearance));  // NOLINT
    CArchive archiveIn(READING_ARCHIVE);
    if (!archiveIn.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        archiveIn.Release();
        delete[] buffer;
        return false;
    }
    archiveIn.Read(buffer, sizeof(CMonitoredAppearance), nRecs);
    archiveIn.Release();

    apps.reserve(apps.size() + nRecs);
    for (size_t i = 0; i < nRecs; i++) {
        apps.push_back(buffer[i]);
        if (func && !(*func)(buffer[i], data))
            return false;
    }

    delete[] buffer;
    return true;
}

//----------------------------------------------------------------
void establishMonitorFolders(void) {
    CMonitor m;
    establishFolder(m.getMonitorPath("", false));
    establishFolder(m.getMonitorPath("", true));
}

//----------------------------------------------------------------
void cleanMonitorStage(void) {
    CMonitor m;
    cleanFolder(m.getMonitorPath("", true));
}

//-------------------------------------------------------------------------
string_q getTokenBalanceOf(const CMonitor& token, const address_t& holder, blknum_t blockNum) {
    ostringstream cmd;
    cmd << "[{";
    cmd << "\"to\": \"" << token.address << "\", ";
    cmd << "\"data\": \"0x70a08231" << padLeft(substitute(holder, "0x", ""), 64, '0') << "\"";
    cmd << "}, \"" << uint_2_Hex(blockNum) << "\"]";
    string_q ret = callRPC("eth_call", cmd.str(), false).substr(0, 66);  // take only the first 32 bytes
    if (startsWith(ret, "0x"))
        return bnu_2_Str(str_2_BigUint(ret, 256));
    return "0";
}

//-------------------------------------------------------------------------
string_q getTokenSymbol(const CMonitor& token, blknum_t blockNum) {
    ostringstream cmd;
    cmd << "[{";
    cmd << "\"to\": \"" << token.address << "\", ";
    cmd << "\"data\": \"0x95d89b41\"";
    cmd << "}, \"" << uint_2_Hex(blockNum) << "\"]";
    string_q ret = callRPC("eth_call", cmd.str(), false);
    if (!contains(ret, "error") && !startsWith(ret, "0x"))
        return ret;
    return "";
}

//-------------------------------------------------------------------------
string_q getTokenState(const CMonitor& token, const string_q& what, const CAbi& abi_spec, blknum_t blockNum) {
    static map<string_q, string_q> sigMap;
    if (sigMap.size() == 0) {
        sigMap["totalSupply"] = "0x18160ddd";
        sigMap["decimals"] = "0x313ce567";
        sigMap["symbol"] = "0x95d89b41";
        sigMap["name"] = "0x06fdde03";
    }

    if (sigMap[what].empty())
        return "";

    CEthCall theCall;
    theCall.address = token.address;
    theCall.encoding = sigMap[what];
    theCall.bytes = "";
    theCall.blockNumber = blockNum;
    theCall.abi_spec = abi_spec;
    if (doEthCall(theCall))
        return theCall.getResults();
    return "";
}
// EXISTING_CODE
}  // namespace qblocks
