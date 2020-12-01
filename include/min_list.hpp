//
//  min_list.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef min_list_h
#define min_list_h

namespace cx
{
    template<class T>
    struct MinListNode
    {
        using value_type = T;
        
        T& GetValue()
        {
            return *reinterpret_cast<T*>(this);
        }
        
        const T& GetValue() const
        {
            return *reinterpret_cast<const T*>(this);
        }
        
        MinListNode<T>* prev = this;
        MinListNode<T>* next = this;
    };
    
    template<class T>
    class MinListIterator
    {
    public:
        using value_type = T;
        
        MinListIterator(MinListNode<T>* node)
        : _node(node) {}
        
        T& operator*()
        {
            return _node->GetValue();
        }
        
        const T& operator*() const
        {
            return _node->GetValue();
        }
        
        T* operator->()
        {
            return &_node->GetValue();
        }
        
        const T* operator->() const
        {
            return &_node->GetValue();
        }
        
        MinListIterator<T>& operator++()
        {
            _node = _node->next;
            return *this;
        }
        
        MinListIterator<T>& operator++(int)
        {
            return (*this)++;
        }
        
        bool operator==(const MinListIterator<T>& other) const
        {
            return this->_node == other._node;
        }
        
        bool operator!=(const MinListIterator<T>& other) const
        {
            return this->_node != other._node;
        }
        
    private:
        MinListNode<T>* _node;
    };
    
    template<class T>
    class MinList
    {
    public:
        MinListIterator<T> begin()
        {
            return {_anchor.next};
        }
        
        MinListIterator<T> end()
        {
            return {&_anchor};
        }
        
        void InsertNode(MinListNode<T>* node)
        {
            _anchor.prev->next = node;
            node->prev = _anchor.prev;
            node->next = &_anchor;
            _anchor.prev = node;
        }
        
    private:
        MinListNode<T> _anchor;
    };
}

#endif /* min_list_h */
