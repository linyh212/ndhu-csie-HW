#include <iostream>
using namespace std;
 
class numberguessing{
private:
	int value;
	int times;
public:
	numberguessing(): times(0){}
 	void Set(int v){
		value=v;
		times=0;
	}
	int Guesser(int v){
		times++;
		if(v<value) return -1;
		else if(v>value) return 1;
		else return 0;
	}
	int Time(){
		return times;
	}
	void Reset(){
		times=0;
	}
};

/*int main(){
    numberguessing game;
    game.Set(50);
	int guess;
    int result;
    do{
        cout<<"Enter your guess: ";
        cin>>guess;
        result=game.Guesser(guess);
        if(result==-1) cout<<"The actual number is higher.\n";
        else if(result==1) cout<<"The actual number is lower.\n";
        else cout<<"Correct guess!\n";
    }while(result!=0);
    cout<<"You guessed the number in "<<game.Time()<<" attempts.\n";
    return 0;
}*/
