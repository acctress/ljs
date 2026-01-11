#pragma once

#include <exception>
#include <format>
#include <iomanip>
#include <print>
#include <sstream>
#include <string>
#include <utility>

namespace ljs::error
{
    namespace colours
    {
        inline constexpr auto reset = "\x1b[0m";
        inline constexpr auto gray = "\x1b[38;5;240m";
        inline constexpr auto pastel_pink = "\x1b[38;5;219m";
        inline constexpr auto pastel_red = "\x1b[38;5;204m";
        inline constexpr auto pastel_blue = "\x1b[38;5;39m";
        inline constexpr auto pastel_purple = "\x1b[38;5;111m";
        inline constexpr auto pastel_yellow = "\x1b[38;5;229m";
        inline constexpr auto pastel_orange = "\x1b[38;5;221m";
    }

    /* A simple span structure to represent a chunk of text in a line */
    struct Span
    {
        std::size_t start;
        std::size_t end;
    };

    /* The context to which the error appoints to in the source code. */
    struct ContextLocation
    {
        std::string file;   /* Name of the file which the error is produced from */
        std::size_t line;   /* Line of the context */
        std::size_t column; /* Start column of the context */
        Span span;          /* The span of code which caused the error */

        ContextLocation(
            std::string file,
            const std::size_t line,
            const std::size_t column,
            const Span span
        )
            : file( std::move( file ) )
            , line( line )
            , column( column )
            , span( span )
        {
        }
    };

    class LJSError : std::exception {
    public:
        LJSError(
            std::string_view source,
            std::string explanation,
            ContextLocation&& context,
            std::string type,
            std::string hint
        ) :
            m_source( source ),
            m_explanation( std::move( explanation ) ),
            m_context( std::move( context ) ),
            m_type( std::move( type ) ),
            m_hint( std::move( hint ) )
        {
            message = fmt(  );
        }

        [[nodiscard]] const char* what( ) const override
        {
            return message.c_str(  );
        }

    private:
        std::string message;

        std::string_view m_source;
        std::string m_explanation;
        ContextLocation m_context;
        std::string m_type;
        std::string m_hint;

        [[nodiscard]] std::string fmt( ) const
        {
            std::ostringstream m_formatted;

            m_formatted << "-- " << colours::pastel_red << m_type << colours::reset
                << " " << std::string(10, '-' ) << "> " << colours::pastel_blue
                << std::format("[{}:{}:{}]", m_context.file, m_context.line, m_context.column)
                << colours::reset << std::endl;

            m_formatted << std::endl;

            m_formatted << m_explanation << std::endl;
            m_formatted << std::endl;

            /* get the line in the source */
            std::size_t current_line_num { 1 };
            std::string current_line;
            std::istringstream m_ss ( m_source.data(  ) );

            while ( std::getline( m_ss, current_line ) )
            {
                if ( current_line_num == m_context.line )
                {
                    m_formatted << colours::pastel_purple << m_context.line << colours::reset << colours::gray << " | " << colours::pastel_red << current_line << colours::reset << std::endl;
                    m_formatted << std::string( std::to_string( m_context.line ).length(  ), ' ' ) << colours::gray << " | " << colours::reset;

                    m_formatted << colours::pastel_yellow;
                    for ( auto idx { 1uz }; idx < m_context.column; ++idx )
                        m_formatted << '~';

                    m_formatted << '^';
                    m_formatted << colours::reset << std::endl;
                }

                current_line_num++;
            }

            if ( !m_hint.empty(  ) )
                m_formatted << colours::pastel_orange << "\nHint: " << colours::reset << m_hint << std::endl;

            return m_formatted.str( );
        }
    };

    class LJSErrorBuilder
    {
    public:
        explicit LJSErrorBuilder( std::string_view source) : m_source( source ) { }

        LJSErrorBuilder& at( const std::size_t line, const std::size_t column )
        {
            m_line = line;
            m_column = column;
            return *this;
        }

        LJSErrorBuilder& span( const std::size_t start, const std::size_t length )
        {
            m_span_start = start;
            m_span_length = length;
            return *this;
        }

        LJSErrorBuilder& with_explanation( std::string explanation )
        {
            m_explanation = std::move( explanation );
            return *this;
        }

        LJSErrorBuilder& of_type( std::string type )
        {
            m_type = std::move( type );
            return *this;
        }

        LJSErrorBuilder& with_hint( std::string hint )
        {
            m_hint = std::move( hint );
            return *this;
        }

        LJSErrorBuilder& in_file( std::string file )
        {
            m_file = std::move( file );
            return *this;
        }

        [[nodiscard]] LJSError build( ) const
        {
            ContextLocation ctx {
                m_file,
                m_line,
                m_column,
                Span { m_span_start, m_span_length }
            };

            return LJSError {
                m_source,
                m_explanation,
                std::move( ctx ),
                m_type,
                m_hint
            };
        }

        [[noreturn]] void throw_err( ) const
        {
            throw build( );
        }

    private:
        std::string_view m_source;
        std::string m_explanation;
        std::size_t m_line = 1;
        std::size_t m_column = 1;
        std::size_t m_span_start = 0;
        std::size_t m_span_length = 1;
        std::string m_type = "error";
        std::string m_hint;
        std::string m_file = "unknown";
    };
}