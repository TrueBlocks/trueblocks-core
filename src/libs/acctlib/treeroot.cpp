/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
#include "treeroot.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CTreeRoot, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextTreerootChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextTreerootChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CTreeRoot::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["treeroot_fmt"] : fmtIn);
    if (fmt.empty()) {
        if (expContext().exportFmt == YAML1) {
            toYaml(ctx);
        } else {
            toJson(ctx);
        }
        return;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    while (!fmt.empty())
        ctx << getNextChunk(fmt, nextTreerootChunk, this);
}

//---------------------------------------------------------------------------
string_q nextTreerootChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CTreeRoot*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CTreeRoot::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextTreerootChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'r':
            if (fieldName % "root") {
                if (root)
                    return root->Format();
                return "";
            }
            break;
        default:
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//---------------------------------------------------------------------------------------------------
bool CTreeRoot::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'r':
            if (fieldName % "root") {
                // This drops memory, so we comment it out for now
                // clear();
                // root = new CTreeNode;
                // if (root) {
                //     string_q str = fieldValue;
                //     return root->parseJson3(str);
                // }
                return false;
            }
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
bool CTreeRoot::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    root = NULL;
    bool has_root = false;
    archive >> has_root;
    if (has_root) {
        string_q className;
        archive >> className;
        root = reinterpret_cast<CTreeNode*>(createObjectOfType(className));
        if (!root)
            return false;
        root->Serialize(archive);
    }
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CTreeRoot::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << (root != NULL);
    if (root) {
        archive << root->getRuntimeClass()->getClassNamePtr();
        root->SerializeC(archive);
    }
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CTreeRootArray& array) {
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
CArchive& operator<<(CArchive& archive, const CTreeRootArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CTreeRoot::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CTreeRoot, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CTreeRoot, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CTreeRoot, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CTreeRoot, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CTreeRoot, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CTreeRoot, "root", T_POINTER | TS_OMITEMPTY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CTreeRoot, "schema");
    HIDE_FIELD(CTreeRoot, "deleted");
    HIDE_FIELD(CTreeRoot, "showing");
    HIDE_FIELD(CTreeRoot, "cname");

    builtIns.push_back(_biCTreeRoot);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextTreerootChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CTreeRoot* tre = reinterpret_cast<const CTreeRoot*>(dataPtr);
    if (tre) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, tre);
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
bool CTreeRoot::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CTreeRoot& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode* CTreeRoot::getObjectAt(const string_q& fieldName, size_t index) const {
    if (fieldName % "root")
        return root;

    return NULL;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_TREEROOT = "";

//---------------------------------------------------------------------------
// EXISTING_CODE
string_q idex(char n) {
    switch (n) {
        case 0:
            return "0";
        case 1:
            return "1";
        case 2:
            return "2";
        case 3:
            return "3";
        case 4:
            return "4";
        case 5:
            return "5";
        case 6:
            return "6";
        case 7:
            return "7";
        case 8:
            return "8";
        case 9:
            return "9";
        case 10:
            return "a";
        case 11:
            return "b";
        case 12:
            return "c";
        case 13:
            return "d";
        case 14:
            return "e";
        case 15:
            return "f";
    }
    cerr << "should never happen";
    quickQuitHandler(1);
    return "";
}

//-----------------------------------------------------------------------------
string_q CTreeRoot::at(const string_q& _key) const {
    if (!root)
        return "";
    return root->at(_key);
}

//-----------------------------------------------------------------------------
void CTreeRoot::remove(const string_q& _key) {
    if (root)
        root = root->remove(_key);
}

//-----------------------------------------------------------------------------
void CTreeRoot::insert(const string_q& _key, const string_q& _value) {
    if (_value.empty())
        remove(_key);
    if (verbose == 2) {
        cerr << "treeroot inserting " << _key << " at " << _value << "\n";
    }
    root = root ? root->insert(_key, _value) : new CLeaf(_key, _value);
}

//-----------------------------------------------------------------------------
bool CTreeRoot::visitItems(ACCTVISITOR func, void* data) const {
    ASSERT(func);
    CVisitData* vd = reinterpret_cast<CVisitData*>(data);
    vd->level = 0;
    if (root) {
        bool ret = root->visitItems(func, data);
        return ret;
    }
    return true;
}

//------------------------------------------------------------------
bool forEveryAccount(CTreeRoot* trie, ACCTVISITOR func, void* data) {
    ASSERT(trie);
    return trie->visitItems(func, data);
}
// EXISTING_CODE
}  // namespace qblocks
