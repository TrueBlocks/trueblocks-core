/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "biglib.h"
#include "basenode.h"
#include "sfarchive.h"
#include "exportcontext.h"
#include "conversions.h"
#include "version.h"

namespace qblocks {

    //--------------------------------------------------------------------------------
    CRuntimeClass CBaseNode::classCBaseNode;
    static CBuiltIn _biBaseNode(&CBaseNode::classCBaseNode, "CBaseNode", sizeof(CBaseNode), NULL, NULL);

    //--------------------------------------------------------------------------------
    CBaseNode::CBaseNode(void) {
        Init();
    }

    //--------------------------------------------------------------------------------
    CBaseNode::~CBaseNode(void) {
    }

    //--------------------------------------------------------------------------------
    void CBaseNode::Init(void) {
        m_deleted  = false;
        m_schema = getVersionNum();
        m_showing = true;
        pParent = NULL;
    }

    //--------------------------------------------------------------------------------
    void CBaseNode::Copy(const CBaseNode& bn) {
        m_deleted  = bn.m_deleted;
        m_schema = bn.m_schema;
        m_showing = bn.m_showing;
        pParent = NULL;
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
        return pClassThis->IsDerivedFrom(pClass);
    }

    //--------------------------------------------------------------------------------
    CRuntimeClass* CBaseNode::getRuntimeClass() const {
        return &CBaseNode::classCBaseNode;
    }

    //--------------------------------------------------------------------------------
    SFString CBaseNode::getValueByName(const SFString& fieldName) const {

        switch (tolower(fieldName[0])) {
            case 'd':
                if ( fieldName % "deleted" ) return asStringU(m_deleted);
                break;
            case 's':
                if ( fieldName % "schema" ) return asStringU(m_schema);
                if ( fieldName % "showing" ) return asStringU(m_showing);
                break;
        }

        return "";
    }

    //--------------------------------------------------------------------------------
    char *CBaseNode::parseCSV(char *s, uint32_t& nFields, const SFString *fields) {
        nFields = 0;

        typedef enum { OUTSIDE = 0, INSIDE } parseState;
        parseState state = OUTSIDE;

        char *fieldVal = NULL;
        while (*s) {
            switch (state) {
                case OUTSIDE:
                    if (*s == '\"') {
                        state = INSIDE;
                        fieldVal = s+1;

                    } else if (*s == '\n') {
                        finishParse();
                        return (s+1);
                    }
                    s++;
                    break;

                case INSIDE:
                    if (*s == '\"') {
                        *s = '\0';
                        if (!this->setValueByName(fields[nFields++], fieldVal)) {
//                          fprintf(stderr, "Bad field name %s. Quitting...", (const char*)fields[nFields-1]);
//                          return NULL;
                        }
                        fieldVal = NULL;
                        state = OUTSIDE;

                    }
                    s++;
                    break;
            }
        }
        finishParse();
        return NULL;
    }

    //--------------------------------------------------------------------------------
    char *CBaseNode::parseText(char *s, uint32_t& nFields, const SFString *fields) {
        nFields = 0;
        char *fieldVal = s;
        while (s && *s) {
            switch (*s) {
                case '\r':
                    break;
                case '\t':
                    *s = '\0';
                    this->setValueByName(fields[nFields++], fieldVal);
                    fieldVal = s+1;
                    break;
                case '\n':
                    *s = '\0';
                    this->setValueByName(fields[nFields++], fieldVal);
                    finishParse();
                    return s+1;
            }
            s++;
        }
        this->setValueByName(fields[nFields++], fieldVal);
        finishParse();
        return NULL;
    }

    //--------------------------------------------------------------------------------
    char *CBaseNode::parseJson(char *s) {
        uint32_t nFields = 0;
        return parseJson(s, nFields);
    }

//#define DEBUG_PARSER
#ifdef DEBUG_PARSER
    SFString tbs;
#endif

    //--------------------------------------------------------------------------------
    char *CBaseNode::parseJson(char *s, uint32_t& nFields) {
#ifdef DEBUG_PARSER
        tbs+="\t";
        printf("--------------------------\n%s\n-----------------------------\n", s);
        fflush(stdout);
        printf("--------------------------\n%s\n-----------------------------\n",
               (const char*)SFString(s).substr(SFString(s).find("{"), 300));
        printf("--------------------------\n%s\n-----------------------------\n",
               (const char*)SFString(s).Right(300));
#endif
        typedef enum { OUTSIDE = 0, IN_NAME, IN_VAL } parseState;
        parseState state = OUTSIDE;

        char *fieldName = NULL;
        char *fieldVal = NULL;
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
                    printf("%sfn: %-20.20s fv: %-60.60s ---> %-40.40s\n",
                           (const char*)tbs, fieldName, fieldVal, (s+1));
                    fflush(stdout);
#endif
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
                        tbs.Replace("\t", "");
#endif
                        return s;
                    }
                    break;
            }
        }
#ifdef DEBUG_PARSER
        tbs.Replace("\t", "");
#endif
        finishParse();
        return NULL;
    }

    //--------------------------------------------------------------------------------
    char *cleanUpJson(char *s) {
        if (!s)
            return s;
        char *l = s, *start = s;
        while (*s) {
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
    bool CBaseNode::readBackLevel(SFArchive& archive) {

        // pParent is use for array items to reach up into it's container
        archive.pParent = this;

        // The following code assumes we do not change the format of the header
        archive >> m_deleted;
        archive >> m_schema;
        archive >> m_showing;
        SFString str;
        archive >> str;
        ASSERT(str == SFString(getRuntimeClass()->getClassNamePtr()));

        // Return true if this is a back level version
        return true;
    }

    //---------------------------------------------------------------------------
    bool CBaseNode::Serialize(SFArchive& archive) {
        archive.pParent = this;  // sets this value for items stored in lists or arrays -- read only
        archive >> m_deleted;
        archive >> m_schema;
        archive >> m_showing;
        SFString str;
        archive >> str;
        ASSERT(str == SFString(getRuntimeClass()->getClassNamePtr()));
        return false;
    }

    //---------------------------------------------------------------------------
    bool CBaseNode::SerializeC(SFArchive& archive) const {
        archive.pParent = this;  // sets this value for items stored in lists or arrays -- read only
        archive << m_deleted;
        archive << m_schema;
        archive << m_showing;
        archive << getRuntimeClass()->getClassNamePtr();
        return false;
    }

    //---------------------------------------------------------------------------
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
    SFString indent(void) {
        return SFString(expC.tab, expC.spcs*expC.lev);
    }

    extern SFString decBigNum(const SFString& str);
    //--------------------------------------------------------------------------------
    SFString CBaseNode::toJson1(void) const {
        return toJson().Substitute("\t", " ").Substitute("\n", "").Substitute("  ", " ");
    }
    //--------------------------------------------------------------------------------
    SFString CBaseNode::toJson(void) const {
        const CFieldList *fieldList = getRuntimeClass()->GetFieldList();
        if (!fieldList) {
            cerr << "No fieldList in " << getRuntimeClass()->m_ClassName
                << ". Did you register the class? Quitting...\n";
            cerr.flush();
            exit(0);
        }

        // TODO(tjayrush): THIS PER DISPLAY LOOKUP IS SLOW - THIS SHOULD ONLY BE DONE ONCE
        // If a class is not a direct decendent of CBaseNode we want to include the parent nodes' fields
        // in the list as well
        CRuntimeClass *pThis = getRuntimeClass();
        CRuntimeClass *pPar  = pThis->m_BaseClass;
        CRuntimeClass *pBase = GETRUNTIME_CLASS(CBaseNode);
        if (pPar != pBase) {
            CFieldList theList = *fieldList;
            const CFieldList *fieldListA = pPar->GetFieldList();
            if (fieldListA) {
                LISTPOS lPos = fieldListA->SFList<CFieldData *>::GetHeadPosition();
                while (lPos) {
                    CFieldData *fld = fieldListA->GetNext(lPos);
                    if (!fld->isHidden())
                        theList.AddTail(fld);
                }
            }
            return toJson(&theList);
        }
        return toJson(fieldList);
    }

    //--------------------------------------------------------------------------------
    SFString CBaseNode::toJson(const SFString& fieldsIn) const {
        const CFieldList *fieldList = getRuntimeClass()->GetFieldList();
        if (!fieldList) {
            cerr << "No fieldList in " << getRuntimeClass()->m_ClassName
                << ". Did you register the class? Quitting...\n";
            cerr.flush();
            exit(0);
        }

        // TODO(tjayrush): THIS PER DISPLAY LOOKUP IS SLOW - SAVE THIS STRING FIELD LIST AND ONLY
        // LOAD IF DIFFERENT USE STATIC
        CFieldList theList;
        SFString fields = fieldsIn;
        while (!fields.empty()) {
            SFString field = nextTokenClear(fields, '|');
            const CFieldData *fld = fieldList->getFieldByName(field);
            if (!fld) {
                cerr << "Could not find field " << field << " in class "
                    << getRuntimeClass()->m_ClassName << ". Quitting...\n";
                cerr.flush();
                exit(0);
            }
            theList.AddTail((CFieldData*)(fld));  // NOLINT
        }
        return toJson(&theList);
    }

    //--------------------------------------------------------------------------------
    SFString CBaseNode::toJsonFldList(const CFieldList *fieldList) const {
        SFString ret;
        bool first = true;
        if (!expContext().noFrst)
            ret += indent();
        expContext().noFrst = false;
        LISTPOS lPos = fieldList->SFList<CFieldData *>::GetHeadPosition();
        while (lPos) {
            incIndent();
            CFieldData *fld = fieldList->GetNext(lPos);
            SFString val = getValueByName(fld->m_fieldName);
            if (!fld->isHidden() && (!val.empty() || fld->isArray())) {
                if (!first) {
                    if (expContext().colored)
                        ret += "#";
                    ret += ",\n";
                } else {
                    ret += "\n";
                }
                first = false;
                ret += indent();
                ret += "\"" + fld->m_fieldName + "\"";
                ret += ": ";
                if (expContext().colored)
                    ret += "%";
                if (fld->isArray()) {
                    incIndent();
                    val = getValueByName(fld->m_fieldName).Substitute("\n{","\n"+indent()+"{");
                    ret += (val.empty() ? "[]" : "[\n" + indent() + val);
                    decIndent();
                    ret += (val.empty() ? "" : indent() + "]");

                } else if (fld->isObject()) {
                    ret += val;

                } else if (fld->m_fieldType == T_BLOOM) {
                    ret += "\"" + val + "\"";


                } else if (fld->m_fieldType & TS_NUMERAL) {
                    if (expContext().quoteNums) ret += "\"";
                    ret += (expContext().hexNums) ? toHex2(val) : decBigNum(val);
                    if (expContext().quoteNums) ret += "\"";

                } else if (val == "null") {
                    ret += val;

                } else {
                    ret += "\"" + val + "\"";
                }
            }
            decIndent();
        }

        return ret;
    }

    //--------------------------------------------------------------------------------
    SFString CBaseNode::toJson(const CFieldList *fieldList) const {
        ASSERT(fieldList);
        SFString ret;
        ret += "{";
        ret += Strip(toJsonFldList(fieldList),' ');
        ret += "\n";
        ret += indent();
        ret += "}";
        return ret;
    }

    //--------------------------------------------------------------------------------
    SFString decBigNum(const SFString& str) {
        SFString ret = str;
        size_t len = ret.length();
        if (len > 29) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+29";
        else if (len >28) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+28";
        else if (len > 27) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+27";
        else if (len > 26) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+26";
        else if (len > 25) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+25";
        else if (len > 24) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+24";
        else if (len > 23) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+23";
        else if (len > 22) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+22";
        else if (len > 21) ret = ret.Left(1) + "." + StripTrailing(ret.substr(1), '0') + "e+21";
        ret.Replace(".e+", "e+");
        return ret;
    }

    //--------------------------------------------------------------------------------
    SFString nextBasenodeChunk(const SFString& fieldIn, const CBaseNode *node) {
        if (node) {
            SFString className = node->getRuntimeClass()->getClassNamePtr();
            switch (tolower(fieldIn[0])) {
                case 'd':
                    if ( fieldIn % "deleted" ) return asString(node->isDeleted());
                    break;
                case 'n':
                    if ( fieldIn % "null" ) return "<x>";
                    break;
                case 'p':
                    if ( fieldIn % "parsed" ) {
                        const CFieldList *fieldList = node->getRuntimeClass()->GetFieldList();
                        if (!fieldList) {
                            cerr << "No fieldList in " << node->getRuntimeClass()->m_ClassName
                                << ". Did you register the class? Quitting...\n";
                            cerr.flush();
                            exit(0);
                        }
                        SFString ret;
                        ret += "{";
                        ret += node->toJsonFldList(fieldList);
                        ret += "}";
                        return ret.Substitute("\n", "");;
                    }
                    break;
                case 's':
                    if ( fieldIn % "schema" ) return asStringU(node->m_schema);
                    if ( fieldIn % "showing" ) return asStringU(node->m_showing);
                    break;
                default:
                    break;
            }
        }

        return EMPTY;
    }

    //---------------------------------------------------------------------------------------------
    SFString getNextChunk(SFString& fmtOut, NEXTCHUNKFUNC func, const void *data) {
        SFString chunk = fmtOut;
        if (!fmtOut.Contains("[")) {
            // There are no more tokens.  Return the last chunk and empty out the format
            fmtOut = EMPTY;
            return chunk;
        }

        if (!fmtOut.startsWith('[')) {
            // We've encountered plain text outside of a token. There is more to process so grab
            // the next chunk and then prepare the remaining chunk by prepending the token.
            // The next time through we will hit the token.
            chunk  = nextTokenClear(fmtOut, '[', false);
            fmtOut = "[" + fmtOut;
            return chunk;
        }

        // We've hit a token, toss the start token, look for a field and toss the last token
        // leaving the remainder of the format in fmtOut.  A field is found if we find a pair
        // of squigglies).  Save text inside the start token and outside the field in pre and post
        ASSERT(fmtOut.startsWith('['));

        SFString pre, fieldName, post;
        nextTokenClear(fmtOut, '[', false);  // toss the start token
        if (chunk.Contains("{")) {
            // we've encountered a field
            pre       = nextTokenClear(fmtOut, '{', false);
            fieldName = nextTokenClear(fmtOut, '}', false);
            post      = nextTokenClear(fmtOut, ']', false);
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

        // The fieldname may contain p: or w:width: or both.  If it contains either it
        // must contain them at the beginning of the string (before the fieldName).  Anything
        // left after the last ':' is considered the fieldName
        SFString promptName = fieldName;
        if (fieldName.ContainsI("p:")) {
            isPrompt = true;
            fieldName.ReplaceI("p:", EMPTY);
            promptName = fieldName;
        }

        uint32_t maxWidth = 0xdeadbeef;
        bool rightJust = false;
        if (fieldName.ContainsI("w:")) {
            ASSERT(fieldName.Left(2) % "w:");  // must be first modifier in the string
            fieldName.ReplaceI("w:", EMPTY);   // get rid of the 'w:'
            maxWidth = toLong32u(fieldName);   // grab the width
            nextTokenClear(fieldName, ':');    // skip to the start of the fieldname
        } else if (fieldName.ContainsI("r:")) {
            ASSERT(fieldName.Left(2) % "r:");  // must be first modifier in the string
            fieldName.ReplaceI("r:", EMPTY);   // get rid of the 'w:'
            maxWidth = toLong32u(fieldName);   // grab the width
            nextTokenClear(fieldName, ':');    // skip to the start of the fieldname
            rightJust = true;
        }

        //--------------------------------------------------------------------
#define truncPad(str, size)  (size == 0xdeadbeef ? str : padRight(str.Left(size), size))
#define truncPadR(str, size) (size == 0xdeadbeef ? str : padLeft (str.Left(size), size))

        // Get the value of the field.  If the value of the field is empty we return empty for the entire token.
        SFString fieldValue = (func)(fieldName, data);
        if (!isPrompt && fieldValue.empty())
            return EMPTY;
        if (rightJust) {
            fieldValue = truncPadR(fieldValue, maxWidth);  // pad or truncate
        } else {
            fieldValue = truncPad(fieldValue, maxWidth);  // pad or truncate
        }

        // The field is not hidden, the value of the field is not empty, we are not working
        // on a prompt, so we toss back the token referencing the value of the field.
        if (!isPrompt)
            return pre + fieldValue + post;

        // We are working on a prompt.  Pick up customizations if any
        SFString prompt = promptName;
        if (rightJust) {
            prompt = truncPadR(prompt, maxWidth);  // pad or truncate
        } else {
            prompt = truncPad(prompt, maxWidth);  // pad or truncate
        }
        return pre + prompt + post;
    }

    //--------------------------------------------------------------------------------
    SFString fldNotFound(const SFString& str) {
        return "Field not found: " + str + "\n";
    }

    //--------------------------------------------------------------------------------
    void CBaseNode::doExport(ostream& os) const {

        CFieldList *list = getRuntimeClass()->GetFieldList();
        LISTPOS pos;

        CFieldData *lastVisible = NULL;
        pos = list->GetHeadPosition();
        while (pos) {
            CFieldData *field = list->GetNext(pos);
            if (!field->isHidden())
                lastVisible = field;
        }

        os << "{\n";
        incIndent();
        pos = list->GetHeadPosition();
        while (pos) {
            CFieldData *field = list->GetNext(pos);
            if (!field->isHidden()) {
                SFString name = field->getName();
                os << indent() << "\"" << name << "\": ";
                if (field->isArray()) {
                    uint64_t cnt = toLongU(getValueByName(name+"Cnt"));
                    os << "[";
                    if (cnt) {
                        incIndent();
                        os << "\n";
                        for (uint32_t i = 0 ; i < cnt ; i++) {
                            os << indent();
                            const CBaseNode *node = getObjectAt(name, i);
                            if (node) {
                                node->doExport(os);
                            } else {
                                os << "\"" << getStringAt(name, i) << "\"";
                            }
                            if (i < cnt-1)
                                os << ",";
                            os << "\n";
                        }
                        decIndent();
                        os << indent();
                    }
                    os << "]";
                } else if (field->isObject()) {
                    const CBaseNode *node = getObjectAt(name, 0);
                    if (node) {
                        node->doExport(os);
                    } else {
                        os << getValueByName(name);
                    }
                } else {
                    SFString val = getValueByName(name);
                    bool isNum = field->m_fieldType & TS_NUMERAL;
                    if (isNum && expContext().hexNums && !val.startsWith("0x"))
                        val = toHex2(val);
                    bool quote = (!isNum || expContext().quoteNums) && val != "null";
                    if (quote)
                        os << "\"";
                    os << val;
                    if (quote)
                        os << "\"";
                }
                if (field != lastVisible)
                    os << ",";
                os << "\n";
            }
        }
        decIndent();
        os << indent();
        os << "}";
    }
}  // namespace qblocks

uint64_t testing::Test::nFuncs;
testing::PF testing::Test::funcs[];

