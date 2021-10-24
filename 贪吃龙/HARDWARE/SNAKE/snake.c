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
extern u16 *picindextbl;	  //ͼƬ������ 
extern u16  *picindextbl;	  //ͼƬ������ 
extern FILINFO picfileinfo;  //�ļ���Ϣ
extern  DIR picdir;	 		//ͼƬĿ¼
u8 key;

/*
��ʼ����
*/
void snakeInit(){

	LCD_Clear(RED);
	LCD_Fill(15,25,465,775,WHITE);
	TIM3_Int_Init(4999,7199); //��ʼ����ʱ��
	snake.length=2;           //��ʼ����Ϊ2
	
	snake.snakeX[0]=90;
	snake.snakeX[1]=90;
	
	snake.snakeY[0]=125;
	snake.snakeY[1]=100;
	
	snake.headX=snake.snakeX[0]; //��¼��ͷ����λ��
	snake.headY=snake.snakeY[0]; //��¼��ͷ����λ��
	
	snake.dir=-1;  //�����˶�����
	snake.tpdir=-1;//���ó�ʼ��������
	
	snake.life=1; //1���߻����ţ�0��������
	
	generateFood();//����ʳ��
	ai_load_picfile("/PICTURE/zongzi.jpg",snake.foodX,snake.foodY,25,25,1);//��ʾͼƬ   

	refresh();    //���ú�����ʾ���ߺ�ʳ���λ��
}
/*
ÿ��һ�뽫��ִ��һ�Σ��Դ�ʵ���ߵ��˶�
*/
void snakeGo(int direct){
	u8 res;
		u8 i;
  dir_sdi(&picdir,picindextbl[0]);			//�ı䵱ǰĿ¼����	   
  res=f_readdir(&picdir,&picfileinfo);       		//��ȡ����ͼƬ 

	if(snake.life==1){
		if(abs(direct)!=abs(snake.dir)){	//������µķ����Ǻ��˶��ķ�����ͬ���෴
			snake.dir=direct; 
		}else{
			direct=snake.dir;	//���˶��ķ���������ǰ�˶��ķ���
		}
		switch(direct){
			case 1:
				snake.headY-=25;  //ÿ�˶�һ��ĵ�λΪ50
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
		
	if(snake.headX==snake.foodX&&snake.headY==snake.foodY){ //����Ե���ʳ��
		snake.length++;  //���ȼ�1
		
		for(i=1;i<snake.length;i++){ //��ͷ�����������ǰ��
			snake.snakeX[snake.length-i]=snake.snakeX[snake.length-i-1];
			snake.snakeY[snake.length-i]=snake.snakeY[snake.length-i-1];
		}
		snake.snakeX[0]=snake.foodX; //ͷ���������ʳ�������
		snake.snakeY[0]=snake.foodY;	
		generateFood();  //������һ��ʳ��
		ai_load_picfile("/PICTURE/zongzi.jpg",snake.foodX,snake.foodY,25,25,1);//��ʾͼƬ   
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
	else{ //������ǽ����Ϸ����
		dead();	
	}
}
/*
ˢ����Ļ���ߺ�ʳ���λ��
*/

void refresh(){
	u8 res;
  dir_sdi(&picdir,picindextbl[0]);			//�ı䵱ǰĿ¼����	   
  res=f_readdir(&picdir,&picfileinfo);       		//��ȡ����ͼƬ 
	if(snake.life==1){
	u8 i;
  LCD_Fill(15,25,465,snake.foodY,WHITE);
	LCD_Fill(15,snake.foodY+25,465,775,WHITE);
	LCD_Fill(15,snake.foodY,snake.foodX,snake.foodY+25,WHITE);
	LCD_Fill(snake.foodX+25,snake.foodY,465,snake.foodY+25,WHITE);	
	for(i=1;i<snake.length;i++){  //��ʾ����
	     ai_load_picfile("/PICTURE/body.jpg",snake.snakeX[i],snake.snakeY[i],25,25,1);//��ʾͼƬ   
  }
	switch(snake.dir){
		case -1:
	         ai_load_picfile("/PICTURE/headdown.jpg",snake.headX,snake.headY,25,25,1);//��ʾͼƬ  
           break;		
		case 1:
			       ai_load_picfile("/PICTURE/headup.jpg",snake.headX,snake.headY,25,25,1);//��ʾͼƬ   
		         break;
		case 2:
			      ai_load_picfile("/PICTURE/headright.jpg",snake.headX,snake.headY,25,25,1);//��ʾͼƬ   
		      break;
		case -2:
	     ai_load_picfile("/PICTURE/headleft.jpg",snake.headX,snake.headY,25,25,1);//��ʾͼƬ   
		    break;
	}

	
 }
}
/*
��ʼ��Ϸ
*/

void GameStart(){
	
	while(1){
		LED0=!LED0;
		LED1=!LED1;
		tp_dev.scan(0);
		key=KEY_Scan(0);
		if(key==4)snake.tpdir=1; //��
				if(key==2)snake.tpdir=-1;//��
				if(key==3)snake.tpdir=-2; //��
				if(key==1)snake.tpdir=2;//��
		if(tp_dev.sta)			//������������
		{	
		 	
				if(tp_dev.x[0]>200&&tp_dev.x[0]<280&&tp_dev.y[0]<400&&tp_dev.y[0]>350&&snake.life==0){
					snake.life=1;  //����֮�������¿�ʼ
					snakeInit();					
				
			}
		}
		
		if(count==1){  //��ʱ�����õ�500ms���жϣ�1S����һ���ߵ��˶�����
			count=0;			//���¼���
			snakeGo(snake.tpdir);
			refresh();	
		}
	}

}
/*
�ж����������ʳ���Ƿ���������
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
�ж��Ƿ�ͷ���Ƿ������Լ�
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
���������ʳ������
*/
void generateFood(){
		unsigned int ran=rand()%18; //��Ϊ̰�����˶�����Ϊ15<x<465��25<y<775��ȡ��0��17�������
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
��ͷ��ײ��ǽ֮��ִ����������
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
	//LCD_Fill(200,350,280,400,BLUE);  //������������¿�ʼ��Ϸ
	LCD_ShowString(200,350,150,50,32,(u8 *)"REstart"); 
	
		 
}

