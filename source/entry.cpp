#include <iostream>
#include <print>
#include <ljs/lexer/lexer.hpp>

///@brief The entry point of the application.
std::int32_t main( )
{
    std::string input;
    std::print(">>> ");
    std::getline(std::cin, input);

    ljs::lexer::Lexer lexer { std::move( input ) };

    const auto token = lexer.next(  );

    std::println("next token: {}", token.fmt(  ));

    return EXIT_SUCCESS;
}