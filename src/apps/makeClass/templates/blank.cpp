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
IMPLEMENT_NODE([{CLASS_NAME}], [{BASE_CLASS}], dataSchema());

//---------------------------------------------------------------------------
[{SCOPE}] SFString next[{PROPER}]Chunk(const SFString& fieldIn, bool& force, const void *data);
static SFString next[{PROPER}]Chunk_custom(const SFString& fieldIn, bool& force, const void *data);

//---------------------------------------------------------------------------
void [{CLASS_NAME}]::Format(CExportContext& ctx, const SFString& fmtIn, void *data) const {
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
        ctx << getNextChunk(fmt, next[{PROPER}]Chunk, this);
}

//---------------------------------------------------------------------------
SFString next[{PROPER}]Chunk(const SFString& fieldIn, bool& force, const void *data) {
    const [{CLASS_NAME}] *[{SHORT3}] = (const [{CLASS_NAME}] *)data;
    if ([{SHORT3}]) {
        // Give customized code a chance to override first
        SFString ret = next[{PROPER}]Chunk_custom(fieldIn, force, data);
        if (!ret.empty())
            return ret;

        switch (tolower(fieldIn[0])) {
[FIELD_CASE]		}

        // EXISTING_CODE
        // EXISTING_CODE

        // Finally, give the parent class a chance
        [{PARENT_CHNK}]
        if (!ret.empty())
            return ret;
    }
[{SUBCLASSFLDS}]
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

[{PARENT_SER}][ARCHIVE_READ]	finishParse();
    return true;
}

//---------------------------------------------------------------------------------------------------
bool [{CLASS_NAME}]::SerializeC(SFArchive& archive) const {
[{PARENT_SER1}][ARCHIVE_WRITE]
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
[REGISTER_FIELDS]

    // Hide our internal fields, user can turn them on if they like
    HIDE_FIELD([{CLASS_NAME}], "schema");
    HIDE_FIELD([{CLASS_NAME}], "deleted");

    // EXISTING_CODE
    // EXISTING_CODE
}

//---------------------------------------------------------------------------
SFString next[{PROPER}]Chunk_custom(const SFString& fieldIn, bool& force, const void *data) {
    const [{CLASS_NAME}] *[{SHORT3}] = (const [{CLASS_NAME}] *)data;
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
bool [{CLASS_NAME}]::handleCustomFormat(CExportContext& ctx, const SFString& fmtIn, void *data) const {
    // EXISTING_CODE
    // EXISTING_CODE
    return false;
}

//---------------------------------------------------------------------------
bool [{CLASS_NAME}]::readBackLevel(SFArchive& archive) {
    bool done = false;
    // EXISTING_CODE
    // EXISTING_CODE
    return done;
}

[{OPERATORS}]//---------------------------------------------------------------------------
// EXISTING_CODE
// EXISTING_CODE
[{NAMESPACE2}]
