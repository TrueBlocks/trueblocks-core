/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "acctlib.h"

//-------------------------------------------------------------------------
void acctlib_init(QUITHANDLER qh) {
    etherlib_init(qh);

    CAccountWatch::registerClass();
    CApiSpec::registerClass();
    CEthState::registerClass();
    CIncomeStatement::registerClass();

    CTreeRoot::registerClass();
    CTreeNode::registerClass();
    CInfix::registerClass();
    CBranch::registerClass();
    CLeaf::registerClass();
    CBalanceRecord::registerClass();
}

//-------------------------------------------------------------------------
void acctlib_cleanup(void) {
    etherlib_cleanup();
}
