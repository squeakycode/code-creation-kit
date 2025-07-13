// Copyright (c) 2011-2025 Andreas Gau
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace code_creation_kit
{
    /// paramters for inline template processing
    template <typename StringT>
    struct CInlineTemplateParameters
    {
        CInlineTemplateParameters()
            : enabled(false)
            , inlinePad( 0)
        {
        }

        CInlineTemplateParameters( bool enable, StringT prefix, StringT postfix, StringT generatedPostfix, size_t numInlinePad)
            : enabled( enable)
            , inlinePrefix( prefix)
            , inlinePostfix( postfix)
            , inlineGeneratedPostfix( generatedPostfix)
            , inlinePad( numInlinePad)
        {
        }

        bool operator== ( const CInlineTemplateParameters<StringT>& rhs)
        {
            if ( enabled != rhs.enabled) return false;
            if ( inlinePrefix != rhs.inlinePrefix) return false;
            if ( inlinePostfix != rhs.inlinePostfix) return false;
            if ( inlineGeneratedPostfix != rhs.inlineGeneratedPostfix) return false;
            if ( inlinePad != rhs.inlinePad) return false;
            return true;
        }

        bool    enabled;
        StringT inlinePrefix;
        StringT inlinePostfix;
        StringT inlineGeneratedPostfix;
        size_t  inlinePad;
    };
}
