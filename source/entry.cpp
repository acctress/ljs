#include "ljs/error.hpp"

#include <iostream>
#include <print>
#include <ljs/lexer/lexer.hpp>

///@brief The entry point of the application.
std::int32_t main( )
{
    std::string input { R"('string)" };
    ljs::lexer::Lexer lexer { std::move( input ) };

    try
    {
        for ( const auto& tok : lexer.tokenize_all(  ) )
            std::println("{}", tok.fmt(  ) );
    } catch ( const ljs::error::LJSError& e )
    {
        std::println("{}", e.what( ));
    } catch ( const std::exception& e )
    {
        std::cerr << e.what( ) << std::endl;
    }

    return EXIT_SUCCESS;
}