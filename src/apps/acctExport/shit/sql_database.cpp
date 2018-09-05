/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#if 1
//def BUILD_SQL
#include <mysql.h>
#include "sql_database.h"

namespace qblocks {

    typedef bool (*SQLROWFUNC)(MYSQL_ROW& row, void *data);

    //--------------------------------------------------------------------------
    CMySqlServer::CMySqlServer(void) {
        connection = NULL;
    }

    //--------------------------------------------------------------------------
    CMySqlServer::CMySqlServer(const string_q& h, const string_q& u, const string_q& p) : host(h), user(u), pw(p) {
        openConnection();
    }

    //--------------------------------------------------------------------------
    void CMySqlServer::setConnection(const string_q& h, const string_q& u, const string_q& p, bool open) {
        closeConnection();
        host = h;
        user = u;
        pw = p;
        if (open)
            openConnection();
    }

    //--------------------------------------------------------------------------
    void CMySqlServer::openConnection(void) {
        if (host.empty() || user.empty()) {
            cerr << cRed << "Either host or user is empty. Both must contain a value.\n";
            return;
        }

        closeConnection(); // just in case
        connection = mysql_init(NULL);
        if (!connection) {
            cerr << cRed << "Could not create connection: " << mysql_error(connection) << cOff << "\n";

        } else if (!mysql_real_connect(connection, host.c_str(), user.c_str(), pw.c_str(), NULL, 0, NULL, 0)) {
            cerr << cRed << "Error trying to connect: " << mysql_error(connection) << cOff << "\n";
            mysql_close(connection);
            connection = NULL;

        } else {
            if (verbose)
                cerr << "Connected to SQL at " << forDisplay() << "\n";
        }
    }

    //--------------------------------------------------------------------------
    CMySqlServer::~CMySqlServer(void) {
        closeConnection();
    }

    //--------------------------------------------------------------------------
    void CMySqlServer::closeConnection(void) {
        if (connection) {
            mysql_close(connection);
            if (verbose)
                cerr << "Disconnnected from " << forDisplay() << "\n";
        }
        connection = NULL;
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::runQuery(const string_q& query) {
        if (!connection) {
            cerr << cRed << "You must first make a connection. Quitting...\n" << cOff;
            return false;
        }

        if (query.empty()) {
            cerr << cRed << "The query failed because it is empty.\n" << cOff;
            return false;
        }

        bool result = true;
        if (mysql_query(connection, query.c_str())) { // returns non-zero on error
            //cerr << cRed << "ERROR: Query [" << query << "] failed, " << mysql_error(connection) << "\n";
            cerr << string_q(80, '+') << "\n";
            cerr << "ERROR:\n";
            cerr << string_q(80, '-') << "\n";
            cerr << query << "\n\n";
            cerr << mysql_error(connection) << "\n";
            cerr << string_q(80, '+') << "\n\n";
            result = false;
            //cerr << "Hit enter"; cerr.flush();
            //getchar();
        } else {
            if (verbose && !contains(query, "SELECT '-------"))
                cerr << query  << " " << greenCheck << "\n";
        }
        return result;
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::useDatabase(const string_q& dbName) {
        string_q query = "CREATE DATABASE IF NOT EXISTS " + dbName + ";";
        if (runQuery(query)) {
            query = "USE " + dbName + ";";
            if (runQuery(query)) {
                if (verbose)
                    cerr << "Using database " << cTeal << dbName << cOff << "\n";
                return true;
            }
        }
        if (verbose)
            cerr << "Could not use useDatabase: " << dbName << ". Quitting...\n";
        return false;
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::dropDatabase(const string_q& dbName) {
        string_q query = "DROP DATABASE " + dbName + ";";
        return runQuery(query);
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::createTable(const string_q& table, const string_q& fieldList, const string_q& append) {
        string_q query = "CREATE TABLE IF NOT EXISTS " + table + "(" + fieldList + append + ");";
        return runQuery(query);
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::insertRows(const string_q& table, const string_q& fieldList, const string_q& valueList, const string_q& matchField) {
        if (valueList.empty())
            return true;
        string_q query = "INSERT INTO " + table + " (" + fieldList + ") VALUES " + valueList + " ON DUPLICATE KEY UPDATE " + matchField + "=" + matchField + ";";
        return runQuery(query);
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::describeTable(const string_q& table) {
        string_q query = "DESCRIBE " + table + ";";
        if (!runQuery(query))
            return false;
        return forEveryRowInResult("", printSQLRow, NULL);
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::forEveryRowInTable(const string_q& table, const string_q& fields, const string_q& filter, SQLROWFUNC rowFunc, void *data) {
        if (!rowFunc)
            return false;
        string_q query = "SELECT " + fields + " FROM " + table + ";";
        if (!runQuery(query))
            return false;
        return forEveryRowInResult(filter, rowFunc, data);
    }

    //--------------------------------------------------------------------------
    bool CMySqlServer::forEveryRowInResult(const string_q& filter, SQLROWFUNC rowFunc, void *data) {
        if (!rowFunc)
            return false;

        MYSQL_RES *result = mysql_store_result(connection);
        if (!result) {
            if (verbose)
                cerr << mysql_error(connection) << "\n";
            return false;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        while (row) {
            if (!rowFunc(row, data))
                return false;
            row = mysql_fetch_row(result);
        }
        mysql_free_result(result);
        return true;
    }

    //--------------------------------------------------------------------------
    string_q CMySqlServer::forDisplay(void) const {
        ostringstream os;
        os << cYellow << host << cOff << "(" << cTeal << user << cOff << (pw.empty()?"":", "+cTeal+pw) << cOff << ")";
        return os.str().c_str();
    }

    //--------------------------------------------------------------------------
    bool printSQLRow(MYSQL_ROW& row, void *data) {
        if (verbose && !string_q(*row).empty())
            cout << *row << "\n";
        return true;
    }

} // namespace qblocks
#endif

