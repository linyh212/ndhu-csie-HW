#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Trie{
public:
	/*
	construct a Trie.
	*/
	Trie(){
		root=new TrieNode(); 
	}
	/*
	search trie for key, return true if exist, false if not.
	*/
	bool search(string key){
        TrieNode* current=root;
        for(char c: key){
            bool found=false;
            for(auto& child: current->children){
                if(child.first==c){
                    current=child.second;
                    found=true;
                    break;
                }
            }
            if(!found) return false;
        }
        return current->isEndOfWord;
    }
    void insert(string value){
        TrieNode* current=root;
        for(char c: value){
            bool found=false;
            for(auto& child: current->children){
                if(child.first==c){
                    current=child.second;
                    found=true;
                    break;
                }
            }
            if(!found){
                TrieNode* newNode=new TrieNode();
                current->children.push_back({c, newNode});
                current=newNode;
            }
        }
        current->isEndOfWord=true;
    }
	/*
	display trie in pre-order, each element in a line, display space befoer element base on the level of the element. level 1 for 2 space, level 2 for 4 space, level 3 for 6 space and so on, root is level 	0.
	*/
	void preorder(){
		cout<<"[]"<<endl;
		pre(root, 1);
	}
private:
	struct TrieNode{
		vector<pair<char, TrieNode*>> children;
		bool isEndOfWord;
		TrieNode(): isEndOfWord(false){}
	};
	TrieNode* root;
	void pre(TrieNode* node, int level){
        if(node==nullptr) return;
        for(auto& child: node->children){
        	string acc="";
        	TrieNode* current= &*child.second;
        	acc+=child.first;
        	while(current->children.size()==1&&!current->isEndOfWord){
        		acc+=current->children[0].first;
        		current=current->children[0].second;
			}
        	cout<<string(level*2, ' ')<<acc<<endl;
            pre(current, level+1);
        }
    }
};

int main(){
	Trie *trie=new Trie();
	string command, key, value;
	while(1){
		cin>>command;
		if(command=="insert"){
			cin>>value;
			trie->insert(value);
		}
		else if(command=="search"){
			cin>>key;
			if(trie->search(key))
				cout<<"exist"<<endl;
			else
				cout<<"not exist"<<endl;
		}
		else if(command=="print"){
			trie->preorder();
		}
		else if(command=="exit"){
			break;
		}
	}
}

