#ifndef __AUDIOPLAY_H
#define __AUDIOPLAY_H
#include "sys.h"
#include "includes.h" 	   	 
#include "common.h"
#include "ff.h"	
#include "gui.h"









//��Ƶ���Ų����������
typedef enum {
	AP_OK=0X00,				//�����������
	AP_NEXT,				//������һ��
	AP_PREV,				//������һ��
	AP_ERR=0X80,			//�����д���(û����������,������ʾ����) 
}APRESULT;

//��Ƶ���ſ�����
typedef __packed struct
{  
	//2��I2S�����BUF
	u8 *i2sbuf1;
	u8 *i2sbuf2; 
	u8 *tbuf;			//��ʱ����
	FIL *file;			//��Ƶ�ļ�ָ�� 	
	u32(*file_seek)(u32);//�ļ�������˺��� 

	vu8 status;			//bit0:0,��ͣ����;1,��������
						//bit1:0,��������;1,��������  
						//bit2~3:����
						//bit4:0,�����ֲ���;1,���ֲ����� (������)		
						//bit5:0,�޶���;1,ִ����һ���и����(������)
						//bit6:0,�޶���;1,������ֹ����(���ǲ�ɾ����Ƶ��������),������ɺ�,���������Զ������λ
 						//bit7:0,��Ƶ����������ɾ��/����ɾ��;1,��Ƶ����������������(�������ִ��)
	
	u8 mode;			//����ģʽ
						//0,ȫ��ѭ��;1,����ѭ��;2,�������;
	
	u8 *path;			//��ǰ�ļ���·��
	u8 *name;			//��ǰ���ŵ�MP3��������
	u16 namelen;		//name��ռ�ĵ���.
	u16 curnamepos;		//��ǰ��ƫ��

    u32 totsec ;		//���׸�ʱ��,��λ:��
    u32 cursec ;		//��ǰ����ʱ�� 
    u32 bitrate;	   	//������(λ��)
	u32 samplerate;		//������ 
	u16 bps;			//λ��,����16bit,24bit,32bit
	
	u16 curindex;		//��ǰ���ŵ���Ƶ�ļ�����
	u16 mfilenum;		//�����ļ���Ŀ	    
	u16 *mfindextbl;	//��Ƶ�ļ�������
	
}__audiodev; 
extern __audiodev audiodev;	//��Ƶ���ſ�����

void audio_start(void);
void audio_stop(void); 

void audioplay_task_creat(void);
void audio_stop_req(__audiodev *audiodevx);

//ȡ2��ֵ����Ľ�Сֵ.
#ifndef AUDIO_MIN			
#define AUDIO_MIN(x,y)	((x)<(y)? (x):(y))
#endif




	    

#endif












