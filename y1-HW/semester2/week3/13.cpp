#include <iostream>
#include <vector>
#include <string>
using namespace std;

class SquareMatrix{
private:
	vector< vector<int> > matrix;
public:
	void rotate_right(){
		if(matrix.empty()){
			cout<<"No element in matrix can be rotated."<<endl;
			return;
		}
		int n=matrix.size();
		vector< vector<int> > rotated(n, vector<int>(n));
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				rotated[j][n-1-i]=matrix[i][j];
			}
		}
		matrix=rotated;
	}
	void rotate_left(){
		if(matrix.empty()){
			cout<<"No element in matrix can be rotated."<<endl;
			return;
		}
		int n=matrix.size();
		vector< vector<int> > rotated(n, vector<int>(n));
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				rotated[n-1-j][i]=matrix[i][j];
			}
		}
		matrix=rotated;
	}
	void print_m(){
		if(matrix.empty()){
			cout<<"No element in matrix can be printed."<<endl;
			return;
		}
		for(int i=0; i<matrix.size(); i++){
			for(int j=0; j<matrix[i].size(); j++){
				cout<<matrix[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}
	void scan_m(){
		int n;
		cin>>n;
		matrix.clear();
		matrix.resize(n, vector<int>(n));
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				cin>>matrix[i][j];
			}	
		}
	}
};

int main(){
	SquareMatrix matrix;
	string instruction;
	while(cin>>instruction){
		if(instruction=="rotate"){
			cin>>instruction;
			if(instruction=="right") matrix.rotate_right();
			else if(instruction=="left") matrix.rotate_left();
			else cout<<"unknown"<<endl;
		}
		else if(instruction=="print") matrix.print_m();
		else if(instruction=="scan") matrix.scan_m();
		else if(instruction=="stop") break;
		else cout<<"unknown"<<endl;
	}
}
