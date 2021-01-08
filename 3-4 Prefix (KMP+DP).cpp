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

	int*next = new int[m];     //next��
	int*len = new int[m];      //����ÿһ���㣬next���еĳ��� 
	next[0] = -1;
	len[0] = -1;

	int t = next[0];
	int j = 0;

	while (j < m - 1) {
		if (t < 0 || p[j] == p[t]) {
			next[++j] = ++t;
			len[j] = len[t] + 1;     //����next��ͬʱ����ÿһ����next���еĳ���
			sum += len[j];          //ͬʱ�Ӻ͵������
		}
		else t = next[t];
	}
	if (p[m - 1] == p[next[m - 1]]) {     //�����е����һ��������next��ֵ��ȣ����ر��жϼӺ�
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