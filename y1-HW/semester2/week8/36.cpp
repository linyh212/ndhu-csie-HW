#include <bits/stdc++.h>
using namespace std;

template<class T>
class LinkedList{
public:
    class Node{
	public:
		Node(): next(nullptr), data(0){}
		Node(int d): next(nullptr), data(d){}
		Node(Node *n): next(n), data(0){}
		Node(Node *n, int d): next(n), data(d){}
		T data;
		Node *next;
    };
	LinkedList(): head(NULL){}
	~LinkedList(){
		while(head!=NULL){
			Node *nextDel=head;
			head=head->next;
			delete nextDel;
		}
	}
	bool addFromHead(int d){
		Node *newNode=new Node(d);
		if(newNode==NULL) return false;
		newNode->next=head;
		head=newNode;
		return true;
	}
	bool addFromTail(int d){
		Node *newNode=new Node(d);
		if(newNode==NULL) return false;
		if(head==NULL){
			head=newNode;
			return true;
		}
		Node *ptr;
		for(ptr=head; ptr->next!=NULL; ptr=ptr->next) continue;
		ptr->next=newNode;
        return true;
    }
    bool removeFromHead(){
        if(head==NULL) return false;
		Node *ptr=head->next;
		delete head;
		head=ptr;
		return true;
	}
	bool removeFromTail(){
		if(head==NULL) return false;
		if(head->next==NULL){
			delete head;
			head=NULL;
			return true;
        }
		Node *ptr;
		for(ptr=head; ptr->next!=NULL&&ptr->next->next!=NULL; ptr=ptr->next) continue;
		delete ptr->next;
		ptr->next=NULL;
		return true;
	}
	friend ostream &operator<<(ostream &o, LinkedList *l){
		o<<"{";
		Node *ptr;
		for(ptr=l->head; ptr!=NULL; ptr=ptr->next){
			o<<ptr->data;
			if(ptr->next!=NULL) o<< ", ";
		}
		o<<"}\n";
		return o;
	}
	size_t getSize() const{
		size_t n=0;
		Node *ptr;
		for(ptr=head; ptr!=NULL; ptr=ptr->next) n++;
		return n;
	}
	int searchData(int d){
		Node *n=head;
		int index=0;
		while(n){
			if(n->data==d) return index;
			n=n->next;
			index++;
		}
		return -1;
	}
	int operator[](int i){
		Node *n=head;
		while(i--){
			n=n->next;
		}
		return n->data;
	}
	protected:
        Node *head ;
} ;

template<class T>
class Set: private LinkedList<T>{
public:
    Set<T>(): LinkedList<T>(){}
    ~Set(){}
    bool addElement(T d){
        if(LinkedList<T>::searchData(d)!=-1) return false;
        LinkedList<T>::addFromTail(d);
        return true;
    }
    friend ostream &operator<<(ostream &o, Set<T> *s){
        o<<(LinkedList<T>*)s;
        return o;
    }
    Set<T>* operator+(Set<T> *s) const{
        Set<T>* result=new Set<T>();
        typename LinkedList<T>::Node* current=s->head;
        while(current){
            result->addElement(current->data);
            current=current->next;
        }
        current=this->head;
        while(current){
            result->addElement(current->data);
            current=current->next;
        }
        return result;
    }
    Set<T>* operator-(Set<T> *s) const{
        Set<T>* result=new Set<T>();
        typename LinkedList<T>::Node* current=this->head;
        while(current){
            if(s->LinkedList<T>::searchData(current->data)==-1) 
                result->addElement(current->data);
            current=current->next;
        }
        return result;
    }
    Set<T>* operator*(Set<T> *s) const{
        Set<T>* result=new Set<T>();
        typename LinkedList<T>::Node* current=this->head;
        while(current){
            if(s->LinkedList<T>::searchData(current->data)!=-1) 
                result->addElement(current->data);
            current=current->next;
        }
        return result;
    }
};

int main() {
    Set<int>* s1 = new Set<int>();
    s1->addElement(1);
    s1->addElement(3);
    s1->addElement(2);

    Set<int>* s2 = new Set<int>();
    s2->addElement(3);
    s2->addElement(4);
    s2->addElement(5);

    cout << s1 << s2; // Output: {1, 2, 3}, {3, 4, 5}
    
    Set<int>* sUnion = *s1 + s2;
    Set<int>* sDiff = *s1 - s2;
    Set<int>* sInter = *s1 * s2;

    cout << sUnion << sDiff << sInter; // Output: {1, 2, 3, 4, 5}, {1, 2}, {3}

    delete s1;
    delete s2;
    delete sUnion;
    delete sDiff;
    delete sInter;

    return 0;
}

