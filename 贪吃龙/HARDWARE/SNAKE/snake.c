#include "snake.h"
#include "lcd.h"
#include "timer.h"
#include "touch.h"
#include "stdlib.h"
#include "usart.h"
#include "key.h"
#include "led.h"
#include "main.h"
Snake snake;
extern u16 count;
extern u16 *picindextbl;	  //图片索引表 
extern u16  *picindextbl;	  //图片索引表 
extern FILINFO picfileinfo;  //文件信息
extern  DIR picdir;	 		//图片目录
u8 key;

/*
初始化蛇
*/
void snakeInit(){

	LCD_Clear(RED);
	LCD_Fill(15,25,465,775,WHITE);
	TIM3_Int_Init(4999,7199); //初始化定时器
	snake.length=2;           //初始长度为2
	
	snake.snakeX[0]=90;
	snake.snakeX[1]=90;
	
	snake.snakeY[0]=125;
	snake.snakeY[1]=100;
	
	snake.headX=snake.snakeX[0]; //记录下头部的位置
	snake.headY=snake.snakeY[0]; //记录下头部的位置
	
	snake.dir=-1;  //设置运动方向
	snake.tpdir=-1;//设置初始按键方向
	
	snake.life=1; //1：蛇还活着；0：蛇死亡
	
	generateFood();//生成食物
	ai_load_picfile("/PICTURE/zongzi.jpg",snake.foodX,snake.foodY,25,25,1);//显示图片   

	refresh();    //调用函数显示出蛇和食物的位置
}
/*
每隔一秒将会执行一次，以此实现蛇的运动
*/
void snakeGo(int direct){
	u8 res;
		u8 i;
  dir_sdi(&picdir,picindextbl[0]);			//改变当前目录索引	   
  res=f_readdir(&picdir,&picfileinfo);       		//读取身体图片 

	if(snake.life==1){
		if(abs(direct)!=abs(snake.dir)){	//如果按下的方向不是和运动的方向相同或相反
			snake.dir=direct; 
		}else{
			direct=snake.dir;	//蛇运动的方向仍是以前运动的方向
		}
		switch(direct){
			case 1:
				snake.headY-=25;  //每运动一格的单位为50
			break;
			case -1:
				snake.headY+=25; 
			break;
			case -2:
				snake.headX-=25;  
			break;
			case 2:
				snake.headX+=25; 
			break;
		}
		
	if(snake.headX==snake.foodX&&snake.headY==snake.foodY){ //如果吃到了食物
		snake.length++;  //长度加1
		
		for(i=1;i<snake.length;i++){ //除头部以外的坐标前移
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.foodX; //头部坐标等于食物的坐标
		snake.snakeY[0]=snake.foodY;	
		generateFood();  //再生成一个食物
		ai_load_picfile("/PICTURE/zongzi.jpg",snake.foodX,snake.foodY,25,25,1);//显示图片   
	}else{
		for(i=1;i<snake.length;i++){
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.headX;
		snake.snakeY[0]=snake.headY;			
	}	
	}
	if(snake.headY<775&&snake.headY>24&&snake.headX<465&&snake.headX>14&&isNearSelf()){
		snake.life=1;
	}
	else{ //触碰到墙壁游戏结束
		dead();	
	}
}
/*
刷新屏幕上蛇和食物的位置
*/

void refresh(){
	u8 res;
  dir_sdi(&picdir,picindextbl[0]);			//改变当前目录索引	   
  res=f_readdir(&picdir,&picfileinfo);       		//读取身体图片 
	if(snake.life==1){
	u8 i;
  LCD_Fill(15,25,465,snake.foodY,WHITE);
	LCD_Fill(15,snake.foodY+25,465,775,WHITE);
	LCD_Fill(15,snake.foodY,snake.foodX,snake.foodY+25,WHITE);
	LCD_Fill(snake.foodX+25,snake.foodY,465,snake.foodY+25,WHITE);	
	for(i=1;i<snake.length;i++){  //显示身体
	     ai_load_picfile("/PICTURE/body.jpg",snake.snakeX[i],snake.snakeY[i],25,25,1);//显示图片   
  }
	switch(snake.dir){
		case -1:
	         ai_load_picfile("/PICTURE/headdown.jpg",snake.headX,snake.headY,25,25,1);//显示图片  
           break;		
		case 1:
			       ai_load_picfile("/PICTURE/headup.jpg",snake.headX,snake.headY,25,25,1);//显示图片   
		         break;
		case 2:
			      ai_load_picfile("/PICTURE/headright.jpg",snake.headX,snake.headY,25,25,1);//显示图片   
		      break;
		case -2:
	     ai_load_picfile("/PICTURE/headleft.jpg",snake.headX,snake.headY,25,25,1);//显示图片   
		    break;
	}

	
 }
}
/*
开始游戏
*/

void GameStart(){
	
	while(1){
		LED0=!LED0;
		LED1=!LED1;
		tp_dev.scan(0);
		key=KEY_Scan(0);
		if(key==4)snake.tpdir=1; //上
				if(key==2)snake.tpdir=-1;//下
				if(key==3)snake.tpdir=-2; //左
				if(key==1)snake.tpdir=2;//右
		if(tp_dev.sta)			//触摸屏被按下
		{	
		 	
				if(tp_dev.x[0]>200&&tp_dev.x[0]<280&&tp_dev.y[0]<400&&tp_dev.y[0]>350&&snake.life==0){
					snake.life=1;  //死了之后点击重新开始
					snakeInit();					
				
			}
		}
		
		if(count==1){  //定时器设置的500ms的中断，1S运行一次蛇的运动函数
			count=0;			//重新计数
			snakeGo(snake.tpdir);
			refresh();	
		}
	}

}
/*
判断随机产生的食物是否处于蛇体内
*/
u8 isCover(u16 snakeX[],u16 snakeY[],u16 foodX,u16 foodY){
	u8 i;
	for(i=0;i<snake.length;i++){
		if(snakeX[i]==foodX&&snakeY[i]==foodY)
		{
		return 1;
		}
		
	}
	return 0;
}

/*
判断是否头部是否触碰到自己
*/
u8 isNearSelf(){
	u8 i;
	for(i=1;i<snake.length;i++){
		if(snake.snakeX[i]==snake.headX&&snake.snakeY[i]==snake.headY)
		{
		return 0;
		}	
	}
	return 1;
}
/*
产生随机的食物坐标
*/
void generateFood(){
		unsigned int ran=rand()%18; //因为贪吃蛇运动区域为15<x<465，25<y<775，取得0到17的随机数
		snake.foodX=15+ran*25; 
		ran=rand()%31;
		snake.foodY=25+ran*25;		
	while(isCover(snake.snakeX,snake.snakeY,snake.foodX,snake.foodY)){
		ran=rand()%18; 
		snake.foodX=15+ran*25;  
		ran=rand()%31;
		snake.foodY=25+ran*25;	
	}		

}

/*
在头部撞到墙之后执行死亡程序
*/
void dead(){
	snake.length=0;
	POINT_COLOR=RED;
	snake.snakeX[0]=0;
	snake.snakeX[1]=0;
	snake.snakeX[2]=0;
	
	snake.snakeY[0]=0;
	snake.snakeY[1]=0;
	snake.snakeY[2]=0;
	
	snake.headX=snake.snakeX[0];
	snake.headY=snake.snakeY[0];
	snake.dir=0;  
	
	snake.life=0;	
	
	LCD_ShowString(200,300,200,100,24,(u8 *)"You're dead!!!");
	//LCD_Fill(200,350,280,400,BLUE);  //点击此区域重新开始游戏
	LCD_ShowString(200,350,150,50,32,(u8 *)"REstart"); 
	
		 
}

