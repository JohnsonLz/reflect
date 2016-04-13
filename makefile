#File start

#Project directory
PROJECT_DIR := .
SRC_DIR := $(PROJECT_DIR)/context
LIB_DIR := $(PROJECT_DIR)/lib
INC_DIR := $(PROJECT_DIR)/lib
OBJ_DIR	:= $(PROJECT_DIR)/obj

#Compiler paraments
CPPFLAGS := -std=c++11 -fPIC -g  -I$(PROJECT_DIR) -I$(INC_DIR)/libdwarf/include -I$(INC_DIR)/libelf/include  -I$(INC_DIR)/libtinyxml/include
#LDFLAG := -L$(LIB_DIR) -ljson -static
LDFLAG := -L$(LIB_DIR)/libelf/lib -L$(LIB_DIR)/libdwarf/lib -L$(LIB_DIR)/libtinyxml/lib -ldwarf -lelf -ltinyxml -Wl,-rpath=$(LIB_DIR)/libtinyxml/lib

#Sources and objs
SOURCES := $(wildcard $(SRC_DIR)/*.cc)

OBJS := $(call filter,%.o,$(call patsubst,%.cc,%.o,$(call addprefix,$(OBJ_DIR)/,$(call notdir,$(SOURCES)))))
LIBOBJS := $(call filter-out, $(OBJ_DIR)/parse.o, $(OBJS))

BIN =  ./obj/libreflect.so ./obj/dwarfparser 

#Define the final target which will be generate
all: $(BIN)  $(OBJS)

#Shell command will be excuted every time
$(shell mkdir -p $(OBJ_DIR))
#$(shell mkdir -p ./test)

#Auto generate dependency
$(SRC_DIR)/%.d: $(SRC_DIR)/%.cc 
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS)  $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	sed '$$a\	$(CXX) -c $< $(CPPFLAGS) -o $(OBJ_DIR)/$(call patsubst,%.cc,%.o,$(call notdir,$<))' -i $@; \
	rm -f $@.$$$$



#Include the dependency and command
#Command will not be excuted unless we need them
sinclude $(SOURCES:.cc=.d)

#./src/test:./obj/mempool.o ./obj/md5.o ./obj/test.o
#	$(CXX) -o $@ $(CPPFLAGS) $^
#	rm -f $(SOURCES:.cc=.d)

#./test/file_test: ./obj/file.o ./obj/log.o ./obj/file_test.o
#	$(CXX) -o $@ $(CPPFLAGS) $^
#	rm -f $(SOURCES:.cc=.d)


#./main: ./obj/mempool.o ./obj/alloc.o ./obj/util.o ./obj/diskio.o ./obj/arrayvet.o ./obj/iblock.o ./obj/sysblock.o ./obj/config.o ./obj/bitmapspacemanger.o ./obj/spacemanger.o ./obj/file.o ./obj/directory.o ./obj/defaultpermissionmodle.o ./obj/listfilemanger.o ./obj/filemanger.o ./obj/simdisk.o ./obj/main.o  
#	@echo $(OBJS)
#	$(CXX) -o $@ $(CPPFLAGS) $^

./obj/libreflect.so: $(LIBOBJS)
	$(CXX)  -o $@ -fPIC -shared -std=c++11 $^
	rm -f $(SOURCES:.cc=.d)		

./obj/dwarfparser: $(OBJS)
	$(CXX) -o $@  $^ $(CPPFLAGS) $(LDFLAG)	
	rm -f $(SOURCES:.cc=.d)	

.PHONY:clean install
clean:
	rm ./bin/dwarfparser 
	rm ./lib/libreflect/lib/libreflect.so  
	rm ./lib/libreflect/include/context/reflect.h
	rm ./lib/libreflect/include/context/container.h
	rm ./lib/libreflect/include/context/alloc.h
	rm -r $(OBJ_DIR)
	rm -f $(SOURCES:.cc=.d)	

install:
	mv ./obj/dwarfparser ./bin/dwarfparser
	mv ./obj/libreflect.so ./lib/libreflect/lib/libreflect.so
	cp ./context/reflect.h ./lib/libreflect/include/context/reflect.h
	cp ./context/container.h ./lib/libreflect/include/context/container.h 
	cp ./context/alloc.h ./lib/libreflect/include/context/alloc.h  
	rm -f $(SOURCES:.cc=.d)	
#File end
