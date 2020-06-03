#include "menu.h"

int main(int argc, char** argv) {

    Menu* menu = new Menu(argc, argv);
    menu->mainMenu();
    delete menu;

    return 0;
}
