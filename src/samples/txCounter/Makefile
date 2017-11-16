#---------------------------------------------------------------------------
# NOTE: This makefile assumes that quickBlocks is installed at default path:
# /usr/local/qblocks
# Update the variable QBLOCKS_INSTALL_PATH otherwise
#---------------------------------------------------------------------------


#---------------------------------------------------------------------------
# Quickblocks stuff
#---------------------------------------------------------------------------
QBLOCKS_INSTALL_PATH=/usr/local/qblocks
QBLOCKS_INCLUDE_PATH=$(QBLOCKS_INSTALL_PATH)/include
QBLOCKS_LIB_PATH=$(QBLOCKS_INSTALL_PATH)/lib

QBLOCKS_STATIC_LIBS=-lwallet -ltoken -lacct -lether -lutil 
QBLOCKS_LDFLAGS=-L$(QBLOCKS_LIB_PATH) $(QBLOCKS_STATIC_LIBS)


#---------------------------------------------------------------------------
# Compiler/linker flags
#---------------------------------------------------------------------------
CXX=g++
CFLAGS=-std=c++11 -Werror -Wall -O2 -I. -I$(QBLOCKS_INCLUDE_PATH)
LDFLAGS=$(QBLOCKS_LDFLAGS)  -lcurl

#---------------------------------------------------------------------------
# Program sources and name
#---------------------------------------------------------------------------
SRCS=txCounter.cpp
OBJS_PATH=objs
OBJS=$(patsubst %.cpp,$(OBJS_PATH)/%.o,$(SRCS))

EXE_NAME=txCounter
EXE_PATH=bin


#---------------------------------------------------------------------------
# Rules
#---------------------------------------------------------------------------

# Compilation
$(OBJS_PATH)/%.o: %.cpp 
	@mkdir -p $(OBJS_PATH)
	$(CXX) $(CFLAGS) -c $< -o $@

# Link
all: $(EXE_NAME)

$(EXE_NAME): $(OBJS)
	$(CXX) -o $(EXE_NAME) $(OBJS) $(LDFLAGS)
	@mkdir -p $(EXE_PATH)
	mv $(EXE_NAME) $(EXE_PATH)	


# Clean
clean: 
	$(RM) -rf $(OBJS_PATH) $(EXE_PATH)
