#pragma once
#ifndef PATTERNS_LIB_PIPELINE_HPP__
#define PATTERNS_LIB_PIPELINE_HPP__ "0.0.0@cPipeline.hpp"
/**
*	DESCRIPTION:
*		Module contains implementation of pipeline pattern.
*	AUTHOR:
*		Mikhail Demchenko
*		mailto:dev.echo.mike@gmail.com
*		https://github.com/echo-Mike
**/
/** 
*   MIT License
*
*   Copyright (c) 2017 Mikhail Demchenko dev.echo.mike@gmail.com
*   
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*   
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*   
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
**/
//STD
#include <exception>
#include <utility>
#include <iterator>
//SNIPPETS
//#include <ClassUtilsLib/mClassUtils.hpp>
//#include <DebugLib/mDebugLib.hpp>

namespace Patterns {

    template < typename InterfaceT >
    class Pipeline
    {
    public:
        class PipelineEntry;
 
        class iterator
        {
        public:
            using Interface = InterfaceT;

            using value_type = PipelineEntry;

            using pointer = value_type*;

            using reference = value_type&;

            using iterator_category = std::bidirectional_iterator_tag;

            using difference_type = std::ptrdiff_t;

            iterator() : current(nullptr), last(nullptr) {}

            iterator(const iterator&) = default;

            iterator& operator=(const iterator&) = default;

            explicit iterator(const value_type* ptr) : 
                current(const_cast<pointer>(ptr)),
                last(nullptr) 
            {}

            ~iterator() = default;

            iterator& operator++() {
                if (current) last = current;
                current = (current && current->hasTail()) ? current->tail : nullptr;
                return *this;
            }

            iterator operator++(int) {
                iterator old{ this };
                const_cast<iterator*>(this)->operator++();
                return old;
            }

            iterator& operator--() {
                if (current) {
                    last = current;
                    current = current->hasHead() ? current->head : nullptr;
                } else if (last) {
                    current = last;
                    last = nullptr;
                }
                return *this;
            }

            iterator operator--(int) {
                iterator old{ this };
                const_cast<iterator*>(this)->operator--();
                return old;
            }

            pointer operator->() const { return current; }

            reference operator*() const { return *current; }

            friend void swap(iterator& lhs, iterator& rhs) 
            { 
                std::swap(lhs.current, rhs.current);
                std::swap(lhs.last, rhs.last);
            }
            
            friend inline bool operator==(const iterator& lhs, const iterator& rhs) 
            { return lhs.current == rhs.current; } //&& lhs.last == rhs.last; }
        
            friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }

            operator bool() const { return static_cast<bool>(current); }

        protected:

            static iterator makeEnd(const value_type* ptr) 
            {
                iterator result;
                result.last = const_cast<pointer>(ptr);
                return result;
            }

        private:
			friend Pipeline;
            pointer current;
            pointer last;
        };

        class const_iterator
        {
        public:
            using Interface = InterfaceT;

            using value_type = PipelineEntry;

            using pointer = const value_type*;

            using reference = const value_type&;

            using iterator_category = std::bidirectional_iterator_tag;

            using difference_type = std::ptrdiff_t;

            const_iterator() : current(nullptr), last(nullptr) {}

            const_iterator(const const_iterator&) = default;

            const_iterator& operator=(const const_iterator&) = default;

            const_iterator(const iterator& other) :
                current(other.current), last(other.last)
            {}
            
            const_iterator& operator=(const iterator& other) 
            {
                current = other.current;
                last = other.last;
            }
            
            explicit const_iterator(pointer ptr) : 
                current(ptr), last(nullptr) 
            {}

            ~const_iterator() = default;

            const_iterator& operator++() {
                if (current) last = current;
                current = (current && current->hasTail()) ? current->tail : nullptr;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator old{ this };
                const_cast<const_iterator*>(this)->operator++();
                return old;
            }

            const_iterator& operator--() {
                if (current) {
                    last = current;
                    current = current->hasHead() ? current->head : nullptr;
                } else if (last) {
                    current = last;
                    last = nullptr;
                }
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator old{ this };
                const_cast<const_iterator*>(this)->operator--();
                return old;
            }

            pointer operator->() const { return current; }
            
            reference operator*() const { return *current; }

            friend void swap(const_iterator& lhs, const_iterator& rhs) 
            { 
                std::swap(lhs.current, rhs.current);
                std::swap(lhs.last, rhs.last);
            }
            
            friend inline bool operator==(const const_iterator& lhs, const const_iterator& rhs)
            { return lhs.current == rhs.current; }// && lhs.last == rhs.last; }
        
            friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) { return !(lhs == rhs); }

            operator bool() const { return static_cast<bool>(current); }
        
        protected:

            static const_iterator makeEnd(const value_type* ptr) 
            {
                const_iterator result;
                result.last = ptr;
                return result;
            }

        private:
			friend Pipeline;
            pointer current;
            pointer last;
        };

        class PipelineEntry :
            public InterfaceT
        {
        public:
            using Interface = InterfaceT;

            PipelineEntry() : tail(nullptr), head(nullptr) {}
            
            virtual ~PipelineEntry() {}

            PipelineEntry(const PipelineEntry&) = delete;

            PipelineEntry& operator=(const PipelineEntry&) = delete;

            PipelineEntry(PipelineEntry&& other) :
                head(other.head), tail(other.tail)
            {
                other.head = nullptr;
                other.tail = nullptr;
                if (head) head->tail = const_cast<PipelineEntry*>(this);
                if (tail) tail->head = const_cast<PipelineEntry*>(this);
            }
        
            PipelineEntry& operator=(PipelineEntry&& other)
            {
                if (this == &other) return *this;
                if (head) head->tail = tail;
                if (tail) tail->head = head;
                head = other.head;
                tail = other.tail;
                other.head = nullptr;
                other.tail = nullptr;
                if (head) head->tail = const_cast<PipelineEntry*>(this);
                if (tail) tail->head = const_cast<PipelineEntry*>(this);
                return *this;
            }

            inline Interface* getTail() const { return dynamic_cast<Interface*>(tail); }
            
            inline bool hasTail() const { return static_cast<bool>(tail); }
        
            inline Interface* getHead() const { return dynamic_cast<Interface*>(head); }
        
            inline bool hasHead() const { return static_cast<bool>(head); }

            inline Interface* getThis() const { return dynamic_cast<Interface*>(const_cast<PipelineEntry*>(this)); }

        private:
			friend Pipeline;
            PipelineEntry* tail;
            PipelineEntry* head;
        };

        using Interface = InterfaceT;
        using InterfacePtr = Interface*;
        using InterfaceRef = Interface&;

        using value_type = PipelineEntry;

        using size_type = std::size_t;

        //using reverse_iterator = std::reverse_iterator<iterator>;
        //using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        using difference_type = typename std::iterator_traits<iterator>::difference_type;

        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        Pipeline() : head(nullptr), tail(nullptr) {}

        ~Pipeline() noexcept { /*setHead();*/ clear(head); }

        // Element access

        reference front() { setHead(); return *head; }

        const_reference front() const { return *head; }

        reference back() { setTail(); return *tail; }

        const_reference back() const { return *tail; }

        // Iterators

        iterator begin() noexcept { setHead(); return iterator{ head }; }
        
        const_iterator begin() const noexcept { return const_iterator{ head }; }

        const_iterator cbegin() const noexcept { return const_iterator{ head }; }

        iterator end() noexcept { setTail(); return iterator::makeEnd(tail); }

        const_iterator end() const noexcept { return iterator::makeEnd(tail); }

        const_iterator cend() const noexcept { return iterator::makeEnd(tail); }

        // Capacity
        
        size_type size() const noexcept 
        {
            // setHead();
            // setTail();
            if (!head) return 0;
            auto first = head;
            auto last = tail;
            size_type result = 1;
            while ( first != last ) {
                if (first->tail) {
                    ++result;
                    first = first->tail;
                } else
                    return static_cast<size_type>(-1);
            }
            return result;
        }

        bool empty() const noexcept { return !(static_cast<bool>(head) || static_cast<bool>(tail)); }

        // Modifiers

        void clear() noexcept { setHead(); clear(head); head = nullptr; tail = nullptr; }

        void push_back(pointer newBack) noexcept
        {
            setTail();

            if (tail) {
                tail->tail = newBack;
                newBack->head = tail;
            } else 
                head = newBack;
            tail = newBack;
        }
        
        pointer pop_back() noexcept
        {
            setTail();

            if (!tail) return nullptr;

            pointer result = tail;
            tail = tail->head;
            tail->tail = nullptr;
            result->head = nullptr;
            return result;
        }

        void push_front(pointer newFront) noexcept
        {
            setHead();

            if (head) {
                head->head = newFront;
                newFront->tail = head;
            } else 
                tail = newFront;
            head = newFront;
        }

        pointer pop_front() noexcept
        {
            setHead();

            if (!head) return nullptr;

            pointer result = head;
            head = head->tail;
            head->head = nullptr;
            result->tail = nullptr;
            return result;
        }

        iterator insert(const_iterator pos, pointer value)
        {
            if (value->head) {
                value->head->tail = value->tail;
                value->head = nullptr;
            }
            if (value->tail) {
                value->tail->head = value->head;
                value->tail = nullptr;
            }

            if (pos.current) {
                if (const_cast<pointer>(pos.current) == head) 
                    push_front(value);
                else if (const_cast<pointer>(pos.current) == tail) 
                    push_back(value);
                else {
                    auto ptr_ = const_cast<pointer>(pos.current);
                    value->head = ptr_->head;
                    ptr_->head->tail = value;
                    prt_->head = value;
                    value->tail = ptr_;
                }
            } else if (pos.last) {
                auto ptr_ = const_cast<pointer>(pos.current);
                ptr_->tail = value;
                value->head = ptr_;
            } else
                push_back(value);

            return iterator{ value }; 
        }

        template< class EntryT,  class... Args > 
        iterator emplace( const_iterator pos, Args&&... args )
        {
            //todo: add concepts: 1. Constuctiable 2. is_base_of
            pointer buffer = nullptr;
            try {
                buffer = new EntryT(std::forward<Args>(args)...);
            }
            catch (...) {
                //todo: add debug msg
                return end();
            }
            return insert(pos, buffer);
        }

        iterator erase( const_iterator pos )
        {
            setHead();
            setTail();
            iterator result = end();
            if (!pos.current) return result;
            result.last = nullptr;
            auto obj = const_cast<pointer>(pos.current);
            if (obj == head) {
                try { delete pop_front(); }
                catch(...) { 
                    //todo: add dbg msg 
                }
                result.current = head
                return result;
            }
            if (obj == tail) {
                try { delete pop_back(); }
                catch(...) {
                    //todo: add dbg msg 
                }
                result.current = tail
                return result;
            }

            if (obj->head)
                obj->head->tail = obj->tail;
            else {
                //todo: add dbg msg
                result.current = obj
                return result;
            }

            if (obj->tail) 
                obj->tail->head = obj->head;
            else {
                //todo: add dbg msg
                result.current = obj
                return result;
            }

            result.current = obj->tail;
            try { delete obj; }
            catch(...) { 
                //todo: add dbg msg 
            }
            return result;
        }

        iterator erase( const_iterator first, const_iterator last ) 
        {
            if (first == last || !first.current) return end();
            setHead();
            setTail();

            if (last.current) {
                auto firstObj = const_cast<pointer>(first.current);
                auto lastObj = const_cast<pointer>(last.current);
                pointer beforeLastObj = firstObj;
                while (beforeLastObj->tail != lastObj) {
                    if (beforeLastObj->tail)
                        beforeLastObj = beforeLastObj->tail;
                    else
                        return end();
                }

                beforeLastObj->tail = nullptr;

                if (firstObj == head) {
                    head = lastObj;
                    lastObj->head = nullptr;
                } else if (firstObj->head) {
                    firstObj->head->tail = lastObj;
                    lastObj->head = firstObj->head;
                } else {
                    beforeLastObj->tail = lastObj;
                    return end();
                }

                clear(firstObj);
                return iterator{ lastObj };
            } else {
                auto obj = const_cast<pointer>(first.current);

                if (obj == head) clear();

                if (obj == tail) {
                    try { delete pop_back(); }
                    catch(...) {
                        //todo: add dbg msg 
                    }
                    return end();
                }

                if (obj->head) {
                    obj->head->tail = nullptr;
                    tail = obj->head;
                } else {
                    //todo: add dbg msg
                    return end();
                }

                clear(obj);
                return end();
            }
        }

        template< class EntryT, class... Args >
        reference emplace_back( Args&&... args )
        {
            return *emplace<EntryT, Args...>(end(), std::forward<Args>(args)...);
        }

        template< class EntryT, class... Args >
        reference emplace_front( Args&&... args )
        {
            return *emplace<EntryT, Args...>(begin(), std::forward<Args>(args)...);
        }

        void swap( Pipeline& other ) noexcept { std::swap(head, other.head); std::swap(tail, other.tail); }

    private:
        pointer head;
        pointer tail;

        inline void setHead() noexcept
        {
            if (!head)
                if (tail)
                    head = tail;
                else
                    return;
            
            while (head->head)
                head = head->head;
        }

        inline void setTail() noexcept
        {
            if (!tail)
                if (head)
                    tail = head;
                else
                    return;
            
            while (tail->tail)
                tail = tail->tail;
        }

        static void clear(pointer ptr_) noexcept 
        {
            pointer next = nullptr;
            for(;;) {
                if (ptr_) next = ptr_->tail;
                try { delete ptr_; }
                catch(std::exception& e) {
                    //TODO: add debug_out msg
                }
                catch(...) {
                    //TODO: add debug_out msg
                }
                if (next)
                    ptr_ = next;
                else
                    break;
            }
        }
    };

    template < typename InterfaceT >
    using PipelineEntry = typename Pipeline<InterfaceT>::PipelineEntry;

}

#endif