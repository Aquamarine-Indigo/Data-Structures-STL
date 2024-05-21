#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
	Compare cmp;
	int maxSize;
	int totalNum;
	T *data;
	std::allocator<T> alloc;
	void doubleSpace()
	{
		T *temp = data;
		// data = new T [maxSize];
		data = alloc.allocate(maxSize << 1);
		for(int i = 1; i <= totalNum; i++)
		{
			// data[i] = temp[i];
			alloc.construct(data + i, temp[i]);
			alloc.destroy(temp + i);
		}
		// delete [] temp;
		alloc.deallocate(temp, maxSize);
		maxSize <<= 1;
	}
	void perlocateUp(int hole, const T &val)
	{
		while(hole > 1 && cmp(data[hole >> 1], val) == true)
		{
			data[hole] = data[hole >> 1];
			hole >>= 1;
		}
		data[hole] = val;
	}
	void perlocateDown(int hole)
	{
		int children;
		T temp = data[hole];
		while((hole << 1) <= totalNum)
		{
			children = hole << 1;
			if(children + 1 <= totalNum && cmp(data[children], data[children + 1]) == true)
				children++;
			if(cmp(temp, data[children]) == true)
				data[hole] = data[children];
			else
				break;
			hole = children;
		}
		data[hole] = temp;
	}

public:
	/**
	 * TODO constructors
	 */
	priority_queue()
	{
		maxSize = 128;
		totalNum = 0;
		// data = new T [maxSize];
		data = alloc.allocate(maxSize);
	}

	priority_queue(const priority_queue &other)
	{
		maxSize = other.maxSize;
		totalNum = other.totalNum;
		// data = new T [maxSize];
		data = alloc.allocate(maxSize);
		for(int i = 1; i <= totalNum; i++)
		{
			// data[i] = other.data[i];
			alloc.construct(data + i, other.data[i]);
		}
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue()
	{
		// delete [] data;
		for(int i = 1; i <= totalNum; i++)
			alloc.destroy(data + i);
		alloc.deallocate(data, maxSize);
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other)
	{
		if(this == &other)
			return *this;
		// delete [] data;
		for(int i = 1; i <= totalNum; i++)
			alloc.destroy(data + i);
		alloc.deallocate(data, maxSize);
		maxSize = other.maxSize;
		totalNum = other.totalNum;
		// data = new T [maxSize];
		data = alloc.allocate(maxSize);
		for(int i = 1; i <= totalNum; i++)
		{
			// data[i] = other.data[i];
			alloc.construct(data + i, other.data[i]);
		}
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
		return data[1];
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e)
	{
		if(totalNum + 1 >= maxSize)
			doubleSpace();
		totalNum++;
		// data[totalNum] = e;
		alloc.construct(data + totalNum, e);
		perlocateUp(totalNum, e);
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
		data[1] = data[totalNum];
		alloc.destroy(data + totalNum);
		totalNum--;
		perlocateDown(1);
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const
	{
		return totalNum;
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
};

}

#endif