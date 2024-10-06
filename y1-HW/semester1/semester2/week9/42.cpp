#include <bits/stdc++.h>
using namespace std;
	
class Gate{
public:
	Gate *input[2];
	virtual bool output()=0;
	void setValue(Gate *, int);
	void setValue(bool, int);
};
	
class TRUE: public Gate{
public:
	virtual bool output(){return 1;}
	void setValue(Gate *, int){}
	void setValue(bool, int){}
};

class FALSE: public Gate{
public:
	virtual bool output(){return 0;}
	void setValue(Gate *, int){}
	void setValue(bool, int){}
};

TRUE t;
FALSE f;

void Gate::setValue(bool val, int pin){
    if(val) this->input[pin]=&t;
    else this->input[pin]=&f;
}

void Gate::setValue(Gate *gate, int pin){this->input[pin]=gate;}

class NOT: public Gate{
public:
	virtual bool output(){return !(this->input[0]->output());}
    void setValue(bool val, int pin=0){
		if(val) this->input[0]=&t;
		else this->input[0]=&f;
    }
    void setValue(Gate* gate, int pin=0){this->input[0]=gate;}
};

class NAND: public Gate{
public:
	virtual bool output(){return !(this->input[0]->output())||!(this->input[1]->output());}
};

class NOR: public Gate{
public:
	virtual bool output(){return !(this->input[0]->output())&& !(this->input[1]->output());}
};

class AND: public Gate{
public:
    AND(): Gate(){
		component[0]=new NOT();
		component[1]=new NAND();
	}
    virtual bool output(){
		component[1]->input[0]=this->input[0];
        component[1]->input[1]=this->input[1];
        component[0]->input[0]=component[1];
        return component[0]->output();	
	}
private:
    Gate *component[2];
};

class OR: public Gate{
public:
	OR(): Gate(){
		component[0]=new NOT();
		component[1]=new NOR();
	}
    virtual bool output(){
		component[1]->input[0]=this->input[0];
        component[1]->input[1]=this->input[1];
        component[0]->input[0]=component[1];
        return component[0]->output();
	}
private:
	Gate *component[2];
};

class XOR: public Gate{
public:
	XOR(): Gate(){
		component[0]=new NAND();
		component[1]=new OR();
		component[2]=new AND();
		component[3]=new NOT();
		component[4]=new NOR();
	}
	virtual bool output(){
		component[0]->input[0]=this->input[0];//nand
        component[0]->input[1]=this->input[1];
		        
		component[1]->input[0]=this->input[0];//or
        component[1]->input[1]=this->input[1];
        
		component[2]->input[0]=component[0];//and
        component[2]->input[1]=component[1];
        
        return component[2]->output();
		}
private:
	Gate *component[5];
};

class Adder{
public:
    virtual void setValue(bool, int)=0;
    virtual void setValue(Gate *, int)=0;
    virtual Gate *sum()=0;
    virtual Gate *carryOut()=0;
};

class OneBitHalfAdder: public Adder{
public:
    OneBitHalfAdder(){
		component[0]=new XOR();
		component[1]=new AND();
	}
    virtual void setValue(bool val, int pin){
		this->input[pin]=(val)? static_cast<Gate*>(&t):static_cast<Gate*>(&f);
	}
    virtual void setValue(Gate *gate, int pin){
		this->input[pin]=gate;
	}
    virtual Gate *sum(){
		component[0]->input[0]=this->input[0];
        component[0]->input[1]=this->input[1];
        return component[0];
	}
    virtual Gate *carryOut(){
		component[1]->input[0]=this->input[0];
        component[1]->input[1]=this->input[1];
        return component[1];
	}
private:
    Gate *component[2];
    Gate *input[2];
};

class OneBitFullAdder: public Adder{
public:
    OneBitFullAdder(){
		a[0]=new OneBitHalfAdder();
		a[1]=new OneBitHalfAdder();
		carry=new OR(); 
	}
    virtual void setValue(bool val, int pin){
		this->input[pin]=(val)? static_cast<Gate*>(&t):static_cast<Gate*>(&f);
	}
    virtual void setValue(Gate *gate, int pin){
		this->input[pin]=gate;
	}
    virtual Gate *sum(){
		a[0]->setValue(this->input[0], 0);
		a[0]->setValue(this->input[1], 1);
		Gate *s=a[0]->sum();
		a[1]->setValue(s, 0);
		a[1]->setValue(this->input[2], 1);
		return a[1]->sum();
	}
    virtual Gate *carryOut(){
		a[0]->setValue(this->input[0], 0);
		a[0]->setValue(this->input[1], 1);
		Gate *c1=a[0]->carryOut();
		a[1]->setValue(a[0]->sum(), 0);
		a[1]->setValue(this->input[2], 1);
		Gate *c2=a[1]->carryOut();
		carry->input[0]=c1;
		carry->input[1]=c2;		
		return carry;
	}
private:
    Adder *a[2];
    Gate *carry;
    Gate *input[3];
};

