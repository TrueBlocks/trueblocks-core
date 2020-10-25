/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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
#include <algorithm>
#include "branch.h"
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CBranch, CTreeNode);

//---------------------------------------------------------------------------
static string_q nextBranchChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextBranchChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CBranch::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["branch_fmt"] : fmtIn);
    if (fmt.empty()) {
        toJson(ctx);
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextBranchChunk, this);
}

//---------------------------------------------------------------------------
string_q nextBranchChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CBranch*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CBranch::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextBranchChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "branchValue") {
                return branchValue;
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
bool CBranch::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    if (CTreeNode::setValueByName(fieldName, fieldValue))
        return true;

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "branchValue") {
                branchValue = fieldValue;
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CBranch::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CBranch::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CTreeNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    for (int i = 0; i < 16; i++) {
        if (nodes[i]) {
            delete nodes[i];
            nodes[i] = NULL;
        }

        bool has_val = false;
        archive >> has_val;
        if (has_val) {
            string_q className;
            archive >> className;
            nodes[i] = createTreeNode(className);
            if (!nodes[i])
                return false;
            nodes[i]->Serialize(archive);
        }
    }
    // EXISTING_CODE
    archive >> branchValue;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CBranch::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CTreeNode::SerializeC(archive);

    // EXISTING_CODE
    for (int i = 0; i < 16; i++) {
        archive << bool(nodes[i] != NULL);
        if (nodes[i]) {
            string_q className = nodes[i]->getRuntimeClass()->getClassNamePtr();
            archive << className;
            nodes[i]->SerializeC(archive);
        }
    }
    // EXISTING_CODE
    archive << branchValue;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CBranchArray& array) {
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
CArchive& operator<<(CArchive& archive, const CBranchArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CBranch::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CBranch, "schema"))
        return;

    CTreeNode::registerClass();

    size_t fieldNum = 1000;
    ADD_FIELD(CBranch, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CBranch, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CBranch, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CBranch, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CBranch, "branchValue", T_TEXT, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CBranch, "schema");
    HIDE_FIELD(CBranch, "deleted");
    HIDE_FIELD(CBranch, "showing");
    HIDE_FIELD(CBranch, "cname");

    builtIns.push_back(_biCBranch);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextBranchChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CBranch* bra = reinterpret_cast<const CBranch*>(dataPtr);
    if (bra) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'n':
                if (fieldIn % "nodes") {
                    string_q ret;
                    for (int i = 0; i < 16; i++) {
                        if (bra->nodes[i]) {
                            ret += bra->nodes[i]->Format();
                        }
                    }
                    return ret;
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, bra);
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
bool CBranch::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CBranch& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_BRANCH = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
//-----------------------------------------------------------------------------
CBranch::CBranch(const string_q& _value) : branchValue(_value) {
    memset(nodes, 0, sizeof(CTreeNode*) * 16);
}

//-----------------------------------------------------------------------------
CBranch::CBranch(char _i1, CTreeNode* _n1, const string_q& _value) : branchValue(_value) {
    memset(nodes, 0, sizeof(CTreeNode*) * 16);
    nodes[nodeIndex(_i1)] = _n1;
}

//-----------------------------------------------------------------------------
CBranch::CBranch(char _i1, CTreeNode* _n1, char _i2, CTreeNode* _n2) {
    memset(nodes, 0, sizeof(CTreeNode*) * 16);
    nodes[nodeIndex(_i1)] = _n1;
    nodes[nodeIndex(_i2)] = _n2;
}

//-----------------------------------------------------------------------------
string_q CBranch::at(const string_q& _key) const {
    if (_key.empty())
        return branchValue;

    int idx = nodeIndex(_key[0]);
    if (nodes[idx])
        return nodes[idx]->at(extract(_key, 1));

    return "";
}

//-----------------------------------------------------------------------------
CTreeNode* CBranch::insert(const string_q& _key, const string_q& _value) {
    if (verbose == 2) {
        cerr << "\tbranch inserting " << _key << " at " << _value << "\n";
    }
    if (_key.empty()) {
        // We've reached the end of the key, so store the value here
        if (branchValue.empty()) {
            // store the first encountered block
            branchValue = _value;

        } else {
            // preserve the most recent block encountered
            branchValue = nextTokenClear(branchValue, '|');
            branchValue += "|" + _value;
        }

    } else {
        // Figure out which bucket to store the value in by the next character in the key
        int idx = nodeIndex(_key[0]);
        if (nodes[nodeIndex(_key[0])]) {
            // There is already something stored here, so we need to find room for it
            nodes[idx] = nodes[idx]->insert(extract(_key, 1), _value);

        } else {
            // we've reached a leaf
            nodes[idx] = new CLeaf(extract(_key, 1), _value);
        }
    }
    return this;
}

//-----------------------------------------------------------------------------
CTreeNode* CBranch::remove(const string_q& _key) {
    if (verbose) {
        cerr << endl
             << endl
             << endl
             << idnt << string_q(80, '-') << endl
             << idnt << string_q(80, '-') << endl
             << idnt << "remove branch at [" << _key << "]: ";
        idnt += "\t";
    }

    if (_key.empty()) {
        if (branchValue.length()) {
            if (verbose)
                cerr << "and non-empty value " << endl;
            branchValue = "";
            return rejig();
        }
        if (verbose)
            cerr << "and empty value" << endl;
        replace(idnt, "\t", "");
        return this;
    }

    int idx = nodeIndex(_key[0]);
    if (nodes[idx]) {
        const char* k = _key.c_str();
        CTreeNode* ret = nodes[idx]->remove(&k[1]);
        nodes[idx] = ret;
        CTreeNode* n = rejig();
        if (verbose)
            replace(idnt, "\t", "");
        return n;
    }

    if (verbose) {
        cerr << endl;
        replace(idnt, "\t", "");
    }
    return this;
}

//-----------------------------------------------------------------------------
#define NO_BRANCHS ((char)-1)
#define MULTI_BRANCH ((char)16)
CTreeNode* CBranch::rejig() {
    char n = activeBranch();
    if (n == NO_BRANCHS && branchValue.length()) {
        if (verbose)
            cerr << "No branches, but we have a value, so we save it as a leaf" << endl;
        // revert back down to a leaf
        CTreeNode* r = new CLeaf("", branchValue);
        delete this;
        return r;

    } else if (n != MULTI_BRANCH && branchValue.empty()) {
        if (verbose == 2)
            cerr << idnt << "This single (empty) branch has a single child at [" << idex(n) << "]" << endl;
        // only branching to n...
        int nn = static_cast<int>(n);
        if (auto b = dynamic_cast<CBranch*>(nodes[nn])) {
            // switch to infix
            nodes[nn] = NULL;
            delete this;
            string_q x = idex(n);
            return new CInfix(x, b);

        } else {
            auto x = nodes[nn];
            ASSERT(x);
            // include in child
            x->prefixS = idex(n) + x->prefixS;
            nodes[nn] = NULL;
            delete this;
            return x;
        }
    }

    return this;
}

//-----------------------------------------------------------------------------
/// @returns (byte)-1 when no active branches, 16 when multiple active and the index of the active branch otherwise.
char CBranch::activeBranch() const {
    char n = NO_BRANCHS;
    for (int i = 0; i < 16; i++) {
        if (nodes[i]) {
            if (n == NO_BRANCHS)
                n = static_cast<char>(i);
            else
                return MULTI_BRANCH;
        }
    }
    return n;
}

//------------------------------------------------------------------
bool CBranch::visitItems(ACCTVISITOR func, void* data) const {
    ASSERT(func);
    CVisitData* vd = reinterpret_cast<CVisitData*>(data);
    uint64_t save = vd->type;
    vd->type = T_BRANCH;
    vd->counter = 0;
    vd->strs = vd->strs + branchValue + "+";
    // vd->strs = vd->strs + "+";
    (*func)(this, data);
    for (size_t i = 0; i < 16; ++i) {
        if (nodes[i]) {
            vd->level++;
            vd->strs = vd->strs + "-" + idex((char)i);  // NOLINT
            nodes[i]->visitItems(func, data);
            nextTokenClearReverse(vd->strs, '-');
            vd->level--;
        }
    }
    nextTokenClearReverse(vd->strs, '+');
    vd->type = save;
    return true;
}
// EXISTING_CODE
}  // namespace qblocks
