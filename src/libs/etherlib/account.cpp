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
#include "account.h"

namespace qblocks {

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CAccount, CBaseNode);

//---------------------------------------------------------------------------
static SFString nextAccountChunk(const SFString& fieldIn, const void *dataPtr);
static SFString nextAccountChunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void CAccount::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, nextAccountChunk, this);
}

//---------------------------------------------------------------------------
SFString nextAccountChunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const CAccount *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CAccount::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) { addr = toAddress(fieldValue); return true; }
            break;
        case 'd':
            if ( fieldName % "displayString" ) { displayString = fieldValue; return true; }
            break;
        case 'h':
            if ( fieldName % "header" ) { header = fieldValue; return true; }
            break;
        case 'l':
            if ( fieldName % "lastPage" ) { lastPage = toUnsigned(fieldValue); return true; }
            if ( fieldName % "lastBlock" ) { lastBlock = toLong(fieldValue); return true; }
            break;
        case 'n':
            if ( fieldName % "nVisible" ) { nVisible = toUnsigned(fieldValue); return true; }
            break;
        case 'p':
            if ( fieldName % "pageSize" ) { pageSize = toUnsigned(fieldValue); return true; }
            break;
        case 't':
            if ( fieldName % "transactions" ) {
                char *p = (char *)fieldValue.c_str();
                while (p && *p) {
                    CTransaction item;
                    uint32_t nFields = 0;
                    p = item.parseJson(p, nFields);
                    if (nFields)
                        transactions[transactions.getCount()] = item;
                }
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CAccount::finishParse() {
    // EXISTING_CODE
    for (uint32_t i = 0 ; i < transactions.getCount() ; i++) {
        CTransaction *t = &transactions[i];
        SFString encoding = t->input.substr(0,10);
        t->funcPtr = abi.findFunctionByEncoding(encoding);
    }
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CAccount::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const CAccount*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
    archive >> addr;
    archive >> header;
    archive >> displayString;
    archive >> pageSize;
    archive >> lastPage;
    archive >> lastBlock;
    archive >> nVisible;
    archive >> transactions;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CAccount::SerializeC(SFArchive& archive) const {

    // Writing always write the latest version of the data
    CBaseNode::SerializeC(archive);

    // EXISTING_CODE
    // EXISTING_CODE
    archive << addr;
    archive << header;
    archive << displayString;
    archive << pageSize;
    archive << lastPage;
    archive << lastBlock;
    archive << nVisible;
    archive << transactions;

    return true;
}

//---------------------------------------------------------------------------
void CAccount::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CAccount, "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccount, "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccount, "showing", T_BOOL,  ++fieldNum);
    ADD_FIELD(CAccount, "addr", T_ADDRESS, ++fieldNum);
    ADD_FIELD(CAccount, "header", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccount, "displayString", T_TEXT, ++fieldNum);
    ADD_FIELD(CAccount, "pageSize", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccount, "lastPage", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccount, "lastBlock", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccount, "nVisible", T_NUMBER, ++fieldNum);
    ADD_FIELD(CAccount, "transactions", T_OBJECT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CAccount, "schema");
    HIDE_FIELD(CAccount, "deleted");
    HIDE_FIELD(CAccount, "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextAccountChunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const CAccount *acc = (const CAccount *)dataPtr;
    if (acc) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'n':
                if ( fieldIn % "now" ) return (isTestMode() ? "TESTING_TIME" : Now().Format(FMT_JSON));
                break;
            case 'r':
                if ( fieldIn % "records" ) return (acc->transactions.getCount() == 0 ? "No records" : "");
                break;
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, acc);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool CAccount::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // Split the format string into three parts: pre, post and records.
    // If no records, just process as normal. We do this because it's so slow
    // copying the records into a string, so we write it directly to the
    // export context. If there is no {RECORDS}, then just send handle it like normal
    if (!fmtIn.Contains("{RECORDS}") || transactions.getCount() == 0) {
        SFString fmt = fmtIn;

        while (!fmt.empty())
            ctx << getNextChunk(fmt, nextAccountChunk, this);

    } else {
        SFString postFmt = fmtIn;
        postFmt.Replace("{RECORDS}", "|");
        SFString preFmt = nextTokenClear(postFmt, '|');

        // We assume here that the token was properly formed. For the pre-text we
        // have to clear out the start '[', and for the post text we clear out the ']'
        preFmt.ReplaceReverse("[", "");
        postFmt.Replace("]", "");

        // We handle the display in three parts: pre, records, and post so as
        // to avoid building the entire record list into an ever-growing and
        // ever-slowing string
        while (!preFmt.empty())
            ctx << getNextChunk(preFmt, nextAccountChunk, this);
        uint32_t cnt = 0;
        for (uint32_t i = 0 ; i < transactions.getCount() ; i++) {
            cnt += transactions[i].m_showing;
            if (cnt && !(cnt % REP_INFREQ)) {
                cerr << "\tExporting record " << cnt << " of " << nVisible;
                if (!isTestMode()) {
                    cerr << (transactions.getCount() != nVisible ? " visible" : "") << " records\r";
                    cerr.flush();
                }
            }

            ((CTransaction*)&transactions[i])->pParent = this;
            ctx << transactions[i].Format(displayString);
            if (cnt >= nVisible)
                break;  // no need to keep spinning if we've shown them all
        }
        ctx << "\n";
        while (!postFmt.empty())
            ctx << getNextChunk(postFmt, nextAccountChunk, this);
    }
    return true;
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CAccount::readBackLevel(SFArchive& archive) {

    CBaseNode::readBackLevel(archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
SFString CAccount::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = nextAccountChunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    // Return field values
    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "addr" ) return fromAddress(addr);
            break;
        case 'd':
            if ( fieldName % "displayString" ) return displayString;
            break;
        case 'h':
            if ( fieldName % "header" ) return header;
            break;
        case 'l':
            if ( fieldName % "lastPage" ) return asStringU(lastPage);
            if ( fieldName % "lastBlock" ) return asString(lastBlock);
            break;
        case 'n':
            if ( fieldName % "nVisible" ) return asStringU(nVisible);
            break;
        case 'p':
            if ( fieldName % "pageSize" ) return asStringU(pageSize);
            break;
        case 't':
            if ( fieldName % "transactions" || fieldName % "transactionsCnt" ) {
                uint32_t cnt = transactions.getCount();
                if (fieldName.endsWith("Cnt"))
                    return asStringU(cnt);
                if (!cnt) return "";
                SFString retS;
                for (uint32_t i = 0 ; i < cnt ; i++) {
                    retS += transactions[i].Format();
                    retS += ((i < cnt - 1) ? ",\n" : "\n");
                }
                return retS;
            }
            break;
    }

    // EXISTING_CODE
    // EXISTING_CODE

    // Finally, give the parent class a chance
    return CBaseNode::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const CAccount& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

//---------------------------------------------------------------------------
const CBaseNode *CAccount::getObjectAt(const SFString& name, uint32_t i) const {
    if ( name % "transactions" && i < transactions.getCount() )
        return &transactions[i];
    return NULL;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
uint32_t CAccount::deleteNotShowing(void) {
    uint32_t nDeleted = 0;
    for (uint32_t i = 0 ; i < transactions.getCount() ; i++) {
        CTransaction *t = &transactions[i];
        if (!t->m_showing) {
            t->setDeleted(true);
            nDeleted++;
        }
    }
    return nDeleted;
}
// EXISTING_CODE
}  // namespace qblocks

