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
#include "treenode.h"
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTreeNode, CBaseNode);

//---------------------------------------------------------------------------
extern SFString nextTreenodeChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextTreenodeChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CTreeNode::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextTreenodeChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTreenodeChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CTreeNode *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CTreeNode::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "index" ) { index = toUnsigned(fieldValue); return true; }
            break;
        case 'm':
            if ( fieldName % "m_prefix" ) { m_prefix = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CTreeNode::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CTreeNode::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CTreeNode*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> index;
    archive >> m_prefix;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTreeNode::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);
    archive << index;
    archive << m_prefix;

    return true;
}

//---------------------------------------------------------------------------
void CTreeNode::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CTreeNode, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CTreeNode, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTreeNode, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CTreeNode, "index", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTreeNode, "m_prefix", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTreeNode, "schema");
    HIDE_FIELD(CTreeNode, "deleted");
    HIDE_FIELD(CTreeNode, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTreenodeChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CTreeNode *tre = (const CTreeNode *)dataPtr;
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
bool CTreeNode::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTreeNode::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CTreeNode::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextTreenodeChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // If the class has any fields, return them
    switch (tolower(fieldName[0])) {
        case 'i':
            if ( fieldName % "index" ) return asStringU(index);
            break;
        case 'm':
            if ( fieldName % "m_prefix" ) return m_prefix;
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTreeNode& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
CTreeNode* CTreeNode::newBranch(
    const SFString& _k1,
    const SFString& _v1,
    const SFString& _k2,
    const SFString& _v2) {

    unsigned prefix = commonPrefix(_k1, _k2);
    CTreeNode* ret;
    if (_k1.length() == prefix) {
        if (verbose == 2) cerr << "k1 matches up to " << prefix << endl;
        ret = new CBranch(_k2[(int)prefix], new CLeaf(_k2.substr(prefix+1), _v2), _v1);

    } else if (_k2.length() == prefix) {
        if (verbose == 2) cerr << "k2 matches up to " << prefix << endl;
        ret = new CBranch(_k1[(int)prefix], new CLeaf(_k1.substr(prefix+1), _v1), _v2);

    } else {
        // both continue after split
        if (verbose == 2) cerr << "both keys continue past prefix " << prefix << endl;
        ret = new CBranch(_k1[(int)prefix],
                          new CLeaf(_k1.substr(prefix+1), _v1),
                          _k2[(int)prefix],
                          new CLeaf(_k2.substr(prefix+1), _v2));
    }

    if (prefix) {
        // have shared prefix - split.
        if (verbose == 2) cerr << "shared prefix " << prefix << endl;
        ret = new CInfix(_k1.substr(0, prefix), ret);
    }

    return ret;
}
// EXISTING_CODE
}  // namespace qblocks

