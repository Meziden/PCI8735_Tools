#include <stdio.h>
#include <stdlib.h>

//转速稳定时允许的上下波动
#define PRECISION 400

int main(int argc,char *argv[])
{
	//程序初始化
	puts("AD数据采集统计模块");
	int i,SampleNum,sum_num;
	
	//单样本数据缓冲区
	double cache[16]={0};
	
	//cache累计求和
	double sum[16]={0};
	
	//标准参考转速
	double cache_ref[12]={1200,2400,3600,4800,6000,7200,8400,9600,10800,12000,13200,15000};
	
	//各个转速区间的数据存储区
	FILE *datain,*config,*dataout[12];
	datain=fopen("./testdata_converted.tdf","r");
	config=fopen("./AD_Convert.cfg","r");
	dataout[0]=fopen("./tmp/statis_tmp_sp01.tdf","w");
	dataout[1]=fopen("./tmp/statis_tmp_sp02.tdf","w");
	dataout[2]=fopen("./tmp/statis_tmp_sp03.tdf","w");
	dataout[3]=fopen("./tmp/statis_tmp_sp04.tdf","w");
	dataout[4]=fopen("./tmp/statis_tmp_sp05.tdf","w");
	dataout[5]=fopen("./tmp/statis_tmp_sp06.tdf","w");
	dataout[6]=fopen("./tmp/statis_tmp_sp07.tdf","w");
	dataout[7]=fopen("./tmp/statis_tmp_sp08.tdf","w");
	dataout[8]=fopen("./tmp/statis_tmp_sp09.tdf","w");
	dataout[9]=fopen("./tmp/statis_tmp_sp10.tdf","w");
	dataout[10]=fopen("./tmp/statis_tmp_sp11.tdf","w");
	dataout[11]=fopen("./tmp/statis_tmp_sp12.tdf","w");
	
	//查询配置文件得到总采样数据量
	fscanf(config,"%d",&SampleNum);
	
	puts("正在分配数据");
	
	//采样总量的32倍为采样点数
	for(i=0;i<(SampleNum*32);i++)
	{

		//将该行数据读入缓冲区
		for(int j=0;j<16;j++)
		{
			fscanf(datain,"%lf",&cache[j]);
		}
		//按照设定速度区间来分配数据
		for(int speed=0;speed<12;speed++)
		{
			if(cache[0]>(cache_ref[speed]-PRECISION) && cache[0]<(cache_ref[speed]+PRECISION) )
			{	
				//写入数据到目标文件
				for(int j=0;j<16;j++)
					fprintf(dataout[speed],"%5.1lf\t",cache[j]);
				fprintf(dataout[speed],"\n");
			}
		}
	}
	//关闭文件
	fclose(config);
	fclose(datain);
	for(int filecount=0;filecount<12;filecount++)
		fclose(dataout[filecount]);
	
	
	//将文件读入，用于平均数计算
	FILE *datain_ana[12];
	FILE *dataout_ana=fopen("./testdata_statis.tdf","w");
	datain_ana[0]=fopen("./tmp/statis_tmp_sp01.tdf","r");
	datain_ana[1]=fopen("./tmp/statis_tmp_sp02.tdf","r");
	datain_ana[2]=fopen("./tmp/statis_tmp_sp03.tdf","r");
	datain_ana[3]=fopen("./tmp/statis_tmp_sp04.tdf","r");
	datain_ana[4]=fopen("./tmp/statis_tmp_sp05.tdf","r");
	datain_ana[5]=fopen("./tmp/statis_tmp_sp06.tdf","r");
	datain_ana[6]=fopen("./tmp/statis_tmp_sp07.tdf","r");
	datain_ana[7]=fopen("./tmp/statis_tmp_sp08.tdf","r");
	datain_ana[8]=fopen("./tmp/statis_tmp_sp09.tdf","r");
	datain_ana[9]=fopen("./tmp/statis_tmp_sp10.tdf","r");
	datain_ana[10]=fopen("./tmp/statis_tmp_sp11.tdf","r");
	datain_ana[11]=fopen("./tmp/statis_tmp_sp12.tdf","r");
	
	//文件逐个处理
	for(int filecount=0;filecount<12;filecount++)
	{
		printf("\r正在处理第 %d 个文件，共12个",filecount+1);
		
		//用于统计总数据样本量，便于平均值计算
		sum_num=0;
		
		//数据总和置0
		for(int position=0;position<16;position++)
		{
			sum[position]=0.0;
		}
		
		//将数据累计至sum数组中
		while(fscanf(datain_ana[filecount],"%lf",&cache[0])!=EOF)
		{
			for(int position=1;position<16;position++)
				fscanf(datain_ana[filecount],"%lf",&cache[position]);
			for(int position=0;position<16;position++)
				sum[position]+=cache[position];
			sum_num++;
		}
		
		if(sum_num!=0)
		{
			//提示
			//如需进行功率、效率等数值转换，请将对应公式置于此处，并修改plt文件到对应通道，必要时扩充sum数组的元素数量
			//如sum[channel_power]=sum[channel_volt]*sum[current_channel]
			for(int position=0;position<16;position++)
				fprintf(dataout_ana,"%5.2lf\t",(sum[position]/sum_num));
		}
		//统计数据换行
		fprintf(dataout_ana,"\n");
	}
	fclose(dataout_ana);
	for(int filecount=0;filecount<12;filecount++)
		fclose(datain_ana[filecount]);
	
	//调用GNUPLOT绘制折线图样
	system("gnuplot ./*.plt");
	puts("");
	puts("图像生成完成");
	return 0;
}
