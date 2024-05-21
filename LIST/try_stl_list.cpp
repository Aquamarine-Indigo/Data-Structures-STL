#include <iostream>
// #include <list>
#include "list.hpp"
using namespace std;

sjtu::list<int> l;
sjtu::list<int>::iterator it, _it;

int main()
{
	int n;
	cin >> n;
	int arr[30];
	for(int i = 0 ; i < n; i++)
	{
		cin >> arr[i];
		l.push_back(arr[i]);
	}
	l.sort();
	l.reverse();
	cout << "size of list l = " << l.size() << endl;
	l.unique();
	cout << "size of list l = " << l.size() << endl;
	it = l.begin();
	for(int i = 0; i < l.size(); i++)
	{
		cout << i << ": " << *it << endl;
		it++;
	}
	// cout << *it << endl; // -> invalid
	// it = l.end();
	// _it = l.begin();
	// cout << *_it << ' ' << *it << endl;
	// l.pop_back();
	// l.pop_back();
	// cout << *_it << ' ' << *it << endl;
	// it++;
	// cout << *it;
	// list<int>::const_iterator __it = l.cend();
	// __it++;
	// cout << *__it;
}