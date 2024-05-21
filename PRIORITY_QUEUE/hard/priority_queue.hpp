#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"
#define MIN(a, b) (a < b ? a : b)

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue
{
	// less -> biggest on top
private:
	struct node
	{
		T key;
		node *left, *right;
		int npl;

		node(T val, node *l = nullptr, node *r = nullptr, int _ = 0): \
			key(val), left(l), right(r), npl(_) {}
		~node() = default;
	};
	node *root;
	int totalNum;
	Compare cmp;

	/**
	 * merge with two pointers *ltar and *rtar
	 * private function
	 */
	node* merge(node *ltar, node *rtar)
	{
		if(ltar == nullptr)
			return rtar;
		if(rtar == nullptr)
			return ltar;

		if(cmp(ltar->key, rtar->key) == true) // ltar < rtar
		{
			node *temp = rtar;
			rtar = ltar;
			ltar = temp;
		}
		// rtar <= ltar
		ltar->right = merge(ltar->right, rtar);

		if(ltar->left == nullptr || ltar->left->npl < ltar->right->npl)
		{
			node *temp = ltar->left;
			ltar->left = ltar->right;
			ltar->right = temp;
		}

		if(ltar->right == nullptr || ltar->left == nullptr)
			ltar->npl = 0;
		else
			ltar->npl = MIN(ltar->right->npl, ltar->left->npl) + 1;
		return ltar;
	}

	/**
	 * deconstruct and destoy the heap from certain pointer node *tar
	 * private function
	 */
	void destroy(node *tar)
	{
		if(tar == nullptr)
			return;
		
		if(tar->left != nullptr)
			destroy(tar->left);
		if(tar->right != nullptr)
			destroy(tar->right);
		delete tar;
	}

	/**
	 * clear the elements of the heap
	 * private function
	 */
	void clear()
	{
		destroy(root);
		totalNum = 0;
		root = nullptr;
	}

	void copy(node *tar, node *other)
	{
		// tar->key = other->key;
		tar->npl = other->npl;
		if(other->left != nullptr)
		{
			tar->left = new node(other->left->key);
			copy(tar->left, other->left);
		}
		else
			tar->left = nullptr;
		if(other->right != nullptr)
		{
			tar->right = new node(other->right->key);
			copy(tar->right, other->right);
		}
		else
			tar->right = nullptr;
	}

public:
	/**
	 * TODO constructors
	 */
	priority_queue()
	{
		root = nullptr;
		totalNum = 0;
	}

	priority_queue(const priority_queue &other)
	{
		totalNum = other.totalNum;
		root = new node(other.root->key);
		copy(root, other.root);
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue()
	{
		clear();
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other)
	{
		// priority_queue<T, Compare> buf(other);
		if(root == other.root)
			return *this;
		clear();
		totalNum = other.totalNum;
		root = new node(other.root->key);
		copy(root, other.root);
		return *this;
	}

	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const
	{
		if(totalNum == 0)
			throw container_is_empty();
		return root->key;
	}

	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e)
	{
		if(totalNum == 0)
			root = new node(e);
		else
		{
			node *p = new node(e);
			root = merge(root, p);
		}
		totalNum++;
	}

	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop()
	{
		if(totalNum == 0)
			throw container_is_empty();
		node *l = root->left;
		node *r = root->right;
		delete root;
		root = merge(l, r);
		totalNum--;
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const
	{
		return (size_t)totalNum;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const
	{
		if(totalNum == 0)
			return true;
		return false;
	}
	/**
	 * merge two priority_queues with at least O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other)
	{
		root = merge(root, other.root);
		other.root = nullptr;
		totalNum += other.totalNum;
		other.totalNum = 0;
	}
};

}

#endif