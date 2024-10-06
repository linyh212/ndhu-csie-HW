#include <bits/stdc++.h>
using namespace std;

class PokerCard{
public:
    PokerCard(std::string s, int f){
        suit=s;
        face=f;
        if(face==1) face=14; 
    }
    friend std::ostream &operator<<(std::ostream &out, const PokerCard &p){
    	int d=(p.face==14)? 1:p.face; 
        out<<"["<<d<<" of "<<p.suit<<"]";
        return out;
    }
    //Please finish the comparison operator override
    //compare face first, if the same then compare suit 
    //請完成比較運算子多載
    //先比較 face ， face 一樣再比較 suit
    //1 > 13 > 12 > 11 > 10 > 9 > 8 > 7 > 6 > 5 > 4 > 3 > 2
    //spade > heart > diamond > club
    bool operator>(const PokerCard &b) const{
		if(face!=b.face) return face>b.face;
		else return sValue(suit)>sValue(b.suit);
    }
    bool operator<(const PokerCard &b) const{
		if(face!=b.face) return face<b.face;
		else return sValue(suit)<sValue(b.suit);
    }
    bool operator==(const PokerCard &b) const{
        return face==b.face&&suit==b.suit;
    }
private:
    std::string suit;
    int face;
    int sValue(const std::string &s) const{
    	if(s=="spade") return 4;
        else if(s=="heart") return 3;
        else if(s=="diamond") return 2;
        else if(s=="club") return 1;
        else return 0;
	}
};

int main(){
    PokerCard card1("spade", 1), card2("diamond", 1), card3("spade", 1);
    cout<<"Card 1: "<<card1<<" Card 2: "<<card2<<" Card 3: "<<card3<<endl;
    cout<<"Comparison Results: "<<(card1 > card2)<<" "<<(card2 < card3)<<" "<<(card1 == card3)<<endl;
	return 0;
}

