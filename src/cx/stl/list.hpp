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
            ListNode<T>* prev;
            ListNode<T>* next;
            
            T value;
        };
        
        template<class T>
        class ListNodeIterator
        {
        public:
            ListNodeIterator(ListNode<T>* begin)
            _node(begin)
            {}
            
            T& operator*()
            {
                return _node->value;
            }
            
            T* operator->()
            {
                return &_node->value;
            }
            
            ListNodeIterator& operator--()
            {
                _node = _node->prev;
                return *this;
            }
            
            ListNodeIterator& operator++()
            {
                _node = _node->next;
                return *this;
            }
            
            bool operator==(const ListNodeIterator<T>& other) const
            {
                return _node == other._node;
            }
            
            bool operator!=(const ListNodeIterator<T>& other) const
            {
                return !(*this == other);
            }
            
        private:
            ListNode<T>* _node;
        };
    }
    
    template<class T>
    class list
    {
    public:
        detail::ListNodeIterator<T> begin()
        {
            return {_anchor};
        }
        
        detail::ListNodeIterator<T> end()
        {
            return {nullptr};
        }
        
        // TODO
        
    private:
        detail::ListNode<T>* _anchor = nullptr;
    };
}

#endif /* list_h */
