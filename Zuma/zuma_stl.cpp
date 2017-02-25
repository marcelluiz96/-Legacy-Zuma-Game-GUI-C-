    #include "zuma_stl.h"

zuma_stl::zuma_stl()
{
    aFila = 1; // inicializa o auxilio da Fila
}

zuma_stl::~zuma_stl()
{
    delete(c_bola);
    Z_bola.clear();
}


/** Carrega mapa do jogo */
void zuma_stl::carregarMapa(){
    std::ifstream g_mapa("gfx\\mapa.txt");
    /** Pega atributos do mapa */
    g_mapa >> t_x;
    g_mapa >> t_y;
    g_mapa >> nBolas;
    g_mapa >> tCaminho;

    /** Salva variaveis do mapa */
    game_map.max_x = t_y;
    game_map.max_y = t_x;
    game_map.alocarMapa();

    /** Carrega o mapa com os atributos, 0,1,x e @ */
    for (int c = 0; c < t_x; c++) {
        for (int c2 = 0; c2 < t_y; c2++) {
            g_mapa >> game_map.mapa[c][c2];
        }
    }


   /** Cria o Sumidouro - CAVEIRA, KPETA -n e outros */
    for (int c = 0; c < t_x; c++) {
        for (int c2 = 0; c2 < t_y; c2++) {
            if(game_map.mapa[c][c2] == 'X'){
                game_map.setSumidouro(c,c2);
            }else if(game_map.mapa[c][c2] == '@'){
                game_map.setEntrada(c,c2);
            }
        }
    }
    gerarBola(nBolas);
    g_mapa.close();
}

/** Gera as bolas do Zuma */
void zuma_stl::gerarBola(int tamanho){
	int n[tamanho];
    bolas q[tamanho];
	int c = 0;
	for (;c < tamanho;c++) {
		if (c < tamanho/4)  n[c] = 0;
		else if (c < tamanho/4 * 2 ) n[c] = 1;
		else if (c < tamanho/4 * 3) n[c] = 2;
		if (c >= tamanho/4 * 3) n[c] = 3;
	}

	int a = 0,b = 0,j = 0,d = 0;
	for (c = 0;c < tamanho;c++) {
		if (n[c] == 0) a++;
		if (n[c] == 1) b++;
		if (n[c] == 2) j++;
		if (n[c] == 3) d++;
	}

	std::random_shuffle(&n[0], &n[tamanho-1]);
    for(int i = 0; i < tamanho; i++){
        q[i].type = n[i];
        inserirBola(q[i]);
    }
}

/** Verifica a bola da posição, se é igual, ativa o combo, se n, armazena */
void zuma_stl::verificarBola(bolas b, int pos){
    int p = 0;
    for(Z_it = Z_bola.begin(); Z_it != Z_bola.end(); Z_it++){
        if(p == pos){
            /// É da mesma cor ? Se sim, ativa combo !
            if(Z_it->type == b.type){
                inserirBola(b,pos);
                combo(pos);
                break;
            }else{
                inserirBola(b,pos);
                comboaux(pos);
                break;
            }
        }

        p++;
    }
}

/** Combo de bolas, auxiliar, utilizado quando as cores são diferentes */
void zuma_stl::comboaux(int pos){
    int p = 0, direita = 0;

    for(Z_it = Z_bola.begin(); Z_it != Z_bola.end(); Z_it++){
            if(p == pos){

                /** Verificar se os elementos da direita são iguais */
                for(aux = Z_it; aux != Z_bola.end(); aux++){
                    if(aux->type == Z_it->type)
                        direita++;
                    else
                        break;
                }

                if(direita >= 3){
                    Z_bola.erase(Z_it,aux--);
                }
            break;
        }
    }
}

/** Combo de bolas, verifica as bolas da posição, pra frente e pra trás e ver quantas iguais */
void zuma_stl::combo(int pos){
    int p = 0, direita = 0, esquerda = 0;
    for(Z_it = Z_bola.begin(); Z_it != Z_bola.end(); Z_it++){
            if(p == pos){
                /** Verificar se os elementos da direita são iguais */
                for(aux = Z_it; aux != Z_bola.end(); aux++){
                    if(aux->type == Z_it->type)
                        direita++;
                    else
                        break;
                }

                /** Verifica se os elementos da esquerda são iguais */
                for(aux2 = Z_it; aux2 != Z_bola.begin(); aux2--){

                   if(aux2->type == Z_it->type)
                        esquerda++;
                    else
                        break;
                }

                /** Ver se dá pra remover */
                if((esquerda + direita) - 1 >= 3){
                    Z_bola.erase(aux2,aux);
                    //aux;
                    /** Parar peças de aux pra frente */
                  /*  for(aux = Z_bola.end(); aux != Z_bola.begin(); aux++){
                        aux->parado = true;
                        aux->bWait = esquerda + direita;
                    }
                    */
                        aux = Z_bola.end();
                        aux->parado = true;
                        aux->bWait = esquerda + direita;

                   break;
                }
                break;
            }

        p++;
    }
}

/** Carrega Melhor Pontuacao do Jogo ! */
void zuma_stl::carregarPontuacao(){
    FILE *arq;

    fopen("gfx\\pontos.txt", "r");

    fscanf(arq,"%d",&melhorPontuacao);

    fclose(arq);
}

/** Salva a melhor pontucao, apenas SE sua pontuacao for maior que a melhro */
void zuma_stl::salvarPontucao(int pontos){
    if(pontos > melhorPontuacao){
        FILE *arq;

        fopen("gfx\\pontos.txt", "rw");

        fprintf(arq, "%d", pontos);
    }
}

/** Insere as Bolas na Lista */
void zuma_stl::inserirBola(bolas b){
    Z_bola.insert(Z_bola.end(),b);
}
/** Insere as Bolas na Lista em uma posição */
void zuma_stl::inserirBola(bolas b, int pos){
    int p = 0;

    for(Z_it = Z_bola.begin(); Z_it != Z_bola.end(); Z_it++){
        if(p == pos){
            b.x = c_bola[Z_it->bI].x;
            b.y = c_bola[Z_it->bI].y;
            b.bI = Z_it->bI;

            Z_bola.insert((Z_it),b);
            aux2 = Z_it;

            for(aux = Z_it; aux != Z_bola.begin()++; aux--){
                aux2--;
                aux->bI = aux2->bI;
                aux->x = aux2->x;
                aux->y = aux2->y;
                aux->bWait = aux2->bWait;
                aux->parado = aux2->parado;
            }
            break;
      }
        p++;
    }
}

/** Remove as bolas da lista */
void zuma_stl::removerBola(int pos){
    int p = 0, direita = 0, esquerda = 0;
    for(Z_it = Z_bola.begin(); Z_it != Z_bola.end(); Z_it++){

            if(p == pos){
                /** Verificar se os elementos da direita são iguais */
                for(aux = Z_it; aux != Z_bola.end()++; aux++){
                    if(aux->type == Z_it->type)
                        direita++;
                    else
                        break;
                }

                /** Verifica se os elementos da esquerda são iguais */
                for(aux2 = Z_it; aux2 != Z_bola.begin()++; aux2--){

                   if(aux2->type == Z_it->type)
                        esquerda++;
                    else
                        break;
                }

                /** Ver se dá pra remover */
                if((esquerda + direita) - 1 >= 3){
                    Z_bola.erase(aux2++,aux);
                    //aux;
                    /** Parar peças de aux pra frente */
                  /*  for(aux = Z_bola.end(); aux != Z_bola.begin(); aux++){
                        aux->parado = true;
                        aux->bWait = esquerda + direita;
                    }
                    */
                        aux = Z_bola.end();
                        aux->parado = true;
                        aux->bWait = esquerda + direita;

                   break;
                }
                break;
            }

        p++;
    }
}



/** Retorna a lista - comando de debug */
void zuma_stl::getLista(){
    for(Z_it = Z_bola.begin(); Z_it != Z_bola.end(); Z_it++){
        std::cout << Z_it->type << std::endl;
    }
}
