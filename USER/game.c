#include <stdbool.h> 
#include "sys.h"
#include "delay.h"  
#include "usart.h" 
#include "lcd.h"
#include "touch.h"
#include "key.h"
#include "malloc.h" 
#include "piclib.h"	
#include "sdio_sdcard.h"
#include "guix.h"
#include "text.h"
#include "data_addr.h"
#include "game.h"
#include "remote.h"
#include "rng.h"


#define MAXLIFE 9

#define MAX_LEVEL 9  //����ؿ���
#define BULLET_NUM 50//������Ļ������ӵ���
#define YMAX  59//��ͼ���·����ݼ�����
#define TANKWI 7 //tank�Ŀ�ȣ������꣩
#define HOME_WIDTH 8 //�����ϼҿ��
#define XMED  30//��ͼ���ĺ������
#define UP   1//����÷���Ϊ�����direction��׼��
#define DOWN 2
#define LEFT  3
#define RIGHT 4
#define WALL1 BROWN  //��ͨǽ
#define WATER BLUE //ˮ��
#define WALL3 DGRAY//�����ƻ���ǽ
#define WALL4 GRAY//��ͼ�߽�
#define HOME WHITE//�ϼң���ɫ��
#define COLOR0 RED    //���1��ɫ��ɫ
#define COLOR1 CYAN   //��̹ͨ����ɫ
#define COLOR2 PURPLE  //����̹����ɫ
#define COLOR3 GREEN  //���̹��1��̬��ɫ
#define COLOR4 OLIVE   //���̹��2��̬��ɫ
#define COLOR5 MAGENTA  //���̹��3��̬��ɫ
#define COLOR6 YELLOW	//�ӵ���ɫ
#define COLOR7 DARKBLUE   //���2 ��ɫ
#define COLOR8 DCYAN  //�����ɫ


extern u16 colorlist[15];

extern u16 print_ui_tank[5][5];

u16 print_my_tank[7][7]=
		{
			{BLACK,BLACK,BLACK,COLOR0,BLACK,BLACK,BLACK},
			{BLACK,BLACK,BLACK,COLOR0,BLACK,BLACK,BLACK},
			{BLACK,BLACK,BLACK,COLOR0,BLACK,BLACK,BLACK},
   		{BLACK,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,BLACK},
			{BLACK,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,BLACK},
			{BLACK,COLOR0,COLOR8,COLOR0,COLOR8,COLOR0,BLACK},
			{BLACK,COLOR0,COLOR0,COLOR0,COLOR0,COLOR0,BLACK}

		};

u16 print_my_tank2[7][7]=
		{
			{BLACK,BLACK,BLACK,COLOR7,BLACK,BLACK,BLACK},
			{BLACK,BLACK,BLACK,COLOR7,BLACK,BLACK,BLACK},
			{BLACK,BLACK,BLACK,COLOR7,BLACK,BLACK,BLACK},
   			{BLACK,COLOR7,COLOR7,COLOR7,COLOR7,COLOR7,BLACK},
			{BLACK,COLOR7,COLOR7,COLOR7,COLOR7,COLOR7,BLACK},
			{BLACK,COLOR7,COLOR8,COLOR7,COLOR8,COLOR7,BLACK},
			{BLACK,COLOR7,COLOR7,COLOR7,COLOR7,COLOR7,BLACK}

		};

u16 print_normal_tank[7][7]=
		{
			{BLACK,BLACK,BLACK,COLOR1,BLACK,BLACK,BLACK},
			{BLACK,BLACK,BLACK,COLOR1,BLACK,BLACK,BLACK},
  			{BLACK,BLACK,COLOR1,COLOR1,COLOR1,BLACK,BLACK},
			{BLACK,COLOR1,COLOR1,COLOR1,COLOR1,COLOR1,BLACK},
  			{BLACK,COLOR1,COLOR8,COLOR1,COLOR8,COLOR1,BLACK},
			{BLACK,COLOR1,COLOR1,COLOR8,COLOR1,COLOR1,BLACK},
			{BLACK,BLACK,COLOR1,COLOR1,COLOR1,BLACK,BLACK}
			
		};

u16 print_fast_tank[7][7]=
		{
			{BLACK,BLACK,BLACK,COLOR2,BLACK,BLACK,BLACK},
			{BLACK,BLACK,BLACK,COLOR2,BLACK,BLACK,BLACK},
			{BLACK,BLACK,COLOR2,COLOR2,COLOR2,BLACK,BLACK},
			{BLACK,COLOR2,COLOR2,COLOR8,COLOR2,COLOR2,BLACK},
			{BLACK,BLACK,COLOR2,COLOR2,COLOR2,BLACK,BLACK},
			{BLACK,COLOR2,COLOR2,COLOR2,COLOR2,COLOR2,BLACK},
			{BLACK,BLACK,COLOR2,BLACK,COLOR2,BLACK,BLACK}
		};


u16 print_firm_tank1[7][7]=
		{
			{BLACK,BLACK,COLOR3,BLACK,COLOR3,BLACK,BLACK},
			{BLACK,BLACK,COLOR3,BLACK,COLOR3,BLACK,BLACK},
			{BLACK,COLOR3,COLOR3,COLOR3,COLOR3,COLOR3,BLACK},
			{COLOR3,COLOR3,COLOR3,COLOR3,COLOR3,COLOR3,COLOR3},
			{COLOR3,COLOR8,COLOR8,COLOR3,COLOR8,COLOR8,COLOR3},
			{COLOR3,COLOR3,COLOR8,COLOR3,COLOR8,COLOR3,COLOR3},
			{BLACK,COLOR3,COLOR3,COLOR3,COLOR3,COLOR3,BLACK}
		};


u16 print_firm_tank2[7][7]=
		{
			{BLACK,BLACK,COLOR4,BLACK,COLOR4,BLACK,BLACK},
			{BLACK,BLACK,COLOR4,BLACK,COLOR4,BLACK,BLACK},
			{BLACK,COLOR4,COLOR4,COLOR4,COLOR4,COLOR4,BLACK},
			{COLOR4,COLOR4,COLOR4,COLOR4,COLOR4,COLOR4,COLOR4},
			{COLOR4,COLOR8,COLOR8,COLOR4,COLOR8,COLOR8,COLOR4},
			{COLOR4,COLOR4,COLOR8,COLOR4,COLOR8,COLOR4,COLOR4},
			{BLACK,COLOR4,COLOR4,COLOR4,COLOR4,COLOR4,BLACK}

		};


u16 print_firm_tank3[7][7]=
		{
			
			{BLACK,BLACK,COLOR5,BLACK,COLOR5,BLACK,BLACK},
			{BLACK,BLACK,COLOR5,BLACK,COLOR5,BLACK,BLACK},
			{BLACK,COLOR5,COLOR5,COLOR5,COLOR5,COLOR5,BLACK},
			{COLOR5,COLOR5,COLOR5,COLOR5,COLOR5,COLOR5,COLOR5},
			{COLOR5,COLOR8,COLOR8,COLOR5,COLOR8,COLOR8,COLOR5},
			{COLOR5,COLOR5,COLOR8,COLOR5,COLOR8,COLOR5,COLOR5},
			{BLACK,COLOR5,COLOR5,COLOR5,COLOR5,COLOR5,BLACK}
		};

u16 print_my_home[8][8]=
		{
			{BLACK,BLACK,BLACK,HOME,HOME,HOME,BLACK,BLACK},
			{BLACK,BLACK,BLACK,HOME,HOME,BLACK,BLACK,BLACK},
			{HOME,HOME,HOME,BLACK,BLACK,HOME,HOME,HOME},
			{BLACK,HOME,HOME,HOME,HOME,HOME,HOME,BLACK},
			{BLACK,BLACK,HOME,HOME,HOME,HOME,BLACK,BLACK},
			{BLACK,BLACK,HOME,BLACK,BLACK,HOME,BLACK,BLACK},
			{BLACK,HOME,BLACK,HOME,HOME,BLACK,HOME,BLACK},
			{HOME,HOME,BLACK,HOME,HOME,BLACK,HOME,HOME}
		};

//ȫ�ֱ���
u16 map[60][60];   //��ͼ��ά����
int bul_num=0;      //�ӵ����
int position;     //λ�ü���,��ӦAI̹������λ��,-1Ϊ��λ��,0Ϊ�м�,1Ϊ��,2Ϊ�ҵ�̹��λ��
int speed = 7;      //��Ϸ�ٶ�,������
int remain_enemy;	//ʣ���������
int plyernum;	//1:���� ��2��˫��
int stgindx;	//��ǰ�ǵڼ���
u8 life1,life2;	//���1����ֵ�����2����ֵ
u32 score1,score2;	//���1���������2����
u8 key1,key2=0;		//���1���룻���2����


typedef struct             //����ĳ��ִ���ָ����һ��AI_tank�����еĴ���
{                          //���趨ÿ��AI_tankÿ������̹��ֻ����һ�� ��fast_tank & firm_tank �����ִ���������1
	int fast_tank_order;   //fast_tank���ֵĴ���(�ڵ�fast_tank_order�θ������,�ӵ�0�ο�ʼ)����ÿ��AI_tankֻ����һ��
	int firm_tank_order;   //firm_tank���ֵĴ���ͬ��
} LevInfo;                 //�ؿ����ֵ�̹����Ϣ
LevInfo level_info [MAX_LEVEL] = { {2,-1},{2,3},{3,2},{1,3},{3,1},{1,2},{2,1},{2,3},{3,2} };   //��ʼ�������� { {-1,3} , {2,3} }  (-1����û�и�����̹��)��ʾ�������أ��ڵڶ��ؿ�̹�˺ͼ��̹�˷ֱ��ڵڶ��͵�����̹������ʱ����


typedef struct      //�ӵ��ṹ��
{
	int x, y;        //�ӵ�����,������
	int direction;  //�ӵ��������������̹�˵ķ���
	bool exist;     //�ӵ��������ı���,1Ϊ���ڣ�0������
	bool initial;   //�ӵ��Ƿ��ڽ�����״̬��ֵ��1Ϊ���ڽ�����״̬��0Ϊ���ڷǽ�����״̬
	int my;        //����AI�ӵ�������ӵ��ı��,0ΪAI�ӵ���1Ϊ���1�ӵ�,2Ϊ���2
} Bullet;
Bullet bullet[BULLET_NUM]; 


typedef struct      //̹�˽ṹ��
{
	int x, y;        //̹����������
	int direction;  //̹�˷���  �����Ļᶨ�����ȷ��̹�˷���
	u16 color;      //��ɫ�η�����,������PrintTank����������˵��
	int model;      //̹��ͼ��ģ�ͣ�ֵΪ1,2,3���ֱ����ͬ��̹��ͼ��,0Ϊ�ҵ�̹��ͼ����AI����ʹ��  //
	int stop;       //ֻ����AI̹��ʹ�õĲ�������0����̹��ֹͣ�߶�,0Ϊ�����߶�
	int revive;     //̹�˸������
	int num;        //AI̹�˱�ţ��̶�ֵ��Ϊ��������ʼ�������ж��£�0~3
	int CD;         //�����ӵ���ȴ��ʱ
	int my;        //�Ƿ�з�̹�˲��������1̹�˴˲���Ϊ1,���2̹��Ϊ2���о�Ϊ0
	bool alive;     //���Ϊ1�������Ϊ0
}  Tank;
Tank AI_tank[4], my_tank,my_tank2; 






//ĳЩ��Ҫ����

int game(void);

//void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);

void printf_home(void); //��ӡ��
void Initialize(void);      //��ʼ��
int GameCheck(void);//�����Ϸ����


//�ӵ�����
void BuildAIBullet(Tank* tank);                //AI̹�˷����ӵ������ж�my_tank�Ķ�ȡ,ֻ��ȡ��my_tank���꣩
void BuildmyBullet(Tank*my_tank);         //һ�����¹�������ִ�д˽����ҵ��ӵ�����
void BuildBullet(Tank tank);                 //�ӵ����䣨���������˻����ã�(��ȫ�ֱ���bullet���޸�)�ҵ�̹�˷����ӵ�ֱ�ӵ��øú���,AIͨ��AIshoot��ӵ���
int BulletFly(Bullet bullet[BULLET_NUM]); //�ӵ��ƶ��ʹ�����˻����ã�,
int BulletHit(Bullet* bullet);            //�ӵ���ײ���˻����ã�(��Tankȫ�ֱ������޸�)��ֻͨ��BulletFly���ã��ӵ������ײ���ڱ�����,�ӵ�����ײ����BulletShoot�м�Ⲣ����
void PrintBullet(int x, int y, int T);         //��ӡ�ӵ����˻����ã�
void ClearBullet(int x, int y, int T);         //����ӵ����˻����ã�
int  BulletCheak(int x, int y);               //�ж��ӵ�ǰ��������˻����ã�

//̹�˲���
void BuildAITank(int* position, Tank* AI_tank); //����AI̹��
void BuildMyTank(Tank* my_tank);                //�����ҵ�̹��
void MoveAITank(Tank* AI_tank);                //AI̹���ƶ�
void MoveMyTank(int turn,Tank* tank);                     //�ҵ�̹���ƶ���ֻͨ��keyboard�������ã��ȼ��̿���
void ClearTank(int x, int y);                  //���̹�ˣ��˻����ã�
void PrintTank(Tank tank);                    //��ӡ̹�ˣ��˻����ã�
bool TankCheak(Tank tank, int direction);      //���̹��dirtection������ϰ���,��ֵ1�谭,0 ��ͨ
int  AIPositionCheak(int position);           //���AI̹�˽���λ���Ƿ����ϰ���

///////************************ ��Ϸ���ֵ�UI���� ****************///////////////
extern mybutton btn_start_game;
extern mybutton btn_edit_map;
extern mybutton btn_select_stage;
extern mybutton btn_power_off;
extern mybutton btn_help;

extern void help(void);//�����ĵ�

mybutton btn_oneplayer={60,163,32,17};
mybutton btn_twoplayer={128,163,32,17};
mybutton btn_quit={196,163,32,17};

myframe frame_pause={30,110,180,60};
mybutton btn_continue={48,135,64,16};
mybutton btn_exit={128,135,64,16};

myframe nextgame={30,80,180,80};
mybutton btn_next={60,135,48,16};

myframe title_enmyleft={245,40,65,16};
myframe title_life={245,100,65,16};
myframe title_score={245,170,65,16};


//���ؼ��
u32 touch_scan_2(void)
{	  	
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)		//������������
		{	
		 	if(tp_dev.x[0]> btn_oneplayer.x && tp_dev.x[0]< btn_oneplayer.x + btn_oneplayer.width 
			&& tp_dev.y[0]> btn_oneplayer.y && tp_dev.y[0]< btn_oneplayer.y + btn_oneplayer.height)
			{			
					return 1;
			}
			else if(tp_dev.x[0]> btn_twoplayer.x && tp_dev.x[0]< btn_twoplayer.x + btn_twoplayer.width 
				   && tp_dev.y[0]> btn_twoplayer.y && tp_dev.y[0]< btn_twoplayer.y + btn_twoplayer.height)
			{			
					return 2;
			}
			else if(tp_dev.x[0]> btn_quit.x && tp_dev.x[0]< btn_quit.x + btn_quit.width 
				   && tp_dev.y[0]> btn_quit.y && tp_dev.y[0]< btn_quit.y + btn_quit.height)
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




///////////****************************************************///////////////////


//ѡ����/˫�ˣ�������Ϸ
void pregame(int stgindex){
	int flag = 1;		//��ǰѡ���İ�ť
	int pre = 5;   	//ǰһ��ѡ���İ�ť
	int tmp;
	int update=1;  	//����Ƿ��л��˰�ť
	u32 key;
	u32 touch=0;
	/********���ؽ���********/
	LCD_Fill(btn_start_game.x - 20,btn_start_game.y-2,btn_select_stage.x+64,btn_select_stage.y+btn_select_stage.height+1,BLACK);
	POINT_COLOR=RED;
	Show_Str(btn_oneplayer.x,btn_oneplayer.y,btn_oneplayer.width,btn_oneplayer.height,"����",16,1);
	Show_Str(btn_twoplayer.x,btn_twoplayer.y,btn_twoplayer.width,btn_twoplayer.height,"˫��",16,1);
	Show_Str(btn_quit.x,btn_quit.y,btn_quit.width,btn_quit.height,"�˳�",16,1);
	LCD_Draw_Circle(26,219,10);	
	POINT_COLOR=WHITE;
	LCD_Draw_Circle(70,219,10);
	POINT_COLOR=RED;
	LCD_ShowChar(67,211,'?',16,1);
	delay_ms(100);
	/***********************/
	while(1){
		touch=touch_scan_2();						//����ɨ��
		key=KEY_Scan(0);								//����ɨ��
		POINT_COLOR=BLACK;
		if(key==6) return;							//�˳�
		/*ȷ��ѡ��İ�ť��������Ӧ����ģ��*/
		if(touch == flag || key==5){
			switch(flag){
				//��ťΪ�����ˡ�
				case 1:{
					delay_ms(50);
					plyernum=1;								//�����Ϊ1
					stgindx=stgindex;					//��ǰ����Ϊ��ѡ����
					life1=MAXLIFE;						//��ʼ�����1����ֵ
					score1=0;									//��ʼ�����1����
					while(game()) stgindx++;	//��ʼ������Ϸ
					return;										
				}
				//��ťΪ��˫�ˡ�
				case 2:{
					delay_ms(50);
					plyernum=2;
					stgindx=stgindex;
					life1=MAXLIFE;
					score1=0;
					life2=MAXLIFE;
					score2=0;
					Remote_Start();   //��ʼ��׽�����źţ����2��
					while(game()) stgindx++;	//��ʼ˫����Ϸ
					Remote_Stop();		//ֹͣ��׽�����ź�					
					return;
				}
				//��ťΪ���˳���
				case 3:{	//�˳�
					return;
				}
				//��ťΪ��������
				case 4:{	//����
					PWR_EnterSTANDBYMode();
					break;
				
				}
				//��ťΪ��������
				case 5:{	//����
					help();
					while(ai_load_picfile("0:/PICTURE/������.bmp",0,0,lcddev.width,lcddev.height,1)); //��ʾ����ͼƬ
					POINT_COLOR=RED;
					Show_Str(btn_oneplayer.x,btn_oneplayer.y,btn_oneplayer.width,btn_oneplayer.height,"����",16,1);
					Show_Str(btn_twoplayer.x,btn_twoplayer.y,btn_twoplayer.width,btn_twoplayer.height,"˫��",16,1);
					Show_Str(btn_quit.x,btn_quit.y,btn_quit.width,btn_quit.height,"�˳�",16,1);
					LCD_Draw_Circle(26,219,10);	
					POINT_COLOR=WHITE;
					LCD_Draw_Circle(70,219,10);
					POINT_COLOR=RED;
					LCD_ShowChar(67,211,'?',16,1);
					update=1;
					break;
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
					//LCD_Fill(btn_oneplayer.x - 20,btn_oneplayer.y-2,btn_oneplayer.x,btn_oneplayer.y+btn_oneplayer.height+1,WHITE);
					LCD_Fill(btn_twoplayer.x - 20,btn_twoplayer.y-2,btn_twoplayer.x,btn_twoplayer.y+btn_twoplayer.height+1,BLACK);
					LCD_Fill(btn_quit.x - 20,btn_quit.y-2,btn_quit.x,btn_quit.y+btn_quit.height+1,BLACK);
					for(int i=0;i<5;i++){
						for(int j=0;j<5;j++){
							LCD_Fill(btn_oneplayer.x - 20+j*4,btn_oneplayer.y-2+i*4,btn_oneplayer.x - 20+(j*4+3),btn_oneplayer.y-2+(i*4+3),print_ui_tank[i][j]);
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
					LCD_Fill(btn_oneplayer.x - 20,btn_oneplayer.y-2,btn_oneplayer.x,btn_oneplayer.y+btn_oneplayer.height+1,BLACK);
					//LCD_Fill(btn_twoplayer.x - 20,btn_twoplayer.y-2,btn_twoplayer.x,btn_twoplayer.y+btn_twoplayer.height+1,WHITE);
					LCD_Fill(btn_quit.x - 20,btn_quit.y-2,btn_quit.x,btn_quit.y+btn_quit.height+1,BLACK);
					for(int i=0;i<5;i++){
						for(int j=0;j<5;j++){
							LCD_Fill(btn_twoplayer.x - 20+j*4,btn_twoplayer.y-2+i*4,btn_twoplayer.x - 20+(j*4+3),btn_twoplayer.y-2+(i*4+3),print_ui_tank[i][j]);
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
					LCD_Fill(btn_oneplayer.x - 20,btn_oneplayer.y-2,btn_oneplayer.x,btn_oneplayer.y+btn_oneplayer.height+1,BLACK);
					LCD_Fill(btn_twoplayer.x - 20,btn_twoplayer.y-2,btn_twoplayer.x,btn_twoplayer.y+btn_twoplayer.height+1,BLACK);
					//LCD_Fill(btn_quit.x - 20,btn_quit.y-2,btn_quit.x,btn_quit.y+btn_quit.height+1,WHITE);
					for(int i=0;i<5;i++){
						for(int j=0;j<5;j++){
							LCD_Fill(btn_quit.x - 20+j*4,btn_quit.y-2+i*4,btn_quit.x - 20+(j*4+3),btn_quit.y-2+(i*4+3),print_ui_tank[i][j]);
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
					LCD_Fill(btn_oneplayer.x - 20,btn_oneplayer.y-2,btn_oneplayer.x,btn_oneplayer.y+btn_oneplayer.height+1,BLACK);
					LCD_Fill(btn_twoplayer.x - 20,btn_twoplayer.y-2,btn_twoplayer.x,btn_twoplayer.y+btn_twoplayer.height+1,BLACK);
					LCD_Fill(btn_quit.x - 20,btn_quit.y-2,btn_quit.x,btn_quit.y+btn_quit.height+1,BLACK);
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
					LCD_Fill(btn_oneplayer.x - 20,btn_oneplayer.y-2,btn_oneplayer.x,btn_oneplayer.y+btn_oneplayer.height+1,BLACK);
					LCD_Fill(btn_twoplayer.x - 20,btn_twoplayer.y-2,btn_twoplayer.x,btn_twoplayer.y+btn_twoplayer.height+1,BLACK);
					LCD_Fill(btn_quit.x - 20,btn_quit.y-2,btn_quit.x,btn_quit.y+btn_quit.height+1,BLACK);
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







//��ͣ��Ϸ
//����ֵ��0������Ϸ 1�˳���Ϸ
int pause(int plyrnum){
	u8 key1,key2=0;
	int flag=1; //��ǰѡ����ť
	/***���ؽ���***/
	LCD_Fill(frame_pause.x,frame_pause.y,frame_pause.x+frame_pause.width,frame_pause.y+frame_pause.height,GRAY);
	POINT_COLOR=YELLOW;
	Show_Str(btn_continue.x,btn_continue.y,64,16,"������Ϸ",16,1);
	POINT_COLOR=RED;
	Show_Str(btn_exit.x,btn_exit.y,64,16,"�˳���Ϸ",16,1);
	while(1){
		key1=KEY_Scan(0);
		if(plyrnum==2) key2=Remote_Scan();	//���2����ú���ң����ͣ��Ϸ
		//����ѡ����ť
		if(key1==3 || key1==4 || key2==3 || key2==4)
		{
			if(flag==1){
				flag=2;
				POINT_COLOR=RED;
				Show_Str(btn_continue.x,btn_continue.y,64,16,"������Ϸ",16,1);
				POINT_COLOR=YELLOW;
				Show_Str(btn_exit.x,btn_exit.y,64,16,"�˳���Ϸ",16,1);
			}
			else{
				flag=1;
				POINT_COLOR=YELLOW;
				Show_Str(btn_continue.x,btn_continue.y,64,16,"������Ϸ",16,1);
				POINT_COLOR=RED;
				Show_Str(btn_exit.x,btn_exit.y,64,16,"�˳���Ϸ",16,1);
			}
		}
		//ȷ��ѡ����ť
		else if(key1==5 || key2==5){
			delay_ms(50);
			return (flag-1);		//��ѡ�����ֵ
		}
		//��ȡ����ֱ�Ӽ�����Ϸ
		else if(key1==6 || key2==6){
			delay_ms(50);
			return 0;        //������Ϸ
		}
	}
}


//ͨ��һ�ػ���Ҷ�����֮����ʾ��Ӧ���棬ѡ�������һ�ػ������Ϸ
//����ֵ��1=��һ�أ�0=�˳���Ϸ
int game_end(int win){
	u8* buf;
	POINT_COLOR=RED;
	buf=mymalloc(0,512);
	SD_ReadDisk(buf,MapNumAddr,1);
	if(win){	//Ӯ��
		if(stgindx==*buf){	//��ǰ�ؿ�Ϊ���һ�أ���ͨ��
			LCD_Fill(20,65,220,215,GRAY);
			SD_ReadDisk(buf,HighScoreAddr,1);
			Show_Str(77,72,192,24,"CONGRATS",24,1);
			Show_Str(84,96,96,24,"ͨ������",24,1);
			if(plyernum==1){	//�����Ϊ1
				Show_Str(90,120,128,16,"�÷� ",16,1);
				LCD_ShowxNum(138,120,score1,5,16,1);
				Show_Str(42,136,176,16,"��ʷ��߷� ",16,1);
				LCD_ShowxNum(138,136,*((u32*)buf),5,16,1);
				if(score1 > *((u32*)buf))
				{
					Show_Str(84,152,96,24,"�¼�¼��",24,1);
					*((u32*)buf)=score1;
					SD_WriteDisk(buf,HighScoreAddr,1);
				}
			}
			if(plyernum==2){	//�����Ϊ2
				Show_Str(42,120,128,16,"���1�÷� ",16,1);
				LCD_ShowxNum(138,120,score1,5,16,1);
				Show_Str(42,136,128,16,"���2�÷� ",16,1);
				LCD_ShowxNum(138,136,score2,5,16,1);
				Show_Str(42,152,176,16,"��ʷ��߷� ",16,1);
				LCD_ShowxNum(138,152,*((u32*)buf),5,16,1);
				if(score1>*((u32*)buf)||score2>*((u32*)buf))
				{
					Show_Str(84,168,96,24,"�¼�¼��",24,1);
					*((u32*)buf)=(score1>score2 ? score1 : score2);
					SD_WriteDisk(buf,HighScoreAddr,1);
				}
			}
			myfree(0,buf);
			int cnt=5;
			while(cnt){				//����ʱ5�룬֮���˳���Ϸ
				LCD_ShowxNum(124,192,cnt--,1,16,0);
				delay_ms(1000);
			}
			return 0;
		}
		else{	 							//Ӯ�ˣ�����ûͨ�أ���Ҫ��Ҫ������һ��						
			u8 key1,key2=0;
			int flag=1;	//1=��һ�أ�0=�˳���Ϸ
			LCD_Fill(nextgame.x,nextgame.y,nextgame.x+nextgame.width,nextgame.y+nextgame.height,GRAY);
			Show_Str(nextgame.x+42,nextgame.y+16,96,16,"��ϲͨ������",16,1);
			POINT_COLOR=YELLOW;
			Show_Str(btn_next.x,btn_next.y,48,16,"��һ��",16,1);
			POINT_COLOR=RED;
			Show_Str(btn_exit.x,btn_exit.y,48,16,"������",16,1);
			while(1){
				key1=KEY_Scan(0);
				if(plyernum==2) key2=Remote_Scan();
				if(key1==3 || key1==4 || key2==3 || key2==4)
				{
					if(flag==1){
						flag=0;
						POINT_COLOR=RED;
						Show_Str(btn_next.x,btn_next.y,48,16,"��һ��",16,1);
						POINT_COLOR=YELLOW;
						Show_Str(btn_exit.x,btn_exit.y,48,16,"������",16,1);
					}
					else{
						flag=1;
						POINT_COLOR=YELLOW;
						Show_Str(btn_next.x,btn_next.y,48,16,"��һ��",16,1);
						POINT_COLOR=RED;
						Show_Str(btn_exit.x,btn_exit.y,48,16,"������",16,1);
					}
				}
				else if(key1==5 || key2==5){
					if(flag==0){
					SD_ReadDisk(buf,HighScoreAddr,1);
					LCD_Fill(20,65,220,215,GRAY);
					POINT_COLOR=RED;
					Show_Str(46,84,144,24,"��ӭ�ٴ���ս",24,1);
					if(plyernum==1){
						Show_Str(90,120,128,16,"�÷� ",16,1);
						LCD_ShowxNum(138,120,score1,5,16,1);
						Show_Str(42,136,176,16,"��ʷ��߷� ",16,1);
						LCD_ShowxNum(138,136,*((u32*)buf),5,16,1);
						if(score1>*((u32*)buf))
						{
							Show_Str(84,152,96,24,"�¼�¼��",24,1);
							*((u32*)buf)=score1;
							SD_WriteDisk(buf,HighScoreAddr,1);
						}
					}
					if(plyernum==2){
						Show_Str(42,120,128,16,"���1�÷� ",16,1);
						LCD_ShowxNum(138,120,score1,5,16,1);
						Show_Str(42,136,128,16,"���2�÷� ",16,1);
						LCD_ShowxNum(138,136,score2,5,16,1);
						Show_Str(42,152,176,16,"��ʷ��߷� ",16,1);
						LCD_ShowxNum(138,152,*((u32*)buf),5,16,1);
						if(score1>*((u32*)buf)||score2>*((u32*)buf))
						{
							Show_Str(84,168,96,24,"�¼�¼��",24,1);
							*((u32*)buf)=(score1>score2 ? score1 : score2);
							SD_WriteDisk(buf,HighScoreAddr,1);
						}
					}
					int cnt=5;
					while(cnt){
							LCD_ShowxNum(124,192,cnt--,1,16,0);
							delay_ms(1000);
						}
					}
					myfree(0,buf);
					return flag;		//��ѡ�����ֵ
				}
			}
		}
	}
	else{		//����
		SD_ReadDisk(buf,HighScoreAddr,1);
		LCD_Fill(20,65,220,215,GRAY);
		Show_Str(94,72,192,24,"FAILL",24,1);
			Show_Str(72,96,96,24,"��Ϸ����",24,1);
			if(plyernum==1){	//�����Ϊ1
				Show_Str(90,120,128,16,"�÷� ",16,1);
				LCD_ShowxNum(138,120,score1,5,16,1);
				Show_Str(42,136,176,16,"��ʷ��߷� ",16,1);
				LCD_ShowxNum(138,136,*((u32*)buf),5,16,1);
				if(score1>*((u32*)buf))
				{
					Show_Str(84,152,96,24,"�¼�¼��",24,1);
					*((u32*)buf)=score1;
					SD_WriteDisk(buf,HighScoreAddr,1);
				}
			}
			if(plyernum==2){	//�����Ϊ2
				Show_Str(42,120,128,16,"���1�÷� ",16,1);
				LCD_ShowxNum(138,120,score1,5,16,1);
				Show_Str(42,136,128,16,"���2�÷� ",16,1);
				LCD_ShowxNum(138,136,score2,5,16,1);
				Show_Str(42,152,176,16,"��ʷ��߷� ",16,1);
				LCD_ShowxNum(138,152,*((u32*)buf),5,16,1);
				if(score1>*((u32*)buf)||score2>*((u32*)buf))
				{
					Show_Str(84,168,96,24,"�¼�¼��",24,1);
					*((u32*)buf)=(score1>score2 ? score1 : score2);
					SD_WriteDisk(buf,HighScoreAddr,1);
				}
			}
			myfree(0,buf);
			int cnt=5;
			while(cnt){			//����ʱ5�룬֮���˳���Ϸ
				LCD_ShowxNum(124,192,cnt--,1,16,0);
				delay_ms(1000);
			}
			return 0;
	}
}




//	RNG_Get_RandomRange(min,max); ��������� [min,max]

void load_game_slide(void){
	LCD_Fill(240,0,319,239,GRAY);//�����
	POINT_COLOR=BLUE; 
	Show_Str(257,8,48,16,"�� ��",16,1);
	LCD_ShowxNum(257+16,8,stgindx,1,16,1);
	POINT_COLOR=BLACK; 
	Show_Str(title_enmyleft.x,title_enmyleft.y,title_enmyleft.width,title_enmyleft.height,"ʣ��о�",16,1);
	LCD_ShowxNum(title_enmyleft.x,title_enmyleft.y+16,remain_enemy,2,16,0);
	Show_Str(title_life.x,title_life.y,title_life.width,title_life.height,"����ֵ",16,1);
	LCD_ShowxNum(title_life.x,title_life.y+16,life1,1,16,0);
	if(plyernum==2) LCD_ShowxNum(title_life.x,title_life.y+32,life2,1,16,0);
	Show_Str(title_score.x,title_score.y,65,16,"�÷�",16,1);
	LCD_ShowxNum(title_score.x,title_score.y+16,score1,5,16,0);
	if(plyernum==2) LCD_ShowxNum(title_score.x,title_score.y+32,score2,5,16,0);
}

//��Ϸ
int game(){
	u8* buf1;
	int i,j,rval;
	remain_enemy=16;	//����ģʽ�£���1��2��15�����ˣ���3��4��5��20�����ˣ���6��7��8��25�����ˣ���9��30������
	if(plyernum==2) remain_enemy=remain_enemy*2; //˫��ģʽ�µ�����������
	load_game_slide(); //���ز����
	//����ս��
	LCD_Fill(0,0,239,239,BLACK);
	buf1=mymalloc(0,512*8);
	POINT_COLOR=RED;
	while(SD_ReadDisk(buf1,MapAddr+(stgindx-1)*8,8)) Show_Str(120,150,100,16,"��ͼ����ʧ��!",16,0);
	for(i=0;i<60;i++)
		for(j=0;j<60;j++){
			map[i][j] = colorlist[*(buf1+i*60+j)];
			LCD_Fill(j*4,i*4,j*4+3,i*4+3,map[i][j]);
		}
		//myframe base={104,204,32,32};
	for(i=26;i<=33;i++){
		for(j=51;j<=58;j++){
			LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
		}
	}
	myfree(0,buf1);
	delay_ms(100);
	u8 interval[14] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1 };  //������������飬���ڿ����ٶ�
	Initialize();                         //��ʼ����ȫ�ֱ���level��ֵ����1 
	while(1){
		rval = GameCheck();                //��Ϸʤ�����
		if(rval==0 || rval==1) return rval;
		key1=KEY_Scan(1); 
		if(plyernum==2) key2=Remote_Scan();
		if(key1==6 || key2==6) {	//��ͣ
			u16 screen[60][60];	//��һ����ά�������Ļ��ʾ���Ա�ָ���ʾ
			for(i=0;i<60;i++){
				for(j=0;j<60;j++){
					screen[i][j] = LCD_ReadPoint(i*4,j*4);
				}
			}
			if(key1==6){
				delay_ms(20);
				if(KEY_Scan(1)==6){ 
					if(pause(plyernum)==0)	//������Ϸ
					{
						delay_ms(100);
						for(i=0;i<60;i++)
							for(j=0;j<60;j++){
								LCD_Fill(i*4,j*4,i*4+3,j*4+3,screen[i][j]);
						}
					}
					else	//�˳���Ϸ
					{
						return 0;
					}
				}
			}
			else if(pause(plyernum)==0)	//������Ϸ
			{
				delay_ms(100);
				for(i=0;i<60;i++)
					for(j=0;j<60;j++){
						LCD_Fill(i*4,j*4,i*4+3,j*4+3,screen[i][j]);
					}
			}
			else	//�˳���Ϸ
			{
				return 0;
			}
		}
		else if(key1==1 || key1==2 ||key1==3 || key1==4)
		{
			MoveMyTank(key1,&my_tank);
		}
		if(key2==1 || key2==2 ||key2==3 || key2==4)
		{
			MoveMyTank(key2,&my_tank2);
		}
		rval=BulletFly(bullet);
		delay_ms(15);
		if(rval==0 || rval==1) return rval;
			for (i = 0; i <= 3; i++)         //AI̹���ƶ�ѭ��
			{
				if (AI_tank[i].model == 2 && interval[i]++ % 2 == 0) //�ĸ�̹���еĿ���̹�˵���ʹ�ü�����1,2,3,4
					MoveAITank(&AI_tank[i]);
				if (AI_tank[i].model != 2 && interval[i + 5]++ % 3 == 0) //�ĸ�̹���е�����̹�˵���ʹ�ü�����5,6,7,8
					MoveAITank(&AI_tank[i]);
			}
			int x=(plyernum==1? 4: 8);
			for (i = 0; i <= 3; i++){//����AI̹�˲���
				if(AI_tank[i].alive == 0 && AI_tank[i].revive < x && interval[9]++ % 90 == 0)  //һ���з�̹��ÿ��ֻ��4����
				{                                               //���̹�˲�����ʱ,ÿ�ν����м��  1750 ms
					BuildAITank(&position, &AI_tank[i]);     //����AI̹�ˣ����
					break;                                      //ÿ��ѭ��ֻ����һ��̹��
				}
			}
			for (i = 0; i <= 3; i++)
				if (AI_tank[i].alive)
					BuildAIBullet(&AI_tank[i]);                 //AIshoot�Դ�int��������CD,��ʹ��main�е�CD interval
			if (my_tank.alive && interval[10]++ % 2 == 0)
				BuildmyBullet(&my_tank);
			if (my_tank.alive == 0 && interval[11]++ % 30 == 0 && my_tank.revive < MAXLIFE)
				BuildMyTank(&my_tank);
			if (plyernum==2 && my_tank2.alive && interval[12]++ % 2 == 0)
				BuildmyBullet(&my_tank2);
			if (plyernum==2 && my_tank2.alive == 0 && interval[13]++ % 30 == 0 && my_tank2.revive < MAXLIFE)
				BuildMyTank(&my_tank2);
	LCD_ShowxNum(title_enmyleft.x,title_enmyleft.y+16,remain_enemy,2,16,0);
	LCD_ShowxNum(title_life.x,title_life.y+16,life1,1,16,0);
	if(plyernum==2) LCD_ShowxNum(title_life.x,title_life.y+32,life2,1,16,0);
	LCD_ShowxNum(title_score.x,title_score.y+16,score1,5,16,0);
	if(plyernum==2) LCD_ShowxNum(title_score.x,title_score.y+32,score2,5,16,0);
		delay_ms(10);
	}
}


void BuildAIBullet(Tank* tank)   //AI�ӵ�����   ���������AI̹�����չ��򿪻�ĺ�������������
{
	if (tank->CD == 15)
	{
		if (!(RNG_Get_RandomNum() % 11))     //��ȴ������������ÿ����Ϸ��������10��֮һ�Ŀ��ܷ����ӵ�
		{
			BuildBullet(*tank);
			tank->CD = 0;
		}
	}
	else
		tank->CD++;
	if (tank->CD >= 14)       //AIǿ�����֣�����ȴ����һ����Χ����ʹ��
	{
		if (tank->y == YMAX-TANKWI/2)     //���̹���ڵײ�(���������)
		{
			if (tank->x < XMED-HOME_WIDTH/2) //���ϼ����
		
				if (tank->direction == RIGHT)  //̹�˷�����
				{
					BuildBullet(*tank);     //�����ӵ�
					tank->CD = 0;
				}
			}
			else             //���ϼ��ұ�
				if (tank->direction == LEFT)   //̹�˷�����
				{
					BuildBullet(*tank);     //�����ӵ�
					tank->CD = 0;
				}
		}
		if(plyernum==1){
		if (tank->x == my_tank.x +TANKWI/2 || tank->x == my_tank.x || tank->x == my_tank.x - TANKWI/2)  //AI̹����������"�ڿ�"��׼�ҵ�̹�� ��AI tank�����̹����������λ����ʱAIҪ���ڿڶ�׼���   
		{
			if ((tank->direction == DOWN && my_tank.y > tank->y) || (tank->direction == UP && my_tank.y < tank->y))
			{                               //����AI���²����ҵ�̹����AI̹���·�(��ֵ���������)����AI�����ҵ�̹����AI�Ϸ�
				int big = my_tank.y, smal = tank->y, i;
				if (my_tank.y < tank->y)
				{
					big = tank->y;
					smal = my_tank.y;
				}
				for (i = smal + TANKWI/2+1; i <= big - (TANKWI/2+1); i++)  //�ж�AI�ڿڵ�ֱ������̹�˼������ϰ�  
					if (map[i][tank->x] != BLACK||map[i][tank->x] !=WATER )      //�����ϰ� ��������ϰ��ж�Ϊ��ȡ��ɫ�ж��Ƿ����ϰ�
						break;
				if (i == big - TANKWI/2)                //��i�ߵ�big-?˵�����ϰ�  
				{
					BuildBullet(*tank);     //�����ӵ�
					tank->CD = 0;
				}
			}
		}
		else if (tank->y == my_tank.y + TANKWI/2|| tank->y == my_tank.y || tank->y == my_tank.y - TANKWI/2) //AI̹���ں�����"�ڿ�"��׼�ҵ�̹��    //�˲����ǽ��ϲ��ֵķ���ı䣬���AI tank��������
		{
			if (tank->direction == RIGHT && my_tank.x > tank->x || tank->direction == LEFT && my_tank.x < tank->x)
			{                  //����AI���Ҳ����ҵ�̹����AI̹���ҷ�(��ֵ���������)����AI�����ҵ�̹����AI��
				int big = my_tank.y, smal = tank->y, i;
				if (my_tank.x < tank->x)
				{
					big = tank->x;
					smal = my_tank.x;
				}
				for (i = smal + TANKWI/2+1; i <= big - (TANKWI/2+1); i++)  //�ж�AI�ڿڵ�ֱ������̹�˼������ϰ�
					if (map[tank->y][i] != BLACK ||map[i][tank->x] !=WATER )      //�����ϰ�
						break;
				if (i == big - TANKWI/2)   //��i�ߵ�big-TANKWI/2˵�����ϰ�
				{
					BuildBullet(*tank);     //�����ӵ�
					tank->CD = 0;
				}
			}
		}
	}
	if(plyernum==2){
		if (tank->x == my_tank2.x +TANKWI/2 || tank->x == my_tank2.x || tank->x == my_tank2.x - TANKWI/2)  //AI̹����������"�ڿ�"��׼�ҵ�̹�� ��AI tank�����̹����������λ����ʱAIҪ���ڿڶ�׼���   
		{
			if ((tank->direction == DOWN && my_tank2.y > tank->y) || (tank->direction == UP && my_tank2.y < tank->y))
			{                               //����AI���²����ҵ�̹����AI̹���·�(��ֵ���������)����AI�����ҵ�̹����AI�Ϸ�
				int big = my_tank2.y, smal = tank->y, i;
				if (my_tank2.y < tank->y)
				{
					big = tank->y;
					smal = my_tank2.y;
				}
				for (i = smal + TANKWI/2+1; i <= big - (TANKWI/2+1); i++)  //�ж�AI�ڿڵ�ֱ������̹�˼������ϰ�  
					if (map[i][tank->x] != BLACK||map[i][tank->x] !=WATER )      //�����ϰ� ��������ϰ��ж�Ϊ��ȡ��ɫ�ж��Ƿ����ϰ�
						break;
				if (i == big - TANKWI/2)                //��i�ߵ�big-?˵�����ϰ�  
				{
					BuildBullet(*tank);     //�����ӵ�
					tank->CD = 0;
				}
			}
		}
		else if (tank->y == my_tank2.y + TANKWI/2|| tank->y == my_tank2.y || tank->y == my_tank2.y - TANKWI/2) //AI̹���ں�����"�ڿ�"��׼�ҵ�̹��    //�˲����ǽ��ϲ��ֵķ���ı䣬���AI tank��������
		{
			if (tank->direction == RIGHT && my_tank2.x > tank->x || tank->direction == LEFT && my_tank2.x < tank->x)
			{                  //����AI���Ҳ����ҵ�̹����AI̹���ҷ�(��ֵ���������)����AI�����ҵ�̹����AI��
				int big = my_tank2.y, smal = tank->y, i;
				if (my_tank2.x < tank->x)
				{
					big = tank->x;
					smal = my_tank2.x;
				}
				for (i = smal + TANKWI/2+1; i <= big - (TANKWI/2+1); i++)  //�ж�AI�ڿڵ�ֱ������̹�˼������ϰ�
					if (map[tank->y][i] != BLACK ||map[i][tank->x] !=WATER )      //�����ϰ�
						break;
				if (i == big - TANKWI/2)   //��i�ߵ�big-TANKWI/2˵�����ϰ�
				{
					BuildBullet(*tank);     //�����ӵ�
					tank->CD = 0;
				}
			}
		}
	}
}

void BuildmyBullet(Tank*my_tank)
{
	if (my_tank->CD == 5)
	{
		if (key1==5&& my_tank->my==1)//�˴�Ϊ���鹥�������룬�Ҳ���
		{
			BuildBullet(*my_tank);
			my_tank->CD = 0;
		}
		else if(key2==5&& my_tank->my==2){
			BuildBullet(*my_tank);
			my_tank->CD = 0;
		}
	}
	else
		my_tank->CD++;
}

void BuildBullet(Tank tank)  //�ӵ����䣨������,����ṹ��Tank,��������ı���ȫ�ֱ����ṹ��bullet
{                            //��ʵ�ַ�ʽΪ˳��ѭ�������ӵ���ÿ�ε��øı��bullet����Ԫ�ض���ͬ
	switch (tank.direction)   //��Ϊ�˷���,����bullet�������,bullet��Ϊȫ�ֱ���ʹ��
	{
	case UP:
		bullet[bul_num].x = tank.x;
		bullet[bul_num].y = tank.y - (TANKWI/2+1);
		bullet[bul_num].direction = UP;                              //�����ӵ�����һ��ʼ�Ѷ���
		break; 
	case DOWN:
		bullet[bul_num].x = tank.x;
		bullet[bul_num].y = tank.y + (TANKWI/2+1);
		bullet[bul_num].direction = DOWN;
		break;
	case LEFT:
		bullet[bul_num].x = tank.x - (TANKWI/2+1);
		bullet[bul_num].y = tank.y;
		bullet[bul_num].direction = LEFT;
		break;
	case RIGHT:
		bullet[bul_num].x = tank.x + (TANKWI/2+1);
		bullet[bul_num].y = tank.y;
		bullet[bul_num].direction = RIGHT;
		break;
	}
	bullet[bul_num].exist = 1;    //�ӵ�������,��ֵΪ1����ӵ�����
	bullet[bul_num].initial = 1;  //�ӵ����ڳ�����״̬
	bullet[bul_num].my = tank.my;   //��������1̹�˷�����ӵ�bullet.my=1�����2��2������Ϊ0
	bul_num++;
	if (bul_num == BULLET_NUM)        //����ӵ��������������ӵ�������ô��ͷ��ʼ���
		bul_num = 0;                 
}



int BulletFly(Bullet bullet[]) //�ӵ��ƶ��ʹ��
{                                         //����ȫ�ֱ���Bullet�ĸı�
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].exist)              //����ӵ�����
		{
			if (bullet[i].initial == 0)     //����ӵ����ǳ�������
			{
				if (map[bullet[i].y][bullet[i].x] == BLACK || map[bullet[i].y][bullet[i].x] == WATER)   //����ӵ����굱ǰλ�����ϰ�
					ClearBullet(bullet[i].x, bullet[i].y, BulletCheak(bullet[i].x, bullet[i].y));     //Ĩ���ӵ�ͼ��
				switch (bullet[i].direction)                                      //Ȼ���ӵ�����仯���ӵ��䵽��һ�����꣩
				{
				case UP:(bullet[i].y)--; break;
				case DOWN:(bullet[i].y)++; break;
				case LEFT:(bullet[i].x)--; break;
				case RIGHT:(bullet[i].x)++; break;
				}
			}
			int collide = BulletCheak(bullet[i].x, bullet[i].y);   //�ж��ӵ���ǰλ�����,�ж��ӵ��Ƿ���ײ,�Ƿ�λ��ˮ���ϡ�
			if (collide)                                                //�����⵽��ǰ�ӵ��������ϰ�(����ײ)�������ڵ���������ˮ���ϣ�
				PrintBullet(bullet[i].x, bullet[i].y, collide);       //���ӡ�ӵ���������ײ�򲻴�ӡ
			else{
				
				return BulletHit(&bullet[i]);     //������ײ��ִ���ӵ���ײ����  
			}
			if (bullet[i].initial)             //���ӵ�����������ѳ��������ȥ��
				bullet[i].initial = 0;
			for (int j = 0; j < BULLET_NUM; j++)  //�ӵ������ײ�ж�,�����ҷ��ӵ��͵з��ӵ���ײ��ɾ��,��Ϊ���з��ӵ�������
				if (bullet[j].exist && j != i  && bullet[i].x == bullet[j].x && bullet[i].y == bullet[j].y)
				{                              //ͬ���������ҷ��ӵ������ܲ�����ײ���迼��
					bullet[j].exist = 0;
					bullet[i].exist = 0;
					ClearBullet(bullet[j].x, bullet[j].y, BulletCheak(bullet[j].x, bullet[j].y));  //Ĩ��j�ӵ�ͼ��,�ӵ�iͼ���ѱ�Ĩ��
					break;
				}
		}
	}
	return 2;
}




int BulletHit(Bullet* bullet)  //����Tankȫ�ֱ������޸�,�ӵ������ײ���ڱ�����,�ӵ�����ײ����BulletShoot�м�Ⲣ�����ϸ�����β��
{                               //��ÿ�δ��е�̹�˶���һ���������ܰ�����̹�˷��ڲ�������
	int x = bullet->x;            //�������Tankʹ��ȫ�ֱ���
	int y = bullet->y;            //���ﴫ���ֵ���ӵ�����,������ֵ����Ҫ�ı�
	int i;
	if (map[y][x] == WALL1)  //�ӵ����������ƻ�ש�� 
	{
		if (bullet->direction == UP || bullet->direction == DOWN)   //����ӵ��������
			for (i = -2; i <= 2; i++)
				if (map[y][x + i] == WALL1)  //����ӵ�����ש������Ϊש��,��ɾ��ש,������(һ��Ϊ̹�˻���������)�����
				{
					map[y][x + i] = BLACK;
					LCD_Fill((x+i)*4,y*4,(x+i)*4+3,y*4+3,BLACK);
				}
		if (bullet->direction == LEFT || bullet->direction == RIGHT)     //���ӵ��Ǻ����  (���ӵ�����ʵ��ͬ��)
			for (i = -2; i <= 2; i++)
				if (map[y + i][x] == WALL1)
				{
					 map[y + i][x] = BLACK;
					 LCD_Fill(x*4,(y+i)*4,x*4+3,(y+i)*4+3,BLACK);
				}
		bullet->exist = 0;           //����ӵ��Ѿ���������
	}
	else if (map[y][x] == WALL3 ||map[y][x]==WALL4)  //�ӵ������߿���߲��ɴݻٷ���
		bullet->exist = 0;
	else if (bullet->my &&map[y][x] >= 100 && map[y][x] < 104)  //���ҵ��ӵ������˵з�̹��
	{
		int num = map[y][x] % 100;
		if (AI_tank[num].model == 3 && AI_tank[num].color == COLOR3)   //��Ϊfirm tank,��Ϊ��һ��̬,����û���ܵ��˺�  
			AI_tank[num].color = COLOR4;                        //����color4
		else if (AI_tank[num].model == 3 && AI_tank[num].color == COLOR4)
			AI_tank[num].color = COLOR5;                        //4Ϊ��ɫ
		else                       //�������͵�̹�˻���firm tankΪ��ɫ�����
		{if(bullet->my==1){
			if(AI_tank[num].model==1)	//����ҷ�
				score1+=100;
			if(AI_tank[num].model==2)
				score1+=300;
			if(AI_tank[num].model==3)
				score1+=500;
			}
			else if(bullet->my==2){
			if(AI_tank[num].model==1)	//����ҷ�
				score2+=100;
			if(AI_tank[num].model==2)
				score2+=300;
			if(AI_tank[num].model==3)
				score2+=500;
			}
			AI_tank[num].alive = 0;
			remain_enemy--;
			ClearTank(AI_tank[num].x, AI_tank[num].y);      //�����̹��
		}
		bullet->exist=0;
	}
	else if (map[y][x] == COLOR0 &&my_tank.alive==1&& bullet->my!=my_tank.my)   //���з��ӵ��������1̹��
	{
		my_tank.alive = 0;
		ClearTank(my_tank.x, my_tank.y);
		bullet->exist = 0;
		      //�ҵ�̹�˸������+1(���ҵ�̹�˸������������ֵ�йء���������Լ�)
		life1--;
	}		
		else if (map[y][x] == COLOR7 &&my_tank2.alive==1&& bullet->my!=my_tank2.my)   //���з��ӵ��������2̹��
	{
		my_tank2.alive = 0;
		ClearTank(my_tank2.x, my_tank2.y);
		bullet->exist = 0;
		      //�ҵ�̹�˸������+1(���ҵ�̹�˸������������ֵ�йء���������Լ�)
		life2--;
	}	
	else if(bullet->my==0 && (100 <=map[y][x]&&map[y][x]<=103) )
			bullet->exist=0;
	else if (map[y][x] == HOME)      //�ӵ�������(������˭���ӵ�)
	{
		delay_ms(1000);
		return game_end(0);
	}
	return 2;
}

int BulletCheak(int x, int y)  //�ж��ӵ���ǰλ�����,�ж��ӵ��Ƿ���ײ,�Ƿ�λ��ˮ���ϡ�
{                              //���ϰ�����0,���ϰ����ӵ��ڵ��淵��1���ӵ���ˮ���Ϸ���2
	if (map[y][x] == BLACK)
		return 1;
	else if (map[y][x] == WATER)//ˮ����ɫ
		return 2;
	else
		return 0;
}


void PrintBullet(int x, int y, int T)   //��ǰ����BulletCheak ��ֵ������ T
{
	 LCD_Fill(x*4,y*4,x*4+3,y*4+3,YELLOW);
}

void ClearBullet(int x, int y, int T)   //�ظ���ǰ����
{
	if (T == 2)        //  T==2 ��ʾ�ӵ���ǰ������ˮ����
	{
		 LCD_Fill(x*4,y*4,x*4+3,y*4+3,BLUE);
	}
	else if (T == 1)   //  T==1 ��ʾ�ӵ���ǰ������½����
	{
		 LCD_Fill(x*4,y*4,x*4+3,y*4+3,BLACK);
	}
}

//positionΪ̹������λ��,-1Ϊ��λ��,0Ϊ�м�,1Ϊ��,2Ϊ�ҵ�̹��λ��
void BuildAITank(int* position, Tank* AI_tank)   //ִ��һ�θú���ֻ����һ��̹��
{                                         //rand������ʽ��0<=rand()%(a+1)<=a  0+m<=rand()%(n-m+1)+m<=n  
										  //rand����ʵ��1��n��1<=rand()%(n)+1<=n
	if (AIPositionCheak(*position))        //����λ�����ϰ�,�����ɡ�position�������AIPositionCheak��������
	{
		AI_tank->x =(*position==1? 55 : XMED + (XMED-(TANKWI/2+1+1)) * (*position));  //��Ӧ��������λ�õ�x������
		AI_tank->y = TANKWI/2+1;
		if (AI_tank->revive == level_info[stgindx - 1].firm_tank_order)  //̹�˳���(����)����==�ؿ���Ϣ(level_info)��firm tank�ĳ��ִ���
		{
			AI_tank->model = 3;           //?Ϊfirm tank��ģ��(���) 
			AI_tank->color = COLOR3;           //��ɫ����
		}
		else if (AI_tank->revive == level_info[stgindx - 1].fast_tank_order)  //ͬ��if��������fast_tank��
		{
			AI_tank->model = 2;
			AI_tank->color = COLOR2; 
		}
		else      //��̹ͨ��
		{
			AI_tank->model = 1;
			AI_tank->color = COLOR1; 
		}
		AI_tank->alive = 1;       //̹�˱�Ϊ����
		AI_tank->direction = DOWN;  //������
		AI_tank->revive++;        //�������+1
		PrintTank(*AI_tank);
		(*position)++;
		{
			}//�����ڸ���Ļ�ϴ�ӡʣ��̹��������
		if (*position == 2)          //positionֻ��Ϊ0,1��-1������positionѭ������
			* position = -1;
		return;                  //��������һ��̹�ˣ�������ú���
	}
}


int AIPositionCheak(int position)    //positionΪ̹������λ��2Ϊ�ҵ�̹��λ�ã�����ΪAIλ��-1Ϊ��λ��0Ϊ�м�λ�ã�1Ϊ��λ
{
	int	x, y;

		y = 4, x =(position==1? 55 : XMED + (XMED-(TANKWI/2+1+1)) * (position));  //XMED + (XMED-(TANKWI/2+1)) * (*position) ��Ӧ��������λ�õ�x������
	for (int i = -TANKWI/2; i <=TANKWI/2; i++)
		for (int j = -TANKWI/2; j <=TANKWI/2; j++)
			if (map[y + j ][x + i ] != BLACK)  //���������X���������ϰ���
				return 0;              //�򷵻�0����ʾ������λ�����谭
	return 1;                          //��������1����ʾ������λ�����谭
}


void MoveAITank(Tank* AI_tank) //AIר�ú������ú�����ҪΪAI��ǿ
{
	if (AI_tank->alive)         //���̹�˻���
	{
		if (AI_tank->stop != 0)   //̹���Ƿ�ֹͣ�˶����жϣ���stop������Ϊ0
		{
			AI_tank->stop--;   //���̹�˱��غ�ֹͣ�˶�
			return;
		}
		if (!(RNG_Get_RandomNum() % 23))     //22��֮1�ĸ���ִ�з�������
		{
			AI_tank->direction = RNG_Get_RandomNum() % 4 + 1;
			if (RNG_Get_RandomNum() % 3)     //�ڷ������ú���2��֮1�ĸ���ֹͣ�߶�3����ʱ��
			{
				AI_tank->stop = 2;
				return;
			}
		}
		ClearTank(AI_tank->x, AI_tank->y);
		if (TankCheak(*AI_tank, AI_tank->direction))   //���ǰ�����ϰ�
			switch (AI_tank->direction)
			{
			case UP: AI_tank->y--; break;  //��ǰ��һ��
			case DOWN: AI_tank->y++; break;  //��ǰ��һ��
			case LEFT: AI_tank->x--; break;  //��ǰ��һ��
			case RIGHT: AI_tank->x++; break;  //��ǰ��һ��
			}
		else                     //ǰ�����ϰ�
		{
			if (!(RNG_Get_RandomNum() % 4))      //3��֮1�ĸ�����ת
			{
				AI_tank->direction = RNG_Get_RandomNum() % 4 + 1;
				AI_tank->stop = 2; //��ת֮��ֹͣ�߶�3����ʱ��
				PrintTank(*AI_tank);
				return;          //��continue����������Ĵ�ӡ����,�������ȴ�ӡ
			}
			else                 //����3��֮2�ļ���ѡ����ȷ�ķ���
			{
				int j;
				for (j = 1; j <= 4; j++)
					if (TankCheak(*AI_tank, j))  //ѭ���ж�̹�����������ϰ�,�˺�����ֵ1Ϊ��ͨ��
						break;
				if (j == 5)         //j==5˵����̹�����ܶ����ϰ���޷�ͨ��
				{
					PrintTank(*AI_tank);
					return;      //�����������whileѭ���Է�������
				}
				while (TankCheak(*AI_tank, AI_tank->direction) == 0)  //���ǰ�������ϰ�
					AI_tank->direction = (RNG_Get_RandomNum() % 4 + 1);                    //�򻻸����������
			}
		}
		PrintTank(*AI_tank);     //��ӡAI̹��
	}
}

void BuildMyTank(Tank* my_tank) //�����ҵ�̹��
{
	if(life1==0&&my_tank->my==1) return;
	if(life2==0&&my_tank->my==2) return;
    int flag=1;
	if(my_tank->my==1){
		my_tank->x = 14;//λ��λ�ڻ������
		my_tank->y = 55;
		my_tank->stop = NULL;
		my_tank->direction = UP;
		my_tank->model = 0;
		my_tank->color = COLOR0;
		my_tank->CD = 5;
	}
	else{
		my_tank->x = 45;//λ��λ�ڻ������
		my_tank->y = 55;
		my_tank->stop = NULL;
		my_tank->direction = UP;
		my_tank->model = 0;
		my_tank->color = COLOR7;
		my_tank->CD = 5;
	}
	for(int i=-TANKWI/2;i<=TANKWI/2;i++)
	for(int j=-TANKWI/2;j<=TANKWI/2;j++)
	{
		if(map[my_tank->y+j][my_tank->x+i] != BLACK)
		flag=0;
	}
	if(flag){	
		my_tank->alive = 1;
		PrintTank(*my_tank);   //��ӡ�ҵ�̹��
		my_tank->revive++;
	}
}


void MoveMyTank(int turn,Tank* tank)   //���ר�ú�����turnΪkeyboard�����������벻ͬ�����������Ĳ�ͬ��ֵ
{
	if(tank->alive==0) return;
	ClearTank(tank->x, tank->y);        //map �����С��ҵ�̹�ˡ���������������ڴ˺��������
	tank->direction = turn;                  //����������ķ���ֵ�����ҵ�̹�˷���ֵ
	if (TankCheak(*tank, tank->direction))  //����ʱ�ҵ�̹�˵�ǰ���������ϰ�
		switch (turn)
		{
		case UP: tank->y--; break;  //��ǰ��һ��
		case DOWN: tank->y++; break;  //��ǰ��һ��
		case LEFT: tank->x--; break;  //��ǰ��һ��
		case RIGHT: tank->x++; break;  //��ǰ��һ��
		}                                        //��̹�˵�ǰ���������ϰ�����������仯ֱ�Ӵ�ӡ��ת���̹��
	PrintTank(*tank);
}


bool TankCheak(Tank tank, int direction)  //���̹��ǰ���ϰ�����,����Ϊ�����ꡣ��ֵ1Ϊ��ͨ��,��ֵ0Ϊ�赲(�˻�����)
{
	int i,flag=1;
	switch (direction)                    //direction����  
	{
	case UP:
            for(i=-TANKWI/2;i<=TANKWI/2;i++)
		{
		if (map[tank.y - (TANKWI/2+1)][tank.x+i] != BLACK )
			flag=0;
		}
		return flag;
		
	case DOWN:
		for(i=-TANKWI/2;i<=TANKWI/2;i++)
		{if (map[tank.y + TANKWI/2+1][tank.x+i] != BLACK )
			flag=0;
		}
		return flag;
	case LEFT:
		for(i=-TANKWI/2;i<=TANKWI/2;i++)
		{if (map[tank.y+i][tank.x -(TANKWI/2+1) ] != BLACK )
			flag=0;}
		return flag;
	case RIGHT:
		for(i=-TANKWI/2;i<=TANKWI/2;i++)
		{if (map[tank.y+i][tank.x + TANKWI/2+1] != BLACK )
			flag=0;}
		return flag;
	default:
		return 0;
	}
}


void ClearTank(int x, int y)   //���̹�˺������˻����ã�
{
	for (int i = -TANKWI/2; i <= TANKWI/2; i++)
		for (int j = -TANKWI/2; j <= TANKWI/2; j++)
		{                     
			map[y + j][x + i] = BLACK;  //��������Ϊ���� 
		}
	LCD_Fill((x-TANKWI/2)*4,(y-TANKWI/2)*4,(x+TANKWI/2)*4+3,(y+TANKWI/2)*4+3,BLACK);
}


void PrintTank(Tank tank)     //��ӡ̹�ˣ��˻����ã� ���ڶ�ȡ��Tank�����϶�,�ʾͲ�������һһ������
{   
		int i,j;
	//LCD_Fill((tank.x-TANKWI/2)*4,(tank.y-TANKWI/2)*4,(tank.x+TANKWI/2)*4+3,(tank.y+TANKWI/2)*4+3,tank.color);
		for (i = -TANKWI/2; i <= TANKWI/2; i++)
		for (j = -TANKWI/2; j <= TANKWI/2; j++){
			if (!tank.my)       
				map[tank.y + j ][tank.x + i ] = 100 + tank.num;
				else if(tank.my==1)
					map[tank.y + j ][tank.x + i ]= COLOR0;
				else
					map[tank.y + j ][tank.x + i ]= COLOR7;
		}
	if(tank.my==1&&tank.direction==UP)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank[3+j][3+i]);
					}
				} 
		}
	if(tank.my==1&&tank.direction==DOWN)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank[3-j][3+i]);
					}
				}
		}
	if(tank.my==1&&tank.direction==LEFT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank[3+i][3-j]);
					}
				}
		}
	if(tank.my==1&&tank.direction==RIGHT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank[3-i][3-j]);
					}
				}
		}
	if(tank.my==2&&tank.direction==UP)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank2[3+j][3+i]);
					}
				} 
		}
		
	if(tank.my==2&&tank.direction==DOWN)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank2[3-j][3+i]);
					}
				}
		}
	if(tank.my==2&&tank.direction==LEFT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank2[3+i][3-j]);
					}
				}				
		}
	if(tank.my==2&&tank.direction==RIGHT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_my_tank2[3-i][3-j]);
					}
				}				
		}
		
	if(tank.model==1&&tank.direction==UP)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_normal_tank[3+j][3+i]);
					}
				}				
		}
	if(tank.model==1&&tank.direction==DOWN)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_normal_tank[3-j][3+i]);
					}
				}				
		}
	if(tank.model==1&&tank.direction==LEFT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_normal_tank[3+i][3-j]);
					}
				}					
		}
	if(tank.model==1&&tank.direction==RIGHT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_normal_tank[3-i][3-j]);
					}
				}				
		}
	if(tank.model==2&&tank.direction==UP)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_fast_tank[3+j][3+i]);
					}
				}	
		}
	if(tank.model==2&&tank.direction==DOWN)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_fast_tank[3-j][3+i]);
					}
				}	
		}
	if(tank.model==2&&tank.direction==LEFT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_fast_tank[3+i][3-j]);
					}
				}
		}
	if(tank.model==2&&tank.direction==RIGHT)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_fast_tank[3-i][3-j]);
					}
				}
		}
	if(tank.model==3&&tank.direction==UP&&tank.color==COLOR3)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank1[3+j][3+i]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==DOWN&& tank.color==COLOR3)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank1[3-j][3+i]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==LEFT&&tank.color==COLOR3)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank1[3+i][3-j]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==RIGHT&&tank.color==COLOR3)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank1[3-i][3-j]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==UP&&tank.color==COLOR4)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank2[3+j][3+i]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==DOWN&&tank.color==COLOR4)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank2[3-j][3+i]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==LEFT&&tank.color==COLOR4)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank2[3+i][3-j]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==RIGHT&&tank.color==COLOR4)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank2[3-i][3-j]);
					}
				}
		}
	if(tank.model==3&&tank.direction==UP&&tank.color==COLOR5)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank3[3+j][3+i]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==DOWN&&tank.color==COLOR5)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank3[3-j][3+i]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==LEFT&&tank.color==COLOR5)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank3[3+i][3-j]);
					}
				}	
		}
	if(tank.model==3&&tank.direction==RIGHT&&tank.color==COLOR5)
		{
				for(i=-3;i<=3;i++){
					for(j=-3;j<=3;j++){
						LCD_Fill((tank.x+i)*4,(tank.y+j)*4,(tank.x+i)*4+3,(tank.y+j)*4+3,print_firm_tank3[3-i][3-j]);
					}
				}	
		}
		
}


void Initialize()      //��ʼ��
{
	my_tank.revive = 0;  //�ҵ�̹�˸������Ϊ0
	my_tank.my=1;
	position = 0;
	bul_num = 0;
	BuildMyTank(&my_tank);
	if(plyernum==2) 
	{
	my_tank2.revive=0;
	my_tank2.my=2;
	BuildMyTank(&my_tank2);
	}
	for (int i = 0; i < 50; i++)     //�ӵ���ʼ��
	{
		bullet[i].exist = 0;
		bullet[i].initial = 0;
	}
	for (int i = 0; i <= 3; i++)         //AI̹�˳�ʼ��
	{
		AI_tank[i].revive = 0;
		AI_tank[i].alive = 0;  //��ʼ��̹��ȫ�ǲ����ģ�BuildAITank()�Ὠ�����½���������̹��
		AI_tank[i].stop = 0;
		AI_tank[i].num = i;
		AI_tank[i].my = 0;
		AI_tank[i].CD = 0;
	}

}

int GameCheck()
{                           //ʣ�����Ϊ0����̹��ȫ�������
	if (remain_enemy <= 0 && !AI_tank[0].alive && !AI_tank[1].alive && !AI_tank[2].alive && !AI_tank[3].alive)
		return game_end(1);
	if (life1==0&&life2==0)   //�ҵ�����ֵ(�������)ȫ������ MAX_LIFE
		return game_end(0);        //��Ϸ����������0�����ҵĸ�������ù�(����ֵΪ0)����Ϸ�����������жϣ���һ�����ϼұ���
	else return 2;
}
