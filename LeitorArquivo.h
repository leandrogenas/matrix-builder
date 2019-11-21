#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Utils.cpp"

using namespace std;

class LeitorArquivo
{
private:
    pthread_t thread;
    string camArquivo;
    int offset;
    int length;
    int idThread;
    vector<int> numeros; 

    /**
     * Carrega o arquivo e lê os números, com espaço
     * como delimitador
     */
    void processarArquivo()
    {
        ifstream arq(camArquivo);
        if(!arq.is_open())
            return;

        arq.seekg(offset);
        // Pega o proximo numero
        if(arq.peek() == ' '){
            char c;
            while((c = arq.get()) != ' ');
        }

        int i=0;
        string buf;
        do{
            char c = arq.get();
            if(c == ' '){
                numeros.push_back(stoi(buf));
                buf = "";
            }else{
                buf += c;
            }
        
        }while(arq.good() && i++ < length);

        arq.close();
    }

    /**
     * Ponto de entrada da thread
     */
    static void* funcThreadEntry(void* that)
    {
        ((LeitorArquivo *) that)->processarArquivo();
    }
public:

    LeitorArquivo(const int idThread)
        :idThread(idThread) { }

    vector<int> getNumeros()
    {
        return numeros;
    }

    bool lerArquivo(const string camArq, const int offset, const int length)
    {
        numeros.clear();

        this->camArquivo = camArq;
        this->offset = offset;
        this->length = length;
        return (pthread_create(&thread, NULL, funcThreadEntry, this) == 0);
    }

    void aguardarThread()
    {
        (void) pthread_join(thread, NULL);
    }

    void printNumeros()
    {
        for(int nr: numeros)
            cout << nr << endl;
    }

    int getNumero(const int idx)
    {
        return numeros.at(idx);
    }

};