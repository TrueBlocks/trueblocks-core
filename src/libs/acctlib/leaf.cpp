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
#include "leaf.h"
#include "tree.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLeaf, CTreeNode, curVersion);

//---------------------------------------------------------------------------
extern SFString nextTreenodeChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextLeafChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextLeafChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CLeaf::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextLeafChunk, this);
}

//---------------------------------------------------------------------------
SFString nextLeafChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CLeaf *lea = (const CLeaf *)data;
    if (lea) {
        // Give customized code a chance to override first
        SFString ret = nextLeafChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'm':
                if ( fieldIn % "m_leafValue" ) return lea->m_leafValue;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextTreenodeChunk(fieldIn, force, lea);
        if (!ret.empty())
            return ret;
    }

    return "Field not found: [{" + fieldIn + "}]\n";
}

//---------------------------------------------------------------------------------------------------
bool CLeaf::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'm':
            if ( fieldName % "m_leafValue" ) { m_leafValue = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CLeaf::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLeaf::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CLeaf*)this)->SerializeC(archive);

    CTreeNode::Serialize(archive);

    archive >> m_leafValue;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLeaf::SerializeC(SFArchive& archive) const {
    CTreeNode::SerializeC(archive);

    archive << m_leafValue;

    return true;
}

//---------------------------------------------------------------------------
void CLeaf::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTreeNode::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(CLeaf, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CLeaf, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CLeaf, "m_leafValue", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLeaf, "schema");
    HIDE_FIELD(CLeaf, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextLeafChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CLeaf *lea = (const CLeaf *)data;
    if (lea) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, lea);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool CLeaf::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CLeaf::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    CLeaf::CLeaf(const SFString& _key, const SFString& _value) : m_leafValue(_value) {
        m_prefix = _key;
        if (verbose == 2) cerr << "\t\tCreating leaf " << _key << endl;
    }

    //-----------------------------------------------------------------------------
    SFString CLeaf::at(const SFString& _key) const {
        return contains(_key) ? m_leafValue : "";
    }

    //-----------------------------------------------------------------------------
    bool CLeaf::contains(const SFString& _key) const {
        size_t l1 = _key.length();
        size_t l2 = m_prefix.length();
        const char *s1 = (const char*)_key;
        const char *s2 = (const char*)m_prefix;
        bool found = !memcmp(s1, s2, l1);

        return l1 == l2 && found;
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CLeaf::insert(const SFString& _key, const SFString& _value) {
        if (contains(_key)) {
            // If the leaf exists, we reset the value
            if (verbose) cerr << "\t\tReplacing leaf contents " << _key << "\n";
            // We've reached the end of the key, so store the value here
            if (m_leafValue.empty()) {
                // store the first encountered block
                m_leafValue = _value;

            } else {
                // preserve the most recent block encountered
                m_leafValue = nextTokenClear(m_leafValue, '|');
                m_leafValue += "|" + _value;
            }
            return this;

        } else {
            // If the leaf is not the key, delete convert to a branch
            if (verbose == 2) cerr << "\t\tBranching " << _key << "\n";
            CTreeNode *n = CTreeNode::newBranch(_key, _value, m_prefix, m_leafValue);
            delete this;
            return n;
        }
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CLeaf::remove(const SFString& _key) {
        if (verbose)
            cerr << endl<< endl<< endl
            << idnt << SFString('-', 80) << endl
            << idnt << SFString('-', 80) << endl
            << idnt << "remove infix at [" << _key << "]: ";

        if (contains(_key)) {
            if (verbose)
                cerr << endl << idnt << "removed leaf node at" << _key << endl;
            delete this;
            return NULL;
        }
        if (verbose)
            cerr << endl << idnt << "no node removed at" << _key << endl;
        return this;
    }

    //------------------------------------------------------------------
    bool CLeaf::visitItems(ACCTVISITOR func, void *data) const {
        ASSERT(func);
        CVisitData *vd = reinterpret_cast<CVisitData*>(data);
        uint32_t save = vd->type;
        vd->type = T_LEAF;
        vd->strs = vd->strs + "+" + (cMagenta+m_prefix + cOff + "|" + cBlue + m_leafValue + cOff);
        (*func)(this, data);
        nextTokenClearReverse(vd->strs, '+');
        vd->type = save;
        return true;
    }
// EXISTING_CODE
}  // namespace qblocks

