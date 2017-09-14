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
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTreeRoot, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextTreerootChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTreerootChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTreeRoot::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTreerootChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTreerootChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CTreeRoot *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTreeRoot::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
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
void CTreeRoot::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTreeRoot::Serialize(SFArchive& archive) {
    if (archive.isWriting())
        return ((const CTreeRoot*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

//    archive >> m_root;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTreeRoot::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

//    archive << m_root;

    return true;
}

//---------------------------------------------------------------------------
void CTreeRoot::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTreeRoot, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTreeRoot, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTreeRoot, "m_root", T_POINTER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTreeRoot, "schema");
    HIDE_FIELD(CTreeRoot, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTreerootChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CTreeRoot *tre = (const CTreeRoot *)dataPtr;
    if (tre) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, tre);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CTreeRoot::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTreeRoot::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CTreeRoot::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextTreerootChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'm':
        return "";
//            if ( fieldName % "m_root" ) { expContext().noFrst=true; return m_root.Format(); }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTreeRoot& item) {
    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    SFString CTreeRoot::at(const SFString& _key) const {
        if (!m_root)
            return "";
        return m_root->at(_key);
    }

    //-----------------------------------------------------------------------------
    void CTreeRoot::remove(const SFString& _key) {
        if (m_root)
            m_root = m_root->remove(_key);
    }

    //-----------------------------------------------------------------------------
    void CTreeRoot::insert(const SFString& _key, const SFString& _value) {
        if (_value.empty())
            remove(_key);
        if (verbose == 2) { cerr << "treeroot inserting " << _key << " at " << _value << "\n"; }
        m_root = m_root ? m_root->insert(_key, _value) : new CLeaf(_key, _value);
    }

    //-----------------------------------------------------------------------------
    bool CTreeRoot::visitItems(ACCTVISITOR func, void *data) const {
        if (m_root)
            return m_root->visitItems(func, data);
        return true;
    }

    //------------------------------------------------------------------
    bool forEveryAccount(CTreeRoot *trie, ACCTVISITOR func, void *data) {
        ASSERT(trie);
        return trie->visitItems(func, data);
    }
// EXISTING_CODE
}  // namespace qblocks

