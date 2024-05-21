#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
template<class T>
class list
{
protected:
	class node
	{
	public:
		T *value;
		bool is_constructed;
		// previous node
		node *prev;
		// next node
		node *next;

		node(): next(nullptr), prev(nullptr) { is_constructed = false; }
		node(const T &val, node *p = nullptr, node *n = nullptr): prev(p), next(n)
		{
			value = new T(val);
			is_constructed = true;
		}
		~node()
		{
			if(is_constructed == true)
				delete value;
		}

		void init_value(const T &val)
		{
			value = new T(val);
			is_constructed = true;
		}

		void change_value(const T &val)
		{
			if(is_constructed == true)
				delete value;
			value = new T(val);
			is_constructed = true;
		}
	};
	
	/**
	 * returns (const T&)a < (const T&)b
	 */
	static bool __cmp(const T &a, const T &b)
	{
		return a < b;
	}

protected:
	// std::allocator<T> alloc;
	node *head, *tail;
	int totalLength;

	/**
	 * insert node cur before node pos
	 * return the inserted node cur
	 */
	node *insert(node *pos, node *cur)
	{
		node *before = pos->prev;
		before->next = cur;
		cur->prev = before;
		pos->prev = cur;
		cur->next = pos;
		return cur;
	}

	/**
	 * remove node pos from list but do not delete the node
	 * return the removed node pos
	 */
	node *erase(node *pos)
	{
		node *prev_pos = pos->prev;
		node *next_pos = pos->next;
		prev_pos->next = next_pos;
		next_pos->prev = prev_pos;
		return pos;
	}

public:
	friend class iterator;
	friend class const_iterator;

	class const_iterator;
	class iterator
	{
	private:
		node *position;
		bool is_valid;
		list *pt;

	public:
		friend class list;
		iterator(): position(nullptr), is_valid(false) {}
		iterator(node *cursor, list *pointer)
		{
			position = cursor;
			is_valid = true;
			pt = pointer;
		}
		iterator(const iterator &other)
		{
			position = other.position;
			is_valid = other.is_valid;
			pt = other.pt;
		}

		/**
		 * iter++
		 */
		iterator operator ++ (int)
		{
			if(position == pt->tail)
				throw index_out_of_bound();
			iterator res = *this;
			position = position->next;
			return res;
		}

		/**
		 * ++iter
		 */
		iterator & operator ++ ()
		{
			if(position == pt->tail)
				throw index_out_of_bound();
			position = position->next;
			return *this;
		}

		/**
		 * iter--
		 */
		iterator operator -- (int)
		{
			if(position == pt->head->next)
				throw index_out_of_bound();
			iterator res = *this;
			position = position->prev;
			return res;
		}

		/**
		 * --iter
		 */
		iterator & operator -- ()
		{
			if(position == pt->head->next)
				throw index_out_of_bound();
			position = position->prev;
			return *this;
		}

		/**
		 * TODO *it
		 * throw invalid_iterator if the iterator is invalid
		 */
		T & operator * () const
		{
			if(position == pt->head || position == pt->tail)
				throw invalid_iterator();
			if(is_valid == false)
				throw invalid_iterator();
			return *(position->value);
		}

		/**
		 * TODO it->field
		 * throw invalid_iterator if the iterator is invalid
		 */
		T * operator -> () const noexcept
		{
			if(position == pt->head || position == pt->tail)
				throw invalid_iterator();
			if(is_valid == false)
				throw invalid_iterator();
			return position->value;
		}

		/**
		 * An operator to check whether the two iterators are the same
		 * (pointing to the same memory address)
		 */
		bool operator == (const iterator &rhs) const
		{
			if(is_valid == false || rhs.is_valid == false)
				return false;
			if(position == rhs.position)
				return true;
			return false;
		}
		bool operator == (const const_iterator &rhs) const
		{
			if(is_valid == false || rhs.is_valid == false)
				return false;
			if(position == rhs.position)
				return true;
			return false;
		}

		/**
		 * An operator to check if the two iterators are different
		 */
		bool operator != (const iterator &rhs)
		{
			if(is_valid == false || rhs.is_valid == false)
				return true;
			if(position == rhs.position)
				return false;
			return true;
		}
		bool operator != (const const_iterator &rhs)
		{
			if(is_valid == false || rhs.is_valid == false)
				return true;
			if(position == rhs.position)
				return false;
			return true;
		}
	};
	
	class const_iterator
	{

	private:
		node *position;
		bool is_valid;
		const list *pt;

	public:
		friend class list;
		const_iterator(): position(nullptr), is_valid(false) {}
		const_iterator(node *cursor, const list *pointer)
		{
			position = cursor;
			is_valid = true;
			pt = pointer;
		}
		const_iterator(const iterator &other)
		{
			position = other.position;
			is_valid = other.is_valid;
			pt = other.pt;
		}
		const_iterator(const const_iterator &other)
		{
			position = other.position;
			is_valid = other.is_valid;
			pt = other.pt;
		}

		/**
		 * iter++
		 */
		const_iterator operator ++ (int)
		{
			if(position == pt->tail)
				throw index_out_of_bound();
			iterator res = *this;
			position = position->next;
			return res;
		}

		/**
		 * ++iter
		 */
		const_iterator & operator ++ ()
		{
			if(position == pt->tail)
				throw index_out_of_bound();
			position = position->next;
			return *this;
		}

		/**
		 * iter--
		 */
		const_iterator operator -- (int)
		{
			if(position == pt->head->next)
				throw index_out_of_bound();
			iterator res = *this;
			position = position->prev;
			return res;
		}

		/**
		 * --iter
		 */
		const_iterator & operator -- ()
		{
			if(position == pt->head->next)
				throw index_out_of_bound();
			position = position->prev;
			return *this;
		}

		/**
		 * TODO *it
		 * throw invalid_iterator if the iterator is invalid
		 */
		T operator * () const
		{
			if(position == pt->head || position == pt->tail)
				throw invalid_iterator();
			if(is_valid == false)
				throw invalid_iterator();
			return *(position->value);
		}

		/**
		 * TODO it->field
		 * throw invalid_iterator if the iterator is invalid
		 */
		T * operator -> () const noexcept
		{
			if(position == pt->head || position == pt->tail)
				throw invalid_iterator();
			if(is_valid == false)
				throw invalid_iterator();
			return position->value;
		}

		/**
		 * An operator to check whether the two iterators are the same
		 * (pointing to the same memory address)
		 */
		bool operator == (const iterator &rhs) const
		{
			if(is_valid == false || rhs.is_valid == false)
				return false;
			if(position == rhs.position)
				return true;
			return false;
		}
		bool operator == (const const_iterator &rhs) const
		{
			if(is_valid == false || rhs.is_valid == false)
				return false;
			if(position == rhs.position)
				return true;
			return false;
		}

		/**
		 * An operator to check if the two iterators are different
		 */
		bool operator != (const iterator &rhs)
		{
			if(is_valid == false || rhs.is_valid == false)
				return true;
			if(position == rhs.position)
				return false;
			return true;
		}
		bool operator != (const const_iterator &rhs)
		{
			if(is_valid == false || rhs.is_valid == false)
				return true;
			if(position == rhs.position)
				return false;
			return true;
		}
	};

	/**
	 * Default Constructor
	 */
	list()
	{
		head = new node();
		tail = new node();
		head->next = tail;
		head->prev = nullptr;
		tail->prev = head;
		tail->next = nullptr;
		totalLength = 0;
	}

	/**
	 * Copy Constructor
	 */
	list(const list &other)
	{
		head = new node();
		tail = new node();
		head->next = tail;
		head->prev = nullptr;
		tail->prev = head;
		tail->next = nullptr;
		totalLength = other.totalLength;

		node *p = other.head->next;
		for(int i = 0; i < totalLength; i++)
		{
			node *added_node = new node(*(p->value));
			insert(tail, added_node);
			p = p->next;
		}
	}

	virtual ~list()
	{
		clear();
		delete head;
		delete tail;
	}

	/**
	 * Assignment operator
	 */
	list &operator = (const list other)
	{
		clear();
		totalLength = other.totalLength;

		node *p = other.head->next;
		for(int i = 0; i < totalLength; i++)
		{
			node *added_node = new node(*(p->value));
			insert(tail, added_node);
			p = p->next;
		}
		return *this;
	}

	/**
	 * Access the first element
	 * throw container_is_empty when the container is empty
	 */
	const T & front() const
	{
		if(totalLength == 0)
			throw container_is_empty();
		node *first = head->next;
		return *(first->value);
	}

	/**
	 * Access the last element
	 * throw container_is_empty when the container is empty
	 */
	const T & back() const
	{
		if(totalLength == 0)
			throw container_is_empty();
		node *last = tail->prev;
		return *(last->value);
	}

	/**
	 * returns an iterator pointing to the begining
	 */
	iterator begin()
	{
		iterator res(head->next, this);
		return res;
	}
	const_iterator cbegin() const
	{
		const_iterator res(head->next, this);
		return res;
	}

	/**
	 * returns an iterator pointing to the next element of the end
	 */
	iterator end()
	{
		iterator res(tail, this);
		return res;
	}
	const_iterator cend() const
	{
		const_iterator res(head->next, this);
		return res;
	}

	/**
	 * Clears the contents
	 */
	virtual void clear()
	{
		// while(totalLength > 0)
		// {
		// 	node *del = erase(head->next);
		// 	delete del;
		// 	totalLength--;
		// }
		node *p = head->next, *q;
		while(p != tail)
		{
			q = p->next;
			delete p;
			p = q;
		}
		head->next = tail;
		tail->prev = head;
		totalLength = 0;
	}

	/**
	 * checks whether the container is empty
	 */
	virtual bool empty() const
	{
		if(totalLength == 0)
			return true;
		return false;
	}

	/**
	 * returns the number of elements in the list
	 */
	virtual size_t size() const
	{
		return (size_t)totalLength;
	}

	/**
	 * insert value before pos (could be iterator end())
	 * return an iterator pointing to the inserted value
	 * throw if the iterator is invalid
	 */
	virtual iterator insert(iterator pos, const T &value)
	{
		if(pos.is_valid == false)
			throw invalid_iterator();
		if(pos.pt != this)
			throw invalid_iterator();
		node *add = new node(value);
		insert(pos.position, add);
		totalLength++;
		return iterator(add, this);
	}

	/**
	 * remove the element at position pos (iterator end() is invalid)
	 * return an iterator pointing to the following element
	 * if pos is pointing to the last element, iterator end() will be returned
	 * throw if the container is empty or the iterator is invalid
	 */
	virtual iterator erase(iterator pos)
	{
		if(pos.position == tail)
			throw invalid_iterator();
		if(pos.is_valid == false)
			throw invalid_iterator();
		if(pos.pt != this)
			throw invalid_iterator();
		if(totalLength == 0)
			throw container_is_empty();
		erase(pos.position);
		totalLength--;
		node *del = pos.position->next;
		delete pos.position;
		return iterator(del, this);
	}

	/**
	 * adds an element to the end of the list
	 */
	void push_back(const T &value)
	{
		node *buf = new node(value);
		insert(tail, buf);
		totalLength++;
	}
	/**
	 * remove the last element of the list
	 * throw container_is_empty() when the container is empty
	 */
	void pop_back()
	{
		if(totalLength == 0)
			throw container_is_empty();
		node *del = erase(tail->prev);
		delete del;
		totalLength--;
	}
	/**
	 * inserts an element to the beginning
	 */
	void push_front(const T &value)
	{
		node *buf = new node(value);
		insert(head->next, buf);
		totalLength++;
	}
	/**
	 * removes the first element
	 * throw container_is_empty() when the container is empty
	 */
	void pop_front()
	{
		if(totalLength == 0)
			throw container_is_empty();
		node *del = erase(head->next);
		delete del;
		totalLength--;
	}

	/**
	 * sort the elements in ascending order with the operator < of T
	 */
	void sort()
	{
		// T *data = new T [totalLength + 1];
		std::allocator<T> alloc;
		T *data = alloc.allocate(totalLength + 1);
		node *p = head->next;
		int cur = 0;
		while(p != tail)
		{
			// data[cur] = *(p->value);
			alloc.construct(data + cur, *(p->value));
			cur++;
			p = p->next;
		}
		sjtu::sort<T>(data + 0, data + totalLength, __cmp);
		p = head->next;
		cur = 0;
		while(p != tail)
		{
			p->change_value(data[cur]);
			cur++;
			p = p->next;
		}
		// delete [] data;
		for(int i = 0; i < totalLength; i++)
		{
			alloc.destroy(data + i);
		}
		alloc.deallocate(data, totalLength + 1);
	}

	/**
	 * merge two sorted lists into one (both in ascending order)
	 * compare with operator< of T
	 * container 'other' becomes empty after the operation
	 * for equivalent elements in the two lists, the elements from *this shall always precede the elements from 'other'
	 * the order of equivalent elements of *this and other does not change.
	 * no elements are copied or moved
	 */
	void merge(list &other)
	{
		node *this_cur = head->next;
		node *other_cur = other.head->next;
		// int minlen = totalLength;
		// if(other.totalLength < minlen)
		// 	minlen = other.totalLength;
		while(other_cur != other.tail && this_cur != tail)
		{
			if(*(other_cur->value) < *(this_cur->value))
			{
				node *del = other.erase(other_cur);
				other_cur = other_cur->next;
				this_cur = insert(this_cur->next, del);
			}
			else
			{
				this_cur = this_cur->next;
			}
		}
		if(this_cur == tail)
		{
			while(other_cur != other.tail)
			{
				node *del = other.erase(other_cur);
				other_cur = other_cur->next;
				insert(tail, del);
			}
		}
		totalLength += other.totalLength;
		other.totalLength = 0;
	}

	/**
	 * reverse the order of the elements
	 */
	void reverse()
	{
		// node *new_head = new node(tail);
		// node *new_tail = new node(head);
		node *p = head;
		node *p_next = p->next, *temp;
		while(p_next != nullptr)
		{
			p_next = p->next;
			p->next = p->prev;
			p->prev = p_next;
			p = p_next;
		}
		temp = head;
		head = tail;
		tail = temp;
	}

	/**
	 * remove all consequtive duplicate elements from the container
	 * only the first element in each consequtive duplicate elements is reserved
	 * use operator == of T for comparation
	 */
	void unique()
	{
		node *p = head->next;
		while(p->next != tail)
		{
			if(*(p->value) == *(p->next->value))
			{
				node *del = erase(p->next);
				totalLength--;
				delete del;
			}
			else
				p = p->next;
		}
	}

};

}

#endif // SJTU_LIST_HPP