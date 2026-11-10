#include <bits/stdc++.h>
using namespace std;

class LinkedList{
public:
    class Node{
	public:
		Node(): next(nullptr), data(0){}
		Node(int d): next(nullptr), data(d){}
		Node(Node *n): next(n), data(0){}
		Node(Node *n, int d): next(n), data(d){}
		int data;
		Node *next;
	};
	LinkedList(): head(nullptr){}
	~LinkedList(){
		while(head){
			Node* temp=head;
			head=head->next;
			delete temp;
		}
	}
	bool addFromHead(int d){
		Node* newNode=new Node(d);
		if(!newNode) return false;
		newNode->next=head;
		head=newNode;
		return true;
	}
	bool addFromTail(int d){
		Node* newNode=new Node(d);
		if(!newNode) return false;
		if(!head) head=newNode;
		else{
			Node* current=head;
			while(current->next){
				current=current->next;
			}
			current->next=newNode;
		}
		return true;
	}
	bool removeFromHead(){
		if(!head) return false;
		Node* temp=head;
		head=head->next;
		delete temp;
		return true;
	}
	bool removeFromTail(){
		if(!head) return false;
		if(!head->next){
			delete head;
			head=nullptr;
		}
		else{
			Node* current=head;
			while(current->next->next){
				current=current->next;
			}
			delete current->next;
			current->next=nullptr;
		}
		return true;
	}
	friend ostream &operator<<(ostream &o, LinkedList *l){
		o<<"{";
		Node* current=l->head;
		while(current){
			o<<current->data;
			if(current->next) o<<", ";
			current=current->next;
		}
		o<<"}"<<"\n";
		return o;
	}
	size_t getSize() const{
		size_t size=0;
		Node* current=head;
		while(current){
			size++;
			current=current->next;
		}
		return size;
	}
	protected: Node *head;
};

int main(){
    LinkedList ll;
    ll.addFromHead(4);
    ll.addFromTail(5);
    cout<<&ll; // Output:{4, 5}
    ll.removeFromHead();
    cout<<&ll; // Output:{5}
    ll.removeFromTail();
    cout<<&ll; // Output:{}
    return 0;
}

