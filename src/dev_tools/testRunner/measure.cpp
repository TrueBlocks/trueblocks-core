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
#include "measure.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CMeasure, CBaseNode);

//---------------------------------------------------------------------------
static string_q nextMeasureChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextMeasureChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CMeasure::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["measure_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextMeasureChunk, this);
}

//---------------------------------------------------------------------------
string_q nextMeasureChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CMeasure*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CMeasure::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextMeasureChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cmd") {
                return cmd;
            }
            break;
        case 'd':
            if (fieldName % "date") {
                return date;
            }
            break;
        case 'e':
            if (fieldName % "epoch") {
                return epoch;
            }
            break;
        case 'g':
            if (fieldName % "git_hash") {
                return git_hash;
            }
            if (fieldName % "group") {
                return group;
            }
            break;
        case 'm':
            if (fieldName % "machine") {
                return machine;
            }
            break;
        case 'n':
            if (fieldName % "node") {
                return node;
            }
            if (fieldName % "nTests") {
                return uint_2_Str(nTests);
            }
            if (fieldName % "nPassed") {
                return uint_2_Str(nPassed);
            }
            break;
        case 't':
            if (fieldName % "type") {
                return type;
            }
            if (fieldName % "totSecs") {
                return double_2_Str(totSecs, 5);
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
bool CMeasure::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'c':
            if (fieldName % "cmd") {
                cmd = fieldValue;
                return true;
            }
            break;
        case 'd':
            if (fieldName % "date") {
                date = fieldValue;
                return true;
            }
            break;
        case 'e':
            if (fieldName % "epoch") {
                epoch = fieldValue;
                return true;
            }
            break;
        case 'g':
            if (fieldName % "git_hash") {
                git_hash = fieldValue;
                return true;
            }
            if (fieldName % "group") {
                group = fieldValue;
                return true;
            }
            break;
        case 'm':
            if (fieldName % "machine") {
                machine = fieldValue;
                return true;
            }
            break;
        case 'n':
            if (fieldName % "node") {
                node = fieldValue;
                return true;
            }
            if (fieldName % "nTests") {
                nTests = str_2_Uint(fieldValue);
                return true;
            }
            if (fieldName % "nPassed") {
                nPassed = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 't':
            if (fieldName % "type") {
                type = fieldValue;
                return true;
            }
            if (fieldName % "totSecs") {
                totSecs = str_2_Double(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CMeasure::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CMeasure::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> git_hash;
    archive >> date;
    archive >> machine;
    archive >> node;
    archive >> epoch;
    archive >> group;
    archive >> cmd;
    archive >> type;
    archive >> nTests;
    archive >> nPassed;
    archive >> totSecs;
    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CMeasure::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << git_hash;
    archive << date;
    archive << machine;
    archive << node;
    archive << epoch;
    archive << group;
    archive << cmd;
    archive << type;
    archive << nTests;
    archive << nPassed;
    archive << totSecs;
    // EXISTING_CODE
    // EXISTING_CODE
    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CMeasureArray& array) {
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
CArchive& operator<<(CArchive& archive, const CMeasureArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CMeasure::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CMeasure, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CMeasure, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CMeasure, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CMeasure, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CMeasure, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CMeasure, "git_hash", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "date", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "machine", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "node", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "epoch", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "group", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "cmd", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "type", T_TEXT | TS_OMITEMPTY, ++fieldNum);
    ADD_FIELD(CMeasure, "nTests", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CMeasure, "nPassed", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CMeasure, "totSecs", T_DOUBLE, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CMeasure, "schema");
    HIDE_FIELD(CMeasure, "deleted");
    HIDE_FIELD(CMeasure, "showing");
    HIDE_FIELD(CMeasure, "cname");

    builtIns.push_back(_biCMeasure);

    // EXISTING_CODE
    ADD_FIELD(CMeasure, "avgSecs", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CMeasure, "check", T_TEXT, ++fieldNum);
    ADD_FIELD(CMeasure, "failed", T_TEXT, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextMeasureChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CMeasure* mea = reinterpret_cast<const CMeasure*>(dataPtr);
    if (mea) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if (fieldIn % "avgSecs")
                    return double_2_Str(mea->avgSecs(), 5);
                break;
            case 'c':
                if (fieldIn % "check")
                    return (mea->nPassed == mea->nTests ? greenCheck : "");
                break;
            case 'e':
                if (fieldIn % "epoch")
                    return substitute(mea->epoch, "E-", (mea->allPassed ? "E-" : "F-"));
                break;
            case 'f':
                if (fieldIn % "failed")
                    return uint_2_Str(mea->nTests - mea->nPassed);
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, mea);
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
bool CMeasure::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CMeasure& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_MEASURE =
    "[{GIT_HASH}]\t"
    "[{DATE}]\t"
    "[{MACHINE}]\t"
    "[{NODE}]\t"
    "[{EPOCH}]\t"
    "[{GROUP}]\t"
    "[{CMD}]\t"
    "[{TYPE}]\t"
    "[{NTESTS}]\t"
    "[{NPASSED}]\t"
    "[{TOTSECS}]\t"
    "[{AVGSECS}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
CMeasure::CMeasure(const string_q& g, const string_q& c, const string_q& t) {
    initialize();
    git_hash = "git_" + string_q(GIT_COMMIT_HASH).substr(0, 10);
    date = Now().Format(FMT_EXPORT);
    machine = toLower(getHostName());
    node = isErigon() ? "TG" : isParity() ? "OE" : "OT";
    epoch = getGlobalConfig("testRunner")->getConfigStr("settings", "test_epoch", "E-12");
    group = g;
    cmd = c;
    type = t;
}
// EXISTING_CODE
}  // namespace qblocks
