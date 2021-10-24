#ifndef __SNAKE_H
#define __SNAKE_H
#include "sys.h"
#include "stdlib.h"

typedef struct  
{	
	u8 life;
	u16 headX;			
	u16 headY;		
	u16 snakeX[500];
	u16 snakeY[800];	
	u16 length;
	int dir;
	int tpdir;
	u16 foodX;
	u16 foodY;

}Snake; 	

void snakeInit(void);
void snakeGo(int direct);
void refresh(void);
void GameStart(void);
void generateFood(void);
void dead(void);
u8 isCover(u16 snakeX[],u16 snakeY[],u16 foodX,u16 foodY);
u8 isNearSelf(void);

#endif 
