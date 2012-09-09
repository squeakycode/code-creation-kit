//   Copyright (C) 2011-2012 Andreas Gau
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

#ifndef INCLUDED_ICONSTRAINT_H_4452783
#define INCLUDED_ICONSTRAINT_H_4452783

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

///used to check a constraint for a single table entry 
template <typename StringT>
class IConstraint
{
public:
    virtual bool matchesConstraint( const StringT& text) const = 0;
    virtual bool forAll() const = 0;
    virtual bool flush() const = 0;

    virtual bool operator==( const IConstraint& conversion) const = 0;

    virtual ~IConstraint() 
    {
    };
};


#endif /* INCLUDED_ICONSTRAINT_H_4452783 */
