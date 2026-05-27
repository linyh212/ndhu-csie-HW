#include <iostream>
#include <cmath>
#include <cstdlib>

class Fraction {
public:
    Fraction():numerator(0), denominator(1){}
    Fraction(int n, int m):numerator(n){
        if(m==0) 
            throw "divided by zero";
        denominator=m;
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
        if(m==0) 
            throw "divided by zero";
        denominator=m;
    }
    friend std::ostream& operator<<(std::ostream& out, const Fraction &f){
        if(f.getDenominator()!=1) 
            out<<"["<<f.getNumerator()<<"/"<<f.getDenominator()<<"]";
        else
            out<<f.getNumerator();
        return out;
    }    
private:
    int numerator, denominator;
};
int gcd(int a, int b){
    if(b==0) return a;
    else return gcd(b, a%b);
}
Fraction operator+(const Fraction &f, const Fraction &other){
    int n=f.getNumerator()*other.getDenominator()+other.getNumerator()*f.getDenominator();
    int d=f.getDenominator()*other.getDenominator();
    int val_gcd=gcd(std::abs(n), std::abs(d));
    return Fraction(n/val_gcd, d/val_gcd);
}
Fraction operator-(const Fraction &f, const Fraction &other){
    int n=f.getNumerator()*other.getDenominator()-other.getNumerator()*f.getDenominator();
    int d=f.getDenominator()*other.getDenominator();
    int val_gcd=gcd(std::abs(n), std::abs(d));
    return Fraction(n/val_gcd, d/val_gcd);
}
bool operator==(const Fraction &f, const Fraction &other){
    return (f.getNumerator()*other.getDenominator()==other.getNumerator()*f.getDenominator());
}

int main() {
    int n1, n2, m1, m2;
    std::cin >> n1 >> m1 >> n2 >> m2;
    Fraction f1(n1, m1), f2(n2, m2);
    std::cout << (f1 + f2) << std::endl;
    std::cout << (f1 - f2) << std::endl;
    std::cout << (f1 == f2) << std::endl;
}

