cflags=-std=c++11 -Wall -O2 -I../utillib -I./

product=../abilib.a

src= \
abi.cpp \
function.cpp \
parameter.cpp

#abilib.cpp

all:
	@make $(product)
	@cp -p *.h ../include
	@echo ""

rebuild:
	@echo "Rebuilding abilib"
	@makeClass -arv --namespace:qblocks

lint:
	@pylint *.[ch]*

#-------------------------------------------------
# probably don't have to change below this line

objects = $(patsubst %.cpp,objs/%.o,$(src))

$(product): $(objects)
	-@$(RM) -f $*.a
	ar qc $*.a $(objects)
#	@chown jrush.jrush $(product)

$(objects): | objs

objs:
	@mkdir -p $@

objs/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(cflags) -c $< -o $@

clean:
	@-$(RM) -f $(dest) $(product) objs/*
