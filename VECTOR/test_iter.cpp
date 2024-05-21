#include "v.hpp"
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	cerr << "first0\n";
	sjtu::vector<string> vs;
	string str = "Je ne sais pas!";
	for(int i = 0; i < 100; i++)
		vs.push_back(str);
	cout << "line 13\n";
	sjtu::vector<string>::iterator iter = vs.begin();
	cout << vs.size() << endl;
	// *(iter + 17) = "1989";
	// cout << *(iter + 13);
	int add = 13;
	sjtu::vector<string>::iterator _it = iter + add;
	cout << "newvec\n";
	*_it = "1989";
	cout << *_it << endl;

	// std::vector<string> stdv;
	// // string str = "Je ne sais pas!";
	// for(int i = 0; i < 100; i++)
	// 	stdv.push_back(str);
	
	// std::vector<string>::iterator stditer = stdv.begin();
	// // *(stditer + 17) = "1989";
	// *stditer = "1989";

	int cnt = 0;
	while(true)
	{
		if(cnt == 92)
		{
			vs[95] = "Taylor Swift!";
			*(iter) = "Ariana Grande";
		}
		cerr << "cnt = " << cnt << ", iter = " << *iter << endl;
		cnt++;
		iter++;
		if(iter == vs.end())
			break;
	}
}