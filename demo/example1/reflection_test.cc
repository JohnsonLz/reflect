#include "header.h"
#include "class.h"

int main() {

	BeanFactory::sharedFactory().setContextEnv("context.xml");
	BeanFactory::sharedFactory().configure("configure.xml");

	Sharp* s = static_cast<Sharp*>(BeanFactory::sharedFactory().getBeanByName("Sharp"));
	s->say();
}