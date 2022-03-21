#-------------------------------------------------
dyno=usdprice

#-------------------------------------------------
src= \
$(dyno).cpp

#-------------------------------------------------
# probably don't have to change below this line

#-------------------------------------------------
incs=/usr/local/include/tb
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(incs)/cachelib -I$(incs)/pinlib -I$(incs)/acctlib -I$(incs)/etherlib -I$(incs)/utillib

#-------------------------------------------------
cacheDest=cache/mainnet/objs/
libs=/usr/local/lib/tb/*.a
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	libs += -lcurl
	dest = ~/.local/share/trueblocks/$(cacheDest)
	out = lib$(dyno).so
endif
ifeq ($(UNAME_S),Darwin)
	libs += /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libcurl.4.tbd
	dest = ~/Library/Application\ Support/TrueBlocks/$(cacheDest)
	out = lib$(dyno).dylib
endif

#-------------------------------------------------
product=objs/$(exec)

#-------------------------------------------------
all:
	g++ $(cflags) -fPIC -shared -o $(out) $(libs) $(src)
	@mkdir -p $(dest)
	mv $(out) $(dest)

clean:
	-@$(RM) -fR $(dest)/$(out) 2> /dev/null
