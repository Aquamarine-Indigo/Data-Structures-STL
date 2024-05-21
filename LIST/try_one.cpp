#include <iostream>
#include <cstdio>
#include "list.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

int n;
int arr1[1005];
int arr2[1005];
sjtu::list<int> l1, l2;

int main()
{
	srand(time(NULL));
	n = 1000;
	for(int i = 0; i < n; i++)
	{
		arr1[i] = rand();
		arr2[i] = rand();
		l1.push_back(arr1[i]);
		l2.push_back(arr2[i]);
	}
	sjtu::list<int>::iterator it1 = l1.begin();
	for(int i = 0; i < 105; i++)
		it1++;
	for(int i = 0; i < 100; i++)
	{
		int choice = rand() % 3;
		if(choice == 0)
		{
			it1++;
			continue;
		}
		else if(choice == 1)
		{
			it1 = l1.erase(it1);
		}
		int buf = rand() % 277;
		l1.insert(it1, buf);
	}
	l1.sort();
	l2.sort();
	l2.merge(l1);
	// it1 = l1.begin();
	sjtu::list<int>::iterator it2 = l2.begin();
	for(int i = 0; i < l2.size(); i++)
	{
		cout << *it2 << ' ';
		it2++;
	}
	cout << endl;
}
