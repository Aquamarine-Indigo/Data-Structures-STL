#include <iostream>
#include <cstdio>
#include "exceptions.hpp"
#include "priority_queue.hpp"
#include <queue>
using namespace std;

std::priority_queue<int> stdq;
sjtu::priority_queue<int> myq;

int arr[] = {7, 4, 1, 2, 5, 3, 6, 7, 8, 110, 5, 7};

int main()
{
	int n = 12;
	for(int i = 0; i < n; i++)
	{
		myq.push(arr[i]);
	}
	myq.pop();
	myq.pop();
	myq.pop();
	myq.push(29);
	myq.push(47);
	myq = myq;
	int maxn = myq.size();
	for(int i = 0; i < maxn; i++)
	{
		cout << myq.top() << ' ';
		myq.pop();
	}
}