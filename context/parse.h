/**
    @file
    Provide xxx

    @author Youjie Xie<lz283447@gmail.com>

    Copyright &copy; 2016.
    This software is distributed in the hope that it will be useful, but
    without WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Permission is granted to anyone to make or distribute verbatim
    copies of this software provided that the copyright notice and
    this permission notice are preserved, and that the distributor
    grants the recipient permission for further distribution as permitted
    by this notice.

    Comments and suggestions are always welcome.

*/

#ifndef REFLECTION_CONTEXT_PARSE_H_
#define REFLECTION_CONTEXT_PARSE_H_

#include "context/list.h"

namespace reflection {

class Info;

class Parse {

private:

    void parseCuList_(Dwarf_Debug dbg);
    void parseDieAndSiblings_(Dwarf_Debug dbg, Dwarf_Die in_die,  int in_level);
    void extractDieData_(Dwarf_Debug dbg, Dwarf_Die print_me);
    void extractFuncName_(Dwarf_Debug dbg, Dwarf_Die die, char** name, int* funcAttr);
    void extractFuncPc_(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Addr& lowpc);
    void extractFuncAddr_(Dwarf_Debug dbg, Dwarf_Attribute attr, Dwarf_Addr *val);
    void extractMemberFuncScale_(Dwarf_Debug dbg, Dwarf_Die cur_die, char** scale);
    void extractVirtualFuncScale_(Dwarf_Debug dbg, Dwarf_Die print_me, char** scale);
    void extractFuncScale_(Dwarf_Debug dbg, Dwarf_Die die, int num, char** scale);
    void extractFunc_(Dwarf_Debug dbg, Dwarf_Die print_me);
    void extractScale_(Dwarf_Debug dbg, Dwarf_Attribute* attr, int level, char** scale);
    void extractDie_(Dwarf_Debug dbg, Dwarf_Attribute* attr, Dwarf_Half* tag, Dwarf_Die* type);
    const char* copyStr_(const char* str);

    List<Info> map_;

public:

    Parse(){};
    ~Parse(){}

    void traversal(const char* elfFilePath);
    void Persistence(const char* contestFile);

};

}// namespace reflection

#endif
