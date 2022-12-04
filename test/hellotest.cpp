#include <catch2/catch.hpp>

static int zero(){
    return 0;
}

TEST_CASE(){
    REQUIRE (zero() == 0);
}