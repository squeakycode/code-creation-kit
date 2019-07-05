//  Copyright (c) 2011-2015 Andreas Gau
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//      * Neither the name of the copyright holder nor the
//        names of contributors may be used to endorse or promote products
//        derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "Constraints.h"

using namespace code_creation_kit;

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
