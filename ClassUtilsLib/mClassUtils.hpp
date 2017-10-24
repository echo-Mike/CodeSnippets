#pragma once
#ifndef CLASS_UTILS_LIB_HPP__
#define CALSS_UTILS_LIB_HPP__ "multi@mClassUtils.hpp"
/**
*	DESCRIPTION:
*		Module contains implementation of macro utils for classes.
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
/*******************************/
/*         DESTRUCTOR          */
/*******************************/

#define VirtualDestructor(className)\
    virtual ~className();

/*******************************/
/*        CONSTRUCTOR          */
/*******************************/

#define DefaultConstructible(className)\
    className() = default;

#define NotDefaultConstructible(className)\
    className() = delete;

/*******************************/
/*            COPY             */
/*******************************/

#define DefaultCopyConstructible(className)\
    className(const className&) = default;

#define DefaultCopyAssignable(className)\
    className& operator=(const className&) = default;

#define DefaultCopyable(className)\
    DefaultCopyConstructible(className)\
    DefaultCopyAssignable(className)

#define NotCopyConstructible(className)\
    className(const className&) = delete;

#define NotCopyAssignable(className)\
    className& operator=(const className&) = delete;

#define NotCopyable(className)\
    NotCopyConstructible(className)\
    NotCopyAssignable(className)

/*******************************/
/*            MOVE             */
/*******************************/

#define DefaultMoveConstructible(className)\
    className(className&&) = default;

#define DefaultMoveAssignable(className)\
    className& operator=(className&&) = default;

#define DefaultMovable(className)\
    DefaultMoveConstructible(className)\
    DefaultMoveAssignable(className)

#define NotMoveConstructible(className)\
    className(className&&) = delete;

#define NotMoveAssignable(className)\
    className& operator=(className&&) = delete;

#define NotMovable(className)\
    NotMoveConstructible(className)\
    NotMoveAssignable(className)

/*******************************/
/*           MIXED             */
/*******************************/

#define DefaultCopyableAndMovable(className)\
    DefaultCopyable(className)\
    DefaultMovable(className)

#define NotCopyableNotMovable(className)\
    NotCopyable(className)\
    NotMovable(className)

#define DefaultCopyableNotMovable(className)\
    DefaultCopyable(className)\
    NotMovable(className)

#define DefaultMovableNotCopyable(className)\
    NotCopyable(className)\
    DefaultMovable(className)

#define DefaultCopyAndMoveConstructible(className)\
    DefaultCopyConstructible(className)\
    DefaultMoveConstructible(className)

#define DefaultCopyAndMoveAssignable(className)\
    DefaultCopyAssignable(className)\
    DefaultMoveAssignable(className)

#define DefaultConstructibleNotAssignable(className)\
    DefaultCopyAndMoveConstructible(className)\
    NotCopyAssignable(className)\
    NotMoveAssignable(className)

#define DefaultAssignableNotConstructible(className)\
    NotCopyConstructible(className)\
    NotMoveConstructible(className)\
    DefaultCopyAndMoveAssignable(className)
    
#endif