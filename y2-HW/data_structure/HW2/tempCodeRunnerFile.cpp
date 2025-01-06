#include <bits/stdc++.h>
using namespace std;

template<typename KeyType>
class QueueStack{
private:
    vector<KeyType> data;
    size_t maxSize;
public:
    QueueStack(size_t maxSize=50): maxSize(maxSize){}
    bool IsFull() const{
        return data.size()>=maxSize;
    }
    bool IsEmpty(){
        return data.empty();
    }
    void push(const KeyType& item){
        if(IsFull()){
            cerr<<"QueueStack is full.\n";
            return;
        }
        data.push_back(item);
    }
    KeyType*pop(KeyType& x){
        if(IsEmpty()){
            cerr<<"QueueStack is empty.\n";
            return nullptr;
        }
        x=data.back();
        data.pop_back();
        return &x;
    }
    void addRear(const KeyType& item){
        if(IsFull()){
            cerr<<"QueueStack is full.\n";
            return;
        }
        data.push_back(item);
    }
    KeyType*deleteFront(KeyType& x){
        if(IsEmpty()){
            cerr<<"QueueStack is empty.\n";
            return nullptr;
        }
        x=data.front();
        data.erase(data.begin());
        return &x;
    }
    void print() const{
        if(IsEmpty()){
            cout<<"[Empty]\n";
            return;
        }
        cout<<"[ ";
        for(const auto& item: data){
            cout<<item<<" ";
        }
        cout<<"]\n";
    }
};

int main(){
    QueueStack<int> qs(5);
    cout<<"Initial state:\n";
    qs.print();
    
    cout<<"\nTesting stack operations...\n";
    qs.push(10);
    cout<<"After push(10): ";
    qs.print();
    qs.push(20);
    cout<<"After push(20): ";
    qs.print();
    qs.push(30);
    cout<<"After push(30): ";
    qs.print();    
    int value;
    qs.pop(value);
    cout<<"After pop(): removed "<<value<<", QueueStack: ";
    qs.print();
    
    cout<<"\nTesting queue operations...\n";
    qs.addRear(40);
    cout<<"After addRear(40): ";
    qs.print();
    qs.addRear(50);
    cout<<"After addRear(50): ";
    qs.print();
    qs.deleteFront(value);
    cout<<"After deleteFront(): removed "<<value<<", QueueStack: ";
    qs.print();
    qs.deleteFront(value);
    cout<<"After deleteFront(): removed "<<value<<", QueueStack: ";
    qs.print();
    
    cout<<"\nTesting edge cases...\n";
    qs.pop(value);
    qs.deleteFront(value);
    qs.push(60);
    qs.push(70);
    qs.push(80);
    qs.push(90);
    qs.push(100);
    cout<<"After filling to capacity: ";
    qs.print();
    qs.push(110);
    return 0;
}