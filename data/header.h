void test();

class head {

	public:
	virtual void run() =0 ;
};

class A : public head {

	public:
	virtual void run();
	static void A_st();
};

class B:public head {

	public:
	virtual void run();
};
