#-------------------------------------------------
libraries=../../../../src/libs
bin=bin
cflags=-std=c++11 -Wall -O2 -I. -I$(libraries)/include

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a /usr/lib/libcurl.dylib
# for ubuntu builds
#libs=$(libraries)/*.a -lcurl

#-------------------------------------------------
exec=bytesUsed
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
options.cpp \
bytesUsed.cpp

#-------------------------------------------------
all:
	@make app

app:
	@echo ; colorEcho -t "Building $(exec)..."
	@make $(product) >/dev/null 2>&1
	mv $(product) $(dest) >/dev/null 2>&1

test:
	@make
	@colorEcho -t "Testing $(exec)..."
	@$(dest) | tee tests/results.txt

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
