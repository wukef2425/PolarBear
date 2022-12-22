#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include "cocos2d.h"
USING_NS_CC;

template <class Type> class MinHeap {
private:

	enum { DefaultSize = 10 };
	Type* heap;
	int CurrentSize;	// ��ǰheapװ��ʲôλ����
	int MaxHeapSize;	// heap�����
	void FilterDown(int i, int m);	// ���ϵ���
	void FilterUp(int i);			// ���µ���

public:

	MinHeap(int maxSize);
	MinHeap(Type* arr, int n);
	~MinHeap() {
		delete[]heap;
	}

	bool Insert(const Type& x);		// ������С��
	bool RemoveMin(Type& x);		// �����У�x�����õ����Ǹ���Сֵ
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

	MaxHeapSize = DefaultSize < maxSize ? maxSize : DefaultSize; // ȷ���Ѵ�С
	heap = new Type[MaxHeapSize]; // �����ѿռ�
	CurrentSize = 0; // ��ʼ��
}
template <class Type> MinHeap<Type>::MinHeap(Type* arr, int n) {

	MaxHeapSize = DefaultSize < n ? n : DefaultSize;	// ���ݸ��������е����ݺʹ�С,�����Ѷ���
	heap = new Type[MaxHeapSize];
	heap = arr;			// ���鴫��
	CurrentSize = n;	// ��ǰ�Ѵ�С

	int currentPos = (CurrentSize - 2) / 2;			// ����Ҷ�ڵ�

	while (currentPos >= 0) {
		FilterDown(currentPos, CurrentSize - 1);	// ���µ���������,�γɶ�
		currentPos--;								// ��currentPos��ʼ,��CurrentSizeΪֹ, ����
	}
}
template <class Type> void MinHeap<Type>::FilterDown(int start, int EndOfHeap) {

	int i = start, j = 2 * i + 1;		// j��i������Ů

	Type temp = heap[i];

	while (j <= EndOfHeap) {

		if (j < EndOfHeap && heap[j].key > heap[j + 1].key) {
			j++;
		}
		if (temp.key <= heap[j].key) {
			break;
		}
		else {							// ����Ů��ѡС��
			heap[i] = heap[j];
			i = j;						// jָ���λ�ô����Ϊ�µ�parent
			j = 2 * j + 1;				// j�����parent������Ů
		}
	}
	heap[i] = temp;
}
template <class Type> bool MinHeap<Type>::Insert(const Type& x) {

	if (CurrentSize == MaxHeapSize) {
		return false;
	}
	heap[CurrentSize] = x;	// ��ĩβ�����µ�x
	FilterUp(CurrentSize);	// ���ϵ���Ϊ��
	CurrentSize++;			// ��Ԫ��������һ
	return true;
}
template <class Type> void MinHeap<Type>::FilterUp(int start) {	
	// ��start��ʼ,����ֱ��0,������

	int j = start, i = (j - 1) / 2;		// i��j��˫��
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

	x = heap[0];						 // ��СԪ�س�����
	heap[0] = heap[CurrentSize - 1];	 // ���һλ���ȱ
	CurrentSize--;
	FilterDown(0, CurrentSize - 1);		 // ��0��λ�ÿ�ʼ�Զ����µ���Ϊ��
	return true;
}

struct Bullet {
	int key;
	std::string name;
};


#endif