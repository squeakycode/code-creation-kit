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

#ifndef INCLUDED_CINLINETEMPLATEPARAMETERS_H_3727686
#define INCLUDED_CINLINETEMPLATEPARAMETERS_H_3727686

#if !defined (COMPILER_LACKS_PRAGMA_ONCE)
#pragma once
#endif

/// paramters for inline template processing
template <typename StringT>
struct CInlineTemplateParameters
{
    CInlineTemplateParameters()
        : enabled(false)
    {
    }

    CInlineTemplateParameters( bool enable, StringT prefix, StringT postfix, StringT generatedPostfix)
        : enabled( enable)
        , inlinePrefix( prefix)
        , inlinePostfix( postfix)
        , inlineGeneratedPostfix( generatedPostfix)
    {
    }

    bool operator== ( const CInlineTemplateParameters<StringT>& rhs)
    {
        if ( enabled != rhs.enabled) return false;
        if ( inlinePrefix != rhs.inlinePrefix) return false;
        if ( inlinePostfix != rhs.inlinePostfix) return false;
        if ( inlineGeneratedPostfix != rhs.inlineGeneratedPostfix) return false;
        return true;
    }

    bool    enabled;
    StringT inlinePrefix;
    StringT inlinePostfix;
    StringT inlineGeneratedPostfix;
};

#endif /* INCLUDED_CINLINETEMPLATEPARAMETERS_H_3727686 */
