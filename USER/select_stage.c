#include "sys.h"
#include "delay.h"  
#include "usart.h" 
#include "lcd.h"
#include "touch.h"
#include "key.h"
#include "malloc.h" 
#include "sdio_sdcard.h"
#include "guix.h"
#include "text.h"
#include "edit_map.h"
#include "data_addr.h"
#include "select_stage.h"

//u16 colorlist[15]={BLACK,BLUE,BROWN,WHITE,RED,YELLOW,GREEN,BRED,GBLUE,MAGENTA,CYAN,BRRED,GRED,GRAY,DGRAY};
///////////////////// 0     1     2     3    4    5      6    7     8     9      10    11   12   13   14
extern u16 print_my_home[8][8];

myframe frame_delet={30,80,180,80};

mybutton confirm_select={263,192,32,16};
mybutton delet_stage={263,219,32,16};

mybutton confirm_delet={70,135,32,16};
mybutton quit_delet={138,135,32,16};

void load_select_ui(u8 mapnum){
	int i,x,y;
	POINT_COLOR = BLUE;
	x=258;
	y=8;
	for(i=1;i<=mapnum;i++){
		Show_Str(x,y,48,16,"�� ��",16,1);
		LCD_ShowxNum(x+16,y,i,1,16,1);
		y+=20;
	}
}


//ɾ���ؿ����ָ�����
//����ֵ��0��ɾ��ʧ�ܣ�1���ɹ�ɾ��
int delet(u8* pmapnum,u8 tg){
	u8 *buf1,*buf2;
	int i,j,flag=1;	//��һ��flag���ɾ���Ƿ�ɹ�
	buf1=mymalloc(0,512*8);
	POINT_COLOR=YELLOW;
	Show_Str(confirm_delet.x,confirm_delet.y,32,16,"ȷ��",16,1);
	//��дSD����ʹ��ɾ�ؿ�������йؿ�ȫ����ǰ��1���ؿ��Ŀռ�
	for(i=0;i<*pmapnum-tg;i++){
		if(SD_ReadDisk(buf1,MapAddr+(tg+i)*8,8)) {flag=0; break; } //��SD������
		if(SD_WriteDisk(buf1,MapAddr+(tg+i-1)*8,8)) {flag=0; break; } //дSD������
	}
	POINT_COLOR=RED;
	if(flag){	//�ɹ�ɾ���������ܹؿ���
						buf2=mymalloc(0,1);
						(*pmapnum)--;
						*buf2=*pmapnum;
						i=10;
						while(i--) {
							if(SD_WriteDisk(buf2,MapNumAddr,1)) flag=0;
							else {flag=1; break;}
						}
						if(flag) Show_Str(frame_delet.x+50,frame_delet.y+35,80,16,"�ɹ�ɾ����",16,1);
						myfree(0,buf2);
					}
	else Show_Str(frame_delet.x+50,frame_delet.y+35,80,16,"ɾ���쳣��",16,1);//ɾ��ʧ�ܣ�������ʾ��Ϣ
	delay_ms(700);
	//���²����				
	LCD_Fill(240,0,319,239,GRAY);
	load_select_ui(*pmapnum);
	POINT_COLOR=YELLOW;
	if(tg>*pmapnum) tg=*pmapnum;
	Show_Str(258,8+20*(tg-1),48,16,"�� ��",16,1);
	LCD_ShowxNum(258+16,8+20*(tg-1),tg,1,16,1);
	POINT_COLOR=RED;
	Show_Str(delet_stage.x,delet_stage.y,delet_stage.width,delet_stage.height,"ɾ��",16,1);
	Show_Str(confirm_select.x,confirm_select.y,confirm_select.width,confirm_select.height,"ȷ��",16,1);
	//�ָ�����
	if(SD_ReadDisk(buf1,MapAddr+(tg-1)*8,8)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
	for(i=0;i<60;i++){
		for(j=0;j<60;j++){
			if(i<26||i>33||j<51||j>58)
				LCD_Fill(i*4,j*4,i*4+3,j*4+3,colorlist[*(buf1+j*60+i)]);
		}
	}
	for(i=26;i<=33;i++){	//����
		for(j=51;j<=58;j++){
			LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
		}
	}
	myfree(0,buf1);
	if(flag) return 1;
	else return 0;	
}

//ɾ���Ի�����
//����ֵ��1���ɹ�ɾ����0��δɾ��
int predelet(u8* pmapnum,u8 tg){
	u8 *buf1;
	int i,j;
	u8 key;
	buf1=mymalloc(0,512*8);
	LCD_Fill(frame_delet.x,frame_delet.y,frame_delet.x+frame_delet.width,frame_delet.y+frame_delet.height,GRAY);
	POINT_COLOR=BLACK;
	Show_Str(frame_delet.x+35,frame_delet.y+16,128,16,"ȷ��ɾ���� �أ�",16,1);
	LCD_ShowxNum(frame_delet.x+35+16*5,frame_delet.y+16,tg,1,16,1);
	POINT_COLOR=RED;
	Show_Str(confirm_delet.x,confirm_delet.y,32,16,"ȷ��",16,1);
	Show_Str(quit_delet.x,quit_delet.y,32,16,"ȡ��",16,1);
	while(1){
		tp_dev.scan(0);
		key=KEY_Scan(0);
		if(key==5) {	myfree(0,buf1);	return delet(pmapnum,tg);}//ȷ��ɾ����ִ��delet������delet�ķ���ֵ
		else if(key==6)	//ȡ��ɾ�����ָ�����󷵻�0
		{
			if(SD_ReadDisk(buf1,MapAddr+(tg-1)*8,8)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
			for(i=0;i<60;i++){
				for(j=0;j<60;j++){
					if(i<26||i>33||j<51||j>58)
					LCD_Fill(i*4,j*4,i*4+3,j*4+3,colorlist[*(buf1+j*60+i)]);
				}
			}
					for(i=26;i<=33;i++){	//��
							for(j=51;j<=58;j++){
								LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
							}
					}
			myfree(0,buf1);
			return 0;
		}
		if(tp_dev.sta&TP_PRES_DOWN){
			if(tp_dev.x[0]> confirm_delet.x-2 && tp_dev.x[0]< confirm_delet.x + confirm_delet.width+2 
			&& tp_dev.y[0]> confirm_delet.y-2 && tp_dev.y[0]< confirm_delet.y + confirm_delet.height+2)
			{//ȷ��ɾ����ִ��delet������delet�ķ���ֵ
					myfree(0,buf1);
				  return delet(pmapnum,tg);
			}
			else if(tp_dev.x[0]> quit_delet.x-2 && tp_dev.x[0]< quit_delet.x + quit_delet.width+2 
			&& tp_dev.y[0]> quit_delet.y-2 && tp_dev.y[0]< quit_delet.y + quit_delet.height+2)
			{//ȡ��ɾ�����ָ�����󷵻�0
				if(SD_ReadDisk(buf1,MapAddr+(tg-1)*8,8)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
					for(i=0;i<60;i++){
						for(j=0;j<60;j++){
							if(i<26||i>33||j<51||j>58)
							LCD_Fill(i*4,j*4,i*4+3,j*4+3,colorlist[*(buf1+j*60+i)]);
						}
					}
					for(i=26;i<=33;i++){	//��
							for(j=51;j<=58;j++){
								LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
							}
					}
				myfree(0,buf1);
				return 0;
			}
		}
	}
}

//ѡ��ؿ�����������
//������ѡ����
int select(void){
	u8 selected = 1; //��ǰѡ���Ĺؿ�
	int i,j,k,flag,x=258,y=8,key;
	u8* pmapnum;
	pmapnum = mymalloc(0,512);
	u8* pixel_matrix;
	pixel_matrix = mymalloc(0,512*8);
	LCD_Clear(BLACK);
	POINT_COLOR=WHITE;
	if(SD_ReadDisk(pmapnum,MapNumAddr,1)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
	if(*pmapnum==0){	//��ǰû�йؿ�����ʾ��ʾ��Ϣ
		LCD_Clear(BLACK);
		Show_Str(75,105,200,16,"0.0��ǰû�е�ͼ������",16,1); 
		Show_Str(100,125,120,16,"���ȴ�����ͼ��",16,1);
		delay_ms(2000);
		return 0;
	}
	LCD_Fill(240,0,319,239,GRAY);
	load_select_ui(*pmapnum);	//���ز����
	if(SD_ReadDisk(pixel_matrix,MapAddr,8)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
	for(k=0;k<60;k++){		//��ʾ��һ�ص�ͼ
			for(j=0;j<60;j++){
				if(k<26||k>33||j<51||j>58)
				LCD_Fill(k*4,j*4,k*4+3,j*4+3,colorlist[*(pixel_matrix+j*60+k)]);
			}
	}
	for(i=26;i<=33;i++){	//��Ⱦ����
			for(j=51;j<=58;j++){
					LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
				}
	}
	//ȱʡѡ���һ��
	POINT_COLOR=YELLOW;
	Show_Str(x,y,64,16,"��1��",16,1);
	POINT_COLOR=RED;
	Show_Str(delet_stage.x,delet_stage.y,delet_stage.width,delet_stage.height,"ɾ��",16,1);
	Show_Str(confirm_select.x,confirm_select.y,confirm_select.width,confirm_select.height,"ȷ��",16,1);
	delay_ms(200);
	while(1){
		if(*pmapnum==0){ 	//��ɾ���ؿ���ؿ�����Ϊ0����ʾ��ʾ��Ϣ
			LCD_Clear(BLACK);
			POINT_COLOR=WHITE;
			Show_Str(75,105,200,16,"0.0��ǰû�е�ͼ������",16,1); 
			Show_Str(100,125,120,16,"���ȴ�����ͼ��",16,1);
			delay_ms(2000);
			return 0;
		}
		tp_dev.scan(0);
		if(tp_dev.sta&TP_PRES_DOWN){		//������������
			flag=0;
			y=8;
			for(i=1;i<=*pmapnum;i++){	//ɨ���Ƿ񴥿�ѡ��ĳһ��
				if(tp_dev.x[0]>= x && tp_dev.x[0]<= x+48 
				&& tp_dev.y[0]>= y-2 && tp_dev.y[0]<= y+18){
					flag = 1;
					selected=i;
					if(SD_ReadDisk(pixel_matrix,MapAddr+(selected-1)*8,8)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
					for(k=0;k<60;k++){
						for(j=0;j<60;j++){	//��ʾ��ѡ�ؿ��ĵ�ͼ
							if(k<26||k>33||j<51||j>58)
								LCD_Fill(k*4,j*4,k*4+3,j*4+3,colorlist[*(pixel_matrix+j*60+k)]);
						}
					}
					for(int s=26;s<=33;s++){	//��Ⱦ����
						for(int t=51;t<=58;t++){
							LCD_Fill(s*4,t*4,s*4+3,t*4+3,print_my_home[t-51][s-26]);
						}
					}
					//���²����
					load_select_ui(*pmapnum);	
					POINT_COLOR=YELLOW;
					Show_Str(x,y,48,16,"�� ��",16,1);
					LCD_ShowxNum(x+16,y,i,1,16,1);
					break;
				}
				y+=20;
			}
			if(flag) continue;
			if(tp_dev.x[0]>= delet_stage.x-2 && tp_dev.x[0]<= delet_stage.x + delet_stage.width+1 
			&& tp_dev.y[0]>= delet_stage.y-2 && tp_dev.y[0]<= delet_stage.y + delet_stage.height+1)
			{		//ɾ���ؿ�
					if(predelet(pmapnum,selected) && selected==*pmapnum+1)//��ɾ���˹ؿ���ɾ���������һ��
						selected--; 
			}
			if(tp_dev.x[0]>= confirm_select.x-2 && tp_dev.x[0]<= confirm_select.x + confirm_select.width+1 
			&& tp_dev.y[0]>= confirm_select.y-2 && tp_dev.y[0]<= confirm_select.y + confirm_select.height+1)
			{		//�ͷŷ�����ڴ棬������ѡ�����
					myfree(0,pixel_matrix);
					myfree(0,pmapnum);
					return selected;
			}
			delay_ms(50);
		}
		y=8;
		key=KEY_Scan(0);		//ɨ�谴��
		if(key==1){	//���ˡ��ϡ�������ѡ�������һ
			POINT_COLOR=BLUE;
			Show_Str(x,y+(selected-1)*20,48,16,"�� ��",16,1);
			LCD_ShowxNum(x+16,y+(selected-1)*20,selected,1,16,1);
			selected=(--selected ? selected : *pmapnum);
			POINT_COLOR=YELLOW;
			Show_Str(x,y+(selected-1)*20,48,16,"�� ��",16,1);
			LCD_ShowxNum(x+16,y+(selected-1)*20,selected,1,16,1);
			if(SD_ReadDisk(pixel_matrix,MapAddr+(selected-1)*8,8)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
					for(k=0;k<60;k++){
						for(j=0;j<60;j++){
							if(k<26||k>33||j<51||j>58)
								LCD_Fill(k*4,j*4,k*4+3,j*4+3,colorlist[*(pixel_matrix+j*60+k)]);
						}
					}
					for(i=26;i<=33;i++){	//����
							for(j=51;j<=58;j++){
								LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
							}
					}
		}
		else if(key==2){	//���ˡ��¡�������ѡ�������һ
			POINT_COLOR=BLUE;
			Show_Str(x,y+(selected-1)*20,48,16,"�� ��",16,1);
			LCD_ShowxNum(x+16,y+(selected-1)*20,selected,1,16,1);
			selected=(++selected>(*pmapnum) ? 1 : selected);
			POINT_COLOR=YELLOW;
			Show_Str(x,y+(selected-1)*20,48,16,"�� ��",16,1);
			LCD_ShowxNum(x+16,y+(selected-1)*20,selected,1,16,1);
			if(SD_ReadDisk(pixel_matrix,MapAddr+(selected-1)*8,8)) Show_Str(65,105,128,16,"��ȡSD��ʧ��!",16,1);
					for(k=0;k<60;k++){
						for(j=0;j<60;j++){
							if(k<26||k>33||j<51||j>58)
							LCD_Fill(k*4,j*4,k*4+3,j*4+3,colorlist[*(pixel_matrix+j*60+k)]);
						}
					}
					for(i=26;i<=33;i++){	//����
							for(j=51;j<=58;j++){
								LCD_Fill(i*4,j*4,i*4+3,j*4+3,print_my_home[j-51][i-26]);
							}
					}
		}
		else if(key==5){	//���ˡ�ȷ���������ͷŷ�����ڴ棬������ѡ�����
			myfree(0,pixel_matrix);
			myfree(0,pmapnum);
			return selected;
		}
		else if(key==6){	//���ˡ�ȡ��������ɾ��
			if(predelet(pmapnum,selected) && selected==*pmapnum+1)	selected--; //ɾ��
		}
	}
}
