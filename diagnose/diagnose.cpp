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

ColorModelBuilder ColorModelBuilder::withVars(std::list<std::string> vs) {
    vars = vs;
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

std::string Constraint::toString() const {
    if(type == 1){
        return varName + " != " + rhsVarname;
    } else {
        return varName + " == " + std::to_string(rhsLiteralValue);
    }
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

bool ColorModelBuilder::solve(){
    ColorModel* m = build(constraints);
    Gecode::DFS<ColorModel> e(m);
    if(ColorModel* solution = e.next()){
        return true;
    } else {
        return false;
    }
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
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return propagate(ac) != "CONTRADICTION";
}

std::string ColorModelBuilder::findConflict(){
    std::list<Constraint> conflictingConstraints = qx(constraints);  
    return toString(conflictingConstraints);  
}

std::list<std::string> ColorModelBuilder::toStringList(const std::list<std::list<Constraint>> solutions){
    std::list<std::string> s_solutions;
    std::transform(solutions.begin(), solutions.end(), back_inserter(s_solutions), [this](std::list<Constraint> cs){
        return toString(cs);
    });
    return s_solutions;
}

std::string ColorModelBuilder::toString(const std::list<Constraint> cs){
    std::string result;
    std::vector<Constraint> conflictingConstraints{ std::begin(cs), std::end(cs) };
    for(int i = 0; i < conflictingConstraints.size(); i++){
        result += " | ";
        result += conflictingConstraints.at(i).toString();
    }
    result += " | ";
    return result;
}

std::list<Constraint> ColorModelBuilder::qx(std::list<Constraint> ac){
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

std::list<std::string> ColorModelBuilder::findDiagnoses(){
    return toStringList(hsDAG(constraints));  
    // return {toString(fd(constraints))};
}

std::list<std::list<Constraint>> ColorModelBuilder::hsDAG(std::list<Constraint> ac){
    std::queue<std::list<Constraint>> queue;
    queue.push(std::list<Constraint>());
    std::list<std::list<Constraint>> visited;
    std::list<std::list<Constraint>> diags;
    while(!queue.empty()){
        std::list<Constraint> path = queue.front();
        queue.pop();
        std::list<Constraint> diag = fd(subtract(ac, path));
        if(!diag.empty() && !contains(toStringList(diags), toString(diag))){
            diags.push_back(diag);
        }
        for(Constraint c : diag){
            std::list<Constraint> newPath = path;
            newPath.push_back(c);
            newPath.sort([](const Constraint &a, const Constraint &b) {
                return a.toString() > b.toString();
            });
            if(!contains(toStringList(visited), toString(newPath))){
                queue.push(newPath);
                visited.push_back(newPath);
            }
        }
    }
    return diags;
}

std::list<Constraint> ColorModelBuilder::fd(std::list<Constraint> ac){
    if(ac.empty()){
        return ac;
    } else {
        return fd(std::list<Constraint>(), ac, ac);
    }
}

std::list<Constraint> ColorModelBuilder::fd(std::list<Constraint> d, std::list<Constraint> c, std::list<Constraint> ac){
    if(!d.empty() && isConsistent(ac)){
        return std::list<Constraint>();
    }
    if(c.size() == 1){
        return c;
    }
    auto middle = std::next(c.begin(), c.size() / 2);
    std::list<Constraint> c1( c.begin(), middle );
    std::list<Constraint> c2( middle, c.end() );
    std::list<Constraint> acMinusC2 = subtract(ac, c2);
    std::list<Constraint> d1 = fd(c2, c1, acMinusC2); 
    std::list<Constraint> acMinusD1 = subtract(ac, d1); 
    std::list<Constraint> d2 = fd(d1, c2, acMinusD1);
    std::list<Constraint> d1d2 = combine(d1, d2);
    return d1d2;
}

std::list<Constraint> ColorModelBuilder::subtract(const std::list<Constraint> lhs, const std::list<Constraint> rhs){
    std::list<Constraint> l;
    std::list<std::string> r;
    std::transform(rhs.begin(), rhs.end(), std::back_inserter(r), [](Constraint c){return c.name;});
    auto pred = [r](Constraint constraint) { 
        return !std::any_of(r.begin(), r.end(), [constraint](std::string name_from_r){ 
            return name_from_r == constraint.name; 
        });
    };
    std::copy_if(lhs.begin(), lhs.end(), std::back_inserter(l), pred);
    return l;
}

std::list<Constraint> ColorModelBuilder::combine(const std::list<Constraint> lhs, const std::list<Constraint> rhs){
    std::list<Constraint> l = lhs;
    std::vector<Constraint> r{ std::begin(rhs), std::end(rhs) };
    for(int i = 0; i < r.size(); i++){
        l.push_back(r.at(i));
    }
    return l;
}

bool contains (const std::list<std::string> items, const std::string s){
    bool found = false;
    for(auto item : items){
        if(s.find(item) != std::string::npos){
            return true;
        }
    }
    return false;
}