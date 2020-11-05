#pragma once
// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_value.h"
#include "basetypes.h"
#include "basenode.h"
#include "logging.h"

namespace qblocks {

//--------------------------------------------------------------------------------
class JsonReader {
  public:
    JsonReader(void);
    bool parseJson(const string_q& jsonStr, Value& root, CBaseNode* node);

  private:
    JsonReader(JsonReader const&) = delete;      // no copies
    void operator=(JsonReader const&) = delete;  // no copies

    bool readNextToken(Token& token);
    bool readNextValue(void);

    bool readOneString(void);
    bool readOneObject(Token& token);
    bool readOneArray(Token& token);

    bool readNumberToken(bool checkInf);

    bool tryToRecover(TokenType skipUntilToken);
    void skipWhitespace();
    bool match(const char* pattern, int patternLength);

    stack<Value*> nodeStack12{};
    stack<CBaseNode*> tbStack{};
    stack<string_q> curField{};

    const char* begin_ = nullptr;
    const char* end_ = nullptr;
    const char* current_ = nullptr;
};

}  // namespace qblocks
