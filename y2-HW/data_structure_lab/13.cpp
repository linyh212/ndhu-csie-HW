#include <iostream>
#include <cassert>
#include <vector>
#include <stdexcept>
using namespace std;

template<class T>
class MaxHeap{
public:
	//Constructing a empty heap.
	MaxHeap(){}
	//To add an element to the heap.
	void insert(T value){
		heap.push_back(value);
		shiftUp(heap.size()-1);
	}
	//To remove the root element in the heap and return it.
	T extract(){
		if(heap.empty()) throw out_of_range("Heap is empty");
		T maxValue=heap[0];
		heap[0]=heap.back();
		heap.pop_back();
		shiftDown(0);
		return maxValue;
	}
	//Return the number of element int the heap.
	int count(){
		return heap.size();
	}
private:
	vector<T> heap;
	void shiftUp(int index){
		while(index>0){
			int parent=(index-1)/2;
			if(heap[index]<=heap[parent]) break;
			swap(heap[index], heap[parent]);
			index=parent;
		}
	}
	void shiftDown(int index){
		int size=heap.size();
		while(index*2+1<size){
			int left=index*2+1;
			int right=index*2+2;
			int largest=index;
			if(left<size&&heap[left]>heap[largest]) largest=left;
			if(right<size&&heap[right]>heap[largest]) largest=right;
			if(largest==index) break;
			swap(heap[index], heap[largest]);
			index=largest;
		}
	}
};

/*int main() {
    MaxHeap<int> heap;
    // 測試1: 插入元素並檢查堆的大小
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);
    heap.insert(30);
    assert(heap.count()==4); // 確保堆中有4個元素
    cout<<"插入後堆大小測試通過！"<<endl;
    // 測試2: 檢查 extract() 的正確性（應返回最大值並且維持堆結構）
    int maxElement=heap.extract();
    assert(maxElement==30);  // 最大元素應為30
    assert(heap.count()==3); // 確保取出一個元素後堆大小減少
    maxElement=heap.extract();
    assert(maxElement==20);  // 下一個最大元素應為20
    maxElement=heap.extract();
    assert(maxElement==10);  // 下一個最大元素應為10
    maxElement=heap.extract();
    assert(maxElement==5);   // 下一個最大元素應為5
    assert(heap.count()==0); // 最後堆中應無元素
    cout<<"extract() 測試通過！"<<endl;
    // 測試3: 在空堆上呼叫 extract() 應引發例外
    try{
        heap.extract();
    }catch (const out_of_range& e){
        cout<<"在空堆上 extract() 測試通過！"<<endl;
    }
    cout<<"所有測試通過！"<<endl;
    return 0;
}*/
