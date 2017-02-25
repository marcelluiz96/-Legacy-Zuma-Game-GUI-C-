#ifndef ZUMA_STL_H
#define ZUMA_STL_H

#include <SDL/SDL.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <iterator>
#include <fstream>

class bolas{
    public:
        bolas(){
            x = -10;
            y = -10;
            type = 0;
            anim = 0;
            rec.x = 0;
            rec.y = 0;
            rec.w = 0;
            rec.h = 0;
            parado = false;
            bI = 0;
            bWait = 0;
        }

        SDL_Rect rec;
        int x, y; // Cordenadas das bolas
        int type; // Tipo da bola
        int anim;
        int bI; // Indice do caminho;
        int bWait; // Tempo que vai ser necessário esperar para a lista prosseguir
        bool parado;
        /*** Tipos:
            0
            1
            2
            3
        */
};


class zuma_mapa{
    public:
    /** Atributos do Mapa */

    int max_x, max_y;

    char **mapa;
    struct sumidouro{
        int x = 0;
        int y = 0;
    };

    struct entrada{
        int x = 0;
        int y = 0;
    };

    sumidouro map_sumidouro;
    entrada map_entrada;

    zuma_mapa(){
    }

    ~zuma_mapa(){
        for(int i = 0; i < max_y; ++i) {
            delete [] mapa[i];
        }
            delete [] mapa;
    }

    /** Comandos do mapa */
    int getMaxX(){ return max_x; }
    int getMaxY(){ return max_y; }

    void alocarMapa(){
        if(max_x != 0 && max_y != 0){
            mapa = new char*[max_y];
                for(int i = 0; i < max_y; ++i) {
                    mapa[i] = new char[max_x];
            }
        }
    }

    void setSumidouro(int x, int y){ map_sumidouro.x = x; map_sumidouro.y = y; }
    void setEntrada(int x, int y){ map_entrada.x = x; map_entrada.y = y; }
};

class caminho{
    public:
        int x, y;
        caminho(){
            x = 0;
            y = 0;
        }
};


class zuma_stl
{
    public:
        zuma_stl();
        ~zuma_stl();

        void inserirBola(bolas b); // Insere as Bolas
        void inserirBola(bolas b, int pos); // Insere as bolas na lista em uma Pos

        void gerarBola(int tamanho); // Gera as Bolas do Zuma

        void removerBola(int pos); // Remove a bola

        void verificarBola(bolas b, int pos); // Verifica cor da bola que está naquela posição

        void combo(int pos); // Verificar se teve um combo, pra remover bolas

        void comboaux(int pos); // Verifica se tem combo(usado quando a bola é de cor diferente)

        void getLista(); // DEBUG: Comando para verificar toda a lista

        void carregarPontuacao(); // Carrega a melhor pontuação do jogo !

        void salvarPontucao(int pontos); // Salva a melhor pontucao(obs: se a sua pontuacao, for menor que a melhor, n salva

        void carregarMapa(); // Carrega o Mapa do jogo

        std::list<bolas> Z_bola; //Lista das bolas
        std::list<bolas>::iterator Z_it; // Iterador
        std::list<bolas>::iterator aux; // Iterador
        std::list<bolas>::iterator aux2; //Iterador

        int melhorPontuacao = 0;
        int t_x = 0; // Tamanho do Mapa X
        int t_y = 0; // Tamanho do Mapa Y
        int nBolas = 0; // Número de bolas que são geradas
        int tCaminho = 0; // Total de tiles que é seguida
        int aFila = 0; // Auxilia a fila
        caminho *c_bola; // Caminho a ser seguido


        zuma_mapa game_map;
};

#endif // ZUMA_STL_H
