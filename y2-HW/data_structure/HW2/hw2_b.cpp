#include <bits/stdc++.h>
using namespace std;

template<typename KeyType>
class Stack{
private:
    stack<KeyType> data;
public:
    void push(const KeyType& value){
        data.push(value);
    }
    void pop(const KeyType& value){
        if(!data.empty()) data.pop(value);
    }
    KeyType top() const{
        if(!data.empty()) return data.top();
        else throw runtime_error("Stack is empty.");
    }
    bool empty() const{
        return data.empty();
    }
    void moveToTop(const KeyType& x){
        stack<KeyType> tempStack;
        bool find=false;
        while(!data.empty()){
            if(data.top()==x){
                find=true;
                break;
            }
            tempStack.push(data.top());
            data.pop();
        }
        if(find){
            KeyType target=data.top();
            data.pop();
            while(!tempStack.empty()){
                data.push(tempStack.top());
                tempStack.pop();
            }
            data.push(target);
        }
        else{
            while(!tempStack.empty()){
                data.push(tempStack.top());
                tempStack.pop();
            }
        }
    }
    void print() const{
        stack<KeyType> temp=data;
        while(!temp.empty()){
            cout<<temp.top()<<" ";
            temp.pop();
        }
        cout<<endl;
    }
};

template<typename KeyType>
class Queue{
private:
    queue<KeyType> data;
public:
    void enqueue(const KeyType& value){
        data.push(value);
    }
    void dequeue(const KeyType& value){
        if(!data.empty()) data.pop();
    }
    KeyType front() const{
        if(!data.empty()) return data.front();
        else throw runtime_error("Queue is empty.");
    }
    bool empty() const{
        return data.empty();
    }
    void moveToFront(const KeyType& x){
        queue<KeyType> tempQueue;
        bool find=false;
        while(!data.empty()){
            if(data.front()==x&& !find) find=true;
            else tempQueue.push(data.front());
            data.pop();            
        }
        if(find) data.push(x);
        while(!tempQueue.empty()){
            data.push(tempQueue.front());
            tempQueue.pop();
        }
    }
    void moveToRear(const KeyType& x){
        queue<KeyType> tempQueue;
        bool find=false;
        while(!data.empty()){
            if(data.front()==x&& !find) find=true;
            else tempQueue.push(data.front());
            data.pop();            
        }
        while(!tempQueue.empty()){
            data.push(tempQueue.front());
            tempQueue.pop();
        }
        if(find) data.push(x);
    }
    void print() const{
        queue<KeyType> temp=data;
        while(!temp.empty()){
            cout<<temp.front()<<" ";
            temp.pop();
        }
        cout<<endl;
    }
};

/*int main(){
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    cout<<"Original Stack:"<<endl;
    stack.print();
    stack.moveToTop(2);
    cout<<"After moveToTop(2):"<<endl;
    stack.print();
    stack.moveToTop(5);
    cout<<"After moveToTop(5):"<<endl;
    stack.print();

    Queue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);
    queue.enqueue(4);
    cout<<"\nOriginal Queue:"<<endl;
    queue.print();
    queue.moveToFront(3);
    cout<<"After moveToFront(3):"<<endl;
    queue.print();
    queue.moveToRear(2);
    cout<<"After moveToRear(2):"<<endl;
    queue.print();
    queue.moveToFront(5);
    cout<<"After moveToFront(5):"<<endl;
    queue.print();
    return 0;
}*/