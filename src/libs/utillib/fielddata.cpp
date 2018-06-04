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
