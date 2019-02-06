
/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
static string_q nextAbiChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextAbiChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAbi::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextAbiChunk, this);
}

//---------------------------------------------------------------------------
string_q nextAbiChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CAbi *>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAbi::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) { address = str_2_Addr(fieldValue); return true; }
            break;
        case 'i':
            if ( fieldName % "interfaces" ) {
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
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CAbiArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CAbi::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CAbi, "schema")) return;

    size_t fieldNum = 1000;
    ADD_FIELD(CAbi, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAbi, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAbi, "cname", T_TEXT,  ++fieldNum);
    ADD_FIELD(CAbi, "address", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAbi, "interfaces", T_OBJECT|TS_ARRAY, ++fieldNum);

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
string_q nextAbiChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CAbi *abi = reinterpret_cast<const CAbi *>(dataPtr);
    if (abi) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
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

//---------------------------------------------------------------------------
string_q CAbi::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextAbiChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "address" ) return addr_2_Str(address);
            break;
        case 'i':
            if ( fieldName % "interfaces" || fieldName % "interfacesCnt" ) {
                size_t cnt = interfaces.size();
                if (endsWith(fieldName, "Cnt"))
                    return uint_2_Str(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += interfaces[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
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
const CBaseNode *CAbi::getObjectAt(const string_q& fieldName, size_t index) const {
    if ( fieldName % "interfaces" && index < interfaces.size() )
        return &interfaces[index];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
//---------------------------------------------------------------------------
bool visitABI(const qblocks::string_q& path, void *data) {
    if (!endsWith(path, ".json"))
        return true;
    CAbi *abi = (CAbi*)data;
    if (!abi->loadAbiFromFile(path, true))
        return false;
    return true;
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiKnown(const string_q& which) {
    if (which == "all")
        return forEveryFileInFolder(configPath("known_abis/""*"), visitABI, this);
    return loadAbiFromFile(configPath("known_abis/" + which + ".json"), true);
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiByAddress(address_t addrIn) {
    if (isZeroAddr(addrIn))
        return false;
    string_q addr = toLower(addrIn);
    string_q fileName = blockCachePath("abis/" + addr + ".json");
    return loadAbiFromFile(fileName, false);
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiFromFile(const string_q& fileName, bool builtIn) {
    if (!fileExists(fileName))
        return false;
    string_q contents;
    asciiFileToString(fileName, contents);
    return loadAbiFromString(contents, builtIn);
}

//---------------------------------------------------------------------------
bool CAbi::loadAbiFromString(const string_q& in, bool builtIn) {
    string_q contents = in;
    CFunction func;
    while (func.parseJson3(contents)) {
        func.isBuiltIn = builtIn;
        interfaces.push_back(func);
        func = CFunction();  // reset
    }
    sort(interfaces.begin(), interfaces.end());
    return interfaces.size();
}

//-----------------------------------------------------------------------
bool CAbi::addIfUnique(const string_q& addr, CFunction& func, bool decorateNames) {
    if (func.name.empty())  // && func.type != "constructor")
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
bool CAbi::loadAbiAndCache(const address_t& addr, bool raw, bool silent, bool decNames) {

    if (isZeroAddr(addr))
        return false;

    string_q results;
    string_q fileName = blockCachePath("abis/" + addr + ".json");

    string_q localFile("./" + addr + ".json");
    if (fileExists(localFile) && localFile != fileName) {
        cerr << "Local file copied to cache\n";
        copyFile(localFile, fileName);
    }

    string_q dispName = substitute(fileName, blockCachePath(""), "$CACHE/");
    if (fileExists(fileName) && !raw) {

        if (verbose) {
            cerr << "Reading ABI for address " << addr << " from " << (isTestMode() ? "--" : "cache") << "\r";
            cerr.flush();
        }
        asciiFileToString(fileName, results);

    } else {
        if (verbose) {
            cerr << "Reading ABI for address " << addr << " from " << (isTestMode() ? "--" : "EtherScan") << "\r";
            cerr.flush();
        }
        string_q url = string_q("http:/")
                                + "/api.etherscan.io/api?module=contract&action=getabi&address="
                                + addr;
        results = substitute(urlToString(url), "\\", "");
        if (!contains(results, "NOTOK")) {

            // Clear the RPC wrapper
            if (!isTestMode()) {
                if (verbose)
                    cerr << results << endl;
                cerr << "Caching abi in " << dispName << endl;
            }
            replace(results, "\"result\":\"", "<extract>");
            replaceReverse(results, "\"}", "</extract>");
            results = snagFieldClear(results, "extract", "");
            establishFolder(fileName);
            stringToAsciiFile(fileName, results);

        } else if (contains(toLower(results), "source code not verified")) {

            if (!silent) {
                if (isContractAt(addr)) {
                    cerr << "\n";
                    cerr << cRed << "Warning: " << cOff;
                    cerr << "Failed to grab the ABI. Etherscan returned:\n\n\t";
                    cerr << cTeal << results << cOff << "\n\n";
                    cerr << "However, the ABI may actually be present on EtherScan. QBlocks will use it if\n";
                    cerr << "you copy and paste the ABI json to this file:\n\n\t";
                    cerr << cTeal << localFile << cOff << "\n\n";
                    quickQuitHandler(0);
                } else {
                    cerr << "\n";
                    cerr << cRed << "Warning: " << cOff;
                    cerr << "The address your specified (" << addr << ") does not\n";
                    cerr << "\t does not appear to be a smart contract, therefore there is no ABI. Quitting...\n\n";
                    quickQuitHandler(0);
                }
            }

        } else {

            if (!silent) {
                cerr << "Etherscan returned " << results << "\n";
                cerr << "Could not grab ABI for " + addr + " from etherscan.io.\n";
                quickQuitHandler(0);
            }

            // TODO(tjayrush): If we store the ABI here even if empty, we won't have to get it again, but then
            // what happens if user later posts the ABI? Need a 'refresh' option or clear cache option
            establishFolder(fileName);
            stringToAsciiFile(fileName, "[]");
        }
    }

    CFunction func;
    ostringstream os;
    while (func.parseJson3(results)) {
        if (addIfUnique(addr, func, decNames))
            func.doExport(os);
        func = CFunction();  // reset
    }
    return !os.str().empty();
}

//----------------------------------------------------------------------------
inline unsigned char hex_2_Ascii(char *str) {
    unsigned char c;
    c =  (unsigned char)((str[0] >= 'A' ? ((str[0] & 0xDF) - 'A') + 10 : (str[0] - '0')));
    c *= 16;
    c = (unsigned char)(c + (str[1] >= 'A' ? ((str[1]&0xDF)-'A')+10 : (str[1]-'0')));
    return c;
}

//----------------------------------------------------------------------------
inline string_q hex_2_Str(const string_q& inHex) {
    string_q ret, in = substitute((startsWith(inHex, "0x") ? extract(inHex, 2) : inHex), "2019", "27");
    while (!in.empty()) {
        string_q nibble = extract(in, 0, 2);
        in = extract(in, 2);
        char ch = (char)hex_2_Ascii((char*)nibble.c_str());  // NOLINT
        ret += (char)ch;
    }
    return ret;
}

//---------------------------------------------------------------------------
inline string_q getBaseType(const string_q& type) {
    return type.substr(0, type.find('['));
}

//---------------------------------------------------------------------------
inline void parseType(const string_q& type, string_q& baseType, uint64_t& size, uint64_t& count, uint64_t& nBits, uint64_t& remains) {
    baseType = getBaseType(type);
    count = 1; // not an array
    size = str_2_Uint(substitute(substitute(substitute(substitute(type,"uint",""),"int",""),"bytes",""),"address",""));
    if (contains(type, "[")) {
        string_q t = type;
        nextTokenClear(t, '[');
        count = str_2_Uint(t);
    }
    double ratio = (size / 256.);
    nBits = (uint64_t)(64 * ratio);
    remains = 64 - nBits;
}

static const bigint_t max256Int = str_2_BigInt("115792089237316195423570985008687907853269984665640564039457584007913129639935");
//---------------------------------------------------------------------------
size_t CParameter::parseFixedType(string_q& input) {

    size_t sizeIn = input.length();
    string_q baseType;
    uint64_t size, count, nBits, remains;
    parseType(type, baseType, size, count, nBits, remains);

    // Handle one word and delete it from the input string
    string_q word = extract(input, 0, 64);
    replace(input, word, "");

    if ( startsWith(type, "uint") ) {
        word = "0x" + string_q(remains, '0') + extract(word, remains, nBits);
        value = bnu_2_Str(str_2_Wei(word));

    } else if ( startsWith(type, "int") ) {
        bool neg = startsWith(word, "f");
        string_q xx = "0x" + (neg ? string_q(remains, 'f') : string_q(remains, '0')) + extract(word, remains, nBits);
        if (size <= 64) {
            value = int_2_Str(str_2_Int(xx));
        } else {
            bigint_t w = str_2_BigInt(xx);
            if (w >= (max256Int/2))
                w = w - max256Int - 1; // wrap if larger than max int256
            value = bni_2_Str(w);
        }

    } else if ( type == "bool") {
        value = bool_2_Str(str_2_Uint(trimLeading(word, '0')));

    } else if ( startsWith(type, "address") ) {
        value = str_2_Addr(word);

    } else if ( startsWith(type, "bytes") ) {
        value = "0x" + word;

    } else {
        value = "unknown type: " + type;

    }
    return sizeIn - input.length();
}

//---------------------------------------------------------------------------
size_t CParameter::parseFixedArray(string_q& input) {

    size_t sizeIn = input.length();
    string_q baseType;
    uint64_t size, count, nBits, remains;
    parseType(type, baseType, size, count, nBits, remains);

    string_q ret;
    for (uint64_t i = 0 ; i < count ; i++) {

        string_q word = extract(input, 0, 64);
        replace(input, word, "");

        CParameter pp;
        pp.type = baseType;
        pp.parseFixedType(word);

        if (!ret.empty())
            ret += ", ";
        ret += pp.value;
    }
    value = "[" + ret + "]";
    return sizeIn - input.length();
}

//---------------------------------------------------------------------------
size_t CParameter::parseDynamicType(string_q& input) {

    size_t sizeIn = input.length();
    string_q word = extract(input, 0, 64);
    replace(input, word, "");

    uint64_t nItems = str_2_BigUint("0x" + word).to_ulong();
    uint64_t size = 0;
    uint64_t count = NOPOS, l = NOPOS, s = NOPOS;
    string_q baseType;
    parseType(type, baseType, size, count, l, s);

    uint64_t nn = (((nItems * 2) / 64) + 1) * 64;
    if (type == "string") {
        string_q data = extract(input, 0, nn);
        replace(input, data, "");
        data = extract(data, 0, nItems * 2);
        value += hex_2_Str(data);

    } else if (type == "bytes") {
        string_q data = extract(input, 0, nn);
        replace(input, data, "");
        data = extract(data, 0, nItems * 2);
        while (!data.empty()) {
            string_q d = data.substr(0, 64);
            replace(data, d, ""); // consumes data
            CParameter pp;
            pp.type = baseType;
            pp.parseFixedType(d);
            value += pp.value;
        }

    } else {
        CParameter pp = *this;
        pp.type = baseType + "[" + uint_2_Str(nItems) + "]";
        pp.parseFixedArray(input);
        value = pp.value;
        return sizeIn - input.length();

    }

    if (type != "string" && type != "bytes")
        value = "[" + value + "]";
    return sizeIn - input.length();
}

extern string_q parseArrayMulti(CParameter& p, string_q& input);
//---------------------------------------------------------------------------
void extractParts(CParameterArray& interfaces, const string_q& input, string_q& fPart, string_q& dPart) {
    bool hasDynamic = false;

    // TODO: If I remove the test for bytes, acctExport core dumps. If I put it back in some abi tests fail
    if (false) { //isTestMode()) {
        for (auto i : interfaces)
            if (i.isDyn())
                hasDynamic = true;
    } else {
        for (auto i : interfaces)
            if (i.isDyn() && i.type != "bytes")
                hasDynamic = true;
    }

    if (hasDynamic) {
        uint64_t dStart = str_2_BigUint("0x" + extract(input, 0, 64)).to_ulong() * 2;
        fPart = input.substr(64, dStart - 64);
        dPart = input.substr(dStart);
    } else {
        fPart = input;
        dPart = "";
    }
}

//---------------------------------------------------------------------------
bool decodeRLP(CParameterArray& interfaces, const string_q& inputIn) {

    // We consume these as we go...
    string_q input = substitute(inputIn, "0x", "");

    // We split the data into fixed and dynamic sized
    string_q fPart, dPart;
    extractParts(interfaces, input, fPart, dPart);

    for (auto& in : interfaces) {
        if (in.isMulti()) {
            in.value = "not parsed";
//            string_q newInput = "0x" + padLeft(uint_2_Str(fPart.length()/64+1),64,'0') + fPart + dPart;
//            in.value = parseArrayMulti(in, newInput);
//            input = newInput;

        } else if (in.isDyn()) {
            in.parseDynamicType(dPart);

        } else if (in.isArray) {
            in.parseFixedArray(fPart);

        } else {
            in.parseFixedType(fPart);

        }
    }
    return true;
}

/*
 twice uint[2]               uint[2][2]
 0000000000000000000000000000000000000000000000000000000000000001\
 0000000000000000000000000000000000000000000000000000000000000002\

 0000000000000000000000000000000000000000000000000000000000000004\
 0000000000000000000000000000000000000000000000000000000000000008\
 | [ [ 1, 2 ], [ 4, 8 ] ]


 twice uint128[3]            uint128[2][3], uint
 0000000000000000000000000000000000000000000000000000000000000001\
 0000000000000000000000000000000000000000000000000000000000000002\
 0000000000000000000000000000000000000000000000000000000000000003\

 0000000000000000000000000000000000000000000000000000000000000004\
 0000000000000000000000000000000000000000000000000000000000000005\
 0000000000000000000000000000000000000000000000000000000000000006\
 000000000000000000000000000000000000000000000000000000000000000a\
 | [ [ 1, 2, 3 ], [ 4, 5, 6 ] ], 10



 twice three times uint[2]   uint128[2][3][2], uint
 0000000000000000000000000000000000000000000000000000000000000001\
 0000000000000000000000000000000000000000000000000000000000000002\

 0000000000000000000000000000000000000000000000000000000000000003\
 0000000000000000000000000000000000000000000000000000000000000004\

 0000000000000000000000000000000000000000000000000000000000000005\
 0000000000000000000000000000000000000000000000000000000000000006\


 0000000000000000000000000000000000000000000000000000000000000001\
 0000000000000000000000000000000000000000000000000000000000000002\

 0000000000000000000000000000000000000000000000000000000000000003\
 0000000000000000000000000000000000000000000000000000000000000004\

 0000000000000000000000000000000000000000000000000000000000000005\
 0000000000000000000000000000000000000000000000000000000000000006\

 000000000000000000000000000000000000000000000000000000000000000a\
 |[[[1, 2], [3, 4], [5, 6]], [[1, 2], [3, 4], [5, 6]]], 10



 three times unknown         uint[3][], uint
 0000000000000000000000000000000000000000000000000000000000000040\   skip 2
 000000000000000000000000000000000000000000000000000000000000000a\   10

 0000000000000000000000000000000000000000000000000000000000000002\

 0000000000000000000000000000000000000000000000000000000000000001\
 0000000000000000000000000000000000000000000000000000000000000002\

 0000000000000000000000000000000000000000000000000000000000000003\
 0000000000000000000000000000000000000000000000000000000000000004\

 0000000000000000000000000000000000000000000000000000000000000005\
 0000000000000000000000000000000000000000000000000000000000000006\
 |[[1, 2], [3, 4], [5, 6]], 10


 unknown times three         uint[][3], uint
 0000000000000000000000000000000000000000000000000000000000000080\   skip 4
 00000000000000000000000000000000000000000000000000000000000000e0\   skip 7
 0000000000000000000000000000000000000000000000000000000000000140\   skip 10
 000000000000000000000000000000000000000000000000000000000000000a\

 0000000000000000000000000000000000000000000000000000000000000002\
 0000000000000000000000000000000000000000000000000000000000000001\
 0000000000000000000000000000000000000000000000000000000000000002\

 0000000000000000000000000000000000000000000000000000000000000002\
 0000000000000000000000000000000000000000000000000000000000000003\
 0000000000000000000000000000000000000000000000000000000000000004\

 0000000000000000000000000000000000000000000000000000000000000002\
 0000000000000000000000000000000000000000000000000000000000000005\
 0000000000000000000000000000000000000000000000000000000000000006\
 |[[1, 2], [3, 4], [5, 6]], 10
 */

//---------------------------------------------------------------------------
bool decodeRLP2(CParameterArray& interfaces, string_q& input) {

    // We consume these as we go...
    input = substitute(input, "0x", "");

    // We split the data into fixed and dynamic sized
    string_q fPart, dPart;
    extractParts(interfaces, input, fPart, dPart);
    for (auto& in : interfaces) {
        if (in.isDyn()) {
            in.parseDynamicType(dPart);
        } else if (in.isArray) {
            string_q before = fPart;
            in.parseFixedArray(fPart);
            string_q used = substitute(before, fPart, "");
            replace(input, used, "");
        } else {
            in.parseFixedType(fPart);
        }
    }
    return true;
}

//---------------------------------------------------------------------------
inline string_q parseArrayMulti(CParameter& p, string_q& input) {

//TODO(tjayrush): Not done
    return input;
#if 0
    if (!p.isMulti()) {
        CParameterArray a;
        a.push_back(p);
        decodeRLP2(a, input);
        p.value = a[0].value;
        return p.value;
    }

    // We split the data into fixed and dynamic sized
    string_q fPart, dPart;
    CParameterArray a;
    a.push_back(p);
    extractParts(a, input, fPart, dPart);

    string_q type = p.type;
    string_q baseType = getBaseType(type);
    replace(type, baseType, "");

    CStringArray sizes, backwards;
    replaceAll(type,"]","|");
    replaceAll(type,"[","");
    explode(sizes, type, '|');
    for (size_t i = sizes.size(); i-- > 0; )
        backwards.push_back(sizes[i]);

    string_q ret;
    for (auto size : backwards) {
        CParameter pp = p;
        pp.type = baseType + "[" + backwards[0] + "]";
        pp.value = parseArrayMulti(pp, input);
        if (!ret.empty())
            ret += ", ";
        ret += pp.value;
    }
    return "[" + ret + "]";
#endif
}

//----------------------------------------------------------------------------
inline string_q hex_2_Str_old(const string_q& inHex) {
    string_q ret, in = (startsWith(inHex, "0x") ? extract(inHex, 2) : inHex);
    while (!in.empty()) {
        string_q nibble = extract(in, 0, 2);
        in = extract(in, 2);
        char ch = (char)hex_2_Ascii((char*)nibble.c_str());  // NOLINT
        if (!isprint(ch))
            return "";
        ret += (char)ch;
    }
    return ret;
}

//-----------------------------------------------------------------------
bool CAbi::articulateTransaction(CTransaction *p) const {

    if (!p)
        return false;

    // articulate the events, so we can return with a fully articulated object
    for (auto& log : p->receipt.logs)
        articulateLog(&log);

    // articulate the traces, so we can return with a fully articulated object
    bool hasTraces = false;
    for (auto& trace : p->traces) {
        hasTraces = true;
        trace.articulatedTrace.m_showing = false;
        ((CAbi*)this)->loadAbiByAddress(trace.action.to);
        if (articulateTrace(&trace))
            trace.articulatedTrace.m_showing = true;
    }

    if (p->input.length() >= 10 || p->input == "0x") {
        string_q encoding = extract(p->input, 0, 10);
        string_q input    = extract(p->input, 10);
        for (auto interface : interfaces) {
            if (encoding % interface.encoding) {
                p->articulatedTx = CFunction(interface);
                p->articulatedTx.showOutput = false;
                bool ret1 = decodeRLP(p->articulatedTx.inputs, input);
                bool ret2 = (hasTraces ? decodeRLP(p->articulatedTx.outputs, p->traces[0].result.output) : false);
                return (ret1 || ret2);
            }
        }
        p->articulatedTx.message = hex_2_Str_old(p->input);
    }
    return false;
}

//-----------------------------------------------------------------------
inline bool sortByPosition(const CParameter& v1, const CParameter& v2) {
    return v1.pos < v2.pos;
}

//-----------------------------------------------------------------------
inline bool sortByIndexedPosition(const CParameter& v1, const CParameter& v2) {
    uint64_t val1 = (v1.indexed ? 0 : 10000) + v1.pos;
    uint64_t val2 = (v2.indexed ? 0 : 10000) + v2.pos;
    return val1 < val2;
}

//-----------------------------------------------------------------------
bool CAbi::articulateLog(CLogEntry *p) const {

    if (!p)
        return false;

    size_t nTops = p->topics.size();
    if (nTops > 0) {  // the '0'th topic is the event signature
        string_q data = extract(p->data, 2);
        string_q input;
        bool first = true;
        for (auto t : p->topics) {
            if (!first)
                input += extract(topic_2_Str(t),2);
            first = false;
        }
        input += data;

        bool ret = false;
        CAbi *ncABI = (CAbi*)this;
        for (size_t i = 0 ; i < ncABI->interfaces.size() ; i++) {
            CFunction *intf = &ncABI->interfaces[i];
            if (topic_2_Str(p->topics[0]) % intf->encoding) {
                p->articulatedLog = CFunction(*intf);
                p->articulatedLog.showOutput = false;
                for (size_t j = 0 ; j < p->articulatedLog.inputs.size(); j++)
                    p->articulatedLog.inputs[j].pos = j;
                sort(p->articulatedLog.inputs.begin(), p->articulatedLog.inputs.end(), sortByIndexedPosition);
                ret = decodeRLP(p->articulatedLog.inputs, input);
                sort(p->articulatedLog.inputs.begin(), p->articulatedLog.inputs.end(), sortByPosition);
            }
        }
        return ret;
    }
    return false;
}

//-----------------------------------------------------------------------
bool CAbi::articulateTrace(CTrace *p) const {

    if (!p)
        return false;

    if (p->action.input.length() >= 10 || p->action.input == "0x") {
        string_q encoding = extract(p->action.input, 0, 10);
        string_q input    = extract(p->action.input, 10);
        for (auto interface : interfaces) {
            if (encoding % interface.encoding) {
                p->articulatedTrace = CFunction(interface);
                p->articulatedTrace.showOutput = false;
                bool ret1 = decodeRLP(p->articulatedTrace.inputs, input);
                bool ret2 = decodeRLP(p->articulatedTrace.outputs, p->result.output);
                return (ret1 || ret2);
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------
bool CAbi::articulateOutputs(const string_q& encoding, const string_q& output, CFunction& ret) const {
    for (auto interface : interfaces) {
        if (encoding % interface.encoding) {
            ret = CFunction(interface);
            ret.showOutput = false;
            return decodeRLP(ret.outputs, output);
        }
    }
    return true;
}
// EXISTING_CODE
}  // namespace qblocks

