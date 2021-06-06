#-------------------------------------------------
libraries=../../libs
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib
curl=/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd

none:
	@echo "Run either 'make mac' or 'make linux'."

mac:
	g++ $(cflags) -fPIC -shared -o libbalances.dylib $(libraries)/*.a $(curl) balances.cpp
	mv ./libbalances.dylib ~/Library/Application\ Support/TrueBlocks/cache/objs/

linux:
	g++ $(cflags) -fPIC -shared -o libbalances.so $(libraries)/*.a balances.cpp
	mv ./libbalances.so ~/.local/share/trueblocks/cache/objs/
