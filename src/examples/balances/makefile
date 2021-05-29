#-------------------------------------------------
libraries=../../libs
cflags=-std=c++11 -Werror -Wall -O2 -I. -I$(libraries)/utillib -I$(libraries)/etherlib -I$(libraries)/acctlib
libs=$(libraries)/*.a

shared:
	g++ $(cflags) -fPIC -shared -o libbalances.so $(libs) balances.cpp
	mv ./libbalances.so ~/.local/share/trueblocks/cache/objs/
