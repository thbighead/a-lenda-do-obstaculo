Para rodar o jogo, você precisa ter a biblioteca
gráfica SDL instalada, bem como suas extensões
SDL_Image, SDL_Mixer, e SDL_TTF.

A forma de obtê-las para o Linux está descrita
na seção "Problemas" do Manual do Usuário.

Uma vez tudo instalado, sincronize o terminal do
sistema com a localização deste diretório e digite,
sem as aspas:

"gcc -o projetoFinal projetoFinal.c -Wall `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf"