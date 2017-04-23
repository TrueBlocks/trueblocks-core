#-------------------------------------------------
libraries=../../libs
bin=../../../bin
cflags=-std=c++11 -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/abilib -I$(libraries)/etherlib

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a /usr/lib/libcurl.dylib
# for ubuntu builds
#libs=$(libraries)/*.a -lcurl

product=$(bin)/ethslurp

src= \
ethslurp.cpp \
options.cpp

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))
objs=./objs

all:
	@cd $(libraries) ; make | grep -v 'up to date'
	@make app

app:
	@make $(product)

lint:
	@pylint *.[ch]*

test: all
	@cd theData/tests; make; cd -
	@cd theData/theDAO; make; cd -
	@cd theData/funcs; make; cd -
	@cd theData/ghc; make; cd -
	@cd theData/otherContracts; make; cd -
	@cd theData/whales; make; cd -
	@cd theData/hack; make; cd -
	@make -B back

#	@cd src/summation ; make data ; cd -

back:
	@rm -fR theData/backup
	@mkdir theData/backup
	@cp -pR ~/.ethslurp/abis          theData/backup/
	@cp -pR ~/.ethslurp/configs       theData/backup/
	@cp -pR ~/.ethslurp/slurps        theData/backup/
	@cp -p  ~/.ethslurp/ethslurp.toml theData/backup/
#	@cp -p  ~/.ethslurp/prices.bin    theData/backup/

$(product): $(objects) $(libs)
	$(CXX) -o $(product) $(objects) $(libs)
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
	-@$(RM) -f $(dest) $(product) $(objs)/*.o $(objs)/*.a 2> /dev/null
