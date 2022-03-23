#include "includes.h"		
#include "sys.h"
#include "audioplay.h"
#include "settings.h"
#include "ucos_ii.h"
#include "wm8978.h"
#include "i2s.h"
#include "wavplay.h" 
#include "mp3play.h" 

__audiodev audiodev;	


//��Ƶ��������
//�����������ȼ�
#define AUDIO_PLAY_TASK_PRIO       			2
//���������ջ��С
#define AUDIO_PLAY_STK_SIZE  					1024
//�����ջ	
OS_STK AUDIO_PLAY_TASK_STK[AUDIO_PLAY_STK_SIZE];
//������
void audioplay_task(void *pdata);

//����������������
OS_EVENT * audiombox;//�¼����ƿ�

const char * BGM1="0:/MUSIC/bgm1.wav";		//bgm1·��+�ļ���
const char * BGM2="0:/MUSIC/bgm2.wav";		//bgm2·��+�ļ���
const char * SOUND_EFFECT_1="0:/MUSIC/bgm1.wav";		//sound_effect_1·��+�ļ���

//��ʼ��Ƶ����
void audio_start(void)
{
	audiodev.status|=1<<1;		//��������
	audiodev.status|=1<<0;		//����ͣ״̬
	I2S_Play_Start();
} 
//ֹͣ��Ƶ����
void audio_stop(void)
{
	audiodev.status&=~(1<<0);	//��ͣλ����
	audiodev.status&=~(1<<1);	//��������
	I2S_Play_Stop();
}    

void audioplay_task(void *pdata){
	u8 * fname;
	u32 status;
	LCD_Clear(BLUE);
	WM8978_ADDA_Cfg(1,0);	//����DAC
	WM8978_Input_Cfg(0,0,0);//�ر�����ͨ��
	WM8978_Output_Cfg(1,0);	//����DAC���
 // status = (u32)OSMboxPend(audiombox,0,&rval);
	
/*	switch(status){
		case 1:{
			strcpy((char*)fname,BGM1);
			while(1) wav_play_song(fname);
		}
		case 2:{
			break;
		}*/
	while(1)
	{ 
		audiodev.status|=1<<5;//����и���
		audiodev.status|=1<<4;//������ڲ������� 
		strcpy((char*)fname,"0:/MUSIC/sf.mp3");
		mp3_play_song(fname);
		//delay_ms(100);
	}
			
}

void audioplay_task_creat(void){
	audiodev.status=1<<7;								//������Ƶ������������
	OSTaskCreate(audioplay_task,(void *)0,(OS_STK*)&AUDIO_PLAY_TASK_STK[AUDIO_PLAY_STK_SIZE-1],AUDIO_PLAY_TASK_PRIO);				   
}

