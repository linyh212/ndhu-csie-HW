#include <bits/stdc++.h>
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
    BSTNode(const Element<T>& x): data(x.key), LeftChild(nullptr), RightChild(nullptr){}
    BSTNode* LeftChild;
    BSTNode* RightChild;
    T data;
};

template<class T>
class BST{
public:
    BST(): root(nullptr){}
    BSTNode<T>* Search(const Element<T>& x);
    BSTNode<T>* Search(BSTNode<T>* b, const Element<T>& x);
    BSTNode<T>* IterSearch(const Element<T>& x);
    void Insert(const Element<T>& x);
    int height();
    int height(BSTNode<T>* node);
    int weight();
    int weight(BSTNode<T>* node);
    int heightBF();
    int heightBF(BSTNode<T>* node);
    int weightBF();
    int weightBF(BSTNode<T>* node);
private:
    BSTNode<T>* root;
};

template <class T>
BSTNode<T>* BST<T>::Search(const Element<T>& x){
    return Search(root, x);
}

template <class T>
BSTNode<T>* BST<T>::Search(BSTNode<T>* b, const Element<T>& x){
    if(!b) return nullptr;
    if(x.key==b->data) return b;
    if(x.key<b->data) return Search(b->LeftChild, x);
    return Search(b->RightChild, x);
}

template <class T>
BSTNode<T>* BST<T>::IterSearch(const Element<T>& x){
    for(BSTNode<T>* t=root; t;){
        if(x.key==t->data) return t;
        if(x.key<t->data)
            t=t->LeftChild;
        else t=t->RightChild;
    }
    return nullptr;
}

template <class T>
void BST<T>::Insert(const Element<T>& x){
    BSTNode<T>* p=root, * pp=nullptr;
    while(p){
        pp=p;
        if(x.key<p->data) p=p->LeftChild;
        else if(x.key>p->data) p=p->RightChild;
        else return;
    }
    p=new BSTNode<T>(x);
    if(root){
        if(x.key<pp->data) pp->LeftChild=p;
        else pp->RightChild = p;
    }
    else root=p;
}

template <class T>
int BST<T>::height(){
    return height(root);
}

template <class T>
int BST<T>::height(BSTNode<T>* node){
    if (!node) return 0;
    return 1 + max(height(node->LeftChild), height(node->RightChild));
}

template <class T>
int BST<T>::weight(){
    return weight(root);
}

template <class T>
int BST<T>::weight(BSTNode<T>* node){
    if(!node) return 0;
    return 1+weight(node->LeftChild)+weight(node->RightChild);
}

template <class T>
int BST<T>::heightBF(){
    return heightBF(root);
}

template <class T>
int BST<T>::heightBF(BSTNode<T>* node){
    if (!node) return 0;
    return height(node->LeftChild)-height(node->RightChild);
}

template <class T>
int BST<T>::weightBF(){
    return weightBF(root);
}

template <class T>
int BST<T>::weightBF(BSTNode<T>* node){
    if(!node) return 0;
    return weight(node->LeftChild)-weight(node->RightChild);
}

/*int main(){
    BST<int> tree;
    Element<int> e1{10}, e2{5}, e3{15}, e4{3}, e5{7};

    tree.Insert(e1);
    tree.Insert(e2);
    tree.Insert(e3);
    tree.Insert(e4);
    tree.Insert(e5);

    cout<<"Height of tree: "<<tree.height()<<endl;
    cout<<"Weight of tree: "<<tree.weight()<<endl;
    cout<<"Height Balance Factor: "<<tree.heightBF()<<endl;
    cout<<"Weight Balance Factor: "<<tree.weightBF()<<endl;

    return 0;
}*/
