#include <catch2/catch.hpp>
#include <diagnose.hpp>

const int KB = 0;
const int REQ = 1;
const int KB_AND_REQ = 2;

ColorModelBuilder setup();

TEST_CASE(){
    ColorModelBuilder builder = setup();
    builder.withReq("wa", 0);
    builder.withReq("nt", 1);
    builder.withReq("t", 1);
    REQUIRE (builder.propagate() == "COMPLETE");
}

TEST_CASE(){
    ColorModelBuilder builder = setup();
    builder.withReq("wa", 0);
    builder.withReq("nt", 0);
    REQUIRE (builder.propagate() == "CONTRADICTION");
}

// TEST_CASE(){
//     ColorModelBuilder builder = setup();
//     builder.withReq("wa", 0);
//     builder.withReq("nt", 0);
//     REQUIRE (builder.findConflicts(REQ) == "wa = 0, nt = 0");
// }

// TEST_CASE(){
//     ColorModelBuilder builder = setup();
//     builder.withReq("wa", 0);
//     builder.withReq("nt", 0);
//     REQUIRE (builder.findConflicts(KB) == "");
// }

// TEST_CASE(){
//     ColorModelBuilder builder = setup();
//     builder.withReq("wa", 0);
//     builder.withReq("nt", 0);
//     REQUIRE (builder.findConflicts(KB_AND_REQ) == "wa = 0, nt = 0, wa != nt");
// }

ColorModelBuilder setup() {
    ColorModelBuilder builder;
    builder.withVar("wa");
    builder.withVar("nt");
    builder.withVar("sa");
    builder.withVar("nsw");
    builder.withVar("q");
    builder.withVar("t");
    builder.withVar("v");
    builder.withColorQtt(3);
    builder.withNeighbours("wa", "nt");
    builder.withNeighbours("wa", "sa"); 
    builder.withNeighbours("nt", "sa"); 
    builder.withNeighbours("nt", "q"); 
    builder.withNeighbours("q", "sa"); 
    builder.withNeighbours("q", "nsw"); 
    builder.withNeighbours("nsw", "sa"); 
    builder.withNeighbours("nsw", "v"); 
    builder.withNeighbours("v", "sa");
    return builder;
}