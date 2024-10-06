#include <bits/stdc++.h>
using namespace std;

template<class T>
class Set{
public:
	Set(): count(0){}
	Set(Set<T> &a){
		count=a.getC();
		for(int i=0; i<count; i++)
			element[i]=a.getE()[i];
	}
	int getC() const{
		return count;
	}
	T *getE() const{
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
private:
	T element[100];
	int count;
};
