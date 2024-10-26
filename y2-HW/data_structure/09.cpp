#include <iostream>
#include <cmath>
#include <queue>

using namespace std;

template<class T>
class BinaryTreeInArray{
private:
	T *array;
	int height;
	int numOfElement;
	void resize(int size){
		T *temp=new T[numOfElement];
		int j;
		for(j=0; j<numOfElement; j++)
			temp[j]=array[j];
		delete array;
		array=new T[static_cast<int>(pow(2, height+1))-1];
		for(j=0; j<numOfElement; j++)
			array[j]=temp[j];
		height++;
		delete temp;
  }
public:
	BinaryTreeInArray(): array(nullptr), height(0), numOfElement(0){}
	void addElementAsCompleteTree(T data){
		int cap=pow(2, height)-1;
		if(numOfElement+1>cap)
			resize((cap+1)*2-1);
		array[numOfElement]=data;
		numOfElement++;
	}
	void displayInorder(){
		in(0);
	}
	void displayPreorder(){
		pre(0);
	}
	void displayPostorder(){
		post(0);
	}
private:
	void in(int index){
		if(index>=numOfElement) return;
		in(2*index+1);
		cout<<array[index]<<" ";
		in(2*index+2);
	}
	void pre(int index){
		if(index>=numOfElement) return;
		cout<<array[index]<<" ";
		pre(2*index+1);
		pre(2*index+2);
	}
	void post(int index){
		if(index>=numOfElement) return;
		post(2*index+1);
		post(2*index+2);
		cout<<array[index]<<" ";
	}
};

template<class T>
class BinaryTreeInLinkedList{
private:
	class TreeNode{
	private:
	public:
		TreeNode(T d): data(d), left(nullptr), right(nullptr){}
		TreeNode *left, *right;
		T data;
	};
	TreeNode *root;
	int numOfElement;
public:
	BinaryTreeInLinkedList(): root(nullptr), numOfElement(0){}
	void addElementAsCompleteTree(T data){
		TreeNode *newNode=new TreeNode(data);
		if(!root) root=newNode;
		else{
			queue<TreeNode *> q;
			q.push(root);
			while(!q.empty()){
				TreeNode *current=q.front();
				q.pop();
				if(!current->left){
					current->left=newNode;
					break;
				}
				else{
					q.push(current->left);
				}
				if(!current->right){
					current->right=newNode;
					break;
				}
				else{
					q.push(current->right);
				}
			}
		}
		numOfElement++;
	}
	void displayInorder(){
		in(root);
	}
	void displayPreorder(){
		pre(root);
	}
	void displayPostorder(){
		post(root);
	}
private:
	void in(TreeNode* node){
		if(!node) return;
		in(node->left);
		cout<<node->data<<" ";
		in(node->right);
	}
	void pre(TreeNode* node){
		if(!node) return;
		cout<<node->data<<" ";
		pre(node->left);
		pre(node->right);
	}
	void post(TreeNode* node){
		if(!node) return;
		post(node->left);
		post(node->right);
		cout<<node->data<<" ";
	}
};

int main(){
	BinaryTreeInArray<int> *b=new BinaryTreeInArray<int>;
	BinaryTreeInLinkedList<int> *tree=new BinaryTreeInLinkedList<int>;
	int j, n;
	cin>>n;
	for(j=0; j<n; j++){
		b->addElementAsCompleteTree(j);
		tree->addElementAsCompleteTree(j);
	}
	b->displayInorder();
	cout<<endl;
	tree->displayInorder();
	cout<<endl;
	b->displayPreorder();
	cout<<endl;
	tree->displayPreorder();
	cout<<endl;
	b->displayPostorder();
	cout<<endl;
	tree->displayPostorder();
	cout<<endl;
	return 0;
}
