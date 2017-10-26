#-----------------------------------------
# Compiler to be used
#-----------------------------------------
CC=g++

#-----------------------------------------
# Compiler flags
#-----------------------------------------
CFLAGS = -g -Wall -W -Werror -Wno-unused-function -std=c++11 -I.  -I/usr/include/mysql
QBLOCKS_LIBS = -L/usr/local/qblocks/lib -lether -lacct -lwallet -ltoken -lutil
CURL_LIBS = -lcurl
MYSQL_LIBS = -lmysqlclient 
LDFLAGS = $(MYSQL_LIBS) $(QBLOCKS_LIBS) $(CURL_LIBS)

#-----------------------------------------
# Program name
#-----------------------------------------
SAMPLE_EXE=mysql_db

#-----------------------------------------
# Makefile rules
#-----------------------------------------

#-----------------------------------------
# Default rule (when typed only "make")
#-----------------------------------------
default: all

all: $(SAMPLE_EXE)

#-----------------------------------------
# Compilation rule for each .cpp file
#-----------------------------------------
mysql_db.o: mysql_db.cpp
	$(CC) -c $(CFLAGS) mysql_db.cpp -o mysql_db.o


#-----------------------------------------
$(SAMPLE_EXE): mysql_db.o
	$(CC) -o $(SAMPLE_EXE) mysql_db.o  $(LDFLAGS)
#-----------------------------------------
clean:
	rm -f $(SAMPLE_EXE) mysql_db.o
