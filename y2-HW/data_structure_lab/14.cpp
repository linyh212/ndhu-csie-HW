#include <bits/stdc++.h>

template<class T>
class MinMaxHeap{
public:
	MinMaxHeap(){}
    //The insert function in a Min-Max Heap adds a new element to the heap while maintaining its unique properties.
    void insert(T value){
    	if(search(value)) return;
        heap.push_back(value);
        bubbleUp(heap.size()-1);
    }
    // Retrieve the minimum element in the heap, throw an exception when heap is empty
    T getMin() const{
    	if(empty()) throw std::runtime_error("Heap is empty");
        return heap[0];
    }
    // Retrieve the maximum element in the heap, throw an exception when heap is empty
    T getMax() const{
    	if(empty()) throw std::runtime_error("Heap is empty");
    	if(heap.size()==1) return heap[0];
        if(heap.size()==2) return heap[1];
    	return std::max(heap[1], heap[2]);
    }
    // Delete the minimum element in the heap, throw an exception when heap is empty
    void deleteMin(){
    	if(empty()) throw std::runtime_error("Heap is empty");
    	heap[0]=heap.back();
        heap.pop_back();
        check_down(0);
    }
    // Delete the maximum element in the heap, throw an exception when heap is empty
    void deleteMax(){
    	if(empty()) throw std::runtime_error("Heap is empty");
    	if(heap.size()==1){
            heap.pop_back();
            return;
        }
        int maxIndex=(heap.size()==2||heap[1]>heap[2])? 1:2;
        heap[maxIndex]=heap.back();
        heap.pop_back();
        if(maxIndex<heap.size()) check_down(maxIndex);
    }
private:
<<<<<<< HEAD
	//create any member you need to complete MinMaxHap
    std::vector<T> heap;
    bool empty() const{
        return heap.size()==0;
    }
    bool search(int value){
        for(int i=0; i<heap.size(); i++){
            if(heap[i]==value) return true;
        }
        return false;
    }
    bool minLevel(int index){
        int level=0;
        while(index>0){
            index=(index-1)/2;
            level++;
        }
        return level%2==0;
    }
=======
    //create any member you need to complete MinMaxHap
    bool isMinLevel(int index) const{
		return static_cast<int>(std::log2(index+1))%2==0;
	}
    std::vector<T> data;
>>>>>>> 49dab70c079fc5b6f94be720c1a858143234fc5b
    void bubbleUp(int index){
        if(index==0) return;
        int parent=(index-1)/2;
        if(heap[index]==heap[parent]) return;
        if(minLevel(index)){
            if(heap[index]>heap[parent]){
                std::swap(heap[index], heap[parent]);
                max_bubble(parent);
            }
			else min_bubble(index);
        }
		else{
            if(heap[index]<heap[parent]){
                std::swap(heap[index], heap[parent]);
                min_bubble(parent);
            }
			else max_bubble(index);
        }
    }
    void min_bubble(int index){
        int grandparent=(index-3)/4;
        if(index>2&&heap[index]<heap[grandparent]){
            std::swap(heap[index], heap[grandparent]);
            min_bubble(grandparent);
        }
    }
    void max_bubble(int index){
        int grandparent=(index-3)/4;
        if(heap[index]==heap[grandparent]) return;
        if(index>2&&heap[index]>heap[grandparent]){
            std::swap(heap[index], heap[grandparent]);
            max_bubble(grandparent);
        }
    }
    void check_down(int index){
        if(minLevel(index)) check_min(index);
		else check_max(index);
    }
    void check_min(int index){
        int min=index;
        int left=2*index+1;
        int right=2*index+2;
        if(left<heap.size()&&heap[left]<heap[min]) min=left;
        if(right<heap.size()&&heap[right]<heap[min]) min=right;
        for(int i=1; i<=2; i++){
            int grandchild=2*left+i;
            if(grandchild<heap.size()&&heap[grandchild]<heap[min]) min=grandchild;
            grandchild=2*right+i;
            if(grandchild<heap.size()&&heap[grandchild]<heap[min]) min=grandchild;
        }
        if(min!=index){
            std::swap(heap[index], heap[min]);
            if(min<(heap.size()/2)) check_min(min);
        }
    }
    void check_max(int index){
        int max=index;
        int left=2*index+1;
        int right=2*index+2;
        if(left<heap.size()&&heap[left]>heap[max]) max=left;
        if(right<heap.size()&&heap[right]>heap[max]) max=right;
        for(int i=1; i<=2; i++){
            int grandchild=2*left+i;
            if(grandchild<heap.size()&&heap[grandchild]>heap[max]) max=grandchild;
            grandchild=2*right+i;
            if(grandchild<heap.size()&&heap[grandchild]>heap[max]) max=grandchild;
        }
        if(max!=index){
            std::swap(heap[index], heap[max]);
            if(max<(heap.size()/2)) check_max(max);
        }
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
