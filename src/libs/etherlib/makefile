cflags=-std=c++11 -Wall -O2 -I../utillib -I../abilib -I./

product=../etherlib.a

src= \
account.cpp \
accountwatch.cpp \
balhistory.cpp \
block.cpp \
incomestatement.cpp \
logentry.cpp \
node.cpp \
pricequote.cpp \
receipt.cpp \
rpcresult.cpp \
trace.cpp \
traceaction.cpp \
traceresult.cpp \
transaction.cpp \
webapi.cpp

all:
	@make $(product)
	@cp -p *.h ../include
	@echo ""

rebuild:
	@echo "Rebuilding etherlib"
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
