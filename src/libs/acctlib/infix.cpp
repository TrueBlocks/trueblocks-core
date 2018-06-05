/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "infix.h"
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CInfix, CTreeNode);

//---------------------------------------------------------------------------
static SFString nextInfixChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextInfixChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CInfix::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, dataPtr))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextInfixChunk, this);
}

//---------------------------------------------------------------------------
SFString nextInfixChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CInfix *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CInfix::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "next" ) {
                Clear();
                next = new CTreeNode;
                if (next) {
                    char *p = cleanUpJson((char *)fieldValue.c_str());
                    uint32_t nFields = 0;
                    next->parseJson(p, nFields);
                    return true;
                }
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
bool CInfix::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CInfix*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    CTreeNode::Serialize(archive);

    // EXISTING_CODE
    next = NULL;
    bool has_next = false;
    archive >> has_next;
    if (has_next) {
        SFString className;
        archive >> className;
        next = createTreeNode(className);
        if (!next)
            return false;
        next->Serialize(archive);
    }
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CInfix::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CTreeNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << (next != NULL);
    if (next) {
        SFString className = next->getRuntimeClass()->getClassNamePtr();
        archive << className;
        next->SerializeC(archive);
    }

    return true;
}

//---------------------------------------------------------------------------
void CInfix::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTreeNode::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(CInfix, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CInfix, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CInfix, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CInfix, "next", T_POINTER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CInfix, "schema");
    HIDE_FIELD(CInfix, "deleted");
    HIDE_FIELD(CInfix, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextInfixChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CInfix *inf = (const CInfix *)dataPtr;
    if (inf) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, inf);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CInfix::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CInfix::readBackLevel(SFArchive& archive) {

    CTreeNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CInfix::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextInfixChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'n':
            if ( fieldName % "next" ) {
                if (next)
                    return next->Format();
                return "";
            }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTreeNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CInfix& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    SFString CInfix::at(const SFString& _key) const {
        ASSERT(next);
        return contains(_key) ? next->at(_key.substr(prefixS.length())) : "";
    }

    //-----------------------------------------------------------------------------
    bool CInfix::contains(const SFString& _key) const {
        size_t l1 = _key.length();
        size_t l2 = prefixS.length();
        const char *s1 = (const char*)_key;
        const char *s2 = (const char*)prefixS;
        bool found = !memcmp(s1, s2, l2);

        return
        l1 >= l2 && found;
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CInfix::insert(const SFString& _key, const SFString& _value) {
        if (verbose == 2) { cerr << "\tinfix inserting " << _key << " at " << _value << "\n"; }
        ASSERT(_value.length());
        if (contains(_key)) {
            next = next->insert(_key.substr(prefixS.length()), _value);
            return this;

        } else {
            unsigned prefixA = commonPrefix(_key, prefixS);
            if (prefixA) {
                // one infix becomes two infixes, then insert into the second
                // instead of pop_front()...
                prefixS = prefixS.substr(prefixA);
                return new CInfix(_key.substr(0, prefixA), insert(_key.substr(prefixA), _value));

            } else {
                // split here.
                auto f = prefixS[0];
                prefixS = prefixS.substr(1);
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

    SFString idnt = "";
    //-----------------------------------------------------------------------------
    CTreeNode* CInfix::remove(const SFString& _key) {
        if (verbose) {
            cerr << endl << endl<< endl
            << idnt << SFString('-', 80) << endl
            << idnt << SFString('-', 80) << endl
            << idnt << "remove infix [" << prefixS << "] at [" << _key << "]: ";
            idnt+="\t";
        }

        if (contains(_key)) {
            SFString newKey = _key.substr(prefixS.length());
            next = next->remove(newKey);
            if (auto p = next) {
                // merge with child...
                p->prefixS = prefixS + p->prefixS;
                next = nullptr;
                delete this;
                if (verbose) cerr << idnt << "removed infix replaced with [" << p->prefixS << "]";
                idnt.Replace("\t", "");
                return p;
            }

            if (!next) {
                // we've cleaned up all the children
                delete this;
                idnt.Replace("\t", "");
                return NULL;
            }
        }
        idnt.Replace("\t", "");
        return this;
    }

    //------------------------------------------------------------------
    bool CInfix::visitItems(ACCTVISITOR func, void *data) const {
        ASSERT(func);
        CVisitData *vd = reinterpret_cast<CVisitData*>(data);
        uint32_t save = vd->type;
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

