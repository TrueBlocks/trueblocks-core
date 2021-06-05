// Copyright 2007-2011 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_value.h"
#include "json_writer.h"
#include "basetypes.h"
#include "logging.h"

namespace qblocks {

//--------------------------------------------------------------------------------
string_q codePointToUTF8(unsigned int cp) {
    string_q result;
    if (cp <= 0x7f) {
        result.resize(1);
        result[0] = static_cast<char>(cp);
    } else if (cp <= 0x7FF) {
        result.resize(2);
        result[1] = static_cast<char>(0x80 | (0x3f & cp));
        result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
    } else if (cp <= 0xFFFF) {
        result.resize(3);
        result[2] = static_cast<char>(0x80 | (0x3f & cp));
        result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
        result[0] = static_cast<char>(0xE0 | (0xf & (cp >> 12)));
    } else if (cp <= 0x10FFFF) {
        result.resize(4);
        result[3] = static_cast<char>(0x80 | (0x3f & cp));
        result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
        result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
        result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
    }
    return result;
}

//--------------------------------------------------------------------------------
bool decodeUniEscSeq_4(Token& token, const char*& current, const char* end, unsigned int& ret) {
    if (end - current < 4) {
        LOG_ERR("Bad unicode escape sequence in string: four digits expected.");  //, token, current);
        return false;
    }
    int unicode = 0;
    for (int index = 0; index < 4; ++index) {
        char c = *current++;
        unicode *= 16;
        if (c >= '0' && c <= '9')
            unicode += c - '0';
        else if (c >= 'a' && c <= 'f')
            unicode += c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            unicode += c - 'A' + 10;
        else {
            LOG_ERR("Bad unicode escape sequence in string: hexadecimal digit expected.");
            return false;
        }
    }
    ret = static_cast<unsigned int>(unicode);
    return true;
}

//--------------------------------------------------------------------------------
bool decodeUniCodePt_4(Token& token, const char*& current, const char* end, unsigned int& unicode) {
    if (!decodeUniEscSeq_4(token, current, end, unicode))
        return false;
    if (unicode >= 0xD800 && unicode <= 0xDBFF) {
        if (end - current < 6) {
            LOG_ERR("additional six characters expected to parse unicode surrogate pair.");
            return false;
        }

        if (*(current++) == '\\' && *(current++) == 'u') {
            unsigned int surrogatePair;
            if (decodeUniEscSeq_4(token, current, end, surrogatePair)) {
                unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
            } else
                return false;

        } else {
            LOG_ERR("expecting another \\u token to begin the second half of a unicode surrogate pair");
            return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
bool decodeString_4(Token& token, Value& vvvv) {
    string_q decoded;
    decoded.reserve(static_cast<size_t>(token.end_ - token.start_ - 2));
    const char* current = token.start_ + 1;  // skip '"'
    const char* end = token.end_ - 1;        // do not include '"'
    while (current != end) {
        char c = *current++;
        if (c == '"')
            break;
        if (c == '\\') {
            if (current == end) {
                LOG_ERR("Empty escape sequence in string");  //, token, current);
                vvvv = decoded;
                return false;
            }
            char escape = *current++;
            switch (escape) {
                case '"':
                    decoded += '"';
                    break;
                case '/':
                    decoded += '/';
                    break;
                case '\\':
                    decoded += '\\';
                    break;
                case 'b':
                    decoded += '\b';
                    break;
                case 'f':
                    decoded += '\f';
                    break;
                case 'n':
                    decoded += '\n';
                    break;
                case 'r':
                    decoded += '\r';
                    break;
                case 't':
                    decoded += '\t';
                    break;
                case 'u': {
                    unsigned int unicode;
                    if (!decodeUniCodePt_4(token, current, end, unicode)) {
                        vvvv = decoded;
                        return false;
                    }
                    decoded += codePointToUTF8(unicode);
                } break;
                default:
                    LOG_ERR("Bad escape sequence in string");  //, token, current);
                    vvvv = decoded;
                    return false;
            }
        } else {
            decoded += c;
        }
    }
    vvvv = decoded;
    return true;
}

//--------------------------------------------------------------------------------
bool decodeDouble_4(Token& token, Value& decoded) {
    double value = 0;
    const string_q buffer(token.start_, token.end_);
    istringstream is(buffer);
    if (!(is >> value)) {
        LOG_ERR("'" + string_q(token.start_, token.end_) + "' is not a number.");  //, token);
        return false;
    }
    decoded = value;
    return true;
}

//--------------------------------------------------------------------------------
bool decodeNumber_4(Token& token, Value& decoded) {
    // Attempts to parse the number as an integer. If the number is
    // larger than the maximum supported value of an integer then
    // we decode the number as a double.
    const char* current = token.start_;
    const bool isNegative = *current == '-';
    if (isNegative) {
        ++current;
    }

    // We assume we can represent the largest and smallest integer types as
    // unsigned integers with separate sign. This is only true if they can fit
    // into an unsigned integer.
    static_assert(Value::maxLargestInt <= Value::maxLargestUInt, "Int must be smaller than unsigned int");

    // We need to convert minLargestInt into a positive number. The easiest way
    // to do this conversion is to assume our "threshold" value of minLargestInt
    // divided by 10 can fit in maxLargestInt when absolute valued. This should
    // be a safe assumption.
    static_assert(Value::minLargestInt <= -Value::maxLargestInt,
                  "The absolute value of minLargestInt must be greater than or "
                  "equal to maxLargestInt");
    static_assert(Value::minLargestInt / 10 >= -Value::maxLargestInt,
                  "The absolute value of minLargestInt must be only 1 magnitude "
                  "larger than maxLargest Int");

    static constexpr uint64_t positive_threshold = Value::maxLargestUInt / 10;
    static constexpr unsigned int positive_last_digit = Value::maxLargestUInt % 10;

    // For the negative values, we have to be more careful. Since typically
    // -Value::minLargestInt will cause an overflow, we first divide by 10 and
    // then take the inverse. This assumes that minLargestInt is only a single
    // power of 10 different in magnitude, which we check above. For the last
    // digit, we take the modulus before negating for the same reason.
    static constexpr auto negative_threshold = uint64_t(-(Value::minLargestInt / 10));
    static constexpr auto negative_last_digit = (unsigned int)(-(Value::minLargestInt % 10));

    const uint64_t threshold = isNegative ? negative_threshold : positive_threshold;
    const unsigned int max_last_digit = isNegative ? negative_last_digit : positive_last_digit;

    uint64_t value = 0;
    while (current < token.end_) {
        char c = *current++;
        if (c < '0' || c > '9')
            return decodeDouble_4(token, decoded);

        const auto digit(static_cast<unsigned int>(c - '0'));
        if (value >= threshold) {
            // We've hit or exceeded the max value divided by 10 (rounded down). If
            // a) we've only just touched the limit, meaing value == threshold,
            // b) this is the last digit, or
            // c) it's small enough to fit in that rounding delta, we're okay.
            // Otherwise treat this number as a double to avoid overflow.
            if (value > threshold || current != token.end_ || digit > max_last_digit) {
                return decodeDouble_4(token, decoded);
            }
        }
        value = value * 10 + digit;
    }

    if (isNegative) {
        // We use the same magnitude assumption here, just in case.
        const auto last_digit = static_cast<unsigned int>(value % 10);
        decoded = -int64_t(value / 10) * 10 - last_digit;
    } else if (value <= uint64_t(Value::maxLargestInt)) {
        decoded = int64_t(value);
    } else {
        decoded = value;
    }

    return true;
}

//--------------------------------------------------------------------------------
bool decodeToken_4(Token& token, Value& decoded) {
    return true;
}

}  // namespace qblocks
