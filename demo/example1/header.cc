#include "tinyxml.h"
#include "tinystr.h"

#include "context/reflect.h"
#include "context/alloc.h"
#include "header.h"
using namespace reflection;

void BeanFactory::configure(const char* name) {

    TiXmlDocument* Doc;
    Doc = new TiXmlDocument(name);

    if(Doc->LoadFile()) {

        TiXmlElement* root = 0;

        root = Doc->FirstChildElement("Configure");
        if(root) {

            TiXmlElement* func = root->FirstChildElement("Function");
            if(func) {
                const char* name = func->Attribute("name");
                const char* scole = func->Attribute("scale");

                TiXmlElement* arg = 0;
                arg =  func->FirstChildElement("Argument");
                std::vector<const char*> args;
                while(arg) {
                    const char* tmp = copyStr_(arg->FirstChild()->Value());
                	args.push_back(tmp);
                	arg = arg->NextSiblingElement("Argument");
                }
                Reflect::sharedContext().invoke(name, scole, args);
                for(int i=0; i<args.size(); i++) {
                    Alloc::dellocate(const_cast<char*>(args[i]));
                }
            }

            TiXmlElement* cla = root->FirstChildElement("Bean");
            if(cla) {
            	while(cla) {
            		std::string k(cla->Attribute("name"));
            		std::string v(cla->Attribute("reference"));
            		beanMap_.insert(make_pair(k, v));
            		cla = cla->NextSiblingElement("Bean");
            	}
            }
        }
        else {
        	printf("parse configure.xml error\n");
        	delete Doc;
        }
    }
    else {
    	printf("configure doest not exist\n");
    	delete Doc;
    }
    delete Doc;
}

void BeanFactory::setContextEnv(const char* name) {

	Reflect::sharedContext().setContextEnv(name);
}

void* BeanFactory::getBeanByName(const char* name) {

	std::map<std::string, std::string>::iterator iter;
    std::string s(name);
    iter = beanMap_.find(s);

    return Reflect::sharedContext().instanceOfClass(iter->second.c_str());
}

const char* BeanFactory::copyStr_(const char* str) {

    int length = strlen(str);
    char* res = static_cast<char*>(Alloc::allocate(length+1));
    strncpy(res, str, length);
    res[length] = '\0';
    return res;
}