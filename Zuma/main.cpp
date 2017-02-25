#include "zuma_sdl.h"

int main(int argc, char *argv[]){
    zuma_stl mySTL;

    mySTL.carregarMapa();

    zuma_sdl mySDL(mySTL.game_map.getMaxX(),mySTL.game_map.getMaxY(), &mySTL);
    mySDL.carregarGraficos();
    /** O jogo vai ser divido em 3 partes  *
        1 - Tela Inicial
        2 - Novo Jogo
        3 - Créditos
    */
    mySDL.inIntro = true;
    while(1){
        mySDL.gameRender(&mySTL);
    }

    return 0;
}
