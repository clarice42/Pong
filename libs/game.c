#include "i2c.h"
#include "oled.h"
#include "game.h"
#include <stdlib.h>

const unsigned char oled_font_number[10][5]={{0x3E, 0x51, 0x49, 0x45, 0x3E},  // 0
                                       {0x00, 0x42, 0x7F, 0x40, 0x00},  // 1
                                       {0x42, 0x61, 0x51, 0x49, 0x46},  // 2
                                       {0x21, 0x41, 0x45, 0x4B, 0x31},  // 3
                                       {0x18, 0x14, 0x12, 0x7F, 0x10},  // 4
                                       {0x27, 0x49, 0x49, 0x49, 0x31},  // 5
                                       {0x3C, 0x4A, 0x49, 0x49, 0x30},  // 6
                                       {0x01, 0x71, 0x09, 0x05, 0x03},  // 7
                                       {0x36, 0x49, 0x49, 0x49, 0x36},  // 8
                                       {0x06, 0x49, 0x49, 0x29, 0x1E}};  // 9

uint8_t board[OLED_PAGES][OLED_LCDWIDTH] = {0,0};

void printBoard(){
    uint8_t buffer[9],i,j;
    buffer[0] = 0x40;

    for (i=0; i<OLED_LCDWIDTH; i++)
    {
        for (j=1; j<9; j++)
            buffer[j] = board[j-1][i];

        i2cWrite(OLED_I2C_ADDRESS, buffer, 9);
    }
}

void playersInit(Player *P1, Player *P2, Ball *ball){
    //Player 1
    P1->score = 0;
    P1->bar.length = 16;
    P1->bar.thickness = 3;
    P1->bar.x = 2;
    P1->bar.y = (2064/OLED_LCDHEIGHT) - (P1->bar.length/2) - 1;

    // Status iniciais do player 2
    P2->score = 0;
    P2->bar.length = 16;
    P2->bar.thickness = 3;
    P2->bar.x = 124;
    P2->bar.y = (2064/OLED_LCDHEIGHT) - (P2->bar.length/2) - 1;

    // Status inicial da bola
    ball->x = OLED_LCDWIDTH / 2;
    ball->y = OLED_LCDHEIGHT / 2;
    ball->size = 4;
    ball->speedX = (rand() % 3 + 1) * ( (rand()%2)*2 - 1);
    ball->speedY = (rand() % 3 + 1);
}

void movePlayerDown(Player *player){
    //Se a barra não estiver o máximo para baixo, desça a barra
    if( ( (player->bar.y + player->bar.length) < OLED_LCDHEIGHT) && (player->bar.y < 64) ) {
        player->bar.y += 3;
    }
    return;
}

void movePlayerUp(Player *player){
    //Se a barra não estiver o máximo para cima, suba a barra
    if( player->bar.y > 3 ){
        player->bar.y -= 3;
    }
    return;
}

void moveBall(Ball *ball) {
    uint16_t aux_x, aux_y;

    // Incrementa a posição da bola no eixo x e y
    ball->x = ball->x + ball->speedX;
    ball->y = ball->y + ball->speedY;

    // Incrementa a posição da bola no eixo x e y
    aux_x = ball->x + ball->speedX;
    aux_y = ball->y + ball->speedY;


    // Verifica se a bola não passou dos limites do quadro no eixo y
    // e inverte o sentido do movimento
    if(aux_y == 0) {
        ball->speedY = ball->speedY * (-1);
    }
    if((aux_y + ball->size) >= OLED_LCDHEIGHT) {
        ball->y = OLED_LCDHEIGHT - ball->size;
        ball->speedY = ball->speedY * (-1);
    }
}

void hitBar(Player *player1, Player *player2, Ball *ball) {
    if((ball->x + ball->size >= player2->bar.x)) {
        if(((ball->y + ball->size) >= player2->bar.y) && (ball->y <= (player2->bar.y + player2->bar.length))) {
            ball->speedX = ball->speedX * (-1);
        }
        else {
            player1->score += 1;
            ball->x = OLED_LCDWIDTH / 2;
            ball->y = OLED_LCDHEIGHT / 2;
            ball->speedX = (rand() % 3 + 1) * ( (rand()%2)*2 - 1);
            ball->speedY = rand() % 3 + 1;
        }
    }


    if((ball->x <= player1->bar.x + player1->bar.thickness)) {
        if(((ball->y + ball->size) >= player1->bar.y) && (ball->y <= (player1->bar.y + player1->bar.length))) {
            ball->speedX = ball->speedX * (-1);
        }
        else {
            player2->score += 1;
            ball->x = OLED_LCDWIDTH / 2;
            ball->y = OLED_LCDHEIGHT / 2;
            ball->speedX = (rand() % 3 + 1) * ( (rand()%2)*2 - 1);
            ball->speedY = rand() % 3 + 1;
        }
    }

}

void drawMiddleLine(){
    uint8_t i=0;
    for(i=0;i<OLED_PAGES;i++){
        board[i][63] = 0xF0;
        board[i][64] = 0xF0;
    }
}

void changeScore(int8_t num, uint8_t score){
    uint8_t aux=0;
    if(num==0){                 // Não foi gol para ninguém
        return;
    }
    if(num<0){                  // Foi gol do jogador à esquerda
        if(score<10){
            board[1][20] = oled_font_number[score][0];
            board[1][21] = oled_font_number[score][1];
            board[1][22] = oled_font_number[score][2];
            board[1][23] = oled_font_number[score][3];
            board[1][24] = oled_font_number[score][4];
        }
        else{
            aux = score/10;
            board[1][20] = oled_font_number[aux][0];
            board[1][21] = oled_font_number[aux][1];
            board[1][22] = oled_font_number[aux][2];
            board[1][23] = oled_font_number[aux][3];
            board[1][24] = oled_font_number[aux][4];
            aux = score%10;
            board[1][26] = oled_font_number[aux][0];
            board[1][27] = oled_font_number[aux][1];
            board[1][28] = oled_font_number[aux][2];
            board[1][29] = oled_font_number[aux][3];
            board[1][30] = oled_font_number[aux][4];
        }
    }

    if(num>0){                  // Foi gol do jogador à direita
        if(score<10){
            board[1][97] = oled_font_number[score][0];
            board[1][98] = oled_font_number[score][1];
            board[1][99] = oled_font_number[score][2];
            board[1][100] = oled_font_number[score][3];
            board[1][101] = oled_font_number[score][4];
        }
        else{
            aux = score/10;
            board[1][97] = oled_font_number[aux][0];
            board[1][98] = oled_font_number[aux][1];
            board[1][99] = oled_font_number[aux][2];
            board[1][100] = oled_font_number[aux][3];
            board[1][101] = oled_font_number[aux][4];
            aux = score%10;
            board[1][103] = oled_font_number[aux][0];
            board[1][104] = oled_font_number[aux][1];
            board[1][105] = oled_font_number[aux][2];
            board[1][106] = oled_font_number[aux][3];
            board[1][107] = oled_font_number[aux][4];
        }
    }

}

void drawVerticalBar(uint8_t x, uint8_t y, uint8_t length, uint8_t thickness){
    // O OLED é dividido em 8 linhas (pages) com 128 colunas e 8 píxeis em cada coluna
    // Assim, é necessário saber quais linhas (pages) preencher

    uint8_t mask1, mask2, i;

    for( i=0; i<thickness;i++){
        uint8_t line = y / OLED_PAGES;              // Encontra em qual linha a barra começa
        uint8_t start = y % OLED_PAGES;             // Em qual píxel começar a preencher.
        uint8_t lenToFill = length;                 // Quantidade de píxeis a preencher

        if(line>0){                                     // Apaga os resquícios que ficaram na linha anterior
            board[line-1][x+i] = 0x00;
        }
        while(line<OLED_PAGES && lenToFill > 0){
            mask1 = 0xFF << start;
            mask2 = 0xFF;
            if((8-start) > lenToFill){
                mask2 = 0xFF >> (8-start) - lenToFill;  // Calcular o quanto foi preenchido a mais
                lenToFill = 0;
            }
            else{
                lenToFill -= (8-start);
            }
            board[line][x+i] = (mask1 & mask2);
            line++;
            start=0;
        }
        line--;
        if(line < OLED_PAGES-1){                        // Apaga os resquícios que ficaram na linha posterior
            board[line+1][x+i] = 0x00;
        }
    }
}

uint8_t gameOver(Player *player1, Player *player2) {
    if(player1->score > 12 || player2->score > 12) {
        return 1;
    }
    else {
        return 0;
    }
}

void clear() {
    uint8_t width, height;

    for(height = 0; height < OLED_PAGES; height++) {
        for(width = 0; width < OLED_LCDWIDTH; width++) {
            board[height][width] = 0;
        }
    }
}
