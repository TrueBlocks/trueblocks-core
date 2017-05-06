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
exec=whenBlock
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
options.cpp \
whenBlock.cpp

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
	@cat tests/in.txt tests/out.txt >tests/results.txt
	@echo "Testing" >>tests/results.txt
	@whenBlock --file:tests/in.txt | tee x ; cat x >>tests/results.txt
	@echo "--------------------------------" | tee x ; cat x >>tests/results.txt
	@whenBlock --file:tests/out.txt | tee x ; cat x >>tests/results.txt ; rm -f x

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
