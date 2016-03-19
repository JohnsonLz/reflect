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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <new>
#include <cstdio>

#include "dwarf.h"
#include "libdwarf.h"
#include "tinyxml.h"
#include "tinystr.h"

#include "context/parse.h"
#include "context/alloc.h"
#include "context/scope.h"
#include "context/info.h"
#include "context/statue.h"

namespace reflection {

void Parse::traversal(const char* elfName) {

    Dwarf_Debug dbg = 0;  
    int fd = -1;  
    int res;
    Dwarf_Error error;  
    Dwarf_Handler errhand = 0;  
    Dwarf_Ptr errarg = 0;  
  
    fd = open(elfName, O_RDONLY);    
    if (fd < 0) {  
        throw Statue:: ELF_NOT_FOUND; 
    }  

    ON_SCOPE_EXIT([&]{ close(fd);})

    res = dwarf_init(fd, DW_DLC_READ, errhand, errarg, &dbg, &error);  
    if (res == DW_DLV_ERROR) {
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_INIT_FAILED;
    }
    parseCuList_(dbg);  

    ON_SCOPE_EXIT([&] {
        res = dwarf_finish(dbg, &error);    
        if (res != DW_DLV_OK) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_DESTORY_FAILED; 
        }   
    })
}


void Parse::parseCuList_(Dwarf_Debug dbg) {

    Dwarf_Unsigned cu_header_length = 0;  
    Dwarf_Half version_stamp = 0;  
    Dwarf_Unsigned abbrev_offset = 0;  
    Dwarf_Half address_size = 0;  
    Dwarf_Unsigned next_cu_header = 0;  
    Dwarf_Error error;  
    int cu_number = 0;  
  
    for (;; ++cu_number) {  
        Dwarf_Die no_die = 0;  
        Dwarf_Die cu_die = 0;  
        int res = DW_DLV_ERROR;  
        res = dwarf_next_cu_header(dbg, &cu_header_length, &version_stamp,  
                &abbrev_offset, &address_size, &next_cu_header, &error);  
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_CU_HEADER_FAILED;              
        }  
        if (res == DW_DLV_NO_ENTRY) {  
            return;  
        }  

        res = dwarf_siblingof(dbg, no_die, &cu_die, &error);  
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_CU_DIE_FAILED;   
        }  
        if (res == DW_DLV_NO_ENTRY) {  
            throw Statue:: DWARF_PAESE_UNKNOWN_ERROR; 
        }
        ON_SCOPE_EXIT([&]{ dwarf_dealloc(dbg, cu_die, DW_DLA_DIE); })  
        parseDieAndSiblings_(dbg, cu_die, 0);    
    }  
}

void Parse::parseDieAndSiblings_(Dwarf_Debug dbg, Dwarf_Die in_die,  int in_level) {

    int res = DW_DLV_ERROR;  
    Dwarf_Die cur_die = in_die;  
    Dwarf_Die child = 0;  
    Dwarf_Error error;  
    Dwarf_Half tag;  
  //  int get_child = 0;  
  
//    extractDieData_(dbg, in_die);  

    for (;;) {  
        Dwarf_Die sib_die = 0;  

        res = dwarf_tag(cur_die, &tag, &error);
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_DIE_FAILED;   
        }   
        if (res != DW_DLV_OK) {  
            throw Statue:: DWARF_PARSE_DIE_FAILED;   
        }
        if(!in_level) {
            res = dwarf_child(cur_die, &child, &error);  
            if (res == DW_DLV_ERROR) {  
                dwarf_dealloc(dbg, error, DW_DLA_ERROR);
                throw Statue:: DWARF_PARSE_DIE_FAILED;   
            }  
            ON_SCOPE_EXIT([&]{ dwarf_dealloc(dbg, child, DW_DLA_DIE);})
            if (res == DW_DLV_OK) {  
                parseDieAndSiblings_(dbg, child, in_level+1);  
            } 
        }  
        if (tag == DW_TAG_subprogram)  {
            extractDieData_(dbg, cur_die);  
        }

        res = dwarf_siblingof(dbg, cur_die, &sib_die, &error);  
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_DIE_FAILED; 
        }  
        if (res == DW_DLV_NO_ENTRY) {  
            break;  
        }  
        if (cur_die != in_die) {  
            dwarf_dealloc(dbg, cur_die, DW_DLA_DIE);  
        }  
        cur_die = sib_die;
  
    }
    if (cur_die != in_die) {  
        dwarf_dealloc(dbg, cur_die, DW_DLA_DIE);  
    }        
    return;  
}

void Parse::extractDieData_(Dwarf_Debug dbg, Dwarf_Die print_me) {  
 
//    char *name = 0;  
    Dwarf_Error error = 0;  
    Dwarf_Half tag = 0;  
    //const char *tagname = 0;  
    //int localname = 0;  
  
    // if (level == 0)  
    //     return;  
  
    // int res = dwarf_diename(print_me, &name, &error);  
    // if (res == DW_DLV_ERROR) {  
    //     dwarf_dealloc(dbg, error, DW_DLA_ERROR);
    //     throw Statue:: DWARF_PARSE_DIE_FAILED; 
    // }  
    // if (res == DW_DLV_NO_ENTRY) {
    //     name = 0;
    //     //localname = 1;  
    // }
    
    int res = dwarf_tag(print_me, &tag, &error);
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED; 
    }    
    if (res != DW_DLV_OK) {  
        throw Statue:: DWARF_PARSE_DIE_FAILED;  
    }  
    // if (tag == DW_TAG_lexical_block) { //局部块  
    //     printf("\n< %2d >", level);  
    //     printf("局部块\n");  
    //     print_subprog(dbg, print_me, level);  
    // }  

    //TODO: location name scale
    if (tag == DW_TAG_subprogram) { //子程序  

        extractFunc_(dbg, print_me);
        // if(0 == name) {
        //     extractFuncName_(print_me, name);
        // }
        // Dwarf_Addr lowpc;
        // extractFuncPc_(dbg, print_me, level, lowpc);  
        //res = dwarf_attr(print_me, DW_AT_frame_base, &attr, &error);  
    }  
    // if (tag == DW_TAG_formal_parameter { 
    //     if(strncmp(name, "this", 4)==0) {
    //         extractFuncScale_(dbg, print_me, level, 0);
    //     }
    //     else {
    //         //golbal
    //     }  
    // }  
  
    // res = dwarf_get_TAG_name(tag, &tagname);  
    // if (res != DW_DLV_OK) {  
    //     printf("Error in dwarf_get_TAG_name , level %d \n", level);  
    //     exit(1);  
    // }  

} 

void Parse::extractFunc_(Dwarf_Debug dbg, Dwarf_Die print_me) {

    int res = DW_DLV_ERROR;  
    Dwarf_Die cur_die = print_me;  
    Dwarf_Die child = 0; 
    Dwarf_Error error = 0;  
    Dwarf_Bool return_bool;

    res = dwarf_hasattr(cur_die, DW_AT_low_pc, &return_bool, &error);
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED;
    } 
    if (res != DW_DLV_OK || return_bool == 0) {
        return;
    }  

    res = dwarf_hasattr(cur_die, DW_AT_specification, &return_bool, &error);
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED;
    } 
    if (res != DW_DLV_OK || return_bool == 0) {
        return;
    }  

    res = dwarf_hasattr(cur_die, DW_AT_object_pointer, &return_bool, &error);
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED;
    } 
    if (res != DW_DLV_OK || return_bool == 0) {
        return;
    }  

    Dwarf_Addr lowpc;
    extractFuncPc_(dbg, cur_die, lowpc);  

    char* nametmp = 0;
    int funcAttr;
    extractFuncName_(dbg, cur_die, &nametmp, &funcAttr);
    if(nametmp == 0) {
        return;
    }
    ON_SCOPE_EXIT([&] {dwarf_dealloc(dbg, nametmp, DW_DLA_STRING);})

    char* scaletmp = 0;
    if(funcAttr == 1) {

        extractVirtualFuncScale_(dbg, cur_die, &scaletmp);
    }
    else {
        extractMemberFuncScale_(dbg, cur_die, &scaletmp);
    }
    if(scaletmp == 0) {
        return;
    }
    ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, scaletmp, DW_DLA_STRING);})

    const char* name = copyStr_(nametmp);
    const char* scale = copyStr_(scaletmp);
    Info* inf = static_cast<Info*>(Alloc::allocate(sizeof(Info)));
    new(inf) Info(name, scale, lowpc);
    map_.insert(inf);
}

void Parse::extractFuncName_(Dwarf_Debug dbg, Dwarf_Die die, char** name, int* funcAttr) {

    int res;  
    Dwarf_Error error = 0;  
    Dwarf_Attribute t_attr; 
    Dwarf_Half tag = 0; 
    res = dwarf_attr(die, DW_AT_specification, &t_attr, &error);  
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED; 
    }

    ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);})
    if (res == DW_DLV_OK) { 
        Dwarf_Die typeDie;
        extractDie_(dbg, &t_attr, &tag, &typeDie);
        ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, typeDie, DW_DLA_DIE); })
        res = dwarf_diename(typeDie, name, &error);
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_DIE_FAILED; 
        }

        Dwarf_Bool return_bool;
        res = dwarf_hasattr(typeDie, DW_AT_virtuality, &return_bool, &error);
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_DIE_FAILED;
        } 
        if (res == DW_DLV_OK) {
            if(return_bool) {
                *funcAttr = 1;
            }
            else {
                *funcAttr = 0;
            }
        }


    }      
}

void Parse::extractFuncPc_(Dwarf_Debug dbg, Dwarf_Die die, Dwarf_Addr& lowpc) {  

    int res;  
    Dwarf_Error error = 0;  
//   Dwarf_Addr lowpc = 0;  
//    Dwarf_Addr highpc = 0;  
    Dwarf_Attribute t_attr;  
    res = dwarf_attr(die, DW_AT_inline, &t_attr, &error);  
    if (res == DW_DLV_OK) { 
        //printf("< %2d >", level);  
        //printf("inline sub !\n");  
        dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);
        return;  
    }  
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
    }  
  
    
    res = dwarf_attr(die, DW_AT_low_pc, &t_attr, &error);  
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED;
    }
    ON_SCOPE_EXIT([&]{ dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);}) 
    if (res == DW_DLV_OK) // 获取DW_AT_low_pc  
        extractFuncAddr_(dbg, t_attr, &lowpc);  
  
    // res = dwarf_attr(die, DW_AT_high_pc, &t_attr, &error);  
    // if (res == DW_DLV_OK) // 获取DW_AT_high_pc  
    //     get_addr(t_attr, &highpc);  
  
    // if (lowpc) {  

    // }  
    // if (highpc) {  
    //     printf("< %2d >", level);  
    //     printf("high_pc: 0x%" DW_PR_DUx "\n", (Dwarf_Unsigned) highpc);  
    // }  
} 

void Parse::extractFuncAddr_(Dwarf_Debug dbg, Dwarf_Attribute attr, Dwarf_Addr *val) {  
    Dwarf_Error error = 0;  
    int res;  
    Dwarf_Addr uval = 0;  
    res = dwarf_formaddr(attr, &uval, &error);  
    if (res == DW_DLV_OK) {  
        *val = uval;  
        return;  
    }
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED;
    }  
    return;  
}  

void Parse::extractMemberFuncScale_(Dwarf_Debug dbg, Dwarf_Die cur_die, char** scale) {

    Dwarf_Die child;
    Dwarf_Error error;

    int res = dwarf_child(cur_die, &child, &error);  
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED;
    } 
    if (res == DW_DLV_NO_ENTRY) {
        return;
    }

    ON_SCOPE_EXIT([&]{ dwarf_dealloc(dbg, child, DW_DLA_DIE);})  
    if (res == DW_DLV_OK) {  
        Dwarf_Half tag = 0;  
        int localname = 0;  
 
        res = dwarf_tag(child, &tag, &error);  
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_DIE_FAILED;
        } 
        if (tag == DW_TAG_formal_parameter) {
            char* para; 
            res = dwarf_diename(child, &para, &error);  
            if (res == DW_DLV_ERROR) {  
                dwarf_dealloc(dbg, error, DW_DLA_ERROR);
                throw Statue:: DWARF_PARSE_DIE_FAILED;
            }

            ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, para, DW_DLA_STRING); }) 
            if(strncmp(para, "this", 4)==0) {
                extractFuncScale_(dbg, child, 0, scale);
            }
            else {
                throw Statue::DWARF_PARSE_DIE_FAILED;
            }
     
        }  
    }
}

void Parse::extractFuncScale_(Dwarf_Debug dbg, Dwarf_Die die, int num, char** scale) {  

    int res;  
    Dwarf_Error error = 0;  
    Dwarf_Attribute t_attr;  
    res = dwarf_attr(die, DW_AT_declaration, &t_attr, &error);
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
    }    
    if (res == DW_DLV_OK) { //extern变量,没有位置信息  
        // printf("< %2d >", level);  
        // printf("extern变量! \n");
        dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);  
        return;  
    }  
  
    // res = dwarf_attr(die, DW_AT_location, &t_attr, &error);  
    // if (res == DW_DLV_OK) //变量位置  
    //     get_location(dbg, t_attr, level);  
  
    res = dwarf_attr(die, DW_AT_type, &t_attr, &error);
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED;
    }

    ON_SCOPE_EXIT([&]{ dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);})    
    if (res == DW_DLV_OK) { // 类型  
    //    printf("< %2d >变量类型:", level);  
        extractScale_(dbg, &t_attr, num, scale);  
    }  
}


void Parse::extractScale_(Dwarf_Debug dbg, Dwarf_Attribute* attr, int level, char** scale) {  
 
    char *name = 0;  
    Dwarf_Half tag;  
    Dwarf_Unsigned size;  
    Dwarf_Error error = 0;  
    Dwarf_Attribute t_attr;  
    int res;  
    Dwarf_Die typeDie = 0;

    extractDie_(dbg, attr, &tag, &typeDie);
    ON_SCOPE_EXIT([&] { if(typeDie) dwarf_dealloc(dbg, typeDie, DW_DLA_DIE);})  
    
    if (typeDie) {  
        switch (tag) {
        // need const ptr class
        case DW_TAG_const_type:
            if(level == 0) {  
                res = dwarf_attr(typeDie, DW_AT_type, &t_attr, &error);  
                if (res == DW_DLV_ERROR) {  
                    dwarf_dealloc(dbg, error, DW_DLA_ERROR);
                    throw Statue:: DWARF_PARSE_DIE_FAILED;
                }
                ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);})
                if (res == DW_DLV_OK) {  
                    extractScale_(dbg, &t_attr, 1, scale);  
                }
            }
            break;          
        case DW_TAG_pointer_type:  
            if(level == 1) {  
                res = dwarf_attr(typeDie, DW_AT_type, &t_attr, &error);    
                if (res == DW_DLV_ERROR) {  
                    dwarf_dealloc(dbg, error, DW_DLA_ERROR);
                    throw Statue:: DWARF_PARSE_DIE_FAILED;
                }
                ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);})
                if (res == DW_DLV_OK) {  
                    extractScale_(dbg, &t_attr, 2, scale);  
                }
            }
            break;    
        default:
            if(level == 2) {
                res = dwarf_diename(typeDie, scale, &error);  
                if (res == DW_DLV_ERROR) {  
                    dwarf_dealloc(dbg, error, DW_DLA_ERROR);
                    throw Statue:: DWARF_PARSE_DIE_FAILED;
                }  
                dwarf_dealloc(dbg, name, DW_DLA_STRING);
            }
            break;
        }
    }

    return;  
}  


void Parse::extractVirtualFuncScale_(Dwarf_Debug dbg, Dwarf_Die print_me, char** scale) {

    int res;  
    Dwarf_Error error = 0;  
    Dwarf_Attribute t_attr; 
    Dwarf_Half tag = 0; 
    Dwarf_Die child;

    res = dwarf_attr(print_me, DW_AT_specification, &t_attr, &error);  
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED; 
    }

    ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, t_attr, DW_DLA_ATTR);})
    if (res == DW_DLV_OK) { 
        Dwarf_Die typeDie;
        extractDie_(dbg, &t_attr, &tag, &typeDie);
        ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, typeDie, DW_DLA_DIE); })

        Dwarf_Attribute typeAttr;
        res = dwarf_attr(typeDie, DW_AT_containing_type, &typeAttr, &error);  
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_DIE_FAILED; 
        }

        ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, typeAttr, DW_DLA_ATTR);})
        if (res == DW_DLV_OK) { 
            Dwarf_Die type;
            extractDie_(dbg, &typeAttr, &tag, &type);
            char* name;
            res = dwarf_diename(type, &name, &error);  
            if (res == DW_DLV_ERROR) {  
                dwarf_dealloc(dbg, error, DW_DLA_ERROR);
                throw Statue:: DWARF_PARSE_DIE_FAILED;
            }  
            *scale = name;
        }       
    }
}

void Parse::extractDie_(Dwarf_Debug dbg, Dwarf_Attribute* attr, Dwarf_Half* tag, Dwarf_Die* R) {

    Dwarf_Error error = 0;  
    int res;  
    Dwarf_Off offset;
    Dwarf_Die typeDie;  

    res = dwarf_global_formref(*attr, &offset, &error);
    if (res == DW_DLV_ERROR) {  
        dwarf_dealloc(dbg, error, DW_DLA_ERROR);
        throw Statue:: DWARF_PARSE_DIE_FAILED; 
    }    
    if (res == DW_DLV_OK) {  
        res = dwarf_offdie(dbg, offset, &typeDie, &error);  
        if (res == DW_DLV_ERROR) {  
            dwarf_dealloc(dbg, error, DW_DLA_ERROR);
            throw Statue:: DWARF_PARSE_DIE_FAILED; 
        }
       // ON_SCOPE_EXIT([&] { dwarf_dealloc(dbg, typeDie, DW_DLA_DIE); })  
        
        if (res == DW_DLV_OK) {  
            res = dwarf_tag(typeDie, tag, &error);  
            if (res == DW_DLV_OK) {  
                *R = typeDie;
                return;  
            }
            if (res == DW_DLV_ERROR) {  
                dwarf_dealloc(dbg, error, DW_DLA_ERROR);
                dwarf_dealloc(dbg, typeDie, DW_DLA_DIE);
                throw Statue:: DWARF_PARSE_DIE_FAILED; 
            }  
        }  
    }  
}     

const char* Parse::copyStr_(const char* str) {

    int length = strlen(str);
    char* res = static_cast<char*>(Alloc::allocate(length+1));
    strncpy(res, str, length);
    res[length] = '\0';
    return res;
}

void Parse::Persistence(const char* contestFile) {

    List<Info>::const_iterator iter;
    iter = map_.start();

    TiXmlDocument* Doc = new TiXmlDocument();
    ON_SCOPE_EXIT([&]{ delete Doc;})

    char buffer[255];
    TiXmlElement root("context");
    while(iter != map_.end()) {

        TiXmlElement elem("func");
        elem.SetAttribute("name", iter->data->Name);
        elem.SetAttribute("scole", iter->data->Scale);
        sprintf(buffer, "%lld", iter->data->PC);
        elem.SetAttribute("pc", buffer);

        root.InsertEndChild(elem);
        iter = iter->next;
    }

    Doc->InsertEndChild(root);
    Doc->SaveFile(contestFile);

    map_.freeValueType([](void* item) {

        Info* it = static_cast<Info*>(item);
        Alloc::dellocate(const_cast<char*>(it->Scale));
        Alloc::dellocate(const_cast<char*>(it->Name));
        Alloc::dellocate(it);       
    });

}

} // namespace reflection

using namespace reflection;

int main(int argc, char** argv) {

    if(argc < 3) {
        printf("Argument error \n");
        return 0;
    }
    Parse p;
    try {
        p.traversal(argv[1]);
    }
    catch(Statue s) {
        if(s == Statue::ELF_NOT_FOUND) {
            printf("%s does not exist\n", argv[1]);
        }
        if(s == Statue::DWARF_INIT_FAILED) {
            printf("Dwarf initailize failed\n");
        }
        if(s == Statue::DWARF_DESTORY_FAILED) {
            printf("Dwarf destory error\n");
        }
        else {
            printf("Dwarf parse failed\n");
        }
        return 0 ;
    }
    p.Persistence(argv[2]);
}