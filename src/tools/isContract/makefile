#-------------------------------------------------
libraries=../../libs
bin=../../../bin
cflags=-std=c++11 -Wall -O2 -I. -I$(libraries)/include

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a /usr/lib/libcurl.dylib
# for ubuntu builds
#libs=$(libraries)/*.a -lcurl

#-------------------------------------------------
exec=isContract
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
options.cpp \
isContract.cpp

#-------------------------------------------------
all:
	@cd $(libraries) ; make | grep -v 'up to date'
	@make app

app:
	@make $(product)
	mv $(product) $(dest)

lint:
	@pylint *.[ch]*

test:
	@make
	@cd tests ; ./do_tests 2>&1 | tee results.txt

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
