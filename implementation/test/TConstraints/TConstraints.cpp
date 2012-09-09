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

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "Constraints.h"

template <typename StringT, typename MatchesConstraintT>
void testMatches()
{
    { //matches no ignore case
        MatchesConstraintT matches( "b");

        BOOST_CHECK( !matches.matchesConstraint( "a"));
        BOOST_CHECK( matches.matchesConstraint( "b"));
    }

    { //matches ignore case
        MatchesConstraintT matches( "b");
        matches.ignoreCase(true);

        BOOST_CHECK( !matches.matchesConstraint( "a"));
        BOOST_CHECK( matches.matchesConstraint( "B"));
    }

    { //test compare
        MatchesConstraintT matchesA( "b");
        MatchesConstraintT matchesB( "a");
        MatchesConstraintT matchesC( "b");

        IConstraint<StringT>* cA = &matchesA;
        IConstraint<StringT>* cB = &matchesB;
        IConstraint<StringT>* cC = &matchesC;

        BOOST_CHECK( !(*cA == *cB));
        BOOST_CHECK( *cA == *cA);
        BOOST_CHECK( *cA == *cC);
    }

}

BOOST_AUTO_TEST_CASE( TConstraints)
{
    testMatches<std::string, CMatchesConstraint<std::string> >();
    testMatches<std::string, CRegexMatchesConstraint<std::string> >();

    CAnyConstraint<std::string> any;
    BOOST_CHECK( any.matchesConstraint( "text"));
}
