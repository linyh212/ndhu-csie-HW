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
    // ����1: ���J�������ˬd�諸�j�p
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);
    heap.insert(30);
    assert(heap.count()==4); // �T�O�襤��4�Ӥ���
    cout<<"���J���j�p���ճq�L�I"<<endl;
    // ����2: �ˬd extract() �����T�ʡ]����^�̤j�ȨåB�����ﵲ�c�^
    int maxElement=heap.extract();
    assert(maxElement==30);  // �̤j��������30
    assert(heap.count()==3); // �T�O���X�@�Ӥ������j�p���
    maxElement=heap.extract();
    assert(maxElement==20);  // �U�@�ӳ̤j��������20
    maxElement=heap.extract();
    assert(maxElement==10);  // �U�@�ӳ̤j��������10
    maxElement=heap.extract();
    assert(maxElement==5);   // �U�@�ӳ̤j��������5
    assert(heap.count()==0); // �̫�襤���L����
    cout<<"extract() ���ճq�L�I"<<endl;
    // ����3: �b�Ű�W�I�s extract() ���޵o�ҥ~
    try{
        heap.extract();
    }catch (const out_of_range& e){
        cout<<"�b�Ű�W extract() ���ճq�L�I"<<endl;
    }
    cout<<"�Ҧ����ճq�L�I"<<endl;
    return 0;
}*/
