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
#include <algorithm>
#include "abi.h"
#include "node.h"

//---------------------------------------------------------------------------
namespace qblocks {

extern bool toPrintable(const string_q& inHex, string_q& result);
static size_t level = 0;

void prettyPrintParams(const CParameterArray& params) {
    string_q indnt = substitute(string_q(level == 0 ? 0 : level - 1, '\t'), "\t", " ");
    uint64_t cnt = 0;
    for (auto param : params) {
        cerr << indnt << padNum3T(cnt) << ": " << param.type << (param.name.empty() ? "" : " " + param.name);
        if (!param.value.empty())
            cerr << " = " << param.value;
        cerr << endl;
        if (!param.internalType.empty() && (param.internalType != param.type)) {
            cerr << indnt << "  internalType: " << param.internalType << endl;
            if (param.components.size()) {
                cerr << indnt << "  compont.size: " << param.components.size() << endl;
                for (auto component : param.components)
                    cerr << indnt << "    " << component.type << " " << component.name << endl;
            }
        }
        cnt++;
    }
}

void prettyPrint(CParameterArray& params, const CStringArray& dataArray, const size_t& readOffset, size_t dataStart);
void prettyPrint2(CParameterArray& params, const CStringArray& dataArray, const size_t& readOffset, size_t dataStart) {
    // prettyPrintParams(params);
}
//------------------------------------------------------------------------------------------------
void prettyPrint(CParameterArray& params, const CStringArray& dataArray, const size_t& readOffset, size_t dataStart) {
    if (!isTestMode())
        return;

    string_q indnt = substitute(string_q(level == 0 ? 0 : level - 1, '\t'), "\t", " ");
    if (level == 1)
        cerr << endl;
    cerr << string_q(50, '=') << endl;
    cerr << indnt << "level: " << level << endl;
    cerr << indnt << "readOffset: " << readOffset << endl;
    cerr << indnt << "dataStart: " << dataStart << endl;
    cerr << indnt << "params.size: " << params.size() << endl;

    prettyPrintParams(params);

    uint64_t cnt = 0;
    cerr << indnt << "dataArray.size: " << dataArray.size() << endl;
    for (auto data : dataArray) {
        cerr << indnt << padNum3T(cnt) << " (0x" << (padLeft(substitute(uint_2_Hex(cnt * 32), "0x", ""), 3, '0'))
             << ") " << data;
        if (cnt == dataStart)
            cerr << " <=d";
        else if (cnt == readOffset)
            cerr << " <-r";
        cerr << endl;
        cnt++;
    }
}

//------------------------------------------------------------------------------------------------
#define LOG_DECODE_ERR(tag, l1, v1, t, l2, v2)                                                                         \
    {                                                                                                                  \
        ostringstream es;                                                                                              \
        es << "{ \"" << tag << "\": \"decodeAnObject: " << l1 << "(" << v1 << ") " << t << " " << l2 << "(" << v2      \
           << ")\"},";                                                                                                 \
        LOG_WARN(es.str());                                                                                            \
    }

//------------------------------------------------------------------------------------------------
#define LOG_DECODE_OKAY(p)                                                                                             \
    { prettyPrintParams(p); }

//------------------------------------------------------------------------------------------------
size_t decodeAnObject(CParameterArray& params, const CStringArray& dataArray, size_t& readOffset, size_t dataStart) {
    level++;
    for (auto& param : params) {
        prettyPrint(params, dataArray, readOffset, dataStart);
        if (readOffset > dataArray.size()) {
            LOG_DECODE_ERR("err3", "readOffset", readOffset, ">=", "dataArray.size", dataArray.size());
            prettyPrint2(params, dataArray, readOffset, dataStart);
            level--;
            return false;
        }

        bool isBaseType = (!contains(param.type, "[") && !(param.type == "string" || param.type == "bytes"));
        if (isBaseType) {
            if (contains(param.type, "bool")) {
                param.value = (dataArray[readOffset++][63] == '1' ? "true" : "false");
                LOG_DECODE_OKAY(params);
                continue;

            } else if (contains(param.type, "address")) {
                size_t bits = 160;
                param.value =
                    "0x" + padLeft(toLower(bnu_2_Hex(str_2_BigUint("0x" + dataArray[readOffset++], bits))), 40, '0');
                LOG_DECODE_OKAY(params);
                continue;

            } else if (contains(param.type, "uint")) {
                size_t bits = str_2_Uint(substitute(param.type, "uint", ""));
                param.value = bnu_2_Str(str_2_BigUint("0x" + dataArray[readOffset++], bits));
                LOG_DECODE_OKAY(params);
                continue;

            } else if (contains(param.type, "int")) {
                size_t bits = str_2_Uint(substitute(param.type, "int", ""));
                param.value = bni_2_Str(str_2_BigInt("0x" + dataArray[readOffset++], bits));
                LOG_DECODE_OKAY(params);
                continue;

            } else if (contains(param.type, "bytes")) {
                // this is a bytes<M> (fixed length)
                param.value = "0x" + dataArray[readOffset++];
                LOG_DECODE_OKAY(params);
                continue;

            } else if (param.type == "tuple") {
                LOG_TEST("Section: ", "tuple of type " + param.type + "--" + param.internalType, false);
#if 0
                // TODO(tjayrush): If I turn this on, the code fixes the test case called broken_unparsable (which is
                // commented), but
                // TODO(tjayrush): it breaks other tests
                size_t newStart = str_2_Uint("0x" + dataArray[readOffset]) / 32;
                readOffset++;
                if (newStart <= dataArray.size()) {
                    if (decodeAnObject(param.components, dataArray, newStart, newStart)) {
                        param.value = "{";
                        for (auto p : param.components) {
                            param.value += (param.value != "{" ? ", " : "");
                            param.value += ("\"" + p.name + "\":\"" + p.value + "\"");
                        }
                        param.value += "}";
                        LOG_DECODE_OKAY(params);
                        continue;

                    } else {
                        LOG_DECODE_ERR("err11", "decodeAnObect failed", 0, "", "", 0);
                        prettyPrint2(params, dataArray, readOffset, dataStart);
                        level--;
                        return false;
                    }
                } else {
                    LOG_DECODE_ERR("err6", "newStart", newStart, ">", "dataArray.size", dataArray.size());
                    prettyPrint2(params, dataArray, readOffset, dataStart);
                    level--;
                    return false;
                }
#else
                // size_t newStart = str_2_Uint("0x" + dataArray[readOffset]) / 32;
                // cerr << "newStart: " << newStart << " dataStart: " << dataStart << endl;
                if (decodeAnObject(param.components, dataArray, readOffset, dataStart)) {
                    param.value = "{";
                    for (auto p : param.components) {
                        param.value += (param.value != "{" ? ", " : "");
                        param.value += ("\"" + p.name + "\":\"" + p.value + "\"");
                    }
                    param.value += "}";
                    LOG_DECODE_OKAY(params);
                    continue;

                } else {
                    LOG_DECODE_ERR("err12", "decodeAnObect failed", 0, "", "", 0);
                    prettyPrint2(params, dataArray, readOffset, dataStart);
                    level--;
                    return false;
                }
#endif

            } else {
                LOG_DECODE_ERR("err7", "Unknown type", param.type, "", "", "");
                prettyPrint2(params, dataArray, readOffset, dataStart);
                level--;
                return true;  // we can just skip this
            }
            LOG_ERR("Should never happen at line ", __LINE__, " of file ", __FILE__);
            quickQuitHandler(-1);

        } else {
            if (param.type == "string" || param.type == "bytes") {
                // Strings and bytes are dynamic sized. The fixed size part resides at readOffset and points to
                // start of string. Start of string is length of string. Start of string + 1 is the string
                string_q result;
                uint64_t newStart = (str_2_Uint("0x" + dataArray[readOffset++]) / 32);
                if (newStart < dataArray.size()) {
                    uint64_t nBytes = str_2_Uint("0x" + dataArray[newStart]);
                    size_t nWords = (nBytes / 32) + 1;
                    if (nWords <= dataArray.size()) {  // some of the data sent in may be bogus, so we protext ourselves
                        for (size_t w = 0; w < nWords; w++) {
                            size_t pos = newStart + 1 + w;
                            if (pos < dataArray.size())
                                result += dataArray[pos].substr(0, nBytes * 2);  // at most 64
                            if (nBytes >= 32)
                                nBytes -= 32;
                            else
                                nBytes = 0;
                        }
                        param.value = (param.type == "string" ? hex_2_Str("0x" + result) : "0x" + result);
                        LOG_DECODE_OKAY(params);
                        continue;

                    } else {
                        LOG_DECODE_ERR("err4", "nWords", nWords, ">=", "dataArray.size", dataArray.size());
                        prettyPrintParams(params);
                        level--;
                        return false;
                    }
                } else {
                    param.value = "";  // we've run out of bytes -- protect ourselves from bad data
                    LOG_DECODE_ERR("err5", "newStart", newStart, ">=", "dataArray.size", dataArray.size());
                    prettyPrintParams(params);
                    level--;
                    return false;
                }

            } else if (endsWith(param.type, "[]")) {
                // ends with type...[]. We need to pick up the size from the data
                LOG_TEST("Section: ", "Variable array of " + param.type, false);
                size_t newStart = str_2_Uint("0x" + dataArray[readOffset++]) / 32;
                if (newStart <= dataArray.size()) {
                    CParameterArray tmp;
                    CParameter p;
                    p.type = param.type;
                    p.internalType = param.internalType;
                    p.components = param.components;
                    size_t nItems = str_2_Uint("0x" + dataArray[newStart]);
                    if (nItems > 0) {
                        replaceReverse(p.type, "[]", "[" + uint_2_Str(nItems) + "]");
                        replace(p.type, "bytes[", "bytes32[");
                        tmp.push_back(p);
                        newStart++;
                        if (decodeAnObject(tmp, dataArray, newStart, newStart - 1)) {
                            param.value = tmp[0].value;
                            LOG_DECODE_OKAY(params);
                            continue;

                        } else {
                            LOG_DECODE_ERR("err10", "decodeAnObect failed", 0, "", "", 0);
                            prettyPrint2(params, dataArray, readOffset, dataStart);
                            level--;
                            return false;
                        }
                    }
                    LOG_TEST("Zero sized array", param.name, false);
                    continue;

                } else {
                    LOG_DECODE_ERR("err8", "newStart", newStart, ">", "dataArray.size", dataArray.size());
                    prettyPrint2(params, dataArray, readOffset, dataStart);
                    level--;
                    return false;
                }
                LOG_ERR("Should never happen at line ", __LINE__, " of file ", __FILE__);
                quickQuitHandler(-1);

            } else {
                LOG_TEST("Section: ", "Fixed array of " + param.type, false);
                ASSERT(contains(param.type, "["));
                ASSERT(contains(param.type, "]"));
                string_q type = param.type;
                replaceReverse(type, "[", "|");
                replaceReverse(type, "]", "|");
                CStringArray parts;
                explode(parts, type, '|');
                string_q subType = parts[0];

                CParameterArray tmp;
                size_t nItems = str_2_Uint(parts[1]);
                if (nItems <= dataArray.size()) {
                    for (size_t i = 0; i < nItems; i++) {
                        CParameter p;
                        p.type = subType;
                        p.internalType = contains(param.internalType, "struct") ? param.internalType : subType;
                        p.components = param.components;
                        tmp.push_back(p);
                    }
                    if (decodeAnObject(tmp, dataArray, readOffset, dataStart)) {
                        param.value = "[" + params_2_Str(tmp) + "]";
                        LOG_DECODE_OKAY(params);
                        continue;

                    } else {
                        LOG_DECODE_ERR("err14", "decodeAnObect failed", 0, "", "", 0);
                        prettyPrint2(params, dataArray, readOffset, dataStart);
                        level--;
                        return false;
                    }

                } else {
                    LOG_DECODE_ERR("err9", "nItems", nItems, ">", "dataArray.size", dataArray.size());
                    prettyPrint2(params, dataArray, readOffset, dataStart);
                    level--;
                    return false;
                }
            }
        }
    }

    prettyPrint2(params, dataArray, readOffset, dataStart);
    level--;
    return true;
}

#define SPEEDY1
#define SPEEDY2
#define right(s, r) (s.length() >= r ? s.substr(s.length() - r) : "")  // NOLINT
#define middle(str, s, e) (str.length() >= s ? str.substr(s, e) : "")  // NOLINT
typedef map<string_q, NEXTCHUNKFUNC> parse_map_t;
parse_map_t parseMap;
//-----------------------------------------------------------------------------------------
string_q parse_addr(const string_q& input, const void* data = NULL) {
    return "0x" + right(input, 40);
}
string_q parse_bool(const string_q& input, const void* data = NULL) {
    if (right(input, 1) == "1")
        return "true";
    return "false";
}
string_q parse_i8__(const string_q& input, const void* data = NULL) {
    ostringstream os;
    os << (int)(signed char)strtol(("0x" + right(input, 2)).c_str(), NULL, 16);  // NOLINT
    return os.str();
}
string_q parse_i16_(const string_q& input, const void* data = NULL) {
    ostringstream os;
    os << (int)(signed char)strtol(("0x" + right(input, 4)).c_str(), NULL, 16);  // NOLINT
    return os.str();
}
string_q parse_i32_(const string_q& input, const void* data = NULL) {
    ostringstream os;
    os << (int)(signed char)strtol(("0x" + right(input, 8)).c_str(), NULL, 16);  // NOLINT
    return os.str();
}
string_q parse_i64_(const string_q& input, const void* data = NULL) {
    return int_2_Str(str_2_Int("0x" + right(input, 16)));
}
static const string_q leads48 = "000000000000000000000000000000000000000000000000";
string_q parse_i128(const string_q& input, const void* data = NULL) {
#ifdef SPEEDY2
    if (startsWith(input, leads48))
        return parse_i64_(substitute(input, leads48, ""));
#endif
    return bni_2_Str(str_2_BigInt("0x" + input, 128));
}
string_q parse_i256(const string_q& input, const void* data = NULL) {
#ifdef SPEEDY2
    if (startsWith(input, leads48))
        return parse_i64_(substitute(input, leads48, ""));
#endif
    return bni_2_Str(str_2_BigInt("0x" + input, 256));
}
string_q parse_u8__(const string_q& input, const void* data = NULL) {
    return uint_2_Str(str_2_Uint("0x" + middle(input, input.length() - (8 / 4), string::npos)));
}
string_q parse_u16_(const string_q& input, const void* data = NULL) {
    return uint_2_Str(str_2_Uint("0x" + middle(input, input.length() - (16 / 4), string::npos)));
}
string_q parse_u32_(const string_q& input, const void* data = NULL) {
    return uint_2_Str(str_2_Uint("0x" + middle(input, input.length() - (32 / 4), string::npos)));
}
string_q parse_u64_(const string_q& input, const void* data = NULL) {
    return uint_2_Str(str_2_Uint("0x" + middle(input, input.length() - (64 / 4), string::npos)));
}
string_q parse_u128(const string_q& input, const void* data = NULL) {
#ifdef SPEEDY2
    if (startsWith(input, "000000000000000000000000000000000000000000000000"))
        return parse_u64_(input);
#endif
    return bnu_2_Str(str_2_BigUint("0x" + input, 128));
}
string_q parse_u256(const string_q& input, const void* data = NULL) {
#ifdef SPEEDY2
    if (startsWith(input, "000000000000000000000000000000000000000000000000"))
        return parse_u64_(input);
#endif
    return bnu_2_Str(str_2_BigUint("0x" + input, 256));
}
string_q parse_by32(const string_q& input, const void* data = NULL) {
    string_q ret;
    if (toPrintable(input, ret))
        return ret;
    return "0x" + input;
}
string_q parse_addr_addr(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64));
}
string_q parse_addr_bool(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_bool(middle(input, 64, 64));
}
string_q parse_addr_by32(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_by32(middle(input, 64, 64));
}
string_q parse_str(const string_q& input, const void* data) {
    uint64_t len = str_2_Uint("0x" + right(middle(input, 64, 64), 16)) * 2;
    return hex_2_Str(middle(input, 128, len));
}
string_q parse_addr_i256(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_i256(middle(input, 64, 64));
}
string_q parse_addr_u256(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64));
}
string_q parse_u8___u8__(const string_q& input, const void* data) {
    return parse_u8__(middle(input, 0, 64)) + "," + parse_u8__(middle(input, 64, 64));
}
string_q parse_u16__u16_(const string_q& input, const void* data) {
    return parse_u16_(middle(input, 0, 64)) + "," + parse_u16_(middle(input, 64, 64));
}
string_q parse_u32__u32_(const string_q& input, const void* data) {
    return parse_u32_(middle(input, 0, 64)) + "," + parse_u32_(middle(input, 64, 64));
}
string_q parse_u64__u64_(const string_q& input, const void* data) {
    return parse_u64_(middle(input, 0, 64)) + "," + parse_u64_(middle(input, 64, 64));
}
string_q parse_u112_u112(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64));
}
string_q parse_u256_addr(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64));
}
string_q parse_u256_bool(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_bool(middle(input, 64, 64));
}
string_q parse_u256_u8__(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u8__(middle(input, 64, 64));
}
string_q parse_u256_u256(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64));
}
string_q parse_by32_addr(const string_q& input, const void* data) {
    return parse_by32(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64));
}
string_q parse_by32_by32(const string_q& input, const void* data) {
    return parse_by32(middle(input, 0, 64)) + "," + parse_by32(middle(input, 64, 64));
}
string_q parse_by32_u256(const string_q& input, const void* data) {
    return parse_by32(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64));
}
string_q parse_addr_addr_addr(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64)) + "," +
           parse_addr(middle(input, 128, 64));
}
string_q parse_addr_addr_u256(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64));
}
string_q parse_addr_u256_addr(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_addr(middle(input, 128, 64));
}
string_q parse_addr_u256_u256(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64));
}
string_q parse_by32_by32_u256(const string_q& input, const void* data) {
    return parse_by32(middle(input, 0, 64)) + "," + parse_by32(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64));
}
string_q parse_u256_addr_u256(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64));
}
string_q parse_u256_u256_u256(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64));
}
string_q parse_addr_addr_addr_u256(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64)) + "," +
           parse_addr(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64));
}
string_q parse_addr_addr_u256_u256(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_addr(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64));
}
string_q parse_addr_u256_u256_u256(const string_q& input, const void* data) {
    return parse_addr(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64));
}
string_q parse_u256_u256_u256_u256(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64));
}
string_q parse_u256_u256_u256_u256_u256(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64)) + "," +
           parse_u256(middle(input, 256, 64));
}
string_q parse_u256_u256_u256_u256_u256_u256(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64)) + "," +
           parse_u256(middle(input, 256, 64)) + "," + parse_u256(middle(input, 320, 64));
}
string_q parse_two_u256_one_addr(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_addr(middle(input, 128, 64));
}
string_q parse_three_u256_one_addr(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_addr(middle(input, 192, 64));
}
string_q parse_four_u256_one_addr(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64)) + "," +
           parse_addr(middle(input, 256, 64));
}
string_q parse_four_u256_two_addr(const string_q& input, const void* data) {
    return parse_u256(middle(input, 0, 64)) + "," + parse_u256(middle(input, 64, 64)) + "," +
           parse_u256(middle(input, 128, 64)) + "," + parse_u256(middle(input, 192, 64)) + "," +
           parse_addr(middle(input, 256, 64)) + "," + parse_addr(middle(input, 320, 64));
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
    parseMap["uint112,uint112"] = parse_u112_u112;
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
    parseMap["address,address,uint256,uint256"] = parse_addr_addr_u256_u256;
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
bool decodeRLP(CParameterArray& params, const string_q& desc, const string_q& inputStrIn) {
    string_q typeList = desc;
    string_q inputStr = (inputStrIn == "0x" ? "" : inputStrIn);
    LOG_TEST("decodeRLP--------------------------------------", "", false);
    LOG_TEST("typeList: ", typeList, false);
    LOG_TEST("inputStr: ", inputStr, false);

    // The parseMap contains very fast parsing functions for known typeLists. If we find a parsing function
    // in parseMap, we use it since it's so much faster (see below)...
    NEXTCHUNKFUNC func = parseMap[typeList];
    if (!func) {
        // If we don't find a parsing function directly from the user provided typeList, we try to build
        // a typeList from the supplied parameter array...
        typeList = "";
        for (auto i : params)
            typeList += (i.type + ",");
        typeList = trim(typeList, ',');
        func = parseMap[typeList];
    }

    // If we have a parsing function and a non-empty inputString, use it...
    if (func) {
        if (!inputStr.empty()) {
            LOG_TEST("Parse func found: ", typeList, false);
            string_q result = (*func)(substitute(inputStr, "0x", ""), NULL);
            for (auto& item : params)
                item.value = nextTokenClear(result, ',');
            return true;
        }
    }

    // We did not find a parsing function. Here we clean up the input to work with the
    // provided params -- one input row per 32 bytes (64 chars).
    CStringArray inputs;
    if (inputStr.empty() || inputStr == "0x") {
        // There is nothing to stop the user from providing an ABI that disagrees with the actual data on
        // chain. We try to handle that where we can, such as here, where we insert '0' value strings to
        // accomodate whatever params we are given.
        for (auto unused : params)
            inputs.push_back(string_q(64, '0'));

    } else {
        // Put each 32-byte segment into it's own string in the array
        string_q str = trim(substitute(inputStr, "0x", ""), ' ');
        ASSERT(!(str.size() % 64));
        while (!str.empty()) {
            string_q item = str.substr(0, 64);
            ASSERT(items.size() == 64);
            replace(str, item, "");
            inputs.push_back(item);
        }
    }

    size_t readOffset = 0;
    size_t dataStart = 0;
    prettyPrint(params, inputs, readOffset, dataStart);
    return decodeAnObject(params, inputs, readOffset, dataStart);
}

}  // namespace qblocks
