#include <iostream>
#include <vector>
#include "MatrixBuilder.h"

int main(int argc, char** argv)
{
    vector<string> args;
    for(int i=0; i<argc; i++)
        args.push_back(std::string(argv[i]));

    MatrixBuilder ord;
    ord.setParametros(args);
    ord.carregarArquivos();
    ord.salvarArquivoSaida();

}