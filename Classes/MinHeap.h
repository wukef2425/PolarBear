#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include "cocos2d.h"
USING_NS_CC;

template <class Type> class MinHeap {
private:

	enum { DefaultSize = 10 };
	Type* heap;
	int CurrentSize;	// 当前heap装到什么位置了
	int MaxHeapSize;	// heap开多大
	void FilterDown(int i, int m);	// 向上调整
	void FilterUp(int i);			// 向下调整

public:

	MinHeap(int maxSize);
	MinHeap(Type* arr, int n);
	~MinHeap() {
		delete[]heap;
	}

	bool Insert(const Type& x);		// 插入最小堆
	bool RemoveMin(Type& x);		// 出队列，x就是拿到的那个最小值
	bool IsEmpty() const {
		return CurrentSize == 0;
	}
	bool IsFull() const {
		return CurrentSize == MaxHeapSize;
	}
	void MakeEmpty() {
		CurrentSize = 0;
	}
};

template <class Type> MinHeap<Type>::MinHeap(int maxSize) {

	MaxHeapSize = DefaultSize < maxSize ? maxSize : DefaultSize; // 确定堆大小
	heap = new Type[MaxHeapSize]; // 创建堆空间
	CurrentSize = 0; // 初始化
}
template <class Type> MinHeap<Type>::MinHeap(Type* arr, int n) {

	MaxHeapSize = DefaultSize < n ? n : DefaultSize;	// 根据给定数组中的数据和大小,建立堆对象
	heap = new Type[MaxHeapSize];
	heap = arr;			// 数组传送
	CurrentSize = n;	// 当前堆大小

	int currentPos = (CurrentSize - 2) / 2;			// 最后非叶节点

	while (currentPos >= 0) {
		FilterDown(currentPos, CurrentSize - 1);	// 从下到上逐步扩大,形成堆
		currentPos--;								// 从currentPos开始,到CurrentSize为止, 调整
	}
}
template <class Type> void MinHeap<Type>::FilterDown(int start, int EndOfHeap) {

	int i = start, j = 2 * i + 1;		// j是i的左子女

	Type temp = heap[i];

	while (j <= EndOfHeap) {

		if (j < EndOfHeap && heap[j].key > heap[j + 1].key) {
			j++;
		}
		if (temp.key <= heap[j].key) {
			break;
		}
		else {							// 两子女中选小者
			heap[i] = heap[j];
			i = j;						// j指向的位置代替成为新的parent
			j = 2 * j + 1;				// j是这个parent的左子女
		}
	}
	heap[i] = temp;
}
template <class Type> bool MinHeap<Type>::Insert(const Type& x) {

	if (CurrentSize == MaxHeapSize) {
		return false;
	}
	heap[CurrentSize] = x;	// 在末尾插入新的x
	FilterUp(CurrentSize);	// 向上调整为堆
	CurrentSize++;			// 堆元素数量加一
	return true;
}
template <class Type> void MinHeap<Type>::FilterUp(int start) {	
	// 从start开始,向上直到0,调整堆

	int j = start, i = (j - 1) / 2;		// i是j的双亲
	Type temp = heap[j];

	while (j > 0) {
		if (heap[i].key <= temp.key) {
			break;
		}
		else {
			heap[j] = heap[i];
			j = i;
			i = (i - 1) / 2;
		}
	}
	heap[j] = temp;
}
template <class Type> bool MinHeap <Type>::RemoveMin(Type& x) {

	if (!CurrentSize) {
		return false;
	}

	x = heap[0];						 // 最小元素出队列
	heap[0] = heap[CurrentSize - 1];	 // 最后一位填补空缺
	CurrentSize--;
	FilterDown(0, CurrentSize - 1);		 // 从0号位置开始自顶向下调整为堆
	return true;
}

struct Bullet {
	int key;
	std::string name;
};


#endif