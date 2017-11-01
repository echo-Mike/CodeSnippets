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
#include <ClassUtilsLib/mClassUtils.hpp>

namespace Patterns {

    template < typename InterfaceT >
    class PipelineIteratorInterface {
    public:
        virtual PipelineIteratorInterface& operator++() = 0;
        virtual PipelineIteratorInterface& operator++(int) = 0;
        virtual PipelineIteratorInterface& operator--() = 0;
        virtual PipelineIteratorInterface& operator--(int) = 0;
        virtual InterfaceT& operator*() const = 0;
        virtual InterfaceT* operator->() const = 0;
        friend void swap(PipelineIteratorInterface& lhs, PipelineIteratorInterface& rhs);
        friend bool operator==(const PipelineIteratorInterface& lhs, const PipelineIteratorInterface& rhs);
        friend bool operator!=(const PipelineIteratorInterface& lhs, const PipelineIteratorInterface& rhs);
    };

    template < typename T >
    class PipelineContainerInterface {
    public:
        using iterator = PipelineIteratorInterface<typename T::Interface> *;
        using const_iterator = const PipelineIteratorInterface<typename T::Interface> *;

        virtual void push_back(T*) = 0;
        virtual T* pop_back() = 0;
        virtual void push_front(T*) = 0;
        virtual T* pop_front(T*&) = 0;
        virtual T* clear() = 0;
        virtual iterator begin() = 0;
        virtual const_iterator cbegin() = 0;
        virtual iterator end() = 0;
        virtual const_iterator cend() = 0;

    };

    class Pipeline;

    template < typename InterfaceT >
    class PipelineEntry : 
        public InterfaceT, 
        public PipelineContainerInterface<PipelineEntry>, 
        public PipelineIteratorInterface<InterfaceT>
    {
        friend class Pipeline;

        PipelineEntry* tail;
        const PipelineEntry* head;

        inline void setTail(const PipelineEntry* tail_) { next = const_cast<PipelineEntry*>(tail_); }

        inline void setHead(const PipelineEntry* head_) { head = const_cast<PipelineEntry*>(head_); }

    protected:
    /// Implementation of container interface

    void push_back(PipelineEntry* newTail) override final {
        if (tail)
            static_cast<PipelineContainerInterface*>(tail)->push_back(newBack);
        else {
            tail = newBack;
            (*tail).setHead(this); 
        }
    }

    PipelineEntry* pop_back() override final {
        if (tail) {
            auto result = static_cast<PipelineContainerInterface*>(tail)->pop_back();
            if (result == tail) tail == nullptr;
            return result;
        } else
            return const_cast<PipelineEntry*>(this);
    }

    void push_front(PipelineEntry* newHead) override final {
        if (head)
            static_cast<PipelineContainerInterface*>(head)->push_front(newHead);
        else {
            head = newHead;
            (*head).setTail(this); 
        }
    }

    PipelineEntry* pop_front(PipelineEntry* &newFront) override final {
        if (head) {
            auto result = static_cast<PipelineContainerInterface*>(head)->pop_front(newFront);
            if (result == head) {
                head = nullptr;
                newFront = const_cast<PipelineEntry*>(this);
            }
            return result;
        } else 
            return const_cast<PipelineEntry*>(this);
    }

    PipelineEntry* clear() override final {
        if (tail) delete (*tail).clear();
        return const_cast<PipelineEntry*>(this);
    }

    public:
        using Interface = InterfaceT;

        PipelineEntry() : next(nullptr), owner(nullptr) {}

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

        inline Interface* getNext() { return static_cast<Interface*>(next); }

        inline bool hasNext() { return static_cast<bool>(next); }

        inline Interface* getOwner() { return static_cast<Interface*>(owner); }

        inline bool hasOwner() { return static_cast<bool>(owner); }

    protected:

        inline iterator begin() override final { 
            return head ? static_cast<PipelineContainerInterface*>(head)->begin() : static_cast<iterator>(this);
        }

        inline const_iterator cbegin() override final { 
            return head ? static_cast<PipelineContainerInterface*>(head)->cbegin() : static_cast<const_iterator>(this);
        }

        inline iterator end() override final { return nullptr; }

        inline const_iterator cend() override final { return nullptr; }

    };
}


#endif