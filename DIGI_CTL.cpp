#include <stdio.h>
#include <stdlib.h>		//system()
#include "stdafx.h"
#include "windows.h"

//需要启动的设备的硬件编号，如只有一张采集卡请将该宏定义为0
#define DEVICE_HW_SERIAL 0

// 驱动接口头文件(必须)
#include "PCI8735.h"

//用于显示数据
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
	puts("PCI8735数据采集卡实用程序（Digi_IO模块）");
	int cmd; 
	BYTE DigiIn[16];
	BYTE DigiOut[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	//检测设备工作情况
	HANDLE dioport=PCI8735_CreateDevice(DEVICE_HW_SERIAL);
	PCI8735_GetDeviceDI(dioport,DigiIn);
	if(dioport==INVALID_HANDLE_VALUE)
	{
		printf("\n-----错误警告-----\n数字IO口设备对象创建失败，请检查PCI设备连接情况。\n");
	}
	
	//初始化 输出数据
	puts("输入数据");
	display(DigiIn);
	puts("");
	PCI8735_RetDeviceDO(dioport,DigiOut);
	puts("输出数据");
	display(DigiOut);
	puts("");	
	
	while(1)
	{
		printf("请输入需要改变的端口输出编号\n输入 0 | 仅刷新当前io口数据\n输入-1 | 启动AD转换程序\n输入-2 | 退出程序\n");
		scanf("%d",&cmd);
		if(cmd==-2)
		{break;}
		else
		{
			if(cmd==-1)
			{
				//启动目标程序
				system("start ./AD_Convert");
				puts("AD转换程序已启动");
			}
			else 
			{
				if(cmd<17&&cmd>0)
				{
					system("cls");
					DigiOut[cmd-1]=abs(DigiOut[cmd-1]-1);
					PCI8735_SetDeviceDO(dioport,DigiOut);
					PCI8735_GetDeviceDI(dioport,DigiIn);
					puts("输入数据");
					display(DigiIn);
					puts("");
					PCI8735_RetDeviceDO(dioport,DigiOut);
					puts("输出数据");
					display(DigiOut);
					puts("");
				}
				else
				{
					if(cmd==0)
					{
						system("cls");
						puts("输入数据");
						display(DigiIn);
						puts("");
						PCI8735_RetDeviceDO(dioport,DigiOut);
						puts("输出数据");
						display(DigiOut);
						puts("");							
					}
					else
					{
						puts("输入值超出许可范围。");
						continue;
					}
				}
			}
		}
	}
	
	//释放设备对象
	PCI8735_ReleaseDevice(dioport);
	
	puts("程序即将结束");
	Sleep(1000);
	return 0;
}
