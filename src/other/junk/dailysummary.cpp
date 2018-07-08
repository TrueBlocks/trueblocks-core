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
#include <algorithm>
#include "dailysummary.h"
#include "etherlib.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CDailySummary, CBaseNode);

//---------------------------------------------------------------------------
extern string_q nextDailysummaryChunk(const string_q& fieldIn, const void *dataPtr);
static string_q nextDailysummaryChunk_custom(const string_q& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CDailySummary::Format(ostream& ctx, const string_q& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextDailysummaryChunk, this);
}

//---------------------------------------------------------------------------
string_q nextDailysummaryChunk(const string_q& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CDailySummary *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CDailySummary::setValueByName(const string_q& fieldName, const string_q& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'e':
            if ( fieldName % "endBlock" ) { endBlock = toUnsigned(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nBlocks" ) { nBlocks = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nTraces" ) { nTraces = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nTxErrors" ) { nTxErrors = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nTcErrors" ) { nTcErrors = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nSuccess" ) { nSuccess = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nReward" ) { nReward = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nTxCreates" ) { nTxCreates = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nTcCreates" ) { nTcCreates = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nTcFails" ) { nTcFails = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nCalls" ) { nCalls = toUnsigned(fieldValue); return true; }
            if ( fieldName % "nSelfDestruct" ) { nSelfDestruct = toUnsigned(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "startBlock" ) { startBlock = toUnsigned(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "timestamp" ) { timestamp = toTimestamp(fieldValue); return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CDailySummary::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CDailySummary::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> timestamp;
    archive >> nBlocks;
    archive >> startBlock;
    archive >> endBlock;
    archive >> nTraces;
    archive >> nTxErrors;
    archive >> nTcErrors;
    archive >> nSuccess;
    archive >> nReward;
    archive >> nTxCreates;
    archive >> nTcCreates;
    archive >> nTcFails;
    archive >> nCalls;
    archive >> nSelfDestruct;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CDailySummary::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << timestamp;
    archive << nBlocks;
    archive << startBlock;
    archive << endBlock;
    archive << nTraces;
    archive << nTxErrors;
    archive << nTcErrors;
    archive << nSuccess;
    archive << nReward;
    archive << nTxCreates;
    archive << nTcCreates;
    archive << nTcFails;
    archive << nCalls;
    archive << nSelfDestruct;

    return true;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CDailySummaryArray& array) {
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
SFArchive& operator<<(SFArchive& archive, const CDailySummaryArray& array) {
    uint64_t count = array.size();
    archive << count;
    for (size_t i = 0 ; i < array.size() ; i++)
        array[i].SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
void CDailySummary::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    size_t fieldNum = 1000;
    ADD_FIELD(CDailySummary, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CDailySummary, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CDailySummary, "timestamp", T_TIMESTAMP, ++fieldNum);
    ADD_FIELD(CDailySummary, "nBlocks", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "startBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "endBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nTraces", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nTxErrors", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nTcErrors", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nSuccess", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nReward", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nTxCreates", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nTcCreates", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nTcFails", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nCalls", T_NUMBER, ++fieldNum);
    ADD_FIELD(CDailySummary, "nSelfDestruct", T_NUMBER, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CDailySummary, "schema");
    HIDE_FIELD(CDailySummary, "deleted");
    HIDE_FIELD(CDailySummary, "showing");

    builtIns.push_back(_biCDailySummary);

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
string_q nextDailysummaryChunk_custom(const string_q& fieldIn, const void *dataPtr) {
    const CDailySummary *dai = (const CDailySummary *)dataPtr;  // NOLINT
    if (dai) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'd':
                if ( fieldIn % "date") {
                    SFTime date = dateFromTimeStamp(dai->timestamp);
                    return
                        trimLeading(extract(date.Format(FMT_JSON),5,2),'0') + "/" +
                        trimLeading(extract(date.Format(FMT_JSON),8,2),'0') + "/" +
                        extract(date.Format(FMT_JSON),2,2);
                }
                break;
            case 'n':
                if ( fieldIn % "nSuccess")
                    return asStringU(dai->nTraces - dai->nTcErrors);
                else if ( fieldIn % "nCreates")
                    return asStringU(dai->nTxCreates + dai->nTcCreates + dai->nTcFails);
                else if ( fieldIn % "nErrors")
                    return asStringU(dai->nTxErrors + dai->nTcErrors);
                break;
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, dai);
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
bool CDailySummary::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFArchive& operator<<(SFArchive& archive, const CDailySummary& dai) {
    dai.SerializeC(archive);
    return archive;
}

//---------------------------------------------------------------------------
SFArchive& operator>>(SFArchive& archive, CDailySummary& dai) {
    dai.Serialize(archive);
    return archive;
}

//---------------------------------------------------------------------------
string_q CDailySummary::getValueByName(const string_q& fieldName) const {

    // Give customized code a chance to override first
    string_q ret = nextDailysummaryChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'e':
            if ( fieldName % "endBlock" ) return asStringU(endBlock);
            break;
        case 'n':
            if ( fieldName % "nBlocks" ) return asStringU(nBlocks);
            if ( fieldName % "nTraces" ) return asStringU(nTraces);
            if ( fieldName % "nTxErrors" ) return asStringU(nTxErrors);
            if ( fieldName % "nTcErrors" ) return asStringU(nTcErrors);
            if ( fieldName % "nSuccess" ) return asStringU(nSuccess);
            if ( fieldName % "nReward" ) return asStringU(nReward);
            if ( fieldName % "nTxCreates" ) return asStringU(nTxCreates);
            if ( fieldName % "nTcCreates" ) return asStringU(nTcCreates);
            if ( fieldName % "nTcFails" ) return asStringU(nTcFails);
            if ( fieldName % "nCalls" ) return asStringU(nCalls);
            if ( fieldName % "nSelfDestruct" ) return asStringU(nSelfDestruct);
            break;
        case 's':
            if ( fieldName % "startBlock" ) return asStringU(startBlock);
            break;
        case 't':
            if ( fieldName % "timestamp" ) return fromTimestamp(timestamp);
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CDailySummary& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    item.Format(os, "", nullptr);
    os << "\n";
    return os;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
SFTime fromStr(const string_q& dateStr) {
    string_q dStr = dateStr;
    uint32_t m = (uint32_t)toUnsigned(nextTokenClear(dStr, '/'));
    uint32_t d = (uint32_t)toUnsigned(nextTokenClear(dStr, '/'));
    uint32_t y = 2000 + (uint32_t)toUnsigned(nextTokenClear(dStr, '/'));
    return SFTime(y, m, d, 0, 0, 0);
}

CDailySummary::CDailySummary(const string_q& lineIn) {
    string_q line = lineIn;
    while (!line.empty()) {
        timestamp = toTimestamp(fromStr(nextTokenClear(line, '\t')));
        nBlocks = toUnsigned(nextTokenClear(line, '\t'));
        startBlock = toUnsigned(nextTokenClear(line, '\t'));
        endBlock = toUnsigned(nextTokenClear(line, '\t'));
        nTraces = toUnsigned(nextTokenClear(line, '\t'));
        nTxErrors = toUnsigned(nextTokenClear(line, '\t'));
        nTcErrors = toUnsigned(nextTokenClear(line, '\t'));
                    toUnsigned(nextTokenClear(line, '\t'));
        nTxCreates = toUnsigned(nextTokenClear(line, '\t'));
        nTcCreates = toUnsigned(nextTokenClear(line, '\t'));
        nTcFails = toUnsigned(nextTokenClear(line, '\t'));
                    toUnsigned(nextTokenClear(line, '\t'));
        nCalls = toUnsigned(nextTokenClear(line, '\t'));
        nSelfDestruct = toUnsigned(nextTokenClear(line, '\t'));
    }
}
// EXISTING_CODE

