#include <iostream>
#include <list>
#include <unordered_map>
#include <queue>
#include <memory>

using namespace std;

template <typename T>
class LinkedBinaryTree{
private:
    struct Node{
        T value;
        Node* left;
        Node* right;
        Node* next;
        Node(T val): value(val), left(nullptr), right(nullptr), next(nullptr){}
    };
    Node* root;
    unordered_map<T, Node*> headers;
    void addToList(Node* node){
        if(headers.find(node->value)==headers.end())
            headers[node->value]=node;
        else{
            Node* header=headers[node->value];
            while(header->next){
                header=header->next;
            }
            header->next=node;
        }
    }
    void clear(Node* node){
        if(!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    void printList(Node* header){
        while(header){
            cout<<header->value<<" -> ";
            header=header->next;
        }
        cout<<"null\n";
    }
public:
    LinkedBinaryTree(): root(nullptr){}
    ~LinkedBinaryTree(){
        clear(root);
    }
    void insert(T value){
        Node* newNode=new Node(value);
        if(!root) root=newNode;
        else{
            queue<Node*> q;
            q.push(root);
            while(!q.empty()){
                Node* current=q.front();
                q.pop();
                if (!current->left){
                    current->left=newNode;
                    break;
                }
                else
                    q.push(current->left);
                if(!current->right){
                    current->right=newNode;
                    break;
                }
                else q.push(current->right);
            }
        }
        addToList(newNode);
    }
    /*void printLinkedLists(){
        for(auto it=headers.begin(); it!=headers.end(); it++){
            cout<<"Value "<<it->first<<": ";
            printList(it->second);
        }
    }*/
};

/*1. Use Case: A database index where nodes with the same key (or value) need to be grouped for fast retrieval.
     Benefit: The linked list allows for quick traversal of entries with identical keys.
  2. Use Case: Storing frequency distributions where elements with the same frequency are grouped together.
     Benefit: Quick access to all nodes with the same frequency (e.g., word occurrences in a document).
  3. Use Case: Identifying and grouping duplicate entries in a dataset.
     Benefit: Helps in finding and processing duplicate records efficiently.*/

/*int main(){
    LinkedBinaryTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);
    tree.printLinkedLists();
    return 0;
}*/
