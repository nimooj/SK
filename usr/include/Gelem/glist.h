#pragma once

template<class T> class GObNode
{
public:
	GObNode() { m_pLeft = m_pRight = 0; }
public:
	T	data;
	GObNode<T> *m_pLeft, *m_pRight;
};

template<class T> class GPtrNode
{
public:
	GPtrNode() { m_pLeft = m_pRight = 0; }
	~GPtrNode() { delete data; }
public:
	T	data;
	GPtrNode<T> *m_pLeft, *m_pRight;
};

template<class T> class GObList
{
public:
	// default constructor.
	GObList();
	// copy constructor.
	GObList(GObList<T>& list);
	// destructor.
	~GObList();
	// operator=  overloading.
	GObList<T>&		operator=(GObList<T>& list);
	// operator+  overloading.
	GObList<T>&		operator+(GObList<T>& list);
	// return the index of Node.
	int	Search(const T&) const;
	// return node that is specified by index.
	T& operator[](int index) const;
	// delete the spicified node.
	GObList<T>&		Delete(T&);
	// insert the new node.
	GObList<T>&		Insert(T&);
	// delete all nodes.
	GObList<T>&		DeleteAll();
public:
	int m_nCount;
	GObNode<T> *m_pLeftEnd, *m_pRightEnd;
};
template<class T>
GObList<T>::GObList()
{
	m_pLeftEnd = m_pRightEnd = 0;
	m_nCount = 0;
}
template< class T >
GObList<T>::GObList(GObList<T>& list)
{
	m_pLeftEnd = m_pRightEnd = 0;
	m_nCount = 0;
	GObNode<T>* pNode = list.m_pLeftEnd;
	while (pNode)
	{
		this->Insert(pNode->data);
		pNode = pNode->m_pRight;
	}
}
template<class T>
GObList<T>::~GObList()
{
	GObNode<T> *next;
	while (m_pLeftEnd)
	{
		next = m_pLeftEnd->m_pRight;
		delete m_pLeftEnd;
		m_pLeftEnd = next;
	}
}
//////////////////////////////////////////////////////////////////////
template<class T >
int GObList<T>::Search(const T& x) const
{
	/*GObNode<T> *start = m_pLeftEnd;
	int k=0;
	while( start )
	{
		if ( start->data == x )
			return k;

		start = start->m_pRight;
		k++;
	}*/
	return 0;

}
template < class T >
GObList<T>& GObList<T>::Delete(T& x)
{
	GObNode<T> *start = m_pLeftEnd;
	while (start)
	{
		if (start->data == x)
		{
			if (m_nCount == 1)
			{
				delete start;
				m_nCount = 0;
				m_pLeftEnd = m_pRightEnd = 0;
				return *this;
			}
			if (start->m_pLeft == 0)
			{

				m_pLeftEnd = start->m_pRight;
				start->m_pRight->m_pLeft = 0;
			}
			else if (start->m_pRight == 0)
			{
				m_pRightEnd = start->m_pLeft;
				start->m_pLeft->m_pRight = 0;
			}
			else
			{
				start->m_pLeft->m_pRight = start->m_pRight;
				start->m_pRight->m_pLeft = start->m_pLeft;
			}
			delete start;
			m_nCount--;
			return *this;
		}
		start = start->m_pRight;
	}
	return *this;
}
template <class T>
GObList<T>& GObList<T>::DeleteAll()
{
	GObNode<T> *next;
	while (m_pLeftEnd)
	{
		next = m_pLeftEnd->m_pRight;
		delete m_pLeftEnd;
		m_pLeftEnd = next;
	}
	m_nCount = 0;
	m_pLeftEnd = m_pRightEnd = 0;
	return *this;
}

template <class T>
GObList<T>& GObList<T>::Insert(T& x)
{
	GObNode<T> *X = new GObNode<T>;
	GObNode<T> *Y = m_pRightEnd;
	m_nCount++;
	if (Y == 0)
	{
		X->data = x;
		m_pLeftEnd = X;
		m_pRightEnd = X;
		return *this;
	}
	X->data = x;
	X->m_pLeft = Y;
	X->m_pRight = Y->m_pRight;
	Y->m_pRight = X;
	m_pRightEnd = X;
	return *this;
}
template <class T>
T& GObList<T>::operator[](int index) const
{
	GObNode<T> *start = m_pLeftEnd;
	int temp = 0;
	while (start)
	{
		if (index == temp)
			return start->data;
		temp++;
		start = start->m_pRight;
	}
	return m_pLeftEnd->data;
}
template<class T>
GObList<T>& GObList<T>::operator=(GObList<T>& list)
{
	this->DeleteAll();
	m_pLeftEnd = m_pRightEnd = 0;
	m_nCount = 0;
	GObNode<T>* pNode = list.m_pLeftEnd;
	while (pNode)
	{
		this->Insert(pNode->data);
		pNode = pNode->m_pRight;
	}
	return *this;
}
template< class T >
GObList<T>& GObList<T>::operator+(GObList<T>& list)
{
	GObNode<T> * pNode = list.m_pLeftEnd;
	while (pNode)
	{
		this->Insert(pNode->data);
		pNode = pNode->m_pRight;
	}
	return *this;
}


template<class T> class GPtrList
{
public:
	// default constructor.
	GPtrList();
	// copy constructor.
	GPtrList(GPtrList<T>& list);
	// destructor.
	~GPtrList();
	// operator=  overloading.
	GPtrList<T>&		operator=(GPtrList<T>& list);
	// operator+  overloading.
	GPtrList<T>&		operator+(GPtrList<T>& list);
	// return the index of Node.
	int	Search(const T&) const;
	// return node that is specified by index.
	T& operator[](int index) const;
	// delete the spicified node.
	GPtrList<T>&		Delete(T&);
	// insert the new node.
	GPtrList<T>&		Insert(T&);
	// delete all nodes.
	GPtrList<T>&		DeleteAll();
public:
	int m_nCount;
	GPtrNode<T> *m_pLeftEnd, *m_pRightEnd;
};
template<class T>
GPtrList<T>::GPtrList()
{
	m_pLeftEnd = m_pRightEnd = 0;
	m_nCount = 0;
}
template< class T >
GPtrList<T>::GPtrList(GPtrList<T>& list)
{
	m_pLeftEnd = m_pRightEnd = 0;
	m_nCount = 0;
	GPtrNode<T>* pNode = list.m_pLeftEnd;
	while (pNode)
	{
		this->Insert(pNode->data);
		pNode = pNode->m_pRight;
	}
}
template<class T>
GPtrList<T>::~GPtrList()
{
	GPtrNode<T> *next;
	while (m_pLeftEnd)
	{
		next = m_pLeftEnd->m_pRight;
		delete m_pLeftEnd;
		m_pLeftEnd = next;
	}
}
//////////////////////////////////////////////////////////////////////
template<class T >
int GPtrList<T>::Search(const T& x) const
{
	GPtrNode<T> *start = m_pLeftEnd;
	while (start)
	{
		if (start->data == x) return 1;
		start = start->m_pRight;
	}
	return 0;
}
template < class T >
GPtrList<T>& GPtrList<T>::Delete(T& x)
{
	GPtrNode<T> *start = m_pLeftEnd;
	while (start)
	{
		if (start->data == x)
		{
			if (m_nCount == 1)
			{
				delete start;
				m_nCount = 0;
				m_pLeftEnd = m_pRightEnd = 0;
				return *this;
			}
			if (start->m_pLeft == 0)
			{

				m_pLeftEnd = start->m_pRight;
				start->m_pRight->m_pLeft = 0;
			}
			else if (start->m_pRight == 0)
			{
				m_pRightEnd = start->m_pLeft;
				start->m_pLeft->m_pRight = 0;
			}
			else
			{
				start->m_pLeft->m_pRight = start->m_pRight;
				start->m_pRight->m_pLeft = start->m_pLeft;
			}
			delete start;
			m_nCount--;
			return *this;
		}
		start = start->m_pRight;
	}
	return *this;
}
template <class T>
GPtrList<T>& GPtrList<T>::DeleteAll()
{
	GPtrNode<T> *next;
	while (m_pLeftEnd)
	{
		next = m_pLeftEnd->m_pRight;
		delete m_pLeftEnd;
		m_pLeftEnd = next;
	}
	m_nCount = 0;
	m_pLeftEnd = m_pRightEnd = 0;
	return *this;
}

template <class T>
GPtrList<T>& GPtrList<T>::Insert(T& x)
{
	GPtrNode<T> *X = new GPtrNode<T>;
	GPtrNode<T> *Y = m_pRightEnd;
	m_nCount++;
	if (Y == 0)
	{
		X->data = x;
		m_pLeftEnd = X;
		m_pRightEnd = X;
		return *this;
	}
	X->data = x;
	X->m_pLeft = Y;
	X->m_pRight = Y->m_pRight;
	Y->m_pRight = X;
	m_pRightEnd = X;
	return *this;
}
template <class T>
T& GPtrList<T>::operator[](int index) const
{
	assert(m_nCount > index);
	GPtrNode<T> *start = m_pLeftEnd;
	int temp = 0;
	while (start)
	{
		if (index == temp)
			return start->data;
		temp++;
		start = start->m_pRight;
	}
	T a;
	return a;

}
template<class T>
GPtrList<T>& GPtrList<T>::operator=(GPtrList<T>& list)
{
	this->DeleteAll();
	m_pLeftEnd = m_pRightEnd = 0;
	m_nCount = 0;
	GPtrNode<T>* pNode = list.m_pLeftEnd;
	while (pNode)
	{
		this->Insert(pNode->data);
		pNode = pNode->m_pRight;
	}
	return *this;
}
template< class T >
GPtrList<T>& GPtrList<T>::operator+(GPtrList<T>& list)
{
	GPtrNode<T> * pNode = list.m_pLeftEnd;
	while (pNode)
	{
		this->Insert(pNode->data);
		pNode = pNode->m_pRight;
	}
	return *this;
}
