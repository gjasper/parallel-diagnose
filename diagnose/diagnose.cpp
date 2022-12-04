#include "diagnose.hpp"

ColorModelBuilder ColorModelBuilder::withReq(std::string var, int color) {
    reqs.push_back(make_tuple(var, color));
    return *this;
}

ColorModelBuilder ColorModelBuilder::withVar(std::string var) {
    vars.push_back(var);
    return *this;
}
                  
ColorModelBuilder ColorModelBuilder::withColorQtt(int qtt) {
    colorQtt = qtt;
    return *this;
}

ColorModelBuilder ColorModelBuilder::withNeighbours(std::string a, std::string b) {
    neighbours.push_back(make_tuple(a, b));
    return *this;
}

ColorModelBuilder::ColorModelBuilder() {}

ColorModel::ColorModel(
        std::list<std::string> vs
    ,   std::list<std::tuple<std::string, int>> rs
    ,   std::list<std::tuple<std::string, std::string>> ns
    ,   int colorQtt) {

    vars = Gecode::IntVarArray(*this, vs.size(), 0, colorQtt - 1);

    std::vector<std::string> variables{ std::begin(vs), std::end(vs) };
    std::map<std::string, Gecode::IntVar> varMap;
    for(int i = 0; i < vs.size(); i++){
        varMap[variables.at(i)] = vars[i];
    }

    std::cout << std::endl;
    std::vector<std::tuple<std::string, std::string>> neighbours{ std::begin(ns), std::end(ns) };
    for(int i = 0; i < neighbours.size(); i++){
        rel(*this, varMap.at(std::get<0>(neighbours.at(i))), Gecode::IRT_NQ, varMap.at(std::get<1>(neighbours.at(i))));
        std::cout << std::get<0>(neighbours.at(i)) << " nq " << std::get<1>(neighbours.at(i)) << std::endl;
    }

    std::cout << std::endl;
    std::vector<std::tuple<std::string, int>> reqs{ std::begin(rs), std::end(rs) };
    for(int i = 0; i < reqs.size(); i++){
        rel(*this, varMap.at(std::get<0>(reqs.at(i))), Gecode::IRT_EQ, std::get<1>(reqs.at(i)));
        std::cout << std::get<0>(reqs.at(i)) << " eq " << std::get<1>(reqs.at(i)) << std::endl;
    }
    branch(*this, vars, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_MIN());
}

ColorModel* ColorModelBuilder::build(){
    return new ColorModel(vars, reqs, neighbours, colorQtt);
}

std::string ColorModelBuilder::propagate(){
    ColorModel* ac = build();
    if(ac -> status() == Gecode::SS_FAILED){
        return "CONTRADICTION";
    } else if(ac -> status() == Gecode::SS_BRANCH){
        return "BRANCH";
    } else if(ac -> status() == Gecode::SS_SOLVED){
        return "COMPLETE";
    }
    return "INVALID";
}

std::string ColorModelBuilder::findConflicts(int mode){
    return "findConflicts return";
}