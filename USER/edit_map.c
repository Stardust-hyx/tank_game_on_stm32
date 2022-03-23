#include "sys.h"
#include "delay.h"  
#include "usart.h" 
#include "lcd.h"
#include "touch.h"
#include "malloc.h" 
#include "sdio_sdcard.h"
#include "guix.h"
#include "text.h"
#include "edit_map.h"
#include "data_addr.h"

extern u16 print_my_home[8][8];

myframe frame_save={30,59,180,120};
myframe base={104,204,32,32};
myframe birth1={44,208,28,28};
myframe birth2={168,208,28,28};

mybutton wall0={260,20,40,17};
mybutton wall1={260,62,40,17};
mybutton river={260,104,40,17};
mybutton rub={260,146,40,17};
mybutton btn_save={260,192,40,24};
mybutton btn_exit_edit={260,215,40,24};

mybutton confirm={70,159,32,16};
mybutton quit={138,159,32,16};

//u16 colorlist[15]={BLACK,BLUE,BROWN,WHITE,RED,YELLOW,GREEN,BRED,GBLUE,MAGENTA,CYAN,BRRED,GRED,GRAY,DGRAY};
///////////////////// 0     1     2     3    4    5      6    7     8     9      10    11   12   13   14

//���ز����
void edit_load_ui(void){
	POINT_COLOR=BLACK;
	LCD_Fill(wall0.x,wall0.y,wall0.x+wall0.width-1,wall0.y+wall0.height-1,BROWN);
	LCD_Fill(wall1.x,wall1.y,wall1.x+wall1.width-1,wall1.y+wall1.height-1,DGRAY);
	LCD_Fill(river.x,river.y,river.x+river.width-1,river.y+river.height-1,BLUE);
	LCD_Fill(rub.x,rub.y,rub.x+rub.width-1,rub.y+rub.height-1,BLACK);
	Show_Str(wall0.x+4,wall0.y+19,40,16,"��ǽ",16,1);
	Show_Str(wall1.x+4,wall1.y+19,40,16,"��ǽ",16,1);
	Show_Str(river.x+4,river.y+19,40,16,"����",16,1);
	Show_Str(rub.x-4,rub.y+19,60,16,"��Ƥ��",16,1);
	POINT_COLOR=RED;
	Show_Str(btn_save.x+4,btn_save.y,btn_save.width,btn_save.height,"����",16,1);
	Show_Str(btn_exit_edit.x+4,btn_exit_edit.y,btn_exit_edit.width,btn_exit_edit.height,"�˳�",16,1);
}


//���ر���Ի���
void load_save_frame(u8 mapnum){
	int i,j,x,y=frame_save.y+27;
	POINT_COLOR=BLACK;
	LCD_Fill(frame_save.x,frame_save.y,frame_save.x+frame_save.width,frame_save.y+frame_save.height,GRAY);
	Show_Str(frame_save.x+18,frame_save.y+4,150,16,"��Ϊ�ڼ��صĵ�ͼ��",16,1);
	POINT_COLOR=BLUE;
	//ѭ�����ɣ�����������ɣ�ֻ����ʾ��֮����ɨ��ʱ��ͬ�ֹ���ɨ����ʾ�˰�ť��λ�ã����йؿ��İ�ť
	for(i=1;i<=((mapnum-1)/3)+1;i++){
		x=frame_save.x+9;
		for(j=1;j<=3;j++){
			if((i-1)*3+j>mapnum) break;
			Show_Str(x,y,48,16,"�� ��",16,1);
			LCD_ShowxNum(x+16,y,(i-1)*3+j,1,16,1);
			x+=53;
		}
		if((i-1)*3+j>mapnum) break;
		y+=20;
	}
	//���ɡ���ӹؿ�����ť
	if(mapnum<=8){
		if(!mapnum) Show_Str(frame_save.x+9,y,65,16,"��ӹؿ�",16,1);
		else if(!(mapnum%3)) Show_Str(frame_save.x+9,y+20,65,16,"��ӹؿ�",16,1);
		else Show_Str(x,y,65,16,"��ӹؿ�",16,1);
	}
	POINT_COLOR=RED;
	//���ɡ�ȷ�ϡ�����ȡ������ť
	Show_Str(confirm.x,confirm.y,32,16,"ȷ��",16,1);
	Show_Str(quit.x,quit.y,32,16,"ȡ��",16,1);
}

//���棨������水ť��ִ�У�
void save(u8* buf){
	u8 selected=0,i,j,x,y;
	u8* mapnum;
	mapnum=mymalloc(0,512);
	if(SD_ReadDisk(mapnum,MapNumAddr,1)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
	load_save_frame(*mapnum);
	while(1){
		tp_dev.scan(0);
		y=frame_save.y+27;
		int flag=0;
		for(i=1;i<=((*mapnum-1)/3)+1;i++){	//ɨ���Ƿ�����ĳ�����йؿ�
			x=frame_save.x+9;
			for(j=1;j<=3;j++){
				if((i-1)*3+j>*mapnum) break;
				if(tp_dev.x[0]> x-2 && tp_dev.x[0]< x+50
				&& tp_dev.y[0]> y-2 && tp_dev.y[0]< y+18)
				{
					flag=1;
					selected=(i-1)*3+j;
					load_save_frame(*mapnum);
					POINT_COLOR=YELLOW;
					Show_Str(x,y,48,16,"�� ��",16,1);
					LCD_ShowxNum(x+16,y,(i-1)*3+j,1,16,1);
					break;
				}
				x+=53;
			}
			if((i-1)*3+j>*mapnum) break;
			if(flag) break;
			y+=20;
		}
		if(flag)continue;		//��ɨ�赽���û�����İ�ť��ִ����һ��ѭ��
		if(*mapnum<=8){			//ɨ���Ƿ����ˡ���ӹؿ���
			if(!(*mapnum)) { 
				if(tp_dev.x[0]> frame_save.x+9 && tp_dev.x[0]< frame_save.x+73
				&& tp_dev.y[0]> y && tp_dev.y[0]< y+18)
				{
					flag=1;
					selected=*mapnum+1;
					load_save_frame(*mapnum);
					POINT_COLOR = YELLOW;
					Show_Str(frame_save.x+9,y,65,16,"��ӹؿ�",16,1);
				}
			}
			else if(!(*mapnum%3)){ 
				if(tp_dev.x[0]> frame_save.x+9 && tp_dev.x[0]< frame_save.x+73
				&& tp_dev.y[0]> y+20 && tp_dev.y[0]< y+38)
				{
					flag=1;
					selected=*mapnum+1;
					load_save_frame(*mapnum);
					POINT_COLOR = YELLOW;
					Show_Str(frame_save.x+9,y+20,65,16,"��ӹؿ�",16,1);
				}
			}
			else if(tp_dev.x[0]> x+9 && tp_dev.x[0]< x+73
				&& tp_dev.y[0]> y && tp_dev.y[0]< y+18)
			{
					flag=1;
					selected=*mapnum+1;
					load_save_frame(*mapnum);
					POINT_COLOR = YELLOW;
					Show_Str(x,y,65,16,"��ӹؿ�",16,1);
			}
		}
		if(flag) continue;	//��ɨ�赽���û�����İ�ť��ִ����һ��ѭ��
		if(tp_dev.x[0]> confirm.x-2 && tp_dev.x[0]< confirm.x + confirm.width+2 
			&& tp_dev.y[0]> confirm.y-2 && tp_dev.y[0]< confirm.y + confirm.height+2)
		{	//���"ȷ��"
			if(!selected){	//��δָ���ѵ�ͼ��Ϊ��һ�أ�������ʾ��Ϣ
				Show_Str(frame_save.x+44,frame_save.y+82,96,16,"��ָ���ؿ���",16,1);
			}
			else{
				POINT_COLOR = YELLOW;
				Show_Str(confirm.x,confirm.y,32,16,"ȷ��",16,1);
				POINT_COLOR = RED;
				if(SD_WriteDisk(buf,MapAddr+(selected-1)*8,8)) {//�����ͼ��������ʧ�ܣ�������ʾ��Ϣ
					Show_Str(frame_save.x+18,frame_save.y+82,144,16,"����ʧ�ܣ������ԣ�",16,1);
				}
				else{//����ɹ���дSD���Ը����ܵ�ͼ�����ɹ��󵯳���ʾ��Ϣ������ָ����壬�˳�����
					if(selected==*mapnum+1){ *mapnum+=1; SD_WriteDisk(mapnum,MapNumAddr,1);}
					Show_Str(frame_save.x+50,frame_save.y+82,80,16,"����ɹ���",16,1);
					myfree(0,mapnum);
					delay_ms(700);
					for(i=0;i<60;i++){
						for(j=0;j<60;j++){
							LCD_Fill(i*4,j*4,i*4+3,j*4+3,colorlist[*(buf+j*60+i)]);
						}
					}
					LCD_Fill(4,4,31,31,RED);
					LCD_Fill(106,4,133,31,RED);
					LCD_Fill(208,4,235,31,RED);
					LCD_Fill(birth1.x,birth1.y,birth1.x+birth1.width-1,birth1.y+birth1.height-1,RED);
					LCD_Fill(birth2.x,birth2.y,birth2.x+birth2.width-1,birth2.y+birth2.height-1,RED);
					for(i=26;i<=33;i++){	//��
						for(j=51;j<=58;j++){
							LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
						}
					}
					return;
				}
			}
		}
		else if(tp_dev.x[0]> quit.x-2 && tp_dev.x[0]< quit.x + quit.width+2 
			&& tp_dev.y[0]> quit.y-2 && tp_dev.y[0]< quit.y + quit.height+2)
		{//ȡ�����棬ֱ�ӻָ����壬�˳�����
			for(i=0;i<60;i++){
					for(j=0;j<60;j++){
						LCD_Fill(i*4,j*4,i*4+3,j*4+3,colorlist[*(buf+j*60+i)]);
					}
			}
			LCD_Fill(4,4,31,31,RED);
			LCD_Fill(106,4,133,31,RED);
			LCD_Fill(208,4,235,31,RED);
			LCD_Fill(birth1.x,birth1.y,birth1.x+birth1.width-1,birth1.y+birth1.height-1,RED);
			LCD_Fill(birth2.x,birth2.y,birth2.x+birth2.width-1,birth2.y+birth2.height-1,RED);
			for(i=26;i<=33;i++){	//��
						for(j=51;j<=58;j++){
							LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
						}
					}
			myfree(0,mapnum);
			delay_ms(250);
			return;
		}
	}
}

//�༭�����棩��ͼ���ܵ�������
void edit(void){
	u8* pixel_matrix;
	if(!(pixel_matrix=mymalloc(0,512*8))) LCD_ShowString(100,100,200,24,24,"OVERFLOW");
  u8	pen = 2;   //��ǰ��������
	int px,py;	//������ĺ�������
	int i,j;
	//��ʼ������
	LCD_Fill(0,0,239,239,BLACK);
	LCD_Fill(240,0,319,239,GRAY);
	LCD_Fill(4,4,31,31,RED);
	LCD_Fill(106,4,133,31,RED);
	LCD_Fill(208,4,235,31,RED);
	LCD_Fill(birth1.x,birth1.y,birth1.x+birth1.width-1,birth1.y+birth1.height-1,RED);
	LCD_Fill(birth2.x,birth2.y,birth2.x+birth2.width-1,birth2.y+birth2.height-1,RED);
	//��ʼ����ͼ����Ԫ����
	for(i=0;i<60;i++)
		for(j=0;j<60;j++)
			*(pixel_matrix+i*60+j)=0;
	//����ͼ�߽�
	LCD_Fill(0,0,239,3,GRAY);	
	LCD_Fill(0,0,3,239,GRAY);
	LCD_Fill(0,236,239,239,GRAY);
	LCD_Fill(236,0,239,239,GRAY);
	//���µ�ͼ����Ԫ����
	for(i=0;i<60;i++){                
		*(pixel_matrix+0*60+i)=13;//GRAY
		*(pixel_matrix+59*60+i)=13;
		*(pixel_matrix+i*60+0)=13;
		*(pixel_matrix+i*60+59)=13;
	}
	//������
	for(i=26;i<=33;i++){	
		for(j=51;j<=58;j++){
			LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
		}
	}
	//���µ�ͼ����Ԫ����
	for(i=26;i<34;i++)							
		for(j=51;j<59;j++)
			*(pixel_matrix+j*60+i)=3;
	//������ǽ�����µ�ͼ����Ԫ����
	for(i=19;i<=24;i++)	
		for(j=46;j<=58;j++)
		{
			*(pixel_matrix+j*60+i)=2;
			LCD_Fill(i*4,j*4,i*4+3,j*4+3,BROWN);
			*(pixel_matrix+j*60+i+16)=2;
			LCD_Fill((i+16)*4,j*4,(i+16)*4+3,j*4+3,BROWN);
		}
	for(i=25;i<=35;i++)
		for(j=46;j<=49;j++)
		{
			*(pixel_matrix+j*60+i)=2;
			LCD_Fill(i*4,j*4,i*4+3,j*4+3,BROWN);
		}
	//���ز����
	edit_load_ui();
	POINT_COLOR=YELLOW;
	//Ĭ��һ��ʼ�Ļ���Ϊ����ǽ��
	Show_Str(wall0.x+4,wall0.y+19,40,16,"��ǽ",16,1);
	delay_ms(250);
	while(1){
		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN)		//������������
		{	
			if(tp_dev.x[0]> wall0.x && tp_dev.x[0]< wall0.x + wall0.width 
			&& tp_dev.y[0]> wall0.y && tp_dev.y[0]< wall0.y + wall0.height+18)
			{//���ʱ�Ϊ����ǽ��
				pen=2;	
				edit_load_ui();
				POINT_COLOR=YELLOW;
				Show_Str(wall0.x+4,wall0.y+19,40,16,"��ǽ",16,1);
			}
			if(tp_dev.x[0]> wall1.x && tp_dev.x[0]< wall1.x + wall1.width 
			&& tp_dev.y[0]> wall1.y && tp_dev.y[0]< wall1.y + wall1.height+18)
			{//���ʱ�Ϊ����ǽ��
				pen=14;	
				edit_load_ui();
				POINT_COLOR=YELLOW;
				Show_Str(wall1.x+4,wall1.y+19,40,16,"��ǽ",16,1);
			}
			else if(tp_dev.x[0]> river.x && tp_dev.x[0]< river.x + river.width 
			&& tp_dev.y[0]> river.y && tp_dev.y[0]< river.y + river.height+18)
			{//���ʱ�Ϊ��������
				pen=1;	
				edit_load_ui();
				POINT_COLOR=YELLOW;
				Show_Str(river.x+4,river.y+19,40,16,"����",16,1);
			}
			else if(tp_dev.x[0]> rub.x && tp_dev.x[0]< rub.x + rub.width 
			&& tp_dev.y[0]> rub.y && tp_dev.y[0]< rub.y + rub.height+18)
			{//���ʱ�Ϊ����Ƥ����
				pen=0;	
				edit_load_ui();
				POINT_COLOR=YELLOW;
				Show_Str(rub.x-4,rub.y+19,60,16,"��Ƥ��",16,1);
			}
			else if(tp_dev.x[0]> btn_save.x && tp_dev.x[0]< btn_save.x + btn_save.width 
			&& tp_dev.y[0]> btn_save.y && tp_dev.y[0]< btn_save.y + btn_save.height)
			{//���ˡ����桱��ִ��save����
				save(pixel_matrix);
			}
			else if(tp_dev.x[0]> btn_exit_edit.x && tp_dev.x[0]< btn_exit_edit.x + btn_exit_edit.width 
			&& tp_dev.y[0]> btn_exit_edit.y && tp_dev.y[0]< btn_exit_edit.y + btn_exit_edit.height)
			{//�˳�
				myfree(0,pixel_matrix);
				return;
			}
			else if(tp_dev.x[0]<240 && tp_dev.y[0]<240 && 
			!(tp_dev.x[0]>=base.x-15 && tp_dev.x[0]<base.x+base.width+15 && tp_dev.y[0]>=base.y-15)
			&& !(tp_dev.x[0]<=31+15 && tp_dev.y[0]<=31+15)
			&& !(tp_dev.x[0]>=106-15 && tp_dev.x[0]<=133+15 && tp_dev.y[0]<=31+15)
			&& !(tp_dev.x[0]>=208-15 && tp_dev.x[0]<=235+15 && tp_dev.y[0]<=31+15)
			&& !(tp_dev.x[0]>=birth1.x-15 && tp_dev.x[0]<birth1.x+birth1.width+15 && tp_dev.y[0]>=birth1.y-15)
			&& !(tp_dev.x[0]>=birth2.x-15 && tp_dev.x[0]<birth2.x+birth1.width+15 && tp_dev.y[0]>=birth2.y-15)
			){//��
				px = tp_dev.x[0]/4;
				py = tp_dev.y[0]/4;
				for(i=-2;(px+i)>=1 && (px+i)<59 && i<=2;i++){
					for(j=-2;(py+j)>=1 && (py+j<59) && j <=2; j++){
						*(pixel_matrix+(py+j)*60+px+i)=pen;
						LCD_Fill((px+i)*4,(py+j)*4,(px+i)*4+3,(py+j)*4+3,colorlist[*(pixel_matrix+(py+j)*60+px+i)]);
					}
				}
			}
		}
	}
}
