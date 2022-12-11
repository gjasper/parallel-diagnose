#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <queue>
#include <future>
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
        std::string toString() const;
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
        void print(void) const {
            std :: cout << vars << std::endl;
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
        std::list<std::list<Constraint>> hsDAG(std::list<Constraint>);
        std::list<std::list<Constraint>> parallelHsDAG(std::list<Constraint>);
        std::list<Constraint> qx(std::list<Constraint>);
        std::list<Constraint> qx(std::list<Constraint>, std::list<Constraint>, std::list<Constraint>);
        std::list<Constraint> fd(std::list<Constraint>);
        std::list<Constraint> fd(std::list<Constraint>, std::list<Constraint>);
        std::list<Constraint> fd(std::list<Constraint>, std::list<Constraint>, std::list<Constraint>);
    public:
        ColorModelBuilder();
        ColorModel* build(std::list<Constraint>);
        ColorModelBuilder withReq(std::string, int);
        ColorModelBuilder withNeighbours(std::string, std::string);
        ColorModelBuilder withVar(std::string);
        ColorModelBuilder withVars(std::list<std::string>);
        ColorModelBuilder withColorQtt(int);
        bool solve();
        std::string propagate();
        std::string findConflict();
        std::string findDiagnose(){
            return findDiagnose(0);
        }
        std::string findDiagnose(int i) {
            std::list<std::string> diags = findDiagnoses();
            std::vector<std::string> ds{ std::begin(diags), std::end(diags) };
            return ds.at(i);
        }
        std::list<std::string> findDiagnoses(){
            return findDiagnoses(false);
        }
        std::list<std::string> findDiagnoses(bool);
        std::string toString(std::list<Constraint>);
        std::list<std::string> toStringList(std::list<std::list<Constraint>>);
};

bool contains(std::list<std::string>, std::string);
bool find(std::list<std::string>, std::string);