cflags=-std=c++11 -Wall -O2

product=../utillib.a

src= \
basenode.cpp \
bigint.cpp \
biglib.cpp \
biguint.cpp \
curl_code.cpp \
database.cpp \
dates.cpp \
exportcontext.cpp \
fielddata.cpp \
filenames.cpp \
options_base.cpp \
performance.cpp \
runtimeclass.cpp \
sfarchive.cpp \
sfos.cpp \
sfstring.cpp \
sftime.cpp \
toml.cpp

all:
	@make $(product)
	@cp -p *.h ../include
	@echo ""

rebuild:
	@echo "Rebuilding utillib"
	@makeClass -arv

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
