#include<iostream>
#include<cstdio>
#include<string>
#include"crc.h"
using namespace std;

#define d 700001     //膜数，也为散列表的大小
#define hash(n) n%d

inline unsigned int read() {
	unsigned int x = 0, f = 1;
	char c = getchar();
	while (c < '0' || c > '9') { if (c == '-') f = -1; c = getchar(); }
	while (c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
	return x * f;
}
#define in(a) a=read()

struct point {
	unsigned int crc;    //crc的值
	string pw;           //密码的值

	point(unsigned int c, string p) {
		crc = c;
		pw = p;
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
	ListNodePosi find(unsigned int const& e) const {
		int n = size - 1;
		ListNodePosi p = trailer;
		while (0 <= n--)
			if (e == (p = p->pred)->data.crc) return p; 
		return NULL; 
	}
};


List* table[d];    //hashtable
string rkl;        //弱口令
string salt;       //盐


void insertHash(string pw, unsigned int crc) {    //插入hash表

	point p(crc, pw);
	int index = hash(crc);
	if (table[index] == NULL) { table[index] = new List(); } 
	if (table[index]->find(crc) != NULL) {
		table[index]->find(crc)->data.pw = "-1";        //duplicate
	}
	else {
		table[index]->insertAsLast(p);
	}
}

void init() {
	for (int i = 0; i < d; i++) table[i] = NULL;   //初始化为NULL
	string s = "0123456789tsinghua";

	//五位密码的所有组合形式
	for (int i = 0; i < s.length(); i++) {
		string temp1;
		temp1 += s[i];
		temp1 += salt;
		const char*pw1 = temp1.data();
		unsigned int crc1 = crc32((const unsigned char*)pw1, 1 + salt.length());
		insertHash(pw1, crc1);
		for (int j = 0; j < s.length(); j++) {
			string temp2;
			temp2 += s[i];
			temp2 += s[j];
			temp2 += salt;
			const char*pw2 = temp2.data();
			unsigned int crc2 = crc32((const unsigned char*)pw2, 2 + salt.length());
			insertHash(pw2, crc2);
			for (int w = 0; w < s.length(); w++) {
				string temp3;
				temp3 += s[i];
				temp3 += s[j];
				temp3 += s[w];
				temp3 += salt;
				const char*pw3 = temp3.data();
				unsigned int crc3 = crc32((const unsigned char*)pw3, 3 + salt.length());
				insertHash(pw3, crc3);
				for (int v = 0; v < s.length(); v++) {
					string temp4;
					temp4 += s[i];
					temp4 += s[j];
					temp4 += s[w];
					temp4 += s[v];
					temp4 += salt;
					const char*pw4 = temp4.data();
					unsigned int crc4 = crc32((const unsigned char*)pw4, 4 + salt.length());
					insertHash(pw4, crc4);
					for (int h = 0; h < s.length(); h++) {
						string temp5;
						temp5 += s[i];
						temp5 += s[j];
						temp5 += s[w];
						temp5 += s[v];
						temp5 += s[h];
						temp5 += salt;
						const char*pw5 = temp5.data();
						unsigned int crc5 = crc32((const unsigned char*)pw5, 5 + salt.length());
						insertHash(pw5, crc5);
					}

				}
			}
		}
	}
}

void search(unsigned int crc) {    //查找
	List*l = table[hash(crc)];
	if (l == NULL || l->find(crc) == NULL) {    //找不到的情况
		string result = "No";
		printf("%s\n", result.c_str());
	}
	else {
		string pw = l->find(crc)->data.pw;
		if (pw == "-1") {                       //pw为'-1'，则为duplicate
			string result = "duplicate";
			printf("%s\n", result.c_str());
		}
		else {
			pw.erase(pw.end() - salt.length(), pw.end());    //去除密码中salt的部分
			printf("%s\n", pw.c_str());

			rkl += pw[0];    //对弱口令的添加
			if (rkl.length() > 7) rkl.erase(rkl.begin(), rkl.begin() + 1);  //弱口令只保留7位
		}
	}
}

void putrkl() {       //对于弱口令的插入
	string temp = rkl + salt;
	const char*pw = temp.data();
	unsigned int crc = crc32((const unsigned char*)pw, rkl.length() + salt.length());
	insertHash(pw, crc);
}

int main() {
	int n;
	in(n);
	cin >> salt;
	init();

	int op;
	for (int i = 0; i < n; i++) {
		in(op);
		if (op == 0) {
			unsigned int crc;
			in(crc);
			search(crc);
		}
		else {
			putrkl();
		}
	}
	return 0;
}


