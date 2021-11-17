#-------------------------------------------------
libraries=../../../libs
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib
curl=/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd

none:
	@echo "Run either 'make mac' or 'make linux'."

mac:
	cd ../../../../build ; make -j 12 ; cd -
	g++ $(cflags) -fPIC -shared -o libtrav1.dylib $(libraries)/*.a $(curl) trav1.cpp
	g++ $(cflags) -fPIC -shared -o libtrav2.dylib $(libraries)/*.a $(curl) trav2.cpp
	mv ./libtrav1.dylib ~/Library/Application\ Support/TrueBlocks/cache/objs/
	mv ./libtrav2.dylib ~/Library/Application\ Support/TrueBlocks/cache/objs/

linux:
	cd ../../../../build ; make -j 12 ; cd -
	g++ $(cflags) -fPIC -shared -o libtrav1.so $(libraries)/*.a trav1.cpp
	mv ./libtrav1.so ~/.local/share/trueblocks/cache/objs/
	g++ $(cflags) -fPIC -shared -o libtrav2.so $(libraries)/*.a trav2.cpp
	mv ./libtrav2.so ~/.local/share/trueblocks/cache/objs/
