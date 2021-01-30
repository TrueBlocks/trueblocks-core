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
#include "lmdbstat.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CLmdbStat, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextLmdbstatChunk(const string_q& fieldIn, const void* dataPtr);
static string_q nextLmdbstatChunk_custom(const string_q& fieldIn, const void* dataPtr);

//---------------------------------------------------------------------------
void CLmdbStat::Format(ostream& ctx, const string_q& fmtIn, void* dataPtr) const {
    if (!m_showing)
        return;

    // EXISTING_CODE
    // EXISTING_CODE

    string_q fmt = (fmtIn.empty() ? expContext().fmtMap["lmdbstat_fmt"] : fmtIn);
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
        ctx << getNextChunk(fmt, nextLmdbstatChunk, this);
}

//---------------------------------------------------------------------------
string_q nextLmdbstatChunk(const string_q& fieldIn, const void* dataPtr) {
    if (dataPtr)
        return reinterpret_cast<const CLmdbStat*>(dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------
string_q CLmdbStat::getValueByName(const string_q& fieldName) const {
    // Give customized code a chance to override first
    string_q ret = nextLmdbstatChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // EXISTING_CODE
    // EXISTING_CODE

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "branch_pages") {
                return uint_2_Str(branch_pages);
            }
            break;
        case 'd':
            if (fieldName % "depth") {
                return uint_2_Str(depth);
            }
            break;
        case 'e':
            if (fieldName % "entries") {
                return uint_2_Str(entries);
            }
            break;
        case 'l':
            if (fieldName % "leaf_pages") {
                return uint_2_Str(leaf_pages);
            }
            break;
        case 'o':
            if (fieldName % "overflow_pages") {
                return uint_2_Str(overflow_pages);
            }
            break;
        case 'p':
            if (fieldName % "psize") {
                return uint_2_Str(psize);
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
bool CLmdbStat::setValueByName(const string_q& fieldNameIn, const string_q& fieldValueIn) {
    string_q fieldName = fieldNameIn;
    string_q fieldValue = fieldValueIn;

    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'b':
            if (fieldName % "branch_pages") {
                branch_pages = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'd':
            if (fieldName % "depth") {
                depth = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'e':
            if (fieldName % "entries") {
                entries = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'l':
            if (fieldName % "leaf_pages") {
                leaf_pages = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'o':
            if (fieldName % "overflow_pages") {
                overflow_pages = str_2_Uint(fieldValue);
                return true;
            }
            break;
        case 'p':
            if (fieldName % "psize") {
                psize = str_2_Uint(fieldValue);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CLmdbStat::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CLmdbStat::Serialize(CArchive& archive) {
    if (archive.isWriting())
        return SerializeC(archive);

    // Always read the base class (it will handle its own backLevels if any, then
    // read this object's back level (if any) or the current version.
    CBaseNode::Serialize(archive);
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> psize;
    archive >> depth;
    archive >> branch_pages;
    archive >> leaf_pages;
    archive >> overflow_pages;
    archive >> entries;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CLmdbStat::SerializeC(CArchive& archive) const {
    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << psize;
    archive << depth;
    archive << branch_pages;
    archive << leaf_pages;
    archive << overflow_pages;
    archive << entries;

    return true;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLmdbStatArray& array) {
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
CArchive& operator<<(CArchive& archive, const CLmdbStatArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0; i < array.size(); i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CLmdbStat::registerClass(void) {
    // only do this once
    if (HAS_FIELD(CLmdbStat, "schema"))
        return;

    size_t fieldNum = 1000;
    ADD_FIELD(CLmdbStat, "schema", T_NUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "deleted", T_BOOL, ++fieldNum);
    ADD_FIELD(CLmdbStat, "showing", T_BOOL, ++fieldNum);
    ADD_FIELD(CLmdbStat, "cname", T_TEXT, ++fieldNum);
    ADD_FIELD(CLmdbStat, "psize", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "depth", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "branch_pages", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "leaf_pages", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "overflow_pages", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "entries", T_UNUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CLmdbStat, "schema");
    HIDE_FIELD(CLmdbStat, "deleted");
    HIDE_FIELD(CLmdbStat, "showing");
    HIDE_FIELD(CLmdbStat, "cname");

    builtIns.push_back(_biCLmdbStat);

    // EXISTING_CODE
    ADD_FIELD(CLmdbStat, "total_pages", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "total_bytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "branch_bytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "leaf_bytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "overflow_bytes", T_UNUMBER, ++fieldNum);
    ADD_FIELD(CLmdbStat, "total_pct", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CLmdbStat, "branch_pct", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CLmdbStat, "leaf_pct", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CLmdbStat, "overflow_pct", T_DOUBLE, ++fieldNum);
    ADD_FIELD(CLmdbStat, "avg", T_DOUBLE, ++fieldNum);
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextLmdbstatChunk_custom(const string_q& fieldIn, const void* dataPtr) {
    const CLmdbStat* lmd = reinterpret_cast<const CLmdbStat*>(dataPtr);
    if (lmd) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'a':
                if (fieldIn % "avg") {
                    if (lmd->entries == 0)
                        return "0.00";
                    uint64_t total = lmd->psize * (lmd->branch_pages + lmd->leaf_pages + lmd->overflow_pages);
                    double avg = ((double)total / (double)lmd->entries);
                    return double_2_Str(avg, 2);
                }
                break;
            case 'b':
                if (fieldIn % "branch_bytes") {
                    if (lmd->branch_pages == 0)
                        return "0";
                    return uint_2_Str(lmd->psize * lmd->branch_pages);
                }
                if (fieldIn % "branch_pct") {
                    if (lmd->branch_pages == 0)
                        return "0.0";
                    double total = double(lmd->branch_pages + lmd->leaf_pages + lmd->overflow_pages);
                    return double_2_Str((((double)lmd->branch_pages) / total) * 100, 1);
                }
                break;
            case 'l':
                if (fieldIn % "leaf_bytes") {
                    if (lmd->leaf_pages == 0)
                        return "0";
                    return uint_2_Str(lmd->psize * lmd->leaf_pages);
                }
                if (fieldIn % "leaf_pct") {
                    if (lmd->leaf_pages == 0)
                        return "0.0";
                    double total = double(lmd->branch_pages + lmd->leaf_pages + lmd->overflow_pages);
                    return double_2_Str((((double)lmd->leaf_pages) / total) * 100, 1);
                }
                break;
            case 'o':
                if (fieldIn % "overflow_bytes") {
                    if (lmd->overflow_pages == 0)
                        return "0";
                    return uint_2_Str(lmd->psize * lmd->overflow_pages);
                }
                if (fieldIn % "overflow_pct") {
                    if (lmd->overflow_pages == 0)
                        return "0.0";
                    double total = double(lmd->branch_pages + lmd->leaf_pages + lmd->overflow_pages);
                    return double_2_Str((((double)lmd->overflow_pages) / total) * 100, 1);
                }
                break;
            case 't':
                if (fieldIn % "total_pages") {
                    uint64_t total = (lmd->branch_pages + lmd->leaf_pages + lmd->overflow_pages);
                    return uint_2_Str(total);
                }
                if (fieldIn % "total_bytes") {
                    uint64_t total = lmd->psize * (lmd->branch_pages + lmd->leaf_pages + lmd->overflow_pages);
                    if (total == 0)
                        return "0";
                    return uint_2_Str(total);
                }
                if (fieldIn % "total_pct") {
                    uint64_t total = (lmd->branch_pages + lmd->leaf_pages + lmd->overflow_pages);
                    if (total == 0)
                        return "0.0";
                    return double_2_Str(100, 1);
                }
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if (fieldIn % "parsed")
                    return nextBasenodeChunk(fieldIn, lmd);
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
bool CLmdbStat::readBackLevel(CArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
CArchive& operator<<(CArchive& archive, const CLmdbStat& lmd) {
    lmd.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
CArchive& operator>>(CArchive& archive, CLmdbStat& lmd) {
    lmd.Serialize(archive);
    return archive;
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CLmdbStat& it) {
    // EXISTING_CODE
    // EXISTING_CODE

    it.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
const char* STR_DISPLAY_LMDBSTAT =
    "[{PSIZE}]\t"
    "[{DEPTH}]\t"
    "[{BRANCH_PAGES}]\t"
    "[{LEAF_PAGES}]\t"
    "[{OVERFLOW_PAGES}]\t"
    "[{ENTRIES}]";

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
}  // namespace qblocks
