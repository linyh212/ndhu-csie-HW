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
	void setPre(Node *p){pre=p;}
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
	void bubbleSort(){
		if(list==NULL) return;
		bool swapped=true;
		Node *current;
		Node *last=NULL;
		while(swapped){
			swapped=false;
			current=list;
			while(current->getNext()!=last){
				if(current->getData()>current->getNext()->getData()){
					int temp=current->getData();
					current->setData(current->getNext()->getData());
					current->getNext()->setData(temp);
					swapped=true;
				}
				current=current->getNext();
			}
			last=current;
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
				int temp=current->getData();
				current->setData(min->getData());
				min->setData(temp);
			}
			current=current->getNext(); 
		}
	}
	void insertionSort(){
		if(list==NULL) return;
		Node *sort=NULL;
		Node *current=list;
		while(current!=NULL){
			Node *next=current->getNext();
			if(sort==NULL||sort->getData()>=current->getData()){
				current->setNext(sort);
				if(sort!=NULL) sort->setPre(current);
				sort=current;
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
