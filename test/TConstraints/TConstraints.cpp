// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "Constraints.h"

using namespace code_creation_kit;

template <typename StringT, typename MatchesConstraintT>
void testMatches()
{
    { //matches no ignore case
        MatchesConstraintT matches( "b");

        CHECK( !matches.matchesConstraint( "a"));
        CHECK( matches.matchesConstraint( "b"));
    }

    { //matches ignore case
        MatchesConstraintT matches( "b");
        matches.ignoreCase(true);

        CHECK( !matches.matchesConstraint( "a"));
        CHECK( matches.matchesConstraint( "B"));
    }

    { //test compare
        MatchesConstraintT matchesA( "b");
        MatchesConstraintT matchesB( "a");
        MatchesConstraintT matchesC( "b");

        IConstraint<StringT>* cA = &matchesA;
        IConstraint<StringT>* cB = &matchesB;
        IConstraint<StringT>* cC = &matchesC;

        CHECK( !(*cA == *cB));
        CHECK( *cA == *cA);
        CHECK( *cA == *cC);
    }

}

TEST_CASE( "TConstraints", "[TConstraints]")
{
    testMatches<std::string, CMatchesConstraint<std::string> >();
    testMatches<std::string, CRegexMatchesConstraint<std::string> >();

    CAnyConstraint<std::string> any;
    CHECK( any.matchesConstraint( "text"));
}
