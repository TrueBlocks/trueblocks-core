#---------------------------------------------------------------------------
# NOTE: This makefile assumes that quickBlocks is installed at default path:
# /usr/local/qblocks
# Update the variable QBLOCKS_INSTALL_PATH otherwise
#---------------------------------------------------------------------------

#---------------------------------------------------------------------------
# Quickblocks stuff
#---------------------------------------------------------------------------
QBLOCKS_INSTALL_PATH = /usr/local/qblocks
QBLOCKS_INCLUDE_PATH = $(QBLOCKS_INSTALL_PATH)/include
QBLOCKS_LIB_PATH     = $(QBLOCKS_INSTALL_PATH)/lib

QBLOCKS_STATIC_LIBS  = -lwallet -ltoken -lacct -lether -lutil
QBLOCKS_LDFLAGS      = -L$(QBLOCKS_LIB_PATH) $(QBLOCKS_STATIC_LIBS)

#---------------------------------------------------------------------------
# Compiler/linker flags
#---------------------------------------------------------------------------
CXX=g++
CFLAGS=-std=c++11 -Werror -Wall -O2 -I. -I$(QBLOCKS_INCLUDE_PATH) -I/usr/local/mysql/include/
LDFLAGS=$(QBLOCKS_LDFLAGS) -lcurl -L/usr/local/mysql/lib/ -lmysqlclient

#---------------------------------------------------------------------------
# Program sources and name
#---------------------------------------------------------------------------
SRCS=mysql_db.cpp
OBJS_PATH=objs
OBJS=$(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRCS))

EXE_NAME=mysql_db
EXE_PATH=bin

#---------------------------------------------------------------------------
# Rules
#---------------------------------------------------------------------------
all: $(EXE_NAME)

$(OBJS_PATH)/%.o: %.cpp 
	@mkdir -p $(OBJS_PATH)
	$(CXX) $(CFLAGS) -c $< -o $@

$(EXE_NAME): $(OBJS)
	$(CXX) -o $(EXE_NAME) $(OBJS) $(LDFLAGS)
	@mkdir -p $(EXE_PATH)
	mv $(EXE_NAME) $(EXE_PATH)

clean:
	$(RM) -rf $(OBJS_PATH) $(EXE_PATH)
