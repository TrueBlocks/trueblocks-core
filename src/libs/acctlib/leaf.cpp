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
#include "leaf.h"
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLeaf, CTreeNode);

//---------------------------------------------------------------------------
static string_q nextLeafChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextLeafChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CLeaf::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
    if (!m_showing)
        return;

    if (fmtIn.empty()) {
        ctx << toJson();
        return;
    }

    string_q fmt = fmtIn;
    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextLeafChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLeafChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CLeaf *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CLeaf::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blocks" ) {
                string_q str = fieldValue;
                while (!str.empty()) {
                    blocks.push_back(toUnsigned(nextTokenClear(str, ',')));
                }
                return true;
            }
            break;
        case 'c':
            if ( fieldName % "counter" ) { counter = toUnsigned(fieldValue); return true; }
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
    CTreeNode::Serialize(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> blocks;
    archive >> counter;
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
    archive << counter;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CLeafArray& array) {
    uint64_t count;
    archive >> count;
    array.resize(count);
    for (size_t i = 0 ; i < count ; i++) {
        ASSERT(i < array.capacity());
        array.at(i).Serialize(archive);
    }
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CLeafArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLeaf::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    CTreeNode::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CLeaf, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CLeaf, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLeaf, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CLeaf, "blocks", T_OBJECT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CLeaf, "counter", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLeaf, "schema");
    HIDE_FIELD(CLeaf, "deleted");
    HIDE_FIELD(CLeaf, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLeafChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CLeaf *lea = (const CLeaf *)dataPtr;
    if (lea) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, lea);
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
bool CLeaf::readBackLevel(SFArchive& archive) {

    CTreeNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
string_q CLeaf::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextLeafChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if ( fieldName % "blocks" || fieldName % "blocksCnt" ) {
                size_t cnt = blocks.size();
                if (endsWith(fieldName, "Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                string_q retS;
                for (size_t i = 0 ; i < cnt ; i++) {
                    retS += asStringU(blocks[i]);
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
        case 'c':
            if ( fieldName % "counter" ) return asStringU(counter);
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
const string_q CLeaf::getStringAt(const string_q& name, size_t i) const {
    if ( name % "blocks" && i < blocks.size() )
        return asStringU(blocks[i]);
    return "";
}

//---------------------------------------------------------------------------
// EXISTING_CODE
    //-----------------------------------------------------------------------------
    CLeaf::CLeaf(const string_q& _key, const string_q& _value) {
#ifdef OLD_CODE_Y
        string_q last = _value;
        string_q first = nextTokenClear(last, '|');
        if (!first.empty()) {
            blocks.push_back(toUnsigned(first));
            if (!last.empty())
                blocks.push_back(toUnsigned(last));
        }
#else
        counter = 1;
#endif
        prefixS = _key;
        if (verbose == 2) cerr << "\t\tCreating leaf " << _key << " at " << _value << endl;
    }

    //-----------------------------------------------------------------------------
    string_q CLeaf::at(const string_q& _key) const {
        if (!contains(_key))
            return "";

        string_q ret;
#ifdef OLD_CODE_Y
        for (int i = 0 ; i < blocks.size() ; i++) {
            ret += asString(blocks[i]);
            if (i < blocks.size()-1)
                ret += ",";
        }
#else
        ret = asStringU(counter);
#endif
        return ret;
    }

    //-----------------------------------------------------------------------------
    bool CLeaf::contains(const string_q& _key) const {
        size_t l1 = _key.length();
        size_t l2 = prefixS.length();
        const char *s1 = _key.c_str();
        const char *s2 = prefixS.c_str();
        bool found = !memcmp(s1, s2, l1);

        return l1 == l2 && found;
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CLeaf::insert(const string_q& _key, const string_q& _value) {
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
                blocks.push_back(toUnsigned(_value));
#else
                counter++;
#endif
            }
            return this;

        } else {
            // If the leaf is not the key, delete and convert to a branch
            if (verbose == 2) { cerr << "\tleaf branching " << _key << " at " << _value << "\n"; }
            string_q curVal;
            if (!contains(_key) || blocks.size() == 0) {
                curVal = "";
            } else {
#ifdef OLD_CODE_Y
                for (int i = 0 ; i < blocks.size() ; i++) {
                    curVal += asString(blocks[i]);
                    if (i < blocks.size()-1)
                        curVal += ",";
                }
#else
                curVal = asStringU(counter);
#endif
            }
            CTreeNode *n = CTreeNode::newBranch(_key, _value, prefixS, curVal);
            delete this;
            return n;
        }
    }

    //-----------------------------------------------------------------------------
    CTreeNode* CLeaf::remove(const string_q& _key) {
        if (verbose)
            cerr << endl<< endl<< endl
            << idnt << string_q(80, '-') << endl
            << idnt << string_q(80, '-') << endl
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
        uint64_t save = vd->type;
        vd->counter = counter;
        vd->type = T_LEAF;
        vd->strs = vd->strs + "+" + (cMagenta+prefixS + cOff + "|" + cBlue + at(prefixS) + cOff);
        (*func)(this, data);
        nextTokenClearReverse(vd->strs, '+');
        vd->type = save;
        return true;
    }
// EXISTING_CODE
}  // namespace qblocks

