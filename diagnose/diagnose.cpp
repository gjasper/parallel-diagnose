#include "diagnose.hpp"

Constraint::Constraint(std::string cname, std::string var, int value){
    type = 0;
    name = cname;
    varName = var;
    rhsLiteralValue = value;
}

Constraint::Constraint(std::string cname, std::string a, std::string b){
    type = 1;
    name = cname;
    varName = a;
    rhsVarname = b;
}

ColorModelBuilder ColorModelBuilder::withNeighbours(std::string a, std::string b) {
    constraints.push_back(Constraint("c" + constraints.size(), a, b));
    return *this;
}

ColorModelBuilder ColorModelBuilder::withReq(std::string var, int color) {
    constraints.push_back(Constraint("c" + constraints.size(), var, color));
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
    std::vector<Constraint> constrs{ std::begin(cs), std::end(cs) };
    for(int i = 0; i < constrs.size(); i++){
        Constraint c = constrs.at(i);
        if(c.type == 1){
            rel(*this, varMap.at(c.varName), Gecode::IRT_NQ, varMap.at(c.rhsVarname));
        } else{
            rel(*this, varMap.at(c.varName), Gecode::IRT_EQ, c.rhsLiteralValue);
        }
    }
    branch(*this, vars, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_MIN());
}

ColorModel* ColorModelBuilder::build(std::list<Constraint> cs){
    return new ColorModel(vars, cs, colorQtt);
}

std::string ColorModelBuilder::propagate(){
    return propagate(constraints);
}

std::string ColorModelBuilder::propagate(std::list<Constraint> cs){
    ColorModel* ac = build(cs);
    if(ac -> status() == Gecode::SS_FAILED){
        return "CONTRADICTION";
    } else if(ac -> status() == Gecode::SS_BRANCH){
        return "BRANCH";
    } else if(ac -> status() == Gecode::SS_SOLVED){
        return "COMPLETE";
    }
    return "INVALID";
}

bool ColorModelBuilder::isConsistent(std::list<Constraint> ac){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "passing by a consistency check" << std::endl;
    return propagate(ac) != "CONTRADICTION";
}

std::string ColorModelBuilder::findConflict(){
    std::string result;
    std::list<Constraint> ccs = qx(constraints);    
    std::vector<Constraint> conflictingConstraints{ std::begin(ccs), std::end(ccs) };
    for(int i = 0; i < conflictingConstraints.size(); i++){
        result += " | ";
        Constraint c = conflictingConstraints.at(i);
        if(c.type == 1){
            result += c.varName + " != " + c.rhsVarname;
        } else {
            result += c.varName + " == " + std::to_string(c.rhsLiteralValue);
        }
    }
    result += " | ";
    return result;
}

std::list<Constraint> ColorModelBuilder::qx(std::list<Constraint> ac){
    std::cout << std::endl << "starting qx" << std::endl;
    if(ac.empty()){
        return ac;
    } else {
        return qx(std::list<Constraint>(), ac, std::list<Constraint>());
    }
}

std::list<Constraint> ColorModelBuilder::qx(std::list<Constraint> d, std::list<Constraint> c, std::list<Constraint> b){
    if(!d.empty() && !isConsistent(b)){
        return std::list<Constraint>();
    }
    if(c.size() == 1){
        return c;
    }
    auto middle = std::next(c.begin(), c.size() / 2);
    std::list<Constraint> c1( c.begin(), middle );
    std::list<Constraint> c2( middle, c.end() );
    std::list<Constraint> bc2 = combine(b, c2);
    std::list<Constraint> cs1 = qx(c2, c1, bc2); 
    std::list<Constraint> bcs1 = combine(b, cs1); 
    std::list<Constraint> cs2 = qx(cs1, c2, bcs1);
    std::list<Constraint> cs1cs2 = combine(cs1, cs2);
    return cs1cs2;
}

std::list<Constraint> ColorModelBuilder::combine(const std::list<Constraint> lhs, const std::list<Constraint> rhs){
    std::list<Constraint> l = lhs;
    std::vector<Constraint> r{ std::begin(rhs), std::end(rhs) };
    for(int i = 0; i < r.size(); i++){
        l.push_back(r.at(i));
    }
    return l;
}

std::string ColorModelBuilder::findDiagnose(){
    return "TODO: implement FastDiag";
}