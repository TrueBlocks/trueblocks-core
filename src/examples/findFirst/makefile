#-------------------------------------------------
libraries=../../libs
bin=bin
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib

#-------------------------------------------------
mac_libs=$(libraries)/*.a /Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd
linux_libs=$(libraries)/*.a -lcurl

#-------------------------------------------------
exec=findFirst
product=objs/$(exec)
dest=$(bin)/$(exec)

#-------------------------------------------------
src= \
$(exec).cpp

#-------------------------------------------------
all:
	@echo "Run either 'make mac' or 'make linux'."

mac:
	@make $(product)-mac
	mv $(product) $(dest)

linux:
	@make $(product)-linux
	mv $(product) $(dest)

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

$(product)-mac: $(objects) $(libs)
	@mkdir -p objs bin
	g++ -o $(product) $(objects) $(mac_libs)
	@strip $(product)

$(product)-linux: $(objects) $(linux_libs)
	@mkdir -p objs bin
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
