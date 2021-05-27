#-------------------------------------------------
libraries=../../libs
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib
curl=/Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/lib/libcurl.4.tbd

#-------------------------------------------------
# for mac builds
libs=$(libraries)/*.a $(curl)

trav1:
	g++ $(cflags) -dynamiclib -o libtrav1.dylib $(libs) trav1.cpp
	ls -l

trav2:
	g++ $(cflags) -dynamiclib -o libtrav2.dylib $(libs) trav2.cpp
	ls -l
