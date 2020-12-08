//
//  list.hpp
//  cxkrnltest
//
//  Created by Nikita Ivanov on 02.12.2020.
//  Copyright © 2020 PlakOS. All rights reserved.
//

#ifndef list_h
#define list_h

namespace cx::std
{
    namespace detail
    {
        template<class T>
        struct ListNode
        {
            using value_type = T;
            
            ListNode<T>* prev = nullptr;
            ListNode<T>* next = nullptr;
            
            T value = {};
            
            ~ListNode()
            {
                if (next)
                    delete next;
            }
        };
        
        template<class Node, class T = typename Node::value_type>
        class ListNodeIteratorBase
        {
        public:
            ListNodeIteratorBase(Node* begin)
            : _node(begin)
            {}
            
            T& operator*() const
            {
                return _node->value;
            }
            
            T* operator->() const
            {
                return &_node->value;
            }
            
            ListNodeIteratorBase& operator--()
            {
                _node = _node->prev;
                return *this;
            }
            
            ListNodeIteratorBase& operator--(int)
            {
                return --(*this);
            }
            
            ListNodeIteratorBase& operator++()
            {
                _node = _node->next;
                return *this;
            }
            
            ListNodeIteratorBase& operator++(int)
            {
                return ++(*this);
            }
            
            bool operator==(const ListNodeIteratorBase<Node, T>& other) const
            {
                return _node == other._node;
            }
            
            bool operator!=(const ListNodeIteratorBase<Node, T>& other) const
            {
                return !(*this == other);
            }
            
        private:
            Node* _node;
        };
        
        template<class T>
        using ListNodeIterator = ListNodeIteratorBase<ListNode<T>>;
        
        template<class T>
        using ListNodeConstIterator = ListNodeIteratorBase<const ListNode<T>, const T>;
    }
    
    template<class T>
    class list
    {
    public:
        using value_type = T;
        using iterator = detail::ListNodeIterator<T>;
        using const_iterator = detail::ListNodeConstIterator<T>;
        
        list() = default;
        
        list(const list& other)
        {
            for(auto& value : other)
                push_back(value);
        }
        
        list(list&& other)
        {
            this->_anchor = other._anchor;
            other._anchor = nullptr;
        }
        
        ~list()
        {
            if(_anchor)
                delete _anchor;
        }
        
        iterator begin()
        {
            return {_anchor};
        }
        
        iterator end()
        {
            return {nullptr};
        }
        
        const_iterator begin() const
        {
            return {_anchor};
        }
        
        const_iterator end() const
        {
            return {nullptr};
        }
        
        iterator push_back(const T& value)
        {
            if(!_anchor)
            {
                CreateAnchor(value);
                return _anchor;
            }
            else
            {
                auto node = new detail::ListNode<T>{_anchor->prev, nullptr, value};
                _anchor->prev->next = node;
                _anchor->prev = node;
                return node;
            }
        }
        
        iterator push_front(const T& value)
        {
            if(!_anchor)
            {
                CreateAnchor(value);
            }
            else
            {
                auto node = new detail::ListNode<T>{_anchor->prev, _anchor, value};
                _anchor = node;
            }
            
            return _anchor;
        }
        
        template<class F>
        iterator find_if(F&& predicate)
        {
            for(auto it = begin(); it != end(); it++)
                if(predicate(*it))
                    return it;
            
            return end();
        }
        
        iterator find(const T& value)
        {
            return find_if([&value] (const T& in)
                           {
                               return in == value;
                           });
        }
        
    private:
        detail::ListNode<T>* _anchor = nullptr;
        
        void CreateAnchor(const T& value)
        {
            if(_anchor)
                delete _anchor;
            
            _anchor = new detail::ListNode<T>{nullptr, nullptr, value};
            _anchor->prev = _anchor;
        }
    };
}

#endif /* list_h */
