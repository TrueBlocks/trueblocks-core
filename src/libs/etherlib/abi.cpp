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
#include "abi.h"
#include "node.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAbi, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextAbiChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextAbiChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CAbi::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["abi_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextAbiChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAbiChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAbi*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CAbi::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextAbiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                return addr_2_Str(address);
            }
            break;
        case 'i':
            if (fieldName % "interfaces" || fieldName % "interfacesCnt") {
                size_t cnt = interfaces.size();
                if (endsWith(toLower(fieldName), "cnt"))
                    return uint_2_Str(cnt);
                if (!cnt)
                    return "";
                string_q retS;
                for (size_t i = 0; i < cnt; i++) {
                    retS += interfaces[i].Format();
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
bool CAbi::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    if (fieldName % "interfaces") {
        CFunction func;
        string_q str = fieldValue;
        while (func.parseJson3(str)) {
            loadAbiAddInterface(func);
            func = CFunction();  // reset
        }
        return true;
    }
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if (fieldName % "address") {
                address = str_2_Addr(fieldValue);
                return true;
            }
            break;
        case 'i':
            if (fieldName % "interfaces") {
                CFunction obj;
                string_q str = fieldValue;
                while (obj.parseJson3(str)) {
                    interfaces.push_back(obj);
                    obj = CFunction();  // reset
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
void CAbi::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAbi::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> address;
    archive >> interfaces;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbi::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << address;
    archive << interfaces;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAbi::Migrate(CArchive& archiveIn, CArchive& archiveOut) const {
    ASSERT(archiveIn.isReading());
    ASSERT(archiveOut.isWriting());
    CAbi copy;
    // EXISTING_CODE
    // EXISTING_CODE
    copy.Serialize(archiveIn);
    copy.SerializeC(archiveOut);
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAbiArray& array) {
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
CArchive& operator<<(CArchive& archive, const CAbiArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAbi::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAbi, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAbi, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAbi, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CAbi, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CAbi, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CAbi, "address", T_ADDRESS | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CAbi, "interfaces", T_OBJECT | TS_ARRAY | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAbi, "schema");
    HIDE_FIELD(CAbi, "deleted");
    HIDE_FIELD(CAbi, "showing");
    HIDE_FIELD(CAbi, "cname");

    builtIns.push_back(_biCAbi);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextAbiChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CAbi* abi = reinterpret_cast<const CAbi*>(dataPtr);
    if (abi) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, abi);
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
bool CAbi::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAbi& abi) {
    abi.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CAbi& abi) {
    abi.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAbi& it) {
    // EXISTING_CODE
    if (sizeof(it) != 0) {  // always true, but we do this to avoid a warning
        for (auto interface : it.interfaces) {
            os << interface.Format() << "\n";
        }
        return os;
    }
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CAbi::getObjectAt(const string_q& fieldName, size_t index) const {
    // EXISTING_CODE
    // EXISTING_CODE
    if (fieldName % "interfaces") {
        if (index == NOPOS) {
            CFunction empty;
            ((CAbi*)this)->interfaces.push_back(empty);  // NOLINT
            index = interfaces.size() - 1;
        }
        if (index < interfaces.size())
            return &interfaces[index];
    }
    // EXISTING_CODE
    // EXISTING_CODE

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_ABI =
    "[{ADDRESS}]\t"
    "[{ENCODING}]\t"
    "[{TYPE}]\t"
    "[{NAME}]\t"
    "[{SIGNATURE}]\t"
    "[{INPUT_NAMES}]\t"
    "[{OUTPUT_NAMES}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
//-----------------------------------------------------------------------
#undef LOG_TEST
#define LOG_TEST(a, b)

//---------------------------------------------------------------------------
bool loadAbiFile(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", loadAbiFile, data);
    } else {
        if (endsWith(path, ".json")) {
            CAbi* abi = (CAbi*)data;  // NOLINT
            if (!abi->loadAbiFromFile(path))
                return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------
bool loadAbiString(const string_q& jsonStr, CAbi& abi) {
    bool ret = abi.loadAbiFromString(jsonStr);
    if (ret)
        sort(abi.interfaces.begin(), abi.interfaces.end(), sortByFuncName);
    return ret;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbisFromKnown(bool tokensOnly) {
    if (tokensOnly) {
        bool ret1 = loadAbiFromFile(configPath("abis/known-000/erc_00020.json"));
        bool ret2 = loadAbiFromFile(configPath("abis/known-000/erc_00721.json"));
        return (ret1 && ret2);
    }

    string_q srcPath = configPath("abis/");
    if (abiSourcesMap[srcPath])
        return true;

    string_q binPath = getCachePath("abis/known.bin");
    if (fileExists(binPath)) {
        // If any file is newer, don't use binary cache
        fileInfo info = getNewestFileInFolder(srcPath);
        if (info.fileName == binPath || fileLastModifyDate(binPath) > info.fileTime) {
            abiInterfacesMap.clear();
            CArchive archive(READING_ARCHIVE);
            if (archive.Lock(binPath, modeReadOnly, LOCK_NOWAIT)) {
                archive >> *this;
                archive.Release();
                LOG_TEST("Loaded " + uint_2_Str(interfaces.size()) + " interfaces from",
                         substitute(substitute(binPath, getCachePath(""), "$CACHE/"), configPath(""), "$CONFIG/"));
                for (auto func : interfaces) {
                    abiInterfacesMap[func.encoding] = true;
                    LOG_TEST("Inserting", func.type + "-" + func.signature);
                }
                abiSourcesMap[srcPath] = true;
                return true;
            }
        }
    }

    LOG4("Freshening abi cache for path: ", srcPath);
    if (!forEveryFileInFolder(srcPath + "*", loadAbiFile, this))
        return false;

    sort(interfaces.begin(), interfaces.end(), sortByFuncName);
    abiSourcesMap[srcPath] = true;

    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(binPath, modeWriteCreate, LOCK_NOWAIT)) {
        archive << *this;
        archive.Release();
        LOG_TEST("Saved " + uint_2_Str(interfaces.size()) + " interfaces in",
                 substitute(substitute(binPath, getCachePath(""), "$CACHE/"), configPath(""), "$CONFIG/"));
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiFromAddress(const address_t& addr, bool recurse) {
    ASSERT(!isZeroAddr(addr));

    string_q fileName = getCachePath("abis/" + addr + ".json");
    string_q localFile = (getCWD() + addr + ".json");
    if (fileExists(localFile) && localFile != fileName) {
        LOG_TEST("Local file copied to cache", "./" + addr + ".json");
        copyFile(localFile, fileName);
    }

    if (!loadAbiFromFile(fileName))
        return false;
    abiSourcesMap[addr] = true;

    // if (abiInterfacesMap["0x59679b0f"]) {
    //     cerr << addr << endl;
    //     // This is a proxy. Load the ABI from the proxied-to address as well
    //     CEthCall theCall;
    //     theCall.address = addr;
    //     theCall.encoding = "0x59679b0f";  // implementation()
    //     theCall.bytes = "";
    //     theCall.blockNumber = getBlockProgress(BP_CLIENT).client;
    //     theCall.checkProxy = false;
    //     if (doEthCall(theCall))
    //         loadAbiFromAddress(theCall.getResult(), false);
    // }

    return true;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiFromFile(const string_q& fileName) {
    if (abiSourcesMap[fileName])
        return true;

    if (!fileExists(fileName)) {
        LOG_TEST("loadAbiFromFile", "Could not load file " + dispName(fileName));
        return false;
    }
    LOG_TEST("loadAbiFromFile", dispName(fileName));

    if (loadAbiFromString(asciiFileToString(fileName))) {
        if (interfaces.size() && interfaces[0].abi_source.empty()) {
            for (auto& interface : interfaces)
                if (interface.abi_source.empty()) {
                    string_q str = substitute(substitute(fileName, getCachePath("abis/"), ""), configPath("abis/"), "");
                    nextTokenClear(str, '/');
                    interface.abi_source = str;
                }
        }
        sort(interfaces.begin(), interfaces.end(), sortByFuncName);
        abiSourcesMap[fileName] = true;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiFromString(const string_q& in) {
    string_q contents = in;
    CFunction func;
    while (func.parseJson3(contents)) {
        loadAbiAddInterface(func);
        func = CFunction();  // reset
    }
    return interfaces.size();
}

//-----------------------------------------------------------------------
bool CAbi::loadAbiFromEtherscan(const address_t& addr) {
    if (isZeroAddr(addr))
        return true;

    if (abiSourcesMap[addr])
        return true;

    if (loadAbiFromAddress(addr, true))
        return true;

    // If this isn't a smart contract, don't bother
    if (!isContractAt(addr, getBlockProgress(BP_CLIENT).client)) {
        abiSourcesMap[addr] = true;
        return true;
    }

    if (loadAbisOneKnown(addr)) {
        abiSourcesMap[addr] = true;
        return true;
    }

    const char* STR_CONTRACT_API =
        "http://api.etherscan.io/api?module=contract&action=getabi&address=[{ADDRESS}]&apikey=[{KEY}]";
    if (!isTestMode())
        LOG4("Reading ABI for address ", addr, " from ", (isTestMode() ? "--" : "EtherScan"), "\r");
    string_q url = substitute(substitute(STR_CONTRACT_API, "[{ADDRESS}]", addr), "[{KEY}]",
                              getApiKey("Etherscan",
                                        "http:/"
                                        "/api.etherscan.io/apis"));
    string_q fromES = urlToString(url);

    // some of etherscan's data kind of sucks, so clean it
    replaceAll(fromES, "\\r", "\r");
    replaceAll(fromES, "\\n", "\n");

    string_q fileName = getCachePath("abis/" + addr + ".json");
    establishFolder(fileName);

    string_q results = substitute(fromES, "\\", "");
    if (!contains(results, "NOTOK")) {
        LOG_TEST("loadAbiFromEtherscan", "for address " + addr);

        replace(results, "\"result\":\"", "<extract>");
        replaceReverse(results, "\"}", "</extract>");
        results = snagFieldClear(results, "extract", "");

        stringToAsciiFile(fileName, results);
        return loadAbiFromAddress(addr, true);
    }

    // We write an empty file to the abi cache even though at some point in the future
    // this ABI may be published to EtherScan.
    stringToAsciiFile(fileName, "");
    abiSourcesMap[addr] = true;
    return false;
}

//-----------------------------------------------------------------------
void CAbi::loadAbiAddInterface(const CFunction& func) {
    if (func.name.empty() && func.type != "constructor")
        return;

    // first in wins
    if (abiInterfacesMap[func.encoding]) {
        LOG_TEST("Skipping", func.type + "-" + func.signature);
        return;
    }

    LOG_TEST("Inserting", func.type + "-" + func.signature);
    if (func.type != "constructor") {
        interfaces.push_back(func);
        abiInterfacesMap[func.encoding] = true;
    }
}

//---------------------------------------------------------------------------
size_t CAbi::nFunctions(void) const {
    size_t cnt = 0;
    for (auto i : interfaces)
        if (i.type == "function")
            cnt++;
    return cnt;
}

//---------------------------------------------------------------------------
size_t CAbi::nEvents(void) const {
    size_t cnt = 0;
    for (auto i : interfaces)
        if (i.type == "event")
            cnt++;
    return cnt;
}

//---------------------------------------------------------------------------
size_t CAbi::nOther(void) const {
    return interfaces.size() - nFunctions() - nEvents();
}

//-----------------------------------------------------------------------
bool sortByFuncName(const CFunction& f1, const CFunction& f2) {
    string_q name1 = toLower(f1.name);
    while (startsWith(name1, '_'))
        replace(name1, "_", "");
    string_q s1 = (f1.type == "event" ? "zzzevent" : f1.type) + name1 + f1.encoding;
    for (auto f : f1.inputs)
        s1 += f.name;

    string_q name2 = toLower(f2.name);
    while (startsWith(name2, '_'))
        replace(name2, "_", "");
    string_q s2 = (f2.type == "event" ? "zzzevent" : f2.type) + name2 + f2.encoding;
    for (auto f : f2.inputs)
        s2 += f.name;
    return s1 < s2;
}

//-----------------------------------------------------------------------
bool isKnownAbi(const string_q& addr, string_q& path) {
    path = configPath("abis/known-000/" + addr + ".json");
    if (fileExists(path))
        return true;
    path = configPath("abis/known-005/" + addr + ".json");
    if (fileExists(path))
        return true;
    path = configPath("abis/known-010/" + addr + ".json");
    if (fileExists(path))
        return true;
    path = configPath("abis/known-015/" + addr + ".json");
    if (fileExists(path))
        return true;
    path = "";
    return false;
}

//-----------------------------------------------------------------------
bool CAbi::loadAbisKnown(const string_q& addr) {
    string_q path;
    if (!isKnownAbi(addr, path))
        return false;
    loadAbiFromFile(path);
    return true;
}
// EXISTING_CODE
}  // namespace qblocks
