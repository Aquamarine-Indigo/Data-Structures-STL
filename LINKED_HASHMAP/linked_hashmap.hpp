#ifndef SJTU_LINKEDHASHMAP_HPP
#define SJTU_LINKEDHASHMAP_HPP

#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
#include <cstring>
// #include <iostream>


namespace sjtu 
{

template< class Key, class T,
	class Hash = std::hash<Key>, 
	class Equal = std::equal_to<Key> >
class linked_hashmap
{
public:
	typedef pair<const Key, T> value_type;
private:
	constexpr static float LoadFactor = 0.75;
	int Capacity;
	int totalLength;
	struct node
	{
		using value_type = typename linked_hashmap::value_type;
		node *prev, *next;
		node *order_prev, *order_next;
		// Key key;
		// T *value;
		value_type *value;
		bool val_constructed;
		node()
		{
			val_constructed = false;
			order_next = nullptr;
			order_prev = nullptr;
			prev = nullptr;
			next = nullptr;
		}
		node(const value_type &tar, node *ptar, node *ntar): prev(ptar), next(ntar)
		{
			value = new value_type (tar);
			val_constructed = true;
			order_next = nullptr;
			order_prev = nullptr;
			ptar->next = this;
			if(ntar != nullptr)
				ntar->prev = this;
		}
		node(const Key &k, T &val, node *ptar, node *ntar): prev(ptar), next(ntar)
		{
			value = new value_type (k, val);
			val_constructed = true;
			order_next = nullptr;
			order_prev = nullptr;
			ptar->next = this;
			if(ntar != nullptr)
				ntar->prev = this;
		}
		~node()
		{
			if(val_constructed == true)
				delete value;
		}
		void set_order(node *p, node *n)
		{
			order_next = n;
			order_prev = p;
		}
		value_type * pointer()
		{
			val_constructed = true;
			return value;
		}
		value_type & reference()
		{
			val_constructed = true;
			return (*value);
		}
	};
	node *head, *tail;
	node *data;

	node * orderLink_append(node *tar)
	{
		node *prev = tail->order_prev;
		tar->set_order(prev, tail);
		prev->order_next = tar;
		tail->order_prev = tar;
		return tar;
	}

	node * orderLink_delete(node *tar)
	{
		node *prev = tar->order_prev;
		node *next = tar->order_next;
		prev->order_next = next;
		next->order_prev = prev;
		return tar;
	}

	node * addNode(const Key &k, T &val)
	{
		size_t h = getHash(k) % Capacity;
		node *n = data[h].next;
		node *p = new node(k, val, &data[h], n);
		orderLink_append(p);
		p->val_constructed = true;
		return p;
	}

	node * addNode(const value_type &tar)
	{
		size_t h = getHash(tar.first) % Capacity;
		node *n = data[h].next;
		node *p = new node(tar, &data[h], n);
		orderLink_append(p);
		p->val_constructed = true;
		return p;
	}

	void delNode(node *tar)
	{
		node *p = tar->prev;
		node *n = tar->next;
		p->next = n;
		if(n != nullptr)
			n->prev = p;
		orderLink_delete(tar);
	}

	void doubleSpace()
	{
		node *p = head->order_next;
		node *temp = data;
		Capacity <<= 1;
		data = new node [Capacity];
		for(int i = 0; i < Capacity; i++)
		{
			data[i].next = nullptr;
			data[i].val_constructed = false;
		}
		size_t h;
		while(p != tail)
		{
			h = getHash(p->value->first) % Capacity;
			node *n = data[h].next;
			data[h].next = p;
			if(n != nullptr)
				n->prev = p;
			p->prev = &data[h];
			p->next = n;
			p = p->order_next;
		}
		delete [] temp;
	}
	void halfSpace()
	{
		node *p = head->order_next;
		node *temp = data;
		Capacity >>= 1;
		data = new node [Capacity];
		for(int i = 0; i < Capacity; i++)
		{
			data[i].next = nullptr;
			data[i].val_constructed = false;
		}
		size_t h;
		while(p != tail)
		{
			h = getHash(p->value->first) % Capacity;
			node *n = data[h].next;
			data[h].next = p;
			if(n != nullptr)
				n->prev = p;
			p->prev = &data[h];
			p->next = n;
			p = p->order_next;
		}
		delete [] temp;
	}

public:
	Hash getHash;
	Equal eq;

	/**
	 * iterator in linked_hashmap iterates in the order that keys were inserted into the map
	 */
	class const_iterator;
	class iterator
	{
	private:
		linked_hashmap *host;
		node *pt;
	public:
		friend class linked_hashmap;
		using difference_type = std::ptrdiff_t;
		using value_type = typename linked_hashmap::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::output_iterator_tag;
		
		iterator()
		{
			host = nullptr;
			pt = nullptr;
		}
		
		iterator(const iterator &other)
		{
			host = other.host;
			pt = other.pt;
		}

		/**
		 * TODO iter++
		 */
		iterator operator ++ (int)
		{
			iterator ret = *this;
			if(pt == host->tail)
				throw index_out_of_bound();
			pt = pt->order_next;
			return ret;
		}

		/**
		 * TODO ++iter
		 */
		iterator & operator ++ ()
		{
			if(pt == host->tail)
				throw index_out_of_bound();
			pt = pt->order_next;
			return *this;
		}

		/**
		 * TODO iter--
		 */
		iterator operator -- (int)
		{
			iterator ret = *this;
			if(pt == host->head || pt == host->head->order_next)
				throw index_out_of_bound();
			pt = pt->order_prev;
			return ret;
		}

		/**
		 * TODO --iter
		 */
		iterator & operator -- ()
		{
			if(pt == host->head || pt == host->head->order_next)
				throw index_out_of_bound();
			pt = pt->order_prev;
			return *this;
		}

		value_type & operator * () const
		{
			return pt->reference();
		}

		bool operator == (const iterator &rhs) const 
		{
			if(host != rhs.host)
				return false;
			if(pt != rhs.pt)
				return false;
			return true;
		}
		bool operator == (const const_iterator &rhs) const 
		{
			if(host != rhs.host)
				return false;
			if(pt != rhs.pt)
				return false;
			return true;
		}
		bool operator != (const iterator &rhs) const
		{
			if(pt != rhs.pt)
				return true;
			return false;
		}
		bool operator != (const const_iterator &rhs) const
		{
			if(pt != rhs.pt)
				return true;
			return false;
		}

		value_type* operator -> () const noexcept
		{
			return pt->pointer();
		}
	};

	class const_iterator
	{
	private:
		const linked_hashmap *host;
		node *pt;
	public:
		friend class linked_hashmap;
		using difference_type = std::ptrdiff_t;
		using value_type = typename linked_hashmap::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::output_iterator_tag;
		
		const_iterator()
		{
			host = nullptr;
			pt = nullptr;
		}
		
		const_iterator(const const_iterator &other)
		{
			host = other.host;
			pt = other.pt;
		}
		const_iterator(const iterator &other)
		{
			host = other.host;
			pt = other.pt;
		}

		/**
		 * TODO iter++
		 */
		const_iterator operator ++ (int)
		{
			const_iterator ret = *this;
			if(pt == host->tail)
				throw index_out_of_bound();
			pt = pt->order_next;
			return ret;
		}

		/**
		 * TODO ++iter
		 */
		const_iterator & operator ++ ()
		{
			if(pt == host->tail)
				throw index_out_of_bound();
			pt = pt->order_next;
			return *this;
		}

		/**
		 * TODO iter--
		 */
		const_iterator operator -- (int)
		{
			const_iterator ret = *this;
			if(pt == host->head || pt == host->head->order_next)
				throw index_out_of_bound();
			pt = pt->order_prev;
			return ret;
		}

		/**
		 * TODO --iter
		 */
		const_iterator & operator -- ()
		{
			if(pt == host->head || pt == host->head->order_next)
				throw index_out_of_bound();
			pt = pt->order_prev;
			return *this;
		}

		value_type & operator * () const
		{
			return pt->reference();
		}

		bool operator == (const iterator &rhs) const 
		{
			if(host != rhs.host)
				return false;
			if(pt != rhs.pt)
				return false;
			return true;
		}
		bool operator == (const const_iterator &rhs) const 
		{
			if(host != rhs.host)
				return false;
			if(pt != rhs.pt)
				return false;
			return true;
		}
		bool operator != (const iterator &rhs) const
		{
			if(pt != rhs.pt)
				return true;
			return false;
		}
		bool operator != (const const_iterator &rhs) const
		{
			if(pt != rhs.pt)
				return true;
			return false;
		}

		value_type* operator -> () const noexcept
		{
			return (pt->value);
		}
	};

	friend class iterator;
	friend class const_iterator;

	linked_hashmap()
	{
		Capacity = 389;
		data = new node [Capacity];
		head = new node ();
		tail = new node ();
		head->set_order(nullptr, tail);
		tail->set_order(head, nullptr);
		totalLength = 0;
	}
	linked_hashmap(const linked_hashmap &other)
	{
		Capacity = other.Capacity;
		data = new node [Capacity];
		head = new node ();
		tail = new node ();
		head->set_order(nullptr, tail);
		tail->set_order(head, nullptr);

		node *p = other.head->order_next;
		while(p != other.tail)
		{
			addNode(*(p->value));
			p = p->order_next;
		}
		totalLength = other.totalLength;
	}

	linked_hashmap & operator = (const linked_hashmap &other)
	{
		if(&other == this)
			return *this;
		node *p = head->order_next;
		node *q;
		while(p != tail)
		{
			q = p->order_next;
			delete p;
			p = q;
		}
		delete [] data;
		Capacity = other.Capacity;
		data = new node [Capacity];
		// for(int i = 0; i < Capacity; i++)
		// 	data[i].next = nullptr;
		head->set_order(nullptr, tail);
		tail->set_order(head, nullptr);
		p = other.head->order_next;
		while(p != other.tail)
		{
			addNode(*(p->value));
			p = p->order_next;
		}
		totalLength = other.totalLength;
		return *this;
	}

	~linked_hashmap()
	{
		// clear();
		for(int i = 0; i < Capacity; i++)
		{
			node *p = data[i].next;
			node *q;
			while(p != nullptr)
			{
				q = p->next;
				delete p;
				p = q;
			}
		}
		delete head;
		delete tail;
		delete [] data;
	}

	/**
	 * returns an iterator pointing to the beginning
	 */
	iterator begin()
	{
		iterator iter;
		iter.host = this;
		iter.pt = head->order_next;
		return iter;
	}
	const_iterator cbegin() const
	{
		const_iterator iter;
		iter.host = this;
		iter.pt = head->order_next;
		return iter;
	}

	/**
	 * returns an iterator pointing to the post-end element
	 */
	iterator end()
	{
		iterator iter;
		iter.host = this;
		iter.pt = tail;
		return iter;
	}
	const_iterator cend() const
	{
		const_iterator iter;
		iter.host = this;
		iter.pt = tail;
		return iter;
	}


	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key)
	{
		size_t h = getHash(key) % Capacity;
		node *p = data[h].next;
		while(p != nullptr && eq(p->value->first, key) == false)
		{
			p = p->next;
		}
		if(p == nullptr)
			throw index_out_of_bound();
		return p->value->second;
	}
	const T & at(const Key &key) const
	{
		size_t h = getHash(key) % Capacity;
		node *p = data[h].next;
		while(p != nullptr && eq(p->value->first, key) == false)
		{
			p = p->next;
		}
		if(p == nullptr)
			throw index_out_of_bound();
		return p->value->second;
	}

	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator [] (const Key &key)
	{
		size_t h = getHash(key) % Capacity;
		node *p = data[h].next;
		while(p != nullptr && eq(p->value->first, key) == false)
		{
			p = p->next;
		}
		if(p == nullptr)
		{
			if(totalLength >= (Capacity * LoadFactor))
				doubleSpace();
			T __t;
			p = addNode(key, __t);
			
			totalLength++;
		}
		return p->value->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator [] (const Key &key) const
	{
		size_t h = getHash(key) % Capacity;
		node *p = data[h].next;
		while(p != nullptr && eq(p->value->first, key) == false)
		{
			p = p->next;
		}
		if(p == nullptr)
			throw index_out_of_bound();
		return p->value->second;
	}

	bool empty() const
	{
		if(totalLength == 0)
			return true;
		return false;
	}

	size_t size() const
	{
		return totalLength;
	}

	/**
	 * clears the content
	 */
	void clear()
	{
		node *p = head->order_next;
		node *q;
		while(p != tail)
		{
			q = p->order_next;
			delete p;
			p = q;
		}
		totalLength = 0;
		for(int i = 0; i < Capacity; i++)
			data[i].next = nullptr;
		head->set_order(nullptr, tail);
		tail->set_order(head, nullptr);
	}

	/**
	 * pair <pointer, if inserted successfully>
	 */
	pair<iterator, bool> insert(const value_type &value)
	{
		if(totalLength >= (float)(Capacity * LoadFactor))
			doubleSpace();
		size_t h = getHash(value.first) % Capacity;
		node *p = data[h].next;
		while(p != nullptr && eq(p->value->first, value.first) == false)
		{
			p = p->next;
		}
		iterator iter;
		iter.host = this;
		if(p != nullptr)
		{
			iter.pt = p;
			return pair<iterator, bool>(iter, false);
		}
		totalLength++;
		iter.pt = addNode(value);
		return pair<iterator, bool>(iter, true);
	}

	/**
	 * erase the element at position pos
	 * throw if pos points to an invalid element
	 */
	void erase(iterator pos)
	{
		if(this != pos.host)
			throw invalid_iterator();
		if(pos.pt == this->tail)
			throw index_out_of_bound();
		delNode(pos.pt);
		delete pos.pt;
		totalLength--;
		if(Capacity > 389 && totalLength <= ((Capacity * LoadFactor) / 2))
			halfSpace();
	}

	size_t count(const Key &key) const
	{
		size_t h = getHash(key) % Capacity;
		node *p = data[h].next;
		while(p != nullptr && eq(p->value->first, key) == false)
		{
			p = p->next;
		}
		if(p == nullptr)
			return 0;
		else return 1;
	}

	/**
	 * returns an iterator whose key is equivalent to the given key
	 * if no such element, returns end()
	 */
	iterator find(const Key &key)
	{
		size_t h = getHash(key) % Capacity;
		node *p = data[h].next;
		// node *q;
		while(p != nullptr && eq(p->value->first, key) == false)
		{
			// q = p;
			p = p->next;
		}
		iterator iter;
		iter.host = this;
		if(p != nullptr)
			iter.pt = p;
		else
			iter.pt = tail;
		return iter;
	}
	const_iterator find(const Key &key) const
	{
		size_t h = getHash(key) % Capacity;
		node *p = data[h].next;
		// node *q;
		while(p != nullptr && eq(p->value->first, key) == false)
		{
			// q = p;
			p = p->next;
		}
		const_iterator iter;
		iter.host = this;
		if(p != nullptr)
			iter.pt = p;
		else
			iter.pt = tail;
		return iter;
	}
};
}

#endif