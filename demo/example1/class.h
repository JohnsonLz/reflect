#ifndef CLASS_H_
#define CLASS_H_

class Sharp {

public:
	virtual void say()  =0 ;

};

class Point: public Sharp {

public:
	virtual void say();
};

class Line: public Sharp {

public:
	virtual void say();
};

class Init {

public:
	void declation(const char* arg);
};


#endif