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
        toJson(ctx);
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
        case 'a':
            if (fieldName % "abi_spec") {
                if (abi_spec == CAbi())
                    return "{}";
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
                return bool_2_Str(enabled);
            }
            break;
        case 'f':
            if (fieldName % "fm_mode") {
                return uint_2_Str(fm_mode);
            }
            break;
        case 's':
            if (fieldName % "statement") {
                if (statement == CReconciliationNumeric())
                    return "{}";
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
bool CMonitor::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
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
void CMonitor::finishParse() {
    // EXISTING_CODE
    if (getCurlContext()->nodeRequired)
        bloom = makeBloom(address);
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
bool CMonitor::SerializeC(CArchive& archive) const {
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
    ADD_FIELD(CMonitor, "abi_spec", T_OBJECT, ++fieldNum);
    ADD_FIELD(CMonitor, "statement", T_OBJECT, ++fieldNum);
    ADD_FIELD(CMonitor, "stateHistory", T_OBJECT | TS_ARRAY, ++fieldNum);
    ADD_FIELD(CMonitor, "curBalance", T_WEI, ++fieldNum);
    ADD_FIELD(CMonitor, "enabled", T_BOOL, ++fieldNum);
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
ostream& operator<<(ostream& os, const CMonitor& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CMonitor::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "abi_spec")
        return &abi_spec;
    if (fieldName % "statement")
        return &statement;
    if (fieldName % "stateHistory" && index < stateHistory.size())
        return &stateHistory[index];
    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_MONITOR = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool CMonitor::openCacheFile1(void) {
    if (tx_cache != NULL)
        return true;
    tx_cache = new CArchive(WRITING_ARCHIVE);
    if (tx_cache == NULL)
        return false;
    return tx_cache->Lock(getMonitorPath(address, fm_mode), modeWriteAppend, LOCK_WAIT);
}

//-------------------------------------------------------------------------
void CMonitor::writeLastBlock(blknum_t bn) {
    stringToAsciiFile(getMonitorLast(address, fm_mode), uint_2_Str(bn) + "\n");
}

//-------------------------------------------------------------------------
void CMonitor::writeLastExport(blknum_t bn) {
    stringToAsciiFile(getMonitorExpt(address, fm_mode), uint_2_Str(bn) + "\n");
}

//-------------------------------------------------------------------------
void CMonitor::writeARecord(blknum_t bn, blknum_t tx_id) {
    if (tx_cache == NULL)
        return;
    *tx_cache << bn << tx_id;
    tx_cache->flush();
}

//-------------------------------------------------------------------------
void CMonitor::writeAnArray(const CAppearanceArray_base& items) {
    if (tx_cache == NULL)
        return;
    for (auto item : items)
        *tx_cache << item.blk << item.txid;
    tx_cache->flush();
}

//--------------------------------------------------------------------------------
blknum_t CMonitor::nextBlockAsPerMonitor(void) const {
    if (fileExists(getMonitorLast(address)))
        return str_2_Uint(asciiFileToString(getMonitorLast(address)));

    // Contracts can receive ether prior to being deployed (counter-factually). By default, we ignore
    // this and start our scan at the deploy block. EOAs return NOPOS as deploy block.
    if (false) {  // getGlobalConfig("acctScrape")->getConfigBool("settings", "start-when-deployed", true)) {
        blknum_t deployed = getDeployBlock(address);
        return (deployed == NOPOS ? 0 : deployed);
    }
    return 0;
}

//--------------------------------------------------------------------------------
#define checkLock(fn, b)                                                                                               \
    if (fileExists((fn) + ".lck")) {                                                                                   \
        msg = ("The " + string_q(b) + " file for monitor " + address + " is locked. Quitting...");                     \
        return true;                                                                                                   \
    }

//--------------------------------------------------------------------------------
bool CMonitor::isLocked(string_q& msg) const {
    checkLock(getMonitorPath(address), "cache");
    checkLock(getMonitorLast(address), "last block");
    checkLock(getMonitorExpt(address), "last export");
    checkLock(getMonitorBals(address), "last export");
    checkLock(getMonitorDels(address), "marker");
    return false;
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
    lockSection(true);
    bool binExists = fileExists(getMonitorPath(address, FM_STAGING));
    if (binExists) {
        doMoveFile(getMonitorPath(address, FM_STAGING), getMonitorPath(address));
        doMoveFile(getMonitorLast(address, FM_STAGING), getMonitorLast(address));
        doMoveFile(getMonitorExpt(address, FM_STAGING), getMonitorExpt(address));
        doMoveFile(getMonitorBals(address, FM_STAGING), getMonitorBals(address));
    } else {
        // For some reason (user quit, UI switched to adding a different address to monitor, something went
        // wrong...) the binary cache was not created. Cleanup everything. The user will have to start over.
        ::remove(getMonitorPath(address, FM_STAGING).c_str());
        ::remove(getMonitorLast(address, FM_STAGING).c_str());
        ::remove(getMonitorExpt(address, FM_STAGING).c_str());
        ::remove(getMonitorBals(address, FM_STAGING).c_str());
    }
    lockSection(false);
}

//---------------------------------------------------------------------------
string_q getMonitorPath(const string_q& addr, freshen_e mode) {
    string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
    if (!isAddress(addr))  // empty for example
        return getCachePath(base + addr);
    return getCachePath(base + addr + ".acct.bin");
}

//---------------------------------------------------------------------------
string_q getMonitorLast(const string_q& addr, freshen_e mode) {
    string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
    if (!isTestMode() && !isAddress(addr)) {
        cerr << "Not an address: " << addr << endl;
        quickQuitHandler(0);
    }
    return getCachePath(base + addr + ".last.txt");
}

//---------------------------------------------------------------------------
string_q getMonitorExpt(const string_q& addr, freshen_e mode) {
    string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
    if (!isTestMode() && !isAddress(addr)) {
        cerr << "Not an address: " << addr << endl;
        quickQuitHandler(0);
    }
    return getCachePath(base + addr + ".expt.txt");
}

//---------------------------------------------------------------------------
string_q getMonitorBals(const string_q& addr, freshen_e mode) {
    string_q base = ((mode == FM_STAGING) ? "monitors/staging/" : "monitors/");
    if (!isTestMode() && !isAddress(addr)) {
        cerr << "Not an address: " << addr << endl;
        quickQuitHandler(0);
    }
    return getCachePath(base + addr + ".bals.bin");
}

//---------------------------------------------------------------------------
string_q getMonitorDels(const string_q& addr, freshen_e mode) {
    return getMonitorPath(addr) + ".deleted";
}

//---------------------------------------------------------------------------
void cleanMonitor(const address_t& addr) {
    ::remove(getMonitorPath(addr).c_str());
    ::remove(getMonitorLast(addr).c_str());
    ::remove(getMonitorExpt(addr).c_str());
    ::remove(getMonitorBals(addr).c_str());
    ::remove(getMonitorDels(addr).c_str());
}

//---------------------------------------------------------------------------
void cleanMonitors(const CAddressArray& addrs) {
    for (auto addr : addrs)
        cleanMonitor(addr);
}

//----------------------------------------------------------------
void establishTestMonitors(void) {
    string_q loc = getCWD() + "./app_tests/";
    if (!folderExists(loc)) {
        cerr << "apps test files not found at: " << loc << endl;
        exit(0);
    }

    ostringstream os;
    os << "cp -p " << loc << "app_tests.tar.gz " << getMonitorPath("") << " && ";
    os << "cd " << getMonitorPath("") << " && ";
    os << "gunzip *.gz 2>/dev/null && ";
    os << "tar -xvf *.tar 2>/dev/null && ";
    os << "rm -f *.tar && ";
    os << "cd - 2>&1 1>/dev/null";
    // clang-format off
    if (system(os.str().c_str())) {}  // Don't remove cruft. Silences compiler warnings
    // clang-format on
}

//----------------------------------------------------------------
void establishMonitorFolders(void) {
    establishFolder(getMonitorPath("", FM_PRODUCTION));
    establishFolder(getMonitorPath("", FM_STAGING));
}

//----------------------------------------------------------------
void cleanMonitorStage(void) {
    cleanFolder(getMonitorPath("", FM_STAGING));
}

//-----------------------------------------------------------------------
bool CMonitor::loadMonitor(CAppearanceArray& items) {
    ENTER("loadMonitor");

    string_q fn = getMonitorPath(address);
    size_t nRecords = (fileSize(fn) / sizeof(CAppearance_base));
    if (!nRecords)
        EXIT_MSG("No records found for address '" + address + "'.", true);

    CAppearance_base* buffer = new CAppearance_base[nRecords];
    if (buffer) {
        bzero(buffer, nRecords * sizeof(CAppearance_base));
        CArchive txCache(READING_ARCHIVE);
        if (txCache.Lock(fn, modeReadOnly, LOCK_NOWAIT)) {
            txCache.Read(buffer, sizeof(CAppearance_base), nRecords);
            txCache.Release();
        } else {
            EXIT_FAIL("Could not open cache file'" + fn + "'. Quitting...\n");
        }
        // Expand the apps array (which may be non-empty)
        items.reserve(items.size() + nRecords);
        for (size_t i = 0; i < nRecords; i++) {
            CAppearance app;
            app.bn = buffer[i].blk;
            app.tx = buffer[i].txid;
            items.push_back(app);
        }
        delete[] buffer;
    } else {
        EXIT_FAIL("Could not allocate memory for address " + address + "Quitting...\n");
    }

    // Sort them, so when we write them later we can remove dups
    sort(items.begin(), items.end());

    EXIT_NOMSG(true);
}

//-----------------------------------------------------------------------
uint64_t CMonitor::getRecordCount(void) const {
    return fileSize(getMonitorPath(address)) / sizeof(CAppearance_base);
}

//-----------------------------------------------------------------------
bool CMonitor::exists(void) const {
    if (fileExists(getMonitorPath(address)))
        return true;
    if (fileExists(getMonitorLast(address)))
        return true;
    if (fileExists(getMonitorExpt(address)))
        return true;
    if (fileExists(getMonitorBals(address)))
        return true;
    if (fileExists(getMonitorDels(address)))
        return true;
    return false;
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
bool CMonitor::isDeleted(void) const {
    return fileExists(getMonitorDels(address));
}

//-----------------------------------------------------------------------
void CMonitor::undeleteMonitor(void) {
    ::remove(getMonitorDels(address).c_str());
}

//-----------------------------------------------------------------------
void CMonitor::deleteMonitor(void) {
    stringToAsciiFile(getMonitorDels(address), Now().Format(FMT_EXPORT));
}
// EXISTING_CODE
}  // namespace qblocks
