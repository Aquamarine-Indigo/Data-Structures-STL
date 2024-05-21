#include "map.hpp"
#include "exceptions.hpp"
#include "utility.hpp"
#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;
sjtu::map<int, int> mp;

// class chrcmp
// {
// public:
// 	bool operator () (char *a, char *b)
// 	{
// 		int ret =  strcmp(a, b);
// 		if(ret < 0)
// 			return true;
// 		return false;
// 	}
// };
// sjtu::map<char *, int, chrcmp> mp;

// void print(sjtu::map<char *, int, chrcmp>::node *pos)
// {
// 	if(pos == nullptr)
// 		return;
// 	cout << "\n(" << pos->data->first << ", " << (pos->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << "): ";
// 	if(pos->father == nullptr)
// 		printf("father: --\n");
// 	else
// 	{
// 		printf("father: (");
// 		cout << pos->father->data->first << ", " << (pos->father->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << ")\n";
// 	}
// 	if(pos->left == nullptr)
// 	{
// 		cout << "(" << pos->data->first << ", " << (pos->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << "): ";
// 		printf("left: --\n");
// 	}
// 	else
// 	{
// 		cout << "(" << pos->data->first << ", " << (pos->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << "): ";
// 		printf("left: (");
// 		cout << pos->left->data->first << ", " << (pos->left->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << ")\n";
// 		print(pos->left);
// 	}
// 	if(pos->right == nullptr)
// 	{
// 		cout << "(" << pos->data->first << ", " << (pos->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << "): ";
// 		printf("right: --\n");
// 	}
// 	else
// 	{
// 		cout << "(" << pos->data->first << ", " << (pos->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << "): ";
// 		printf("right: (");
// 		cout << pos->right->data->first << ", " << (pos->right->colour == sjtu::map<char *, int, chrcmp>::Noir ? "BLACK" : "RED") << ")\n";
// 		print(pos->right);
// 	}
// }
// void printtree()
// {
// 	// sjtu::map<char *, int, chrcmp>::node *pt = mp.index->father;
// 	// mp.print(pt);
// 	mp.printtree();
// }

// int main()
// {
// 	char *str = "bon";
// 	mp.insert(sjtu::pair<char *, int>(str, 1));
// 	cout << mp["bon"] << endl;
// 	mp["mal"] = 277;
// 	cout << mp["bon"] << endl;
// 	sjtu::map<char *, int, chrcmp>::iterator bit = mp.begin();
// 	sjtu::map<char *, int, chrcmp>::iterator eit = mp.end();
// 	cout << ((*bit).second) << endl;
// 	// --it;
// 	eit--;
// 	cout << ((*eit).second) << endl;
// 	// cout << ((*bit).second) << endl;
// 	cout << "ttl = " << mp.size() << endl;
// 	// eit--;
// 	mp["123123"] = 10;
// 	mp["296456"] = 144;
// 	mp["jaohtwo"] = 986;
// 	mp["jaaa"] = 4793;
// 	mp["z"] = -273;
// 	mp["0019"] = 19;
// 	mp["break"] = -472;
// 	mp["str"] = 90;
// 	mp["zzz"] = 1;
// 	mp["zsm"] = 0;
// 	mp["zsz"] = 10;
// 	mp["yrers"] = 7;
// 	mp["rsbs"] = 9;
// 	mp["aspst"] = 47;
// 	mp["==sdf"] = 79;
// 	// mp["67"] = 67;
// 	// mp["wind"] = 14;
// 	// mp["jhtwo"] = 986;
// 	// mp["ja"] = 4793;
// 	// mp["sdtz"] = -273;
// 	// mp["00"] = 19;
// 	// mp["JetuAime"] = -472;
// 	// mp["strrstr"] = 90;
// 	// mp["strcpy"] = 1;
// 	// mp["_z``sm"] = 0;
// 	// mp["z_sz"] = 10;
// 	// mp["asdfsdfyrefd rs"] = 7;
// 	// mp["1!2`rsbs"] = 9;
// 	// mp["sdfs`e34tpst"] = 47;
// 	// mp["+sdf"] = 79;
// 	// mp["_y uty00 67"] = 67;
// 	// mp["nonwind"] = 14;
// 	// mp["004t3454519"] = 19;
// 	// mp["bfghreak"] = -472;
// 	// mp["sghf3tr"] = 90;
// 	// mp["zz z"] = 1;
// 	// mp["zs33m"] = 0;
// 	// mp["z3szrrr"] = 10;
// 	// mp["yreefrs"] = 7;
// 	// mp["rsbs"] = 9;
// 	// mp["as34t3pst"] = 47;
// 	// mp["=3=sydfsdf"] = 79;
// 	// mp["6r7"] = 67;
// 	// mp["w43tind"] = 14;
// 	// mp["jet43htwo"] = 986;
// 	// mp["jaert`"] = 4793;
// 	// mp["s` fdtz"] = -273;
// 	// mp["0`````0"] = 19;
// 	// mp["Jet```uAime"] = -472;
// 	// mp["st`rrstr"] = 90;
// 	// mp["strcprey"] = 1;
// 	// mp["_z465sm"] = 0;
// 	// mp["z_6sz"] = 10;
// 	// mp["yr789efdert rs"] = 7;
// 	// mp["1!r`t789sbs"] = 9;
// 	// mp["sd`e90980fspst"] = 47;
// 	// mp["+sdweff"] = 79;
// 	// mp["_007=-= 67"] = 67;
// 	// mp["no0nwhgjtind"] = 14;
// 	// printtree();
// 	char *newstr = "coeur";
// 	sjtu::pair<sjtu::map<char *, int, chrcmp>::iterator, bool> pr = mp.insert(sjtu::pair<char *, int>(newstr, 35));
// 	bit = pr.first;
// 	printtree();
// 	mp.erase(bit);
// 	printtree();
// 	eit = mp.begin();
// 	cout << mp.size() << endl;
// 	for(int i = 1; i <= mp.size(); i++)
// 	{
// 		cout << "<" << ((*eit).first) << ", " << ((*eit).second) << ">\n";
// 		// if(eit.pointer->father != nullptr)
// 		// 	cout << "father = " << eit.pointer->father->data.first << "\t";
// 		// if(eit.pointer->left != nullptr)
// 		// 	cout << "left = " << eit.pointer->left->data.first << "\t";
// 		// if(eit.pointer->right != nullptr)
// 		// 	cout << "right = " << eit.pointer->right->data.first << "\t";
// 		++eit;
// 	}
// 	// mp.erase(bit);
// 	printtree();
// 	while(mp.size() > 0)
// 	{
// 		sjtu::map<char *, int, chrcmp>::iterator iter = mp.end();
// 		--iter;
// 		cout << "\ndelete " << ((*iter).first) << endl;
// 		mp.erase(iter);
// 		printtree();
// 		cout << "\n\n";
// 	}

// }

#include <ctime>
#include <cstdlib>
#include <vector>

const std::vector<int> & generator(int n = 50001) {
	static std::vector<int> raw;
	raw.clear();
	for (int i = 0; i < n; i++) {
		raw.push_back(rand());
	}
	return raw;
}


int main()
{
	srand(time(NULL));
	auto ret = generator(51);
	for(int i = 0; i < 51; i++)
	{
		// mp[rand()] = i;
		cout << "ret[" << i << "] = " << ret[i] << endl;
		mp[ret[i]] = i;
		cout << mp.begin()->first << endl;
	}
	mp.printtree();

	// sjtu::map<int, int>::iterator iter = mp.end();
	// for(int i = 1; i <= 1000; i++)
	// {
	// 	iter = mp.end();
	// 	for(int i = 1; i <= 1000; i++)
	// 		--iter;
	// 	mp.erase(iter);
	// }

	for(auto iter = mp.begin(); iter != mp.end(); ++iter)
	{
		cout << (iter->first) << '\t';
	}

	// while(mp.size() > 0)
	// {
	// 	iter = mp.end();
	// 	--iter;
	// 	cout << "\ndelete " << (iter->first) << endl;
	// 	mp.erase(iter);
	// 	// printtree();
	// 	cout << "\n";
	// }
}