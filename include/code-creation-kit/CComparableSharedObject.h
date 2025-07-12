// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <memory>

namespace code_creation_kit
{
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

        CComparableSharedObject( std::shared_ptr<T> ptr)
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

        std::shared_ptr<T> get()
        {
            return m_ptr;
        }

        std::shared_ptr<const T> get() const
        {
            return m_ptr;
        }

        operator std::shared_ptr<T>()
        {
            return m_ptr;
        }

        operator std::shared_ptr<const T>() const
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
        std::shared_ptr<T> m_ptr;
    };
}
