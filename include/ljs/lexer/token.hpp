#pragma once

#include <string_view>
#include <string>
#include <format>

namespace ljs::tok
{
    ///@brief An enum of token types.
    enum class token_type : std::uint8_t
    {
        IDENTIFIER,
        NUMBER,
        STRING,
        T_EOF,
    };

    static std::string_view token_type_to_string( const token_type type )
    {
        switch ( type )
        {
            case token_type::IDENTIFIER:
                return "identifier";
            case token_type::NUMBER:
                return "number";
            case token_type::STRING:
                return "string";
            case token_type::T_EOF:
                return "eof";
            default:
                return "";
        }
    }

    ///@brief A struct which represents a token.
    struct token
    {
        token_type type;
        std::string lexeme;
        std::size_t line;
        std::size_t start;
        std::size_t end;

        token(
            const token_type type,
            std::string lexeme,
            const std::size_t line,
            const std::size_t start,
            const std::size_t end
        ) :
            type( type ),
            lexeme( std::move( lexeme ) ),
            line( line ),
            start( start ),
            end( end )
        {
        }

        ///@brief Format the token into a readable format.
        [[nodiscard]]
        std::string fmt( ) const
        {
            return std::format( "token<{}> \"{}\" [{}:{}->{}]", token_type_to_string( type ), lexeme, line, start, end );
        }
    };
}