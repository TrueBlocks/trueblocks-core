/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "basetypes.h"
#include "fielddata.h"

namespace qblocks {

    //-------------------------------------------------------------------------
    const CFieldData *CFieldList::getFieldByID(uint32_t id) const {
        LISTPOS lPos = GetHeadPosition();
        while (lPos) {
            CFieldData *field = GetNext(lPos);
            if (field->m_fieldID == id)
                return field;
        }

        ASSERT(0);  // should never happen
        return NULL;
    }

    //-------------------------------------------------------------------------
    const CFieldData *CFieldList::getFieldByName(const SFString& fieldString) const {

        // the input may contain more than just fieldName
        SFString fieldName = fieldString; fieldName = nextTokenClear(fieldName, '|');

        const CFieldData *field = NULL;
        LISTPOS lPos = GetHeadPosition();
        while (lPos) {
            CFieldData *current = GetNext(lPos);
            if (current->m_fieldName % fieldName) {
                field = current;
                continue;
            }
        }

        if (!field) {
            static CFieldData non_field;
            non_field.m_fieldName = "unknown";
            field = &non_field;
        }

        return field;
    }
}  // namespace qblocks
