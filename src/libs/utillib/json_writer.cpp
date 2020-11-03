// Copyright 2011 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_writer.h"
#include "basetypes.h"
#include "conversions.h"

namespace qblocks {

//--------------------------------------------------------------------------------
static bool doesAnyCharRequireEscaping(char const* s, size_t n) {
    assert(s || !n);
    return any_of(s, s + n, [](unsigned char c) { return c == '\\' || c == '"' || c < 0x20 || c > 0x7F; });
}

//--------------------------------------------------------------------------------
static unsigned int utf8ToCodepoint(const char*& s, const char* e) {
    const unsigned int REPLACEMENT_CHARACTER = 0xFFFD;

    unsigned int firstByte = static_cast<unsigned char>(*s);

    if (firstByte < 0x80)
        return firstByte;

    if (firstByte < 0xE0) {
        if (e - s < 2)
            return REPLACEMENT_CHARACTER;

        unsigned int calculated = ((firstByte & 0x1F) << 6) | (static_cast<unsigned int>(s[1]) & 0x3F);
        s += 1;
        // oversized encoded characters are invalid
        return calculated < 0x80 ? REPLACEMENT_CHARACTER : calculated;
    }

    if (firstByte < 0xF0) {
        if (e - s < 3)
            return REPLACEMENT_CHARACTER;

        unsigned int calculated = ((firstByte & 0x0F) << 12) | ((static_cast<unsigned int>(s[1]) & 0x3F) << 6) |
                                  (static_cast<unsigned int>(s[2]) & 0x3F);
        s += 2;
        // surrogates aren't valid codepoints itself
        // shouldn't be UTF-8 encoded
        if (calculated >= 0xD800 && calculated <= 0xDFFF)
            return REPLACEMENT_CHARACTER;
        // oversized encoded characters are invalid
        return calculated < 0x800 ? REPLACEMENT_CHARACTER : calculated;
    }

    if (firstByte < 0xF8) {
        if (e - s < 4)
            return REPLACEMENT_CHARACTER;

        unsigned int calculated = ((firstByte & 0x07) << 18) | ((static_cast<unsigned int>(s[1]) & 0x3F) << 12) |
                                  ((static_cast<unsigned int>(s[2]) & 0x3F) << 6) |
                                  (static_cast<unsigned int>(s[3]) & 0x3F);
        s += 3;
        // oversized encoded characters are invalid
        return calculated < 0x10000 ? REPLACEMENT_CHARACTER : calculated;
    }

    return REPLACEMENT_CHARACTER;
}

//--------------------------------------------------------------------------------
static const char hex2[] =
    "000102030405060708090a0b0c0d0e0f"
    "101112131415161718191a1b1c1d1e1f"
    "202122232425262728292a2b2c2d2e2f"
    "303132333435363738393a3b3c3d3e3f"
    "404142434445464748494a4b4c4d4e4f"
    "505152535455565758595a5b5c5d5e5f"
    "606162636465666768696a6b6c6d6e6f"
    "707172737475767778797a7b7c7d7e7f"
    "808182838485868788898a8b8c8d8e8f"
    "909192939495969798999a9b9c9d9e9f"
    "a0a1a2a3a4a5a6a7a8a9aaabacadaeaf"
    "b0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
    "c0c1c2c3c4c5c6c7c8c9cacbcccdcecf"
    "d0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
    "e0e1e2e3e4e5e6e7e8e9eaebecedeeef"
    "f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";

//--------------------------------------------------------------------------------
static string_q toHex16Bit(unsigned int x) {
    const unsigned int hi = (x >> 8) & 0xff;
    const unsigned int lo = x & 0xff;
    string_q result(4, ' ');
    result[0] = hex2[2 * hi];
    result[1] = hex2[2 * hi + 1];
    result[2] = hex2[2 * lo];
    result[3] = hex2[2 * lo + 1];
    return result;
}

//--------------------------------------------------------------------------------
static void appendHex(string_q& result, unsigned ch) {
    result.append("\\u").append(toHex16Bit(ch));
}

//--------------------------------------------------------------------------------
static string_q valueToQuotedStringN(const char* value, unsigned length) {
    if (value == nullptr)
        return "";

    if (!doesAnyCharRequireEscaping(value, length))
        return string_q("\"") + value + "\"";

    // We have to walk value and escape any special characters.
    // Appending to string is not efficient, but this should be rare.
    // (Note: forward slashes are *not* rare, but I am not escaping them.)
    string_q::size_type maxsize = length * 2 + 3;  // allescaped+quotes+NULL
    string_q result;
    result.reserve(maxsize);  // to avoid lots of mallocs
    result += "\"";
    char const* end = value + length;
    for (const char* c = value; c != end; ++c) {
        switch (*c) {
            case '\"':
                result += "\\\"";
                break;
            case '\\':
                result += "\\\\";
                break;
            case '\b':
                result += "\\b";
                break;
            case '\f':
                result += "\\f";
                break;
            case '\n':
                result += "\\n";
                break;
            case '\r':
                result += "\\r";
                break;
            case '\t':
                result += "\\t";
                break;
                // case '/':
                // Even though \/ is considered a legal escape in JSON, a bare
                // slash is also legal, so I see no reason to escape it.
                // (I hope I am not misunderstanding something.)
                // blep notes: actually escaping \/ may be useful in javascript to avoid </
                // sequence.
                // Should add a flag to allow this compatibility mode and prevent this
                // sequence from occurring.
            default: {
                unsigned codepoint = utf8ToCodepoint(c, end);  // modifies `c`
                if (codepoint < 0x20) {
                    appendHex(result, codepoint);
                } else if (codepoint < 0x80) {
                    result += static_cast<char>(codepoint);
                } else if (codepoint < 0x10000) {
                    // Basic Multilingual Plane
                    appendHex(result, codepoint);
                } else {
                    // Extended Unicode. Encode 20 bits as a surrogate pair.
                    codepoint -= 0x10000;
                    appendHex(result, 0xd800 + ((codepoint >> 10) & 0x3ff));
                    appendHex(result, 0xdc00 + (codepoint & 0x3ff));
                }
            } break;
        }
    }
    result += "\"";
    return result;
}

//--------------------------------------------------------------------------------
int StreamWriter::write(ostream& sout, const Value& root) {
    sout << '\n' << indentStr();
    writeValue(sout, root);
    return 0;
}

//--------------------------------------------------------------------------------
void StreamWriter::writeValue(ostream& sout, const Value& value) {
    switch (value.type()) {
        case nullValue:
            sout << "null";
            break;
        case intValue:
            sout << int_2_Str(value.asInt64());
            break;
        case uintValue:
            sout << uint_2_Str(value.asUInt64());
            break;
        case realValue:
            sout << double_2_Str(value.asDouble());
            break;
        case booleanValue:
            sout << bool_2_Str(value.asBool());
            break;
        case stringValue: {
            // Is NULL is possible for value.string_? No.
            char const* str;
            char const* end;
            bool ok = value.getString(&str, &end);
            if (ok)
                sout << valueToQuotedStringN(str, static_cast<unsigned>(end - str));
            else
                sout << "";
            break;
        }
        case arrayValue:
            if (value.size() == 0) {
                sout << "[]";
            } else {
                sout << '\n' << indentStr() << "[";
                indent();
                bool hasChildValue = !childValues_.empty();
                unsigned index = 0;
                for (;;) {
                    Value const& childValue = value[index];
                    if (hasChildValue) {
                        sout << '\n' << indentStr();
                        sout << childValues_[index];
                    } else {
                        sout << '\n' << indentStr();
                        writeValue(sout, childValue);
                    }
                    if (++index == value.size()) {
                        break;
                    }
                    sout << ",";
                }
                unindent();
                sout << '\n' << indentStr() << "]";
            }
            break;
        case objectValue: {
            Value::Members members(value.getMemberNames());
            if (members.empty())
                sout << "{}";
            else {
                sout << '\n' << indentStr() << "{";
                indent();
                auto it = members.begin();
                for (;;) {
                    string_q const& name = *it;
                    Value const& childValue = value[name];
                    auto valStr = valueToQuotedStringN(name.data(), static_cast<unsigned>(name.length()));
                    sout << '\n' << indentStr() << valStr << ": ";
                    writeValue(sout, childValue);
                    if (++it == members.end()) {
                        break;
                    }
                    sout << ",";
                }
                unindent();
                sout << '\n' << indentStr() << "}";
            }
        } break;
    }
}

}  // namespace qblocks
