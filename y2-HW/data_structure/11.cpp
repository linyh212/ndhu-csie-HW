#include <iostream>
#include <string>

using namespace std;

template<class T>
class BinarySearchTree{
private:
	struct Node{
		T data;
		Node *left, *right;
		Node(T value): data(value), left(nullptr), right(nullptr){}
	};
	void inorder(Node *cur){
	    if(!cur)            
			return;     
		inorder(cur->left);
		cout<<cur->data<<" ";
		inorder(cur->right);
	}
	Node *root;
public:
	BinarySearchTree(): root(nullptr){}
	void insertElement(T d){
		Node *newNode=new Node(d);
		if(!root){
			root=newNode;
			return;
		}
		Node *current=root, *parent=nullptr;
		while(current){
			parent=current;
			if(d<current->data) current=current->left;
			else if(d>current->data) current=current->right;
			else{
				delete newNode;
				return;
			}
		}
		if(d<parent->data) parent->left=newNode;
		else parent->right=newNode;
	}
	void print(){
		inorder(root);
		cout<<endl;
	}
	bool search(T d){
		Node *current=root;
		while(current){
			if(d==current->data) return true;
			else if(d<current->data) current=current->left;
			else current=current->right;
		}
		return false;
	}
	int height(){
		return h(root);
	}
private:
	int h(Node *node){
		if(!node) return -1;
		int left=h(node->left);
		int right=h(node->right);
		return max(left, right)+1;
	}
};

int main(){
	int data;
	string command;
	BinarySearchTree<int> *bst=new BinarySearchTree<int>();
	while(true){
		cin>>command;
		if(command=="insert"){
			cin>>data;
			bst->insertElement(data);
		}
		else if(command=="search"){
			cin>>data;
			if(bst->search(data))
				cout<<"true"<<endl;
			else
				cout<<"false"<<endl;
		}
		else if(command=="height"){
			cout<<bst->height()<<endl;
		}
		else if(command=="print"){
			bst->print();
		}
		else if(command=="exit"){
			break;
		}
	}
}

