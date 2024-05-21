#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
// #include <cstdio>
#include <iostream>

namespace sjtu
{
template<
	class Key,
	class T,
	class Compare = std::less<Key> >
class map {
public:
	typedef pair<const Key, T> value_type;

private:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	enum colourT {Rouge, Noir};

	struct node
	{
		using value_type = typename map::value_type;
		value_type *data;
		colourT colour;
		node *left, *right, *father;
		bool dir_from_fa; // 0->left, 1->right

		node() { data = nullptr; father = left = right = nullptr; colour = Noir;}
		/**
		 * value, colour, direction, father, left, right
		 */
		node(const value_type &val, colourT prop = Noir, bool dir = 0, \
			node *fa = nullptr, node *lpt = nullptr, node *rpt = nullptr): \
			colour(prop), dir_from_fa(dir)
		{
			father = fa;
			left = lpt;
			right = rpt;
			data = new value_type (val);
		}
		
		~node()
		{
			if(data != nullptr)
				delete data;
		}
	};
	/**
	 * root: the nullptr of the Red-Black Tree
	 * left_end: begin of the nullptr
	 * right_end: end of the nullptr
	 * end() = nullptr
	 */
	node *index;
	node *left_end;
	node *right_end;
	node *root;
	Compare cmp;
	int totalLength;

	void clear(node *pt)
	{
		if(pt->left != nullptr)
		{
			clear(pt->left);
			pt->left = nullptr;
		}
		if(pt->right != nullptr)
		{
			clear(pt->right);
			pt->right = nullptr;
		}
		delete pt;
	}

	void LL(node *grandParent)
	{
		/**
		 * 	     GrandParent	 mother
		 * 	       /  \		  /  \
		 * 	 mother    uncle  -> self    GrandParent
		 *       /   \	    / \			 / \
		 *     self  x	   a   b	 	x   uncle
		 */
		// std::cout << "LL\n";
		node *parent = grandParent->left;
		// node *self = parent->left;
		node *x = parent->right;

		node *anc = grandParent->father;
		bool dir = grandParent->dir_from_fa;
		parent->father = anc;
		parent->dir_from_fa = dir;
		if(anc != nullptr)
		{
			if(dir == 0)
				anc->left = parent;
			else
				anc->right = parent;
		}
		else
			root = parent;
		parent->right = grandParent;
		grandParent->father = parent;
		grandParent->left = x;
		grandParent->dir_from_fa = 1;
		if(x != nullptr)
		{
			x->dir_from_fa = 0;
			x->father = grandParent;
		}
		parent->colour = Noir;
		grandParent->colour = Rouge;
	}

	void RR(node *grandParent)
	{
		/**
		 * 	GrandParent		   parent
		 * 	  /   \			   /    \
		 *    uncle   parent   -> GrandParent   self
		 * 	      /   \	    /    \
		 * 	     x    self	  uncle   x
		 */
		// std::cout << "RR\n";
		node *anc = grandParent->father;
		node *parent = grandParent->right;
		node *x = parent->left;
		bool dir = grandParent->dir_from_fa;
		parent->father = anc;
		parent->dir_from_fa = dir;
		if(anc != nullptr)
		{
			if(dir == 0)
				anc->left = parent;
			else
				anc->right = parent;
		}
		else
			root = parent;
		parent->left = grandParent;
		grandParent->father = parent;
		grandParent->dir_from_fa = 0;
		grandParent->right = x;
		if(x != nullptr)
		{
			x->father = grandParent;
			x->dir_from_fa = 1;
		}
		parent->colour = Noir;
		grandParent->colour = Rouge;
	}

	void LR(node *grandParent)
	{
		/**
		 * 	GrandParent		    self
		 * 	  /   \			    /  \
		 *  parent    uncle    ->      parent   GrandParent
		 *   /  \			/  \	   /    \
		 *  x   self		       x   b	  c	uncle
		 *      /  \
		 *     b    c
		 */
		// std::cout << "LR\n";
		node *anc = grandParent->father;
		node *parent = grandParent->left;
		bool dir = grandParent->dir_from_fa;
		node *self = parent->right;
		self->father = anc;
		self->dir_from_fa = dir;
		if(anc != nullptr)
		{
			if(dir == 0)
				anc->left = self;
			else
				anc->right = self;
		}
		else
			root = self;
		node *b = self->left;
		node *c = self->right;

		parent->father = self;
		self->left = parent;
		parent->dir_from_fa = 0;

		grandParent->father = self;
		self->right = grandParent;
		grandParent->dir_from_fa = 1;

		parent->right = b;
		if(b != nullptr)
		{
			b->father = parent;
			b->dir_from_fa = 1;
		}

		grandParent->left = c;
		if(c != nullptr)
		{
			c->father = grandParent;
			c->dir_from_fa = 0;
		}
		self->colour = Noir;
		grandParent->colour = Rouge;
	}

	void RL(node *grandParent)
	{
		/**
		 * 	GrandParent		     self
		 * 	  /   \			    /   \
		 *    uncle  parent   ->  GrandParent    parent
		 *	     /   \	    /    \	 /   \
		 * 	  self	  x	 uncle    b	c     x
		 *    	  /  \
		 *       b    c
		 */
		// std::cout << "RL\n";
		node *anc = grandParent->father;
		node *parent = grandParent->right;
		bool dir = grandParent->dir_from_fa;
		node *self = parent->left;
		self->father = anc;
		self->dir_from_fa = dir;
		if(anc != nullptr)
		{
			if(dir == 0)
				anc->left = self;
			else
				anc->right = self;
		}
		else
			root = self;
		node *b = self->left;
		node *c = self->right;

		parent->father = self;
		self->right = parent;
		parent->dir_from_fa = 1;

		grandParent->father = self;
		self->left = grandParent;
		grandParent->dir_from_fa = 0;

		grandParent->right = b;
		if(b != nullptr)
		{
			b->father = grandParent;
			b->dir_from_fa = 1;
		}

		parent->left = c;
		if(c != nullptr)
		{
			c->father = parent;
			c->dir_from_fa = 0;
		}
		self->colour = Noir;
		grandParent->colour = Rouge;
	}

	void insertAdjust(node *grandParent, node *parent, node *t)
	{
		// the colour of t is red
		if(parent == nullptr)
			return;
		if(parent->colour == Noir)
			return;
		if(parent == root)
		{
			parent->colour = Noir;
			return;
		}
		if(grandParent->left == parent)
		{
			if(parent->left == t)
			{
				LL(grandParent);
			}
			else
			{
				LR(grandParent);
			}
		}
		else
		{
			if(parent->right == t)
			{
				RR(grandParent);
			}
			else
			{
				RL(grandParent);
			}
		}
	}

	bool bothBlack(node *pos)
	{
		if(pos->left == nullptr || (pos->left != nullptr && pos->left->colour == Noir))
		{
			if(pos->right == nullptr || (pos->right != nullptr && pos->right->colour == Noir))
			{
				return true;
			}
		}
		return false;
	}
	bool bothRed(node *pos)
	{
		if(pos->left == nullptr || (pos->left != nullptr && pos->left->colour == Rouge))
		{
			if(pos->right == nullptr || (pos->right != nullptr && pos->right->colour == Rouge))
			{
				return true;
			}
		}
		return false;
	}

	bool _eq(const value_type *a, const value_type *b)
	{
		if(cmp(a->first, b->first) == true)
			return false;
		if(cmp(b->first, a->first) == true)
			return false;
		return true;
	}
	bool key_eq(const Key &a, const Key &b) const
	{
		if(cmp(a, b) == true)
			return false;
		if(cmp(b, a) == true)
			return false;
		return true;
	}

	void delNode(node *self)
	{
		moveIndex(self);
		delete self;
		totalLength--;
		if(root != nullptr)
			root->colour = Noir;
	}

public:
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		node *pointer;
		map *host;
		
	public:
		friend class map;
		friend class const_iterator;
		iterator()
		{
			// TODO
			host = nullptr;
			pointer = nullptr;
		}
		iterator(const iterator &other)
		{
			// TODO
			host = other.host;
			pointer = other.pointer;
		}
		/**
		 * TODO iter++
		 */
		iterator operator ++ (int)
		{
			iterator ret = *this;
			if(pointer == nullptr)
				throw invalid_iterator();
			if(pointer->right != nullptr)
			{
				pointer = pointer->right;
				while(pointer->left != nullptr)
					pointer = pointer->left;
			}
			else if(pointer->dir_from_fa == 0)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 1)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator ++ ()
		{
			if(pointer == nullptr)
				throw invalid_iterator();
			if(pointer->right != nullptr)
			{
				pointer = pointer->right;
				while(pointer->left != nullptr)
				{
					pointer = pointer->left;	
				}
			}
			else if(pointer->dir_from_fa == 0)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 1)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator -- (int) 
		{
			if(pointer == host->left_end)
				throw invalid_iterator();
			iterator ret = *this;
			if(pointer == nullptr) // end()
			{
				pointer = host->right_end;
			}
			else if(pointer->left != nullptr)
			{
				pointer = pointer->left;
				while(pointer->right != nullptr)
					pointer = pointer->right;
			}
			else if(pointer->dir_from_fa == 1)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 0)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return ret;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator -- ()
		{
			if(pointer == host->left_end)
				throw invalid_iterator();
			if(pointer == nullptr) // end()
			{
				pointer = host->right_end;
			}
			else if(pointer->left != nullptr)
			{
				pointer = pointer->left;
				while(pointer->right != nullptr)
					pointer = pointer->right;
			}
			else if(pointer->dir_from_fa == 1)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 0)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator * () const
		{
			return *(pointer->data);
		}

		bool operator == (const iterator &rhs) const
		{
			if(rhs.host != host)
				return false;
			if(pointer == rhs.pointer)
				return true;
			return false;
		}
		bool operator == (const const_iterator &rhs) const
		{
			if(rhs.host != host)
				return false;
			if(pointer == rhs.pointer)
				return true;
			return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator != (const iterator &rhs) const
		{
			if(rhs.host != host)
				return true;
			if(pointer != rhs.pointer)
				return true;
			return false;
		}
		bool operator != (const const_iterator &rhs) const
		{
			if(rhs.host != host)
				return true;
			if(pointer != rhs.pointer)
				return true;
			return false;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type * operator -> () const noexcept
		{
			return (pointer->data);
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		node *pointer;
		const map *host;
		
	public:
		friend class map;
		friend class iterator;
		const_iterator()
		{
			// TODO
			host = nullptr;
			pointer = nullptr;
		}
		const_iterator(const iterator &other)
		{
			// TODO
			host = other.host;
			pointer = other.pointer;
		}
		const_iterator(const const_iterator &other)
		{
			// TODO
			host = other.host;
			pointer = other.pointer;
		}

		const_iterator(const map *h, node *pt)
		{
			host = h;
			pointer = pt;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator ++ (int)
		{
			const_iterator ret = *this;
			if(pointer == nullptr)
				throw invalid_iterator();
			if(pointer->right != nullptr)
			{
				pointer = pointer->right;
				while(pointer->left != nullptr)
					pointer = pointer->left;
			}
			else if(pointer->dir_from_fa == 0)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 1)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return ret;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator & operator ++ ()
		{
			if(pointer == nullptr)
				throw invalid_iterator();
			if(pointer->right != nullptr)
			{
				pointer = pointer->right;
				while(pointer->left != nullptr)
				{
					pointer = pointer->left;	
				}
			}
			else if(pointer->dir_from_fa == 0)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 1)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator -- (int) 
		{
			if(pointer == host->left_end)
				throw invalid_iterator();
			const_iterator ret = *this;
			if(pointer == nullptr) // end()
			{
				pointer = host->right_end;
			}
			else if(pointer->left != nullptr)
			{
				pointer = pointer->left;
				while(pointer->right != nullptr)
					pointer = pointer->right;
			}
			else if(pointer->dir_from_fa == 1)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 0)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return ret;
		}
		/**
		 * TODO --iter
		 */
		const_iterator & operator -- ()
		{
			if(pointer == host->left_end)
				throw invalid_iterator();
			if(pointer == nullptr) // end()
			{
				pointer = host->right_end;
			}
			else if(pointer->left != nullptr)
			{
				pointer = pointer->left;
				while(pointer->right != nullptr)
					pointer = pointer->right;
			}
			else if(pointer->dir_from_fa == 1)
				pointer = pointer->father;
			else
			{
				while(pointer != nullptr && pointer->dir_from_fa == 0)
					pointer = pointer->father;
				if(pointer != nullptr)
					pointer = pointer->father;
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator * () const
		{
			return *(pointer->data);
		}

		bool operator == (const iterator &rhs) const
		{
			if(rhs.host != host)
				return false;
			if(pointer == rhs.pointer)
				return true;
			return false;
		}
		bool operator == (const const_iterator &rhs) const
		{
			if(rhs.host != host)
				return false;
			if(pointer == rhs.pointer)
				return true;
			return false;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator != (const iterator &rhs) const
		{
			if(rhs.host != host)
				return true;
			if(pointer != rhs.pointer)
				return true;
			return false;
		}
		bool operator != (const const_iterator &rhs) const
		{
			if(rhs.host != host)
				return true;
			if(pointer != rhs.pointer)
				return true;
			return false;
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type * operator -> () const noexcept
		{
			return (pointer->data);
		}
	};

	friend class iterator;
	friend class const_iterator;

	void constructTree(node *pos, node *ref, const map &other)
	{
		if(other.left_end == ref)
			left_end = pos;
		if(other.right_end == ref)
			right_end = pos;
		if(ref->left != nullptr)
		{
			pos->left = new node(*(ref->left->data), ref->left->colour, 0, pos);
			constructTree(pos->left, ref->left, other);
		}
		if(ref->right != nullptr)
		{
			pos->right = new node(*(ref->right->data), ref->right->colour, 1, pos);
			constructTree(pos->right, ref->right, other);
		}
	}
	/**
	 * TODO two constructors
	 */
	map()
	{
		// index = nullptr;
		totalLength = 0;
		root = left_end = right_end = nullptr;
	}
	map(const map &other)
	{
		root = left_end = right_end = nullptr;
		totalLength = other.totalLength;
		if(totalLength == 0)
			return;
		root = new node(*(other.root->data));
		constructTree(root, other.root, other);
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other)
	{
		if(&other == this)
			return *this;
		if(root != nullptr)
			clear(root);
		root = right_end = left_end = nullptr;
		totalLength = other.totalLength;
		if(totalLength == 0)
			return *this;
		root = new node(*(other.root->data));
		constructTree(root, other.root, other);
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map()
	{
		if(root != nullptr)
			clear(root);
		root = nullptr;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key)
	{
		node *p = root;
		while(p != nullptr)
		{
			if(cmp(key, p->data->first) == true)
				p = p->left;
			else if(cmp(p->data->first, key) == true)
				p = p->right;
			else
			{
				return p->data->second;
			}
		}
		throw index_out_of_bound();
	}
	const T & at(const Key &key) const
	{
		node *p = root;
		while(p != nullptr)
		{
			if(cmp(key, p->data->first) == true)
				p = p->left;
			else if(cmp(p->data->first, key) == true)
				p = p->right;
			else
			{
				return p->data->second;
			}
		}
		throw index_out_of_bound();
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key)
	{
		value_type val(key, T());
		if(totalLength == 0)
		{
			node *t = new node(val, Noir, 0, nullptr);
			left_end = right_end = root = t;
			totalLength = 1;
			return t->data->second;
		}

		node *t = root;
		while(t != nullptr)
		{
			if(cmp(key, t->data->first) == true)
				t = t->left;
			else if(cmp(t->data->first, key) == true)
				t = t->right;
			else
			{
				return t->data->second;
			}
		}

		t = root;
		// node *parent = root, *grandParent = root;
		node *parent = nullptr, *grandParent = nullptr;
		bool dir = 0;
		while(t != nullptr)
		{
			if((t->left != nullptr && t->left->colour == Rouge) && \
				(t->right != nullptr && t->right->colour == Rouge))
			{
				// Both of the children is Red
				// If grandparent is red -> two adjacent red (sibling is black) -> spin
				t->left->colour = t->right->colour = Noir;
				t->colour = Rouge;
				insertAdjust(grandParent, parent, t);
			}
			grandParent = parent;
			parent = t;
			if(cmp(key, t->data->first) == true)
			{
				dir = 0;
				t = t->left;
			}
			else
			{
				dir = 1;
				t = t->right;
			}
		}
		t = new node(val, Rouge, dir, parent);
		if(cmp(key, parent->data->first) == true)
		{
			parent->left = t;
			t->dir_from_fa = 0;
		}
		else
		{
			parent->right = t;
			t->dir_from_fa = 1;
		}
		insertAdjust(grandParent, parent, t);
		root->colour = Noir;
		if(totalLength == 0)
		{
			left_end = right_end = t;
		}
		else
		{
			if(cmp(key, left_end->data->first) == true)
				left_end = t;
			if(cmp(right_end->data->first, key) == true)
				right_end = t;
		}
		totalLength++;
		return t->data->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const
	{
		node *p = root;
		while(p != nullptr)
		{
			if(cmp(key, p->data->first) == true)
				p = p->left;
			else if(cmp(p->data->first, key) == true)
				p = p->right;
			else
			{
				return p->data->second;
			}
		}
		throw index_out_of_bound();
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin()
	{
		iterator ret;
		ret.host = this;
		ret.pointer = left_end;
		return ret;
	}
	const_iterator cbegin() const
	{
		const_iterator ret(this, left_end);
		return ret;
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end()
	{
		iterator ret;
		ret.host = this;
		ret.pointer = nullptr;
		return ret;
	}
	const_iterator cend() const
	{
		const_iterator ret(this, nullptr);
		return ret;
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const
	{
		if(totalLength == 0)
			return true;
		return false;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const
	{
		return totalLength;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
		if(root != nullptr)
			clear(root);
		root = left_end = right_end = nullptr;
		totalLength = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value)
	{
		iterator ret;
		ret.host = this;
		if(root == nullptr)
		{
			root = new node(value, Noir, 0);
			ret.pointer = root;
			left_end = right_end = root;
			totalLength++;
			return pair<iterator, bool> (ret, true);
		}

		// node *parent = root;
		node *parent = nullptr;
		node *t = root;
		// node *grandParent = root;
		node *grandParent = nullptr;
		while(t != nullptr)
		{
			if(cmp(value.first, t->data->first) == true)
				t = t->left;
			else if(cmp(t->data->first, value.first) == true)
				t = t->right;
			else
			{
				ret.pointer = t;
				return pair<iterator, bool>(ret, false);
			}
		}
		t = root;
		bool dir = 0;
		while(t != nullptr)
		{
			if((t->left != nullptr && t->left->colour == Rouge) && \
				(t->right != nullptr && t->right->colour == Rouge))
			{
				// Both of the children is Red
				// If grandparent is red -> two adjacent red (sibling is black) -> spin
				t->left->colour = t->right->colour = Noir;
				t->colour = Rouge;
				insertAdjust(grandParent, parent, t);
			}
			grandParent = parent;
			parent = t;
			if(cmp(value.first, t->data->first) == true)
			{
				dir = 0;
				t = t->left;
			}
			else
			{
				dir = 1;
				t = t->right;
			}
		}
		t = new node(value, Rouge, dir, parent);
		if(cmp(value.first, parent->data->first) == true)
		{
			parent->left = t;
			t->dir_from_fa = 0;
		}
		else
		{
			parent->right = t;
			t->dir_from_fa = 1;
		}
		insertAdjust(grandParent, parent, t);
		ret.pointer = t;
		totalLength++;
		if(totalLength == 1)
		{
			left_end = right_end = t;
		}
		else
		{
			if(cmp(value.first, left_end->data->first) == true)
				left_end = t;
			if(cmp(right_end->data->first, value.first) == true)
				right_end = t;
		}
		return pair<iterator, bool>(ret, true);
	}

	void moveIndex(node *pos)
	{
		if(totalLength == 1)
		{
			right_end = left_end = nullptr;
			return;
		}
		if(pos == right_end)
		{
			iterator iter;
			iter.host = this;
			iter.pointer = pos;
			--iter;
			right_end = iter.pointer;
		}
		if(pos == left_end)
		{
			iterator iter;
			iter.host = this;
			iter.pointer = pos;
			++iter;
			left_end = iter.pointer;
		}
	}

	/**
	 * erase the element at pos.
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos)
	{
		if(pos.host != this)
			throw invalid_iterator();
		if(pos.pointer == nullptr)
			throw invalid_iterator();

		node *pos_pointer = pos.pointer;
		node *self = pos_pointer; //
		node *sibling;
		node *parent = self->father;
		if(self->left == nullptr)
		{
			if(self == root)
			{
				moveIndex(self);
				root = self->right;
				if(root != nullptr)
				{
					root->colour = Noir;
					root->father = nullptr;
				}
				delete self;
				totalLength--;
				return;
			}
		}
		else if(self->right == nullptr)
		{
			if(self == root)
			{
				moveIndex(self);
				root = self->left;
				if(root != nullptr)
				{
					root->colour = Noir;
					root->father = nullptr;
				}
				delete self;
				totalLength--;
				return;
			}
		}
		else // left and right both not nullptr
		{
			node *temp = self->right;
			while(temp->left != nullptr)
				temp = temp->left;
			// exchange self and temp
			if(self->right == temp)
			{
				// std::cout << "1\n";
				self->right = temp->right;
				if(temp->right != nullptr)
					temp->right->father = self;
				temp->father = self->father;
				temp->dir_from_fa = self->dir_from_fa;
				if(self->father != nullptr)
				{
					if(self->dir_from_fa == 0)
						self->father->left = temp;
					else
						self->father->right = temp;
				}
				else
					root = temp;
				temp->left = self->left;
				self->left->father = temp;
				self->left = nullptr;
				temp->right = self;
				self->father = temp;
				self->dir_from_fa = 1;
			}
			else
			{
				// std::cout << "2\n";
				node *afa = temp->father;
				bool adir = temp->dir_from_fa;
				node *al = temp->left;
				node *ar = temp->right;
				node *bfa = self->father;
				bool bdir = self->dir_from_fa;
				node *bl = self->left;
				node *br = self->right;

				temp->father = bfa;
				temp->dir_from_fa = bdir;
				if(bfa != nullptr)
				{
					if(self->dir_from_fa == 0)
						bfa->left = temp;
					else
						bfa->right = temp;
				}
				else
					root = temp;
				self->father = afa;
				self->dir_from_fa = adir;
				if(afa != nullptr)
				{
					if(adir == 0)
						afa->left = self;
					else
						afa->right = self;
				}
				else
					root = self;

				temp->right = br;
				if(br != nullptr)
					br->father = temp;
				temp->left = bl;
				if(bl != nullptr)
					bl->father = temp;
				
				self->left = al;
				if(al != nullptr)
					al->father = self;
				self->right = ar;
				if(ar != nullptr)
					ar->father = self;
			}
			std::swap(self->colour, temp->colour);
			
			pos_pointer = self;
			parent = self->father;
		}

		if(self->colour == Noir)
		{
			// Now self could not have two children
			if(self->left != nullptr)
				self->left->colour = Noir;
			else if(self->right != nullptr)
				self->right->colour = Noir;
			else
			{
				while(self != root)
				{
					parent = self->father;
					if(self->dir_from_fa == 0)
					{
						sibling = parent->right;
						if(sibling->colour == Rouge)
						{
							RR(parent);
							sibling = parent->right;
						}
						else
						{
							if(bothBlack(sibling))
							{
								sibling->colour = Rouge;
								if(parent->colour == Rouge)
								{
									parent->colour = Noir;
									break;
								}
								self = parent;
								parent = self->father;
							}
							else
							{
								if(sibling->right != nullptr && sibling->right->colour == Rouge)
								{
									colourT parent_colour = parent->colour;
									RR(parent);
									parent->colour = Noir;
									sibling->colour = parent_colour;
									sibling->right->colour = Noir;
								}
								else
								{
									LL(sibling);
									sibling = parent->right;
									continue;
								}
								break;
							}
						}
					}
					else
					{
						sibling = parent->left;
						if(sibling->colour == Rouge)
						{
							LL(parent);
							sibling = parent->left;
						}
						else
						{
							if(bothBlack(sibling))
							{
								sibling->colour = Rouge;
								if(parent->colour == Rouge)
								{
									parent->colour = Noir;
									break;
								}
								self = parent;
								parent = self->father;
							}
							else
							{
								if(sibling->left != nullptr && sibling->left->colour == Rouge)
								{
									colourT parent_colour = parent->colour;
									LL(parent);
									parent->colour = Noir;
									sibling->colour = parent_colour;
										sibling->left->colour = Noir;
								}
								else
								{
									RR(sibling);
									sibling = parent->left;
									continue;
								}
								break;
							}
						}
					}
				}
			}
		}
		self = pos_pointer;
		parent = self->father;
		moveIndex(self);
		if(self->left != nullptr)
		{
			node *ch = self->left;
			ch->father = self->father;
			ch->dir_from_fa = self->dir_from_fa;
			if(self->father != nullptr)
			{
				if(self->dir_from_fa == 0)
					self->father->left = ch;
				else
					self->father->right = ch;
			}
			else
				root = ch;
			ch->colour = Noir;
		}
		else if(self->right != nullptr)
		{
			node *ch = self->right;
			ch->father = self->father;
			ch->dir_from_fa = self->dir_from_fa;
			if(self->father != nullptr)
			{
				if(self->dir_from_fa == 0)
					self->father->left = ch;
				else
					self->father->right = ch;
			}
			else
				root = ch;
			ch->colour = Noir;
		}
		else
		{
			if(self->dir_from_fa == 0)
				self->father->left = nullptr;
			else
				self->father->right = nullptr;
		}
		delete self;
		totalLength--;
		if(root != nullptr)
			root->colour = Noir;
		return;
	}

	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const
	{
		node *p = root;
		while(p != nullptr)
		{
			if(cmp(key, p->data->first) == true)
				p = p->left;
			else if(cmp(p->data->first, key) == true)
				p = p->right;
			else
				return (size_t)1;
		}
		return (size_t)0;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key)
	{
		node *p = root;
		while(p != nullptr && key_eq(p->data->first, key) == false)
		{
			// a < b == true
			if(cmp(p->data->first, key) == true)
				p = p->right;
			else
				p = p->left;
		}
		if(p == nullptr)
			return end();
		iterator ret;
		ret.host = this;
		ret.pointer = p;
		return ret;
	}
	const_iterator find(const Key &key) const
	{
		node *p = root;
		while(p != nullptr && key_eq(p->data->first, key) == false)
		{
			// a < b == true
			if(cmp(p->data->first, key) == true)
				p = p->right;
			else
				p = p->left;
		}
		if(p == nullptr)
			return const_iterator(this, nullptr);
		const_iterator ret(this, p);
		return ret;
	}

};

}

#endif
