#include <iostream>
#include <cmath>

class Fraction{
public:
	Fraction():numerator(0), denominator(1){}
	Fraction(int n, int m): numerator(n){
		if(m==0) throw"divided by zero";
		else{
			int value=gcd(abs(n), abs(m));
			numerator=n/value;
			denominator=m/value;
			if(denominator<0){
				numerator*=-1;
				denominator*=-1;
			}
		}
	}
	int getNumerator() const{
		return numerator;
	}
	int getDenominator() const{
		return denominator;
	}
	void setNumerator(int n){
		numerator=n;
	}
	void setDenominator(int m){
		if(m==0) throw"divided by zero";
		else denominator=m;
	}
	void display(){
        std::cout<< *this;
    }
    friend std::ostream& operator<<(std::ostream& out, Fraction &f){
        if(f.denominator!=1) out<<"("<<f.numerator<<"/"<<f.denominator<<")";
        else out <<"("<<f.numerator<<")";
        return out;
    }
	Fraction operator+(const Fraction &other){
		int n=numerator*other.denominator+other.numerator*denominator;
		int d=denominator*other.denominator;
		return Fraction(n, d);
	}
	Fraction operator-(const Fraction &other){
		int n=numerator*other.denominator-other.numerator*denominator;
		int d=denominator*other.denominator;
		return Fraction(n, d);
	}
	Fraction operator*(const Fraction &other){
		int n=numerator*other.numerator;
		int d=denominator*other.denominator;
		return Fraction(n, d);
	}
	Fraction operator/(const Fraction &other){
		int n=numerator*other.denominator;
		int d=denominator*other.numerator;
		return Fraction(n, d);
	}
	Fraction& operator=(const Fraction &other){
		numerator=other.numerator;
		denominator=other.denominator;
		return *this;
	}
private:
	int numerator, denominator;
	static int gcd(int a, int b){
		if(b==0) return a;
		else return gcd(b, a%b);
	}
};

int main()
{
	Fraction a, b(1, 2), c(1, 2), d(1, 2), e(1, 2), f(1, 2);
	a = (((b + c) - d) * e) / f;
	a.display();
	return 0;
}
