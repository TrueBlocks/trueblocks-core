<<<<<<< HEAD:src/apps/ethprice/makefile
#-------------------------------------------------
libraries=../../libs
bin=../../../bin
cflags=-std=c++11 -Wall -O2 -I. -I$(libraries)/include

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a /usr/lib/libcurl.dylib
# for ubuntu builds
#libs=$(libraries)/*.a -lcurl

product=$(bin)/ethprice

src= \
ethprice.cpp \
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
	@cd tests ; ./do_tests

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
=======
lint:
	@pylint *.[ch]*

test:
	@make
	@echo "No tests"
>>>>>>> develop:src/tools/getLogs/makefile
