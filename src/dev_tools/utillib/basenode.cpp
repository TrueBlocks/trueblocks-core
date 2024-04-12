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
#include "basetypes.h"
#include "basenode.h"
#include "sfarchive.h"
#include "conversions.h"
#include "runtimeclass.h"

namespace qblocks {

//--------------------------------------------------------------------------------
CRuntimeClass CBaseNode::classCBaseNode;

//--------------------------------------------------------------------------------
CBaseNode::CBaseNode(void) {
    initialize();
}

//--------------------------------------------------------------------------------
CBaseNode::~CBaseNode(void) {
}

//--------------------------------------------------------------------------------
void CBaseNode::initialize(void) {
    m_deleted = false;
    m_schema = 20000 + 500 + 8;
    m_showing = true;
}

//--------------------------------------------------------------------------------
void CBaseNode::duplicate(const CBaseNode& bn) {
    m_deleted = bn.m_deleted;
    m_schema = bn.m_schema;
    m_showing = bn.m_showing;
}

//--------------------------------------------------------------------------------
bool CBaseNode::isDeleted(void) const {
    return (m_deleted);
}

//--------------------------------------------------------------------------------
void CBaseNode::setDeleted(bool del) {
    m_deleted = del;
}

//--------------------------------------------------------------------------------
CRuntimeClass* CBaseNode::getRuntimeClass() const {
    return &CBaseNode::classCBaseNode;
}

}  // namespace qblocks
