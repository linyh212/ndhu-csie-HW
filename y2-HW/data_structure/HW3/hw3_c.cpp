#include <iostream>
using namespace std;

template <class T>
class Element{
public:
    T key;
};

template <class T>
class BSTNode{
public:
    BSTNode(): LeftChild(nullptr), RightChild(nullptr){}
    BSTNode(const Element<T> &x): data(x.key), LeftChild(nullptr), RightChild(nullptr){}
private:
    BSTNode *LeftChild;
    T data;
    BSTNode *RightChild;
    template<class U>
    friend class BST;
};

template <class T>
class BST{
public:
    BST(): root(nullptr), isReversed(false){}
    BSTNode<T>* Search(const Element<T>& x);
    BSTNode<T>* Search(BSTNode<T>* b, const Element<T>& x);
    BSTNode<T>* IterSearch(const Element<T>& x);
    void Insert(const Element<T>& x);
    void Delete(const Element<T>& x);
    void Reverse();
    void InOrderTraversal(BSTNode<T>* node) const;
    void PrintInOrder() const;
private:
    BSTNode<T> *root;
    bool isReversed;
    void ReverseHelper(BSTNode<T>* node);
};

template <class T>
BSTNode<T>* BST<T>::Search(const Element<T>& x){
    return Search(root, x);
}

template <class T>
BSTNode<T>* BST<T>::Search(BSTNode<T>* b, const Element<T>& x){
    if(!b) return nullptr;
    if(x.key==b->data) return b;
    if((x.key<b->data)!=isReversed) return Search(b->LeftChild, x);
    return Search(b->RightChild, x);
}

template <class T>
BSTNode<T>* BST<T>::IterSearch(const Element<T>& x){
    for(BSTNode<T>* t=root; t;){
        if(x.key==t->data) return t;
        if((x.key<t->data)!=isReversed) t=t->LeftChild;
        else t=t->RightChild;
    }
    return nullptr;
}

template <class T>
void BST<T>::Insert(const Element<T>& x){
    BSTNode<T> *p=root, *pp=nullptr;
    while(p){
        pp=p;
        if((x.key < p->data)!=isReversed) p=p->LeftChild;
        else if((x.key > p->data)!=isReversed) p=p->RightChild;
        else return;
    }
    p=new BSTNode<T>(x);
    if(root){
        if((x.key < pp->data)!=isReversed) pp->LeftChild=p;
        else pp->RightChild=p;
    }
    else root=p;
}

template <class T>
void BST<T>::Reverse(){
    isReversed=!isReversed;
    ReverseHelper(root);
}

template <class T>
void BST<T>::ReverseHelper(BSTNode<T>* node){
    if(!node) return;
    swap(node->LeftChild, node->RightChild);
    ReverseHelper(node->LeftChild);
    ReverseHelper(node->RightChild);
}

/*template <class T>
void BST<T>::InOrderTraversal(BSTNode<T>* node) const{
    if(!node) return;
    InOrderTraversal(node->LeftChild);
    cout<<node->data<<" ";
    InOrderTraversal(node->RightChild);
}

template <class T>
void BST<T>::PrintInOrder() const{
    InOrderTraversal(root);
    cout<<endl;
}

int main(){
    BST<int> tree;
    Element<int> e1{10}, e2{5}, e3{20}, e4{15}, e5{25};
    tree.Insert(e1);
    tree.Insert(e2);
    tree.Insert(e3);
    tree.Insert(e4);
    tree.Insert(e5);

    cout<<"In-order before reverse: ";
    tree.PrintInOrder();

    tree.Reverse();

    cout<<"In-order after reverse: ";
    tree.PrintInOrder();

    return 0;
}*/
