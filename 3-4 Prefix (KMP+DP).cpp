#include<iostream>
#include<stdio.h>
#include<string>

using namespace std;

inline string read() {
	char ch = getchar();
	string str = "";
	while (!((ch >= 'a') && (ch <= 'z')))
		ch = getchar();
	while ((ch >= 'a') && (ch <= 'z'))
		str += ch, ch = getchar();
	return str;
}

long long buildnext(string p) {
	int m = p.length();
	long long sum = m;

	int*next = new int[m];     //next表
	int*len = new int[m];      //对于每一个点，next序列的长度 
	next[0] = -1;
	len[0] = -1;

	int t = next[0];
	int j = 0;

	while (j < m - 1) {
		if (t < 0 || p[j] == p[t]) {
			next[++j] = ++t;
			len[j] = len[t] + 1;     //更新next的同时更新每一个点next序列的长度
			sum += len[j];          //同时加和到结果中
		}
		else t = next[t];
	}
	if (p[m - 1] == p[next[m - 1]]) {     //若序列的最后一个点与其next的值相等，则特别判断加和
		sum += len[next[m - 1] + 1] + 1;
	}
	return sum;
}

int main() {
	string s1;
	s1 = read();
	long long sum = buildnext(s1);
	printf("%lld\n", sum);
	return 0;
}