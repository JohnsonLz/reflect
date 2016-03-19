
#ifndef HEADER_H_
#define HEADER_H_

#include <string>
#include <map>

class BeanFactory {

private:
	// struct func {

	// 	const char* name;
	// 	const char* scole;
	// 	std::std::vector<const char*> args;
	// };

	std::map<std::string, std::string> beanMap_;
	// std::vector<func*> v;

	// const char* copyStr(const char* src);
	BeanFactory(){}
	BeanFactory(const BeanFactory&){}
	void operator = (const BeanFactory&){}

	const char* copyStr_(const char* src);

public:

	static BeanFactory& sharedFactory() {
		static BeanFactory f;
		return f;
	}

	void configure(const char* fileName);
	void setContextEnv(const char* fileName);

	void* getBeanByName(const char* name);
};

#endif