// Copyright 2007-2011 Baptiste Lepilleur and The JsonCpp Authors
// Copyright (C) 2016 InfoTeCS JSC. All rights reserved.
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE
#include "json_reader.h"

namespace qblocks {

//--------------------------------------------------------------------------------
bool JsonReader::readOneObject(Token& token) {
    Value* top = nodeStack12.top();
    CBaseNode* tbTop = tbStack.top();
    string_q curName;

    Token nameTok;
    while (readNextToken(nameTok)) {
        if (nameTok.type_ == tokObjEnd && curName.empty()) {  // empty object or trailing comma
            if (tbTop)
                tbTop->finishParse();
            return true;
        }
        curName.clear();
        if (nameTok.type_ == tokString) {
            Value nameValue;
            if (!decodeString_4(nameTok, nameValue))
                return tryToRecover(tokObjEnd);
            curName = nameValue.asString();
        } else {
            break;
        }
        if (curName.length() >= (1U << 30))
            throwRuntimeError("keylength >= 2^30");
        if (top->isMember(curName)) {
            string_q msg = "Duplicate key: '" + curName + "'";
            LOG_ERR(msg);
            return tryToRecover(tokObjEnd);
        }

        Token colonTok;
        if (!readNextToken(colonTok) || colonTok.type_ != tokMemberSep) {
            LOG_ERR("Missing ':' after object member name");
            return tryToRecover(tokObjEnd);
        }

        bool ok = true;
        Value& value = top->operator[](curName);
        CBaseNode* ch = (CBaseNode*)(tbTop != NULL ? tbTop->getObjectAt(curName, 0) : NULL);
        curField.push(curName);
        string_q strVal;
        if (ch) {
            nodeStack12.push(&value);
            tbStack.push(ch);
            ok = readNextValue();
            tbStack.pop();
            nodeStack12.pop();
        } else {
            nodeStack12.push(&value);
            ok = readNextValue();
            strVal = value.asString();
            if (tbTop)
                tbTop->setValueByName(curName, strVal);
            nodeStack12.pop();
        }
        curField.pop();
        if (!ok)
            return tryToRecover(tokObjEnd);

        Token commaTok;
        if (!readNextToken(commaTok) || (commaTok.type_ != tokObjEnd && commaTok.type_ != tokArraySep)) {
            LOG_ERR("Missing ',' or '}' in object declaration");
            return tryToRecover(tokObjEnd);
        }
        if (commaTok.type_ == tokObjEnd) {
            if (tbTop)
                tbTop->finishParse();
            return true;
        }
    }
    LOG_ERR("Missing '}' or object member name");
    return tryToRecover(tokObjEnd);
}

//--------------------------------------------------------------------------------
bool JsonReader::readOneArray(Token& token) {
    Value* top = nodeStack12.top();
    CBaseNode* tbTop = tbStack.top();
    string_q curName = curField.top();

    int index = 0;
    for (;;) {
        skipWhitespace();
        if (current_ != end_ && *current_ == ']' && (index == 0)) {
            Token endArray;
            readNextToken(endArray);
            return true;
        }
        Value& value = top->operator[](index++);
        nodeStack12.push(&value);
        CBaseNode* ch = (CBaseNode*)tbTop->getObjectAt(curName, NOPOS);
        if (ch)
            tbStack.push(ch);
        bool ok = readNextValue();
        string_q strVal = value.asString();
        curField.push(curName);
        if (ch) {
            tbStack.pop();
            ch->setValueByName(curName, strVal);
        } else {
            tbTop->setValueByName(curName, strVal);
        }
        curField.pop();
        nodeStack12.pop();
        if (!ok)
            return tryToRecover(tokArrayEnd);

        Token currentToken;
        ok = readNextToken(currentToken);
        bool badTokenType = (currentToken.type_ != tokArraySep && currentToken.type_ != tokArrayEnd);
        if (!ok || badTokenType) {
            LOG_ERR("Missing ',' or ']' in array declaration");
            return tryToRecover(tokArrayEnd);
        }
        if (currentToken.type_ == tokArrayEnd)
            break;
    }

    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::readOneString(void) {
    char c = 0;
    while (current_ != end_) {
        c = *current_;
        current_++;
        if (c == '\\') {
            if (current_ != end_)
                current_++;
        } else if (c == '"') {
            break;
        }
    }
    return c == '"';
}

//--------------------------------------------------------------------------------
bool JsonReader::readNumberToken(bool checkInf) {
    const char* p = current_;
    if (checkInf && p != end_ && *p == 'I') {
        current_ = ++p;
        return false;
    }
    char c = '0';  // stopgap for already consumed character integral part
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
bool JsonReader::tryToRecover(TokenType skipUntilToken) {
    Token skip;
    for (;;) {
        readNextToken(skip);
        if (skip.type_ == skipUntilToken || skip.type_ == tokEOS)
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
void JsonReader::skipWhitespace() {
    while (current_ != end_) {
        char c = *current_;
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            ++current_;
        else
            break;
    }
}

//--------------------------------------------------------------------------------
bool JsonReader::parseJson(const string_q& jsonStr, Value& root, CBaseNode* node) {
    begin_ = jsonStr.c_str();
    end_ = jsonStr.c_str() + jsonStr.length();
    current_ = begin_;
    while (!nodeStack12.empty())
        nodeStack12.pop();
    while (!tbStack.empty())
        tbStack.pop();
    while (!curField.empty())
        curField.pop();
    nodeStack12.push(&root);
    tbStack.push(node);
    curField.push("");
    // skip byte order mark if it exists at the beginning of the UTF-8 text.
    if ((end_ - begin_) >= 3 && strncmp(begin_, "\xEF\xBB\xBF", 3) == 0) {
        begin_ += 3;
        current_ = begin_;
    }
    bool successful = readNextValue();
    curField.pop();
    tbStack.pop();
    nodeStack12.pop();
    if (!root.isArray() && !root.isObject()) {
        LOG_ERR("A valid JSON document must be either an array or an object value.");  //, token);
        return false;
    }
    return successful;
}

//--------------------------------------------------------------------------------
JsonReader::JsonReader(void) {
}

//--------------------------------------------------------------------------------
bool JsonReader::readNextValue(void) {
    if (nodeStack12.size() > 1000 || tbStack.size() > 1000 || curField.size() > 1000)
        throwRuntimeError("Exceeded stackLimit (1000) in readNextValue().");

    Value* top = nodeStack12.top();

    Token token;
    readNextToken(token);
    switch (token.type_) {
        case tokObjBegin: {
            Value init(objValType);
            swap(top->bits_, init.bits_);
            swap(top->value_, init.value_);
            top->start_ = (token.start_ - begin_);
            bool ret = readOneObject(token);
            top->limit_ = (current_ - begin_);
            return ret;
        } break;
        case tokArrayBegin: {
            Value init(arrayValType);
            swap(top->bits_, init.bits_);
            swap(top->value_, init.value_);
            top->start_ = (token.start_ - begin_);
            bool ret = readOneArray(token);
            top->limit_ = (current_ - begin_);
            return ret;
        } break;
        case tokString: {
            Value v;
            if (!decodeString_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokNumber: {
            Value v;
            if (!decodeNumber_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokNull: {
            Value v;
            if (!decodeToken_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokTrue: {
            Value v(true);
            if (!decodeToken_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokFalse: {
            Value v(false);
            if (!decodeToken_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokNaN: {
            Value v(numeric_limits<double>::quiet_NaN());
            if (!decodeToken_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokPosInf: {
            Value v(numeric_limits<double>::infinity());
            if (!decodeToken_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokNegInf: {
            Value v(-numeric_limits<double>::infinity());
            if (!decodeToken_4(token, v))
                return false;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            return true;
        } break;
        case tokArraySep:
        case tokArrayEnd:
        case tokObjEnd: {
            current_--;
            Value v;
            swap(top->bits_, v.bits_);
            swap(top->value_, v.value_);
            top->start_ = (current_ - begin_ - 1);
            top->limit_ = (current_ - begin_);
            return true;
        } break;
        default: {
            top->start_ = (token.start_ - begin_);
            top->limit_ = (token.end_ - begin_);
            LOG_ERR("Syntax error: value, object or array expected.");
            return false;
        } break;
    }
    return true;
}

//--------------------------------------------------------------------------------
bool JsonReader::readNextToken(Token& token) {
    skipWhitespace();
    token.start_ = current_;
    char c = 0;
    if (current_ != end_) {
        c = *current_;
        current_++;
    }
    bool ok = true;
    switch (c) {
        case '{':
            token.type_ = tokObjBegin;
            break;
        case '}':
            token.type_ = tokObjEnd;
            break;
        case '[':
            token.type_ = tokArrayBegin;
            break;
        case ']':
            token.type_ = tokArrayEnd;
            break;
        case '"':
            token.type_ = tokString;
            ok = readOneString();
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
            token.type_ = tokNumber;
            readNumberToken(false);
            break;
        case '-':
            if (readNumberToken(true)) {
                token.type_ = tokNumber;
            } else {
                token.type_ = tokNegInf;
                ok = match("nfinity", 7);
            }
            break;
        case '+':
            if (readNumberToken(true)) {
                token.type_ = tokNumber;
            } else {
                token.type_ = tokPosInf;
                ok = match("nfinity", 7);
            }
            break;
        case 't':
            token.type_ = tokTrue;
            ok = match("rue", 3);
            break;
        case 'f':
            token.type_ = tokFalse;
            ok = match("alse", 4);
            break;
        case 'n':
            token.type_ = tokNull;
            ok = match("ull", 3);
            break;
        case 'N':
            token.type_ = tokNaN;
            ok = match("aN", 2);
            break;
        case 'I':
            token.type_ = tokPosInf;
            ok = match("nfinity", 7);
            break;
        case ',':
            token.type_ = tokArraySep;
            break;
        case ':':
            token.type_ = tokMemberSep;
            break;
        case 0:
            token.type_ = tokEOS;
            break;
        default:
            ok = false;
            break;
    }
    if (!ok)
        token.type_ = tokError;
    token.end_ = current_;
    return ok;
}

}  // namespace qblocks
