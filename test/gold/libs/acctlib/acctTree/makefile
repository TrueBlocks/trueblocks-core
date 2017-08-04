#-------------------------------------------------
libraries=../../../../src/libs
b_libraries=../../../../build/libs
bin=bin
cflags=-std=c++11 -Wall -O2 -I. -I$(libraries)/include

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a /usr/lib/libcurl.dylib
# for ubuntu builds
#libs=$(libraries)/*.a -lcurl

#-------------------------------------------------
exec=acctTest
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
options.cpp \
acctTree.cpp

#-------------------------------------------------
all:
	@cd $(b_libraries) ; build ; cd -
	@make app

app:
	@make $(product)
	mv $(product) $(dest)

test:
	@make
	@$(dest) -s --nocolors 2>&1 | tee tests/results.txt

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
