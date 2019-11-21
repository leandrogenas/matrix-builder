#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <bits/stdc++.h> 

using namespace std;

class Utils
{
private:
    Utils();

public:
    static int getTamanhoArquivo(const string nomeArq)
    {
        std::ifstream in(nomeArq, std::ifstream::ate | std::ifstream::binary);
        return in.tellg(); 
    }

    static void medirTempo(const time_t inicio)
    {
        time_t fim;
        time(&fim);
        // Calculating total time taken by the program. 
        double time_taken = double(fim - inicio); 
        cout << "Time taken by program is : " << fixed 
            << time_taken << setprecision(5); 
        cout << " sec " << endl; 

    }

    static void gerarArquivoDados(const string nomeArq, const int qtdNumeros)
    {
        int min = 1, max = 15000;
        ofstream arq(nomeArq);
        if(!arq.is_open())
            return;
        
        for(int i=0; i<qtdNumeros; i++){
            string num = to_string((rand() % max) + min) + " ";
            arq.write(num.c_str(), num.length());
        }

        arq.close();
    }

};