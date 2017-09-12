/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
/*
 * This file was generated with makeClass. Edit only those parts of the code inside
 * of 'EXISTING_CODE' tags.
 */
#include "infix.h"
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CInfix, CTreeNode, dataSchema());

//---------------------------------------------------------------------------
static SFString nextInfixChunk(const SFString& fieldIn, const void *data);
static SFString nextInfixChunk_custom(const SFString& fieldIn, const void *data);

//---------------------------------------------------------------------------
void CInfix::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    SFString fmt = fmtIn;
    if (handleCustomFormat(ctx, fmt, data))
        return;

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextInfixChunk, this);
}

//---------------------------------------------------------------------------
SFString nextInfixChunk(const SFString& fieldIn, const void *data) {
    const CInfix *inf = (const CInfix *)data;
    if (inf) {
        // Give customized code a chance to override first
#ifdef NEW_CODE
        SFString ret = inf->getValueByName(fieldIn);
        if (!ret.empty())
            return ret;
#else
        SFString ret = nextInfixChunk_custom(fieldIn, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'm':
            return "";
//                if ( fieldIn % "m_next" ) { expContext().noFrst=true; return inf->m_next.Format(); }
                break;
        }
#endif
        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTreenodeChunk(fieldIn, inf);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CInfix::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'm':
            return true;
//            if ( fieldName % "m_next" ) { /* m_next = fieldValue; */ return false; }
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

    CTreeNode::Serialize(archive);

//    archive >> m_next;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CInfix::SerializeC(SFArchive& archive) const {
    CTreeNode::SerializeC(archive);

//    archive << m_next;

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
    ADD_FIELD(CInfix, "m_next", T_POINTER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CInfix, "schema");
    HIDE_FIELD(CInfix, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextInfixChunk_custom(const SFString& fieldIn, const void *data) {
    const CInfix *inf = (const CInfix *)data;
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
bool CInfix::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CInfix::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CInfix::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE

#ifdef NEW_CODE
    // Give customized code a chance to override first
    SFString ret = nextInfixChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    switch (tolower(fieldName[0])) {
        case 'm':
        return "";
//            if ( fieldName % "m_next" ) { expContext().noFrst=true; return m_next.Format(); }
            break;
    }
    return "";
#else
    return Format("[{"+toUpper(fieldName)+"}]");
#endif
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    SFString CInfix::at(const SFString& _key) const {
        ASSERT(m_next);
        return contains(_key) ? m_next->at(_key.substr(m_prefix.length())) : "";
    }

    //-----------------------------------------------------------------------------
    bool CInfix::contains(const SFString& _key) const {
        size_t l1 = _key.length();
        size_t l2 = m_prefix.length();
        const char *s1 = (const char*)_key;
        const char *s2 = (const char*)m_prefix;
        bool found = !memcmp(s1, s2, l2);

        return
        l1 >= l2 && found;
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CInfix::insert(const SFString& _key, const SFString& _value) {
        if (verbose == 2) { cerr << "\tinfix inserting " << _key << " at " << _value << "\n"; }
        ASSERT(_value.length());
        if (contains(_key)) {
            m_next = m_next->insert(_key.substr(m_prefix.length()), _value);
            return this;

        } else {
            unsigned prefix = commonPrefix(_key, m_prefix);
            if (prefix) {
                // one infix becomes two infixes, then insert into the second
                // instead of pop_front()...
                m_prefix = m_prefix.substr(prefix);
                return new CInfix(_key.substr(0, prefix), insert(_key.substr(prefix), _value));

            } else {
                // split here.
                auto f = m_prefix[0];
                m_prefix = m_prefix.substr(1);
                CTreeNode* n = m_prefix.empty() ? m_next : this;
                if (n != this) {
                    m_next = NULL;
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
            << idnt << "remove infix [" << m_prefix << "] at [" << _key << "]: ";
            idnt+="\t";
        }

        if (contains(_key)) {
            SFString newKey = _key.substr(m_prefix.length());
            m_next = m_next->remove(newKey);
            if (auto p = m_next) {
                // merge with child...
                p->m_prefix = m_prefix + p->m_prefix;
                m_next = nullptr;
                delete this;
                if (verbose) cerr << idnt << "removed infix replaced with [" << p->m_prefix << "]";
                idnt.Replace("\t", "");
                return p;
            }

            if (!m_next) {
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
        vd->cnt = 0;
        vd->type = T_INFIX;
        vd->strs = vd->strs + "+" + m_prefix;
        (*func)(this, data);
        if (m_next)
            m_next->visitItems(func, data);
        nextTokenClearReverse(vd->strs, '+');
        vd->type = save;
        return true;
    }
// EXISTING_CODE
}  // namespace qblocks

