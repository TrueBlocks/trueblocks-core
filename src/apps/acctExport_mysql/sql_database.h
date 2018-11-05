#pragma once
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
#include <mysql.h>
#include "etherlib.h"

namespace qblocks {
    typedef bool (*SQLROWFUNC)(MYSQL_ROW& row, void *data);
    class CMySqlServer {
    public:
        string_q  host;
        string_q  user;
        string_q  pw;
        MYSQL    *connection;

             CMySqlServer       (void);
             CMySqlServer       (const string_q& h, const string_q& u, const string_q& p);
            ~CMySqlServer       (void);

        void setConnection      (const string_q& h, const string_q& u, const string_q& p, bool open=false);
        void openConnection     (void);
        void closeConnection    (void);
        bool runQuery           (const string_q& query);
        bool useDatabase        (const string_q& dbName);
        bool dropDatabase       (const string_q& dbName);
        bool createTable        (const string_q& table, const string_q& fieldList, const string_q& append);
        bool insertRows         (const string_q& table, const string_q& fieldList, const string_q& valueList, const string_q& matchField="blockNumber");
        bool describeTable      (const string_q& table);
        bool comment            (const string_q& msg);

        bool forEveryRowInTable (const string_q& table, const string_q& fields, const string_q& filter, SQLROWFUNC rowFunc, void *data);
        bool forEveryRowInResult(const string_q& filter, SQLROWFUNC rowFunc, void *data);

        string_q forDisplay    (void) const;
    };

    extern bool printSQLRow(MYSQL_ROW& row, void *data);

} // namespace qblocks

