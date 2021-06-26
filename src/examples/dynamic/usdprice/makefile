#-------------------------------------------------
libraries=../../../libs
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib
curl=/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd

none:
	@echo "Run either 'make mac' or 'make linux'."

mac:
	cd ../../../../build ; make -j 12 ; cd -
	g++ $(cflags) -fPIC -shared -o libusdprice.dylib $(libraries)/*.a $(curl) usdprice.cpp
	mv ./libusdprice.dylib ~/Library/Application\ Support/TrueBlocks/cache/objs/

linux:
	g++ $(cflags) -fPIC -shared -o libusdprice.so $(libraries)/*.a usdprice.cpp
	mv ./libusdprice.so ~/.local/share/trueblocks/cache/objs/
