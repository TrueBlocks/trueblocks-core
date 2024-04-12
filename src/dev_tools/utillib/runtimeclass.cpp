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
#include <algorithm>
#include "basetypes.h"
#include "runtimeclass.h"

namespace qblocks {

//-------------------------------------------------------------------------
CRuntimeClass::CRuntimeClass(void) {
}

//-------------------------------------------------------------------------
CRuntimeClass::~CRuntimeClass(void) {
}

//-------------------------------------------------------------------------
char* CRuntimeClass::getClassNamePtr(void) const {
    return m_ClassName;
}

//-------------------------------------------------------------------------
CBaseNode* CRuntimeClass::createObject(void) {
    if (m_CreateFunc)
        return (*m_CreateFunc)();
    return NULL;
}

//-------------------------------------------------------------------------
void CRuntimeClass::sortFieldList(void) {
    sort(fieldList.begin(), fieldList.end());
    return;
}

//-------------------------------------------------------------------------
bool CRuntimeClass::forEveryField(FIELDVISITFUNC func, void* data) {
    if (!func)
        return true;

    for (auto field : fieldList) {
        if (!(*func)(field, data))
            return false;
    }

    return true;
}

}  // namespace qblocks
