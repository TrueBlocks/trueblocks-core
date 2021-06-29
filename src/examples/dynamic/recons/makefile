#-------------------------------------------------
libraries=../../../libs
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib
curl=/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd

none:
	@echo "Run either 'make mac' or 'make linux'."

mac:
	cd ../../../../build ; make -j 12 ; cd -
	g++ $(cflags) -fPIC -shared -o librecons.dylib $(libraries)/*.a $(curl) recons.cpp
	mv ./librecons.dylib ~/Library/Application\ Support/TrueBlocks/cache/objs/

linux:
	g++ $(cflags) -fPIC -shared -o librecons.so $(libraries)/*.a recons.cpp
	mv ./librecons.so ~/.local/share/trueblocks/cache/objs/
