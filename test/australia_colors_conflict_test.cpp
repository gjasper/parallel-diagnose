#include <catch2/catch.hpp>
#include <diagnose.hpp>

ColorModelBuilder setup();
void assertSolution(std::string);

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

TEST_CASE(){
    ColorModelBuilder builder = setup();
    builder.withReq("wa", 0);
    builder.withReq("nt", 0);
    REQUIRE (builder.findConflict() == " | wa != nt | wa == 0 | nt == 0 | ");
}

TEST_CASE(){
    ColorModelBuilder builder = setup();
    builder.withReq("wa", 0);
    builder.withReq("nt", 0);
    builder.withReq("q", 2);
    builder.withReq("nsw", 2);
    builder.withReq("t", 0);
    assertSolution(builder.findDiagnose()); 
}

void assertSolution(std::string solution){
    bool solves1stConflict = find({"| wa != nt |", "| wa == 0 |", "| nt == 0 |"}, solution);
    bool solves2ndConflict = find({"| q != nsw |", "| q == 2 |", "| nsw == 2 |"}, solution);
    REQUIRE (solves1stConflict);
    REQUIRE (solves2ndConflict);
}

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
    builder.withNeighbours("wa", "sa"); 
    builder.withNeighbours("nt", "sa"); 
    builder.withNeighbours("nt", "q"); 
    builder.withNeighbours("q", "sa"); 
    builder.withNeighbours("wa", "nt");
    builder.withNeighbours("q", "nsw"); 
    builder.withNeighbours("nsw", "sa"); 
    builder.withNeighbours("nsw", "v"); 
    builder.withNeighbours("v", "sa");
    return builder;
}