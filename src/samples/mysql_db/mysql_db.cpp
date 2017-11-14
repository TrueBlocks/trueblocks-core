/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/

#include <iostream>
#include <string>

#include <mysql.h>

#include "etherlib.h"

//--------------------------------------------------------------------------

// mysql connection handler
MYSQL *conn = NULL;

// connection credentials, normally at config file
std::string host = "127.0.0.1";
std::string user = "root"; //root
std::string password = ""; //Sep2017$

//--------------------------------------------------------------------------
extern int runQuery(const char* query);
extern int connectServer(void);
extern void disconnectServer(void);
extern int initDb(void);
extern int createTable(void);
extern int addDbData(void);
extern int displayDbData(void);

//--------------------------------------------------------------------------
int main() {

    // Stablish connection with mysql server
    if(connectServer()) {
        return -1;
    }

    // Create db and tables
    if(initDb()) {
        disconnectServer();
        return -1;
    }

    // Insert some dummy data
    if(addDbData()) {
        disconnectServer();
        return -1;
    }

    // Display entries
    displayDbData();

    // Disconnect from server
    disconnectServer();

    return 0;
}

//--------------------------------------------------------------------------
int connectServer() {
    // connect to mysql server at socket level
    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), NULL, 0,
                            NULL, 0)) {
        cerr << "Unable to connect to mysql. Error: " << mysql_error(conn) << "\n";
        return 1;
    }

    cout << "Connected with mysql at " <<   host  << "\n";

    return 0;
}

//--------------------------------------------------------------------------
void disconnectServer() {
    // close socket
    mysql_close(conn);
    conn = NULL;

    cout << "Disconnnected from mysql\n";
}

//--------------------------------------------------------------------------
int createTable(const char* query) {
    if(runQuery(query)) {
        cerr << "ERROR: Unable to create table\n";
        return 1;
    }

    return 0;
}

//--------------------------------------------------------------------------
int initDb() {
    std::string query = "CREATE DATABASE IF NOT EXISTS quickBlocks;";
    // create database from scratch
    if(runQuery(query.c_str())) {
        cerr << "ERROR: Unable to create QBLOCKS database\n";
        return 1;
    }

    // use this database now onwards
    query = "USE quickBlocks;";

    if(runQuery(query.c_str())) {
        cerr << "ERROR: Unable to use QBLOCKS database\n";
        return 1;
    }

    // create the tables required

    // Blocks table
    query = "CREATE TABLE IF NOT EXISTS Blocks ( \
    blockNumber BIGINT, \
    timestamp TIMESTAMP, \
    PRIMARY KEY (blockNumber) \
    );";

    if(createTable(query.c_str())) {
        return 1;
    }

    // Transactions table
    query = "CREATE TABLE IF NOT EXISTS Transactions ( \
    blockNumber BIGINT, \
    indexInTheBlock INT, \
    fromAddr VARCHAR(256), \
    toAddr VARCHAR(256), \
    value VARCHAR(256), \
    input VARCHAR(256), \
    FOREIGN KEY (blockNumber) REFERENCES Blocks(blockNumber) \
    );";

    if(createTable(query.c_str())) {
        return 1;
    }

    return 0;
}

//--------------------------------------------------------------------------
int addDbData() {
    std::string query;
    // Simulate we insert N blocks with a given fake timestamp
    // conversion between C typs and mysql types is required always

    // Add N blocks
    int dummyBlocksNum = 100;

    for(int i=0; i < dummyBlocksNum; i++) {
        std::ostringstream stringStream;
        stringStream << "INSERT INTO Blocks VALUES(" << i << "," <<
        "STR_TO_DATE('26,10,2017','%d,%m,%Y'));";

        if(runQuery(stringStream.str().c_str())) {
            cerr << "ERROR: Unable to insert block " << i << "\n";
            return 1;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------
int displayDbData() {
    std::string query = "SELECT * from Blocks;";

    if(runQuery(query.c_str())) {
        cerr << "ERROR: Unable to display Blocks information\n";
        return 1;
    }

    // Once query is executed we retrieve the result
    MYSQL_RES *result = mysql_store_result(conn);

    if(result) {
        // Get now the rows information present in result
        // row will be an array with as many fields as select filter (we request '*' = 2 fields )
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)))  {
            cout << "BlockNum: " << row[0] << ", timestamp: " << row[1] << endl;
        }

        // deallocate dynamic result
        mysql_free_result(result);
    }

    return 0;
}

//--------------------------------------------------------------------------
int runQuery(const char* query) {
    int result = 1;

    if(query) {
        // run the query
        cout << "Running query [" <<  query  << "]\n";

        if (mysql_query(conn, query)) {
            cerr << "ERROR: Query [" << query << "] failed, " << mysql_error(conn) << "\n";
        } else {
            // Query executed - success
            result = 0;
        }

    } else {
        cerr << "ERROR: NULL query received\n";
    }

    return result;
}

