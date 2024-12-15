#include <iostream>
using namespace std;

template <class T> class List;

template <class T>
class ListNode {
    friend class List <T>;
private:
    T data;
    ListNode<T> *link;
    ListNode(T val): data(val), link(nullptr){}
};

template <class T>
class List{
public:
	List(){first=last=0;}
	void Attach(T k);
    bool appearM(T k, int m);
    int replaceALL(T a1, T a2);
    void rotate(char direction, int k);
    List<T> subList(int start, int end);
    void print();
private:
	ListNode<T> *first;
	ListNode<T> *last;
};

template <class T>
void List<T>::Attach(T k){
    ListNode<T>* newnode=new ListNode<T>(k);
    if(first==0) 
	    first=last=newnode;
    else{
	    last->link=newnode;
	    last=newnode;
    }
};

template <class T>
bool List<T>::appearM(T k, int m){
    int count=0;
    ListNode<T>* current=first;
    while(current){
        if(current->data==k){
            count++;
            if(count>=m) return true;
        }
        current=current->link;
    }
    return false;
};

template <class T>
int List<T>::replaceALL(T a1, T a2){
    int rep=0;
    ListNode<T>* current=first;
    while(current){
        if(current->data==a1){
            current->data = a2;
            rep++;
        }
        current=current->link;
    }
    return rep;
};

template <class T>
void List<T>::rotate(char direction, int k){
    if(!first||k<=0) return;
    int n=0;
    ListNode<T>* current=first;
    while (current){
        n++;
        current=current->link;
    }
    k%=n;
    if(k==0) return;
    if(direction=='R') k=n-k;
    ListNode<T>* prev=nullptr;
    current=first;
    for(int i=0; i<k; i++){
        prev=current;
        current=current->link;
    }
    if(prev) prev->link=nullptr;
    last->link=first;
    first=current;
    while(last->link) last=last->link;
};

template <class T>
List<T> List<T>::subList(int start, int end){
    List<T> result;
    if(start>end||start<0) return result;
    int index=0;
    ListNode<T>* current=first;
    while (current&&index<=end){
        if(index>=start) result.Attach(current->data);
        current=current->link;
        index++;
    }
    return result;  
};

/*template <class T>
void List<T>::print(){
    ListNode<T>* current=first;
    while (current){
        cout<<current->data<<" ";
        current=current->link;
    }
    cout<<endl;
}

int main(){
    List<int> myList;
    myList.Attach(1);
    myList.Attach(2);
    myList.Attach(3);
    myList.Attach(2);
    myList.Attach(4);

    cout<<"Original List: ";
    myList.print();

    cout<<"Appear 2 at least 2 times: "<<(myList.appearM(2, 2)? "True":"False")<<endl;

    cout<<"Replacements of 2 with 5: "<<myList.replaceALL(2, 5)<<endl;
    cout<<"Modified List: ";
    myList.print();

    myList.rotate('R', 2);
    cout<<"List after rotating right by 2: ";
    myList.print();

    myList.rotate('L', 1);
    cout<<"List after rotating left by 1: ";
    myList.print();

    List<int> sublist=myList.subList(1, 3);
    cout<<"Sublist from index 1 to 3: ";
    sublist.print();

    return 0;
}*/
