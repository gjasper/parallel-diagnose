#include <string>
#include <list>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <gecode/int.hh>

class Constraint {
    public:
        int type;
        std::string name;
        std::string varName;
        std::string rhsVarname;
        int rhsLiteralValue;
        Constraint(std::string, std::string, std::string);
        Constraint(std::string, std::string, int);
};

class ColorModel: public Gecode::Space {
    protected:
        Gecode::IntVarArray vars;
    public:
        ColorModel(
                std::list<std::string>
            ,   std::list<Constraint> constraints
            ,   int colorQtt
            );
        virtual Space* copy(){
            return new ColorModel(*this);
        }
};

class ColorModelBuilder {
    private:
        std::list<std::string> vars;
        std::list<Constraint> constraints;
        int colorQtt;
        std::string propagate(std::list<Constraint>);
        bool isConsistent(std::list<Constraint>);
        std::list<Constraint> combine(std::list<Constraint>, std::list<Constraint>);
        std::list<Constraint> subtract(std::list<Constraint>, std::list<Constraint>);
        std::list<Constraint> qx(std::list<Constraint>);
        std::list<Constraint> qx(std::list<Constraint>, std::list<Constraint>, std::list<Constraint>);
        std::list<Constraint> fd(std::list<Constraint>);
        std::list<Constraint> fd(std::list<Constraint>, std::list<Constraint>, std::list<Constraint>);
    public:
        ColorModelBuilder();
        ColorModel* build(std::list<Constraint>);
        ColorModelBuilder withReq(std::string, int);
        ColorModelBuilder withNeighbours(std::string, std::string);
        ColorModelBuilder withVar(std::string);
        ColorModelBuilder withColorQtt(int);
        std::string propagate();
        std::string findConflict();
        std::string findDiagnose();
};
