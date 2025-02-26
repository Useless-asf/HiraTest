Explication du code source HiraTest :

Petit programme de test des connaissances sur les hiraganas de base (un des alphabets japonais).

-----------------------------------------------------

L'intégralité du code est contenue dans le main.cpp
Créer en C++ via SDL3,SDL3_image et SDL3_ttf.

Le point d'entrée est SDL_AppInit()
chaque itération est gérer par SDL_AppIterate()
a chaque input/event est appelé SDL_AppEvent()
et lors de la fermeture du programme SDL_AppQuit() est appelé.

----------------------------------------------------

Le bazar de ce code provient du fait que ce dernier est juste une expérimentation et ne suivait pas de réelle structure :

SDL_AppInit : Gère l'initialisation de l'ensemble des variables (création de la fenêtre et du renderer, chargement des polices et des textures déjà crée, création des textures des hiragans)

SDL_AppIterate : Selon quelle variable (isOnAcceuil, isOnPlay et isOnLvlFinish) est "true" la fonction appelle une des fonctions suivantes render_Accueil, render_Play, render_LvlFinish. Et dans le cas du isOnPlay appelle actualise().


SDL_AppEvent : Selon quelle variable est "true" la fonction gère différement les inputs.

SDL_AppQuit : Décharge l'ensemble des variables pour éviter la moindre fuite de mémoire.


actualise : Cette fonction a comme but d'actualiser l'ensemble des variables a chaques itération du render_Play().


-----------------------------------------------------


