#include <ljs/lexer/lexer.hpp>

std::vector< ljs::lexer::token > ljs::lexer::Lexer::tokenize_all( )
{
    std::vector< token > tokens;

    do
    {
        tokens.push_back( next( ) );
    } while ( not_at_end(  ) );

    return tokens;
}

ljs::lexer::token ljs::lexer::Lexer::next( )
{
    if ( !peek(  ).has_value(  ) )
        throw std::runtime_error(
            std::format( "Unable to peek current character in source code at position {}.", m_position )
        );

    const auto current = peek(  ).value(  );
    switch ( current )
    {
        default :
        {
            if ( std::isalpha( current ) || current == '_' )
            {
                const std::size_t start = m_position;

                do
                {
                    advance( );
                } while ( not_at_end( ) && ( std::isalnum( static_cast< unsigned char >( m_source[ m_position ] ) ) || m_source[ m_position ] == '_' ) );

                const std::string_view value { &m_source[ start ], m_position - start };
                return token { token_type::IDENTIFIER, std::string( value ), m_line, start };
            }

            return token( token_type::T_EOF, "", m_line, m_column );
        }
    }
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
