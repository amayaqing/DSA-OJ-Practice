#include<iostream>
using namespace std;

#define d 500009
#define hash(n) n%d

inline int read() {
	int x = 0, f = 1;
	char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') f = -1; c = getchar(); }
	while (c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
	return x * f;
}
#define in(a) a=read()

struct point {
	int id;      //学生ID
	int time;    //提交记录

	point(int a,int b) {
		id = a;
		time = b;
	}
	point() {};
};

#define ListNodePosi ListNode* 
struct ListNode {
	point data;
	ListNodePosi pred;
	ListNodePosi succ;
	ListNode() {}
	ListNode(point e, ListNodePosi p, ListNodePosi s) {
		data = e;
		pred = p;
		succ = s;
	}
	ListNodePosi insertAsPred(const point& e) {
		ListNodePosi x = new ListNode(e, pred, this);
		pred->succ = x; pred = x;
		return x;
	}
	ListNodePosi insertAsSucc(const point& e) {
		ListNodePosi x = new ListNode(e, this, succ);
		succ->pred = x; succ = x;
		return x;
	}
};
class List {
private:
	int size;
	ListNodePosi header;
	ListNodePosi trailer;

public:
	List() {
		header = new ListNode;
		trailer = new ListNode;
		header->succ = trailer; header->pred = NULL;
		trailer->pred = header; trailer->succ = NULL;
		size = 0;
	}
	bool empty() { return size <= 0; }
	ListNodePosi first() { return header->succ; }
	ListNodePosi insertAsLast(const point& e) {
		size++;
		return trailer->insertAsPred(e);
	}
	void remove(ListNodePosi p) {
		p->pred->succ = p->succ; p->succ->pred = p->pred;
		delete p;
		size--;
	}
	ListNodePosi find(int const& e) const {
		int n = size - 1;
		ListNodePosi p = trailer;
		while (0 <= n--)
			if (e == (p = p->pred)->data.id) return p;
		return NULL; 
	}
};


List* table[d];    //hashtable
int last = 0;     //上一次的记录，作为全局变量更新

void init() {   //将hashtable的所有项初始化为NULL
	for (int i = 0; i < d; i++) {
		table[i] = NULL;
	}
}

void insertHash(int id, int time) {   //插入hash表
	point p(id, time);
	int index = hash(id);
	if (table[index] == NULL) { table[index] = new List(); }
	if (table[index]->find(id) != NULL) {
		table[index]->find(id)->data.time = time;   //已经存在的元素，更新time
	}
	else {
		table[index]->insertAsLast(p);   //还未插入的元素，进行插入
	}
}

int main() {

	init();   //初始化

	int n;
	in(n);
	for (int i = 0; i < n; i++) {
		int id;
		int result;
		in(id);
		id ^= last;
		List*l = table[hash(id)];
		if (l == NULL || l->find(id) == NULL)     //此id第一次出现的情况（还未插入hashtable的情况）
			result = i + 1;
		else 
			result = l->find(id)->data.time;     //之前已插入的情况，找到上一次的time
		
		printf("%d\n", result);
		insertHash(id, i + 1);    //对未插入的元素进行插入，对已经插入的元素进行time更新
		last = result;   //更新last
	}
	return 0;
}