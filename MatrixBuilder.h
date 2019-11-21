#pragma once

#include <iostream>
#include <vector>
#include <unistd.h>
#include <bits/stdc++.h> 
#include "Utils.cpp"
#include "LeitorArquivo.h"

using namespace std;

class MatrixBuilder
{
private:
    int** matriz;
    vector<int>* matriz1;

    int nThreads;
    vector<LeitorArquivo*> threads;
    vector<string> arquivos;
    string arqSaida;
    int arqsLidos = 0;
    int lenMaiorLinha = 0;
    int* szLinhas;
public:

    /**
     * Define e valida os parâmetros de entrada
     */
    void setParametros(vector<string>& params)
    {
        try{
            if(params.size() < 5)
                throw "Parâmetros inválidos";

            nThreads = stoi(params.at(1));

            try{
                for(int i=2; i<params.size(); i++){
                    if(params[i] == "-o") 
                        arqSaida = params.at(++i);
                    else
                        arquivos.push_back(params[i]);
                }
            }catch(out_of_range ex){
                throw "Informe um arquivo de saída"; 
            }

            if(nThreads != 2 && nThreads != 4 && nThreads != 8 && nThreads != 16)
                throw "Quantidade de threads inválida";
            if(arqSaida.empty())
                throw "Informe um arquivo de saída";
            if(arquivos.empty())
                throw "Informe pelo menos um arquivo de entrada";
        }catch(const char* msg){
            cout << "Erro: " << msg << endl;
            cout << "-- Utilização --" << endl;
            cout << params[0] << " <numThreads [2, 4, 8, 16]> <arq_1..n> -o <arqSaida>" << endl;
        }
    }

    /**
     * Imprime a matriz
     */
    bool printMatriz()
    {
        for(int i=0; i<arquivos.size(); i++){
            for(int j=0; j<szLinhas[i]; j++){
                cout << matriz[i][j] << ", ";
            }
            cout << endl;
        }
    }

    /**
     * Carrega os arquivos informados
     */
    void carregarArquivos()
    { 
        // Aloca a matriz com o conteúdo dos arquivos e seus tamanhos
        matriz = (int **) calloc(sizeof(int *), arquivos.size());
        szLinhas = (int *) calloc(sizeof(int), arquivos.size());
        
        // Inicializa as threads
        for(int i=0; i<nThreads; i++)
            threads.push_back(new LeitorArquivo(i));

        clock_t inicio = clock();
        for(int i=0; i<arquivos.size(); i++){
            // 'Pedaço' do arquivo que a thread irá ler
            int fatiaThread = Utils::getTamanhoArquivo(arquivos[i]) / nThreads;
            
            // Ler o arquivo e aguardar cada thread
            for(int j=0; j<nThreads; j++)
                threads[j]->lerArquivo(arquivos[i], fatiaThread * j, fatiaThread);
            for(int j=0; j<nThreads; j++)
                threads[j]->aguardarThread();

            // Coletar o conteúdo de cada thread
            vector<int> cont;
            for(int j=0; j<nThreads; j++){
                vector<int> vet = threads[j]->getNumeros();
                cont.insert(cont.end(), vet.begin(), vet.end());
            }
            matriz[arqsLidos] = (int *) calloc(sizeof(int), cont.size());
            szLinhas[arqsLidos] = cont.size();

            // Alimenta a matriz principal
            for(int j=0; j<cont.size(); j++)
                matriz[arqsLidos][j] = cont[j];

            // Define a maior linha (para adição de zeros)
            if(cont.size() > lenMaiorLinha)
                lenMaiorLinha = cont.size();

            arqsLidos++;

        }

        cout << "Tempo para ler o arquivo com " << nThreads << " threads :" << endl;
        clock_t fim = clock();
        double time_taken = double(fim - inicio) / double(CLOCKS_PER_SEC) * 1000;
        cout << time_taken << setprecision(7) << " ms " << endl;

        salvarArquivoSaida();
        
    }
    

    /**
     *  Grava o arquivo de saída 
     */
    void salvarArquivoSaida()
    {
        ofstream arq(arqSaida);
        for(int i=0; i<arquivos.size(); i++){
            string linha;
            for(int j=0; j<szLinhas[i]; j++)
                linha.append(to_string(matriz[i][j]).append("\t"));
            for(int j=0; j<lenMaiorLinha - szLinhas[i]; j++)
                linha.append("0\t");

            linha.append("\n");
            arq.write(linha.c_str(), linha.size());

        }

    }


};