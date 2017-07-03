// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#include<iostream>
#include"provided.h"
using namespace std;
template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap()
    {
        m_root = nullptr;
        m_size = 0;
    }
    ~MyMap(){clear();}
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    struct Node //used for the binary search tree
    {
        Node(const KeyType& key, const ValueType& value)
        {
            m_key = key;
            m_value = value;
            m_left = nullptr;
            m_right = nullptr;
        }
        ~Node()
        {}
        KeyType m_key;
        ValueType m_value; //holds the 2 values being stored
        Node* m_left;
        Node* m_right;
    };
    
    Node* m_root; //points to the first value in the search tree
    int m_size;
    const ValueType* search(Node* root, const KeyType& key) const;
    void insert(Node*& root, const KeyType& key, const ValueType& value);
    void freetree(Node* root);
};

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType,ValueType>:: search(Node* root, const KeyType& key) const
{
    if(root == nullptr)
    {//cerr<<"nothing"<<endl;
        return nullptr;}
    if(root -> m_key == key)
    {//cerr<<"found"<<endl;
        return &(root -> m_value);}
    else if(root -> m_key > key)
    {//cerr<<"left"<<endl;
        return search(root -> m_left,key);}
    else if(root -> m_key < key)
    {//cerr<<"right"<<endl;
        return search(root -> m_right,key);}
    else
    {//cerr<<"something wrong";
        return nullptr;}
}


template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType,ValueType>::find(const KeyType& key) const
{
    //cerr<<"search"<<endl;
    const ValueType* i;
    i = search(m_root,key);
    return i;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType,ValueType>::insert(Node*& root, const KeyType& key, const ValueType& value)
{
    
    if(root == nullptr)
    {   root = new Node(key, value);
        m_size++;
        //cerr<<"call"<<endl;
        //cerr<<root -> m_key<<" "<<root -> m_value<<endl;
        
        return;}
    if(root -> m_key == key)
    {    root -> m_value = value;
        //cerr<<"callsame"<<endl;
        return;}
    else if(root -> m_key > key)
    {insert(root -> m_left,key,value);
        //cerr<< "callless"<<endl;
    }
    else if(root -> m_key < key)
    {insert(root -> m_right,key,value);
        //cerr<<"callmore"<<endl;
    }
   
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType,ValueType>::associate(const KeyType& key, const ValueType& value)
{
    insert(m_root,key,value);
    //cerr<<"call"<<endl;
}


template<typename KeyType, typename ValueType>
void MyMap<KeyType,ValueType>::freetree(Node* root)
{
    if(root == nullptr)
        return;
    freetree(root -> m_left);
    freetree(root -> m_right);
    delete root;
}
template<typename KeyType, typename ValueType>
int MyMap<KeyType,ValueType>::size() const
{
    return m_size;
}


template<typename KeyType, typename ValueType>
void MyMap<KeyType,ValueType>::clear()
{
    freetree(m_root);
}
//#ifndef MYMAP_INCLUDED
//#define MYMAP_INCLUDED
//
//#include <map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN
//
//// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
//// since you are not allowed to use any STL associative containers, and
//// this implementation uses std::map.
//
//// This code is deliberately obfuscated.
//
//// If you can not get your own MyMap class template working, you may use
//// this one during development in order to let you proceed with implementing
//// the other classes for this project; you can then go back to working on
//// fixing your own MyMap class template.
//
//template <typename KeyType, typename ValueType>
//class MyMap
//{
//public:
//    MyMap(const MyMap &other) = delete;
//    MyMap &operator=(const MyMap &other) = delete;
//    typedef KeyType O;using O1=int;void clear(){l01.clear();}
//    using l0=ValueType;MyMap(){}using l10=O const;O1 size()
//    const{return l01.size();}using ll0=l0 const;using Const=
//    MyMap<O,l0>;void associate(l10&Using,ll0&first){l01[
//                                                        Using]=first;}using l1=std::map<O,l0>;using l00=Const
//    const;ll0*find(l10&l11)const{auto first=l01.find(l11);
//        return(first!=l01.end()?&first->second:0);}l0*find(l10&
//                                                           l01){return(l0*)(*(l00*)(this)).find(l01);}private:l1
//    l01;
//};
//
//#endif // MYMAP_INCLUDED
