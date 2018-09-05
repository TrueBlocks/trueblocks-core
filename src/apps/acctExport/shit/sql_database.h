#pragma once
/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

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

