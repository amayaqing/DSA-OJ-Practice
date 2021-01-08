#include<iostream>
#include<stdio.h>
#include<string>

using namespace std;

struct share {
	int value = 0;
	int t = -1;
};

share zero;

typedef int Rank;
#define ListNodePosi ListNode* 

struct ListNode {
	share data; ListNodePosi pred; ListNodePosi succ; 									
	ListNode() {} 
	ListNode(share e, ListNodePosi p = NULL, ListNodePosi s = NULL)
		: data(e), pred(p), succ(s) {} 
	ListNodePosi insertAsPred(const share& e);
	ListNodePosi insertAsSucc(const share& e);
};

ListNodePosi ListNode::insertAsSucc(const share& e) {
	ListNodePosi x = new ListNode(e, this, succ); 
	succ->pred = x; succ = x; 
	return x; 
}

ListNodePosi ListNode::insertAsPred(const share& e) {
	ListNodePosi x = new ListNode(e, pred, this); 
	pred->succ = x; pred = x; 
	return x; 
}


class List {
private:
	int _size; ListNodePosi header; ListNodePosi trailer;
protected:
	void init(); 

public:
	List() { init(); } 
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; } 
	int& operator[] (Rank r) const; 
	ListNodePosi first() const { return header->succ; } 
	ListNodePosi last() const { return trailer->pred; } 

	ListNodePosi insertAsFirst(const share& e); 
	ListNodePosi insertAsLast(const share& e); 
	void remove(ListNodePosi p); 
}; //List

void List::init() { 
	header = new ListNode; 
	trailer = new ListNode; 
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
	_size = 0; 
}

ListNodePosi List::insertAsFirst(const share& e)
{
	_size++;
	return header->insertAsSucc(e);
} 

ListNodePosi List::insertAsLast(const share& e)
{
	_size++; return trailer->insertAsPred(e);
}

void List::remove(ListNodePosi p) { 
	p->pred->succ = p->succ; p->succ->pred = p->pred; 
	delete p; _size--; 
}

class Queue : public List { 
public: 
	void enqueue(const share& e) { insertAsLast(e); } 
	void dequeue() { remove(first()); } 
	share& front() { return first()->data; }
	void dequeue_last() { remove(last()); }
	share getMax() { return first()->data; }
	void in(const share& e) {
		while (e.value >= (last()->data).value && size()>0) {
			dequeue_last();
		}
		enqueue(e);
	}
	//reference:https://blog.csdn.net/cabi_zgx/article/details/52701266

	void out() {
		dequeue();
		if (empty()) {
			insertAsFirst(zero);
		}
	}
};

int exp(int j) {
	int n = 1;
	for (int i = 0; i < j; i++) n *= 10;
	return n;
}

int main()
{
	Queue Q;
	long long sum = 0;
	int N;
	cin >> N;
	cin.get();    //消去之后的换行符
	int outSequence = 0;

	share current;
	current.value = 0;
	current.t = -1;

	Q.insertAsFirst(zero);
	for (int k = 0; k < 2 * N; k++) {
		char c;
		char t[100];
		int a = 0;
		cin.get(c);
		int i = 0;
		for (i; c != '\n' && c != ' '; i++) {
			t[i] = c;
			cin.get(c);
		}
		//将time进行转换
		int length = i;
		int time = 0;
		for (int k = length - 1, j = 0; j < i; j++, k--) time += int(t[j] - 48) * exp(k);

		if (c == ' ') {
			cin >> a;
			current.value = a;
			current.t++;
			cin.get();//消去末尾的换行符
			sum += (long long)(Q.getMax().value)*time;
			Q.in(current);
		}
		else {
			sum += (long long)(Q.getMax().value)*time;
			if (Q.first()->data.t == outSequence) {
				Q.out();
			}
			outSequence++;
		}
	}
	cout << sum << endl;

	return 0;
}