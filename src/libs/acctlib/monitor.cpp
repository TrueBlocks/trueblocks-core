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
extern string_q nextMonitorChunk(const string_q& fieldIn, const void* dataPtr);
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
            if (fieldName % "lastVisitedBlock") {
                return uint_2_Str(lastVisitedBlock);
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
            if (fieldName % "lastVisitedBlock") {
                lastVisitedBlock = str_2_Uint(fieldValue);
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
    // archive >> lastVisitedBlock;
    // archive >> sizeInBytes;
    // EXISTING_CODE
    // archive >> apps;
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMonitor::SerializeC(CArchive& archive) const {
    // Writing always writes the latest version of the data
    CAccountName::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    // archive << nAppearances;
    // archive << lastExport;
    // archive << firstAppearance;
    // archive << latestAppearance;
    // archive << lastVisitedBlock;
    // archive << sizeInBytes;
    // EXISTING_CODE
    // archive << apps;
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMonitor::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CMonitor copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
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
    ADD_FIELD(CMonitor, "lastVisitedBlock", T_BLOCKNUM, ++fieldNum);
    HIDE_FIELD(CMonitor, "lastVisitedBlock");
    ADD_FIELD(CMonitor, "sizeInBytes", T_UNUMBER | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMonitor, "sizeInBytes");

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CMonitor, "schema");
    HIDE_FIELD(CMonitor, "deleted");
    HIDE_FIELD(CMonitor, "showing");
    HIDE_FIELD(CMonitor, "cname");

    builtIns.push_back(_biCMonitor);

    // EXISTING_CODE
    ADD_FIELD(CMonitor, "apps", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);
    HIDE_FIELD(CMonitor, "apps");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextMonitorChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CMonitor* mon = reinterpret_cast<const CMonitor*>(dataPtr);
    if (mon) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if (fieldIn % "apps" || fieldIn % "appsCnt") {
                    size_t cnt = mon->apps.size();
                    if (endsWith(toLower(fieldIn), "cnt"))
                        return uint_2_Str(cnt);
                    if (!cnt)
                        return "";
                    string_q retS;
                    for (size_t i = 0; i < cnt; i++) {
                        // retS += mon->apps[i].Format();
                        retS += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return retS;
                }
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

// EXISTING_CODE
// EXISTING_CODE

//---------------------------------------------------------------------------
bool CMonitor::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CMonitor& mon) {
    mon.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CMonitor& mon) {
    mon.Serialize(archive);
    return archive;
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
    // EXISTING_CODE
    if (fieldName % "apps") {
        if (index == NOPOS) {
            CAppearance_mon empty;
            ((CMonitor*)this)->apps.push_back(empty);  // NOLINT
            index = apps.size() - 1;
        }
        // if (index < apps.size())
        //     return &apps[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_MONITOR =
    "[{NAPPS}]\t"
    "[{FIRSTAPP}]\t"
    "[{LATESTAPP}]\t"
    "[{SIZEINBYTES}]\t"
    "[{TAGS}]\t"
    "[{ADDRESS}]\t"
    "[{NAME}]\t"
    "[{ISCUSTOM}]\t"
    "[{DELETED}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
size_t CMonitor::getFileSize(const string_q& path) const {
    return ::fileSize(path);
}

//-------------------------------------------------------------------------
size_t CMonitor::getRecordCnt(const string_q& path) const {
    return ::fileSize(path) / sizeof(CAppearance_mon);
}

//-------------------------------------------------------------------------
bool CMonitor::openForWriting(bool staging) {
    if (tx_cache != NULL)
        return true;
    tx_cache = new CArchive(WRITING_ARCHIVE);
    if (tx_cache == NULL)
        return false;
    return tx_cache->Lock(getPathToMonitor(address, staging), modeWriteAppend, LOCK_WAIT);
}

//-------------------------------------------------------------------------
void CMonitor::writeMonitorArray(const CAppearanceArray_mon& items) {
    if (tx_cache == NULL)
        return;
    for (auto item : items)
        *tx_cache << item.blk << item.txid;
    tx_cache->flush();
}

//---------------------------------------------------------------------------
string_q CMonitor::getPathToMonitor(const address_t& addr, bool staging) const {
    string_q fn = isAddress(addr) ? addr + ".acct.bin" : addr;
    string_q base = cacheFolder_monitors + (staging ? "staging/" : "");
    if (isTestMode())
        base = chainConfigsFolder_mocked + "monitors/" + (staging ? "staging/" : "");
    return base + fn;
}

//---------------------------------------------------------------------------
string_q CMonitor::getPathToMonitorDels(const address_t& addr) const {
    return getPathToMonitor(addr, false) + ".deleted";
}

//---------------------------------------------------------------------------
string_q CMonitor::getPathToMonitorLast(const address_t& addr, bool staging) const {
    return substitute(getPathToMonitor(addr, staging), ".acct.bin", ".last.txt");
}

//-------------------------------------------------------------------------
void CMonitor::writeLastBlockInMonitor(blknum_t bn, bool staging) {
    lastVisitedBlock = bn;
    stringToAsciiFile(getPathToMonitorLast(address, staging), uint_2_Str(bn) + "\n");
}

//-----------------------------------------------------------------------
blknum_t CMonitor::getLastBlockInMonitorPlusOne(void) const {
    return str_2_Uint(asciiFileToString(getPathToMonitorLast(address, false)));
}

//--------------------------------------------------------------------------------
blknum_t CMonitor::getNextBlockToVisit(bool fresh) const {
    if (lastVisitedBlock == NOPOS || fresh) {
        // If the monitor exists, the next block is stored in the database...
        if (fileExists(getPathToMonitorLast(address, false))) {
            ((CMonitor*)this)->lastVisitedBlock =
                str_2_Uint(asciiFileToString(getPathToMonitorLast(address, false)));  // NOLINT

        } else {
            // Accounts can receive ETH counter-factually. By default, we ignore
            // this and start our scan from the account's deploy block (in the case
            // of a contract) or the zero block. User can change this setting.
            if (getGlobalConfig("acctExport")->getConfigBool("settings", "start_when_deployed", true)) {
                blknum_t deployed = getDeployBlock(address);
                ((CMonitor*)this)->lastVisitedBlock = (deployed == NOPOS ? 0 : deployed);  // NOLINT
            }
        }
    }
    return lastVisitedBlock;
}

//--------------------------------------------------------------------------------
#define checkLock(fn, b)                                                                                               \
    if (fileExists((fn) + ".lck")) {                                                                                   \
        msg = ("The " + string_q(b) + " file for monitor " + address + " is locked. Quitting...");                     \
        return true;                                                                                                   \
    }

//-----------------------------------------------------------------------
bool CMonitor::monitorExists(void) const {
    if (fileExists(getPathToMonitor(address, false)))
        return true;
    if (fileExists(getPathToMonitorLast(address, false)))
        return true;
    if (fileExists(getPathToMonitorDels(address)))
        return true;
    return false;
}

//--------------------------------------------------------------------------------
bool CMonitor::isMonitorLocked(string_q& msg) const {
    checkLock(getPathToMonitor(address, false), "cache");
    checkLock(getPathToMonitorLast(address, false), "last block");
    checkLock(getPathToMonitorDels(address), "marker");
    return false;
}

//--------------------------------------------------------------------------------
bool CMonitor::clearMonitorLocks(void) {
    ::remove((getPathToMonitor(address, false) + ".lck").c_str());
    ::remove((getPathToMonitorLast(address, false) + ".lck").c_str());
    ::remove((getPathToMonitorDels(address) + ".lck").c_str());
    return true;
}

//--------------------------------------------------------------------------------
void doMoveFile(const string_q& from, const string_q& to) {
#define CLEAN(a) (cTeal + (isTestMode() ? substitute((a), cacheFolder, "$CACHE/") : (a)) + cOff)
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
    bool binExists = fileExists(getPathToMonitor(address, true));
    bool lastExists = fileExists(getPathToMonitorLast(address, true));
    lockSection();
    if (binExists || lastExists) {
        doMoveFile(getPathToMonitor(address, true), getPathToMonitor(address, false));
        doMoveFile(getPathToMonitorLast(address, true), getPathToMonitorLast(address, false));
    } else {
        // For some reason (user quit, UI switched to adding a different address to monitor, something went
        // wrong...) the binary cache was not created. Cleanup everything. The user will have to start over.
        ::remove(getPathToMonitor(address, true).c_str());
        ::remove(getPathToMonitorLast(address, true).c_str());
    }
    unlockSection();
}

//-----------------------------------------------------------------------
bool CMonitor::isDeleted(void) const {
    return fileExists(getPathToMonitorDels(address));
}

//-----------------------------------------------------------------------
void CMonitor::deleteMonitor(void) {
    stringToAsciiFile(getPathToMonitorDels(address), Now().Format(FMT_EXPORT));
}

//-----------------------------------------------------------------------
void CMonitor::undeleteMonitor(void) {
    ::remove(getPathToMonitorDels(address).c_str());
}

//---------------------------------------------------------------------------
void removeFile(const string_q& fn) {
    ::remove(fn.c_str());
    ::remove((fn + ".lck").c_str());
}

//-----------------------------------------------------------------------
void CMonitor::removeMonitor(void) {
    removeFile(getPathToMonitor(address, false));
    removeFile(getPathToMonitorLast(address, false));
    removeFile(getPathToMonitorDels(address));
    // TODO(tjayrush): remove reconciliations
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
blknum_t CMonitor::loadAppearances(MONAPPFUNC func, void* data) {
    string_q path = getPathToMonitor(address, false);
    blknum_t nRecs = this->getRecordCnt(path);
    if (!nRecs)
        return false;

    CAppearance_mon* buffer = new CAppearance_mon[nRecs];
    if (!buffer)
        return false;

    bzero((void*)buffer, nRecs * sizeof(CAppearance_mon));  // NOLINT
    CArchive archiveIn(READING_ARCHIVE);
    if (!archiveIn.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        archiveIn.Release();
        delete[] buffer;
        return false;
    }
    archiveIn.Read(buffer, sizeof(CAppearance_mon), nRecs);
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
    establishFolder(m.getPathToMonitor("", false));
    establishFolder(m.getPathToMonitor("", true));
}

//----------------------------------------------------------------
void cleanMonitorStage(void) {
    CMonitor m;
    cleanFolder(m.getPathToMonitor("", true));
}

//----------------------------------------------------------------
address_t path_2_Addr(const string_q& path) {
    if (!endsWith(path, ".acct.bin"))
        return "";
    CStringArray parts;
    explode(parts, path, '/');
    return substitute(parts[parts.size() - 1], ".acct.bin", "");
}
// EXISTING_CODE
}  // namespace qblocks
