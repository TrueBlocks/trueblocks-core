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
#include <algorithm>
#include "abi.h"
#include "node.h"

//---------------------------------------------------------------------------
namespace qblocks {

    //-----------------------------------------------------------------------------------------
    string_q params_2_Str(CParameterArray& interfaces) {
        string_q ret;
        for (auto p : interfaces) {
            if (!ret.empty())
                ret += ", ";
            ret += p.value;
        }
        return trim(ret);
    }

    //------------------------------------------------------------------------------------------------
    size_t decodeTheData(CParameterArray& interfaces, const CStringArray& dataArray, size_t& readIndex) {

        if (interfaces.size() > dataArray.size()) {
            cerr << "{ \"error\": \"Bad data encountered in decodeTheData. Ignoring...\" }," << endl;
            return 1;
        }

        for (size_t q = 0 ; q < interfaces.size() ; q++) {

            CParameter *pPtr = &interfaces[q];
            string_q type = pPtr->type;

            bool isBaseType = (type.find("[") == string::npos);
            if (isBaseType) {

                if (type.find("bool") != string::npos) {

                    // sugar
                    CParameterArray tmp;
                    { CParameter p; p.type = "uint256"; tmp.push_back(p); }
                    decodeTheData(tmp, dataArray, readIndex);
                    pPtr->value = ((tmp[0].value == "1") ? "true" : "false");

                } else if (type.find("address") != string::npos) {

                    // sugar
                    CParameterArray tmp;
                    { CParameter p; p.type = "uint160"; tmp.push_back(p); }
                    decodeTheData(tmp, dataArray, readIndex);
                    pPtr->value = "0x" + padLeft(toLower(bnu_2_Hex(str_2_BigUint(tmp[0].value))), 40, '0');

                } else if (type.find("uint") != string::npos) {
                    size_t bits = str_2_Uint(substitute(type, "uint", ""));
                    pPtr->value = bnu_2_Str(str_2_BigUint("0x" + dataArray[readIndex++], bits));

                } else if (type.find("int") != string::npos) {

                    size_t bits = str_2_Uint(substitute(type, "int", ""));
                    pPtr->value = bni_2_Str(str_2_BigInt("0x" + dataArray[readIndex++], bits));

                } else if (type == "string" || type == "bytes") {

                    // Strings and bytes are dynamic sized. The fixed size part resides at readIndex and points to
                    // start of string. Start of string is length of string. Start of string + 1 is the string
                    string_q result;
                    uint64_t dataStart = (str_2_Uint("0x" + dataArray[readIndex++]) / 32);
                    uint64_t nBytes = str_2_Uint("0x" + dataArray[dataStart]);
                    size_t nWords = (nBytes / 32) + 1;
                    if (nWords <= dataArray.size()) { // some of the data sent in may be bogus, so we protext ourselves
                        for (size_t w = 0 ; w < nWords ; w++) {
                            size_t pos = dataStart + 1 + w;
                            if (pos < dataArray.size())
                                result += dataArray[pos].substr(0, nBytes * 2);  // at most 64
                            nBytes -= 32;
                        }
                    }
                    pPtr->value = (type == "string" ? hex_2_Str("0x" + result) : "0x" + result);

                } else if (type.find("bytes") != string::npos) {

                    // bytes1 through bytes32 are fixed length
                    pPtr->value = "0x" + dataArray[readIndex++];

                } else {

                    LOG_WARN("Unknown type: ", type, " in decodeTheData");

                }

            } else {

                ASSERT(!isBaseType);
                if (type.find("[") == type.find("[]")) { // the first bracket is a dynamic array

                    size_t found = type.find('[');
                    string baseType = type.substr(0, found);
                    if (baseType == "bytes") baseType = "bytes32";
                    if (found + 2 != type.size())
                        baseType += type.substr(found + 2);

                    uint64_t dataStart = (str_2_Uint("0x" + dataArray[readIndex++]) / 32);
                    uint64_t nItems = str_2_Uint("0x" + dataArray[dataStart]);

                    CParameterArray tmp;
                    for (size_t i = 0; i < nItems; i++)
                    { CParameter p; p.type = baseType; tmp.push_back(p); }

                    size_t tPtr = dataStart + 1;
                    decodeTheData(tmp, dataArray, tPtr);
                    pPtr->value = "[" + params_2_Str(tmp) + "]";

                } else if (type.find("]") != string::npos) {

                    //int tempPointer;
                    /*
                     if(interfaces.size() != 1) {
                     // Find offset pointing to "real values" of dynamic array
                     uint64_t dataStart = str_2_Uint(dataArray[readIndex]);
                     tempPointer = dataStart / 32;
                     } else {
                     tempPointer = readIndex;
                     }
                     */
                    size_t found1 = type.find('[');
                    size_t found2 = type.find(']');
                    string subType = type.substr(0, found1);
                    if (found2 + 1 != type.size())
                        subType += type.substr(found2 + 1);

                    int nBytes = stoi(type.substr(type.find('[')+1, type.find(']')));
                    CParameterArray tmp;
                    for (int i = 0; i < nBytes; i++)
                    { CParameter p; p.type = subType; tmp.push_back(p); }
                    decodeTheData(tmp, dataArray, readIndex);
                    pPtr->value = "[" + params_2_Str(tmp) + "]";
                    //readIndex++;
                }
            }
        }
        return 1;
    }

#define SPEEDY1
#define SPEEDY2
#define right(s,r) (s.substr(s.length()-r))
    typedef map<string_q, NEXTCHUNKFUNC> parse_map_t;
    parse_map_t parseMap;
    //-----------------------------------------------------------------------------------------
    string_q parse_addr(const string_q& input, const void *data=NULL) {
        return "0x" + right(input,40);
    }
    string_q parse_bool(const string_q& input, const void *data=NULL) {
        if (input[input.length()-1] == '1')
            return "true";
        return "false";
    }
    string_q parse_i8__(const string_q& input, const void *data=NULL) {
        ostringstream os;
        os << (int)(signed char)strtol(("0x" + right(input, 2)).c_str(), NULL, 16);
        return os.str();
    }
    string_q parse_i16_(const string_q& input, const void *data=NULL) {
        ostringstream os;
        os << (int)(signed char)strtol(("0x" + right(input, 4)).c_str(), NULL, 16);
        return os.str();
    }
    string_q parse_i32_(const string_q& input, const void *data=NULL) {
        ostringstream os;
        os << (int)(signed char)strtol(("0x" + right(input, 8)).c_str(), NULL, 16);
        return os.str();
    }
    string_q parse_i64_(const string_q& input, const void *data=NULL) {
        return int_2_Str(str_2_Int("0x"+right(input,16)));
    }
    static const string_q leads48 = "000000000000000000000000000000000000000000000000";
    string_q parse_i128(const string_q& input, const void *data=NULL) {
#ifdef SPEEDY2
        if (startsWith(input, leads48))
            return parse_i64_(substitute(input, leads48, ""));
#endif
        return bni_2_Str(str_2_BigInt("0x" + input, 128));
    }
    string_q parse_i256(const string_q& input, const void *data=NULL) {
#ifdef SPEEDY2
        if (startsWith(input, leads48))
            return parse_i64_(substitute(input, leads48, ""));
#endif
        return bni_2_Str(str_2_BigInt("0x" + input, 256));
    }
    string_q parse_u8__(const string_q& input, const void *data=NULL) {
        return uint_2_Str(str_2_Uint("0x"+input.substr(input.length()-( 8/4))));
    }
    string_q parse_u16_(const string_q& input, const void *data=NULL) {
        return uint_2_Str(str_2_Uint("0x"+input.substr(input.length()-(16/4))));
    }
    string_q parse_u32_(const string_q& input, const void *data=NULL) {
        return uint_2_Str(str_2_Uint("0x"+input.substr(input.length()-(32/4))));
    }
    string_q parse_u64_(const string_q& input, const void *data=NULL) {
        return uint_2_Str(str_2_Uint("0x"+input.substr(input.length()-(64/4))));
    }
    string_q parse_u128(const string_q& input, const void *data=NULL) {
#ifdef SPEEDY2
        if (startsWith(input, "000000000000000000000000000000000000000000000000"))
            return parse_u64_(input);
#endif
        return bnu_2_Str(str_2_BigUint("0x" + input, 128));
    }
    string_q parse_u256(const string_q& input, const void *data=NULL) {
#ifdef SPEEDY2
        if (startsWith(input, "000000000000000000000000000000000000000000000000"))
            return parse_u64_(input);
#endif
        return bnu_2_Str(str_2_BigUint("0x" + input, 256));
    }
    string_q parse_by32(const string_q& input, const void *data=NULL) {
        return "0x" + input;
    }
    string_q parse_addr_addr(const string_q& input, const void *data) {
        return
        parse_addr(input.substr( 0, 64)) + "," +
        parse_addr(input.substr(64, 64));
    }
    string_q parse_addr_bool(const string_q& input, const void *data) {
        return
        parse_addr(input.substr( 0, 64)) + "," +
        parse_bool(input.substr(64, 64));
    }
    string_q parse_addr_by32(const string_q& input, const void *data) {
        return
        parse_addr(input.substr( 0, 64)) + "," +
        parse_by32(input.substr(64, 64));
    }
    string_q parse_str(const string_q& input, const void *data) {
        uint64_t len = str_2_Uint("0x"+right(input.substr(64,64),16)) * 2;
        return hex_2_Str(input.substr(128,len));
    }
    string_q parse_addr_i256(const string_q& input, const void *data) {
        return
        parse_addr(input.substr( 0, 64)) + "," +
        parse_i256(input.substr(64, 64));
    }
    string_q parse_addr_u256(const string_q& input, const void *data) {
        return
        parse_addr(input.substr( 0, 64)) + "," +
        parse_u256(input.substr(64, 64));
    }
    string_q parse_u8___u8__(const string_q& input, const void *data) {
        return
        parse_u8__(input.substr( 0, 64)) + "," +
        parse_u8__(input.substr(64, 64));
    }
    string_q parse_u16__u16_(const string_q& input, const void *data) {
        return
        parse_u16_(input.substr( 0, 64)) + "," +
        parse_u16_(input.substr(64, 64));
    }
    string_q parse_u32__u32_(const string_q& input, const void *data) {
        return
        parse_u32_(input.substr( 0, 64)) + "," +
        parse_u32_(input.substr(64, 64));
    }
    string_q parse_u64__u64_(const string_q& input, const void *data) {
        return
        parse_u64_(input.substr( 0, 64)) + "," +
        parse_u64_(input.substr(64, 64));
    }
    string_q parse_u256_addr(const string_q& input, const void *data) {
        return
        parse_u256(input.substr( 0, 64)) + "," +
        parse_addr(input.substr(64, 64));
    }
    string_q parse_u256_bool(const string_q& input, const void *data) {
        return
        parse_u256(input.substr( 0, 64)) + "," +
        parse_bool(input.substr(64, 64));
    }
    string_q parse_u256_u8__(const string_q& input, const void *data) {
        return
        parse_u256(input.substr( 0, 64)) + "," +
        parse_u8__(input.substr(64, 64));
    }
    string_q parse_u256_u256(const string_q& input, const void *data) {
        return
        parse_u256(input.substr( 0, 64)) + "," +
        parse_u256(input.substr(64, 64));
    }
    string_q parse_by32_addr(const string_q& input, const void *data) {
        return
        parse_by32(input.substr( 0, 64)) + "," +
        parse_addr(input.substr(64, 64));
    }
    string_q parse_by32_by32(const string_q& input, const void *data) {
        return
        parse_by32(input.substr( 0, 64)) + "," +
        parse_by32(input.substr(64, 64));
    }
    string_q parse_by32_u256(const string_q& input, const void *data) {
        return
        parse_by32(input.substr( 0, 64)) + "," +
        parse_u256(input.substr(64, 64));
    }
    string_q parse_addr_addr_addr(const string_q& input, const void *data) {
        return
        parse_addr(input.substr(  0, 64)) + "," +
        parse_addr(input.substr( 64, 64)) + "," +
        parse_addr(input.substr(128, 64));
    }
    string_q parse_addr_addr_u256(const string_q& input, const void *data) {
        return
        parse_addr(input.substr(  0, 64)) + "," +
        parse_addr(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64));
    }
    string_q parse_addr_u256_addr(const string_q& input, const void *data) {
        return
        parse_addr(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_addr(input.substr(128, 64));
    }
    string_q parse_addr_u256_u256(const string_q& input, const void *data) {
        return
        parse_addr(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64));
    }
    string_q parse_by32_by32_u256(const string_q& input, const void *data) {
        return
        parse_by32(input.substr(  0, 64)) + "," +
        parse_by32(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64));
    }
    string_q parse_u256_addr_u256(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_addr(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64));
    }
    string_q parse_u256_u256_u256(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64));
    }
    string_q parse_addr_addr_addr_u256(const string_q& input, const void *data) {
        return
        parse_addr(input.substr(  0, 64)) + "," +
        parse_addr(input.substr( 64, 64)) + "," +
        parse_addr(input.substr(128, 64)) + "," +
        parse_u256(input.substr(192, 64));
    }
    string_q parse_addr_u256_u256_u256(const string_q& input, const void *data) {
        return
        parse_addr(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64)) + "," +
        parse_u256(input.substr(192, 64));
    }
    string_q parse_u256_u256_u256_u256(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64)) + "," +
        parse_u256(input.substr(192, 64));
    }
    string_q parse_u256_u256_u256_u256_u256(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64)) + "," +
        parse_u256(input.substr(192, 64)) + "," +
        parse_u256(input.substr(256, 64));
    }
    string_q parse_u256_u256_u256_u256_u256_u256(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64)) + "," +
        parse_u256(input.substr(192, 64)) + "," +
        parse_u256(input.substr(256, 64)) + "," +
        parse_u256(input.substr(320, 64));
    }
    string_q parse_two_u256_one_addr(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_addr(input.substr(128, 64));
    }
    string_q parse_three_u256_one_addr(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64)) + "," +
        parse_addr(input.substr(192, 64));
    }
    string_q parse_four_u256_one_addr(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64)) + "," +
        parse_u256(input.substr(192, 64)) + "," +
        parse_addr(input.substr(256, 64));
    }
    string_q parse_four_u256_two_addr(const string_q& input, const void *data) {
        return
        parse_u256(input.substr(  0, 64)) + "," +
        parse_u256(input.substr( 64, 64)) + "," +
        parse_u256(input.substr(128, 64)) + "," +
        parse_u256(input.substr(192, 64)) + "," +
        parse_addr(input.substr(256, 64)) + "," +
        parse_addr(input.substr(320, 64));
    }

    //-----------------------------------------------------------------------------------------
    void loadParseMap(void) {
        parseMap["address"] = parse_addr;
        parseMap["bool"] = parse_bool;
        parseMap["int8"] = parse_i8__;
        parseMap["int16"] = parse_i16_;
        parseMap["int32"] = parse_i32_;
        parseMap["int64"] = parse_i64_;
        parseMap["int128"] = parse_i128;
        parseMap["int256"] = parse_i256;
        parseMap["uint8"] = parse_u8__;
        parseMap["uint16"] = parse_u16_;
        parseMap["uint32"] = parse_u32_;
        parseMap["uint64"] = parse_u64_;
        parseMap["uint128"] = parse_u128;
        parseMap["uint256"] = parse_u256;
        parseMap["bytes32"] = parse_by32;
        parseMap["address,address"] = parse_addr_addr;
        parseMap["address,bool"] = parse_addr_bool;
        parseMap["address,bytes32"] = parse_addr_by32;
        parseMap["address,int256"] = parse_addr_i256;
        parseMap["address,uint256"] = parse_addr_u256;
        parseMap["uint8,uint8"] = parse_u8___u8__;
        parseMap["uint16,uint16"] = parse_u16__u16_;
        parseMap["uint32,uint32"] = parse_u32__u32_;
        parseMap["uint64,uint64"] = parse_u64__u64_;
        parseMap["uint256,address"] = parse_u256_addr;
        parseMap["uint256,bool"] = parse_u256_bool;
        parseMap["uint256,uint8"] = parse_u256_u8__;
        parseMap["uint256,uint256"] = parse_u256_u256;
        parseMap["bytes32,address"] = parse_by32_addr;
        parseMap["bytes32,bytes32"] = parse_by32_by32;
        parseMap["bytes32,uint256"] = parse_by32_u256;
        parseMap["address,address,address"] = parse_addr_addr_addr;
        parseMap["address,address,uint256"] = parse_addr_addr_u256;
        parseMap["address,uint256,address"] = parse_addr_u256_addr;
        parseMap["address,uint256,uint256"] = parse_addr_u256_u256;
        parseMap["bytes32,bytes32,uint256"] = parse_by32_by32_u256;
        parseMap["uint256,address,uint256"] = parse_u256_addr_u256;
        parseMap["uint256,uint256,uint256"] = parse_u256_u256_u256;
        parseMap["address,address,address,uint256"] = parse_addr_addr_addr_u256;
        parseMap["address,uint256,uint256,uint256"] = parse_addr_u256_u256_u256;
        parseMap["uint256,uint256,uint256,uint256"] = parse_u256_u256_u256_u256;
        parseMap["uint256,uint256,uint256,uint256,uint256"] = parse_u256_u256_u256_u256_u256;
        parseMap["uint256,uint256,uint256,uint256,uint256,uint256"] = parse_u256_u256_u256_u256_u256_u256;
        parseMap["uint256,uint256,address"] = parse_two_u256_one_addr;
        parseMap["uint256,uint256,uint256,address"] = parse_three_u256_one_addr;
        parseMap["uint256,uint256,uint256,uint256,address"] = parse_four_u256_one_addr;
        parseMap["uint256,uint256,uint256,uint256,address,address"] = parse_four_u256_two_addr;
    }

    //---------------------------------------------------------------------------
    bool decodeRLP(CParameterArray& interfaces, const string_q& desc, const string_q& inputStr) {

        string_q built;
        // we use fast, simple routines for common patters if we can. This is purely for performance reasons
        NEXTCHUNKFUNC func = parseMap[desc];
        if (!func) {
            for (auto i : interfaces)
                built += (i.type + ",");
            built = trim(built,',');
            func = parseMap[built];
        }
        if (func) {
            if (!inputStr.empty()) {
                string_q result = (*func)(substitute(inputStr,"0x",""), NULL);
                for (auto& item : interfaces)
                    item.value = nextTokenClear(result, ',');
                return true;
            }
        }

        // Clean up the input to work with the provided interfaces - one input row per 32 bytes
        CStringArray inputs;
        if (inputStr.empty() || inputStr == "0x") {
            // In the case where the input is empty, we fill up all the fixed sized slots with zero bytes
            for (auto i : interfaces)
                inputs.push_back(string_q(64, '0'));
        } else {
            // Put each 32-byte segment into it's own string in the array
            string_q str = trim(substitute(inputStr, "0x", ""), ' ');
            ASSERT(!(str.size() % 64));
            while (!str.empty()) {
                string_q item = str.substr(0,64);
                ASSERT(items.size() == 64);
                replace(str, item, "");
                inputs.push_back(item);
            }
        }

        size_t offset = 0;
        return decodeTheData(interfaces, inputs, offset);
    }

}  // namespace qblocks
