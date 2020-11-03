#pragma once
// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_value.h"
#include "basetypes.h"
#include "logging.h"

namespace qblocks {

//--------------------------------------------------------------------------------
enum TokenType {
    tokenEndOfStream = 0,
    tokenObjectBegin,
    tokenObjectEnd,
    tokenArrayBegin,
    tokenArrayEnd,
    tokenString,
    tokenNumber,
    tokenTrue,
    tokenFalse,
    tokenNull,
    tokenNaN,
    tokenPosInf,
    tokenNegInf,
    tokenArraySeparator,
    tokenMemberSeparator,
    tokenError
};

//--------------------------------------------------------------------------------
class Token {
  public:
    TokenType type_;
    const char* start_;
    const char* end_;
};

//--------------------------------------------------------------------------------
class JsonReader {
  public:
    explicit JsonReader(void);
    bool parseString(const string_q& jsonStr, Value& root);
    bool parseStream(istream& is, Value& root);

  private:
    JsonReader(JsonReader const&);      // no copies
    void operator=(JsonReader const&);  // no copies

    bool readValue();
    bool readNumber(bool checkInf);
    bool readString();
    bool readStringSingleQuote();
    bool readObject(Token& token);
    bool readArray(Token& token);
    bool readToken(Token& token);

    bool decodeNumber(Token& token);
    bool decodeString(Token& token);
    bool decodeDouble(Token& token);
    bool decodeNumber(Token& token, Value& decoded);
    bool decodeString(Token& token, string_q& decoded);
    bool decodeDouble(Token& token, Value& decoded);

    bool decodeUnicodeCodePoint(Token& token, const char*& current, const char* end, unsigned int& unicode);
    bool decodeUnicodeEscapeSequence(Token& token, const char*& current, const char* end, unsigned int& unicode);

    bool recoverFromError(TokenType skipUntilToken);
    void skipUntilSpace();
    void skipSpaces();
    bool match(const char* pattern, int patternLength);

    Value& currentValue();
    char getNextChar();

    static string_q normalizeEOL(const char* begin, const char* end);
    static bool containsNewLine(const char* begin, const char* end);

    stack<Value*> nodes_{};
    string_q document_{};

    const char* begin_ = nullptr;
    const char* end_ = nullptr;
    const char* current_ = nullptr;
    const char* lastValueEnd_ = nullptr;
    Value* lastValue_ = nullptr;
};

}  // namespace qblocks
