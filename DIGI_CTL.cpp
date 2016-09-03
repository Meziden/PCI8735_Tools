#include <stdio.h>
#include <stdlib.h>		//system()
#include "stdafx.h"
#include "windows.h"

//��Ҫ�������豸��Ӳ����ţ���ֻ��һ�Ųɼ����뽫�ú궨��Ϊ0
#define DEVICE_HW_SERIAL 0

// �����ӿ�ͷ�ļ�(����)
#include "PCI8735.h"

//������ʾ����
void display(BYTE data[])
{
	for(int i=0;i<16;i++)
	{
		if(i%4==0)
			printf("\n");
		printf("PORT%2d:%2d ",i+1,data[i]);
	}
	puts("");
}

int main()
{
	puts("PCI8735���ݲɼ���ʵ�ó���Digi_IOģ�飩");
	int cmd; 
	BYTE DigiIn[16];
	BYTE DigiOut[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	//����豸�������
	HANDLE dioport=PCI8735_CreateDevice(DEVICE_HW_SERIAL);
	PCI8735_GetDeviceDI(dioport,DigiIn);
	if(dioport==INVALID_HANDLE_VALUE)
	{
		printf("\n-----���󾯸�-----\n����IO���豸���󴴽�ʧ�ܣ�����PCI�豸���������\n");
	}
	
	//��ʼ�� �������
	puts("��������");
	display(DigiIn);
	puts("");
	PCI8735_RetDeviceDO(dioport,DigiOut);
	puts("�������");
	display(DigiOut);
	puts("");	
	
	while(1)
	{
		printf("��������Ҫ�ı�Ķ˿�������\n���� 0 | ��ˢ�µ�ǰio������\n����-1 | ����ADת������\n����-2 | �˳�����\n");
		scanf("%d",&cmd);
		if(cmd==-2)
		{break;}
		else
		{
			if(cmd==-1)
			{
				//����Ŀ�����
				system("start ./AD_Convert");
				puts("ADת������������");
			}
			else 
			{
				if(cmd<17&&cmd>0)
				{
					system("cls");
					DigiOut[cmd-1]=abs(DigiOut[cmd-1]-1);
					PCI8735_SetDeviceDO(dioport,DigiOut);
					PCI8735_GetDeviceDI(dioport,DigiIn);
					puts("��������");
					display(DigiIn);
					puts("");
					PCI8735_RetDeviceDO(dioport,DigiOut);
					puts("�������");
					display(DigiOut);
					puts("");
				}
				else
				{
					if(cmd==0)
					{
						system("cls");
						puts("��������");
						display(DigiIn);
						puts("");
						PCI8735_RetDeviceDO(dioport,DigiOut);
						puts("�������");
						display(DigiOut);
						puts("");							
					}
					else
					{
						puts("����ֵ������ɷ�Χ��");
						continue;
					}
				}
			}
		}
	}
	
	//�ͷ��豸����
	PCI8735_ReleaseDevice(dioport);
	
	puts("���򼴽�����");
	Sleep(1000);
	return 0;
}
