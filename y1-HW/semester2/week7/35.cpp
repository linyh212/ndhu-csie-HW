#include <bits/stdc++.h>
using namespace std;

/*class Stack: public Container{
private:
	vector<int> n;
public:
	void push(int temp) override{
		n.push_back(temp);
	}
	void pop() override{
		if(empty()) throw out_of_range("out_of_range");
		n.pop_back();
	}
	int top() const override{
		if(empty()) throw out_of_range("out_of_range");
		return n.back();
	}
    bool empty() const override{
		return n.empty();
	}
};
class PriorityQueue: public Container{
private:
	vector<int> n;
public:
	void push(int tmp) override{
		n.push_back(tmp);
	}
	void pop() override{
		if(empty()) throw out_of_range("out_of_range");
		auto max=max_element(n.begin(), n.end());
		n.erase(max);
	}
	int top() const override{
		if(empty()) throw out_of_range("out_of_range");
		auto max= *max_element(n.begin(), n.end());
		return max();
	}
    bool empty() const override{
		return n.empty();
	}
};*/

int judge(Container *c){
	if(dynamic_cast<Stack *>(c)) return 1;
	else return 2;
}

