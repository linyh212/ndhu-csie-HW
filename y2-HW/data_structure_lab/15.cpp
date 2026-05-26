#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

//design your won SuffixTreeNode using in SuffixTree
class SuffixTreeNode{
public:
	std::unordered_map<char, SuffixTreeNode*> children;
	int count;
	SuffixTreeNode(): count(0){}
	~SuffixTreeNode(){
		for(auto &child: children){
			delete child.second;
		}			
	}
};

class SuffixTree{
private:
	SuffixTreeNode *root;
    std::string text;
    void addSuffix(const std::string &suffix){
    	SuffixTreeNode *current=root;
    	for(char c: suffix){
    		if(current->children.find(c)==current->children.end()) current->children[c]=new SuffixTreeNode();
    		current=current->children[c];
    		current->count++;
		}
	}
public:
    SuffixTree(const std::string &text): text(text){
    	root=new SuffixTreeNode();
    	for(size_t i=0; i<text.size(); i++){
    		addSuffix(text.substr(i));
		}
    }
    bool exist(const std::string &substring){
    	SuffixTreeNode *current=root;
    	for(char c: substring){
    		if(current->children.find(c)==current->children.end()) return false;
    		current=current->children[c];
		}
		return true;
    }
    int count(const std::string &substring){
    	SuffixTreeNode *current=root;
    	for(char c: substring){
    		if(current->children.find(c)==current->children.end()) return 0;
    		current=current->children[c];
		}
		return current->count;
    }
    ~SuffixTree(){
    	delete root;
    }
};

int main(){
    std::string text="";
    while(true){
		std::string temp;
		getline(std::cin, temp);
		if(temp=="")
			break;
		text+=temp;
    }
    SuffixTree tree(text);
    std::string query;
    while(true){
		getline(std::cin, query);
		if(query=="")
			break;
		std::cout<<"Existence of '"<<query<<"': "<<(tree.exist(query)? "Yes":"No")<<std::endl;
		std::cout<<"Count of '"<<query<< "': "<<tree.count(query)<<std::endl;
    }
    return 0;
}

