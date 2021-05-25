#-------------------------------------------------
libraries=../../libs
bin=bin
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib
curl=/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a $(curl)

#-------------------------------------------------
exec=dynoload
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
main.cpp

#-------------------------------------------------
all:
	@make app

every:
	@cd ../../../build ; make ; cd -
	@make app

app:
	@mkdir -p bin
	@make $(product)
	mv $(product) $(dest)

test:
	@make
	@echo "No tests"

shared:
	g++ $(cflags) -dynamiclib -o libshared.dylib $(libs) shared.cpp

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
