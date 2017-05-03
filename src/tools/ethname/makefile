#-------------------------------------------------
libraries=../../libs
bin=../../../bin
cflags=-std=c++11 -Wall -O2 -I. -I$(libraries)/utillib

#-------------------------------------------------
# for mac builds
libs=$(libraries)/utillib.a /usr/lib/libcurl.dylib
# for ubuntu builds
#libs=$(libraries)/utillib.a -lcurl

#-------------------------------------------------
exec=ethname
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
ethname.cpp \
options.cpp

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
	-@$(RM) -f $(dest) objs/* 2> /dev/null
