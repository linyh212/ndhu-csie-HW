#include <iostream>
#include <cmath>
using namespace std;

class LicensePlate{
public:
	bool dig(char num[4]){
		int a=0;
		for(int i=0; i<4; i++){
			if(num[i]=='4') a++;			
		}
		return !(a>0);
	}	
};

int main(){
	char x[4];
	for(int i=0; i<4; i++){
		cin>>x[i];
	}	
	LicensePlate license_plate;
	bool y=license_plate.dig(x);
	if(y) cout<<"No"<<endl;
	else cout<<"Yes"<<endl;
}
