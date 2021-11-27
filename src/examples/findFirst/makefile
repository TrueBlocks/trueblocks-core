#-------------------------------------------------
exec=findFirst

#-------------------------------------------------
libs=/usr/local/lib/tb/*.a
incs=/usr/local/include/tb
bin=./bin
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(incs)/utillib -I$(incs)/etherlib

#-------------------------------------------------
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    libs += -lcurl
endif
ifeq ($(UNAME_S),Darwin)
    libs += /Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd
endif

#-------------------------------------------------
# No need to edit below here
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
$(exec).cpp

#-------------------------------------------------
all:
	@make app

app:
	@mkdir -p bin objs
	@make $(product)
	mv $(product) $(dest)

test:
	@make
	@./do_tests

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
	-@$(RM) -f $(product) objs/* 2> /dev/null
