#include <iostream>
#include <cstdlib>
#include <ctime>

#define SIZE 100

using namespace std;

class Node{
public:
	Node(){
		next=NULL;
		pre=NULL;
	}
	Node(int n){
		data=n;
		next=NULL;
		pre=NULL;
	}
	int getData(){return data;}
	Node *getNext(){return next;}
	Node *getPre(){return pre;}
	void setData(int d){data=d;}
	void setNext(Node *n){next=n;}
	void setPre(Node *p){ pre=p;}
private:
	int data;
	Node *next, *pre; 
};

class List{
public:
	List(){list=NULL;}
	List(int n){generate(n);}	
	void generate(int n){
		int j;
		list=NULL;
		for(j=0; j<n; j++)
			generate();
	}
	void generate(){
		Node *buf=new Node(rand());
		buf->setNext(list);
		if(list!=NULL)
			list->setPre(buf);
		list=buf;
	}
	void swap(Node *a, Node *b){
		if(a==b) return;
		Node *preA=a->getPre();
		Node *preB=b->getPre();
		Node *nextA=a->getNext();
		Node *nextB=b->getNext();
		if(nextA==b){
			a->setNext(nextB);
			if(nextB) nextB->setPre(a);
			b->setPre(preA);
			if(preA) preA->setNext(b);
			b->setNext(a);
			a->setPre(b);
		}
		else if(nextB==a){
			b->setNext(nextA);
			if(nextA) nextA->setPre(b);
			a->setPre(preB);
			if(preB) preB->setNext(a);
			b->setPre(a);
			a->setNext(b);
		}
		else{
			if(preA) preA->setNext(b);
			if(preB) preB->setNext(a);
			if(nextA) nextA->setPre(b);
			if(nextB) nextB->setPre(a);
			a->setNext(nextB);
			a->setPre(preB);
			b->setNext(nextA);
			b->setPre(preA);
		}
		if(list==a) list=b;
		else if(list==b) list=a;
	}
	void bubbleSort(){
		if(list==NULL) return;
		bool swapped=true;
		while(swapped){
			swapped=false;
			Node *current=list;
			while(current->getNext()!=NULL){
				if(current->getData()>current->getNext()->getData()){
					swap(current, current->getNext());
					swapped=true;
				}
				else{
					current=current->getNext();
				}
			}
		}
	}
	void selectionSort(){
		if(list==NULL) return;
		Node *current=list;
		while(current!=NULL){
			Node *min=current;
			Node *next=current->getNext();
			while(next!=NULL){
				if(next->getData()<min->getData()){
					min=next;
				}
				next=next->getNext();
			}
			if(min!=current){
				swap(current, min);
				current=min;
			}
			current=current->getNext(); 
		}
	}
	void insertionSort(){
		if(list==NULL) return;
		Node *sort=list;
		Node *current=list->getNext();
		sort->setNext(NULL);
		while(current!=NULL){
			Node *next=current->getNext();
			if(sort->getData()>current->getData()){
				current->setNext(sort);
				sort->setPre(current);
				sort=current;
				sort->setPre(NULL);
			}
			else{
				Node *temp=sort;
				while(temp->getNext()!=NULL&&temp->getNext()->getData()<current->getData()){
					temp=temp->getNext();
				}
				current->setNext(temp->getNext());
				if(temp->getNext()!=NULL) temp->getNext()->setPre(current);
				temp->setNext(current);
				current->setPre(temp);
			}
			current=next;
		}
		list=sort;
	}
	void print(){
		Node *cur=list;
		while(cur!=NULL){
			cout<<cur->getData()<<" ";
			cur=cur->getNext();
		}
		cout<<endl;
	}
private:
	Node *list;
};

int main(){
	srand(time(NULL));
	List *l=new List(10);
	l->print();
	l->bubbleSort();
	l->print();
	cout<<endl;
	
	l=new List(10);
	l->print();
	l->selectionSort();
	l->print();
	cout<<endl;
	
	l=new List(10);
	l->print();
	l->insertionSort();
	l->print();
}

