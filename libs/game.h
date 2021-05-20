#ifndef __GAME_H
#define __GAME_H
#include <msp430.h>
#include <stdint.h>
#include "oled.h"



typedef struct ball {
    uint8_t x;               // Posição do centro em x
    uint8_t y;               // Posição do centro em y
    uint8_t size;            // Tamanho da bolinha (vertical e horizontal)
    int8_t speedX;           // Velocidade em X
    int8_t speedY;           // Velocidade em Y
} Ball;

typedef struct bar {
    uint8_t x;               // Posição em x
    uint8_t y;               // Posição em y
    uint8_t length;          // Largura (y)
    uint8_t thickness;       // Espessura (x)
} Bar;

typedef struct player {
    uint8_t score;           // Pontuação do Jogador
    Bar bar;                 // Barrinha do Jogador
} Player;


void drawVerticalBar(Bar bar);
void playersInit(Player *P1, Player *P2);
void printBoard();
void movePlayerDown(Player *player);
void movePlayerUp(Player *player);

#endif  // __GAME_H
