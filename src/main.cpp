#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <random>



///         TABLEAU DES HIRAGANAS
struct HiraganaDict {
    const char* key;
    const std::string value;
};

HiraganaDict hiragana_table[] = {
    {"あ", "a"}, {"い", "i"}, {"う", "u"}, {"え", "e"}, {"お", "o"},
    {"か", "ka"}, {"き", "ki"}, {"く", "ku"}, {"け", "ke"}, {"こ", "ko"},
    {"さ", "sa"}, {"し", "shi"}, {"す", "su"}, {"せ", "se"}, {"そ", "so"},
    {"た", "ta"}, {"ち", "chi"}, {"つ", "tsu"}, {"て", "te"}, {"と", "to"},
    {"な", "na"}, {"に", "ni"}, {"ぬ", "nu"}, {"ね", "ne"}, {"の", "no"},
    {"は", "ha"}, {"ひ", "hi"}, {"ふ", "fu"}, {"へ", "he"}, {"ほ", "ho"},
    {"ま", "ma"}, {"み", "mi"}, {"む", "mu"}, {"め", "me"}, {"も", "mo"},
    {"や", "ya"}, {"ゆ", "yu"}, {"よ", "yo"},
    {"ら", "ra"}, {"り", "ri"}, {"る", "ru"}, {"れ", "re"}, {"ろ", "ro"},
    {"わ", "wa"}, {"を", "wo"}, {"ん", "n"}
};

const int HIRAGANA_TABLE_SIZE = sizeof(hiragana_table) / sizeof(HiraganaDict);
std::vector<int> num_tab;


///         VARIABLES GLOBALES

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;
static TTF_Font* font = NULL;
static TTF_Font* same_font_but_bigger = NULL;

const double M_PI = 3.141592653589793;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

bool isOnAcceuil = true;
bool isOnPlay = false;
bool isOnLvlFinish = false;

SDL_Color color = { 255, 255, 255, SDL_ALPHA_OPAQUE };



///         TEXTURE DE L'ACCEUIL

const int hello_text_lenghtJ = 8;
SDL_Texture* hello_textureJ[hello_text_lenghtJ];
const char* hello_textJ[] = { "ひ", "ら", "が", "な", "ぞ", "学", "ぶ", "!"};

const int hello_text_lenghtF = 15;
SDL_Texture* hello_textureF[hello_text_lenghtF];
const char* hello_textF[] = { "H", "i", "r", "a", "g", "a", "n", "a", " ", "L", "e", "a", "r", "n", "!"};

SDL_Texture* play_textureF;
SDL_Texture* play_textureJ;
const char* play_textF = "Play !";
const char* play_textJ = "プレイ";

SDL_Texture* play_halo;
SDL_FRect halo_box;

///         FONCTION DE L'ACCEUIL

void render_JHello_Text();
void render_FHello_Text();
void render_Acceuil();

bool mouse_on_play = false;
SDL_FRect play_button_box;


///         FONCTION DE LvlFinish
void render_LvlFinish();

///         VARIABLES DE LvlFinish
SDL_Texture* felicitations_textureh1;
SDL_Texture* felicitations_textureh2;


///         VARIABLES DE JEU
int actual_hir = 0;
int error_count = 0;

SDL_Texture* hiragana_textures[HIRAGANA_TABLE_SIZE];
SDL_FRect hiragana_box;

SDL_Texture* letter_bar;
SDL_Texture* cross_texture;

std::stringstream actual_string;

int anim_lenght = 20;
int anim_time = 0;
int start_anim = 10;
bool onAnim = false;

bool onSuppr = false;

///         FONCTIONs DE JEU

void render_Play();
void actualise();

int counterValid = 0; bool onValid = false;



SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_Surface* text;

    
    if (!SDL_CreateWindowAndRenderer("HiraLearn", 1000, 800, NULL, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    
    font = TTF_OpenFont("./ressource/KosugiMaru-Regular.ttf", 32);
    if (!font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    same_font_but_bigger = TTF_OpenFont("./ressource/KosugiMaru-Regular.ttf", 96);
    if (!same_font_but_bigger) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //      Titre en Japonais
    for (int i = 0; i < hello_text_lenghtJ; i++)
    {
        text = TTF_RenderText_Blended(font, hello_textJ[i], 0, color);
        if (text) {
            hello_textureJ[i] = SDL_CreateTextureFromSurface(renderer, text);
            SDL_DestroySurface(text);
        }
        if (!hello_textureJ[i]) {
            SDL_Log("Couldn't create text: %s\n", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }
    //      Titre en Anglais
    for (int i = 0; i < hello_text_lenghtF; i++)
    {
        text = TTF_RenderText_Blended(font, hello_textF[i], 0, color);
        if (text) {
            hello_textureF[i] = SDL_CreateTextureFromSurface(renderer, text);
            SDL_DestroySurface(text);
        }
        if (!hello_textureF[i]) {
            SDL_Log("Couldn't create text: %s\n", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }
    //      Bouton play en Anglais
    text = TTF_RenderText_Blended(font, play_textF, 0, color);
    if (text) {
        play_textureF = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
    else {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    //      Bouton play en japonais
    text = TTF_RenderText_Blended(font, play_textJ, 0, color);
    if (text) {
        play_textureJ = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
    else {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    play_button_box = { float(SCREEN_WIDTH / 2 - play_textureF->w / 2), float(3 * SCREEN_HEIGHT / 4 - play_textureF->h / 2), float(play_textureF->w), float(play_textureF->h) };

    //      Halo lumineux
    play_halo = IMG_LoadTexture(renderer, "./ressource/halo.png");
    if (!play_halo)
    {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    halo_box = { float(SCREEN_WIDTH / 2 - play_halo->w), float(3 * SCREEN_HEIGHT / 4 - play_halo->h), float(play_halo->w*2), float(play_halo->h*2) };

    //      Premier hiragana
    for (int i = 0; i < HIRAGANA_TABLE_SIZE; i++)
    {
        text = TTF_RenderText_Blended(same_font_but_bigger, hiragana_table[i].key, 0, color);
        if (text) {
            if (i == 26) { SDL_SetWindowIcon(window, text); }
            hiragana_textures[i] = SDL_CreateTextureFromSurface(renderer, text);
            SDL_DestroySurface(text);
        }
        if (!hiragana_textures[i]) {
            SDL_Log("Couldn't create text: %s\n", SDL_GetError());
            return SDL_APP_FAILURE;
        }
    }
    hiragana_box = { float(SCREEN_WIDTH / 2 - hiragana_textures[0]->w/2), float(SCREEN_HEIGHT / 2 - hiragana_textures[0]->h/2), float(hiragana_textures[0]->w), float(hiragana_textures[0]->h) };

    //      Cross texture

    text = TTF_RenderText_Blended(same_font_but_bigger, "X", 0, color);
    cross_texture = SDL_CreateTextureFromSurface(renderer, text);
    SDL_DestroySurface(text);

    //      texture bar
    letter_bar = IMG_LoadTexture(renderer, "./ressource/letter bar.png");
    if (!letter_bar)
    {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    //      Malange des hiraganas
    for (int i = 0; i < HIRAGANA_TABLE_SIZE; i++) {
        num_tab.push_back(i);
    }
    std::random_device rd;
    std::mt19937 f(rd());
    std::shuffle(num_tab.begin(), num_tab.end(), f);

    //      Lvlfinish textures
    text = TTF_RenderText_Blended(same_font_but_bigger, "Bravo, vous venez de finir !", 0, color);
    if (text) {
        felicitations_textureh1 = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    } 
    if (!felicitations_textureh1) {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    text = TTF_RenderText_Blended(font, "Vous avez un total d'erreurs de :", 0, color);
    if (text) {
        felicitations_textureh2 = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
    if (!felicitations_textureh1) {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if (event->type == SDL_EVENT_MOUSE_MOTION && isOnAcceuil) {
        float mx = event->motion.x, my = event->motion.y, px1 = play_button_box.x, px2 = play_button_box.x + play_button_box.w, py1 = play_button_box.y, py2 = play_button_box.y + play_button_box.h;

        if (mx >= px1 && mx <= px2 && my >= py1 && my <= py2){
            mouse_on_play = true;
        }
        else {
            mouse_on_play = false;
        }
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && isOnAcceuil) {
        float mx = event->button.x, my = event->button.y, px1 = play_button_box.x, px2 = play_button_box.x + play_button_box.w, py1 = play_button_box.y, py2 = play_button_box.y + play_button_box.h;

        if (mx >= px1 && mx <= px2 && my >= py1 && my <= py2) {
            isOnAcceuil = false;
            isOnPlay = true;
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT));
        }
    }
    if (event->type == SDL_EVENT_KEY_DOWN && isOnPlay) {
        int key = event->key.key;
        if (key >= 97 && key <= 122)
        {
            actual_string << char(key);
        }
        if (event->key.key == 8 && !event->key.repeat && !onAnim && counterValid==0) {
            std::string tmp = actual_string.str();
            if (!tmp.empty()) {
                tmp.pop_back();
                actual_string.str("");
                actual_string << tmp;
            }
        }
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (isOnAcceuil) {
        render_Acceuil();
    }
    else if (isOnPlay) {
        actualise(); 
        render_Play();
        onValid ? counterValid++ : NULL;
    }
    else if (isOnLvlFinish) {
        render_LvlFinish();
    }


    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    TTF_CloseFont(same_font_but_bigger);
    if (font) {
        TTF_CloseFont(font);

    }
    for (int i = 0; i < hello_text_lenghtJ; i++) {
        SDL_DestroyTexture(hello_textureJ[i]);
    }
    for (int i = 0; i < hello_text_lenghtF; i++) {
        SDL_DestroyTexture(hello_textureF[i]);
    }
    for (int i = 0; i < HIRAGANA_TABLE_SIZE; i++) {
        SDL_DestroyTexture(hiragana_textures[i]);
    }
    SDL_DestroyTexture(play_textureF);
    SDL_DestroyTexture(play_textureJ);
    SDL_DestroyTexture(cross_texture);
    SDL_DestroyTexture(play_halo);
    SDL_DestroyTexture(letter_bar);
    SDL_DestroyTexture(felicitations_textureh1);
    SDL_DestroyTexture(felicitations_textureh2);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
}





///         DEFINITION FONCTION ACCEUIL

void render_Acceuil() {
    render_FHello_Text();
    render_JHello_Text();

    SDL_RenderTexture(renderer, mouse_on_play?play_textureJ:play_textureF, NULL, &play_button_box);
    mouse_on_play ? SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER)) : SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT));
    mouse_on_play ? SDL_RenderTexture(renderer, play_halo, NULL, &halo_box): NULL;
}

void render_JHello_Text() {
    double hello_step = M_PI / 10;
    double hello_start = (3 * M_PI / 2) - (hello_step * (hello_text_lenghtJ - 1) / 2);

    int mx = SCREEN_WIDTH / 2, my = SCREEN_HEIGHT / 2;

    for (int i = 0; i < hello_text_lenghtJ; i++) {

        int w = hello_textureJ[i]->w * 1.5, h = hello_textureJ[i]->h * 1.5;

        double angle = hello_start + i * hello_step; // Réduire l'angle (M_PI/16) et décaler pour être centré
        int x = mx + 200 * cos(angle) - w / 2;
        int y = my + 200 * sin(angle) - h / 2;

        SDL_FRect tmp = { float(x), float(y), float(w), float(h) };

        SDL_RenderTexture(renderer, hello_textureJ[i], NULL, &tmp);
    }
}

void render_FHello_Text() {
    double hello_step = M_PI / 20;
    double hello_start = (3 * M_PI / 2) - (hello_step * (hello_text_lenghtF - 1.5) / 2);

    int mx = SCREEN_WIDTH / 2, my = SCREEN_HEIGHT / 2;

    for (int i = 0; i < hello_text_lenghtF; i++) {

        int w = hello_textureF[i]->w, h = hello_textureF[i]->h;

        double angle = hello_start + i * hello_step; // Réduire l'angle (M_PI/16) et décaler pour être centré
        int x = mx + 150 * cos(angle) - w / 2;
        int y = my + 150 * sin(angle) - h / 2;

        SDL_FRect tmp = { float(x), float(y), float(w), float(h) };

        SDL_RenderTexture(renderer, hello_textureF[i], NULL, &tmp);
    }
}


///         DEFINITION FONCTION JEU
int counter = 0, dx=2, dy=2;
void render_Play() {

    Uint64 startTick = SDL_GetTicks();

    if(isOnPlay)
    {   
        if (actual_hir >= num_tab.size()) { NULL; }
        else if (hiragana_table[num_tab[actual_hir]].value.length() == 1) {
            SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 50), float(SCREEN_HEIGHT - 100), float(100), float(40) };
            SDL_RenderTexture(renderer, letter_bar, NULL, &tmp);

            if (actual_string.str().length() > 0) {
                SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, &actual_string.str()[0], 0, color)), NULL, &tmp);
            }
        }
        else if (hiragana_table[num_tab[actual_hir]].value.length() == 2) {
            SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 100), float(SCREEN_HEIGHT - 100), float(100), float(40) };
            SDL_RenderTexture(renderer, letter_bar, NULL, &tmp);
            tmp = { float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT - 100), float(100), float(40) };
            SDL_RenderTexture(renderer, letter_bar, NULL, &tmp);

            if (actual_string.str().length() == 1) {
                SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 100 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, &actual_string.str()[0], 0, color)), NULL, &tmp);
            }
            else if (actual_string.str().length() == 2) {
                SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 100 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                const char tmp1[] = { actual_string.str().c_str()[0], '\0' };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, tmp1, 0, color)), NULL, &tmp);
                tmp = { float(SCREEN_WIDTH / 2 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                const char tmp2[] = { actual_string.str().c_str()[1], '\0' };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, tmp2, 0, color)), NULL, &tmp);
            }
        }
        else if (hiragana_table[num_tab[actual_hir]].value.length() == 3) {
            SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 150), float(SCREEN_HEIGHT - 100), float(100), float(40) };
            SDL_RenderTexture(renderer, letter_bar, NULL, &tmp);
            tmp = { float(SCREEN_WIDTH / 2 - 50), float(SCREEN_HEIGHT - 100), float(100), float(40) };
            SDL_RenderTexture(renderer, letter_bar, NULL, &tmp);
            tmp = { float(SCREEN_WIDTH / 2 + 50), float(SCREEN_HEIGHT - 100), float(100), float(40) };
            SDL_RenderTexture(renderer, letter_bar, NULL, &tmp);

            if (actual_string.str().length() == 1) {
                SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 150 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, &actual_string.str()[0], 0, color)), NULL, &tmp);
            }
            else if (actual_string.str().length() == 2) {
                SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 150 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                const char tmp1[] = { actual_string.str().c_str()[0], '\0' };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, tmp1, 0, color)), NULL, &tmp);
                tmp = { float(SCREEN_WIDTH / 2 - 50 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                const char tmp2[] = { actual_string.str().c_str()[1], '\0' };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, tmp2, 0, color)), NULL, &tmp);
            }
            else if (actual_string.str().length() == 3) {
                SDL_FRect tmp = { float(SCREEN_WIDTH / 2 - 150 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                const char tmp1[] = { actual_string.str().c_str()[0], '\0' };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, tmp1, 0, color)), NULL, &tmp);
                tmp = { float(SCREEN_WIDTH / 2 - 50 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                const char tmp2[] = { actual_string.str().c_str()[1], '\0' };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, tmp2, 0, color)), NULL, &tmp);
                tmp = { float(SCREEN_WIDTH / 2 + 50 + 25), float(SCREEN_HEIGHT - 200), float(50), float(100) };
                const char tmp3[] = { actual_string.str().c_str()[2], '\0' };
                SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Blended(same_font_but_bigger, tmp3, 0, color)), NULL, &tmp);
            }
        }
        if (onAnim && anim_time >= start_anim) {
            SDL_FRect rect = { 0.0, 0.0, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) };
            switch (hiragana_table [num_tab[actual_hir]] .value.length())
            {
            case 1:
                rect = { float(SCREEN_WIDTH / 2 - 60), float(SCREEN_HEIGHT - 200), float(120), float(120) };
                break;
            
            case 2:
                rect = { float(SCREEN_WIDTH / 2 - 120), float(SCREEN_HEIGHT - 200), float(240), float(120) };
                break;
            
            case 3:
                rect = { float(SCREEN_WIDTH / 2 - 180), float(SCREEN_HEIGHT - 200), float(360), float(120) };
                break;
            
            default:
                break;
            }

            SDL_RenderTexture(renderer, cross_texture, NULL, &rect);
        }

        if (hiragana_textures[num_tab[actual_hir]] != NULL || hiragana_textures[num_tab[actual_hir]] != nullptr) {
            SDL_RenderTexture(renderer, hiragana_textures[num_tab[actual_hir]], NULL, &hiragana_box);
        }
    }

    Uint64 endTick = SDL_GetTicks();
    Uint64 frameDuration = endTick - startTick;
    if (frameDuration < 16.666) {
        SDL_Delay(16.66 - frameDuration);
    }
}

void actualise() {

    if (actual_hir >= num_tab.size() - 1)
    {
        isOnLvlFinish = true;
        isOnAcceuil = false;
        isOnPlay = false;
        actual_hir = 0;
    }
    if(isOnPlay)
    {
        if (actual_string.str() == hiragana_table[num_tab[actual_hir]].value) { onValid = true; }

        if (actual_string.str().length() == hiragana_table[num_tab[actual_hir]].value.length() && actual_string.str() != hiragana_table[num_tab[actual_hir]].value || actual_string.str().length() > hiragana_table[num_tab[actual_hir]].value.length()) {
            if (onAnim) {
                anim_time++;
            }
            else {
                onAnim = true;
            }
        }
        if (anim_time == anim_lenght) {
            actual_string.str("");
            error_count++;
            onAnim = false;
            anim_time = 0;
        }

        if (counterValid == 20) { counterValid = 0; onValid = false; actual_string.str(""); actual_hir++; }

        if (hiragana_box.x + dx< 0 || hiragana_box.x + dx > SCREEN_WIDTH - hiragana_box.w) {
            dx = -dx;
        }
        if (hiragana_box.y + dy < 0 || hiragana_box.y + dy > 2 * SCREEN_HEIGHT / 3 - hiragana_box.h) {
            dy = -dy;
        }
        hiragana_box.x += dx;
        hiragana_box.y += dy;
    }
}



///         DEFINITION FONCTION LvlFinish
void render_LvlFinish() {

    SDL_FRect tmp = { float(15), float(SCREEN_HEIGHT / 4), float(SCREEN_WIDTH - 30), float(felicitations_textureh1->h * 0.74) };
    SDL_RenderTexture(renderer, felicitations_textureh1, NULL, &tmp);


    tmp = { float(SCREEN_WIDTH / 2 - felicitations_textureh2->w / 2), float(SCREEN_HEIGHT * 3 / 4 - felicitations_textureh2->h), float(felicitations_textureh2->w), float(felicitations_textureh2->h) };
    SDL_RenderTexture(renderer, felicitations_textureh2, NULL, &tmp);

    std::string x = std::to_string(error_count);
    SDL_Surface* text = TTF_RenderText_Blended(font, x.c_str(), 0, color);
    tmp = { float(SCREEN_WIDTH / 2 - text->w / 2), float(SCREEN_HEIGHT * 3 / 4 + 10), float(text->w), float(text->h) };
    SDL_RenderTexture(renderer, SDL_CreateTextureFromSurface(renderer, text), NULL, &tmp);
    SDL_DestroySurface(text);
}