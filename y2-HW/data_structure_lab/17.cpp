//PREPEND BEGIN
/*some text*/
//PREPEND END

//TEMPLATE BEGIN
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <ctime>

template<class T>
class Node{
public:
	Node(){
		data=new T;
	}
	Node(T d){
		data=new T;
		(*data)=d;
	}
	Node &operator=(T d){
		(*data)=d;
		return *this;
	}
	friend std::ostream &operator<<(std::ostream &out, Node n){
		out<<*(n.data);
		return out;
	}
	friend std::ostream &operator<<(std::ostream &out, Node *n){
		out<<*(n->data);
		return out;
	}
	void setData(T d){
		*data=d;
	}
	T &getData() const{
		return *data;
	}
protected:
	T *data;
};

template<class T>
class BinaryTreeNode: public Node<T>{
public:
	BinaryTreeNode(): Node<T>(){
		left=NULL;
		right=NULL;
	}
	BinaryTreeNode(T d): Node<T>(d){
		left=NULL;
		right=NULL;
	}
	BinaryTreeNode(BinaryTreeNode<T> *l, BinaryTreeNode<T> *r): Node<T>(){
		left=l;
		right=r;
	}
	BinaryTreeNode(T d, BinaryTreeNode<T> *l, BinaryTreeNode<T> *r): Node<T>(d){
		left=l;
		right=r;
	}
	void setLeft(BinaryTreeNode<T> *l){
		left=l;
	}
	void setRight(BinaryTreeNode<T> *r){
		right=r;
	}
	BinaryTreeNode<T> *&getLeft(){
		return left;
	}
	BinaryTreeNode<T> *&getRight(){
		return right;
	}
	bool operator>(BinaryTreeNode<T> n){
		if(*(this->data)>*(n.data))
			return true;
		return false;
	}
	bool operator==(BinaryTreeNode<T> n){
		if(*(this->data)==*(n.data))
			return true;
		return false;
	}
private:
	BinaryTreeNode<T> *left, *right;
};

template<class T>
class AVLTree{
private:
	BinaryTreeNode<T> *root;
	void inorder(BinaryTreeNode<T> *cur, int n){
		if(cur==NULL)
			return;
	    inorder(cur->getRight(), n+1);
	    int j;
	    for(j=0; j<n; j++)
			std::cout<<"  ";
	    std::cout<<cur<<std::endl;
	    inorder(cur->getLeft(), n+1);
	}	
public:
	AVLTree(){
		root=nullptr;
	}
	void insert(T d){
		root=insNode(root, d);
	}
	void inorder(){
		inorder(root, 0);
	}
	/*void display() const { printTree(root, "", false); }*/
private:
	int height(BinaryTreeNode<T> *node){
		if(node==nullptr) return 0;
		return std::max(height(node->getLeft()), height(node->getRight()))+1;
	}
	int getBalanceFactor(BinaryTreeNode<T> *node){
		if(node==nullptr) return 0;
		return height(node->getLeft())-height(node->getRight());
	}
	BinaryTreeNode<T>* rotateLeft(BinaryTreeNode<T> *x){
		BinaryTreeNode<T> *y=x->getRight();
		BinaryTreeNode<T> *T2=y->getLeft();
		y->setLeft(x);
		x->setRight(T2);
		return y;
	}
	BinaryTreeNode<T>* rotateRight(BinaryTreeNode<T> *y){
		BinaryTreeNode<T> *x=y->getLeft();
		BinaryTreeNode<T> *T2=y->getRight();
		x->setRight(y);
		y->setLeft(T2);
		return x;
	}
	BinaryTreeNode<T>* insNode(BinaryTreeNode<T> *node, T d){
		if(node==nullptr) return new BinaryTreeNode<T>(d);
		if(d<node->getData()) node->setLeft(insNode(node->getLeft(), d));
		else if(d>node->getData()) node->setRight(insNode(node->getRight(), d));
		else return node;
		int bal=getBalanceFactor(node);
		if(bal>1&&d<node->getLeft()->getData()) return rotateRight(node);
		if(bal<-1&&d>node->getRight()->getData()) return rotateLeft(node);
		if(bal>1&&d>node->getLeft()->getData()){
			node->setLeft(rotateLeft(node->getLeft()));
			return rotateRight(node);
		}
		if(bal<-1&&d<node->getRight()->getData()){
			node->setRight(rotateRight(node->getRight()));
			return rotateLeft(node);
		}
		return node;	
	}
	/*void printTree(BinaryTreeNode<T>* node, std::string prefix, bool isLeft) const {
        if (!node) return;
        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");
        std::cout << node->getData() << std::endl;
        printTree(node->getLeft(), prefix + (isLeft ? "│   " : "    "), true);
        printTree(node->getRight(), prefix + (isLeft ? "│   " : "    "), false);
    }*/
};

int main(){
	AVLTree<int> *tree=new AVLTree<int>();
	srand(0);
	int j, k, i;
	for(j=0; j<20; j++){
		tree->insert(rand()%100);
		tree->inorder();
	}
}

/*int main() {
    AVLTree<int> tree;
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < 10; ++i) {
        int value = rand() % 100;
        std::cout << "Inserting: " << value << std::endl;
        tree.insert(value);
        std::cout << "AVL Tree structure:" << std::endl;
        tree.display();
        std::cout << "--------------------------" << std::endl;
    }
    return 0;
}*/
