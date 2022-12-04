#include "diagnose.hpp"

Constraint::Constraint(std::string name, int value){
    type = 0;
    varName = name;
    rhsLiteralValue = value;
}

Constraint::Constraint(std::string a, std::string b){
    type = 1;
    varName = a;
    rhsVarname = b;
}

ColorModelBuilder ColorModelBuilder::withNeighbours(std::string a, std::string b) {
    constraints.push_back(Constraint(a, b));
    return *this;
}

ColorModelBuilder ColorModelBuilder::withReq(std::string var, int color) {
    constraints.push_back(Constraint(var, color));
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

ColorModelBuilder::ColorModelBuilder() {}

ColorModel::ColorModel(
        std::list<std::string> vs
    ,   std::list<Constraint> cs
    ,   int colorQtt) {

    vars = Gecode::IntVarArray(*this, vs.size(), 0, colorQtt - 1);

    std::vector<std::string> variables{ std::begin(vs), std::end(vs) };
    std::map<std::string, Gecode::IntVar> varMap;
    for(int i = 0; i < vs.size(); i++){
        varMap[variables.at(i)] = vars[i];
    }

    std::vector<Constraint> constraints{ std::begin(cs), std::end(cs) };
    for(int i = 0; i < constraints.size(); i++){
        Constraint c = constraints.at(i);
        if(c.type == 1){
            rel(*this, varMap.at(c.varName), Gecode::IRT_NQ, varMap.at(c.rhsVarname));
        } else{
            rel(*this, varMap.at(c.varName), Gecode::IRT_EQ, c.rhsLiteralValue);
        }
    }

    branch(*this, vars, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_MIN());
}

ColorModel* ColorModelBuilder::build(){
    return new ColorModel(vars, constraints, colorQtt);
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

std::list<int> quickXplain(){

}

std::list<int> qx(){

}