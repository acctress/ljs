#pragma once

#include "ljs/lexer/token.hpp"
#include "ljs/error.hpp"
#include <optional>
#include <string>
#include <vector>

using namespace ljs::tok;

namespace ljs::lexer
{
    class Lexer
    {
    public:
        explicit Lexer( std::string m_source ) : m_source( std::move( m_source ) ) { }

        ///@brief Process all tokens into a vector
        std::vector< token > tokenize_all( );

        ///@brief Process the next token in the source code.
        [[nodiscard]]
        std::optional< token > next( );

        [[nodiscard]] bool had_error( ) const
        {
            return m_had_error;
        }

        [[nodiscard]] const std::vector< error::LJSError > &get_errors( ) const
        {
            return m_errors;
        }

    private:
        std::string m_source;
        std::size_t m_position { };
        std::size_t m_line { 1 };
        std::size_t m_column { };
        bool m_had_error { };
        std::vector< error::LJSError > m_errors;

        ///@brief Advance the reading position to the next character.
        void advance( );

        ///@brief Peek at the current character without advancing
        [[nodiscard]] std::optional< char > peek( ) const;

        ///@brief Peek ahead once without advancing
        [[nodiscard]] std::optional< char > peek_next( ) const;

        ///@brief Returns true if the reading position is not at the end of the source code.
        [[nodiscard]] bool not_at_end( ) const;
    };
}