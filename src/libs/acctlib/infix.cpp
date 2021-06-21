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
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "infix.h"
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CInfix, CTreeNode);

//---------------------------------------------------------------------------
static string_q nextInfixChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextInfixChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CInfix::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["infix_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextInfixChunk, this);
}

//---------------------------------------------------------------------------
string_q nextInfixChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CInfix*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CInfix::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextInfixChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'n':
            if (fieldName % "next") {
                if (next)
                    return next->Format();
                return "";
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTreeNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CInfix::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'n':
            if (fieldName % "next") {
                // This drops memory, so we comment it out for now
                // clear();
                // next = new CTreeNode;
                // if (next) {
                //     string_q str = fieldValue;
                //     return next->parseJson3(str);
                // }
                return false;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CInfix::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CInfix::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTreeNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    next = NULL;
    bool has_next = false;
    archive >> has_next;
    if (has_next) {
        string_q className;
        archive >> className;
        next = reinterpret_cast<CTreeNode*>(createObjectOfType(className));
        if (!next)
            return false;
        next->Serialize(archive);
    }
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CInfix::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CTreeNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << (next != NULL);
    if (next) {
        archive << next->getRuntimeClass()->getClassNamePtr();
        next->SerializeC(archive);
    }
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CInfixArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0; i < count; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CInfixArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CInfix::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CInfix, "schema"))
        return;

    CTreeNode::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CInfix, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CInfix, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CInfix, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CInfix, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CInfix, "next", T_POINTER | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CInfix, "schema");
    HIDE_FIELD(CInfix, "deleted");
    HIDE_FIELD(CInfix, "showing");
    HIDE_FIELD(CInfix, "cname");

    builtIns.push_back(_biCInfix);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextInfixChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CInfix* inf = reinterpret_cast<const CInfix*>(dataPtr);
    if (inf) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, inf);
                // EXISTING_CODE
                // EXISTING_CODE
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CInfix::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CInfix& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CInfix::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "next")
        return next;

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_INFIX = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//-----------------------------------------------------------------------------
string_q CInfix::at(const string_q& _key) const {
    ASSERT(next);
    return contains(_key) ? next->at(extract(_key, prefixS.length())) : "";
}

//-----------------------------------------------------------------------------
bool CInfix::contains(const string_q& _key) const {
    size_t l1 = _key.length();
    size_t l2 = prefixS.length();
    const char* s1 = _key.c_str();
    const char* s2 = prefixS.c_str();
    bool found = !memcmp(s1, s2, l2);

    return l1 >= l2 && found;
}

//-----------------------------------------------------------------------------
CTreeNode* CInfix::insert(const string_q& _key, const string_q& _value) {
    if (verbose == 2) {
        cerr << "\tinfix inserting " << _key << " at " << _value << "\n";
    }
    ASSERT(_value.length());
    if (contains(_key)) {
        next = next->insert(extract(_key, prefixS.length()), _value);
        return this;

    } else {
        unsigned prefixA = commonPrefix(_key, prefixS);
        if (prefixA) {
            // one infix becomes two infixes, then insert into the second
            // instead of pop_front()...
            prefixS = extract(prefixS, prefixA);
            return new CInfix(extract(_key, 0, prefixA), insert(extract(_key, prefixA), _value));

        } else {
            // split here.
            auto f = prefixS[0];
            prefixS = extract(prefixS, 1);
            CTreeNode* n = prefixS.empty() ? next : this;
            if (n != this) {
                next = NULL;
                delete this;
            }
            CBranch* ret = new CBranch(f, n);
            ret->insert(_key, _value);
            return ret;
        }
    }
}

string_q idnt = "";
//-----------------------------------------------------------------------------
CTreeNode* CInfix::remove(const string_q& _key) {
    if (verbose) {
        cerr << endl
             << endl
             << endl
             << idnt << string_q(80, '-') << endl
             << idnt << string_q(80, '-') << endl
             << idnt << "remove infix [" << prefixS << "] at [" << _key << "]: ";
        idnt += "\t";
    }

    if (contains(_key)) {
        string_q newKey = extract(_key, prefixS.length());
        next = next->remove(newKey);
        if (auto p = next) {
            // join with child...
            p->prefixS = prefixS + p->prefixS;
            next = nullptr;
            delete this;
            if (verbose)
                cerr << idnt << "removed infix replaced with [" << p->prefixS << "]";
            replace(idnt, "\t", "");
            return p;
        }

        if (!next) {
            // we've cleaned up all the children
            delete this;
            replace(idnt, "\t", "");
            return NULL;
        }
    }
    replace(idnt, "\t", "");
    return this;
}

//------------------------------------------------------------------
bool CInfix::visitItems(ACCTVISITOR func, void* data) const {
    ASSERT(func);
    CVisitData* vd = reinterpret_cast<CVisitData*>(data);
    uint64_t save = vd->type;
    vd->counter = 0;
    vd->type = T_INFIX;
    vd->strs = vd->strs + "+" + prefixS;
    (*func)(this, data);
    if (next) {
        vd->level++;
        next->visitItems(func, data);
        vd->level--;
    }
    nextTokenClearReverse(vd->strs, '+');
    vd->type = save;
    return true;
}
// EXISTING_CODE
}  // namespace qblocks
