#include <gecode/int.hh>

using namespace Gecode;

class AustraliaColors: public Space {

    protected:
        IntVarArray vars;
    public:
        AustraliaColors(void): vars(*this, 7, 0, 2){
            IntVar  wa(vars[0]), nt(vars[1]), sa(vars[2]), q(vars[3])
                 , nsw(vars[4]),  v(vars[5]),  t(vars[6]); 
            rel(*this,  wa, IRT_EQ,   0);
            rel(*this,  nt, IRT_EQ,   1);
            rel(*this,  wa, IRT_NQ,  nt); 
            rel(*this,  wa, IRT_NQ,  sa); 
            rel(*this,  nt, IRT_NQ,  sa); 
            rel(*this,  nt, IRT_NQ,   q); 
            rel(*this,   q, IRT_NQ,  sa); 
            rel(*this,   q, IRT_NQ, nsw); 
            rel(*this, nsw, IRT_NQ,  sa); 
            rel(*this, nsw, IRT_NQ,   v); 
            rel(*this,   v, IRT_NQ,  sa);
        }
        virtual Space* copy(){
            return new AustraliaColors(*this);
        }
        void print(void) const {
            std :: cout << vars << std::endl;
        }

};

int main(){
    AustraliaColors* ac = new AustraliaColors();
    ac -> print();
    ac -> status();
    ac -> print();
    delete ac;
    return 0;
}