#include <functional>
#include <iostream>

template<class T>
class BST
{

	struct Node
	{
		T data;
		Node* left;
		Node* right;
		Node* parent;
		int height;

		Node(T _data) :data(_data), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
	};

	class const_iterator
	{

	protected:
		Node* p;
		BST* b;

	public:
		const_iterator(Node* p, BST* b) :p(p), b(b) {}

		const T& operator*() const
		{
			return p->data;
		}

		const T* operator->() const
		{
			return p->data;
		}

		bool operator !=(const const_iterator& it) const noexcept
		{
			return p != it.p;
		}

		const_iterator& operator++() noexcept;

	};

	class iterator : public const_iterator
	{
	public:
		iterator(Node* p, BST* b) :const_iterator(p, b) {}

		T& operator*() const
		{
			return this->p->data;
		}

		iterator& operator++();

	};

	Node* root;

public:
	BST() :root(nullptr) {}
	BST(const BST& rhs) :root(copy_tree(rhs.root)) {} //copy constructor
	BST(BST&& rhs) : root(rhs.root) { rhs.root = nullptr; } //move constructor
	BST(const size_t size, const std::function<T(const size_t)>& f)
	{
		for (size_t i = 0; i < size; ++i)
			insert(f(i));
	}

	//copy assignment
	BST& operator= (const BST& rhs);

	//move assingnment
	BST& operator=(BST&& rhs);

	iterator begin() noexcept
	{
		return iterator(find_min(root), this);
	}

	iterator end()
	{
		return iterator(nullptr, this);
	}

	iterator find_it(const T& key);

	Node* find(const T& key) const;

	bool is_balanced();

	Node* insert(const T& key);

	Node* remove(const T& key);

	//void print(Node* _root);

	~BST()
	{
		delete_bst(root);
	}

private:
	Node* copy_tree(Node* n);

	Node* delete_bst(Node*& r);

	int max(int a, int b);

	Node* rotate_right(Node* y);

	Node* rotate_left(Node* x);

	int get_balance_factor(Node* n);

	Node* insert(Node*& n, const T& key);

	Node* remove(Node* n, const T& key);

	Node* find_min(Node* x) const;

	Node* find_next(Node* x) const;

	Node* find_max() const;

	bool is_balanced(Node* n);

	int height(Node* n);

	int h1(Node* n);
};

template<class T>
typename BST<T>::const_iterator& BST<T>::const_iterator::operator++() noexcept
{
	Node* node_ptr;

	if (p->right != nullptr)
	{
		p = p->right;
		while (p->left != nullptr)
		{
			p = p->left;
		}
	}

	else
	{
		node_ptr = p->parent;
		auto max = b->find_max();

		if (p != nullptr && p == max)
		{
			p = nullptr;
		}

		else {
			while (node_ptr != nullptr && p == node_ptr->right)
			{
				p = node_ptr;
				node_ptr = node_ptr->parent;
			}
			p = node_ptr;

		}

	}

	return *this;

}

template<class T>
typename BST<T>::iterator& BST<T>::iterator::operator++()
{
	Node* node_ptr;

	if (this->p->right != nullptr)
	{
		this->p = this->p->right;
		while (this->p->left != nullptr)
		{
			this->p = this->p->left;
		}
	}

	else
	{
		node_ptr = this->p->parent;
		auto max = this->b->find_max();

		if (this->p != nullptr && this->p == max)
		{
			this->p = nullptr;
		}

		else {
			while (node_ptr != nullptr && this->p == node_ptr->right)
			{
				this->p = node_ptr;
				node_ptr = node_ptr->parent;
			}
			this->p = node_ptr;

		}

	}

	return *this;

}

template<class T>
typename BST<T>& BST<T>::operator=(const BST& rhs)
{
	if (&rhs != this)
	{
		if (root != nullptr)
			delete_bst(root);
		if (rhs.root == nullptr)
			root = nullptr;
		else
			root = copy_tree(rhs.root);
	}
	return *this;
}

template<class T>
typename BST<T>& BST<T>::operator=(BST&& rhs)
{
	if (&rhs != this)
	{
		if (root != nullptr)
			delete_bst(root);
		/*if (rhs.root == nullptr)
			root = nullptr;
		else*/
		root = copy_tree(rhs.root);
		rhs.root = nullptr;
	}
	return *this;
}

template<class T>
typename BST<T>::iterator BST<T>::find_it(const T& key)
{
	Node* r = root;

	while (r != nullptr)
	{
		if (key > r->data)
			r = r->right;

		else if (key < r->data)
			r = r->left;

		else
			return iterator(r);
	}

	return iterator(nullptr);
}

template<class T>
typename BST<T>::Node* BST<T>::find(const T& key) const
{
	Node* r = root;

	while (r != nullptr)
	{
		if (key > r->data)
			r = r->right;

		else if (key < r->data)
			r = r->left;

		else
			return r;
	}

	return nullptr;
}

template<class T>
bool BST<T>::is_balanced(Node* n)
{
	int left_h, right_h;

	if (n == nullptr)
		return true;

	left_h = h1(n->left);
	right_h = h1(n->right);

	if (abs(left_h - right_h) <= 1 && is_balanced(n->left) && is_balanced(n->right))
		return true;
	return false;
}

template<class T>
bool BST<T>::is_balanced()
{
	if (is_balanced(root))
		return true;
	return false;
}

template<class T>
typename BST<T>::Node* BST<T>::insert(const T& key)
{
	root = insert(root, key);
	return root;
}

template<class T>
typename BST<T>::Node* BST<T>::remove(const T& key)
{
	root = remove(root, key);
	return root;
}

//template<class T>
//inline void BST<T>::print(Node* _root)
//{
//	if (_root != nullptr)
//	{
//		print(_root->left);
//		std::cout << _root->data << " ";
//		print(_root->right);
//	}
//}

template<class T>
typename BST<T>::Node* BST<T>::copy_tree(Node* n)
{
	if (n != nullptr)
	{
		Node* new_node = new Node(n->data);
		new_node->left = copy_tree(n->left);
		new_node->right = copy_tree(n->right);
		return new_node;
	}
	return nullptr;
}

template<class T>
typename BST<T>::Node* BST<T>::delete_bst(Node*& r)
{
	if (r == nullptr)
		return nullptr;

	if (r != nullptr)
	{
		delete_bst(r->left);
		delete_bst(r->right);
		delete r;
	}
	return nullptr;
}

template<class T>
int BST<T>::max(int a, int b)
{
	return (a > b) ? a : b;
}

template<class T>
typename BST<T>::Node* BST<T>::rotate_left(Node* x)
{
	Node* p = x->parent;
	Node* y = x->right;
	x->right = y->left;

	if (y->left != nullptr)
		y->left->parent = x;
	y->left = x;
	x->parent = y;
	y->parent = p;

	if (p != nullptr)
	{
		if (p->right == x)
			p->right = y;
		else
			p->left = x;
	}

	x->height = max(height(x->left),
		height(x->right)) + 1;
	y->height = max(height(y->left),
		height(y->right)) + 1;

	return y;
}

template<class T>
typename BST<T>::Node* BST<T>::rotate_right(Node* y)
{
	Node* p = y->parent;
	Node* x = y->left;
	y->left = x->right;

	if (x->right != nullptr)
		x->right->parent = y;
	x->right = y;
	y->parent = x;
	x->parent = p;

	if (p != nullptr)
	{
		if (p->left == y)
			p->left = x;
		else
			p->right = x;
	}

	x->height = max(height(x->left),
		height(x->right)) + 1;
	y->height = max(height(y->left),
		height(y->right)) + 1;

	return x;
}

template<class T>
int BST<T>::get_balance_factor(Node* n)
{
	if (n == nullptr)
		return 0;
	return height(n->left) - height(n->right);
}

template<class T>
typename BST<T>::Node* BST<T>::insert(Node*& n, const T& key)
{
	if (n == nullptr)
		return new Node(key);

	else
	{
		if (key < n->data)
		{
			n->left = insert(n->left, key);
			n->left->parent = n;
		}
		else
		{
			n->right = insert(n->right, key);
			n->right->parent = n;
		}
	}

	n->height = 1 + max(height(n->left), height(n->right));
	int balance = get_balance_factor(n);

	if (balance > 1 && key < n->left->data)
		return rotate_right(n);

	if (balance < -1 && key > n->right->data)
		return rotate_left(n);

	if (balance > 1 && key > n->left->data)
	{
		n->left = rotate_left(n->left);
		return rotate_right(n);
	}

	if (balance < -1 && key < n->right->data)
	{
		n->right = rotate_left(n->right);
		return rotate_right(n);
	}

	return n;

}

template<class T>
typename BST<T>::Node* BST<T>::remove(Node* n, const T& key)
{
	if (n == nullptr)
		return n;

	if (key < n->data)
		n->left = remove(n->left, key);

	else if (key > n->data)
		n->right = remove(n->right, key);

	//brak potomków
	/*if (n->left == nullptr && n->right == nullptr)
	{
		delete n;
		n = nullptr;
		return n;
	}

	else if (root->left == nullptr)
	{
		Node* temp = n;
		n = n->right;
		delete temp;
	}

	else if (root->right == nullptr)
	{
		Node* temp = n;
		n = n->left;
		delete temp;
	}

	else
	{
		Node* temp = find_min(n->right);
		n->data = temp->data;
		n->right = remove(n->right,temp->data);
	}*/

	else
	{
		if (n->left == nullptr || n->right == nullptr)
		{
			Node* temp;
			if (n->left != nullptr)
				temp = n->left;
			else temp = n->right;

			//lisc
			if (temp == nullptr)
			{
				temp = n;
				n = nullptr;
			}

			else
				n = temp;
			delete temp;
		}

		else
		{
			Node* temp = find_min(n->right);
			n->data = temp->data;
			n->right = remove(n->right, temp->data);
		}
	}

	if (n == nullptr)
		return n;

	n->height = 1 + max(height(n->left), height(n->right));

	int balance = get_balance_factor(n);

	if (balance > 1 && get_balance_factor(n->left) >= 0)
		return rotate_right(n);

	if (balance > 1 && get_balance_factor(n->left) < 0)
	{
		n->left = rotate_left(n->left);
		return rotate_right(n);

	}

	if (balance < -1 && get_balance_factor(n->right) <= 0)
		return rotate_left(n);

	if (balance < -1 && get_balance_factor(n->right) > 0)
	{
		n->right = rotate_right(n->right);
		return rotate_right(n);
	}

	return n;
}

template<class T>
typename BST<T>::Node* BST<T>::find_min(Node* x) const
{
	Node* min = x;

	while (min->left != nullptr)
		min = min->left;

	return min;
}

template<class T>
typename BST<T>::Node* BST<T>::find_next(Node* x) const
{
	Node* temp;

	if (x->right != nullptr)
		return find_min(x->right);

	temp = x->parent;
	while (temp != nullptr && x == temp->right)
	{
		x = temp;
		temp = temp->parent;
	}

	return temp;
}

template<class T>
typename BST<T>::Node* BST<T>::find_max() const
{
	Node* max = root;

	while (max->right != nullptr)
		max = max->right;

	return max;
}

template<class T>
int BST<T>::height(Node* n)
{
	if (n == nullptr)
		return 0;
	else return n->height;
}

template<class T>
int BST<T>::h1(Node* n)
{
	if (n == nullptr)
		return 0;
	return 1 + max(height(n->left), height(n->right));
}

