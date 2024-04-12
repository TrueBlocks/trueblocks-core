/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "basetypes.h"
#include "basenode.h"
#include "sfarchive.h"
#include "conversions.h"
#include "runtimeclass.h"

namespace qblocks {

//--------------------------------------------------------------------------------
CRuntimeClass CBaseNode::classCBaseNode;

//--------------------------------------------------------------------------------
CBaseNode::CBaseNode(void) {
    initialize();
}

//--------------------------------------------------------------------------------
CBaseNode::~CBaseNode(void) {
}

//--------------------------------------------------------------------------------
void CBaseNode::initialize(void) {
    m_deleted = false;
    m_schema = 20000 + 500 + 8;
    m_showing = true;
}

//--------------------------------------------------------------------------------
void CBaseNode::duplicate(const CBaseNode& bn) {
    m_deleted = bn.m_deleted;
    m_schema = bn.m_schema;
    m_showing = bn.m_showing;
}

//--------------------------------------------------------------------------------
bool CBaseNode::isDeleted(void) const {
    return (m_deleted);
}

//--------------------------------------------------------------------------------
void CBaseNode::setDeleted(bool del) {
    m_deleted = del;
}

//--------------------------------------------------------------------------------
CRuntimeClass* CBaseNode::getRuntimeClass() const {
    return &CBaseNode::classCBaseNode;
}

//--------------------------------------------------------------------------------
bool CBaseNode::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cname") {
                // CRuntimeClass* pClass = getRuntimeClass();
                // return extract(string_q(pClass->m_ClassName), 1);
            }
            break;
        case 'd':
            if (fieldName % "deleted") {
                m_deleted = str_2_Bool(fieldValue);
                return true;
            }
            break;
        case 's':
            if (fieldName % "schema") {
                // return uint_2_Str(m_schema);
            }
            if (fieldName % "showing") {
                m_showing = str_2_Bool(fieldValue);
                return true;
            }
            break;
    }
    return false;
}

//--------------------------------------------------------------------------------
string_q CBaseNode::getValueByName(const string_q& fieldName) const {
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cname") {
                CRuntimeClass* pClass = getRuntimeClass();
                return extract(string_q(pClass->m_ClassName), 1);
            }
            break;
        case 'd':
            if (fieldName % "deleted")
                return bool_2_Str(m_deleted);
            break;
        case 's':
            if (fieldName % "schema")
                return uint_2_Str(m_schema);
            if (fieldName % "showing")
                return bool_2_Str(m_showing);
            break;
    }

    return "";
}

//--------------------------------------------------------------------
inline bool isWhiteSpace(char c) {
    return (c == '\0' || c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

//--------------------------------------------------------------------------------
char* cleanUpJson(char* s) {
    if (!s)
        return s;
    char *l = s, *start = s;
    while (*s) {
        // NOTE: A generic JSON parser won't do this, but since the Ethereum data
        // contains no preservable white space, it's okay here.
        if (!isWhiteSpace(*s) && *s != '\"') {  // zap all the white space and quotes
            *l = *s;
            l++;
        }
        s++;
    }
    *l = '\0';
    return start;
}

//--------------------------------------------------------------------------------
bool isEmptyObj(const string_q& str) {
    bool startToken = false;
    bool endToken = false;
    char* s = (char*)str.c_str();  // NOLINT
    while (*s) {
        switch (*s) {
            case '{':
                startToken = true;
                break;
            case '}':
                endToken = true;
                break;
            default:
                if (!isWhiteSpace(*s))
                    return false;
        }
        s++;
    }
    return startToken && endToken;
}

//--------------------------------------------------------------------------------
string_q nextBasenodeChunk(const string_q& fieldIn, const CBaseNode* node) {
    if (node) {
        string_q className = node->getRuntimeClass()->getClassNamePtr();
        switch (tolower(fieldIn[0])) {
            case 'd':
                if (fieldIn % "deleted")
                    return int_2_Str(node->isDeleted());
                break;
            case 'n':
                if (fieldIn % "null")
                    return "<x>";
                break;
            case 'p':
                if (fieldIn % "parsed") {
                    CRuntimeClass* pClass = node->getRuntimeClass();
                    if (!pClass || pClass->fieldList.size() == 0) {
                        return "";
                    }
                    ostringstream os;
                    return os.str();
                }
                break;
            case 's':
                if (fieldIn % "schema")
                    return uint_2_Str(node->m_schema);
                if (fieldIn % "showing")
                    return uint_2_Str(node->m_showing);
                break;
            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------------------------
string_q getNextChunk(string_q& fmtOut, NEXTCHUNKFUNC func, const void* data) {
    string_q chunk = fmtOut;
    if (!contains(fmtOut, "[")) {
        // There are no more tokens. Return the last chunk and empty out the format
        fmtOut = "";
        return chunk;
    }

    if (!startsWith(fmtOut, '[')) {
        // We've encountered plain text outside of a token. There is more to process so grab
        // the next chunk and then prepare the remaining chunk by prepending the token.
        // The next time through we will hit the token.
        chunk = nextTokenClear(fmtOut, '[', false);
        fmtOut = "[" + fmtOut;
        return chunk;
    }

    // We've hit a token, toss the start token, look for a field and toss the last token
    // leaving the remainder of the format in fmtOut. A field is found if we find a pair
    // of squigglies).  Save text inside the start token and outside the field in pre and post
    ASSERT(startsWith(fmtOut, '['));

    string_q pre, fieldName, post;
    nextTokenClear(fmtOut, '[', false);  // toss the start token
    if (contains(chunk, "{")) {
        // we've encountered a field
        pre = nextTokenClear(fmtOut, '{', false);
        fieldName = nextTokenClear(fmtOut, '}', false);
        post = nextTokenClear(fmtOut, ']', false);
    } else {
        // we've encountered a token with no field inside of it.  Just pull off
        // the entire contents into post.  It will be returned just below.
        post = nextTokenClear(fmtOut, ']', false);
    }

    // Either no squigglies were found or an empty pair of squigglies were found.  In either
    // case return surrounding text (text inside the token and outside the squiggles)
    if (fieldName.empty())
        return pre + post;

    // We have a field so lets process it.
    ASSERT(fieldName.length() >= 2);
    bool isPrompt = false;

    // The fieldname may contain b: in which case the field is a bool. Display only 'true' values
    // (in other words, false is same as empty)
    bool isBool = false;
    if (contains(fieldName, "b:")) {
        isBool = true;
        replace(fieldName, "b:", "");
    }

    // The fieldname may contain p: or w:width: or both.  If it contains either it
    // must contain them at the beginning of the string (before the fieldName).  Anything
    // left after the last ':' is considered the fieldName
    string_q promptName = fieldName;
    if (contains(fieldName, "p:")) {
        isPrompt = true;
        replace(fieldName, "p:", "");
        promptName = fieldName;
    }

    size_t maxWidth = 0xdeadbeef, lineWidth = 0xdeadbeef;
    bool rightJust = false, lineJust = false, zeroJust = false;
    if (contains(fieldName, "w:")) {
        ASSERT(extract(fieldName, 0, 2) % "w:");  // must be first modifier in the string
        replace(fieldName, "w:", "");             // get rid of the 'w:'
        maxWidth = str_2_Uint(fieldName);         // grab the width
        nextTokenClear(fieldName, ':');           // skip to the start of the fieldname
    } else if (contains(fieldName, "r:")) {
        ASSERT(extract(fieldName, 0, 2) % "r:");  // must be first modifier in the string
        replace(fieldName, "r:", "");             // get rid of the 'w:'
        maxWidth = str_2_Uint(fieldName);         // grab the width
        nextTokenClear(fieldName, ':');           // skip to the start of the fieldname
        rightJust = true;
    } else if (contains(fieldName, "z:")) {
        ASSERT(extract(fieldName, 0, 2) % "z:");  // must be first modifier in the string
        replace(fieldName, "z:", "");             // get rid of the 'w:'
        maxWidth = str_2_Uint(fieldName);         // grab the width
        nextTokenClear(fieldName, ':');           // skip to the start of the fieldname
        zeroJust = true;
    } else if (contains(fieldName, "l:")) {
        ASSERT(extract(fieldName, 0, 2) % "l:");  // must be first modifier in the string
        replace(fieldName, "l:", "");             // get rid of the 'w:'
        lineWidth = str_2_Uint(fieldName);        // grab the width
        nextTokenClear(fieldName, ':');           // skip to the start of the fieldname
        lineJust = true;
    }

    //--------------------------------------------------------------------
#define truncPad(str, size) (size == 0xdeadbeef ? str : padRight(extract(str, 0, size), size))
#define truncPadR(str, size) (size == 0xdeadbeef ? str : padLeft(extract(str, 0, size), size))

    // Get the value of the field.  If the value of the field is empty we return empty for the entire token.
    string_q fieldValue = (func)(fieldName, data);
    if (isBool && fieldValue == "0")
        fieldValue = "";
    if (!isPrompt && fieldValue.empty())
        return "";
    if (isBool)  // we know it's true, so we want to only show the pre and post
        fieldValue = "";
    if (rightJust) {
        fieldValue = truncPadR(fieldValue, maxWidth);  // pad or trun cate
    } else if (zeroJust) {
        fieldValue = padLeft(fieldValue, maxWidth, '0');  // pad
    } else {
        fieldValue = truncPad(fieldValue, maxWidth);  // pad or trun cate
    }
    if (lineJust) {
        extern string_q reformat1(const string_q& in, size_t len);
        fieldValue = reformat1(fieldValue, lineWidth);
    }

    // The field is not hidden, the value of the field is not empty, we are not working
    // on a prompt, so we toss back the token referencing the value of the field.
    if (!isPrompt)
        return pre + fieldValue + post;

    // We are working on a prompt.  Pick up customizations if any
    string_q prompt = promptName;
    if (rightJust) {
        prompt = truncPadR(prompt, maxWidth);  // pad or trun cate
    } else {
        prompt = truncPad(prompt, maxWidth);  // pad or trun cate
    }
    return pre + prompt + post;
}

//--------------------------------------------------------------------------------
string_q fldNotFound(const string_q& str) {
    return "Field not found: " + str + "\n";
}

//-----------------------------------------------------------------------
string_q reformat1(const string_q& in, size_t len) {
    string_q ret = in;
    if (ret.length() > len + 10) {
        CStringArray parts;
        while (!ret.empty()) {
            string_q s = extract(ret, 0, len);
            replace(ret, s, "");
            if (s.length() == len) {
                s += "...";
                s += "\r\n\t\t\t    ";
            }
            parts.push_back(s);
        }
        ASSERT(ret.empty());
        for (size_t i = 0; i < parts.size(); i++)
            ret += parts[i];
    }
    return ret;
}
}  // namespace qblocks
