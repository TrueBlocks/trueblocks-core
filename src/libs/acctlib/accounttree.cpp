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
#include "accounttree.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccountTree, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextAccounttreeChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextAccounttreeChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CAccountTree::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextAccounttreeChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAccounttreeChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CAccountTree *acc = (const CAccountTree *)data;
    if (acc) {
        // Give customized code a chance to override first
        SFString ret = nextAccounttreeChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'm':
            return EMPTY;
//                if ( fieldIn % "m_root" ) { expContext().noFrst=true; return acc->m_root.Format(); }
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, acc);
        if (!ret.empty())
            return ret;
    }

    return "Field not found: [{" + fieldIn + "}]\n";
}

//---------------------------------------------------------------------------------------------------
bool CAccountTree::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'm':
            return true;
//            if ( fieldName % "m_root" ) { /* m_root = fieldValue; */ return false; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccountTree::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccountTree::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CAccountTree*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

//    archive >> m_root;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccountTree::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

//    archive << m_root;

    return true;
}

//---------------------------------------------------------------------------
void CAccountTree::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CAccountTree, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CAccountTree, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CAccountTree, "m_root", T_POINTER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccountTree, "schema");
    HIDE_FIELD(CAccountTree, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAccounttreeChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CAccountTree *acc = (const CAccountTree *)data;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, acc);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool CAccountTree::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAccountTree::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    SFString CAccountTree::at(const SFString& _key) const {
        if (!m_root)
            return "";
        return m_root->at(_key);
    }

    //-----------------------------------------------------------------------------
    void CAccountTree::remove(const SFString& _key) {
        if (m_root)
            m_root = m_root->remove(_key);
    }

    //-----------------------------------------------------------------------------
    void CAccountTree::insert(const SFString& _key, const SFString& _value) {
        if (_value.empty())
            remove(_key);
        m_root = m_root ? m_root->insert(_key, _value) : new CLeaf(_key, _value);
    }

    //-----------------------------------------------------------------------------
    bool CAccountTree::visitItems(ACCTVISITOR func, void *data) const {
        if (m_root)
            return m_root->visitItems(func, data);
        return true;
    }

    //------------------------------------------------------------------
    bool forEveryAccount(CAccountTree *trie, ACCTVISITOR func, void *data) {
        ASSERT(trie);
        return trie->visitItems(func, data);
    }
// EXISTING_CODE
}  // namespace qblocks

