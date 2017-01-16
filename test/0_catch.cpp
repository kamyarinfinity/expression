#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <string>

#include "calculate/info.h"
#include "calculate/binding.h"


TEST_CASE("Metadata", "[metadata]") {

    SECTION("Set metadata") {
        char output[4096];

        calculate_lib.version(output);
        CHECK(output == std::string("1.2.0.dev0"));
        calculate_lib.author(output);
        CHECK(output == std::string("Alberto Lorenzo"));
        calculate_lib.date(output);
        CHECK(output == std::string("2017/01/16"));
    }

}
