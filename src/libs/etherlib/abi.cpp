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

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["abi_fmt"] : fmtIn);
    if (fmt.empty()) {
        ctx << toJson();
        return;
    }

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
                    cerr << "The address you specified (" << addr << ") does not\n";
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

//------------------------------------------------------------------------------------------------
string_q decodeParams(const CStringArray& typeArray, const CStringArray& dataArray, size_t& readHead) {

    string ret = "";
    for (auto type : typeArray) {

        if (!ret.empty())
            ret += ", ";

        bool isArray = (type.find("[") != string::npos);
        if (!isArray) {

            if (type.find("bool") != string::npos) {
                CStringArray array;
                array.push_back("uint256");
                string_q rr = decodeParams(array, dataArray, readHead);
                ret += ((rr == "1") ? "true" : "false");

            } else if (type.find("address") != string::npos) {
                CStringArray array;
                array.push_back("uint160");
                ret += "0x" + padLeft(toLower(bnu_2_Hex(str_2_BigUint(decodeParams(array, dataArray, readHead)))), 40, '0');

            } else if (type.find("uint") != string::npos) {

                string dataItem = dataArray[readHead];
                size_t bits = str_2_Uint(substitute(type,"uint",""));
                dataItem = "0x" + dataItem;
                biguint_t value = str_2_BigUint(dataItem, bits);
                ret += bnu_2_Str(value);
                readHead++;

            } else if (type.find("int") != string::npos) {

                string dataItem = dataArray[readHead];
                size_t bits = str_2_Uint(substitute(type,"int",""));
                dataItem = "0x" + dataItem;

                bigint_t value = str_2_BigInt(dataItem, bits);
                ret += bni_2_Str(value);
                readHead++;

            } else if (type.find("string") != string::npos) {

                string_q result;
                uint64_t strStart = (str_2_Uint("0x" + dataArray[readHead]) / 32);
                uint64_t stringLen = str_2_Uint("0x" + dataArray[strStart]);
                size_t nWords = (stringLen / 32) + 1;
                if (nWords <= dataArray.size()) { // some of the data sent in may be bogus, so we protext ourselves
                    for (size_t w = 0 ; w < nWords ; w++) {
                        size_t pos = strStart + 1 + w;
                        if (pos < dataArray.size())
                            result += dataArray[pos].substr(0, stringLen*2);
                        stringLen -= 32;
                    }
                }
                ret += hex_2_Str("0x" + result, 10000);
                readHead++;

            } else if (type == "bytes") {

                uint64_t dataOffset = str_2_Uint("0x" + dataArray[readHead++]);
                size_t tPtr = dataOffset / 32;
                uint64_t byteLength = str_2_Uint("0x" + dataArray[tPtr++]);
                string_q result;
                size_t nWords = (byteLength / 32) + 1;
                for (size_t i = 0 ; i < nWords ; i++) {
                    result += dataArray[tPtr++].substr(0, min(uint64_t(32*2), byteLength * 2));
                    byteLength -= (32 * 2);
                }
                ret = "0x" + result;

            } else if (type.find("bytes") != string::npos) {

                string dataItem = "0x" + dataArray[readHead];
                ret += dataItem;
                readHead++;

            }

        } else {
            ASSERT(isArray);
            if (type.find("]") == type.find("[") + 1) {

                size_t tPtr = readHead++;
                uint64_t arrOffset = str_2_Uint("0x" + dataArray[tPtr]);
                tPtr = arrOffset / 32;
                uint64_t elementNum = str_2_Uint("0x" + dataArray[tPtr++]);

                CStringArray tmpArray;
                size_t firstBracePos = type.find('[');
                string paramType = type.substr(0, firstBracePos);
                if (paramType == "bytes")
                    paramType = "bytes32";
                if(firstBracePos + 2 != type.size())
                    paramType += type.substr(firstBracePos + 2);
                for (size_t i = 0; i < elementNum; i++)
                    tmpArray.push_back(paramType);
                ret += "[" + decodeParams(tmpArray, dataArray, tPtr) + "]";

            } else if (type.find("]") != string::npos) {

                //int tempPointer;
                /*
                 if(typeArray.size() != 1) {
                 // Find offset pointing to "real values" of dynamic array
                 uint64_t arrOffset = str_2_Uint(dataArray[readHead]);
                 tempPointer = arrOffset / 32;
                 } else {
                 tempPointer = readHead;
                 }
                 */
                int elementNum = stoi(type.substr(type.find('[')+1, type.find(']')));
                CStringArray arrParams;
                size_t firstLBracePos = type.find('[');
                size_t firstRBracePos = type.find(']');
                string paramType = type.substr(0, firstLBracePos);
                if (firstRBracePos + 1 != type.size())
                    paramType += type.substr(firstRBracePos + 1);
                for (int i = 0; i < elementNum; i++)
                    arrParams.push_back(paramType);
                ret += "[" + decodeParams(arrParams, dataArray, readHead) + "]";
                //readHead++;
            }
        }
    }
    return trim(ret);
}

    //------------------------------------------------------------------------------------------------
    size_t decodeParams2(CParameterArray& typeArray, const CStringArray& dataArray, size_t& readHead) {

        for (size_t q = 0 ; q < typeArray.size() ; q++) {
            CParameter *pPtr = &typeArray[q];
            string_q type = pPtr->type;

            bool isArray = (type.find("[") != string::npos);
            if (!isArray) {

                if (type.find("bool") != string::npos) {
                    CParameterArray array;
                    CParameter p;
                    p.type = "uint256";
                    array.push_back(p);
                    decodeParams2(array, dataArray, readHead);
                    string_q val = array[0].value;
                    pPtr->value = ((val == "1") ? "true" : "false");

                } else if (type.find("address") != string::npos) {

                    CParameterArray array;
                    CParameter p;
                    p.type = "uint160";
                    array.push_back(p);
                    decodeParams2(array, dataArray, readHead);
                    string_q val = array[0].value;
                    pPtr->value = "0x" + padLeft(toLower(bnu_2_Hex(str_2_BigUint(val))), 40, '0');

                } else if (type.find("uint") != string::npos) {

                    string dataItem = dataArray[readHead];
                    size_t bits = str_2_Uint(substitute(type,"uint",""));
                    dataItem = "0x" + dataItem;
                    biguint_t value = str_2_BigUint(dataItem, bits);
                    pPtr->value = bnu_2_Str(value);
                    readHead++;

                } else if (type.find("int") != string::npos) {

                    string dataItem = dataArray[readHead];
                    size_t bits = str_2_Uint(substitute(type,"int",""));
                    dataItem = "0x" + dataItem;

                    bigint_t value = str_2_BigInt(dataItem, bits);
                    pPtr->value = bni_2_Str(value);
                    readHead++;

                } else if (type.find("string") != string::npos) {

                    string_q result;
                    uint64_t strStart = (str_2_Uint("0x" + dataArray[readHead]) / 32);
                    uint64_t stringLen = str_2_Uint("0x" + dataArray[strStart]);
                    size_t nWords = (stringLen / 32) + 1;
                    if (nWords <= dataArray.size()) { // some of the data sent in may be bogus, so we protext ourselves
                        for (size_t w = 0 ; w < nWords ; w++) {
                            size_t pos = strStart + 1 + w;
                            if (pos < dataArray.size())
                                result += dataArray[pos].substr(0, stringLen*2);
                            stringLen -= 32;
                        }
                    }
                    pPtr->value = hex_2_Str("0x" + result, 10000);
                    readHead++;

                } else if (type == "bytes") {

                    uint64_t dataOffset = str_2_Uint("0x" + dataArray[readHead++]);
                    size_t tPtr = dataOffset / 32;
                    uint64_t byteLength = str_2_Uint("0x" + dataArray[tPtr++]);
                    string_q result;
                    size_t nWords = (byteLength / 32) + 1;
                    for (size_t i = 0 ; i < nWords ; i++) {
                        result += dataArray[tPtr++].substr(0, min(uint64_t(32*2), byteLength * 2));
                        byteLength -= (32 * 2);
                    }
                    pPtr->value = "0x" + result;

                } else if (type.find("bytes") != string::npos) {

                    string dataItem = "0x" + dataArray[readHead];
                    pPtr->value = dataItem;
                    readHead++;

                }

            } else {
                ASSERT(isArray);
                if (type.find("]") == type.find("[") + 1) {

                    size_t tPtr = readHead++;
                    uint64_t arrOffset = str_2_Uint("0x" + dataArray[tPtr]);
                    tPtr = arrOffset / 32;
                    uint64_t elementNum = str_2_Uint("0x" + dataArray[tPtr++]);

                    CParameterArray tmpArray;
                    size_t firstBracePos = type.find('[');
                    string paramType = type.substr(0, firstBracePos);
                    if (paramType == "bytes")
                        paramType = "bytes32";
                    if(firstBracePos + 2 != type.size())
                        paramType += type.substr(firstBracePos + 2);

                    for (size_t i = 0; i < elementNum; i++) {
                        CParameter p;
                        p.type = paramType;
                        tmpArray.push_back(p);
                    }

                    decodeParams2(tmpArray, dataArray, tPtr);
                    string_q val;
                    for (auto pppp : tmpArray) {
                        if (!val.empty())
                            val += ", ";
                        val += pppp.value;
                    }
                    pPtr->value = "[" + val + "]";

                } else if (type.find("]") != string::npos) {

                    //int tempPointer;
                    /*
                     if(typeArray.size() != 1) {
                     // Find offset pointing to "real values" of dynamic array
                     uint64_t arrOffset = str_2_Uint(dataArray[readHead]);
                     tempPointer = arrOffset / 32;
                     } else {
                     tempPointer = readHead;
                     }
                     */
                    int elementNum = stoi(type.substr(type.find('[')+1, type.find(']')));
                    CParameterArray tmpArray;
                    size_t firstLBracePos = type.find('[');
                    size_t firstRBracePos = type.find(']');
                    string paramType = type.substr(0, firstLBracePos);
                    if (firstRBracePos + 1 != type.size())
                        paramType += type.substr(firstRBracePos + 1);
                    for (int i = 0; i < elementNum; i++) {
                        CParameter p;
                        p.type = paramType;
                        tmpArray.push_back(p);
                    }
                    decodeParams2(tmpArray, dataArray, readHead);
                    string_q val;
                    for (auto pppp : tmpArray) {
                        if (!val.empty())
                            val += ", ";
                        val += pppp.value;
                    }
                    pPtr->value = "[" + val + "]";
                    //readHead++;
                }
            }
        }
        return 1;
    }


//-----------------------------------------------------------------------------------------
size_t parseTheData(CStringArray& dataArray, const string_q& dataStr) {

    string_q str = trim(substitute(dataStr, "0x", ""), ' ');
    ASSERT(!(str.size() % 64));
    while (!str.empty()) {
        string_q item = str.substr(0,64);
        ASSERT(items.size() == 64);
        replace(str, item, "");
        dataArray.push_back(item);
    }
    return dataArray.size();
}

//-----------------------------------------------------------------------------------------
size_t extractParams(CStringArray& paramArray, const string_q& paramStr) {

    string_q str = substitute(substitute(paramStr, "(", "|"), ")", "|");

    CStringArray parts;
    explode(parts, str, '|');

    explode(paramArray, parts[1], ',');

    size_t cnt = 0;
    for (auto p : paramArray) {

        // remove extraneous spaces or tabs
        string type = trim(substitute(p, "\t", " "), ' ');
        while (contains(type, " ["))
            type = substitute(type, " [", "[");
        while (contains(type, " ]"))
            type = substitute(type, " ]", "]");

        // strip field names
        if (type.find(" ") != string::npos)
            type = type.substr(0, type.find(" "));

        // clean up syntactic sugar
        if (type == "int") type = "int256";
        if (type == "uint") type = "uint256";
        if (type == "fixed") type = "fixed128x128";
        if (type == "ufixed") type = "ufixed128x128";
        if (startsWith(type, "int[")) type = substitute(type, "int[", "int256[");
        if (startsWith(type, "uint[")) type = substitute(type, "uint[", "uint256[");
        if (startsWith(type, "fixed[")) type = substitute(type, "fixed[", "fixed128x128[");
        if (startsWith(type, "ufixed[")) type = substitute(type, "ufixed[", "ufixed128x128[");

        paramArray[cnt++] = type;
    }
    return paramArray.size();
}

//-----------------------------------------------------------------------------------------
string_q decode(const string_q& function, const string_q& inputStr) {
    CStringArray params;
    extractParams(params, function);
    if (verbose > 1) {
        for (auto param : params)
            cout << param << endl;
    }

    CStringArray inputs;
    parseTheData(inputs, inputStr);
    if (verbose > 1) {
        for (auto input : inputs)
            cout << input << endl;
    }

    size_t offset = 0;
    return decodeParams(params, inputs, offset);
}

//---------------------------------------------------------------------------
bool decodeRLP(CParameterArray& interfaces, const string_q& inputIn) {

    CStringArray inputs;
    if (inputIn.empty()) {
        for (auto i : interfaces)
            inputs.push_back(string_q(64, '0'));
    } else {
        parseTheData(inputs, inputIn);
    }

//    SHOW_FIELD(CParameter, "type");
//    for (auto i : interfaces)
//        cout << i << endl;
//    for (auto ii : inputs)
//        cout << ii << endl;

    size_t offset = 0;
    return decodeParams2(interfaces, inputs, offset);
}

//----------------------------------------------------------------------------
extern string_q hex_2_Str_old(const string_q& inHex);

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

    // From solidity documentation:
    // For all fixed-length Solidity types, the EVENT_INDEXED_ARGS array contains the 32-byte encoded value directly.
    // However, for types of dynamic length, which include string, bytes, and arrays, EVENT_INDEXED_ARGS will contain
    // the Keccak hash of the packed encoded value (see Strict Encoding Mode), rather than the encoded value directly.
    // This allows applications to efficiently query for values of dynamic-length types (by setting the hash of the
    // encoded value as the topic), but leaves applications unable to decode indexed values they have not queried for.
    // For dynamic-length types, application developers face a trade-off between fast search for predetermined values
    // (if the argument is indexed) and legibility of arbitrary values (which requires that the arguments not be
    // indexed). Developers may overcome this tradeoff and achieve both efficient search and arbitrary legibility
    // by defining events with two arguments — one indexed, one not — intended to hold the same value.
    //
    // Upshot: Topics are always fixed length. Data needs to be parsed as RLP encoded. Since the indexed event
    // args and the non-indexed event args can be interleaved, we need to decode them separately
    // TODO: SEARCH FOR ISSUE #1013

//    bool ret = false;
    CAbi *ncABI = (CAbi*)this;
    for (size_t i = 0 ; i < ncABI->interfaces.size() ; i++) {
        CFunction *intf = &ncABI->interfaces[i];
        if (topic_2_Str(p->topics[0]) % intf->encoding) {
            // we found the event we're looking for, make a copy
            p->articulatedLog = CFunction(*intf);
            p->articulatedLog.showOutput = false;

            // separate out the topic params from the data params
            string_q topicStr; size_t which = 1, pos = 0;
            CParameterArray topics, data;
            for (auto in : p->articulatedLog.inputs) {
                if (in.indexed) {
                    in.pos = pos++;
                    topics.push_back(in);
                    topicStr += topic_2_Str(p->topics[which++]);
                } else {
                    in.pos = pos++;
                    data.push_back(in);
                }
            }
//            SHOW_FIELD(CParameter, "pos");
//            SHOW_FIELD(CParameter, "type");
//            SHOW_FIELD(CParameter, "value");
//            SHOW_FIELD(CParameter, "indexed");
//            for (auto a : topics)
//                cout << a << endl;
//            for (auto a : data)
//                cout << a << endl;

            decodeRLP(topics, topicStr);
            decodeRLP(data, p->data);

//            for (auto a : p->articulatedLog.inputs)
//                cout << a << endl;
            p->articulatedLog.inputs.clear();
            for (auto a : topics) {
                p->articulatedLog.inputs.push_back(a);
//                cout << a << endl;
            }
            for (auto a : data) {
                p->articulatedLog.inputs.push_back(a);
//                cout << a << endl;
            }
            sort(p->articulatedLog.inputs.begin(), p->articulatedLog.inputs.end(), sortByPosition);
//            for (auto a : p->articulatedLog.inputs)
//                cout << a << endl;
        }
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

#if 0
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
#endif

// TODO: If I remove the test for bytes, acctExport core dumps. If I put it back in some abi tests fail
// TODO: SEARCH FOR ISSUE #1013
//{
//1848         "address": "0x159cf1e9ae58211b588f5e3bf1d7e423952d959b",
//1849         "data": "
//1850          0x
//1851          0000000000000000000000000000000000000000000000190b89b83816780000
//1852          0000000000000000000000000000000000000000000000000000000000000040
//1853          0000000000000000000000000000000000000000000000000000000000000025
//1854          486920426f6220476c617365722d2d20486572652061726520796f757220746f
//1855          6b656e7321
//1850         "logIndex": 3,
//1851         "topics": [
//1852           "0xc620c23091c77c537ae2f7deb703ecdea8b2cb3df9053ac018b281ffa1a9cae4",
//1853           "0x000000000000000000000000f3c9c5719eb4f26a3ab45cb86771827629f9a999",
//1854           "0x00000000000000000000000071d402ac181de4a8739a15f5a56141f64fc522f8"
//1855         ],
//1856         "articulatedLog": {
//1857           "name": "TransferX",
//1858           "inputs": [
//1859             {
//1860               "name": "_from",
//1861               "type": "address",
//1862               "value": "0xf3c9c5719eb4f26a3ab45cb86771827629f9a999"
//1863             },
//1864             {
//1865               "name": "_to",
//1866               "type": "address",
//1867               "value": "0x71d402ac181de4a8739a15f5a56141f64fc522f8"
//1868             },
//1869             {
//1870               "name": "_value",
//1871               "type": "uint256",
//1872               "value": "462000000000000000000"
//1873             },
//1874             {
//1875               "name": "_transferMetaData",
//1876               "type": "string"
//1877             }
//1878           ],
//1879           "outputs": []
//1880         }
//
// Test data from ./test/gold/libs/etherlib/abiTest/decode.txt
//
// THIS TEST CASE HAS A FIXED LEN DATA FOLLOWED BY DYNAMIC LEN ARRAY, BUT THE LENGHT PARAM IS FIRST UNLIKE ABOVE DATA
// decode|decoding bool, uint256[]|function bz(bool, uint256[])|
// 0x
// 0000000000000000000000000000000000000000000000000000000000000001
// 0000000000000000000000000000000000000000000000000000000000000040
// 0000000000000000000000000000000000000000000000000000000000000001
// 000000000000000000000000000000000000000000000000000000000000002a
// |true, [42]
//
//
// THIS TEST CASE CAPTURES THE CORE DUMP IF WE REMOVE THE TRY/CATCH BELOW
// decode|decoding address,address,uint256,string|function baz(address,address,uint256,string)|
// 0x
// 000000000000000000000000f3c9c5719eb4f26a3ab45cb86771827629f9a999
// 00000000000000000000000071d402ac181de4a8739a15f5a56141f64fc522f8
// 0000000000000000000000000000000000000000000000190b89b83816780000
// 0000000000000000000000000000000000000000000000000000000000000040
// 0000000000000000000000000000000000000000000000000000000000000025
// 486920426f6220476c617365722d2d20486572652061726520796f757220746f
// 6b656e7321|
// 0xf3c9c5719eb4f26a3ab45cb86771827629f9a999,71d402ac181de4a8739a15f5a56141f64fc522f8,462000000000000000000,_transferX
//
// Remove the next test, and uncomment the above test case in ../test/gold/libs/etherlib/abiTest/decode.txt, and you
// will have a test case that fails to ease debuggi ng
//
// There is also testing data in ../quickBlocks-monitors/clients/88_NthRound/ (note that you must have a file called
// 'debug' in the local folder otherwise the data gets sent to the end user!!!)
// TODO: SEARCH FOR ISSUE #1013
