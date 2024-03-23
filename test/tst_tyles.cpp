#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>

#include "styles/elements/colors.h"


int main(int argc, char** argv) {
    return Catch::Session().run(argc, argv);
}

TEST_CASE("String To Color", "[fancy]") {

    REQUIRE(Style::strToColor(Style::colorToStr(Style::Color::WHITE)) == Style::Color::WHITE);

}
