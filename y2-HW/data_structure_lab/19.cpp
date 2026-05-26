#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

template<class T>
class BinaryThreadedTreeNode{
private:
public:
    T *data;
    BinaryThreadedTreeNode *left, *right, *thread;
    BinaryThreadedTreeNode(T d): right(nullptr), left(nullptr), isThread(false){
        data=new T{d};
    }
    BinaryThreadedTreeNode(): data(nullptr), right(nullptr), left(nullptr), isThread(false){}
    bool isThread;
};

template<class T>
class BinarySearchThreadedTree: BinaryThreadedTreeNode<T>{
private:
    void CreateThread(BinaryThreadedTreeNode<T>* &prev, BinaryThreadedTreeNode<T>* cur){
        if(!cur) return;
        CreateThread(prev, static_cast<BinarySearchThreadedTree<T>*>(cur->left));
        if(prev&&!prev->right){
            prev->right=cur;
            prev->isThread=true;
        }
        prev=cur;
        if(!cur->isThread) CreateThread(prev, static_cast<BinarySearchThreadedTree<T>*>(cur->right));
    }
public:
    BinarySearchThreadedTree(): BinaryThreadedTreeNode<T>(){}
    BinarySearchThreadedTree *insert(T d){
        if(!this->data){
            this->data=new T{d};
            this->left=new BinarySearchThreadedTree();
            this->right=new BinarySearchThreadedTree();
            return this;
        }
        else{
            if(*this->data>d)
                return (static_cast<BinarySearchThreadedTree<T> *>(this->left))->insert(d);
            else
                return (static_cast<BinarySearchThreadedTree<T> *>(this->right))->insert(d);
        }
    }    
    void Thread(){
        BinaryThreadedTreeNode<T>* prev=nullptr;
        CreateThread(prev, this);
    }
    void print(){
        BinaryThreadedTreeNode<T>* current=this;
        while(current&&current->left){
            current=static_cast<BinarySearchThreadedTree<T>*>(current->left);
        }
        while(current){
            if(current->data) cout<< *(current->data)<<" ";
            if(current->isThread) current=current->right;
            else{
                current=static_cast<BinarySearchThreadedTree<T>*>(current->right);
                while(current&&current->left){
                    current=static_cast<BinarySearchThreadedTree<T>*>(current->left);
                }
                
            }
        }
        cout<<endl;
    }
};

int main(){
    int j;
    BinarySearchThreadedTree<int> bstt;
    srand(time(nullptr));
    for(j=0; j<10; j++)
        bstt.insert(rand()%100);
    bstt.Thread();
    bstt.print();
}
