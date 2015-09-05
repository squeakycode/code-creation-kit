//   Copyright (C) 2011-2015 Andreas Gau
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

#ifndef INCLUDED_ICONVERSION_H_4158703
#define INCLUDED_ICONVERSION_H_4158703

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

#include <vector>

///used to modify an inserted table entry
template <typename StringT>
class IConversion
{
public:
    typedef std::vector<StringT> StringListT;

    virtual void modify( StringListT& text) const = 0;

    virtual bool operator==( const IConversion& conversion) const = 0;

    virtual ~IConversion() 
    {
    };
};

#endif /* INCLUDED_ICONVERSION_H_4158703 */
