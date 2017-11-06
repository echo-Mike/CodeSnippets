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
//SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>
//#include <DebugLib/mDebugLib.hpp>

namespace Patterns {

    template < typename T >
    class PipelineContainerInterface {
    public:
        virtual void push_back(T*) = 0;
        virtual T* pop_back() = 0;
        virtual void push_front(T*) = 0;
        virtual T* pop_front(T*&) = 0;
        virtual T* clear() = 0;
    };

    template < typename InterfaceT > class Pipeline;
    template < typename InterfaceT > class PipelineIterator;

    template < typename InterfaceT >
    class PipelineEntry : 
        public InterfaceT,
        public PipelineContainerInterface<PipelineEntry<InterfaceT>>
    {
        friend class Pipeline<InterfaceT>;
        friend class PipelineIterator<InterfaceT>;

        using ContainerInterfcae = PipelineContainerInterface<PipelineEntry>;

        PipelineEntry* tail;
        const PipelineEntry* head;

        inline void setTail(const PipelineEntry* tail_) { tail = const_cast<PipelineEntry*>(tail_); }

        inline void setHead(const PipelineEntry* head_) { head = const_cast<PipelineEntry*>(head_); }

    protected: /// Implementation of container interface
        
        void push_back(PipelineEntry* newTail) override final {
            if (tail)
                dynamic_cast<ContainerInterfcae*>(tail)->push_back(newTail);
            else {
                tail = newTail;
                (*tail).setHead(this); 
            }
        }

        PipelineEntry* pop_back() override final {
            if (tail) {
                auto result = dynamic_cast<ContainerInterfcae*>(tail)->pop_back();
                if (result == tail) tail = nullptr;
                return result;
            } else
                return const_cast<PipelineEntry*>(this);
        }

        void push_front(PipelineEntry* newHead) override final {
            if (head)
                dynamic_cast<ContainerInterfcae*>(const_cast<PipelineEntry*>(head))->push_front(newHead);
            else {
                head = newHead;
                (*const_cast<PipelineEntry*>(head)).setTail(this); 
            }
        }

        PipelineEntry* pop_front(PipelineEntry* &newFront) override final {
            if (head) {
                auto result = dynamic_cast<ContainerInterfcae*>(const_cast<PipelineEntry*>(head))->pop_front(newFront);
                if (result == head) {
                    head = nullptr;
                    newFront = const_cast<PipelineEntry*>(this);
                }
                return result;
            } else 
                return const_cast<PipelineEntry*>(this);
        }

        PipelineEntry* clear() override final {
            if (tail) delete dynamic_cast<ContainerInterfcae*>(tail)->clear();
            return const_cast<PipelineEntry*>(this);
        }

    public: /// Implementation of entry interface
        using Interface = InterfaceT;

        PipelineEntry() : tail(nullptr), head(nullptr) {}

        virtual ~PipelineEntry() {}

        NotCopyConstructible(PipelineEntry)

        NotCopyAssignable(PipelineEntry)

        PipelineEntry(PipelineEntry&& other) :
            head(other.head),
            tail(other.tail)
        {
            other.head = nullptr;
            other.tail = nullptr;
            if (head) (*head).setTail(this);
            if (tail) (*tail).setHead(this);
        }

        PipelineEntry& operator=(PipelineEntry&& other)
        {
            if (this == &other) return *this;
            if (head) (*head).setTail(tail);
            if (tail) (*tail).setHead(head);
            head = other.head;
            tail = other.tail;
            other.head = nullptr;
            other.tail = nullptr;
            if (head) (*head).setTail(this);
            if (tail) (*tail).setHead(this);
            return *this;
        }

        inline Interface* getTail() { return dynamic_cast<Interface*>(tail); }

        inline bool hasTail() { return static_cast<bool>(tail); }

        inline Interface* getHead() { return dynamic_cast<Interface*>(head); }

        inline bool hasHead() { return static_cast<bool>(head); }

    //protected:

        // inline iterator begin() override final { 
        //     return head ? static_cast<PipelineContainerInterface*>(head)->begin() : static_cast<iterator>(this);
        // }

        // inline const_iterator cbegin() override final { 
        //     return head ? static_cast<PipelineContainerInterface*>(head)->cbegin() : static_cast<const_iterator>(this);
        // }

        // inline iterator end() override final { return nullptr; }

        // inline const_iterator cend() override final { return nullptr; }

    };

    template < typename InterfaceT > 
    class PipelineIterator 
    {
    public:
        using Interface = InterfaceT;
        using EntryType = PipelineEntry<Interface>;
    private:
        EntryType* ptr;
    public:
        PipelineIterator() : ptr(nullptr) {}

        PipelineIterator(const EntryType* ptr_) : ptr(const_cast<EntryType*>(ptr_)) {}

        PipelineIterator(const PipelineIterator& other) : ptr(other.ptr) {}

        PipelineIterator& operator=(const PipelineIterator& other) {
            ptr = other.ptr;
            return *this;
        }

        PipelineIterator& operator++() {
            ptr = (ptr && ptr->hasTail()) ? ptr->tail : nullptr;
            return *this;
        }

        PipelineIterator operator++(int) {
            PipelineIterator old{ this };
            const_cast<PipelineIterator*>(this)->operator++();
            return old;
        }

        PipelineIterator& operator--() {
            ptr = (ptr && ptr->hasHead()) ? ptr->head : nullptr;
            return *this;
        }

        PipelineIterator& operator--(int) {
            PipelineIterator old{ this };
            const_cast<PipelineIterator*>(this)->operator--();
            return old;
        }

        EntryType* operator->() const { return ptr; }

        operator bool() const { return static_cast<bool>(ptr); }

        friend void swap(PipelineIterator& lhs, PipelineIterator& rhs) { std::swap(lhs.ptr, rhs.ptr); }

        friend bool operator==(const PipelineIterator& lhs, const PipelineIterator& rhs) { return lhs.ptr == rhs.ptr; }

        friend bool operator!=(const PipelineIterator& lhs, const PipelineIterator& rhs) { return !(lhs == rhs); }
    };

    template < typename InterfaceT >
    class Pipeline : 
        public PipelineContainerInterface<PipelineEntry<InterfaceT>> 
    {
    public:
        using Interface = InterfaceT;
        using EntryType = PipelineEntry<Interface>;
        using iterator = PipelineIterator<Interface>;
    private:
        EntryType* head;
        using EntryContainerInterface = PipelineContainerInterface<EntryType>;
    public:

        Pipeline() : head(nullptr) {}

        ~Pipeline() noexcept { clear(head); }

        void push_back(EntryType* newTail) override final {
            if (head)
                static_cast<EntryContainerInterface*>(head)->push_back(newTail);
            else
                head = newTail;
        }

        EntryType* pop_back() override final {
            if (head) {
                auto result = dynamic_cast<EntryContainerInterface*>(head)->pop_back();
                if (result == head) head = nullptr;
                return result;
            } else
                return nullptr;
        }

        void push_front(EntryType* newHead) override final {
            if (head)
                dynamic_cast<EntryContainerInterface*>(head)->push_front(newHead);
            else
                head = newHead;
        }

        EntryType* pop_front() {
            if (head) {
                EntryType* newFront = nullptr;
                auto result = dynamic_cast<EntryContainerInterface*>(head)->pop_front(newFront);
                head = newFront;
                return result;
            } else
                return nullptr;
        }

        void clear(Pipeline*) { clear(head); }
        
        bool empty() { return static_cast<bool>(head); }

        iterator begin() { return iterator{ head }; }

        const iterator cbegin() { return iterator{ head }; }

        iterator end() { return iterator(); }

        const iterator cend() { return iterator(); }

    private:

        static void clear(EntryType* ptr_) noexcept 
        {
            try {
                if (ptr_) delete dynamic_cast<EntryContainerInterface*>(ptr_)->clear(); 
            }
            catch(std::exception& e) {
                //TODO: add debug_out msg
            }
            catch(...) {
                //TODO: add debug_out msg
            }
        }

        EntryType* clear() override final { return nullptr; }

        EntryType* pop_front(EntryType* &/*newFront*/) override final { return nullptr; }
    };
}

#endif