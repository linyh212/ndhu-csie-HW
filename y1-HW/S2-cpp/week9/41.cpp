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
/* Reference example
class XNOR : public Gate
{
    public :
        XNOR() : Gate()
        {
            component[0] = new NOT ;
            component[1] = new XOR ;
        }
        virtual bool output()
        {
            component[1] -> input[0] = this -> input[0] ;
            component[1] -> input[1] = this -> input[1] ;
            component[0] -> input[0] = component[1] ;
            return component[0] -> output() ;
        }
    private :
        Gate *component[2] ;
} ;
*/

int main(){
    XOR xor_1, xor_2, xor_3, xor_4;
    xor_1.setValue(false, 0);
    xor_1.setValue(false, 1); 
    xor_2.setValue(false, 0);
    xor_2.setValue(true, 1);
    xor_3.setValue(true, 0);
    xor_3.setValue(false, 1); 
    xor_4.setValue(true, 0);
    xor_4.setValue(true, 1);    
	cout<<xor_1.output()<<endl<<xor_2.output()<<endl<<xor_3.output()<<endl<<xor_4.output()<<endl;
    return 0;
}
