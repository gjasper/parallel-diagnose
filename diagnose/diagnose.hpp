#include <string>
#include <list>
#include <vector>
#include <map>
#include <tuple>
#include <gecode/int.hh>

class ColorModel: public Gecode::Space {
    protected:
        Gecode::IntVarArray vars;
    public:
        ColorModel(
                std::list<std::string>
            ,   std::list<std::tuple<std::string, std::string>>
            ,   int colorQtt
            );
        ColorModel(
                std::list<std::string>
            ,   std::list<std::tuple<std::string, int>>
            ,   std::list<std::tuple<std::string, std::string>>
            ,   int colorQtt
            );
        virtual Space* copy(){
            return new ColorModel(*this);
        }
};

class ColorModelBuilder {
    private:
        std::list<std::string> vars;
        std::list<std::tuple<std::string, std::string>> neighbours;
        std::list<std::tuple<std::string, int>> reqs;
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
