#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>


namespace sjtu
{
template <typename T>
class vector
{
private:
	const int Init_Max_Size = 64;
	int *identity;
	T *data;
	int totalLength;
	int maxSize;
	std::allocator<T> alloc;

	void doubleSpace()
	{
		T *temp = data;
		data = alloc.allocate(maxSize << 1);
		for(int i = 0; i < totalLength; i++)
		{
			alloc.construct(data + i, temp[i]);
			alloc.destroy(temp + i);
		}
		alloc.deallocate(temp, maxSize);
		maxSize = maxSize * 2;

		// data = temp;
		_first.reset(data);
		_last.reset(data + totalLength);
		_MemAllocEnd.reset(data + maxSize);
	}

public:
	class const_iterator;
	class iterator
	{
		friend class vector<T>;
		friend class const_iterator;
	private:
		T *index;
		vector *link;
	public:
		// static void setIdentity()
		// {
		// 	first = identity;
		// }
		iterator(): index(nullptr) {}
		
		iterator(T *pos): index(pos) {}

		iterator(T *pos, vector *id)
		{
			index = pos;
			link = id;
		}

		iterator(const iterator &source_it)
		{
			index = source_it.index;
			link = source_it.link;
		}

		iterator& reset(T *pos)
		{
			index = pos;
			return *this;
		}
		
		iterator& reset(T *pos, vector *l)
		{
			index = pos;
			link = l;
			return *this;
		}

		const T* position() const
		{
			return index;
		}
		
		iterator operator + (const int &n)
		{
			int _index = *this - link->_first;
			if(_index + n >= link->totalLength)
			{
				throw index_out_of_bound();
			}
			iterator ret = *this;
			ret.index += n;
			return ret;
		}

		iterator operator - (const int &n)
		{
			int _index = *this - link->_first;
			if(_index - n < 0)
			{
				throw index_out_of_bound();
			}
			iterator ret = *this;
			ret.index -= n;
			return ret;
		}

		int operator - (const iterator &rhs) const
		{
			// if(std::exception::what())
			if(link != rhs.link)
			{
				throw invalid_iterator();
			}
			return (index - rhs.index);
		}

		iterator& operator += (const int &n)
		{
			int _index = *this - link->_first;
			if(_index + n >= link->totalLength)
			{
				throw index_out_of_bound();
			}
			index += n;
			return *this;
		}

		iterator& operator -= (const int &n)
		{
			int _index = *this - link->_first;
			if(_index - n < 0)
			{
				throw index_out_of_bound();
			}
			index -= n;
			return *this;
		}

		iterator operator ++ (int)
		{
			if(*this == link->_last)
			{
				throw index_out_of_bound();
			}
			iterator ret = *this;
			index += 1;
			return ret;
		}

		iterator& operator ++ ()
		{
			if(*this == link->_last)
			{
				throw index_out_of_bound();
			}
			index++;
			return *this;
		}

		iterator operator -- (int)
		{
			if(*this == link->_last)
			{
				throw index_out_of_bound();
			}
			iterator ret = *this;
			index -= 1;
			return ret;
		}

		iterator& operator -- ()
		{
			if(*this == link->_first)
			{
				throw index_out_of_bound();
			}
			index--;
			return *this;
		}

		T & operator * () const
		{
			return *index;
		}

		bool operator == (const iterator &rhs) const
		{
			// if not the same vector?
			if(rhs.index == index)
				return true;
			return false;
		}

		bool operator == (const const_iterator &rhs) const
		{
			if(rhs.index == index)
				return true;
			return false;
		}

		bool operator != (const iterator &rhs) const
		{
			if(rhs.index == index)
				return false;
			return true;
		}

		bool operator != (const const_iterator &rhs) const
		{
			if(rhs.index == index)
				return false;
			return true;
		}
	};

	class const_iterator
	{
		friend class vector<T>;
		friend class iterator;
	private:
		T *index;
		const vector *link;
	public:
		// static void setIdentity()
		// {
		// 	first = identity;
		// }
		const_iterator(): index(nullptr) {}
		
		const_iterator(T *pos): index(pos) {}

		const_iterator(T *pos, const vector *id)
		{
			index = pos;
			link = id;
		}

		const_iterator(const const_iterator &source_it)
		{
			index = source_it.index;
			link = source_it.link;
		}

		const_iterator& reset(T *pos)
		{
			index = pos;
			return *this;
		}
		
		iterator& reset(T *pos, vector *l)
		{
			index = pos;
			link = l;
			return *this;
		}

		const T* position() const
		{
			return index;
		}
		
		const_iterator operator + (const int &n)
		{
			int _index = *this - link->_first;
			if(_index + n >= link->totalLength)
			{
				throw index_out_of_bound();
			}
			const_iterator ret = *this;
			ret.index += n;
			return ret;
		}

		const_iterator operator - (const int &n)
		{
			int _index = *this - link->_first;
			if(_index - n < 0)
			{
				throw index_out_of_bound();
			}
			const_iterator ret = *this;
			ret.index -= n;
			return ret;
		}

		int operator - (const const_iterator &rhs) const
		{
			// if(std::exception::what())
			if(link != rhs.link)
			{
				throw invalid_iterator();
			}
			return (index - rhs.index);
		}

		const_iterator& operator += (const int &n)
		{
			int _index = *this - link->_first;
			if(_index + n >= link->totalLength)
			{
				throw index_out_of_bound();
			}
			index += n;
			return *this;
		}

		const_iterator& operator -= (const int &n)
		{
			int _index = *this - link->_first;
			if(_index - n < 0)
			{
				throw index_out_of_bound();
			}
			index -= n;
			return *this;
		}

		const_iterator operator ++ (int)
		{
			if(*this == link->_last)
			{
				throw index_out_of_bound();
			}
			const_iterator ret = *this;
			index += 1;
			return ret;
		}

		const_iterator& operator ++ ()
		{
			if(*this == link->_last)
			{
				throw index_out_of_bound();
			}
			index++;
			return *this;
		}

		const_iterator operator -- (int)
		{
			if(*this == link->_last)
			{
				throw index_out_of_bound();
			}
			const_iterator ret = *this;
			index -= 1;
			return ret;
		}

		const_iterator& operator -- ()
		{
			if(*this == link->_first)
			{
				throw index_out_of_bound();
			}
			index--;
			return *this;
		}

		T & operator * () const
		{
			return *index;
		}

		// T & operator * () const
		// {
		// 	return *index;
		// }
		// friend T & operator * (const iterator _it);

		bool operator == (const iterator &rhs) const
		{
			// if not the same vector?
			if(rhs.index == index)
				return true;
			return false;
		}

		bool operator == (const const_iterator &rhs) const
		{
			if(rhs.index == index)
				return true;
			return false;
		}

		bool operator != (const iterator &rhs) const
		{
			if(rhs.index == index)
				return false;
			return true;
		}

		bool operator != (const const_iterator &rhs) const
		{
			if(rhs.index == index)
				return false;
			return true;
		}
	};
	
	iterator _first;
	iterator _last;
	iterator _MemAllocEnd;

	vector()
	{
		maxSize = Init_Max_Size;
		// data = new T [maxSize];
		data = alloc.allocate(maxSize);
		totalLength = 0;
		// identity = new int(totalLength);

		_first.reset(data, this);
		_last.reset(data + totalLength, this);
		_MemAllocEnd.reset(data + maxSize, this);
	}
	vector(const vector &other)
	{
		maxSize = other.maxSize;
		totalLength = other.totalLength;
		// data = new T [maxSize];
		data = alloc.allocate(maxSize);
		for(int i = 0; i < totalLength; i++)
		{
			// data[i] = other.data[i];
			alloc.construct(data + i, other.data[i]);
		}
		// identity = new int(totalLength);
		
		_first.reset(data, this);
		_last.reset(data + totalLength, this);
		_MemAllocEnd.reset(data + maxSize, this);
	}

	~vector()
	{
		for(int i = 0; i < totalLength; i++)
			alloc.destroy(data + i);
		// delete [] data;
		alloc.deallocate(data, maxSize);
		// delete identity;
	}

	vector &operator = (const vector &other)
	{
		// maxSize = other.maxSize;
		// totalLength = other.totalLength;
		T *temp = alloc.allocate(other.maxSize);
		for(int i = 0; i < other.totalLength; i++)
		{
			alloc.construct(temp + i, other.data[i]);
		}
		for(int i = 0; i < totalLength; i++)
			alloc.destroy(data + i);
		alloc.deallocate(data, maxSize);
		data = temp;
		maxSize = other.maxSize;
		totalLength = other.totalLength;
		
		_first.reset(data, this);
		_last.reset(data + totalLength, this);
		_MemAllocEnd.reset(data + maxSize, this);
		return *this;
	}

	T & at(const size_t &pos)
	{
		if(pos >= totalLength || pos < 0)
			throw index_out_of_bound();
		return data[pos];
	}

	const T & at(const size_t &pos) const
	{
		if(pos >= totalLength || pos < 0)
			throw index_out_of_bound();
		return data[pos];
	}

	T & operator [] (const size_t &pos)
	{
		if(pos < 0 || pos >= totalLength)
			throw index_out_of_bound();
		// std::cerr << "data[pos] = " << data[pos] << std::endl;
		return data[pos];
	}

	const T & operator [] (const size_t &pos) const
	{
		if(pos < 0 || pos >= totalLength)
			throw index_out_of_bound();
		// std::cerr << "data[pos] = " << data[pos] << std::endl;
		return data[pos];
	}

	const T & front() const
	{
		return data[0];
	}

	const T & back() const
	{
		return data[totalLength - 1];
	}

	iterator begin()
	{
		iterator ret(data, this);
		return ret;
	}

	const_iterator cbegin() const
	{
		const_iterator ret(data, this);
		return ret;
	}

	iterator end()
	{
		iterator ret(data + totalLength, this);
		return ret;
	}

	const_iterator cend() const
	{
		const_iterator ret(data + totalLength, this);
		return ret;
	}

	bool empty() const
	{
		if(totalLength <= 0)
			return true;
		return false;
	}

	size_t size() const
	{
		// return (size_t)(sizeof(data[0])) * totalLength;
		return (size_t)totalLength;
	}

	void clear()
	{
		// memset(data, 0, sizeof(data));
		totalLength = 0;
		_last = _first;
	}

	iterator insert(iterator pos, const T &value)
	{
		int ind = pos.position() - data;
		if(ind < 0 || ind >= totalLength)
			throw index_out_of_bound();
		if(totalLength + 1 == maxSize)
			doubleSpace();
		totalLength++;
		alloc.construct(data + totalLength - 1, data[totalLength - 2]);
		for(int i = totalLength - 1; i > ind; i--)
		{
			data[i] = data[i - 1];
		}
		_last.index++;
		data[ind] = value;
		// pos.index = *data[ind];
		pos.reset(data + ind, this);
		return pos;
	}

	iterator insert(const size_t &ind, const T &value)
	{
		if(ind > totalLength || ind < 0)
			throw index_out_of_bound();
		if(totalLength + 1 == maxSize)
			doubleSpace();
		totalLength++;
		alloc.construct(data + totalLength - 1, data[totalLength - 2]);
		for(int i = totalLength - 1; i > ind; i--)
		{
			data[i] = data[i - 1];
		}
		_last.index++;
		// data[ind] = value;
		this->at(ind) = value;
		iterator ret(data + ind, this);
		return ret;
	}

	iterator erase(iterator pos)
	{
		int ind = pos.position() - data;
		if(ind < 0 || ind >= totalLength)
			throw index_out_of_bound();
		for(int i = ind; i < totalLength - 1; i++)
		{
			data[i] = data[i + 1];
		}
		alloc.destroy(data + totalLength - 1);
		totalLength--;
		if(ind == totalLength - 1)
		{
			return end();
		}
		_last.index--;
		// pos.index = *data[ind];
		pos.reset(data + ind, this);
		return pos;
	}

	iterator erase(const size_t &ind)
	{
		if(ind >= totalLength || ind < 0)
			throw index_out_of_bound();
		for(int i = ind; i < totalLength - 1; i++)
		{
			data[i] = data[i + 1];
		}
		alloc.destroy(data + totalLength - 1);
		_last.index--;
		totalLength--;
		iterator ret(data + ind, this);
		return data;
	}

	void push_back(const T &value)
	{
		if(totalLength + 1 == maxSize)
			doubleSpace();
		totalLength++;
		// data[totalLength - 1] = value;
		alloc.construct(data + totalLength - 1, value);
		_last.index++;
	}
	
	void pop_back()
	{
		totalLength--;
		// data[totalLength] = 0;
		alloc.destroy(data + totalLength);
		_last.index--;
	}
};

}

#endif