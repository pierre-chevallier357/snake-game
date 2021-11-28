#include <time.h>
#include <SFML/Graphics.hpp>
using namespace sf;

const int LARGEUR_TERRAIN = 16;
const int HAUTEUR_TERRAIN = 9;
const int LONGUEUR_INITIALE_SERPENT = 4;
const int TAILLE_CASE = 1;
const int LARGEUR_ECRAN = TAILLE_CASE * LARGEUR_TERRAIN;
const int HAUTEUR_ECRAN = TAILLE_CASE * HAUTEUR_TERRAIN;

int direction = 4;
int longueurSerpent = LONGUEUR_INITIALE_SERPENT;

struct Snake {
    int x, y;
} serpent[LARGEUR_TERRAIN * HAUTEUR_TERRAIN];

struct Fruit {
    int x, y;
} fruit;

void Tick() {
    // Création du serpent de la taille longueurSerpent
    for (int i = longueurSerpent; i > 0; --i) {
        serpent[i].x = serpent[i - 1].x;
        serpent[i].y = serpent[i - 1].y;
    }

    // Suivant la direction
    // On fait avancer le serpent d'une case vers la direction
    switch (direction) {
        case 0:
            serpent[0].y += 1;
            break;
        case 1:
            serpent[0].x -= 1;
            break;
        case 2:
            serpent[0].x += 1;
            break;
        case 3:
            serpent[0].y -= 1;
            break;
    }

    // Si le serpent sort de l'écran
    // On le fait apparaître à l'opposer de l'écran
    if (serpent[0].x > LARGEUR_TERRAIN - 1) {
        serpent[0].x = 0;
    }
    if (serpent[0].x < 0) {
        serpent[0].x = LARGEUR_TERRAIN - 1;
    }
    if (serpent[0].y > HAUTEUR_TERRAIN - 1) {
        serpent[0].y = 0;
    }
    if (serpent[0].y < 0) {
        serpent[0].y = HAUTEUR_TERRAIN - 1;
    }

    // Si le serpent touche le fruit
    // On augmente la longueur du serpent de un
    // On créé un nouveau fruit a des coordonées au hasard
    if ((serpent[0].x == fruit.x) && (serpent[0].y == fruit.y)) {
        longueurSerpent++;
        fruit.x = rand() % LARGEUR_TERRAIN;
        fruit.y = rand() % HAUTEUR_TERRAIN;
    }

    // Pour chaque case du serpent
    for (int i = 1; i < longueurSerpent; i++) {
        // Si la tête touche cette case
        // On réinitialise le serpent
        if (serpent[0].x == serpent[i].x && serpent[0].y == serpent[i].y) {
            longueurSerpent = LONGUEUR_INITIALE_SERPENT;
        }
    }
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN), "Jeu du Snake", Style::Close | Style ::Fullscreen | Style::Titlebar);

    Texture noir, rouge, vert;
    noir.loadFromFile("noir.png");
    rouge.loadFromFile("rouge.png");
    vert.loadFromFile("vert.png");

    Sprite caseNoire(noir);
    Sprite caseRouge(rouge);
    Sprite caseVert(vert);

    Clock horloge;
    float tempsQuiPasse = 0;
    float delai = 0.1;
    float temps;

    fruit.x = rand() % LARGEUR_TERRAIN;
    fruit.y = rand() % HAUTEUR_TERRAIN;

    View view(Vector2f(0, 0), Vector2f(LARGEUR_ECRAN, HAUTEUR_ECRAN));
    view.setCenter(LARGEUR_ECRAN / 2.0f, HAUTEUR_ECRAN / 2.0f);

    while (window.isOpen()) {
        temps = horloge.getElapsedTime().asSeconds();
        horloge.restart();
        tempsQuiPasse += temps;

        Event evenement;
        while (window.pollEvent(evenement)) {
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            direction = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            direction = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            direction = 2;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            direction = 3;
        }

        if (tempsQuiPasse > delai) {
            tempsQuiPasse = 0;
            Tick();
        }

        /////// AFFICHAGE ///////
        window.clear();
        window.setView(view);

        // Affichage du fond avec des cases noires
        for (int i = 0; i < LARGEUR_TERRAIN; i++) {
            for (int j = 0; j < HAUTEUR_TERRAIN; j++) {
                caseNoire.setPosition(i * TAILLE_CASE, j * TAILLE_CASE);
                window.draw(caseNoire);
            }
        }

        // Affichage du serpent avec des cases vertes
        for (int i = 0; i < longueurSerpent; i++) {
            caseVert.setPosition(serpent[i].x * TAILLE_CASE, serpent[i].y * TAILLE_CASE);
            window.draw(caseVert);
        }

        // Affichage du fruit avec une case rouge
        caseRouge.setPosition(fruit.x * TAILLE_CASE, fruit.y * TAILLE_CASE);
        window.draw(caseRouge);

        window.display();
    }

    return 0;
}
