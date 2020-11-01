// Copyright 2007-2011 Baptiste Lepilleur and The JsonCpp Authors
// Copyright (C) 2016 InfoTeCS JSC. All rights reserved.
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_reader.h"

namespace qblocks {

//--------------------------------------------------------------------------------
bool JsonReader::containsNewLine(const char* begin, const char* end) {
    return any_of(begin, end, [](char b) { return b == '\n' || b == '\r'; });
}

//--------------------------------------------------------------------------------
JsonReader::JsonReader(void) {
}

//--------------------------------------------------------------------------------
bool JsonReader::readValue() {
    if (nodes_.size() > 1000)
        throwRuntimeError("Exceeded stackLimit in readValue().");

    Token token;
    readToken(token);
    bool successful = true;

    switch (token.type_) {
        case tokenObjectBegin:
            successful = readObject(token);
            currentValue().setOffsetLimit(current_ - begin_);
            break;
        case tokenArrayBegin:
            successful = readArray(token);
            currentValue().setOffsetLimit(current_ - begin_);
            break;
        case tokenNumber:
            successful = decodeNumber(token);
            break;
        case tokenString:
            successful = decodeString(token);
            break;
        case tokenTrue: {
            Value v(true);
            currentValue().swapPayload(v);
            currentValue().setOffsetStart(token.start_ - begin_);
            currentValue().setOffsetLimit(token.end_ - begin_);
        } break;
        case tokenFalse: {
            Value v(false);
            currentValue().swapPayload(v);
            currentValue().setOffsetStart(token.start_ - begin_);
            currentValue().setOffsetLimit(token.end_ - begin_);
        } break;
        case tokenNull: {
            Value v;
            currentValue().swapPayload(v);
            currentValue().setOffsetStart(token.start_ - begin_);
            currentValue().setOffsetLimit(token.end_ - begin_);
        } break;
        case tokenNaN: {
            Value v(numeric_limits<double>::quiet_NaN());
            currentValue().swapPayload(v);
            currentValue().setOffsetStart(token.start_ - begin_);
            currentValue().setOffsetLimit(token.end_ - begin_);
        } break;
        case tokenPosInf: {
            Value v(numeric_limits<double>::infinity());
            currentValue().swapPayload(v);
            currentValue().setOffsetStart(token.start_ - begin_);
            currentValue().setOffsetLimit(token.end_ - begin_);
        } break;
        case tokenNegInf: {
            Value v(-numeric_limits<double>::infinity());
            currentValue().swapPayload(v);
            currentValue().setOffsetStart(token.start_ - begin_);
            currentValue().setOffsetLimit(token.end_ - begin_);
        } break;
        case tokenArraySeparator:
        case tokenObjectEnd:
        case tokenArrayEnd: {
            // "Un-read" the current token and mark the current value as a null token.
            current_--;
            Value v;
            currentValue().swapPayload(v);
            currentValue().setOffsetStart(current_ - begin_ - 1);
            currentValue().setOffsetLimit(current_ - begin_);
        } break;
        default:
            currentValue().setOffsetStart(token.start_ - begin_);
            currentValue().setOffsetLimit(token.end_ - begin_);
            LOG_ERR("Syntax error: value, object or array expected.");  //, token);
            return false;
    }

    return successful;
}

//--------------------------------------------------------------------------------
bool JsonReader::readToken(Token& token) {
    skipSpaces();
    token.start_ = current_;
    char c = getNextChar();
    bool ok = true;
    switch (c) {
        case '{':
            token.type_ = tokenObjectBegin;
            break;
        case '}':
            token.type_ = tokenObjectEnd;
            break;
        case '[':
            token.type_ = tokenArrayBegin;
            break;
        case ']':
            token.type_ = tokenArrayEnd;
            break;
        case '"':
            token.type_ = tokenString;
            ok = readString();
            break;
        case '\'':  // we allow the use of single quotes
            token.type_ = tokenString;
            ok = readStringSingleQuote();
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            token.type_ = tokenNumber;
            readNumber(false);
            break;
        case '-':
            if (readNumber(true)) {
                token.type_ = tokenNumber;
            } else {
                token.type_ = tokenNegInf;
                ok = match("nfinity", 7);
            }
            break;
        case '+':
            if (readNumber(true)) {
                token.type_ = tokenNumber;
            } else {
                token.type_ = tokenPosInf;
                ok = match("nfinity", 7);
            }
            break;
        case 't':
            token.type_ = tokenTrue;
            ok = match("rue", 3);
            break;
        case 'f':
            token.type_ = tokenFalse;
            ok = match("alse", 4);
            break;
        case 'n':
            token.type_ = tokenNull;
            ok = match("ull", 3);
            break;
        case 'N':
            token.type_ = tokenNaN;
            ok = match("aN", 2);
            break;
        case 'I':
            token.type_ = tokenPosInf;
            ok = match("nfinity", 7);
            break;
        case ',':
            token.type_ = tokenArraySeparator;
            break;
        case ':':
            token.type_ = tokenMemberSeparator;
            break;
        case 0:
            token.type_ = tokenEndOfStream;
            break;
        default:
            ok = false;
            break;
    }
    if (!ok)
        token.type_ = tokenError;
    token.end_ = current_;
    return ok;
}

//--------------------------------------------------------------------------------
void JsonReader::skipSpaces() {
    while (current_ != end_) {
        char c = *current_;
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            ++current_;
        else
            break;
    }
}

//--------------------------------------------------------------------------------
bool JsonReader::match(const char* pattern, int patternLength) {
    if (end_ - current_ < patternLength)
        return false;

    int index = patternLength;
    while (index--)
        if (current_[index] != pattern[index])
            return false;
    current_ += patternLength;

    return true;
}

//--------------------------------------------------------------------------------
string_q JsonReader::normalizeEOL(const char* begin, const char* end) {
    string_q normalized;
    normalized.reserve(static_cast<size_t>(end - begin));
    const char* current = begin;
    while (current != end) {
        char c = *current++;
        if (c == '\r') {
            if (current != end && *current == '\n') {
                // convert dos EOL
                ++current;
            }
            // convert Mac EOL
            normalized += '\n';
        } else {
            normalized += c;
        }
    }
    return normalized;
}

//--------------------------------------------------------------------------------
bool JsonReader::readNumber(bool checkInf) {
    const char* p = current_;
    if (checkInf && p != end_ && *p == 'I') {
        current_ = ++p;
        return false;
    }
    char c = '0';  // stopgap for already consumed character
                   // integral part
    while (c >= '0' && c <= '9')
        c = (current_ = p) < end_ ? *p++ : '\0';
    // fractional part
    if (c == '.') {
        c = (current_ = p) < end_ ? *p++ : '\0';
        while (c >= '0' && c <= '9')
            c = (current_ = p) < end_ ? *p++ : '\0';
    }
    // exponential part
    if (c == 'e' || c == 'E') {
        c = (current_ = p) < end_ ? *p++ : '\0';
        if (c == '+' || c == '-')
            c = (current_ = p) < end_ ? *p++ : '\0';
        while (c >= '0' && c <= '9')
            c = (current_ = p) < end_ ? *p++ : '\0';
    }
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::readString(void) {
    char c = 0;
    while (current_ != end_) {
        c = getNextChar();
        if (c == '\\')
            getNextChar();
        else if (c == '"')
            break;
    }
    return c == '"';
}

//--------------------------------------------------------------------------------
bool JsonReader::readStringSingleQuote(void) {
    char c = 0;
    while (current_ != end_) {
        c = getNextChar();
        if (c == '\\')
            getNextChar();
        else if (c == '\'')
            break;
    }
    return c == '\'';
}

//--------------------------------------------------------------------------------
bool JsonReader::readObject(Token& token) {
    Token tokenName;
    string_q name;
    Value init(objectValue);
    currentValue().swapPayload(init);
    currentValue().setOffsetStart(token.start_ - begin_);
    while (readToken(tokenName)) {
        if (tokenName.type_ == tokenObjectEnd && name.empty())  // empty object or trailing comma
            return true;
        name.clear();
        if (tokenName.type_ == tokenString) {
            if (!decodeString(tokenName, name))
                return recoverFromError(tokenObjectEnd);
        } else {
            break;
        }
        if (name.length() >= (1U << 30))
            throwRuntimeError("keylength >= 2^30");
        if (currentValue().isMember(name)) {
            string_q msg = "Duplicate key: '" + name + "'";
            LOG_ERR(msg);
            return recoverFromError(tokenObjectEnd);
        }

        Token colon;
        if (!readToken(colon) || colon.type_ != tokenMemberSeparator) {
            LOG_ERR("Missing ':' after object member name");
            return recoverFromError(tokenObjectEnd);
        }
        Value& value = currentValue()[name];
        nodes_.push(&value);
        bool ok = readValue();
        nodes_.pop();
        if (!ok)  // error already set
            return recoverFromError(tokenObjectEnd);

        Token comma;
        if (!readToken(comma) || (comma.type_ != tokenObjectEnd && comma.type_ != tokenArraySeparator)) {
            LOG_ERR("Missing ',' or '}' in object declaration");
            return recoverFromError(tokenObjectEnd);
        }
        if (comma.type_ == tokenObjectEnd)
            return true;
    }
    LOG_ERR("Missing '}' or object member name");
    return recoverFromError(tokenObjectEnd);
}

//--------------------------------------------------------------------------------
bool JsonReader::readArray(Token& token) {
    Value init(arrayValue);
    currentValue().swapPayload(init);
    currentValue().setOffsetStart(token.start_ - begin_);
    int index = 0;
    for (;;) {
        skipSpaces();
        if (current_ != end_ && *current_ == ']' && (index == 0))  // empty array or trailing
                                                                   // comma
        {
            Token endArray;
            readToken(endArray);
            return true;
        }
        Value& value = currentValue()[index++];
        nodes_.push(&value);
        bool ok = readValue();
        nodes_.pop();
        if (!ok)  // error already set
            return recoverFromError(tokenArrayEnd);

        Token currentToken;
        // Accept Comment after last item in the array.
        ok = readToken(currentToken);
        bool badTokenType = (currentToken.type_ != tokenArraySeparator && currentToken.type_ != tokenArrayEnd);
        if (!ok || badTokenType) {
            LOG_ERR("Missing ',' or ']' in array declaration");
            return recoverFromError(tokenArrayEnd);
        }
        if (currentToken.type_ == tokenArrayEnd)
            break;
    }
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::decodeNumber(Token& token) {
    Value decoded;
    if (!decodeNumber(token, decoded))
        return false;
    currentValue().swapPayload(decoded);
    currentValue().setOffsetStart(token.start_ - begin_);
    currentValue().setOffsetLimit(token.end_ - begin_);
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::decodeNumber(Token& token, Value& decoded) {
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
            return decodeDouble(token, decoded);

        const auto digit(static_cast<unsigned int>(c - '0'));
        if (value >= threshold) {
            // We've hit or exceeded the max value divided by 10 (rounded down). If
            // a) we've only just touched the limit, meaing value == threshold,
            // b) this is the last digit, or
            // c) it's small enough to fit in that rounding delta, we're okay.
            // Otherwise treat this number as a double to avoid overflow.
            if (value > threshold || current != token.end_ || digit > max_last_digit) {
                return decodeDouble(token, decoded);
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
bool JsonReader::decodeDouble(Token& token) {
    Value decoded;
    if (!decodeDouble(token, decoded))
        return false;
    currentValue().swapPayload(decoded);
    currentValue().setOffsetStart(token.start_ - begin_);
    currentValue().setOffsetLimit(token.end_ - begin_);
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::decodeDouble(Token& token, Value& decoded) {
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
bool JsonReader::decodeString(Token& token) {
    string_q decoded_string;
    if (!decodeString(token, decoded_string))
        return false;
    Value decoded(decoded_string);
    currentValue().swapPayload(decoded);
    currentValue().setOffsetStart(token.start_ - begin_);
    currentValue().setOffsetLimit(token.end_ - begin_);
    return true;
}

//--------------------------------------------------------------------------------
static inline string_q codePointToUTF8(unsigned int cp) {
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
bool JsonReader::decodeString(Token& token, string_q& decoded) {
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
                    if (!decodeUnicodeCodePoint(token, current, end, unicode))
                        return false;
                    decoded += codePointToUTF8(unicode);
                } break;
                default:
                    LOG_ERR("Bad escape sequence in string");  //, token, current);
                    return false;
            }
        } else {
            decoded += c;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::decodeUnicodeCodePoint(Token& token, const char*& current, const char* end, unsigned int& unicode) {
    if (!decodeUnicodeEscapeSequence(token, current, end, unicode))
        return false;
    if (unicode >= 0xD800 && unicode <= 0xDBFF) {
        if (end - current < 6) {
            LOG_ERR("additional six characters expected to parse unicode surrogate pair.");  //, token, current);
            return false;
        }
        if (*(current++) == '\\' && *(current++) == 'u') {
            unsigned int surrogatePair;
            if (decodeUnicodeEscapeSequence(token, current, end, surrogatePair)) {
                unicode = 0x10000 + ((unicode & 0x3FF) << 10) + (surrogatePair & 0x3FF);
            } else
                return false;
        } else {
            LOG_ERR("expecting another \\u token to begin the second half of a unicode surrogate pair");  //, token,
                                                                                                          // current);
            return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::decodeUnicodeEscapeSequence(Token& token, const char*& current, const char* end,
                                             unsigned int& ret_unicode) {
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
            LOG_ERR("Bad unicode escape sequence in string: hexadecimal digit expected.");  //, token, current);
            return false;
        }
    }
    ret_unicode = static_cast<unsigned int>(unicode);
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::recoverFromError(TokenType skipUntilToken) {
    Token skip;
    for (;;) {
        readToken(skip);
        if (skip.type_ == skipUntilToken || skip.type_ == tokenEndOfStream)
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
Value& JsonReader::currentValue() {
    return *(nodes_.top());
}

//--------------------------------------------------------------------------------
char JsonReader::getNextChar() {
    if (current_ == end_)
        return 0;
    return *current_++;
}

//--------------------------------------------------------------------------------
bool JsonReader::parseString(const string_q& jsonStr, Value& root) {
    begin_ = jsonStr.c_str();
    end_ = jsonStr.c_str() + jsonStr.length();
    current_ = begin_;
    lastValueEnd_ = nullptr;
    lastValue_ = nullptr;
    while (!nodes_.empty())
        nodes_.pop();
    nodes_.push(&root);

    // skip byte order mark if it exists at the beginning of the UTF-8 text.
    if ((end_ - begin_) >= 3 && strncmp(begin_, "\xEF\xBB\xBF", 3) == 0) {
        begin_ += 3;
        current_ = begin_;
    }
    bool successful = readValue();
    nodes_.pop();
    if (!root.isArray() && !root.isObject()) {
        LOG_ERR("A valid JSON document must be either an array or an object value.");  //, token);
        return false;
    }
    return successful;
}

//--------------------------------------------------------------------------------
istream& operator>>(istream& sin, Value& root) {
    ostringstream ssin;
    ssin << sin.rdbuf();
    string_q doc = ssin.str();

    JsonReader reader;
    reader.parseString(doc, root);

    return sin;
}

}  // namespace qblocks
