#-------------------------------------------------
libraries=/Users/jrush/Development/trueblocks-core/src/libs/
bin=bin
cflags=-std=c++17 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libcurl.4.tbd

#-------------------------------------------------
exec=tokenTool
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
options.cpp \
stake.cpp \
$(exec).cpp

#-------------------------------------------------
all:
#	@cd ../../../build ; make ; cd -
	@make app

app:
	@make $(product)
	mv $(product) $(dest)

test:
	@make
	@echo "No tests"

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

$(product): $(objects) $(libs)
	g++ -o $(product) $(objects) $(libs)
	@strip $(product)

$(objects): | objs

objs:
	@mkdir -p $@

objs/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(cflags) -c $< -o $@

cleanall:
	@cd $(libraries) ; make clean ; cd -
	@make clean

clean:
	-@$(RM) -f $(dest) $(product) objs/* 2> /dev/null
