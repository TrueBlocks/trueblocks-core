#!/usr/bin/python

import sqlite3
conn = sqlite3.connect('theDatabase.db')
print "  id\tblockNumber\ttx_index\ttrace_id\taddress\treason";
print "  ---------------------------------------------";
cursor = conn.execute("SELECT * from addrsPerBlock")
for row in cursor:
   print "  ", row[0], "\t", row[1], "\t", row[2], "\t", row[3], "\t", row[4], "\t", row[5], "\n",
conn.close()
