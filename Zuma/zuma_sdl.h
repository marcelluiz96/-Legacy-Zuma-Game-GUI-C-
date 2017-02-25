#ifndef ZUMA_SDL_H
#define ZUMA_SDL_H

/** STL Library */
#include "zuma_stl.h"

/** GAME SDL */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

/** Resolução **/
#define ALTURA 320
#define LARGURA 800

/** Posições */
#define ESQUERDA 1
#define DIREITA 2
#define MOVESPEED 4

class jogador{
    public:
        /** Atributos do Jogador */
        int x, y, sprite;
        int offsetX, offsetY;
        int Pos;
        int anim;
        bool emMovimento;
        bool aBola = false;
        int iCor;
        int b_cor[1];
        int pontos;

        bolas bBola;

        jogador(){
            x = (LARGURA/2);
            y = ALTURA - 64;
            sprite = 0;
            offsetX = 0;
            offsetY = 0;
            anim = 0;
            Pos = 0;
            emMovimento = false;
            b_cor[0] = 0;
            b_cor[1] = 0;
            iCor = 0;
            bBola.x = -20;
            bBola.y = -20;
            bBola.rec.x = -100;
            bBola.rec.y = -100;
            bBola.rec.h = -100;
            bBola.rec.y = -100;
        }

        /** Comandos do jogador */
        void setSprite(int n){ sprite = n; }

        void setX(int _x){ x = _x; }
        void setY(int _y){ y = _y; }


        int getSprite(){ return sprite; }
        int getX(){ return x; }
        int getY(){ return y; }


};

class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

class zuma_sdl
{
    public:
        zuma_sdl(int x, int y, zuma_stl *a);
        ~zuma_sdl();

        void gerarBackground(); // Gera o Background do Zuma;

        void gameFlip(); // Atualiza a tela

        void gameKey(); // Verifica as teclas qeu foram apertadas

        void gameTick(zuma_stl *b); // Cuida da parte do Tick(Tempo em MS)

        void desenharJogador(int x, int y, int n); // Desenha jogador na tela

        void desenharLista(zuma_stl *b); // Desenha a lista na tela;

        void desenharFace(); // Desenha a Face na tela

        void desenharBola(); // Desenha a bola na tela

        void desenharMapa(zuma_stl *b); // Desenha o Mapa do Jogo

        void desenharTexto(std::string text, int x, int y, int f_size, char r, char g, char b); // Desenha o texto na tela !

        void carregarGraficos(); // Carrega os gráficos do jogo !

        SDL_Surface *Grafico(std::string filename); // Retorna o gráfico carregado !

        void desenharSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL ); // Desenha o gráfico

        void movimentarPlayer(); // Movimenta o Player pra Direita ou Esquerda;

        void movimentarLista(zuma_stl *b); // É o movimento da lista, até o final do jogo

        void atirarBola(); // Atira a bola em um alvo, no caso será os elementos da lista

        void Colisao(int x1, int y1, int x2, int y2, int pos, SDL_Rect *a, SDL_Rect *b, zuma_stl *c); // Game Colisão, verifia quando o tiro chega a algum alvo da lista

        void gerarBola(); // Gera 2 bolas aleatorias para o Player

        void gameRender(zuma_stl *game); // Renderiza tudo que é necessário, será divido em partes

        void gameIntro(); // Desenha a Introdução do jogo


        // Menus de Seleção
        bool inIntro = false;
        bool inGame = false;
        bool inCreditos = false;

    private:
        /** Game Surface */
        SDL_Surface* tela; // Tela do Zuma
        SDL_Surface* sprite; // Gráfico do Jogador
        SDL_Surface* sprite_bola; // Bolas do jogo
        SDL_Surface* face; // Face do Jogador
        SDL_Surface* s_bola; // Sprite da Bola pra atirar
        SDL_Surface* tileset; // Gráficos para mapa do jogo !
        SDL_Surface* texto; // Superficie utilizada para desenhar texto
        SDL_Surface* game_intro; // Gráfico da Introdução

        SDL_Color textColor; // Cor do texto !
        Mix_Music *musica; // Musica
        TTF_Font *font = NULL; // Fonte do texto

        /** Atributos SDL */
        std::string NOME = "Zuma Alpha";
        jogador Player;
        Uint32 Tick, timer, timer2, timer3;
        int frame = 0;
        int d_mapx = 0;
        int d_mapy = 0;
        int vidas = 0;

        /** Timer para Calculo de Quadros por Segundo(FPS) */
        Timer fps;
        Timer update;
};

#endif // ZUMA_SDL_H
