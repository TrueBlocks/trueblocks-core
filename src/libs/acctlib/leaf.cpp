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
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLeaf, CTreeNode);

//---------------------------------------------------------------------------
static SFString nextLeafChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextLeafChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CLeaf::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextLeafChunk, this);
}

//---------------------------------------------------------------------------
SFString nextLeafChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CLeaf *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CLeaf::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blocks" ) {
                SFString str = fieldValue;
                while (!str.empty()) {
                    blocks[blocks.getCount()] = toUnsigned(nextTokenClear(str,','));
                }
                return true;
            }
            break;
        case 'c':
            if ( fieldName % "cnt" ) { cnt = toLong32u(fieldValue); return true; }
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

    if (archive.isWriting())
        return ((const CLeaf*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blocks;
    archive >> cnt;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLeaf::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CTreeNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << blocks;
    archive << cnt;

    return true;
}

//---------------------------------------------------------------------------
void CLeaf::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTreeNode::registerClass();

    uint32_t fieldNum = 1000;
    ADD_FIELD(CLeaf, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CLeaf, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLeaf, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLeaf, "blocks", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CLeaf, "cnt", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLeaf, "schema");
    HIDE_FIELD(CLeaf, "deleted");
    HIDE_FIELD(CLeaf, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextLeafChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CLeaf *lea = (const CLeaf *)dataPtr;
    if (lea) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, lea);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CLeaf::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CLeaf::readBackLevel(SFArchive& archive) {

    CTreeNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CLeaf::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextLeafChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blocks" || fieldName % "blocksCnt" ) {
                uint32_t cnt = blocks.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += asStringU(blocks[i]);
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'c':
            if ( fieldName % "cnt" ) return asStringU(cnt);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CTreeNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLeaf& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const SFString CLeaf::getStringAt(const SFString& name, uint32_t i) const {
    if ( name % "blocks" && i < blocks.getCount() )
        return asStringU(blocks[i]);
    return "";
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    CLeaf::CLeaf(const SFString& _key, const SFString& _value) {
#ifdef OLD_CODE_Y
        SFString last = _value;
        SFString first = nextTokenClear(last, '|');
        if (!first.empty()) {
            blocks[blocks.getCount()] = toUnsigned(first);
            if (!last.empty())
                blocks[blocks.getCount()] = toUnsigned(last);;
        }
#else
        cnt = 1;
#endif
        prefix = _key;
        if (verbose == 2) cerr << "\t\tCreating leaf " << _key << " at " << _value << endl;
    }

    //-----------------------------------------------------------------------------
    SFString CLeaf::at(const SFString& _key) const {
        if (!contains(_key))
            return "";

        SFString ret;
#ifdef OLD_CODE_Y
        for (int i = 0 ; i < blocks.getCount() ; i++) {
            ret += asString(blocks[i]);
            if (i < blocks.getCount()-1)
                ret += ",";
        }
#else
        ret = asStringU(cnt);
#endif
        return ret;
    }

    //-----------------------------------------------------------------------------
    bool CLeaf::contains(const SFString& _key) const {
        size_t l1 = _key.length();
        size_t l2 = prefix.length();
        const char *s1 = (const char*)_key;
        const char *s2 = (const char*)prefix;
        bool found = !memcmp(s1, s2, l1);

        return l1 == l2 && found;
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CLeaf::insert(const SFString& _key, const SFString& _value) {
        if (contains(_key)) {
            // If the leaf exists, we reset the value
            // We've reached the end of the key, so store the value here
//            if (first == 0) {
//                // store the first encountered block
//                if (verbose) cerr << "\t\tStoring first contents " << _key << " at " << _value << "\n";
//                first = toUnsigned(_value);
//
//            } else
            {
                // preserve the most recent block encountered
                if (verbose) cerr << "\t\tReplacing leaf contents " << _key << " at " << _value
//                    << " (" << first << ")"
                    << "\n";
#ifdef OLD_CODE_Y
                blocks[blocks.getCount()] = toUnsigned(_value);
#else
                cnt++;
#endif
            }
            return this;

        } else {
            // If the leaf is not the key, delete and convert to a branch
            if (verbose == 2) { cerr << "\tleaf branching " << _key << " at " << _value << "\n"; }
            SFString curVal;
            if (!contains(_key) || blocks.getCount() == 0) {
                curVal = "";
            } else {
#ifdef OLD_CODE_Y
                for (int i = 0 ; i < blocks.getCount() ; i++) {
                    curVal += asString(blocks[i]);
                    if (i < blocks.getCount()-1)
                        curVal += ",";
                }
#else
                curVal = asStringU(cnt);
#endif
            }
            CTreeNode *n = CTreeNode::newBranch(_key, _value, prefix, curVal);
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
        vd->cnt = cnt;
        vd->type = T_LEAF;
        vd->strs = vd->strs + "+" + (cMagenta+prefix + cOff + "|" + cBlue + at(prefix) + cOff);
        (*func)(this, data);
        nextTokenClearReverse(vd->strs, '+');
        vd->type = save;
        return true;
    }
// EXISTING_CODE
}  // namespace qblocks

