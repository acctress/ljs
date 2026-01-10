#pragma once

#include <format>
#include <optional>
#include <string>
#include <vector>

namespace ljs::lexer
{
    ///@brief An enum of token types.
    enum class token_type : std::uint8_t
    {
        IDENTIFIER,
        NUMBER,
        T_EOF,
    };

    ///@brief A struct which represents a token.
    struct token
    {
        token_type type;
        std::string lexeme;
        std::size_t line;
        std::size_t column;

        token(
            const token_type type,
            std::string lexeme,
            const std::size_t line,
            const std::size_t column
        ) :
            type( type ),
            lexeme( std::move( lexeme ) ),
            line( line ),
            column( column )
        {
        }

        ///@brief Format the token into a readable format.
        [[nodiscard]]
        std::string fmt( ) const
        {
            return std::format( "token {} [{}:{}]", lexeme, line, column );
        }
    };

    class Lexer
    {
    public:
        explicit Lexer( std::string m_source ) : m_source( std::move( m_source ) ) { }

        ///@brief Process all tokens into a vector
        std::vector<token> tokenize_all( );

        ///@brief Process the next token in the source code.
        [[nodiscard]]
        token next( );
    private:
        std::string m_source;
        std::size_t m_position { };
        std::size_t m_line { 1 };
        std::size_t m_column { };

        ///@brief Advance the reading position to the next character.
        void advance( );

        ///@brief Peek at the current character without advancing
        std::optional< char > peek( ) const;

        ///@brief Peek ahead once without advancing
        std::optional< char > peek_next( ) const;

        ///@brief Returns true if the reading position is not at the end of the source code.
        bool not_at_end( ) const;
    };
}