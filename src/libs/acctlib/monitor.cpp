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
// TODO: BOGUS - CLEAN THIS UP
//-------------------------------------------------------------------------
size_t CMonitor::getRecordCnt(const string_q& path) const {
    size_t sz = ::fileSize(path);
    if (!sz)
        return 0;
    size_t s2 = sizeof(CAppearance_mon);
    return (sz / s2) - 1;
}

//---------------------------------------------------------------------------
string_q CMonitor::getPathToMonitor(const address_t& addr, bool staging) const {
    string_q fn = isAddress(addr) ? addr + ".mon.bin" : addr;
    string_q base = cacheFolder_monitors + (staging ? "staging/" : "");
    return base + fn;
}

//--------------------------------------------------------------------------------
bool CMonitor::readHeader(CMonitorHeader& header) const {
    string_q newFilename = getPathToMonitor(address, isStaging);
    if (fileExists(newFilename)) {
        if (header.lastScanned > 0) {
            CArchive archive(READING_ARCHIVE);
            if (archive.Lock(newFilename, modeReadOnly, LOCK_NOWAIT)) {
                archive.Read(&header, sizeof(CMonitorHeader), 1);
                archive.Release();
            }
        }
    }
    return true;
}

//----------------------------------------------------------------
bool CMonitor::readAppearances(MONAPPFUNC func, void* data) {
    string_q path = getPathToMonitor(address, false);
    blknum_t nRecs = this->getRecordCnt(path);
    if (!nRecs) {
        return true;
    }

    CAppearance_mon* buffer = new CAppearance_mon[nRecs];
    if (!buffer) {
        LOG_ERR("Could not allocate buffer for address ", address);
        return false;
    }

    bzero((void*)buffer, nRecs * sizeof(CAppearance_mon));  // NOLINT
    CArchive archiveIn(READING_ARCHIVE);
    if (!archiveIn.Lock(path, modeReadOnly, LOCK_NOWAIT)) {
        archiveIn.Release();
        delete[] buffer;
        LOG_ERR("Could not lock file ", address);
        return false;
    }

    CMonitorHeader header;
    archiveIn.Read(&header, sizeof(CMonitorHeader), 1);
    archiveIn.Read(buffer, sizeof(CAppearance_mon), nRecs);
    archiveIn.Release();

    apps.reserve(apps.size() + nRecs);
    for (size_t i = 0; i < nRecs; i++) {
        apps.push_back(buffer[i]);
        if (func && !(*func)(buffer[i], data)) {
            sort(apps.begin(), apps.end());
            LOG4("forEvery func returns false for address ", address);
            return false;
        }
    }
    // TODO: BOGUS - DO WE NEED THIS?
    sort(apps.begin(), apps.end());

    delete[] buffer;
    return true;
}

//----------------------------------------------------------------
// TODO: BOGUS - Do this in the golang code
void establishMonitorFolders(void) {
    CMonitor m;
    establishFolder(m.getPathToMonitor("", false));
    establishFolder(m.getPathToMonitor("", true));
}

//----------------------------------------------------------------
// TODO: BOGUS - Do this in the golang code
void cleanMonitorStage(void) {
    CMonitor m;
    cleanFolder(m.getPathToMonitor("", true));
}

//----------------------------------------------------------------
bool isMonitorFilePath(const string_q& path) {
    return endsWith(path, "mon.bin");
}
// EXISTING_CODE
}  // namespace qblocks
