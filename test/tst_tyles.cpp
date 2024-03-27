#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>

#include <iostream>

#include "styles/style.h"
#include "styles/elements/styletypes.h"
#include "styles/elements/styles.h"
#include "styles/elements/functions.h"


int main(int argc, char** argv) {
    return Catch::Session().run(argc, argv);
}

TEST_CASE("Style To String to Style", "[fancy]") {
    using namespace Styles;

    auto styleStr{"StylePtr<AudioFlicker<White, Blue>>()"};
    auto style{std::make_shared<Wrapper::StylePtr>(std::vector<Value>{std::make_shared<Style::AudioFlicker>(std::vector<Value>{Color::WHITE, Color::BLUE})})};
    REQUIRE(asString(parseString(styleStr)).value() == asString(style).value());
    std::cout << "StyleStr: " << styleStr << " asString(style): " << asString(style).value() << std::endl;

}
