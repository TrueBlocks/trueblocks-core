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
#include "function.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CFunction, CBaseNode, curVersion);

//---------------------------------------------------------------------------
static SFString nextFunctionChunk(const SFString& fieldIn, bool& force, const void *data);
static SFString nextFunctionChunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void CFunction::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, nextFunctionChunk, this);
}

//---------------------------------------------------------------------------
SFString nextFunctionChunk(const SFString& fieldIn, bool& force, const void *data) {
    const CFunction *fun = (const CFunction *)data;
    if (fun) {
        // Give customized code a chance to override first
        SFString ret = nextFunctionChunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
            case 'a':
                if ( fieldIn % "anonymous" ) return asString(fun->anonymous);
                break;
            case 'c':
                if ( fieldIn % "constant" ) return asString(fun->constant);
                break;
            case 'e':
                if ( fieldIn % "encoding" ) return fun->encoding;
                break;
            case 'i':
                if ( fieldIn % "inputs" ) {
                    uint32_t cnt = fun->inputs.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        ret += fun->inputs[i].Format();
                        ret += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
            case 'n':
                if ( fieldIn % "name" ) return fun->name;
                break;
            case 'o':
                if ( fieldIn % "outputs" ) {
                    uint32_t cnt = fun->outputs.getCount();
                    if (!cnt) return EMPTY;
                    SFString ret;
                    for (uint32_t i = 0 ; i < cnt ; i++) {
                        ret += fun->outputs[i].Format();
                        ret += ((i < cnt - 1) ? ",\n" : "\n");
                    }
                    return ret;
                }
                break;
            case 'p':
                if ( fieldIn % "payable" ) return asString(fun->payable);
                break;
            case 's':
                if ( fieldIn % "signature" ) return fun->signature;
                break;
            case 't':
                if ( fieldIn % "type" ) return fun->type;
                break;
        }

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        ret = nextBasenodeChunk(fieldIn, force, fun);
        if (!ret.empty())
            return ret;
    }

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool CFunction::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

    switch (tolower(fieldName[0])) {
        case 'a':
            if ( fieldName % "anonymous" ) { anonymous = toBool(fieldValue); return true; }
            break;
        case 'c':
            if ( fieldName % "constant" ) { constant = toBool(fieldValue); return true; }
            break;
        case 'e':
            if ( fieldName % "encoding" ) { encoding = fieldValue; return true; }
            break;
        case 'i':
            if ( fieldName % "inputs" ) return true;
            break;
        case 'n':
            if ( fieldName % "name" ) { name = fieldValue; return true; }
            break;
        case 'o':
            if ( fieldName % "outputs" ) return true;
            break;
        case 'p':
            if ( fieldName % "payable" ) { payable = toBool(fieldValue); return true; }
            break;
        case 's':
            if ( fieldName % "signature" ) { signature = fieldValue; return true; }
            break;
        case 't':
            if ( fieldName % "type" ) { type = fieldValue; return true; }
            break;
        default:
            break;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void CFunction::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool CFunction::Serialize(SFArchive& archive) {
    if (!archive.isReading())
        return ((const CFunction*)this)->SerializeC(archive);

    if (!preSerialize(archive))
        return false;

    archive >> name;
    archive >> type;
    archive >> anonymous;
    archive >> constant;
    archive >> payable;
    archive >> signature;
    archive >> encoding;
    archive >> inputs;
    archive >> outputs;
    finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool CFunction::SerializeC(SFArchive& archive) const {
    if (!preSerializeC(archive))
        return false;

    archive << name;
    archive << type;
    archive << anonymous;
    archive << constant;
    archive << payable;
    archive << signature;
    archive << encoding;
    archive << inputs;
    archive << outputs;

    return true;
}

//---------------------------------------------------------------------------
void CFunction::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    uint32_t fieldNum = 1000;
    ADD_FIELD(CFunction, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
    ADD_FIELD(CFunction, "deleted", T_BOOL|TS_LABEL,  ++fieldNum);
    ADD_FIELD(CFunction, "name", T_TEXT, ++fieldNum);
    ADD_FIELD(CFunction, "type", T_TEXT, ++fieldNum);
    ADD_FIELD(CFunction, "anonymous", T_BOOL, ++fieldNum);
    ADD_FIELD(CFunction, "constant", T_BOOL, ++fieldNum);
    ADD_FIELD(CFunction, "payable", T_BOOL, ++fieldNum);
    ADD_FIELD(CFunction, "signature", T_TEXT, ++fieldNum);
    ADD_FIELD(CFunction, "encoding", T_TEXT, ++fieldNum);
    ADD_FIELD(CFunction, "inputs", T_TEXT|TS_ARRAY, ++fieldNum);
    ADD_FIELD(CFunction, "outputs", T_TEXT|TS_ARRAY, ++fieldNum);

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD(CFunction, "schema");
    HIDE_FIELD(CFunction, "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextFunctionChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CFunction *fun = (const CFunction *)data;
    if (fun) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, force, fun);
                break;

            default:
                break;
        }
    }

    return EMPTY;
}

//---------------------------------------------------------------------------
bool CFunction::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool CFunction::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

#pragma once
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
#include "utillib.h"
#include "parameter.h"

//--------------------------------------------------------------------------
class CFunction;
typedef SFArrayBase<CFunction>         CFunctionArray;
typedef SFList<CFunction*>             CFunctionList;
typedef SFUniqueList<CFunction*>       CFunctionListU;

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
class CFunction : public CBaseNode {
public:
    SFString name;
    SFString type;
    bool anonymous;
    bool constant;
    bool payable;
    SFString signature;
    SFString encoding;
    CParameterArray inputs;
    CParameterArray outputs;

public:
    CFunction(void);
    CFunction(const CFunction& fu);
    ~CFunction(void);
    CFunction& operator=(const CFunction& fu);

    DECLARE_NODE(CFunction);

    // EXISTING_CODE
    // EXISTING_CODE

protected:
    void Clear(void);
    void Init(void);
    void Copy(const CFunction& fu);
    bool readBackLevel(SFArchive& archive);

    // EXISTING_CODE
    // EXISTING_CODE
};

//--------------------------------------------------------------------------
inline CFunction::CFunction(void) {
    Init();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline CFunction::CFunction(const CFunction& fu) {
    // EXISTING_CODE
    // EXISTING_CODE
    Copy(fu);
}

// EXISTING_CODE
// EXISTING_CODE

//--------------------------------------------------------------------------
inline CFunction::~CFunction(void) {
    Clear();
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::Clear(void) {
    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::Init(void) {
    CBaseNode::Init();

//    name = EMPTY;
//    type = EMPTY;
    anonymous = 0;
    constant = 0;
    payable = 0;
//    signature = EMPTY;
//    encoding = EMPTY;
//    inputs = ??; /* unknown type: CParameterArray */
//    outputs = ??; /* unknown type: CParameterArray */

    // EXISTING_CODE
    // EXISTING_CODE
}

//--------------------------------------------------------------------------
inline void CFunction::Copy(const CFunction& fu) {
    Clear();
    CBaseNode::Copy(fu);

    name = fu.name;
    type = fu.type;
    anonymous = fu.anonymous;
    constant = fu.constant;
    payable = fu.payable;
    signature = fu.signature;
    encoding = fu.encoding;
    inputs = fu.inputs;
    outputs = fu.outputs;

    // EXISTING_CODE
    // EXISTING_CODE
    finishParse();
}

//--------------------------------------------------------------------------
inline CFunction& CFunction::operator=(const CFunction& fu) {
    Copy(fu);
    // EXISTING_CODE
    // EXISTING_CODE
    return *this;
}

//---------------------------------------------------------------------------
inline SFString CFunction::getValueByName(const SFString& fieldName) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return Format("[{"+toUpper(fieldName)+"}]");
}

//---------------------------------------------------------------------------
IMPLEMENT_ARCHIVE_ARRAY(CFunctionArray);
IMPLEMENT_ARCHIVE_ARRAY_C(CFunctionArray);
IMPLEMENT_ARCHIVE_LIST(CFunctionList);

//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE

