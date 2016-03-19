#File start

#Project directory
PROJECT_DIR := .
SRC_DIR := $(PROJECT_DIR)/
LIB_DIR := /home/lz/Workplace/debug/reflection/lib
INC_DIR := /home/lz/Workplace/debug/reflection/lib
OBJ_DIR	:= $(PROJECT_DIR)/obj

#Compiler paraments
CPPFLAGS := -std=c++11 -fPIC -g  -I$(PROJECT_DIR) -I$(INC_DIR)/libreflect/include  -I$(INC_DIR)/libtinyxml/include
#LDFLAG := -L$(LIB_DIR) -ljson -static
LDFLAG := -L$(LIB_DIR)/libreflect/lib -lreflect -Wl,-rpath=$(LIB_DIR)/libreflect/lib -L$(LIB_DIR)/libtinyxml/lib -ltinyxml -Wl,-rpath=$(LIB_DIR)/libtinyxml/lib 
#Sources and objs
SOURCES := $(wildcard $(SRC_DIR)/*.cc)

OBJS := $(call filter,%.o,$(call patsubst,%.cc,%.o,$(call addprefix,$(OBJ_DIR)/,$(call notdir,$(SOURCES)))))
LIBOBJS := $(call filter-out, $(OBJ_DIR)/parse.o, $(OBJS))

BIN =  ./reflect_test

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

#./lib/reflect.so: $(LIBOBJS)
#	$(CXX)  -o $@ -fPIC -shared -std=c++11 $^
	

./reflect_test: $(OBJS)
	$(CXX) -o $@  $^ $(CPPFLAGS) $(LDFLAG)	
	rm -f $(SOURCES:.cc=.d)	

.PHONY:clean 
clean:

	rm -r $(OBJ_DIR)

#File end
