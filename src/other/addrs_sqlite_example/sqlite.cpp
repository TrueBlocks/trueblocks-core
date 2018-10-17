#include <sqlite3.h>
#include "etherlib.h"
#include "addressperblock.h"

//------------------------------------------------------------------------------
static int callback(void *data, int argc, char **argv, char **azColName) {

    static bool been_here = false;
    if (!been_here) {
        cout << "  ";
        for (int i = 0 ; i < argc ; i++)
            cout << cGreen << azColName[i] << cOff << "\t";
        cout << "\n";
        been_here = true;
    }
    cout << "  ";
    for (int i = 0 ; i < argc ; i++)
        cout << cTeal << (argv[i] ? argv[i] : "NULL") << cOff << "\t";
    cout << "\n";

    CAddressPerBlockArray *array = (CAddressPerBlockArray *)data;
    CAddressPerBlock addrPer;
    for (int i = 0 ; i < argc ; i++)
        addrPer.setValueByName(azColName[i], (argv[i] ? argv[i] : "NULL"));
    array->push_back(addrPer);

    return 0;
}

extern bool insertData(sqlite3 *db);
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    etherlib_init();
    CAddressPerBlock::registerClass();
    if (isTestMode())
        colorsOff();

    sqlite3 *db = NULL;
    if (sqlite3_open("theDatabase.db", &db)) {  // failure returns non-zero
        cerr << cGreen << "c++: " << cOff << "Can't open database: " << cRed << sqlite3_errmsg(db) << cOff << "\n";
        return 0;
    } else {
        cerr << cGreen << "c++: " << cOff << "Opened database sucessfully\n";
    }

    char *zErrMsg = NULL;
#if 0
    if (sqlite3_exec(db, tableCreate_sql.c_str(), callback, 0, &zErrMsg) != SQLITE_OK) {
        cerr << cGreen << "c++: " << cOff << "SQL error message: " << cRed << zErrMsg << cOff << "\n";
        sqlite3_free(zErrMsg);
    } else {
        cerr << cGreen << "c++: " << cOff << "Table created sucessfully\n";
    }

    if (!insertData(db))
        return 0;
#endif

    CAddressPerBlockArray array;
    string_q sql = "SELECT * from addrsPerBlock";
    if (argc > 1)
        sql += string_q(" WHERE address = '") + argv[1] + string_q("'");
    if( sqlite3_exec(db, sql.c_str(), callback, &array, &zErrMsg) != SQLITE_OK ) {
        cerr << cGreen << "c++: " << cOff << "SQL error message: " << cRed << zErrMsg << cOff << "\n";
        sqlite3_free(zErrMsg);
    } else {
        cerr << cGreen << "c++: " << cOff << "Select command successful\n";
    }

    string_q fmt = "  [{id}]\t[{block_number}]\t[{tx_index}]\t[{trace_id}]\t[{address}]\t[{reason}]\n";
    CAddressPerBlock header;
    cout << cGreen << header.Format(substitute(fmt, "{", "{p:")) << cOff;
    for (auto item : array)
        cout << cTeal << item.Format(fmt) << cOff;
    cerr << "written using qblocks\n";

#if 0
    sql = "UPDATE addrsPerBlock set block_number = 5001001 where id = 1; " \
    "SELECT * from addrsPerBlock";
    if( sqlite3_exec(db, sql.c_str(), callback, &array, &zErrMsg) != SQLITE_OK ) {
        cerr << cGreen << "c++: " << cOff << "SQL error message: " << cRed << zErrMsg << cOff << "\n";
        sqlite3_free(zErrMsg);
    } else {
        cerr << cGreen << "c++: " << cOff << "Select command successful\n";
    }
#endif

    if (db)
        sqlite3_close(db);

    return 0;
}

extern string_q insertFmt;
//------------------------------------------------------------------------------
bool insertData(sqlite3 *db) {

    CArchive datafile(READING_ARCHIVE);
    if (!datafile.Lock("./6200000-6250000.txt", asciiReadOnly, LOCK_NOWAIT)) {
        cerr << "Could not read file. Quitting...\n";
        return false;
    }

    while (!datafile.Eof()) {
        char buffer[20001];
        buffer[0] = '\0';
        if (datafile.ReadLine(buffer, 20000)) {
            string_q line = substitute(buffer,"\t\t","\tNULL\t");
            CAddressPerBlock item;
            string_q fields[] = { "block_number", "tx_index", "trace_id", "address", "reason" };
            size_t nFields = sizeof(fields) / sizeof(string_q);
            char *s = (char *)line.c_str();
            item.parseText(s, nFields, fields);

            string_q ins = item.Format(insertFmt);
            char *zErrMsg = NULL;
            if (sqlite3_exec(db, ins.c_str(), callback, 0, &zErrMsg) != SQLITE_OK ) {
                cerr << cGreen << "c++: " << cOff << "SQL error message: " << cRed << zErrMsg << cOff << "\n";
                sqlite3_free(zErrMsg);
                return false;
            } else {
                cerr << cGreen << "c++: " << cOff << ins.substr(85,80) << cOff << "\n";
            }
        }
    }
    return true;
}

//------------------------------------------------------------------------------
string_q insertFmt =
"INSERT INTO addrsPerBlock (block_number, tx_index, trace_id, address, reason) "  \
"VALUES ([{BLOCK_NUMBER}], [{TX_INDEX}], [{TRACE_ID}], '[{ADDRESS}]', '[{REASON}]'); ";

//------------------------------------------------------------------------------
string_q tableCreate_sql =
"CREATE TABLE IF NOT EXISTS addrsPerBlock("    \
    "id             INTEGER PRIMARY KEY AUTOINCREMENT," \
    "block_number   INT NOT NULL," \
    "tx_index       INT,"          \
    "trace_id       INT,"          \
    "address        CHAR(50),"     \
    "reason         TEXT );";
