#include <bits/stdc++.h>
using namespace std;

template<class T>
class Set{
public:
	Set(): count(0){}
	Set(const Set &a){
		count=a.getC();
		for(int i=0; i<count; i++)
			element[i]=a.getE()[i];
	}
	int getC() const{
		return count;
	}
	const T *getE() const{
		return element;
	}
	int add(const T &a){
		if(count==100) return 0;
		for(int i=0; i<count; i++){
			if(element[i]==a) return 0;
		}
		element[count]=a;
		count++;
		return 1;
	}
	int add(const Set<T> &a){
		int added=0;
		for(int i=0; i<a.count; i++){
			added+=add(a.element[i]);	
		}
		return added;
	}
	void display(){
		int j;
		for(j = 0;j < count;j ++)
			std::cout<<element[j]<<" ";
    }
    Set operator+(const Set &a) const{
    	Set temp(*this);
    	temp.add(a);
    	return temp;
	}
	Set operator-(const Set &a) const{
    	Set temp;
    	for(int i=0; i<a.getC(); i++){
    		for(int j=0; j<count; j++){
    			if(element[j]==a.getE()[i]) temp.add(element[j]);
			}
		}
    	return temp;
	}
private:
	T element[100];
	int count;
};
