//(04/03/2013) Eryck Pedro: Bibliotecas.
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

//(04/03/2013) Eryck Pedro: Constantes gerais.
#define SCREEN_W 400
#define SCREEN_H 400
#define BITS 32

//(04/03/2013) Eryck Pedro: Constantes do jogo.
#define POS_X0_BOTOES_TITULO 125
#define POS_XF_BOTOES_TITULO 275
#define POS_Y0_JOGAR_TITULO 194
#define POS_YF_JOGAR_TITULO 234
#define POS_Y0_INSTRUCOES_TITULO 241
#define POS_YF_INSTRUCOES_TITULO 281
#define POS_Y0_CREDITOS_TITULO 292
#define POS_YF_CREDITOS_TITULO 332
#define POS_Y0_SAIR_TITULO 340
#define POS_YF_SAIR_TITULO 380

#define TAM_TAB 300 
#define FONTSIZE 30
#define DIM 50
#define MAX_FASES 3
#define QTD_BLOCOS 12
#define POS_BLOCO_MESTRE 4
#define POS_BOTAO_RESET QTD_BLOCOS
#define POS_BOTAO_QUIT QTD_BLOCOS+1
#define POS_BOTAO_VOLTA QTD_BLOCOS+2
#define POS_BOTAO_AJUDA QTD_BLOCOS+3

#define SEGUNDOS 1000
#define NUM_SLIDES 10
#define ERRO -1


SDL_Surface* screen;
SDL_Surface* tela;
SDL_Surface* fundo;
SDL_Surface* titulo;
SDL_Surface* bloco2V;
SDL_Surface* bloco2H;
SDL_Surface* bloco3V;
SDL_Surface* bloco3H;
SDL_Surface* blocoMestre;
SDL_Surface* reset;
SDL_Surface* quit;
SDL_Surface* volta;
SDL_Surface* ajuda;
SDL_Surface* ajudaInfo;
SDL_Surface* cred;
SDL_Surface* clear;

TTF_Font* my_font;

SDL_Rect tabuleiro = {0, 0, TAM_TAB, TAM_TAB};
SDL_Rect caixaWin = {DIM, DIM + (DIM/2), 0, 0};

int mouse_pressed = 0;
int sel;
int fase = 1;
int qtdBlocos;
int quitJogo = 0;
int prevIni, prevFim, contaMov = 0;
int comecou = 0;

typedef struct
{
  int tam, x, y, xf, yf;
  char d;
} BLOCO;

BLOCO b[QTD_BLOCOS+4];

int bMestre = POS_BLOCO_MESTRE;
int RST = POS_BOTAO_RESET;
int QT = POS_BOTAO_QUIT;
int VLT = POS_BOTAO_VOLTA;
int AJD = POS_BOTAO_AJUDA;

//(04/03/2013) Eryck Pedro: Função que discretiza o tabuleiro.
BLOCO criaBloco(int x, int y, int tam, char dir)
{
  BLOCO b;    
  b.d = dir;
  if(dir == 'H')
  {
    b.tam = tam;
    b.x = x*DIM;
    b.y = y*DIM;
    b.xf = b.x + (b.tam*DIM);
    b.yf = b.y + DIM;
  }
  if(dir == 'V')
  {
    b.tam = tam;
    b.x = x*DIM;
    b.y = y*DIM;
    b.xf = b.x + DIM;
    b.yf = b.y + (b.tam*DIM);
  }
  return b;
}

void defTransp(void)
{
  Uint32 cor_transp1 = SDL_MapRGB(screen->format, 255, 255, 255); //(04/03/2013) Eryck Pedro: Preto.
  Uint32 cor_transp2 = SDL_MapRGB(screen->format, 255, 0, 255); //(04/03/2013) Eryck Pedro: Rosa.
  
  SDL_SetColorKey(blocoMestre, SDL_SRCCOLORKEY, cor_transp1);
  SDL_SetColorKey(bloco2H, SDL_SRCCOLORKEY, cor_transp1);
  SDL_SetColorKey(bloco2V, SDL_SRCCOLORKEY, cor_transp1);
  SDL_SetColorKey(bloco3H, SDL_SRCCOLORKEY, cor_transp1);
  SDL_SetColorKey(bloco3V, SDL_SRCCOLORKEY, cor_transp1);
  SDL_SetColorKey(reset, SDL_SRCCOLORKEY, cor_transp2);
  SDL_SetColorKey(quit, SDL_SRCCOLORKEY, cor_transp2);
  SDL_SetColorKey(volta, SDL_SRCCOLORKEY, cor_transp2);
  SDL_SetColorKey(ajuda, SDL_SRCCOLORKEY, cor_transp2);
  SDL_SetColorKey(clear, SDL_SRCCOLORKEY, cor_transp2);
}

void carregaIMG(void)
{
  tela = SDL_DisplayFormat(IMG_Load("imagens/painel.png"));
  if(!tela)
  {
    printf("Não consegui carregar o painel dos botões. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  fundo = SDL_DisplayFormat(IMG_Load("imagens/tabuleiro.png"));
  if(!fundo)
  {
    printf("Não consegui carregar a imagem do tabuleiro. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  bloco2V = SDL_DisplayFormat(IMG_Load("imagens/bloco2V.png"));
  if(!bloco2V)
  {
    printf("Não consegui carregar a imagem de uma peça. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  bloco2H = SDL_DisplayFormat(IMG_Load("imagens/bloco2H.png"));
  if(!bloco2H)
  {
    printf("Não consegui carregar a imagem de uma peça. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  bloco3V = SDL_DisplayFormat(IMG_Load("imagens/bloco3V.png"));
  if(!bloco3V)
  {
    printf("Não consegui carregar a imagem de uma peça. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  bloco3H = SDL_DisplayFormat(IMG_Load("imagens/bloco3H.png"));
  if(!bloco3H)
  {
    printf("Não consegui carregar a imagem de uma peça. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  blocoMestre = SDL_DisplayFormat(IMG_Load("imagens/blocoP.png"));
  if(!blocoMestre)
  {
    printf("Não consegui carregar a imagem de uma peça. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  reset = SDL_DisplayFormat(IMG_Load("imagens/reset.png"));
  if(!reset)
  {
    printf("Não consegui carregar a imagem do botão de Reset. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  quit = SDL_DisplayFormat(IMG_Load("imagens/quit.png"));
  if(!quit)
  {
    printf("Não consegui carregar a imagem do botão de Quit. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  volta = SDL_DisplayFormat(IMG_Load("imagens/volta.png"));
  if(!volta)
  {
    printf("Não consegui carregar a imagem do botão de Volta. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  ajuda = SDL_DisplayFormat(IMG_Load("imagens/ajuda.png"));
  if(!ajuda)
  {
    printf("Não consegui carregar a imagem do botão de Ajuda. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  ajudaInfo = SDL_DisplayFormat(IMG_Load("imagens/help.png"));
  if(!ajudaInfo)
  {
    printf("Não consegui carregar a imagem das informações de Ajuda. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  clear = SDL_DisplayFormat(IMG_Load("imagens/clear.png"));
  if(!clear)
  {
    printf("Não consegui carregar a imagem de Vitória. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  
  defTransp();
}

void carregaCred(int numSlide)
{
  switch(numSlide)
  {
    case 1:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c1.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
    
    case 2:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c2.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
    
    case 3:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c3.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
    
    case 4:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c4.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
    
    case 5:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c5.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
    
    case 6:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c6.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
   
    case 7:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c7.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
   
    case 8:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c8.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
   
    case 9:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c9.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
    
    case 10:
    {
      cred= SDL_DisplayFormat(IMG_Load("imagens/creditos/c10.png"));
      if(!cred)
      {
	printf("Não consegui carregar uma imagem dos créditos. SDL error = %s.\n", SDL_GetError());
	exit(ERRO);
      }
      if(ERRO == SDL_BlitSurface(cred, NULL, screen, NULL))
      {
	printf("Não consegui dar blit numa imagem dos créditos. SDL error = %s\n", SDL_GetError());
	exit(ERRO);
      }
      SDL_Flip(screen);
      break;
    }
    
    case ERRO:
      break;
      
    default:
      break;
  }
}

void imprimeBlocos(void)
{
  int i;
  
  //(15/02/2013) Eryck Pedro: Imprimindo os blocos.
  for(i = 0; i < qtdBlocos; i++)
  {
    SDL_Rect bck = {b[i].x, b[i].y, b[i].xf, b[i].yf};
    switch (b[i].tam)
    {
      case 2:
      {
	if(b[i].d == 'V')
	  SDL_BlitSurface(bloco2V, NULL, screen, &bck);
	else
	{
	  if(i == bMestre)
	    SDL_BlitSurface(blocoMestre, NULL, screen, &bck);
	  else
	    SDL_BlitSurface(bloco2H, NULL, screen, &bck);
	}
	break;
      }
      
      case 3:
      {
	if(b[i].d == 'V')
	  SDL_BlitSurface(bloco3V, NULL, screen, &bck);
	else
	  SDL_BlitSurface(bloco3H, NULL, screen, &bck);
	break;
      }
    }
  }
  
  //(15/02/2013) Eryck Pedro: Imprimindo os botões de comando.
  SDL_Rect rst = {b[RST].x, b[RST].y, b[RST].xf, b[RST].yf};
  SDL_Rect qt = {b[QT].x, b[QT].y, b[QT].xf, b[QT].yf};
  SDL_Rect vlt = {b[VLT].x, b[VLT].y, b[VLT].xf, b[VLT].yf};
  SDL_Rect ajd = {b[AJD].x, b[AJD].y, b[AJD].xf, b[AJD].yf};
  
  SDL_BlitSurface(reset, NULL, screen, &rst);
  SDL_BlitSurface(quit, NULL, screen, &qt);
  SDL_BlitSurface(volta, NULL, screen, &vlt);
  SDL_BlitSurface(ajuda, NULL, screen, &ajd);
 
  SDL_Flip(screen);
}

void inicializaMenu(void)
{
  Mix_Chunk* hue;
  SDL_Event event;
  int tempo;
  
  //(15/02/2013) Eryck Pedro: Inicializando o SDL.
  if(ERRO == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
  {
    printf("Erro ao iniciar a SDL. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, BITS, SDL_SWSURFACE);
  if(!screen)
  {
    printf("Erro ao iniciar modo video. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  
  //(27/02/2013) Eryck Pedro: Carregando arquivo de fonte.
  if(ERRO == TTF_Init())
  {
    printf("Erro ao iniciar a biblioteca TTF. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  
  my_font = TTF_OpenFont("fonts/verdana.ttf", FONTSIZE);
  if(!my_font)
  {
    printf("Erro ao iniciar o arquivo fonte. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  //(27/02/2013) Eryck Pedro: Carregando a Mixer.
  Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096);
  
  
  if(!comecou)
  { 
    //(27/02/2013) Eryck Pedro: Carregando e tocando a música de abertura.
    hue = Mix_LoadWAV("music/hue.wav");
    if(!hue)
    {
      printf("Não consegui carregar 'hue.wav'. SDL error = %s\n", SDL_GetError());
      exit(ERRO);
    }
    
    Mix_AllocateChannels(16);
    Mix_VolumeChunk(hue, MIX_MAX_VOLUME);
    
    Mix_PlayChannel(ERRO, hue, 0);
    
    //(15/02/2013) Eryck Pedro: Carregando logotipo.
    titulo = IMG_Load("imagens/logo.png");
    if(!titulo)
    {
      printf("Não consegui carregar imagem de título. SDL error = %s.\n", SDL_GetError());
      exit(ERRO);
    }
    
    if(ERRO == SDL_BlitSurface(titulo, NULL, screen, NULL))
    {
      printf("Não consegui preencher tela de título. SDL error = %s.\n", SDL_GetError());
      exit(ERRO);
    }
    
    comecou = 1;
    SDL_Flip(screen);
    
    //(05/03/2013) Thales Nathan: Aplicando tempo de espera.
    tempo = SDL_GetTicks();
    while(SDL_PollEvent(&event) || tempo + 6*SEGUNDOS >= SDL_GetTicks())
    {
      switch(event.type)
      {
	case SDL_QUIT:
	{
	  quitJogo = 1;
	  break;
	}
	default:
	  break;
      }
      if(quitJogo)
	break;
    }
  }
  
  
  //(15/02/2013) Eryck Pedro: Carregando tela de título.
  titulo = IMG_Load("imagens/title.png");
  if(!titulo)
  {
    printf("Não consegui carregar imagem de título. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  
  if(ERRO == SDL_BlitSurface(titulo, NULL, screen, NULL))
  {
    printf("Não consegui preencher tela de título. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  
  SDL_Flip(screen);
}

void inicializaPartida(void)
{
  carregaIMG();
  
  //(15/02/2013) Eryck Pedro: Setando as configurações.
  if(ERRO == SDL_Init(SDL_INIT_VIDEO))
  {
    printf("Erro ao iniciar a SDL. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, BITS, SDL_SWSURFACE);
  if(!screen)
  {
    printf("Erro ao iniciar modo video. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  //(15/02/2013) Eryck Pedro: Carregando as imagens de fundo.
  if(ERRO == SDL_BlitSurface(tela, NULL, screen, NULL))
  {
    printf("Erro ao iniciar a imagem do painel dos botões. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }   
  if(ERRO == SDL_BlitSurface(fundo, NULL, screen, &tabuleiro))
  {
    printf("Erro ao iniciar a imagem do tabuleiro. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  //(15/02/2013) Eryck Pedro: Criação de fases.
  switch(fase)
  {
    case 1:
    {
      qtdBlocos = QTD_BLOCOS-4;
      b[0] = criaBloco(0,0,2,'V');
      b[1] = criaBloco(1,0,2,'V');
      b[2] = criaBloco(3,0,3,'V');
      b[3] = criaBloco(4,0,3,'V');
      b[4] = criaBloco(0,2,2,'H');
      b[5] = criaBloco(5,0,3,'V');
      b[6] = criaBloco(3,3,3,'H');
      b[7] = criaBloco(0,4,2,'H');
      break;
    }
    case 2:
    {
      qtdBlocos = QTD_BLOCOS-1;
      b[0] = criaBloco(0,0,3,'H');
      b[1] = criaBloco(3,0,3,'V');
      b[2] = criaBloco(0,1,3,'V');
      b[3] = criaBloco(1,1,2,'H');
      b[4] = criaBloco(1,2,2,'H');
      b[5] = criaBloco(4,1,2,'H');
      b[6] = criaBloco(5,2,3,'V');
      b[7] = criaBloco(2,3,2,'V');
      b[8] = criaBloco(3,3,2,'H');
      b[9] = criaBloco(4,4,2,'V');
      b[10] = criaBloco(0,5,3,'H');
      break;
    }
    case 3:
    {
      qtdBlocos = QTD_BLOCOS;
      b[0] = criaBloco(0,1,2,'V');
      b[1] = criaBloco(1,1,2,'V');
      b[2] = criaBloco(2,0,2,'V');
      b[3] = criaBloco(3,0,3,'H');
      b[4] = criaBloco(2,2,2,'H');
      b[5] = criaBloco(0,3,3,'H');
      b[6] = criaBloco(4,2,2,'V');
      b[7] = criaBloco(5,1,3,'V');
      b[8] = criaBloco(0,4,2,'V');
      b[9] = criaBloco(1,4,2,'V');
      b[10] = criaBloco(2,4,3,'H');
      b[11] = criaBloco(4,5,2,'H');
    }
  }
  
  //(15/02/2013) Eryck Pedro: Cria os botões de comando.
  b[RST] = criaBloco(7,6,1,'H');
  b[QT] = criaBloco(7,7,1,'H');
  b[VLT] = criaBloco(6,6,1,'H');
  b[AJD] = criaBloco(6,7,1,'H');
  
  imprimeBlocos();
}

void atualiza(void) //(04/03/2013) Eryck Pedro: Função que "limpa" a tela.
{
  if(ERRO == SDL_BlitSurface(fundo, NULL, screen, &tabuleiro))
  {
    printf("Erro ao iniciar tabuleiro. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
 
  imprimeBlocos();
}

int reconheceBotao(SDL_Event event)
{
  if((event.motion.x>POS_X0_BOTOES_TITULO) && (event.motion.x<POS_XF_BOTOES_TITULO) &&
    (event.motion.y>POS_Y0_JOGAR_TITULO) && (event.motion.y<POS_YF_JOGAR_TITULO))
    return 1;
  
  if((event.motion.x>POS_X0_BOTOES_TITULO) && (event.motion.x<POS_XF_BOTOES_TITULO) &&
    (event.motion.y>POS_Y0_INSTRUCOES_TITULO) && (event.motion.y<POS_YF_INSTRUCOES_TITULO))
    return 2;
  
  if((event.motion.x>POS_X0_BOTOES_TITULO) && (event.motion.x<POS_XF_BOTOES_TITULO) &&
    (event.motion.y>POS_Y0_CREDITOS_TITULO) && (event.motion.y<POS_YF_CREDITOS_TITULO))
    return 3;
  
  if((event.motion.x>POS_X0_BOTOES_TITULO) && (event.motion.x<POS_XF_BOTOES_TITULO) &&
    (event.motion.y>POS_Y0_SAIR_TITULO) && (event.motion.y<POS_YF_SAIR_TITULO))
    return 4;
  
  return 0;
}

int dectReset(void)
{
  if(RST == sel)
  {
    contaMov = 0;
    return 1;
  }
  else
    return 0;
}

int dectQuit(void)
{
  if(QT == sel)
  {
    contaMov = 0;
    return 1;
  }
  else
    return 0;
}

int dectVolta(void)
{
  if(VLT == sel)
    return 1;
  else
    return 0;
}

int dectAjuda(void)
{
  if(AJD == sel)
    return 1;
  else
    return 0;
}

int dectWin(void)
{
  if(b[bMestre].xf >= TAM_TAB)
    return 1;
  else
    return 0;
}

int dectColisao(BLOCO rect, BLOCO rect2)
{ 
  if(rect.y >= rect2.yf)
    return 0;
  if(rect.x >= rect2.xf)
    return 0;
  if(rect.yf <= rect2.y)
    return 0;
  if(rect.xf <= rect2.x)
    return 0;
  
  return 1;
}

void mostraCred(void)
{
  int numS = 1, quitCred = 0;
  int tempo;
  
  SDL_Event event;
  
  Mix_Music* credits = Mix_LoadMUS("music/credits.mp3");
  if(!credits)
  {
    printf("Não consegui carregar 'credits.mp3'. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
 
  Mix_PlayMusic(credits, 0);
  
  while(!quitCred)
  {
    if(numS != ERRO)
    {
      carregaCred(numS);
      numS++;
    }
    
    //(05/03/2013) Thales Nathan: Aplicando tempo de espera.
    tempo = SDL_GetTicks();
    while(SDL_PollEvent(&event) || tempo + 11*SEGUNDOS >= SDL_GetTicks())
    {
      switch(event.type)
      {
	case SDL_QUIT:
	{
	  quitJogo = quitCred = 1;
	  break;
	}
	case SDL_KEYDOWN:
	{
	  if(event.key.keysym.sym == SDLK_q)
	  {
	    quitCred = 1;
	    numS = ERRO;
	    Mix_HaltMusic();
	    break;
	  }
	}
	default:
	  break;
      }
      if(quitCred)
	break;
    }
    if(numS > NUM_SLIDES)
    {
      Mix_HaltMusic();
      break;
    }
  }
}

void mostraInstru(void) //(04/03/2013) Eryck Pedro: Instruções do menu principal.
{
  SDL_Event event;
  int pagina = 1;
  int quitInstru = 0;
  
  carregaIMG();
  
  ajudaInfo = SDL_DisplayFormat(IMG_Load("imagens/instr1.png"));
  if(!ajudaInfo)
  {
    printf("Não consegui carregar a imagem das informações de Ajuda. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  
  if(ERRO == SDL_BlitSurface(ajudaInfo, NULL, screen, NULL))
  {
    printf("Não consegui preencher a imagem do botão de Ajuda. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  SDL_Flip(screen);
  
  while(!quitInstru)
  {
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
	case SDL_QUIT:
	{
	  quitJogo = quitInstru = 1;
	  break;
	}
	case SDL_KEYDOWN:
	{
	  if(event.key.keysym.sym == SDLK_RIGHT && pagina == 1)
	  {
	    ajudaInfo = SDL_DisplayFormat(IMG_Load("imagens/instr2.png"));
	    if(!ajudaInfo)
	    {
	      printf("Não consegui carregar a imagem das informações de Ajuda. SDL error = %s.\n", SDL_GetError());
	      exit(ERRO);
	    }
	    
	    if(ERRO == SDL_BlitSurface(ajudaInfo, NULL, screen, NULL))
	    {
	      printf("Não consegui preencher a imagem do botão de Ajuda. SDL error = %s.\n", SDL_GetError());
	      exit(ERRO);
	    }
	    SDL_Flip(screen);
	    pagina = 2;
	  }
	  
	  if(event.key.keysym.sym == SDLK_LEFT && pagina == 2)
	  {
	    ajudaInfo = SDL_DisplayFormat(IMG_Load("imagens/instr1.png"));
	    if(!ajudaInfo)
	    {
	      printf("Não consegui carregar a imagem das informações de Ajuda. SDL error = %s.\n", SDL_GetError());
	      exit(ERRO);
	    }
	    
	    if(ERRO == SDL_BlitSurface(ajudaInfo, NULL, screen, NULL))
	    {
	      printf("Não consegui preencher a imagem do botão de Ajuda. SDL error = %s.\n", SDL_GetError());
	      exit(ERRO);
	    }
	    SDL_Flip(screen);
	    pagina = 1;
	  }
	  
	  if(event.key.keysym.sym == SDLK_q)
	  {
	    quitInstru = 1;
	    break;
	  }
	  break;
	}
	default:
	  break;
      }
    }
  }
}

void mostraAjuda(void) //(04/03/2013) Eryck Pedro: Instruções do botão de Ajuda no jogo.
{
  SDL_Event event;
  int quitAjuda = 0;
  
  carregaIMG();
  
  if(ERRO == SDL_BlitSurface(ajudaInfo, NULL, screen, NULL))
  {
    printf("Não consegui preencher a imagem do botão de Ajuda. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  SDL_Flip(screen);
  
  while(!quitAjuda)
  {
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
	case SDL_QUIT:
	{
	  quitJogo = quitAjuda = 1;
	  break;
	}
	case SDL_KEYDOWN:
	{
	  if(event.key.keysym.sym == SDLK_q)
	  {
	    quitAjuda = 1;
	    break;
	  }
	  break;
	}
	default:
	  break;
      }
    }
  }
}

int dectStart(void) //(04/03/2013) Eryck Pedro: Função do menu principal.
{
  int botao;
  SDL_Event event;
  Mix_Music* title = Mix_LoadMUS("music/title.mp3");
  if(!title)
  {
    printf("Não consegui carregar 'title.mp3'. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  Mix_PlayMusic(title, ERRO);
  
  while(!quitJogo)
  {
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
	case SDL_QUIT:
	{
	  quitJogo = 1;
	  break;
	}
	case SDL_KEYDOWN:
	{
	  if(event.key.keysym.sym == SDLK_ESCAPE)
	  {
	    quitJogo = 1;
	    break;
	  }
	}
	case SDL_MOUSEBUTTONDOWN:
	{
	  botao = reconheceBotao(event);
	  switch(botao)
	  {
	    case 1:
	    {
	      Mix_HaltMusic();
	      return 1;
	    }
	      
	    case 2:
	    {
	      mostraInstru();
	      inicializaMenu();
	      break;
	    }
	    case 3:
	    {
	      Mix_FreeMusic(title);
	      
	      title = Mix_LoadMUS("music/title.mp3");
	      if(!title)
	      {
		printf("Não consegui carregar 'title.mp3'. SDL error = %s\n", SDL_GetError());
		exit(ERRO);
	      }
	      
	      mostraCred();
	      Mix_PlayMusic(title, ERRO);
	      inicializaMenu();
	      break;
	    }
	    case 4:
	    {
	      quitJogo = 1;
	      break;
	    }
	    default:
	      break;
	  }
	  break;
	}
	default:
	  break;
      }
    }
  }
  return 0;
}

//(04/03/2013) Eryck Pedro: Imprime na tela do jogo o número da fase e os movimentos.
void desenhaTexto(char* texto, int xMov, int yMov, char* texto2, int xFas, int yFas)
{
  SDL_Color cor = {255, 255, 255};
  SDL_Surface* src1 = TTF_RenderText_Blended(my_font, texto, cor);
  SDL_Surface* src2 = TTF_RenderText_Blended(my_font, texto2, cor);
  SDL_Rect dst_rect = {xMov, yMov, 0, 0};
  SDL_Rect dst_rect2 = {xFas, yFas, 0, 0};
  
  if(ERRO == SDL_BlitSurface(tela, NULL, screen, NULL))
  {
    printf("Erro ao iniciar a imagem do painel dos botões. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  SDL_BlitSurface(src1, NULL, screen, &dst_rect);
  SDL_BlitSurface(src2, NULL, screen, &dst_rect2);
  
  SDL_FreeSurface(src1);
  SDL_FreeSurface(src2);
}

void imprimeNumero(void)
{
  int centralizador;
  char numMov[BITS], numFase[BITS];
  
  //Transformando as variáveis int em char*
  snprintf(numMov, BITS, "%d", contaMov);
  snprintf(numFase, BITS, "%d", fase);
  
  
  //Tática para centralizar o contador de movimentos quando tiver 2 algarismos
  if(contaMov >= 0 && contaMov < 10)
    centralizador = 5;
  else
    centralizador = 15;
  
  desenhaTexto(numMov, (TAM_TAB/2)-centralizador, TAM_TAB+DIM-5, numFase, TAM_TAB+DIM-5, DIM-5);
}

void passaFase(void)
{
  Mix_Chunk* fanfarra = Mix_LoadWAV("music/fanfarra.wav");
  if(!fanfarra)
  {
    printf("Não consegui carregar 'fanfarra.wav'. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  Mix_VolumeChunk(fanfarra, MIX_MAX_VOLUME);
  Mix_AllocateChannels(16);
  
  atualiza();
  
  if(ERRO == SDL_BlitSurface(clear, NULL, screen, &caixaWin))
  {
    printf("Não consegui dar blit. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
 
  SDL_Flip(screen);
  
  Mix_PlayChannel(ERRO, fanfarra, 0);
  
  sel = prevIni = prevFim = ERRO;
  contaMov = 0;
  
 
  SDL_Delay(3*SEGUNDOS);
  
  Mix_FreeChunk(fanfarra);
}

void congratula(void)
{
  SDL_Surface* msgCong;
  Mix_Chunk* cong = Mix_LoadWAV("music/congratz.wav");
  if(!cong)
  {
    printf("Não consegui carregar 'congratz.wav'. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  Mix_VolumeChunk(cong, MIX_MAX_VOLUME);
  Mix_AllocateChannels(16);
  
  atualiza();
  
  msgCong = SDL_DisplayFormat(IMG_Load("imagens/clearfinal.png"));
  SDL_SetColorKey(msgCong, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 0, 255));
  
  if(!msgCong)
  {
    printf("Não consegui carregar a imagem de congratulações. SDL error = %s\n", SDL_GetError());
    exit(ERRO);
  }
  
  if(ERRO == SDL_BlitSurface(msgCong, NULL, screen, &caixaWin))
  {
    printf("Não consegui dar blit. SDL error = %s.\n", SDL_GetError());
    exit(ERRO);
  }
  
  SDL_Flip(screen);
  
  Mix_PlayChannel(ERRO, cong, 0);
 
  SDL_Delay(5*SEGUNDOS);
  Mix_FreeChunk(cong);
}

//(04/03/2013) Eryck Pedro: Discretiza os movimentos dos blocos.
void discretMove(SDL_Event event)
{
  if (sel >= 0 && sel < qtdBlocos)
  {
    if (b[sel].d == 'H')
    {
      if(b[sel].x % DIM >= (DIM/2))
      {
	b[sel].x += DIM-b[sel].x % DIM;
	b[sel].xf += DIM-b[sel].xf % DIM;
      }
      else
      {
	b[sel].x -= b[sel].x % DIM;
	b[sel].xf -= b[sel].xf % DIM;
      }
      
      if(prevIni != b[sel].x)
	contaMov++;
    }
    else
    {
      if(b[sel].y % DIM >= (DIM/2))
      {
	b[sel].y += DIM-b[sel].y % DIM;
	b[sel].yf += DIM-b[sel].yf % DIM;
      }
      else
      {
	b[sel].y -= b[sel].y % DIM;
	b[sel].yf -= b[sel].yf % DIM;
      }
      
      if(prevIni != b[sel].y)
	contaMov++;
    }
  }
}

void reconheceBloco(SDL_Event event)
{
  int i;
  for(i = 0; i < QTD_BLOCOS+4; i++)
  {
    if((event.motion.x > b[i].x) && (event.motion.x < b[i].xf) &&
      (event.motion.y > b[i].y) && (event.motion.y < b[i].yf))
    {
      sel = i;
      break;
    }
    else
      sel = ERRO;
  }
}

//(04/03/2013) Eryck Pedro: Função que comanda todos os movimentos dos blocos.
void movimenta_figura(SDL_Event event)
{
  int i;
  if(sel >= 0 && sel <= qtdBlocos)
  {
    //(15/02/2013) Eryck Pedro: Testes para movimentar blocos horizontais.
    if(b[sel].d == 'H')
    {
      b[sel].x += event.motion.xrel;
      b[sel].xf += event.motion.xrel;  
      
      for(i = 0; i<qtdBlocos; i++)
	if(dectColisao(b[sel],b[i]) && (sel != i))
	{
	  b[sel].x -= event.motion.xrel;
	  b[sel].xf -= event.motion.xrel;
	}
    }
    //(15/02/2013) Eryck Pedro: Testes para movimentar blocos verticais.
    if(b[sel].d == 'V')
    {
      b[sel].y += event.motion.yrel;
      b[sel].yf += event.motion.yrel;
      
      for(i = 0; i<qtdBlocos; i++)
	if(dectColisao(b[sel],b[i]) && (sel != i))
	{
	  b[sel].y -= event.motion.yrel;
	  b[sel].yf -= event.motion.yrel;
	}
    }
    
    //(15/02/2013) Eryck Pedro: Testes para não deixar o bloco sair da tela.
    if(b[sel].x < 0)
    {
      b[sel].x = 0;
      b[sel].xf = b[sel].x + (b[sel].tam*DIM);
    }
    if(b[sel].xf > TAM_TAB)
    {
      b[sel].xf = TAM_TAB;
      b[sel].x = b[sel].xf - (b[sel].tam*DIM);
    }
    if(b[sel].y < 0)
    {
      b[sel].y = 0;
      b[sel].yf = b[sel].y + (b[sel].tam*DIM);
    }
    if(b[sel].yf > TAM_TAB)
    {
      b[sel].yf = TAM_TAB;
      b[sel].y = b[sel].yf - (b[sel].tam*DIM);
    }
  }
}

void joga(void)
{
  SDL_Event event;
  int quitPartida = 0;
  int cheat;
  int prevSel;
  
  while(!quitJogo)
  {
    quitPartida = 0;
    fase = 1;
    cheat = 0;
    inicializaMenu();
    if(dectStart())
    {
      inicializaPartida();
      
      while(!quitPartida)
      {
	if(cheat == 3)
	{
	  cheat = 0;
	  if(fase <= MAX_FASES)
	  {
	    fase++;
	    passaFase();
	    inicializaPartida();
	  }
	  if(fase > MAX_FASES)
	  {
	    quitPartida = 1;
	    comecou = 0;
	    congratula();
	    mostraCred();
	    break;
	  }
	}
	while(SDL_PollEvent(&event))
	{
	  switch(event.type)
	  {
	    case SDL_QUIT:
	    {
	      quitJogo = quitPartida = 1;
	      break;
	    }
	    case SDL_KEYDOWN:
	    {
	      if(event.key.keysym.sym == SDLK_ESCAPE)
	      {
		quitJogo = quitPartida = 1;
		break;
	      }
	      
	      //(04/03/2013) Eryck Pedro: Verificador da sequência de "cheat".
	      if(cheat == 0 && (event.key.keysym.sym == SDLK_h))
	      {
		cheat++;
		break;
	      }
	      
	      if(cheat == 1 && (event.key.keysym.sym == SDLK_u))
	      {
		cheat++;
		break;
	      }
	      else if(cheat == 1 && (event.key.keysym.sym != SDLK_u))
	      {
		cheat = 0;
		break;
	      }
	      
	      if(cheat == 2 && (event.key.keysym.sym == SDLK_e))
	      {
		cheat++;
		break;
	      }
	      else if(cheat == 2 && (event.key.keysym.sym != SDLK_e))
	      {
		cheat = 0;
		break;
	      }
	    }
	    case SDL_MOUSEMOTION:
	    {
	      if(mouse_pressed)
	      {
		movimenta_figura(event);
		
		if(dectWin())
		{
		  cheat = 0;
		  contaMov++;
		  imprimeNumero();
		  passaFase();
		  fase++;
		  inicializaPartida();
		}
		if(fase > MAX_FASES)
		{
		  quitPartida = 1;
		  comecou = 0;
		  congratula();
		  mostraCred();
		  break;
		}
	      }
	      break;
	    }
	    case SDL_MOUSEBUTTONDOWN:
	    {
	      if(event.button.button == SDL_BUTTON_LEFT)
	      {
		mouse_pressed = 1;
		reconheceBloco(event);
		
		//(04/03/2013) Eryck Pedro: Para guardar o último movimento.
		if(sel >= 0 && sel <= qtdBlocos)
		{
		  prevSel = sel;
		  if(b[sel].d == 'H')
		  {
		    prevIni = b[sel].x;
		    prevFim = b[sel].xf;
		  }
		  else if(b[sel].d == 'V')
		  {
		    prevIni = b[sel].y;
		    prevFim = b[sel].yf;
		  }
		}
		
		//(04/03/2013) Eryck Pedro: Para desfazer o último movimento.
		if(dectVolta() && contaMov > 0)
		{
		  if(b[prevSel].d == 'H')
		  {
		    b[prevSel].x = prevIni;
		    b[prevSel].xf = prevFim;
		  }
		  else if(b[prevSel].d == 'V')
		  {
		    b[prevSel].y = prevIni;
		    b[prevSel].yf = prevFim;
		  }
		  prevSel = ERRO;
		  break;
		}
		
		if(dectReset())
		  inicializaPartida();
		
		if(dectQuit())
		{
		  quitPartida = 1;
		  break;
		}
		
		if(dectAjuda())
		{
		  mostraAjuda();
		  break;
		}
	      }  
	      break;
	    }
	    case SDL_MOUSEBUTTONUP:
	    {
	      if(event.button.button == SDL_BUTTON_LEFT)
	      {
		mouse_pressed = 0;
		discretMove(event);
		
		if(dectWin())
		{
		  cheat = 0;
		  imprimeNumero();
		  passaFase();
		  fase++;
		  inicializaPartida();
		}
		if(fase > MAX_FASES)
		{
		  quitPartida = 1;
		  comecou = 0;
		  congratula();
		  mostraCred();
		  break;
		}
	      }
	      break;
	    }
	    default:
	      break;
	  }
	}
	atualiza();
	imprimeNumero();
      }
    }
  }
}

void termina(void)
{
  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}