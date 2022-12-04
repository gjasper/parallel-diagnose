#include <string>
#include <list>
#include <vector>
#include <map>
#include <gecode/int.hh>


class Constraint {
    public:
        int type;
        std::string varName;
        std::string rhsVarname;
        int rhsLiteralValue;
        Constraint(std::string, std::string);
        Constraint(std::string, int);
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
    public:
        ColorModelBuilder();
        ColorModel* build();
        ColorModelBuilder withReq(std::string, int);
        ColorModelBuilder withNeighbours(std::string, std::string);
        ColorModelBuilder withVar(std::string);
        ColorModelBuilder withColorQtt(int);
        std::string propagate();
        std::string findConflicts(int);
};
