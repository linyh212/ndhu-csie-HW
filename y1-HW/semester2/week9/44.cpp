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

class Decoder{
public:
	virtual void setValue(bool, int)=0;
	virtual void setValue(Gate *, int)=0;
	virtual void setEnable(bool)=0;
	virtual void setEnable(Gate *)=0;
	virtual int output()=0;
	virtual Gate *operator[](int)=0;
protected:
	Gate *enable;
};

class Decoder2_4: public Decoder{
public:
	Decoder2_4(): Decoder2_4(0){}
	Decoder2_4(bool val){
		if(val) this->enable=&t;
		else this->enable=&f;
		for(int i=0; i<2; i++)
			component[i]=new NOT;
		for(int i=2; i<6 ; i++)
			component[i]=new AND;
		for(int i=0; i<4 ; i++)
			enables[i] = new AND ;
    }
	virtual void setEnable(bool val){
		if(val) this->enable= &t;
		else this->enable= &f;
    }
	virtual void setEnable(Gate *gate){this->enable=gate;}
	virtual void setValue(Gate *gate, int i){component[i%2]->input[0]=gate;}
    virtual void setValue(bool val, int i){
		if(val) component[i%2]->input[0]=&t;
		else component[i%2]->input[0]=&f;
	}
	virtual Gate *operator[](int n){
		_out();
		switch(n){
			case 0: return enables[0];
			case 1: return enables[1];
			case 2: return enables[2];
			case 3: return enables[3];
			default: return nullptr;
        }
    }
	friend ostream &operator<<(ostream &out, Decoder2_4 dec){
        for(int i=3 ; i>=0 ; i--)
			out<<dec[i]->output()<<" ";
		return out;
    }
	virtual int output(){
		for(int i=0; i<4; i++)
			if(enables[i]->output()) return i;
        }
private:
	Gate *component[6];
	Gate *enables[4];
	void _out(){
		component[2]->input[0]=component[0];
		component[2]->input[1]=component[1];
		
		component[3]->input[0]=component[0]->input[0];
		component[3]->input[1]=component[1];

        component[4]->input[0]=component[0];
        component[4]->input[1]=component[1]->input[0];

        component[5]->input[0]=component[0]->input[0];
        component[5]->input[1]=component[1]->input[0];
		for(int i=0; i<4; i++){
			enables[i]->input[0]=this->enable;
			enables[i]->input[1]=component[i+2];
		}
	}
};

class Decoder4_16: public Decoder{
public:
	Decoder4_16(){
		for(int i=0; i<5; i++) dec2_4[i]=new Decoder2_4;
	}
	Decoder4_16(bool val){
		for(int i=0; i<5; i++) dec2_4[i]=new Decoder2_4(val);
	}
	virtual void setEnable(bool val){
		if(val) dec2_4[0]->setEnable(&t);
		else dec2_4[0]->setEnable(&f);
	}
	virtual void setEnable(Gate *gate){
		dec2_4[0]->setEnable(gate);
	}
	virtual void setValue(bool val, int pin){
		if(pin==0) for(int i=1; i<5; i++) dec2_4[i]->setValue(val, 0);
		else if(pin==1) for(int i=1; i<5; i++) dec2_4[i]->setValue(val, 1);
        else{
			if(pin==2) dec2_4[0]->setValue(val, 0);
			else if(pin==3) dec2_4[0]->setValue(val, 1);
		}
		for(int i=1; i<5; i++) dec2_4[i]->setEnable((*dec2_4[0])[i-1]);
    }
    virtual void setValue(Gate *gate, int pin){
		if(pin==0) for(int i=1; i<5; i++) dec2_4[i]->setValue(gate, 0);
		else if(pin==1) for(int i=1; i<5; i++) dec2_4[i]->setValue(gate, 1);
		else{
			if(pin==2) dec2_4[0]->setValue(gate, 0);
			else if(pin==3) dec2_4[0]->setValue(gate, 1);
		}
		for(int i=1; i<5; i++) dec2_4[i]->setEnable((*dec2_4[0])[i-1]);
    }
	virtual Gate *operator[](int n){
		switch(n){
			case 0 ... 15: return (*dec2_4[(n/4)+1])[n%4];
            default: return nullptr;
		}
	}
	friend ostream &operator<<(ostream &out, Decoder4_16 dec){
        for(int i=15; i>=0; i--) out<<dec[i]->output()<<" ";
        return out;
    }
	int output(){
        for(int i=0; i<16; i++) if((*this)[i]->output()) return i;
        return -1;
    }
private:
    Decoder *dec2_4[5];
};

class FourBitsRippleAdder: public Adder{
public:
    FourBitsRippleAdder(){
        for(int i=0; i<4; i++) a[i]=new OneBitFullAdder;
        a[0]->setValue(false, 2);
    }
    virtual void setValue(bool val, int pin){
        a[pin/2]->setValue(val, pin%2);
    }
    virtual void setValue(Gate *gate, int pin){
        a[pin/2]->setValue(gate, pin%2);
    }
    virtual Gate *sum(){}
    bool sum2(int n){
        for(int i=1; i<4; i++) a[i]->setValue(a[i-1]->carryOut(), 2);
        return a[n]->sum()->output();
    }
    virtual Gate *carryOut(){
        for(int i=1; i<4; i++) a[i]->setValue(a[i-1]->carryOut(), 2);
        return a[3]->carryOut();
    }
private:
    Adder *a[4];
};

class Decoder5_32: public Decoder{
public:
    Decoder5_32(){
        for(int i=0; i<2; i++) dec4_16[i]=new Decoder4_16;
    }
	Decoder5_32(bool val){
        for(int i=0; i<2; i++) dec4_16[i]=new Decoder4_16(val);
    }
	virtual void setEnable(bool val){
        for(int i=0; i<2; i++) dec4_16[i]->setEnable(val);
    }
	virtual void setEnable(Gate *gate){
        for(int i=0; i<2; i++)
			dec4_16[i]->setEnable(gate);
	}
	virtual void setValue(bool val, int pin){
        if(pin<4) for(int i=0; i<2; i++) dec4_16[i]->setValue(val, pin);
        else{
			dec4_16[0]->setEnable(val);
            dec4_16[1]->setEnable(!val);
        }
    }
	virtual void setValue(Gate *gate, int pin){
		if(pin<4) for(int i=0; i<2; i++) dec4_16[i]->setValue(gate, pin);
        else{
			dec4_16[0]->setEnable(gate);
			dec4_16[1]->setEnable(!(gate->output()));
        }
    }
	virtual Gate *operator[](int n){
        switch(n){
			case 0 ... 31: return (*dec4_16[n/16])[n%16];
                default: return nullptr;
            }
        }
		friend ostream &operator<<(ostream &out, Decoder5_32 dec){
            for(int i=31; i>=0; i--) out<<dec[i]->output()<<" ";
            return out;
        }
		int output(){
            for(int i=0; i<31; i++) if((*this)[i]->output()) return i;
            return -1;
        }
private:
    Decoder *dec4_16[2];
};

int main(){
    FourBitsRippleAdder FBRA;
    int a3, a2, a1, a0, b3, b2, b1, b0;
    cin>>a3>>a2>>a1>>a0>>b3>>b2>>b1>>b0;
    FBRA.setValue(a3, 3);
    FBRA.setValue(a2, 2);
    FBRA.setValue(a1, 1);
    FBRA.setValue(a0, 0);
    FBRA.setValue(b3, 7);
    FBRA.setValue(b2, 6);
    FBRA.setValue(b1, 5);
    FBRA.setValue(b0, 4);
    Decoder5_32 dec(true);
    dec.setValue(FBRA.sum2(3), 3);
    dec.setValue(FBRA.sum2(2), 2);
    dec.setValue(FBRA.sum2(1), 1);
    dec.setValue(FBRA.sum2(0), 0);
    dec.setValue(FBRA.carryOut(), 4);
    cout<<dec<<endl;
    cout<<dec.output(); 
}
