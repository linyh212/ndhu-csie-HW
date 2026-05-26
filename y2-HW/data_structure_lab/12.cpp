#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <stack>
#include <sstream>

class ExpressionTree{
private:
    struct Node{
        std::string value;
        Node* left;
        Node* right;
        Node(const std::string& val): value(val), left(nullptr), right(nullptr){}
    };
    Node* root;
public:
    //Constructor of class ExpressionTree take a string as an infix expression and build the expression tree base on that.
    //there are five different operation you have to consider, +(addition), -(subtraction), *(multiplication), /(division) and %(modulus).
    //you also need to consider -(negative) sign.
    //all operands are integer
    ExpressionTree(const std::string &expression){
    	auto postfix=InfixToPostfix(expression);
    	root=build(postfix);
    }
    ~ExpressionTree(){ deleteTree(root); }
    void deleteTree(Node* node){
        if(node==nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    //print all element in infix order.
    void printInFixOrder(){
    	Infix(root);
    	std::cout<<std::endl;
    }
    //print all element in prefix order.
    void printPreFixOrder(){
    	Prefix(root);
    	std::cout<<std::endl;
    }
    //print all element in postfix order.
    void printPostFixOrder(){
    	Postfix(root);
    	std::cout<<std::endl;
    }
    //evaluate the expression, this function return an int sience % can only work with integer in C
    int eval(){
        return evaluate(root);
    }
private:
	bool isOperator(const std::string& op){
		return op=="+"||op=="-"||op=="*"||op=="/"||op=="%";
	}
	int precedence(const std::string& op){
		if(op=="+"||op=="-") return 1;
		if(op=="*"||op=="/"||op=="%") return 2;
		return 0;
	}
	std::vector<std::string> InfixToPostfix(const std::string& expression){
		std::stack<std::string> operators;
		std::vector<std::string> postfix;
		std::istringstream tokens(expression);
		std::string token;
		while(tokens>>token){
			if(isdigit(token[0])||(token[0]=='-'&&token.length()>1&&isdigit(token[1]))) postfix.push_back(token);
			else if(token=="(") operators.push(token);
			else if(token==")"){
				while(!operators.empty()&&operators.top()!="("){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.pop();
			}
			else if(isOperator(token)){
				while(!operators.empty()&&precedence(operators.top())>=precedence(token)){
					postfix.push_back(operators.top());
					operators.pop();
				}
				operators.push(token);
			}
		}
		while(!operators.empty()){
			postfix.push_back(operators.top());
			operators.pop();
		}
		return postfix;
	}
	Node* build(const std::vector<std::string>& postfix){
		std::stack<Node* >nodes;
		for(const std::string& token: postfix){
			if(isOperator(token)){
				Node* right=nodes.top();
				nodes.pop();
				Node* left=nodes.top();
				nodes.pop();
				Node* node=new Node(token);
				node->right=right;
				node->left=left;
				nodes.push(node);
			}
			else nodes.push(new Node(token));
		}
		return nodes.top();
	}
	void Infix(Node* node){
		if(!node) return;
		if(isOperator(node->value)) std::cout<<"(";
		Infix(node->left);
		std::cout<<node->value<<" ";
		Infix(node->right);
		if(isOperator(node->value)) std::cout<<")";
	}
	void Prefix(Node* node){
		if(!node) return;
		std::cout<<node->value<<" ";
		Prefix(node->left);
		Prefix(node->right);
	}
	void Postfix(Node* node){
		if(!node) return;
		Postfix(node->left);
		Postfix(node->right);
		std::cout<<node->value<<" ";
	}
	int evaluate(Node* node){
		if(!node) return 0;
		if(!isOperator(node->value)) return std::stoi(node->value);
		int leftval=evaluate(node->left);
		int rightval=evaluate(node->right);
		if(node->value=="+") return leftval+rightval;
		if(node->value=="-") return leftval-rightval;
		if(node->value=="*") return leftval*rightval;
		if(node->value=="/"){
			if(rightval==0) throw std::runtime_error("Division by zero");
			return leftval/rightval;
		}
		if(node->value=="%"){
			if(rightval==0) throw std::runtime_error("Division by zero");
			return leftval%rightval;
		}
		throw std::runtime_error("Invalid operator");
	}
};

int main(){
    std::string infix;
    std::getline(std::cin, infix);
    ExpressionTree exprTree(infix);
    std::cout<<"Inorder: ";
    exprTree.printInFixOrder();
    std::cout<<"Preorder: ";
    exprTree.printPreFixOrder();
    std::cout<<"Postorder: ";
    exprTree.printPostFixOrder();
    std::cout<<"Evaluated result: "<< exprTree.eval()<<std::endl;
    return 0;
}

