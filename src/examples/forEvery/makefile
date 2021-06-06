#-------------------------------------------------
libraries=../../libs
bin=bin
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib

#-------------------------------------------------
# for mac builds
#libs=$(libraries)/*.a /usr/lib/libcurl.dylib
# for ubuntu builds
libs=$(libraries)/*.a -lcurl

#-------------------------------------------------
exec=forEvery
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
$(exec).cpp

#-------------------------------------------------
all:
	@make app

app:
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
