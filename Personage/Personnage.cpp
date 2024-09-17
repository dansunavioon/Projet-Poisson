#include "Personnage.h"
#include "var_personnage.h"

// ------------ Variables globales



// -------------------------------

Personnage::Personnage() {

}

void Personnage::handle_events() {

}

void Personnage::affichage() {

}

void Personnage::set_clip() {
    for(int i = 0; i < 4; i ++){
        clipsDroite[ i ].x = TAILLE_PLONGEUR * i;
        clipsDroite[ i ].y = 0;
        clipsDroite[ i ].w = SHEET_WIDTH_BANDE;
        clipsDroite[ i ].h = SHEET_HEIGHT_BANDE;

        clipsGauche[ i ].x = TAILLE_PLONGEUR * i;
        clipsGauche[ i ].y = SHEET_HEIGHT_BANDE;
        clipsGauche[ i ].w = SHEET_WIDTH_BANDE;
        clipsGauche[ i ].h = SHEET_HEIGHT_BANDE;

        clipsBas[ i ].x = 0;
        clipsBas[ i ].y = TAILLE_PLONGEUR * i;
        clipsBas[ i ].w = SHEET_WIDTH_BATON;
        clipsBas[ i ].h = SHEET_HEIGHT_BATON;

        clipsHaut[ i ].x = SHEET_WIDTH_BATON;
        clipsHaut[ i ].y = TAILLE_PLONGEUR * i;
        clipsHaut[ i ].w = SHEET_WIDTH_BATON;
        clipsHaut[ i ].h = SHEET_HEIGHT_BATON;
    }
}
