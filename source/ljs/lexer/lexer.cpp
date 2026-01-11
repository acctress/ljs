#include <ljs/lexer/lexer.hpp>

#include "ljs/error.hpp"
#include <string_view>

using namespace std::string_view_literals;

std::vector< token > ljs::lexer::Lexer::tokenize_all( )
{
    std::vector< token > tokens;

    do
    {
        auto tok = next(  );
        if ( !tok.has_value(  ) )
            throw error::LJSErrorBuilder(m_source)
                .at( m_line, m_column )
                .span( m_column, m_position - m_column )
                .of_type( "lexer error" )
                .with_explanation( "Unable to process token" )
                .with_hint( "Do you have an illegal token in your source code?" )
                .build(  );

        tokens.push_back( std::move( tok.value(  ) ) );
    } while ( not_at_end(  ) );

    return tokens;
}

std::optional< token > ljs::lexer::Lexer::next( )
{
    while ( not_at_end(  ) )
    {
        if ( const auto current = peek( ).value( );
            current == ' ' || current == '\t' || current == '\n' || current == '\r' )
            advance(  );
        else break;
    }

    if ( !not_at_end(  ) )
        return token( token_type::T_EOF, "", m_line, m_column, m_column );

    const auto current = peek( ).value(  );

    if ( std::isalpha( current ) || current == '_' )
    {
        const std::size_t start = m_position;

        do
        {
            advance( );
        } while ( not_at_end( ) && ( std::isalnum( static_cast< unsigned char >( m_source[ m_position ] ) ) || m_source[ m_position ] == '_' ) );

        const std::string_view value { &m_source[ start ], m_position - start };
        return token { token_type::IDENTIFIER, std::string( value ), m_line, start, m_position };
    }

    if ( std::isdigit( current ) )
    {
        const std::size_t start = m_position;
        bool is_float { false };

        do
        {
            if ( m_source[ m_position ] == '.')
            {
                if ( is_float )
                    throw error::LJSErrorBuilder(m_source)
                        .at( m_line, m_column + 1 )
                        .span( m_column, m_position - m_column )
                        .of_type( "syntax error" )
                        .with_explanation( "Cannot have more than one decimal point in a float." )
                        .with_hint( "Try removing the extra decimal point in the literal." )
                        .in_file( "test.js" )
                        .build(  );

                is_float = true;
                advance(  );
            }

            advance(  );
        } while ( not_at_end( ) && ( std::isdigit( static_cast< unsigned char >( m_source[ m_position ] ) ) || m_source[ m_position ] == '.' ) );

        const std::string_view value { &m_source[ start ], m_position - start };
        return token { token_type::NUMBER, std::string( value ), m_line, start, m_position };
    }

    if ( current == '"' || current == '\'' )
    {
        const std::size_t start = m_position;
        const char opening_char = current;
        advance(  );

        do
        {
            if ( m_source[ m_position ] == opening_char )
            {
                advance(  );
                const std::string_view value( &m_source[start + 1], &m_source[m_position - 1] - &m_source[start + 1] );
                return token { token_type::STRING, std::string( value ), m_line, start, m_position };
            }

            advance( );
        } while ( not_at_end(  ) );

        throw error::LJSErrorBuilder(m_source)
              .at(m_line, m_column)
              .span(start, m_position - start)
              .of_type("syntax error")
              .with_explanation(
                  std::format("Unterminated string literal (missing {}).", opening_char))
              .with_hint( std::format("Add {} to close the string.", opening_char) )
              .build();
    }

    return std::nullopt;
}

void ljs::lexer::Lexer::advance( )
{
    if ( not_at_end(  ) )
    {
        if ( m_source[ m_position ] == '\n' )
        {
            m_line++;
            m_column = 1;
        } else
        {
            m_column++;
        }

        ++m_position;
    }
}

std::optional< char > ljs::lexer::Lexer::peek( ) const
{
    if ( not_at_end(  ) )
        return m_source.at( m_position );

    return std::nullopt;
}

std::optional< char > ljs::lexer::Lexer::peek_next( ) const
{
    if ( m_position + 1 < m_source.length(  ) )
        return m_source.at( m_position + 1 );

    return std::nullopt;
}

bool ljs::lexer::Lexer::not_at_end( ) const
{
    return m_position < m_source.length(  );
}

