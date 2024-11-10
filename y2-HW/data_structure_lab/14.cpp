#include <bits/stdc++.h>

template<class T>
class MinMaxHeap{
public:
	MinMaxHeap(){}
    //The insert function in a Min-Max Heap adds a new element to the heap while maintaining its unique properties.
    void insert(T value){
    	data.push_back(value);
    	bubbleUp(data.size()-1);
    }
    // Retrieve the minimum element in the heap, throw an exception when heap is empty
    T getMin() const{
    	if(data.empty()) throw std::runtime_error("Heap is empty");
    	return data[0];
    }
    // Retrieve the maximum element in the heap, throw an exception when heap is empty
    T getMax() const{
    	if(data.empty()) throw std::runtime_error("Heap is empty");
    	if(data.size()==1) return data[0];
    	if(data.size()==2) return data[1];
    	return std::max(data[1], data[2]);
    }
    // Delete the minimum element in the heap, throw an exception when heap is empty
    void deleteMin(){
    	if(data.empty()) throw std::runtime_error("Heap is empty");
    	data[0]=data.back();
    	data.pop_back();
    	trickleDown(0);
    }
    // Delete the maximum element in the heap, throw an exception when heap is empty
    void deleteMax(){
    	if(data.empty()) throw std::runtime_error("Heap is empty");
    	int maxIndex;
    	if(data.size()==1){
    		data.pop_back();
    		return;
		}
		else if(data.size()==2) maxIndex=1;
		else maxIndex=(data[1]>data[2])? 1 : 2;
		data[maxIndex]=data.back();
		data.pop_back();
		trickleDown(maxIndex);
    }
private:
    //create any member you need to complete MinMaxHap
    bool isMinLevel(int index) const{
		return static_cast<int>(std::log2(index+1))%2==0;
	}
    std::vector<T> data;
    void bubbleUp(int index){
    	if(index==0) return;
    	int parent=(index-1)/2;
    	if(isMinLevel(index)){
    		if(data[index]>data[parent]){
    			std::swap(data[index], data[parent]);
    			bubbleMax(parent);
			}
			else bubbleMin(index);
		}
		else{
			if(data[index]<data[parent]){
    			std::swap(data[index], data[parent]);
    			bubbleMin(parent);
			}
			else bubbleMax(index);
		}
	}
	void bubbleMin(int index){
		int grandparent=(index-3)/4;
		if(grandparent>=0&&data[index]<data[grandparent]){
    		std::swap(data[index], data[grandparent]);
    		bubbleMin(grandparent);
		}
	}
	void bubbleMax(int index){
		int grandparent=(index-3)/4;
		if(grandparent>=0&&data[index]>data[grandparent]){
    		std::swap(data[index], data[grandparent]);
    		bubbleMax(grandparent);
		}
	}
	void trickleDown(int index){
		if(isMinLevel(index)) trickleMin(index);
		else trickleMax(index);
	}
	void trickleMin(int index){
		int minChild=MinOrGrand(index);
		if(minChild==-1) return;
		if(minChild>(index*2+1)){
			if(data[minChild]<data[index]){
				std::swap(data[minChild], data[index]);
				int parent=(minChild-1)/2;
				if(data[minChild]>data[parent]) std::swap(data[minChild], data[parent]);
				trickleMin(minChild);
			}
		}
		else{
			if(data[minChild]<data[index]) std::swap(data[minChild], data[index]);
		}
	}
	void trickleMax(int index){
		int maxChild=MaxOrGrand(index);
		if(maxChild==-1) return;
		if(maxChild>(index*2+1)){
			if(data[maxChild]>data[index]){
				std::swap(data[maxChild], data[index]);
				int parent=(maxChild-1)/2;
				if(data[maxChild]<data[parent]) std::swap(data[maxChild], data[parent]);
				trickleMin(maxChild);
			}
		}
		else{
			if(data[maxChild]>data[index]) std::swap(data[maxChild], data[index]);
		}
	}
	int MinOrGrand(int index) const{
		int size=data.size();
		int firstChild=2*index+1;
		int minIndex=-1;
		if(firstChild<size){
			minIndex=firstChild;
			for(int i=1; i<=3&&firstChild+i<size; i++){
				if(data[firstChild+i]<data[minIndex]) minIndex=firstChild+i;
			}
		}
		return minIndex;
	}
	int MaxOrGrand(int index) const{
		int size=data.size();
		int firstChild=2*index+1;
		int maxIndex=-1;
		if(firstChild<size){
			maxIndex=firstChild;
			for(int i=1; i<=3&&firstChild+i<size; i++){
				if(data[firstChild+i]>data[maxIndex]) maxIndex=firstChild+i;
			}
		}
		return maxIndex;
	}
};

int main(){
    MinMaxHeap<int> mmHeap;
    int j;
    srand(time(NULL));
    for(j=0; j<10; j++)
    	mmHeap.insert(rand()%100);
    while(true){
		try{
			std::cout<<mmHeap.getMin()<<" ";
			mmHeap.deleteMin();
		}
		catch(std::exception){
			break;
		}
    }
    std::cout<<std::endl;
    for(j=0; j<10; j++)
		mmHeap.insert(rand()%100);
    while(true){
		try{
			std::cout<<mmHeap.getMax()<<" ";
			mmHeap.deleteMax();
		}
		catch(std::exception){
			break;
		}
	}
    return 0;
}

