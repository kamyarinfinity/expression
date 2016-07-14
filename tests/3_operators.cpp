#include <cstdlib>
#include <cmath>

#include "catch.hpp"
#include "calculate.h"


TEST_CASE("Builtin operators", "[operators]") {

    SECTION("addition") {
        auto x = static_cast<double>(rand() % 1000 + 1);
        auto y = static_cast<double>(rand() % 1000 + 1);
        auto add = calculate::Calculate("x + y", "x, y");

        CHECK(add(x, 0) == Approx(x).epsilon(1e-12));
        CHECK(add(x, 1) == Approx(x + 1).epsilon(1e-12));
        CHECK(add(x, y) == Approx(x + y).epsilon(1e-12));
    }

    SECTION("subtraction") {
        auto x = static_cast<double>(rand() % 1000 + 1);
        auto y = static_cast<double>(rand() % 1000 + 1);
        auto sub = calculate::Calculate("x - y", "x, y");

        CHECK(sub(x, 0) == Approx(x).epsilon(1e-12));
        CHECK(sub(x, 1) == Approx(x - 1).epsilon(1e-12));
        CHECK(sub(x, y) == Approx(x - y).epsilon(1e-12));
    }

    SECTION("multiplication") {
        auto x = static_cast<double>(rand() % 1000 + 1);
        auto y = static_cast<double>(rand() % 1000 + 1);
        auto mul = calculate::Calculate("x * y", "x, y");

        CHECK(mul(x, 0) == Approx(0).epsilon(1e-12));
        CHECK(mul(x, 1) == Approx(x).epsilon(1e-12));
        CHECK(mul(x, y) == Approx(x * y).epsilon(1e-12));
    }

    SECTION("division") {
        auto x = static_cast<double>(rand() % 1000 + 1);
        auto y = static_cast<double>(rand() % 1000 + 1);
        auto mul = calculate::Calculate("x / y", "x, y");

        CHECK(std::isinf(mul(x, 0)));
        CHECK(mul(x, 1) == Approx(x).epsilon(1e-12));
        CHECK(mul(x, y) == Approx(x / y).epsilon(1e-12));
    }

    SECTION("power") {
        auto x = static_cast<double>(rand() % 5 + 1);
        auto y = static_cast<double>(rand() % 5 + 1);
        auto pow = calculate::Calculate("x ^ y", "x, y");
        auto pow2 = calculate::Calculate("x ** y", "x, y");

        CHECK(pow(x, 0) == Approx(1).epsilon(1e-12));
        CHECK(pow2(x, 0) == Approx(1).epsilon(1e-12));
        CHECK(pow(x, 1) == Approx(x).epsilon(1e-12));
        CHECK(pow2(x, 1) == Approx(x).epsilon(1e-12));
        CHECK(pow(x, y) == Approx(pow(x, y)).epsilon(1e-12));
        CHECK(pow2(x, y) == Approx(pow(x, y)).epsilon(1e-12));
        CHECK(std::isnan(pow(-1, x / y)));
        CHECK(std::isnan(pow2(-1, x / y)));
    }

}
