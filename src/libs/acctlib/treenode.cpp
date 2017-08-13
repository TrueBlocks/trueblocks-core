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
IMPLEMENT_NODE(CTreeNode, CBaseNode, curVersion);

//---------------------------------------------------------------------------
extern SFString nextTreenodeChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextTreenodeChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CTreeNode::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextTreenodeChunk, this);
}

//---------------------------------------------------------------------------
SFString nextTreenodeChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CTreeNode *tre = (const CTreeNode *)data;
    if (tre) {
        // Give customized code a chance to override first
        SFString ret = nextTreenodeChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'i':
                if ( fieldIn % "index" ) return asStringU(tre->index);
                break;
            case 'm':
                if ( fieldIn % "m_prefix" ) return tre->m_prefix;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, tre);
        if (!ret.empty())
            return ret;
    }

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
    if (!archive.isReading())
        return ((const CTreeNode*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> index;
    archive >> m_prefix;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTreeNode::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

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
    ADD_FIELD(CTreeNode, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CTreeNode, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CTreeNode, "index", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTreeNode, "m_prefix", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTreeNode, "schema");
    HIDE_FIELD(CTreeNode, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextTreenodeChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CTreeNode *tre = (const CTreeNode *)data;
    if (tre) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, tre);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CTreeNode::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CTreeNode::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
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
        ret = new CBranch(_k2[prefix], new CLeaf(_k2.substr(prefix+1), _v2), _v1);

    } else if (_k2.length() == prefix) {
        if (verbose == 2) cerr << "k2 matches up to " << prefix << endl;
        ret = new CBranch(_k1[prefix], new CLeaf(_k1.substr(prefix+1), _v1), _v2);

    } else {
        // both continue after split
        if (verbose == 2) cerr << "both keys continue past prefix " << prefix << endl;
        ret = new CBranch(_k1[prefix],
                          new CLeaf(_k1.substr(prefix+1), _v1),
                          _k2[prefix],
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

