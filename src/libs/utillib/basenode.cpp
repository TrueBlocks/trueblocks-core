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
#include "basetypes.h"
#include "biglib.h"
#include "basenode.h"
#include "sfarchive.h"
#include "exportcontext.h"
#include "conversions.h"
#include "version.h"
#include "testing.h"

namespace qblocks {

//--------------------------------------------------------------------------------
// TODO(tjayrush): global data
CRuntimeClass CBaseNode::classCBaseNode;
static CBuiltIn _biBaseNode(&CBaseNode::classCBaseNode, "CBaseNode", sizeof(CBaseNode), NULL, NULL);
vector<CBuiltIn> builtIns;  // Keeps track of all the classes that have beebn registered

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
    m_schema = getVersionNum();
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
bool CBaseNode::isKindOf(const CRuntimeClass* pClass) const {
    CRuntimeClass* pClassThis = getRuntimeClass();
    return pClassThis->isDerivedFrom(pClass);
}

//--------------------------------------------------------------------------------
CRuntimeClass* CBaseNode::getRuntimeClass() const {
    return &CBaseNode::classCBaseNode;
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
                return uint_2_Str(m_deleted);
            break;
        case 's':
            if (fieldName % "schema")
                return uint_2_Str(m_schema);
            if (fieldName % "showing")
                return uint_2_Str(m_showing);
            break;
    }

    return "";
}

//--------------------------------------------------------------------------------
bool CBaseNode::parseCSV(const CStringArray& fields, string_q& str) {
    // Assumes no internal quotes or commas
    str = substitute(substitute(str, "\"", ""), ",", "\t");
    return parseText(fields, str);
}

//--------------------------------------------------------------------------------
bool CBaseNode::parseText(const CStringArray& fields, string_q& str) {
    str = substitute(str, "\r", "");
    string_q line = nextTokenClear(str, '\n');
    CStringArray values;
    explode(values, line, '\t');
    size_t cnt = 0;
    for (auto value : values) {
        if (cnt < fields.size()) {
            this->setValueByName(fields[cnt++], value);
        } else {
            finishParse();
            return false;
        }
    }
    finishParse();
    return !str.empty();
}

//--------------------------------------------------------------------------------
inline void preserveSpaces(string_q& str) {
    enum state_t { OUT = 0, IN = 1 };
    state_t state = OUT;
    for (char& s : str) {
        switch (state) {
            case OUT:
                if (s == '\"')
                    state = IN;
                break;
            case IN:
                if (s == ' ') {
                    s = static_cast<char>(5);
                }
                if (s == '\"')
                    state = OUT;
                break;
        }
    }
    return;
}

//--------------------------------------------------------------------------------
inline void restoreSpaces(char* s) {
    while (*s) {
        if (*s == static_cast<char>(5))
            *s = ' ';
        s++;
    }
    return;
}
struct cleaner {
    void operator()(char& s) {
        if (s == static_cast<char>(5))
            s = ' ';
    }
};
//--------------------------------------------------------------------------------
void restoreSpaces(string_q& str) {
    for_each(str.begin(), str.end(), cleaner());
    return;
}

//--------------------------------------------------------------------------------
// The data in 'str' is copied in to the object. Whatever portion of the string
// that represents this object gets consumed. The remained is returned in 'str'
bool CBaseNode::parseJson3(string_q& str) {
    preserveSpaces(str);
    char* s = (char*)str.c_str();  // NOLINT - we own the memory
    char* p = cleanUpJson(s);
    size_t nFields = 0;
    p = parseJson1(p, nFields);
    if (p)
        str = p;  // We give the altered memory back
    return (nFields);
}

//--------------------------------------------------------------------------------
// See above comment on parseJson3
bool CBaseNode::parseJson4(string_q& str) {
    char* p = (char*)str.c_str();  // NOLINT
    size_t nFields = 0;
    p = parseJson1(p, nFields);
    if (p)
        str = p;
    return (nFields);
}

// #define DEBUG_PARSER
#ifdef DEBUG_PARSER
string_q tbs;
#endif

//--------------------------------------------------------------------------------
char* CBaseNode::parseJson1(char* s, size_t& nFields) {
#ifdef DEBUG_PARSER
    string_q ss = s;
    string_q tt(25, '-');
    tt += "\n";
    cout << tt << s << "\n" << tt;
    cout << tt << extract(ss, ss.find("{"), 300) << "\n" << tt;
    cout << tt << extract(ss, ss.length() - 300, 300) << "\n" << tt;
    cout.flush();
    tbs += "\t";
#endif
    typedef enum { OUTSIDE = 0, IN_NAME, IN_VAL } parseState;
    parseState state = OUTSIDE;

    char* fieldName = NULL;
    char* fieldVal = NULL;
    while (s && *s) {
        switch (state) {
            case OUTSIDE:
                if (*s == '{')
                    state = IN_NAME;
                s++;
                break;

            case IN_NAME:
                if (!fieldName) {
                    fieldName = s;

                } else if (*s == ':') {
                    state = IN_VAL;
                    *s = '\0';
#ifdef DEBUG_PARSER
//                      printf("fn: %-10.10s fv: %-40.40s ---> %-60.60s\n" , fieldName, fieldVal, (s+1));
#endif
                }
                s++;
                break;

            case IN_VAL:
                fieldVal = s;
                if (*s == '[') {  // array skip to end of array
                    fieldVal++;
                    s++;
                    int lev = 1;
                    bool done = false;
                    while (s && *s && !done) {
                        if (*s == '[') {
                            lev++;

                        } else if (lev == 1 && *s == ']') {
                            done = true;
                            if (*s == ']')
                                lev--;
                            s--;  // remove the closing bracket

                        } else if (*s == ']') {
                            lev--;
                        }
                        s++;
                    }
                } else {
                    int lev = 1;
                    bool done = false;
                    while (s && *s && !done) {
                        if (*s == '{') {
                            lev++;

                        } else if (lev == 1 && (*s == ',' || *s == '}')) {
                            done = true;
                            if (*s == '}')
                                lev--;
                            s--;

                        } else if (*s == '}') {
                            lev--;
                        }
                        s++;
                    }
                }
                *s = '\0';
#ifdef DEBUG_PARSER
                printf("%sfn: %-20.20s fv: %-60.60s ---> %-40.40s\n", (const char*)tbs, fieldName, fieldVal, (s + 1));
                fflush(stdout);
#endif
                if (!strchr(fieldVal, '{'))  // if it's not an object, replace space savers
                    restoreSpaces(fieldVal);
                nFields += this->setValueByName(fieldName, fieldVal);
                fieldName = NULL;
                fieldVal = NULL;
                state = IN_NAME;
                s++;
                if (*s && *s == ',')
                    s++;
                if (s && *s && (*s == '{' || *s == '}' || *s == ']')) {
                    finishParse();
#ifdef DEBUG_PARSER
                    replace(tbs, "\t", "");
#endif
                    return s;
                }
                break;
        }
    }
#ifdef DEBUG_PARSER
    replace(tbs, "\t", "");
#endif
    finishParse();
    return NULL;
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

//---------------------------------------------------------------------------
bool CBaseNode::readBackLevel(CArchive& archive) {
    // The following code assumes we do not change the format of the header
    archive >> m_deleted;
    archive >> m_schema;
    archive >> m_showing;
    string_q str;
    archive >> str;
    ASSERT(str == string_q(getRuntimeClass()->getClassNamePtr()));
    // We can never upgrade the base node data, so we always return false here (true means we've upgraded)
    return false;
}

//---------------------------------------------------------------------------
bool CBaseNode::Serialize(CArchive& archive) {
    archive >> m_deleted;
    archive >> m_schema;
    archive >> m_showing;
    string_q str;
    archive >> str;
    ASSERT(str == string_q(getRuntimeClass()->getClassNamePtr()));
    return false;
}

//---------------------------------------------------------------------------
bool CBaseNode::SerializeC(CArchive& archive) const {
    // Not happy with this, but we must set the schema version prior to writing
    // the data. We only write the latest version of the to the hard drive.
    ((CBaseNode*)this)->m_schema = getVersionNum();  // NOLINT

    archive << m_deleted;
    archive << m_schema;
    archive << m_showing;
    archive << getRuntimeClass()->getClassNamePtr();
    return false;
}

//---------------------------------------------------------------------------
// TODO(tjayrush): global data
static CExportOptions expC;
CExportOptions& expContext(void) {
    return expC;
}

//---------------------------------------------------------------------------
void incIndent(void) {
    expC.lev++;
}

//---------------------------------------------------------------------------
void decIndent(void) {
    expC.lev--;
}

//---------------------------------------------------------------------------
string_q indent(void) {
    return string_q(expC.spcs * expC.lev, expC.tab);
}

//--------------------------------------------------------------------------------
string_q CBaseNode::toJson1(void) const {
    return substitute(substitute(substitute(toJson(), "\t", " "), "\n", ""), "  ", " ");
}

//--------------------------------------------------------------------------------
string_q CBaseNode::toJson(void) const {
    CRuntimeClass* pClass = getRuntimeClass();
    if (!pClass)
        return "";

    CFieldDataArray fields;

    // Pick up the fields from this class
    for (auto field : pClass->fieldList)
        fields.push_back(field);

    // Pick up the fields from parents classes (if any)
    CRuntimeClass* pParent = pClass->m_BaseClass;
    while (pParent != GETRUNTIME_CLASS(CBaseNode)) {
        for (auto pField : pParent->fieldList)
            fields.push_back(pField);
        pParent = pParent->m_BaseClass;
    }

    if (fields.size() == 0) {
        cerr << "No fieldList in " << pClass->m_ClassName << ". Did you register the class?\n";
        return "";
    }

    return "{" + trim(jsonFromArray(fields)) + "\n" + indent() + "}";
}

//--------------------------------------------------------------------------------
string_q CBaseNode::toJson(const string_q& fieldStrIn) const {
    CRuntimeClass* pClass = getRuntimeClass();
    if (!pClass)
        return "";

    CFieldDataArray fields;
    string_q fieldStr = fieldStrIn;
    while (!fieldStr.empty()) {
        string_q field = nextTokenClear(fieldStr, '|');
        CFieldData* fld = pClass->findField(field);
        if (!fld)
            cerr << "Could not find field " << field << " in class " << pClass->m_ClassName << ".\n";
        else
            fields.push_back(*fld);
    }

    if (fields.size() == 0) {
        cerr << "No fieldList in " << pClass->m_ClassName << ". Did you register the class?\n";
        return "";
    }

    return "{" + trim(jsonFromArray(fields)) + "\n" + indent() + "}";
}

//--------------------------------------------------------------------------------
string_q CBaseNode::jsonFromArray(const CFieldDataArray& fields) const {
    string_q ret;
    bool first = true;
    if (!expContext().noFrst)
        ret += indent();
    expContext().noFrst = false;

    for (auto field : fields) {
        incIndent();
        string_q val = getValueByName(field.m_fieldName);
        if (!field.isHidden() && (!val.empty() || field.isArray())) {
            if (!first) {
                if (expContext().colored)
                    ret += "#";
                ret += ",\n";
            } else {
                ret += "\n";
            }
            first = false;
            ret += indent();
            ret += "\"" + field.m_fieldName + "\"";
            ret += ": ";
            if (expContext().colored)
                ret += "%";
            if (field.isArray()) {
                incIndent();
                val = substitute(getValueByName(field.m_fieldName), "\n{", "\n" + indent() + "{");
                ret += (val.empty() ? "[]" : "[\n" + indent() + val);
                decIndent();
                ret += (val.empty() ? "" : indent() + "]");

            } else if (field.isObject()) {
                ret += val;

            } else if (field.m_fieldType == T_BLOOM) {
                ret += "\"" + val + "\"";

            } else if (field.m_fieldType & TS_NUMERAL) {
                if (expContext().quoteNums)
                    ret += "\"";
                ret +=
                    (expContext().hexNums && ((isNumeral(val) || isHexStr(val)) && !contains(val, ".")) ? str_2_Hex(val)
                                                                                                        : val);
                if (expContext().quoteNums)
                    ret += "\"";

            } else if (val == "null") {
                ret += val;

            } else {
                if (val == "null")
                    ret += val;
                else
                    ret += "\"" + val + "\"";
            }
        }
        decIndent();
    }

    return ret;
}

//--------------------------------------------------------------------------------
void CBaseNode::doExport(ostream& os) const {
    CRuntimeClass* pClass = getRuntimeClass();
    if (!pClass)
        return;

    string_q first;
    for (auto field : pClass->fieldList) {
        if (!field.isHidden() && first.empty()) {
            first = field.getName();
        }
    }

    os << "{";
    if (m_showing) {
        incIndent();
        for (auto field : pClass->fieldList) {
            string_q name = field.getName();
            if (!field.isHidden()) {
                if (field.isArray()) {
                    uint64_t cnt = str_2_Uint(getValueByName(name + "Cnt"));
                    if (cnt || showEmptyField(name)) {
                        if (field.getName() != first)
                            os << ",";
                        os << "\n";
                        os << indent() << "\"" << name << "\": ";
                        os << "[";
                        if (cnt) {
                            incIndent();
                            os << "\n";
                            for (size_t i = 0; i < cnt; i++) {
                                os << indent();
                                const CBaseNode* node = getObjectAt(name, i);
                                if (node) {
                                    node->doExport(os);
                                } else {
                                    os << "\"" << getStringAt(name, i) << "\"";
                                }
                                if (i < cnt - 1)
                                    os << ",";
                                os << "\n";
                            }
                            decIndent();
                            os << indent();
                        }
                        os << "]";
                    }

                } else if (field.isObject()) {
                    if (field.getName() != first)
                        os << ",";
                    os << "\n";
                    os << indent() << "\"" << name << "\": ";
                    const CBaseNode* node = getObjectAt(name, 0);
                    if (node) {
                        node->doExport(os);
                    } else {
                        os << getValueByName(name);
                    }

                } else {
                    if (field.getName() != first)
                        os << ",";
                    os << "\n";
                    os << indent() << "\"" << name << "\": ";
                    string_q val = getValueByName(name);
                    bool isNum = (field.m_fieldType & TS_NUMERAL);
                    if (isNum && expContext().hexNums && !startsWith(val, "0x") && !contains(val, "."))
                        val = str_2_Hex(val);
                    bool quote = (!isNum || expContext().quoteNums) && val != "null";
                    if (quote)
                        os << "\"";
                    os << val;
                    if (quote)
                        os << "\"";
                }
            }
        }
        decIndent();
        os << "\n" << indent();
    }
    os << "}";
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
                        cerr << "No fieldList in " << node->getRuntimeClass()->m_ClassName
                             << ". Did you register the class?\n";
                        return "";
                    }
                    return "{" + substitute(node->jsonFromArray(pClass->fieldList), "\n", "") + "}";
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
        // There are no more tokens.  Return the last chunk and empty out the format
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
    // leaving the remainder of the format in fmtOut.  A field is found if we find a pair
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
        fieldValue = truncPadR(fieldValue, maxWidth);  // pad or truncate
    } else if (zeroJust) {
        fieldValue = padLeft(fieldValue, maxWidth, '0');  // pad
    } else {
        fieldValue = truncPad(fieldValue, maxWidth);  // pad or truncate
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
        prompt = truncPadR(prompt, maxWidth);  // pad or truncate
    } else {
        prompt = truncPad(prompt, maxWidth);  // pad or truncate
    }
    return pre + prompt + post;
}

//---------------------------------------------------------------------------------------------------
CBaseNode* createObjectOfType(const string_q& className) {
    // TODO(tjayrush): global data
    {  // keep the frame
        mutex aMutex;
        lock_guard<mutex> lock(aMutex);
        static bool isSorted = false;
        if (!isSorted) {
            sort(builtIns.begin(), builtIns.end());
            isSorted = true;
        }
    }

    CRuntimeClass* pClass = &CBaseNode::classCBaseNode;
    CBuiltIn search(pClass, className, sizeof(CBaseNode), NULL, NULL);
    const vector<CBuiltIn>::iterator it = find(builtIns.begin(), builtIns.end(), search);
    if (it != builtIns.end() && it->m_pClass && it->m_pClass->m_CreateFunc)
        return (*it->m_pClass->m_CreateFunc)();
    return NULL;
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

uint64_t testing::Test::nFuncs;
testing::PF testing::Test::funcs[];
