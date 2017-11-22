#include <iostream>
#include "Starbucks.cpp"
#include "StarbucksModel2.cpp"


int main(int argc, char* argv[]) {

    if(argc==3){
        Starbucks starbucks(argv[1],argv[2]);
        starbucks.simulate();
        StarbucksModel2 st2(argv[1],argv[2]);
        st2.simulate();
    }
    else
        cout<<"Input file and outfut file names should be given as arguments\n";
    return 0;
}