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

namespace qblocks {

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
    if ( fieldName % "inputs" ) {
        parseParams(true, fieldValue);
        return true;

    } else if ( fieldName % "outputs" ) {
        parseParams(false, fieldValue);
        return true;

    } else if ( fieldName % "signature" ) {
        signature = getSignature(SIG_CANONICAL);
        return true;
    }
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
    for (uint32_t i = 0 ; i < inputs.getCount() ; i++)
        hasAddrs |= (inputs[i].type == "address");
    signature = getSignature(SIG_CANONICAL);
    encoding  = encodeItem();
    // The input parameters need to have a name. If not, we provide one
    int cnt = 0;
    for (uint32_t i = 0 ; i < inputs.getCount() ; i++) {
        if (inputs[i].name.empty())
            inputs[i].name = "param_" + asString(cnt++);
    }
    // TODO(tjayrush): Do we need names for output params?
    //cnt = 0;
    //for (int i = 0 ; i < outputs.getCount() ; i++) {
    //    if (outputs[i].name.empty()) {
    //        SFString type = outputs[i].type;
    //        type.ReplaceAny("0123456789","");
    //        outputs[i].name = type + "_" + asString(cnt++);
    //    }
    //}
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
    HIDE_FIELD(CFunction, "indexed");
    HIDE_FIELD(CFunction, "anonymous");
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString nextFunctionChunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const CFunction *fun = (const CFunction *)data;
    if (fun) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            case 'h':
                if ( fieldIn % "hex" ) {
                    SFString ret = fun->name + "(";
                    for (uint32_t i = 0 ; i < fun->inputs.getCount() ; i++) {
                        ret += fun->inputs[i].type;
                        if (i < fun->inputs.getCount())
                            ret += ",";
                    }
                    ret += ")";
                    return ret + "\t0x" + string2Hex(ret).Substitute(",)", ")");

                } else if ( fieldIn % "hasAddrs" ) {
                    return asString(fun->hasAddrs);

                }
                break;
            case 'i':
                if ( fieldIn % "isBuiltin" ) {
                    return asString(fun->isBuiltin);
                }
                break;
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

    return "";
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
void CFunction::parseParams(bool input, const SFString& contents) {
    char *p = (char *)contents.c_str();
    while (p && *p) {
        CParameter param;
        uint32_t nFields = 0;
        p = param.parseJson(p, nFields);
        if (nFields) {
            if (input)
                inputs[inputs.getCount()] = param;
            else
                outputs[outputs.getCount()] = param;
        }
    }
}

//---------------------------------------------------------------------------
SFString CFunction::getSignature(SFUint32 parts) const {
    uint32_t cnt = inputs.getCount();

    CStringExportContext ctx;
    ctx << (parts & SIG_FTYPE  ? "\t"+type+" " : "");
    ctx << (parts & SIG_FNAME  ? name          : "");
    ctx << (parts & SIG_FSPACE ? SFString(' ', max(0UL, 35-type.length()-name.length())) : "");
    ctx << (parts & SIG_FTYPE || parts & SIG_FNAME  ? "("    : "");
    for (uint32_t j = 0 ; j < cnt ; j++) {
        ctx << (parts & SIG_ITYPE    ? inputs[j].type : "");
        ctx << (parts & SIG_IINDEXED ? (inputs[j].indexed ? " indexed" : "") : "");
        ctx << (parts & SIG_INAME    ? " "+inputs[j].name : "");
        ctx << (parts & SIG_ITYPE    ? (j < cnt-1 ? "," : "") : "");
    }
    ctx << (parts & SIG_FTYPE || parts & SIG_FNAME  ? ")" : "");
    if (parts == SIG_ENCODE)
        ctx << (parts & SIG_ENCODE ? (parts & SIG_FNAME ? " " + encoding : encoding + " ") : "");
    else
        ctx << (parts & SIG_ENCODE ? " [" + encoding + "]" : "");
    if (verbose && parts != SIG_CANONICAL) {
        ctx << (anonymous ? " anonymous" : "");
        ctx << (constant  ? " constant" : "");
        ctx << (payable   ? " payable" : "");
    }

    ctx.str.ReplaceAll(" )", ")");
    return Strip(ctx.str, ' ');
}

//-----------------------------------------------------------------------
SFString CFunction::encodeItem(void) const {
    SFString hex = "0x"+string2Hex(signature);
    SFString ret;
extern bool getSha3(const SFString& hexIn, SFString& shaOut);
    getSha3(hex, ret);
    ret = (type == "event" ? ret : ret.Left(10));
    return ret;
}
// EXISTING_CODE
}  // namespace qblocks

