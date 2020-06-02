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
        toJson(ctx);
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
                CFunction item;
                string_q str = fieldValue;
                while (item.parseJson3(str)) {
                    interfaces.push_back(item);
                    item = CFunction();  // reset
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
    ADD_FIELD(CAbi, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAbi, "interfaces", T_OBJECT | TS_ARRAY, ++fieldNum);

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
ostream& operator<<(ostream& os, const CAbi& item) {
    // EXISTING_CODE
    if (sizeof(item) != 0) {  // always true, but we do this to avoid a warning
        for (auto interface : item.interfaces) {
            os << interface.Format() << "\n";
        }
        return os;
    }
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CAbi::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "interfaces" && index < interfaces.size())
        return &interfaces[index];
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
//---------------------------------------------------------------------------
bool visitBuiltin(const qblocks::string_q& path, void* data) {
    if (!endsWith(path, ".json"))  // we only want to look at jsons (the source)
        return true;
    CAbi* abi = (CAbi*)data;  // NOLINT
    if (!abi->loadAbiFromFile(path, true))
        return false;
    return true;
}

//---------------------------------------------------------------------------
bool visitABI(const qblocks::string_q& path, void* data) {
    if (!endsWith(path, ".json"))  // we only want to look at jsons (the source)
        return true;
    CAbi* abi = (CAbi*)data;  // NOLINT
    if (!abi->loadAbiFromFile(path, false))
        return false;
    return true;
}

//---------------------------------------------------------------------------
bool CAbi::loadAndCacheAbiFolder(const string_q& path, const string_q& name) {
    string_q binPath = path + name + ".bin";
    fileInfo info = getNewestFileInFolder(path);
    if (info.fileName == binPath && fileExists(binPath)) {
        // cerr << "Reading " << name << " abis from cache" << endl;
        CArchive archive(READING_ARCHIVE);
        if (archive.Lock(binPath, modeReadOnly, LOCK_NOWAIT)) {
            archive >> *this;
            archive.Release();
            return true;
        }
    }

    if (!forEveryFileInFolder(path + "*", visitABI, this))
        return false;

    sort(interfaces.begin(), interfaces.end());

    CArchive archive(WRITING_ARCHIVE);
    if (archive.Lock(binPath, modeWriteCreate, LOCK_NOWAIT)) {
        archive << *this;
        archive.Release();
        return true;
    }

    return true;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiKnown(void) {
    return loadAndCacheAbiFolder(configPath("known_abis/"), "known");
}

//---------------------------------------------------------------------------
bool CAbi::loadAbisMonitors(void) {
    return loadAndCacheAbiFolder(getCachePath("abis/"), "monitored");
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiByAddress(const address_t& which) {
    if (isZeroAddr(which))
        return false;
    bool ret = visitABI(getCachePath("abis/" + toLower(which) + ".json"), this);
    if (ret)
        sort(interfaces.begin(), interfaces.end());
    return ret;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiFromFile(const string_q& fileName, bool builtIn) {
    if (!fileExists(fileName))
        return false;

    //    string_q binFile = substitute(fileName, ".json", ".bin");
    //    if (fileExists(binFile)) {
    //        CArchive archive(READING_ARCHIVE);
    //        if (archive.Lock(binFile, modeReadOnly, LOCK_NOWAIT)) {
    //            archive >> *this;
    //            archive.Release();
    //            return true;
    //        }
    //    }

    string_q contents;
    asciiFileToString(fileName, contents);
    bool ret = loadAbiFromString(contents, builtIn);
    if (ret) {
        string_q addr = substitute(
            substitute(substitute(fileName, configPath("known_abis/"), ""), getCachePath("abis/"), ""), ".json", "");
        for (auto i = interfaces.begin(); i != interfaces.end(); i++)
            i->address = addr;
        sort(interfaces.begin(), interfaces.end());
        //        CArchive archive(WRITING_ARCHIVE);
        //        if (archive.Lock(binFile, modeWriteCreate, LOCK_NOWAIT)) {
        //            archive << *this;
        //            archive.Release();
        //            return true;
        //        }
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiFromString(const string_q& in, bool builtIn) {
    string_q contents = in;
    CFunction func;
    while (func.parseJson3(contents)) {
        func.isBuiltIn = builtIn;
        if (!interfaceMap[func.encoding]) {
            interfaces.push_back(func);
            interfaceMap[func.encoding] = true;
        }
        func = CFunction();  // reset
    }
    return interfaces.size();
}

//-----------------------------------------------------------------------
void loadAbiAndCache(CAbi& abi, const address_t& addr, bool raw, CStringArray& errors) {
    if (isZeroAddr(addr))
        return;

    uint64_t saveVerbose = verbose;
    if (getGlobalConfig()->getConfigBool("dev", "debug_ethscan", false))
        verbose = 10;

    string_q results;
    string_q fileName = getCachePath("abis/" + addr + ".json");

    string_q localFile("./" + addr + ".json");
    if (fileExists(localFile) && localFile != fileName) {
        LOG4("Local file copied to cache");
        copyFile(localFile, fileName);
    }

    string_q dispName = substitute(fileName, getCachePath(""), "$BLOCK_CACHE/");
    if (fileExists(fileName) && !raw) {
        if (!isTestMode())
            LOG4("Reading ABI for address ", addr, " from ", (isTestMode() ? "--" : "cache"), "\r");
        asciiFileToString(fileName, results);

    } else {
        const char* STR_CONTRACT_API =
            "http://api.etherscan.io/api?module=contract&action=getabi&address=[{ADDRESS}]&apikey=[{KEY}]";
        if (!isTestMode())
            LOG4("Reading ABI for address ", addr, " from ", (isTestMode() ? "--" : "EtherScan"), "\r");
        string_q url = substitute(substitute(STR_CONTRACT_API, "[{ADDRESS}]", addr), "[{KEY}]",
                                  getApiKey("Etherscan",
                                            "http:/"
                                            "/api.etherscan.io/apis"));
        results = substitute(urlToString(url), "\\", "");

        if (!contains(results, "NOTOK")) {
            if (!isTestMode()) {
                LOG4(results);
                LOG4("Caching abi in ", dispName);
            }
            replace(results, "\"result\":\"", "<extract>");
            replaceReverse(results, "\"}", "</extract>");
            results = snagFieldClear(results, "extract", "");
            establishFolder(fileName);
            stringToAsciiFile(fileName, results);

        } else if (contains(toLower(results), "source code not verified")) {
            ostringstream os;
            os << "Could not get the ABI for " << addr << ". Etherscan returned: ";
            os << substitute(substitute(results, "\"", "'"), "\n", " ") << ". ";
            os << "Copy the ABI to " << addr << ".json in the current folder and re-run.";
            errors.push_back(os.str());
            results = "";

        } else {
            // TODO(tjayrush): If we store the ABI here even if empty, we won't have to get it again, but then
            // what happens if user later posts the ABI? Need a 'refresh' option or clear cache option

            ostringstream os;
            os << "Etherscan returned: " << results << ". ";
            os << "Could not grab ABI for " + addr + " from etherscan.io.";
            errors.push_back(os.str());

            establishFolder(fileName);
            stringToAsciiFile(fileName, "[]");
            results = "";
        }
    }

    if (!results.empty()) {
        CFunction func;
        while (func.parseJson3(results)) {
            abi.addIfUnique(addr, func, false);
            func = CFunction();  // reset
        }
    }

    verbose = saveVerbose;
    return;
}

//-----------------------------------------------------------------------
bool CAbi::addIfUnique(const string_q& addr, CFunction& func, bool decorateNames) {
    if (func.name.empty() && func.type != "constructor")
        return false;

    for (auto f : interfaces) {
        if (f.encoding == func.encoding)
            return false;

        // different encoding same name means a duplicate function name in the code. We won't build with
        // duplicate function names, so we need to modify the incoming function. We do this by appending
        // the first four characters of the contract's address.
        if (decorateNames && f.name == func.name && !f.isBuiltIn) {
            func.origName = func.name;
            func.name += (startsWith(addr, "0x") ? extract(addr, 2, 4) : extract(addr, 0, 4));
        }
    }

    interfaces.push_back(func);
    return true;
}

//-----------------------------------------------------------------------
void removeDuplicateEncodings(CAbiArray& abis) {
    if (abis.size() == 0 || abis.size() == 1)
        return;

    size_t j = 0;
    size_t n = abis.size();
    for (size_t i = 0; i < n - 1; i++) {
        if (abis[i] != abis[i + 1]) {
            abis[j++] = abis[i];
        }
    }
    abis[j++] = abis[n - 1];
}

// TODO(tjayrush): This is terrible code
extern void snagSignatures(string_q& str);
//-----------------------------------------------------------------------
bool sol_2_Abi(CAbi& abi, const string_q& addr) {
    string_q solFile = addr + ".sol";
    string_q contents = asciiFileToString(solFile);

    // remove any unneeded characters (all comments, unused whitespace
    simplifySolidity(contents);

    // prepare the code for identifying the functions and events
    replaceAll(contents, "function ", "~function ");
    replaceAll(contents, "event ", "~event ");
    // replaceAll(contents, "interface ", "contract ");
    replaceAll(contents, " memory ", " ");
    replaceAll(contents, " storage ", " ");
    replaceAll(contents, " calldata ", " ");
    cleanString(contents, true);

    // preserve only characters between '~' and the next following ';' (removing both '~'
    // and ';' and adding newline in place of the ';'
    snagSignatures(contents);

    CStringArray lines;
    explode(lines, contents, '\n');
    for (auto line : lines) {
        replaceAll(line, "  ", " ");
        CFunction func;
        func.fromDefinition(line);
        abi.interfaces.push_back(func);
    }
    return true;
}

//-----------------------------------------------------------------------
void snagSignatures(string_q& str) {
    size_t pos = 0;
    typedef enum { OUT, IN } StateThing;
    StateThing state = OUT;
    for (auto ch : str) {
        switch (state) {
            case OUT:
                if (ch == '~')
                    state = IN;
                break;
            case IN:
                str[pos++] = ch;
                if (ch == ';') {
                    str[pos++] = '\n';
                    state = OUT;
                }
                break;
        }
    }
    str[pos] = '\0';
    str.resize(pos);
}

//-----------------------------------------------------------------------
string_q getAbiPath(const address_t& addr) {
    string_q base = "abis/";
    if (!isAddress(addr))  // empty for example
        return getCachePath(base + addr);
    return getCachePath(base + addr + ".json");
}
// EXISTING_CODE
}  // namespace qblocks
