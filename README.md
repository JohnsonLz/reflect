Funny idea to accomplish the reflect function of C++,
whilch may will never be used in real life becuse it is
inefficiency

The idea is to extract the dwarf file which contain the
meta message of your program in debug. I use the open library
libdwarf(beside, libdwarf rely on libelf) to extact message
and tinyxml to save the message

By the way , I only tested on linux(Ubuntu) and GCC 4.8.4

Install:
./make
./make install

./bin/dwarfparser is the tool to extrace elf file
eg ./bin/dwarfparser yourProgeam ./name.xml

./lib/libdwarf/lib/libdwarf.so 
./lib/libdwarf/include/ the header file need to be included in program

./demo/example1 is a little demo project
cd ./demo/example1 and make
use the extact tool extract
./bin/dwarfparser ./demo/example1/reflect_test ./demo/example1/context.xml
and that all
now you can run the demo by ./reflect_test

to get the meta data we must compile the program in debug mode,
we use argument -g in g++
after extract the pragram and save the data in .xml
you can recompile your program in release mode.