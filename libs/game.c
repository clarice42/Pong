#include "i2c.h"
#include "oled.h"
#include "game.h"

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
void playersInit(Player *P1, Player *P2){
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

}

void movePlayerDown(Player *player){
    //Se a barra não estiver o máximo para baixo, desça a barra
    if( ( (player->bar.y + player->bar.length) < OLED_LCDHEIGHT) && (player->bar.y < 127) ){
      player->bar.y += 3;
    }
    return;
}

void movePlayerUp(Player *player){
    //Se a barra não estiver o máximo para cima, suba a barra
    if( (player->bar.y > 0)){
      player->bar.y -= 3;
    }
    return;
}

void drawVerticalBar(Bar bar){
    // O OLED é dividido em 8 linhas (pages) com 128 colunas e 8 píxeis em cada coluna
    // Assim, é necessário saber quais linhas (pages) preencher

    uint8_t mask1, mask2, i;

    for( i=0; i<bar.thickness;i++){
        uint8_t line = bar.y / OLED_PAGES;              // Encontra em qual linha a barra começa
        uint8_t start = bar.y % OLED_PAGES;             // Em qual píxel começar a preencher.
        uint8_t lenToFill = bar.length;                 // Quantidade de píxeis a preencher

        if(line>0){                                     // Apaga os resquícios que ficaram na linha anterior
            board[line-1][bar.x+i] = 0x00;
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
            board[line][bar.x+i] = (mask1 & mask2);
            line++;
            start=0;
        }
        line--;
        if(line < OLED_PAGES-1){                        // Apaga os resquícios que ficaram na linha posterior
            board[line+1][bar.x+i] = 0x00;
        }
    }
}
