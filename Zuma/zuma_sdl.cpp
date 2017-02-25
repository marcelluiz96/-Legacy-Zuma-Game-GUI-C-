#include "zuma_sdl.h"

zuma_sdl::zuma_sdl(int x, int y, zuma_stl *a)
{
    d_mapx = x * 32, d_mapy = y * 32;
   /** Inicializa todas as funções do SDL */
   if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
      std::cout << "Erro ao inicializar SDL!" << std::endl;
      exit(1);
   }


   /** Inicializa a Fonte ! */
   if( TTF_Init() == -1 )
    {
        exit(1);
    }

   tela = SDL_SetVideoMode(d_mapx, d_mapy, 32, SDL_SWSURFACE);
   /** Verificar se realmente inicializou a tela */
   if(tela == NULL){
     std::cout << "Erro ao inicializar a tela !" << std::endl;
     exit(1);
   }


    if( SDL_Init(SDL_INIT_AUDIO) == -1){

        exit(1);
    }


   /** Nome do jogo */
   SDL_WM_SetCaption(NOME.c_str(), NULL);

   /** Pega tempo em MS do Sistema */
   Tick = SDL_GetTicks();
   timer = SDL_GetTicks() + 500;
   timer2 = SDL_GetTicks() + 1000;
   timer3 = SDL_GetTicks() + 300;

   /** Inicializa o Timer */
    update.start();

   /** Inicia o FPS */
    fps.start();


   /**
   0    1   0   0   0   0   0
   0    1   0   0   0   0   0
   0    1   0   0   0   0   0
   0    1   1   1   1   1   0
   0    0   0   0   0   1   1
   0    0   0   0   0   0   0
   0    0   0   0   0   0   0
   0    0   0   0   0   0   0
   0    0   0   0   0   0   0
   */

    /** Cria caminho da lista */
    a->c_bola = new caminho[a->tCaminho];
    int i = 0;
    for(int x = 0; x < a->game_map.getMaxY(); x++)
        for(int y = 0; y < a->game_map.getMaxX(); y++){
            if(a->game_map.mapa[x][y] == '1'){
               a->c_bola[i].x = y;
               a->c_bola[i].y = x;
               i++;
            }
    }

    /** Número de vidas */
    vidas = 3;

    Player.setY(a->game_map.getMaxY());
    gerarBola();
}

zuma_sdl::~zuma_sdl()
{
    /** Limpa da Memoria os gráficos */
    SDL_FreeSurface( tela );
    SDL_FreeSurface( sprite );
    SDL_FreeSurface( sprite_bola);
    SDL_FreeSurface( face );
    SDL_FreeSurface( s_bola );
    SDL_FreeSurface( tileset );
    SDL_FreeSurface( texto );

    Mix_FreeMusic(musica);

    /** Finaliza fonte e SDL */
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

/** Renderiza todo o Zuma */
void zuma_sdl::gameRender(zuma_stl *game){
        /** Renderiza o Background */
        gerarBackground();


        /**  Renderiza a Introdução do jogo */
        gameIntro();


        /** Renderiza os gráficos */
        if(inGame == true){
            desenharMapa(game);
            desenharJogador(20,20,1);
            desenharBola();
            desenharLista(game);
            desenharFace();
        }
        /** Atributos do Game, Tick, Controles e Atualização de Tela */
        gameTick(game);
        gameKey();
        gameFlip();
}

/** Desenha a Introdução do Jogo */
void zuma_sdl::gameIntro(){
    if(inIntro == true){
        SDL_FreeSurface( tela );

        tela = SDL_SetVideoMode(480,320,32,SDL_SWSURFACE);
        inIntro = false;
    }
    if(inGame == false && inCreditos == false)
        desenharSurface(0,0, game_intro, tela);

}

/** Coloca a cor na tela do jogo */
void zuma_sdl::gerarBackground(){
    SDL_FillRect(tela, &tela->clip_rect, SDL_MapRGB(tela->format,0,0,0));
}

/** Atualiza a tela */
void zuma_sdl::gameFlip(){
    SDL_Flip(tela);
}

/** Carrega os gráficos do jogo */
void zuma_sdl::carregarGraficos(){
    sprite = Grafico("gfx\\1.png");
    sprite_bola = Grafico("gfx\\2.png");
    face = Grafico("gfx\\3.png");
    s_bola = Grafico("gfx\\4.png");
    tileset = Grafico("gfx\\5.png");
    game_intro = Grafico("gfx\\6.png");
    font = TTF_OpenFont( "gfx\\gamefont.fon", 15 );
    musica = Mix_LoadMUS("gfx\\musica.mp3");

    Mix_OpenAudio(11025,AUDIO_S16SYS,2,640);

    Mix_PlayMusic(musica,1);
}

/** Carrega o grafico na memoria */
SDL_Surface *zuma_sdl::Grafico(std::string filename) {
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //Return the optimized surface
    return loadedImage;
}

/** Desenha na tela o gráfico */
void zuma_sdl::desenharSurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip){
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


/** Desenha a Lista na tela */
void zuma_sdl::desenharLista(zuma_stl *b){
    SDL_Rect game_rect;


    for(b->Z_it = b->Z_bola.begin(); b->Z_it != b->Z_bola.end(); b->Z_it++){
        b->Z_it->rec.h = 64;
        b->Z_it->rec.w = 48;
        b->Z_it->rec.y = b->Z_it->type * 64;
        b->Z_it->rec.x = b->Z_it->anim * 48;

        desenharSurface(b->Z_it->x * 32,(b->Z_it->y * 32) - 32,sprite_bola,tela, &b->Z_it->rec);
    }
}

/** Desenha a face na tela */
void zuma_sdl::desenharFace(){
    SDL_Rect game_rect;

    game_rect.h = 96;
    game_rect.w = 96;
    game_rect.x = Player.b_cor[Player.iCor] * 96;
    game_rect.y = 0;

    desenharSurface(0,480,face,tela, &game_rect);
}

void zuma_sdl::desenharMapa(zuma_stl *b){
    SDL_Rect game_rect;

    game_rect.h = 32;
    game_rect.w = 32;
    game_rect.x = 0;
    game_rect.y = 0;

    /** Desenha o Sumidouro */
        game_rect.x = 32;
        desenharSurface(b->game_map.map_sumidouro.y * 32,b->game_map.map_sumidouro.x * 32, tileset,tela,&game_rect );
    for(int x = 0; x < b->game_map.getMaxY(); x++)
        for(int y = 0; y < b->game_map.getMaxX(); y++){
            if(b->game_map.mapa[x][y] == '1'){
                game_rect.x = 0;
                desenharSurface(y * 32,x * 32, tileset,tela,&game_rect );
            }
        }
}

/** Atributos para desenhar jogador, tamanho do retangulo, posição do gráfico e etc */
void zuma_sdl::desenharJogador(int x, int y, int n){
    SDL_Rect game_rect;

    game_rect.h = 64;
    game_rect.w = 48;
    game_rect.x = Player.anim * 48;
    game_rect.y = Player.b_cor[Player.iCor] * 64;

    desenharSurface(Player.getX(),(Player.getY() * 32) - 64,sprite,tela,&game_rect);
}

/** Desenha a bola que foi lançada */
void zuma_sdl::desenharBola(){
    SDL_Rect game_rect;



    Player.bBola.rec.h = 32;
    Player.bBola.rec.w = 32;
    Player.bBola.rec.x = 0;
    Player.bBola.rec.y = 0;

    if(Player.aBola == true)
        desenharSurface((Player.bBola.x * 32) - 2 , (Player.bBola.y * 32), s_bola, tela, &Player.bBola.rec);
}

/** Cuida da Parte do Tick(tempo em ms) */
void zuma_sdl::gameTick(zuma_stl *b){
    std::stringstream texto;
    std::stringstream texto2;
    int i = 0;
    desenharTexto("Atributos",40,400,10,255,255,255);

    texto << "Vidas: " << vidas;
    desenharTexto(texto.str(), 40, 410,10,255,255,255);


    texto.str( std::string() );
    texto.clear();

    texto << "Tamanho da Lista: " << b->Z_bola.size();
    desenharTexto(texto.str(), 40, 420,10,255,255,255);
/*
    texto2 << "Lista CorY: " << b->c_bola[b->bI].y;
    desenharTexto(texto2.str(), 40, 430,10,255,255,255);

    texto2.str( std::string() );
    texto2.clear();

    texto2 << "Sumidouro X: " << b->game_map.map_sumidouro.x;
    desenharTexto(texto2.str(), 40, 440,10,255,255,255);

    texto2.str( std::string() );
    texto2.clear();

    texto2 << "Sumidouro Y: " << b->game_map.map_sumidouro.y;
    desenharTexto(texto2.str(), 40, 450,10,255,255,255);

    texto2.str( std::string() );
    texto2.clear();

    texto2 << "Player CorY: " << Player.getY();
    desenharTexto(texto2.str(), 40, 460,10,255,255,255);

    */

    if(Tick > timer){
        movimentarPlayer();

        atirarBola();
        timer = SDL_GetTicks() + 10;
    }

    if(Tick > timer2){
        if(inGame == true)
            movimentarLista(b);

        timer2 = SDL_GetTicks() + 1000;

    }

        if(Player.aBola == true){
            for(b->Z_it = b->Z_bola.begin(); b->Z_it != b->Z_bola.end(); b->Z_it++){
                if(Player.aBola == false)
                    break;
                Colisao(b->Z_it->x, b->Z_it->y, Player.bBola.x, Player.bBola.y, i,&b->Z_it->rec, &Player.bBola.rec, b);
                i++;
            }
        }

    /** Animações */
    if(Tick > timer3){
        /** Anima a Lista */
        for(b->Z_it = b->Z_bola.begin(); b->Z_it != b->Z_bola.end(); b->Z_it++){
            if(b->Z_it->anim > 2){
                b->Z_it->anim = 0;
            }else{
                b->Z_it->anim++;
            }
        }

        /** Anima o Personagem */
        if(Player.emMovimento == true){
            if(Player.anim > 5){
                Player.anim = 0;
            }else{
                Player.anim += 1;
            }
        }else{
            Player.anim = 0;
        }
        timer3 = SDL_GetTicks() + 100;
    }
    Tick = SDL_GetTicks();

    //Increment the frame counter
    frame++;

    //If a second has passed since the caption was last updated
    if( update.get_ticks() > 1000 )
    {
        //The frame rate as a string
        std::stringstream caption;

        //Calculate the frames per second and create the string
        caption << "Zuma Alpha - FPS: " << frame / ( fps.get_ticks() / 1000.f );

        //Reset the caption
        SDL_WM_SetCaption( caption.str().c_str(), NULL );

        //Restart the update timer
        update.start();
    }


}

/** Movimenta a Lista **/
void zuma_sdl::movimentarLista(zuma_stl *b){
    int i = 0;
    for(b->Z_it = b->Z_bola.begin(); b->Z_it != b->Z_bola.end(); b->Z_it++){
        if(b->aFila == i){
            b->aFila++;
            break;
        }

        /** Verificar se a fila tá em espera ou n */
          if(b->Z_it->bWait > 0){
                b->Z_it->bWait--;
                i++;
                break;
            }else{
                if(b->Z_it->parado == true){
                    b->Z_it->parado = false;
                }
            }

        if(b->Z_it->parado == false){
            b->Z_it->x = b->c_bola[b->Z_it->bI].x;
            b->Z_it->y = b->c_bola[b->Z_it->bI].y;
            b->Z_it->bI++;

            /* Verifica quantas vidas tem, se tem 1, fim de game, se n, reinicia o jogo */
            if(b->Z_it->x == 0 && b->Z_it->y == 0){
                if(vidas == 1){
                    SDL_Quit();
                    exit(1);
                }else{
                    for(b->Z_it = b->Z_bola.begin(); b->Z_it != b->Z_bola.end(); b->Z_it++){
                        b->Z_it->x = -5;
                        b->Z_it->y = -5;
                        b->Z_it->bI = 0;
                    }
                    vidas -=1;

                }
                i++;
                break;
            }
      }
      i++;
    }
}

/** Verificar Colisão */
void zuma_sdl::Colisao(int x1, int y1, int x2, int y2, int pos, SDL_Rect *a, SDL_Rect *b, zuma_stl *c){
    /** Casos que não vai haver colisão */
    bool col = false;

  /*  if(x1 > x2 + b->w)
        col = false;
    else if(y1 > y2 + b->h)
        col = false;
    else if(x1 + a->w < x2)
        col = false;
    else if(y1 + a->h < y2)
        col = false;
    else
        col = true;

        if(y1+1 == y2){
            col = true;
        }
    */
  if(y2 > 0 && x2 > 0 && x1 > 0 && y1 > 0){
       if(y1+1 == y2 && x1 == x2){
            if(y2 != 0 | x2 != 0){
                Player.bBola.x = x1;
                Player.bBola.y = y1;
                c->verificarBola(Player.bBola, pos);
                Player.aBola = false;

            }

     }
  }
}

/** Desenha o texto na tela */
void zuma_sdl::desenharTexto(std::string text, int x, int y, int f_size, char r, char g, char b){
    textColor = {r,g,b};

    /** Removido por existir vazamento de memória ! */
      texto = TTF_RenderText_Solid(font,text.c_str(), textColor);

      desenharSurface(x,y,texto,tela);
      SDL_FreeSurface(texto);
}

/** Movimento da bola que foi atirada */
void zuma_sdl::atirarBola(){
    if(Player.aBola == true){
         Player.bBola.y -= 1;
         if(Player.bBola.y < 0){
            Player.bBola.y = -10;
            Player.bBola.x = -10;
            Player.aBola = false;
         }
    }
}

/** Movimenta Personagem para Esquerda ou Direta */
void zuma_sdl::movimentarPlayer(){
    if(Player.emMovimento == true){
        switch(Player.Pos){
            case DIREITA:
                  if(Player.getX() + 48 < LARGURA)
                    if(Player.offsetX < 32){
                        Player.setX(Player.getX() + MOVESPEED);
                        Player.offsetX += 2;
                    }else{
                        Player.emMovimento = false;
                        Player.offsetX = 0;
                    }
                else
                    Player.setX(LARGURA - 48);

            break;

            case ESQUERDA:
                  if(Player.getX() > 0)
                    if(Player.offsetX < 32){
                        Player.setX(Player.getX() - MOVESPEED);
                        Player.offsetX += 2;
                    }else{
                        Player.emMovimento = false;
                        Player.offsetX = 0;
                    }
                else
                    Player.setX(0);

            break;

        }
    }
}

/** Gera duas bolas aleatorias para o Player */
void zuma_sdl::gerarBola(){
    for(int i = 0; i < 1; i++)
        Player.b_cor[i] = rand() % 4;
}

/** Pega a tecla que foi pressionada */
void zuma_sdl::gameKey(){
 SDL_Event event;

        if( SDL_PollEvent( &event ) )
        {

            if( event.type == SDL_KEYDOWN )
            {
                switch( event.key.keysym.sym )
                {

                    case SDLK_LEFT:
                        Player.Pos = ESQUERDA;
                        Player.emMovimento = true;
                    break;

                    case SDLK_RIGHT:
                        Player.Pos = DIREITA;
                        Player.emMovimento = true;
                    break;

                    case SDLK_ESCAPE:
                        SDL_Quit();
                    break;

                    case SDLK_UP:
                        if(Player.iCor < 1)
                            Player.iCor += 1;
                        else
                            Player.iCor = 0;
                    break;

                    case SDLK_SPACE:
                        if(Player.aBola == false){
                            Player.bBola.x = (Player.getX() / 32);
                            Player.bBola.y = Player.getY() - 2;//(Player.getY() / 32);
                            Player.bBola.type = Player.b_cor[Player.iCor];
                            gerarBola();
                            Player.aBola = true;
                        }
                    break;

                    case SDLK_1:
                        SDL_FreeSurface( tela );
                        tela = SDL_SetVideoMode(d_mapx, d_mapy, 32, SDL_SWSURFACE);
                        inGame = true;

                        inCreditos = false;
                        inIntro = false;
                    break;

                    default:

                    break;
                }

            }

            else if( event.type == SDL_QUIT )
            {
                SDL_Quit();
                exit(1);
            }
        }
}


/** Timer do Game - Calcula FPS e etc */
/** Criado por lazyfoo.net/SDL_tutorials/ */

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

