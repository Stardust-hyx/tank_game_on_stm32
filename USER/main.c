#include "includes.h"
#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h"	 	
#include "malloc.h" 
#include "sram.h"  
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"		
#include "math.h"
#include "audioplay.h"
#include "edit_map.h" 
#include "guix.h"
#include "select_stage.h"
#include "game.h"
#include "remote.h"
#include "rng.h"


void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color); 

u16 colorlist[15]={BLACK,BLUE,BROWN,WHITE,RED,YELLOW,GREEN,BRED,GBLUE,MAGENTA,CYAN,BRRED,GRED,GRAY,DGRAY};
/////////////////// 0     1     2     3    4    5      6    7     8     9      10    11   12   13    14

u16 print_ui_tank[5][5]=	//UI��ʶ��
		{
			{BLACK,RED,BLACK,RED,BLACK},
			{RED,RED,RED,BLACK,BLACK},
			{BLACK,RED,RED,RED,RED},
			{RED,RED,RED,BLACK,BLACK},
			{BLACK,RED,BLACK,RED,BLACK}
		};

mybutton btn_start_game={23,163,70,17};
mybutton btn_edit_map={110,163,70,17};
mybutton btn_select_stage={196,163,70,17};
mybutton btn_power_off={16,209,20,20};
mybutton btn_help={60,209,20,20};
mybutton exit_help={130,207,50,25};

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   

//������
//�����������ȼ�
#define MAIN_TASK_PRIO       			3 
//���������ջ��С
#define MAIN_STK_SIZE  					2048
//�����ջ	
OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];
//������
void main_task(void *pdata);


//////////////////////////////////////////////////////////////////////////////

void init(void);	//��ʼ����Ƭ��
u32 touch_scan(void);	//���ؼ��


//�������
int main(void)
{ 
  init();			//��ʼ��
  OSInit();  	 				//��ʼ��UCOSII
  OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
	
}

void init(void){
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	usmart_dev.init(84);		//��ʼ��USMART
	LED_Init();					//��ʼ��LED 
	KEY_Init(); 				//������ʼ��  
 	LCD_Init();					//LCD��ʼ�� 
	tp_dev.init();				//��������ʼ��
	Remote_Init();				//������ճ�ʼ��
	W25QXX_Init();				//��ʼ��W25Q128
/*	WM8978_Init();				//��ʼ��WM8978
	WM8978_HPvol_Set(40,40);	//������������
	WM8978_SPKvol_Set(25);		//������������*/
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
	f_mount(fs[0],"0:",1); 	//����SD��  
	int key;
	while(font_init()) 			//����ֿ�
	{   
		LCD_Clear(WHITE);		   	//����
 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
		LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");
		while(SD_Init())			//���SD��
		{
			LCD_ShowString(30,70,200,16,16,"SD Card Failed!");
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
		}								 						    
		LCD_ShowString(30,70,200,16,16,"SD Card OK");
		LCD_ShowString(30,90,200,16,16,"Font Updating...");
		key=update_font(20,110,16,"0:");//�����ֿ�
		while(key)//����ʧ��		
		{			 		  
			LCD_ShowString(30,110,200,16,16,"Font Update Failed!");
			delay_ms(200);
			LCD_Fill(20,110,200+20,110+16,WHITE);
			delay_ms(200);		       
		} 		  
		LCD_ShowString(30,110,200,16,16,"Font Update Success!   ");  
	}
		while(RNG_Init())	 		//��ʼ�������������
	{
		LCD_ShowString(30,130,200,16,16,"  RNG Error! ");	 
		delay_ms(200);
		LCD_ShowString(30,130,200,16,16,"RNG Trying...");	 
	}
}

//extern OS_EVENT * audiombox;//�¼����ƿ�

//��ʼ����
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata; 		  
	//audiombox=OSMboxCreate((void*) 0);	//������Ƶ������������  
	OSStatInit();					//��ʼ��ͳ������.�������ʱ1��������	
 	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)
	//audioplay_task_creat();
  OSTaskCreate(main_task,(void *)0,(OS_STK*)&MAIN_TASK_STK[MAIN_STK_SIZE-1],MAIN_TASK_PRIO);	 				   	 				   
 	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}	  

//����������   
void load_main_ui(void)
{
	
	LCD_Clear(BLACK);
	POINT_COLOR=RED;
	while(ai_load_picfile("0:/PICTURE/������.bmp",0,0,lcddev.width,lcddev.height,1)); //��ʾ����ͼƬ
	Show_Str(btn_start_game.x,btn_start_game.y,btn_start_game.width,btn_start_game.height,"������Ϸ",16,1);
	Show_Str(btn_edit_map.x,btn_edit_map.y,btn_edit_map.width,btn_edit_map.height,"�༭��ͼ",16,1);
	Show_Str(btn_select_stage.x,btn_select_stage.y,btn_select_stage.width,btn_select_stage.height,"ѡ��ؿ�",16,1);
	POINT_COLOR=RED;
	LCD_Draw_Circle(26,219,10);	
	POINT_COLOR=WHITE;
	LCD_Draw_Circle(70,219,10);
	POINT_COLOR=RED;
	LCD_ShowChar(67,211,'?',16,1);
}	

//���ؼ��
u32 touch_scan(void)
{	  	
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)		//������������
		{	
		 	if(tp_dev.x[0]> btn_start_game.x && tp_dev.x[0]< btn_start_game.x + btn_start_game.width 
			&& tp_dev.y[0]> btn_start_game.y && tp_dev.y[0]< btn_start_game.y + btn_start_game.height)
			{			
					return 1;
			}
			else if(tp_dev.x[0]> btn_edit_map.x && tp_dev.x[0]< btn_edit_map.x + btn_edit_map.width 
				   && tp_dev.y[0]> btn_edit_map.y && tp_dev.y[0]< btn_edit_map.y + btn_edit_map.height)
			{			
					return 2;
			}
			else if(tp_dev.x[0]> btn_select_stage.x && tp_dev.x[0]< btn_select_stage.x + btn_select_stage.width 
				   && tp_dev.y[0]> btn_select_stage.y && tp_dev.y[0]< btn_select_stage.y + btn_select_stage.height)
			{			
					return 3;
			}
			else if(tp_dev.x[0]> btn_power_off.x && tp_dev.x[0]< btn_power_off.x + btn_power_off.width 
				   && tp_dev.y[0]> btn_power_off.y && tp_dev.y[0]< btn_power_off.y + btn_power_off.height)
			{			
					return 4;
			}
			else if(tp_dev.x[0]> btn_help.x && tp_dev.x[0]< btn_help.x + btn_help.width 
				   && tp_dev.y[0]> btn_help.y && tp_dev.y[0]< btn_help.y +btn_help.height)
			{			
					return 5;
			}
		}
		return 0;
}

//����help
void help(void){
	int key;
	while(ai_load_picfile("0:/PICTURE/����.bmp",0,0,lcddev.width,lcddev.height,1)); //��ʾ����
	POINT_COLOR=RED;
	Show_Str(exit_help.x,exit_help.y,48,24,"ȷ��",24,1);
	while(1){
		key=KEY_Scan(0);
		tp_dev.scan(0);
		if(key==5 || key==6) {return;}	//���ˡ�ȷ������ȡ������
		if(tp_dev.sta&TP_PRES_DOWN)		//������������
		{	//�����ˡ�ȷ�ϡ���ť
		 	if(tp_dev.x[0]> exit_help.x-3 && tp_dev.x[0]< exit_help.x + exit_help.width 
			&& tp_dev.y[0]> exit_help.y-3 && tp_dev.y[0]< exit_help.y + exit_help.height)
			{	//			
					return;
			}
		}
	}
}


//������
void main_task(void *pdata)
{	
	int flag = 1;		//��ǰѡ���İ�ť
	int pre = 5;   	//ǰһ��ѡ���İ�ť
	int tmp;
	int update=1;  	//����Ƿ��л��˰�ť
	int stage=1;    //��ǰѡ���ؿ���ȱʡΪ��һ��
	
	u32 key;
	u32 touch=0;
	piclib_init();	//����ͼƬ��Ҫ��ʼ��ͼƬ��
	load_main_ui();	//����������
	while(1){
		touch=touch_scan();
		key=KEY_Scan(0);
		POINT_COLOR=BLACK;
		/*ȷ��ѡ��İ�ť��������Ӧ����ģ��*/
		if(touch == flag || key==5){
			switch(flag){
				case 1:{
					delay_ms(50);
					pregame(stage);	//��Ϸ
					load_main_ui();
					update=1;
					break;
				}
				case 2:{
					edit();					//�༭��ͼ
					load_main_ui();
					update=1;
					break;
				}
				case 3:{
					stage=select();	//ѡ��ؿ�
					load_main_ui();
					update=1;
					break;
				}
				case 4:{
					PWR_EnterSTANDBYMode();	//����
					break;
				//����
				}
				case 5:{
					help();					//����
					load_main_ui();	
					update=1;
					break;
				//����˵��
				}
			}
		}
		/*���µ�ǰѡ����ť*/
		else if(touch) {
			flag = touch;
			update = 1;
		}
		else if( key==3 ){
			flag = (--flag) ? flag : 5;
			update = 1;
		}
		else if(key == 4){
			flag = (flag++)%5 ? flag : 1 ;
			update = 1;
		}
		else if(key == 1 || key == 2){
			if(pre<4&&flag<4) {pre = flag; flag = 5; }
			else if(pre>=4&&flag>=4) {pre = flag; flag = 1;}
			else{
				tmp = pre;
				pre = flag;
				flag = tmp;
			}
			update = 1;
		}
		/*���½���*/
		else if(update){
			switch(flag){
				case 1:{
					//LCD_Fill(btn_start_game.x - 20,btn_start_game.y-2,btn_start_game.x,btn_start_game.y+btn_start_game.height+1,WHITE);
					LCD_Fill(btn_edit_map.x - 20,btn_edit_map.y-2,btn_edit_map.x,btn_edit_map.y+btn_edit_map.height+1,BLACK);
					LCD_Fill(btn_select_stage.x - 20,btn_select_stage.y-2,btn_select_stage.x,btn_select_stage.y+btn_select_stage.height+1,BLACK);
					for(int i=0;i<5;i++){
						for(int j=0;j<5;j++){
							LCD_Fill(btn_start_game.x - 20+j*4,btn_start_game.y-2+i*4,btn_start_game.x - 20+(j*4+3),btn_start_game.y-2+(i*4+3),print_ui_tank[i][j]);
						}
					}
					POINT_COLOR=RED;
					LCD_Draw_Circle(26,219,10);	
					POINT_COLOR=WHITE;
					LCD_Draw_Circle(70,219,10);
					POINT_COLOR=RED;
					LCD_ShowChar(67,211,'?',16,1);
					update=0;
					break;
				}
				case 2:{
					LCD_Fill(btn_start_game.x - 20,btn_start_game.y-2,btn_start_game.x,btn_start_game.y+btn_start_game.height+1,BLACK);
					//LCD_Fill(btn_edit_map.x - 20,btn_edit_map.y-2,btn_edit_map.x,btn_edit_map.y+btn_edit_map.height+1,WHITE);
					LCD_Fill(btn_select_stage.x - 20,btn_select_stage.y-2,btn_select_stage.x,btn_select_stage.y+btn_select_stage.height+1,BLACK);
					for(int i=0;i<5;i++){
						for(int j=0;j<5;j++){
							LCD_Fill(btn_edit_map.x - 20+j*4,btn_edit_map.y-2+i*4,btn_edit_map.x - 20+(j*4+3),btn_edit_map.y-2+(i*4+3),print_ui_tank[i][j]);
						}
					}
					POINT_COLOR=RED;
					LCD_Draw_Circle(26,219,10);	
					POINT_COLOR=WHITE;
					LCD_Draw_Circle(70,219,10);
					POINT_COLOR=RED;
					LCD_ShowChar(67,211,'?',16,1);				
					update=0;
					break;
				}
				case 3:{
					LCD_Fill(btn_start_game.x - 20,btn_start_game.y-2,btn_start_game.x,btn_start_game.y+btn_start_game.height+1,BLACK);
					LCD_Fill(btn_edit_map.x - 20,btn_edit_map.y-2,btn_edit_map.x,btn_edit_map.y+btn_edit_map.height+1,BLACK);
					//LCD_Fill(btn_select_stage.x - 20,btn_select_stage.y-2,btn_select_stage.x,btn_select_stage.y+btn_select_stage.height+1,WHITE);
					for(int i=0;i<5;i++){
						for(int j=0;j<5;j++){
							LCD_Fill(btn_select_stage.x - 20+j*4,btn_select_stage.y-2+i*4,btn_select_stage.x - 20+(j*4+3),btn_select_stage.y-2+(i*4+3),print_ui_tank[i][j]);
						}
					}
					POINT_COLOR=RED;
					LCD_Draw_Circle(26,219,10);	
					POINT_COLOR=WHITE;
					LCD_Draw_Circle(70,219,10);
					POINT_COLOR=RED;
					LCD_ShowChar(67,211,'?',16,1);
					update=0;
					break;
				//ѡ��ؿ�
				}
				case 4:{
					LCD_Fill(btn_start_game.x - 20,btn_start_game.y-2,btn_start_game.x,btn_start_game.y+btn_start_game.height+1,BLACK);
					LCD_Fill(btn_edit_map.x - 20,btn_edit_map.y-2,btn_edit_map.x,btn_edit_map.y+btn_edit_map.height+1,BLACK);
					LCD_Fill(btn_select_stage.x - 20,btn_select_stage.y-2,btn_select_stage.x,btn_select_stage.y+btn_select_stage.height+1,BLACK);
					POINT_COLOR=WHITE;
					LCD_Draw_Circle(26,219,10);	
					POINT_COLOR=WHITE;
					LCD_Draw_Circle(70,219,10);
					POINT_COLOR=RED;
					LCD_ShowChar(67,211,'?',16,1);
					update=0;
					break;
				}
				case 5:{
					LCD_Fill(btn_start_game.x - 20,btn_start_game.y-2,btn_start_game.x,btn_start_game.y+btn_start_game.height+1,BLACK);
					LCD_Fill(btn_edit_map.x - 20,btn_edit_map.y-2,btn_edit_map.x,btn_edit_map.y+btn_edit_map.height+1,BLACK);
					LCD_Fill(btn_select_stage.x - 20,btn_select_stage.y-2,btn_select_stage.x,btn_select_stage.y+btn_select_stage.height+1,BLACK);
					POINT_COLOR=RED;
					LCD_Draw_Circle(26,219,10);	
					POINT_COLOR=RED;
					LCD_Draw_Circle(70,219,10);
					POINT_COLOR=WHITE;
					LCD_ShowChar(67,211,'?',16,1);
					update=0;
					break;
				}
			}
			
		}
		delay_ms(10);
	}
}




