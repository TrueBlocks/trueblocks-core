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
#include "[{LONG}].h"
[OTHER_INCS]
[{NAMESPACE1}]
//---------------------------------------------------------------------------
IMPLEMENT_NODE([{CLASS_NAME}], [{BASE_CLASS}]);

//---------------------------------------------------------------------------
[{SCOPE}] SFString next[{PROPER}]Chunk(const SFString& fieldIn, const void *dataPtr);
static SFString next[{PROPER}]Chunk_custom(const SFString& fieldIn, const void *dataPtr);

//---------------------------------------------------------------------------
void [{CLASS_NAME}]::Format(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
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
        ctx << getNextChunk(fmt, next[{PROPER}]Chunk, this);
}

//---------------------------------------------------------------------------
SFString next[{PROPER}]Chunk(const SFString& fieldIn, const void *dataPtr) {
    if (dataPtr)
        return ((const [{CLASS_NAME}] *)dataPtr)->getValueByName(fieldIn);

    // EXISTING_CODE
    // EXISTING_CODE

    return fldNotFound(fieldIn);
}

//---------------------------------------------------------------------------------------------------
bool [{CLASS_NAME}]::setValueByName(const SFString& fieldName, const SFString& fieldValue) {
    // EXISTING_CODE
    // EXISTING_CODE

[{PARENT_SET}]
    switch (tolower(fieldName[0])) {
[FIELD_SETCASE]    }
    return false;
}

//---------------------------------------------------------------------------------------------------
void [{CLASS_NAME}]::finishParse() {
    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------------------------------
bool [{CLASS_NAME}]::Serialize(SFArchive& archive) {

    if (archive.isWriting())
        return ((const [{CLASS_NAME}]*)this)->SerializeC(archive);

    // If we're reading a back level, read the whole thing and we're done.
    if (readBackLevel(archive))
        return true;

    // EXISTING_CODE
    // EXISTING_CODE
[ARCHIVE_READ]	finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool [{CLASS_NAME}]::SerializeC(SFArchive& archive) const {

    // EXISTING_CODE
    // EXISTING_CODE

    // Writing always write the latest version of the data
[{PARENT_SER2}][ARCHIVE_WRITE]
    return true;
}

//---------------------------------------------------------------------------
void [{CLASS_NAME}]::registerClass(void) {
    static bool been_here = false;
    if (been_here) return;
    been_here = true;

    [{PARENT_REG}]uint32_t fieldNum = 1000;
    ADD_FIELD([{CLASS_NAME}], "schema",  T_NUMBER, ++fieldNum);
    ADD_FIELD([{CLASS_NAME}], "deleted", T_BOOL,  ++fieldNum);
    ADD_FIELD([{CLASS_NAME}], "showing", T_BOOL,  ++fieldNum);
[REGISTER_FIELDS]

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD([{CLASS_NAME}], "schema");
    HIDE_FIELD([{CLASS_NAME}], "deleted");
    HIDE_FIELD([{CLASS_NAME}], "showing");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString next[{PROPER}]Chunk_custom(const SFString& fieldIn, const void *dataPtr) {
    const [{CLASS_NAME}] *[{SHORT3}] = (const [{CLASS_NAME}] *)dataPtr;
    if ([{SHORT3}]) {
        switch (tolower(fieldIn[0])) {
            // EXISTING_CODE
            // EXISTING_CODE
            case 'p':
                // Display only the fields of this node, not it's parent type
                if ( fieldIn % "parsed" )
                    return nextBasenodeChunk(fieldIn, [{SHORT3}]);
                break;

            default:
                break;
        }
    }

    return "";
}

//---------------------------------------------------------------------------
bool [{CLASS_NAME}]::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *dataPtr) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool [{CLASS_NAME}]::readBackLevel(SFArchive& archive) {

    [{BASE_CLASS}]::[{PAR_READ_HEAD}](archive);
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

[{OPERATORS}]
//---------------------------------------------------------------------------
SFString [{CLASS_NAME}]::getValueByName(const SFString& fieldName) const {

    // Give customized code a chance to override first
    SFString ret = next[{PROPER}]Chunk_custom(fieldName, this);
    if (!ret.empty())
        return ret;

    [{FIELD_CASE}]

    // EXISTING_CODE
    // EXISTING_CODE

[{SUBCLASSFLDS}]    // Finally, give the parent class a chance
    return [{BASE_CLASS}]::getValueByName(fieldName);
}

//-------------------------------------------------------------------------
ostream& operator<<(ostream& os, const [{CLASS_NAME}]& item) {
    // EXISTING_CODE
    // EXISTING_CODE

    os << item.Format() << "\n";
    return os;
}

[{GET_OBJ}][{GET_STR}]//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
[{NAMESPACE2}]
