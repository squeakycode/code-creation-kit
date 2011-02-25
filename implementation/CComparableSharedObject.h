//   Copyright (C) 2011 Andreas Gau
//
//   This file is part of the code-creation-kit.
//
//   The code-creation-kit is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 2 of the License, or
//   (at your option) any later version.
//
//   The code-creation-kit is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with the code-creation-kit. If not, see <http://www.gnu.org/licenses/>.

#ifndef INCLUDED_CCOMPARABLESHAREDOBJECT_H_4365503
#define INCLUDED_CCOMPARABLESHAREDOBJECT_H_4365503

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <boost/shared_ptr.hpp>

///allows to compare objects held by shared_ptr
template <typename T>
class CComparableSharedObject
{
public:
    CComparableSharedObject()
    {
    }

    CComparableSharedObject( T* ptr)
        : m_ptr( ptr)
    {
    }

    CComparableSharedObject( boost::shared_ptr<T> ptr)
        : m_ptr( ptr)
    {
    }

    bool operator == ( const CComparableSharedObject<T>& rhs) const
    {
        if ( (bool) m_ptr == (bool) rhs.m_ptr)
        {
            if ( m_ptr)
            {
                return *m_ptr == *rhs.m_ptr;
            }
            return true;
        }

        return false;
    }

    bool operator != ( const CComparableSharedObject<T>& rhs) const
    {
        return !(*this == rhs);
    }

    T* operator -> ()
    {
        return &(*m_ptr);
    }

    const T* operator -> () const
    {
        return &(*m_ptr);
    }

    boost::shared_ptr<T> get()
    {
        return m_ptr;
    }

    boost::shared_ptr<const T> get() const
    {
        return m_ptr;
    }

    operator boost::shared_ptr<T>()
    {
        return m_ptr;
    }

    operator boost::shared_ptr<const T>() const
    {
        return m_ptr;
    }

    operator bool() const
    {
        return (bool)m_ptr;
    }

    const T& operator*() const
    {
        return *m_ptr;
    }
private:
    boost::shared_ptr<T> m_ptr;
};

#endif /* INCLUDED_CCOMPARABLESHAREDOBJECT_H_4365503 */
