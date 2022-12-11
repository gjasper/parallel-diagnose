#include <catch2/catch.hpp>
#include <chrono>
#include <diagnose.hpp>

ColorModelBuilder setup();
void assertSolution(std::string);

TEST_CASE(){
    ColorModelBuilder builder = setup();

    builder.withReq("TX", 0);
    builder.withReq("LA", 0);

    builder.withReq("NY", 1);
    builder.withReq("PA", 1);

    builder.withReq("VA", 2);
    builder.withReq("NC", 2);
    builder.withReq("SC", 2);

    builder.withReq("WA", 3);
    builder.withReq("OR", 3);

    builder.withReq("CA", 0);
    builder.withReq("NV", 0);

    // builder.withReq("MT", 1);
    // builder.withReq("ND", 1);

    std::cout << "Running sequentially..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::list<std::string> solutions = builder.findDiagnoses();
    auto stop = std::chrono::high_resolution_clock::now();
    int sequentialSolutionsQtt = solutions.size();

    std::cout   << "Sequential run is finished. Elapsed time: " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                << " milliseconds"
                << std::endl;

    std::cout << "Running in parallel..." << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    solutions = builder.findDiagnoses(true);
    stop = std::chrono::high_resolution_clock::now();
    int parallelSolutionsQtt = solutions.size();

    std::cout   << "Parallel run is finished.   Elapsed time: " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                << " milliseconds"
                << std::endl;

    REQUIRE (sequentialSolutionsQtt == parallelSolutionsQtt);
}

ColorModelBuilder setup() {
    ColorModelBuilder builder;
    builder.withVars({
         "AL","AK","AZ","AR","CA","CO","CT","DE"
        ,"DC","FL","GA","HI","ID","IL","IN","IA"
        ,"KS","KY","LA","ME","MD","MA","MI","MN"
        ,"MS","MO","MT","NE","NV","NH","NJ","NM"
        ,"NY","NC","ND","OH","OK","OR","PA","RI"
        ,"SC","SD","TN","TX","UT","VT","VA","WA"
        ,"WV","WI","WY"});
    builder.withColorQtt(4);
    builder.withNeighbours("AK","WA");
    builder.withNeighbours("AL","FL");
    builder.withNeighbours("AL","GA");
    builder.withNeighbours("AL","MS");
    builder.withNeighbours("AL","TN");
    builder.withNeighbours("AR","LA");
    builder.withNeighbours("AR","MO");
    builder.withNeighbours("AR","MS");
    builder.withNeighbours("AR","OK");
    builder.withNeighbours("AR","TN");
    builder.withNeighbours("AR","TX");
    builder.withNeighbours("AZ","CA");
    builder.withNeighbours("AZ","CO");
    builder.withNeighbours("AZ","NM");
    builder.withNeighbours("AZ","NV");
    builder.withNeighbours("AZ","UT");
    builder.withNeighbours("CA","HI");
    builder.withNeighbours("CA","NV");
    builder.withNeighbours("CA","OR");
    builder.withNeighbours("CO","KS");
    builder.withNeighbours("CO","NE");
    builder.withNeighbours("CO","NM");
    builder.withNeighbours("CO","OK");
    builder.withNeighbours("CO","UT");
    builder.withNeighbours("CO","WY");
    builder.withNeighbours("CT","MA");
    builder.withNeighbours("CT","NY");
    builder.withNeighbours("CT","RI");
    builder.withNeighbours("DC","MD");
    builder.withNeighbours("DC","VA");
    builder.withNeighbours("DE","MD");
    builder.withNeighbours("DE","NJ");
    builder.withNeighbours("DE","PA");
    builder.withNeighbours("FL","GA");
    builder.withNeighbours("GA","NC");
    builder.withNeighbours("GA","SC");
    builder.withNeighbours("GA","TN");
    builder.withNeighbours("IA","IL");
    builder.withNeighbours("IA","MN");
    builder.withNeighbours("IA","MO");
    builder.withNeighbours("IA","NE");
    builder.withNeighbours("IA","SD");
    builder.withNeighbours("IA","WI");
    builder.withNeighbours("ID","MT");
    builder.withNeighbours("ID","NV");
    builder.withNeighbours("ID","OR");
    builder.withNeighbours("ID","UT");
    builder.withNeighbours("ID","WA");
    builder.withNeighbours("ID","WY");
    builder.withNeighbours("IL","IN");
    builder.withNeighbours("IL","KY");
    builder.withNeighbours("IL","MO");
    builder.withNeighbours("IL","WI");
    builder.withNeighbours("IN","KY");
    builder.withNeighbours("IN","MI");
    builder.withNeighbours("IN","OH");
    builder.withNeighbours("KS","MO");
    builder.withNeighbours("KS","NE");
    builder.withNeighbours("KS","OK");
    builder.withNeighbours("KY","MO");
    builder.withNeighbours("KY","OH");
    builder.withNeighbours("KY","TN");
    builder.withNeighbours("KY","VA");
    builder.withNeighbours("KY","WV");
    builder.withNeighbours("LA","MS");
    builder.withNeighbours("LA","TX");
    builder.withNeighbours("MA","NH");
    builder.withNeighbours("MA","NY");
    builder.withNeighbours("MA","RI");
    builder.withNeighbours("MA","VT");
    builder.withNeighbours("MD","PA");
    builder.withNeighbours("MD","VA");
    builder.withNeighbours("MD","WV");
    builder.withNeighbours("ME","NH");
    builder.withNeighbours("MI","OH");
    builder.withNeighbours("MI","WI");
    builder.withNeighbours("MN","ND");
    builder.withNeighbours("MN","SD");
    builder.withNeighbours("MN","WI");
    builder.withNeighbours("MO","NE");
    builder.withNeighbours("MO","OK");
    builder.withNeighbours("MO","TN");
    builder.withNeighbours("MS","TN");
    builder.withNeighbours("MT","ND");
    builder.withNeighbours("MT","SD");
    builder.withNeighbours("MT","WY");
    builder.withNeighbours("NC","SC");
    builder.withNeighbours("NC","TN");
    builder.withNeighbours("NC","VA");
    builder.withNeighbours("ND","SD");
    builder.withNeighbours("NE","SD");
    builder.withNeighbours("NE","WY");
    builder.withNeighbours("NH","VT");
    builder.withNeighbours("NJ","NY");
    builder.withNeighbours("NJ","PA");
    builder.withNeighbours("NM","OK");
    builder.withNeighbours("NM","TX");
    builder.withNeighbours("NM","UT");
    builder.withNeighbours("NV","OR");
    builder.withNeighbours("NV","UT");
    builder.withNeighbours("NY","PA");
    builder.withNeighbours("NY","VT");
    builder.withNeighbours("OH","PA");
    builder.withNeighbours("OH","WV");
    builder.withNeighbours("OK","TX");
    builder.withNeighbours("OR","WA");
    builder.withNeighbours("PA","WV");
    builder.withNeighbours("SD","WY");
    builder.withNeighbours("TN","VA");
    builder.withNeighbours("UT","WY");
    builder.withNeighbours("VA","WV");
    return builder;
}