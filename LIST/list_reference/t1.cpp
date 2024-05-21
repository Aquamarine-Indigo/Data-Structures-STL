#include <iostream>
#include <cstdio>
using namespace std;
#define MIN(a, b) (a < b ? a : b)

struct node
{
	int value;
	node *next;
	node *prev;

	node(): next(nullptr), prev(nullptr) {}
	node(const int &val, node *p = nullptr, node *n = nullptr): next(n), prev(p)
	{
		value = val;
	}

	~node() = default;
};

class LongLongInt
{
private:
	node *head, *tail;
	int len;
	bool reverse;
	void add_node(node *pos, node *cur)
	{
		node *p = pos->prev;
		p->next = cur;
		cur->prev = p;

		pos->prev = cur;
		cur->next = pos;
	}
	void del_node(node *pos)
	{
		pos->prev->next = pos->next;
		pos->next->prev = pos->prev;
		delete pos;
	}

public:
	LongLongInt()
	{
		head = new node();
		tail = new node();
		head->next = tail;
		tail->prev = head;
		len = 0;
		reverse = false;
	}

	LongLongInt(const string &tar)
	{
		head = new node();
		tail = new node();
		head->next = tail;
		tail->prev = head;
		reverse = false;

		len = tar.length();
		for(int i = 0; i < len; i++)
		{
			if(tar[i] == '-')
			{
				reverse = true;
				len--;
				continue;
			}
			node *add = new node(int(tar[i] - '0'));
			add_node(tail, add);
		}
	}

	LongLongInt(const LongLongInt &other)
	{
		head = new node();
		tail = new node();
		head->next = tail;
		tail->prev = head;

		reverse = other.reverse;
		len = other.len;
		node *p = other.head->next;
		for(int i = 0; i < len; i++)
		{
			node *add = new node(p->value);
			add_node(tail, add);
			p = p->next;
		}
	}

	void clear()
	{
		while(len > 0)
		{
			del_node(head->next);
			len--;
		}
	}

	~LongLongInt()
	{
		while(len > 0)
		{
			del_node(head->next);
			len--;
		}
		delete head;
		delete tail;
	}

	LongLongInt &operator = (const LongLongInt &other)
	{
		clear();
		len = other.len;
		reverse = other.reverse;
		node *p = other.head->next;
		for(int i = 0; i < len; i++)
		{
			node *add = new node(int(p->value));
			add_node(tail, add);
			p = p->next;
		}
		return *this;
	}

	/**
	 * Add a digit tar into position pos
	 * 0-based, Count from tail->prev
	 */
	void addNumber(int tar, int pos)
	{
		len++;
		node *position = tail;
		while(pos--)
		{
			position = position->prev;
		}
		node *added = new node(tar);
		add_node(position, added);
	}

	friend LongLongInt operator - (LongLongInt a, LongLongInt b);
	friend bool operator < (const LongLongInt &a, const LongLongInt &b);
	friend bool operator == (const LongLongInt &a, const LongLongInt &b);

	void print()
	{
		if(reverse == true)
		{
			cout << '-';
		}
		node *cur = head->next;
		while(cur != tail)
		{
			cout << cur->value;
			cur = cur->next;
		}
	}

	/**
	 * delete zeros in the front of the LongLongInt
	 */
	void del_front_zero()
	{
		node *p = head->next;
		while(p->value == 0)
		{
			del_node(p);
			len--;
			p = head->next;
		}
	}

};

bool operator < (const LongLongInt &a, const LongLongInt &b)
{
	if(a.len < b.len)
		return true;
	else if(a.len > b.len)
		return false;

	node *a_cur = a.head->next;
	node *b_cur = b.head->next;
	for(int i = 0; i < a.len; i++)
	{
		if(a_cur->value < b_cur->value)
			return true;
		else if(a_cur->value > b_cur->value)
			return false;
		a_cur = a_cur->next;
		b_cur = b_cur->next;
	}
	return false;
}

bool operator == (const LongLongInt &a, const LongLongInt &b)
{
	if(a.len != b.len)
		return false;
	node *a_cur = a.head->next;
	node *b_cur = b.head->next;
	for(int i = 0; i < a.len; i++)
	{
		if(a_cur->value != b_cur->value)
			return false;
		a_cur = a_cur->next;
		b_cur = b_cur->next;
	}
	return true;
}

/**
 * a and b must be positive
 */
LongLongInt operator - (LongLongInt a, LongLongInt b)
{
	if(a == b)
	{
		return LongLongInt(string("0"));
	}
	LongLongInt answer;
	int buf = 0;
	int borrowing = 0;

	bool reverse_res = false;
	if(a < b)
	{
		answer = a;
		a = b;
		b = answer;
		answer.clear();
		reverse_res = true;
	}
	node *a_cur = a.tail->prev;
	node *b_cur = b.tail->prev;
	int minlen = MIN(a.len, b.len);

	for(int i = 0; i < minlen; i++)
	{
		buf = a_cur->value - b_cur->value - borrowing;
		if(buf < 0)
		{
			borrowing = 1;
			buf += 10;
		}
		else
			borrowing = 0;
		answer.addNumber(buf, i);
		a_cur = a_cur->prev;
		b_cur = b_cur->prev;
	}

	if(a.len > b.len)
	{
		for(int i = minlen; i < a.len; i++)
		{
			buf = a_cur->value - borrowing;
			if(buf < 0)
			{
				borrowing = 1;
				buf += 10;
			}
			else
				borrowing = 0;
			answer.addNumber(buf, i);
			a_cur = a_cur->prev;
		}
	}
	else if(b.len > a.len)
	{
		for(int i = minlen; i < b.len; i++)
		{
			buf = b_cur->value - borrowing;
			if(buf < 0)
			{
				borrowing = 1;
				buf += 10;
			}
			else
				borrowing = 0;
			answer.addNumber(buf, i);
			b_cur = b_cur->prev;
		}
	}
	answer.reverse = reverse_res;
	answer.del_front_zero();
	return answer;
}

int main()
{
	string a, b;
	cin >> a >> b;
	LongLongInt la(a), lb(b);
	// la.print();
	// lb.print();
	LongLongInt res = la - lb;
	res.print();
	cout << endl;
	return 0;
}