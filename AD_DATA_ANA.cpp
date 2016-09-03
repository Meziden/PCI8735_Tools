#include <stdio.h>
#include <stdlib.h>

//ת���ȶ�ʱ��������²���
#define PRECISION 400

int main(int argc,char *argv[])
{
	//�����ʼ��
	puts("AD���ݲɼ�ͳ��ģ��");
	int i,SampleNum,sum_num;
	
	//���������ݻ�����
	double cache[16]={0};
	
	//cache�ۼ����
	double sum[16]={0};
	
	//��׼�ο�ת��
	double cache_ref[12]={1200,2400,3600,4800,6000,7200,8400,9600,10800,12000,13200,15000};
	
	//����ת����������ݴ洢��
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
	
	//��ѯ�����ļ��õ��ܲ���������
	fscanf(config,"%d",&SampleNum);
	
	puts("���ڷ�������");
	
	//����������32��Ϊ��������
	for(i=0;i<(SampleNum*32);i++)
	{

		//���������ݶ��뻺����
		for(int j=0;j<16;j++)
		{
			fscanf(datain,"%lf",&cache[j]);
		}
		//�����趨�ٶ���������������
		for(int speed=0;speed<12;speed++)
		{
			if(cache[0]>(cache_ref[speed]-PRECISION) && cache[0]<(cache_ref[speed]+PRECISION) )
			{	
				//д�����ݵ�Ŀ���ļ�
				for(int j=0;j<16;j++)
					fprintf(dataout[speed],"%5.1lf\t",cache[j]);
				fprintf(dataout[speed],"\n");
			}
		}
	}
	//�ر��ļ�
	fclose(config);
	fclose(datain);
	for(int filecount=0;filecount<12;filecount++)
		fclose(dataout[filecount]);
	
	
	//���ļ����룬����ƽ��������
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
	
	//�ļ��������
	for(int filecount=0;filecount<12;filecount++)
	{
		printf("\r���ڴ���� %d ���ļ�����12��",filecount+1);
		
		//����ͳ��������������������ƽ��ֵ����
		sum_num=0;
		
		//�����ܺ���0
		for(int position=0;position<16;position++)
		{
			sum[position]=0.0;
		}
		
		//�������ۼ���sum������
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
			//��ʾ
			//������й��ʡ�Ч�ʵ���ֵת�����뽫��Ӧ��ʽ���ڴ˴������޸�plt�ļ�����Ӧͨ������Ҫʱ����sum�����Ԫ������
			//��sum[channel_power]=sum[channel_volt]*sum[current_channel]
			for(int position=0;position<16;position++)
				fprintf(dataout_ana,"%5.2lf\t",(sum[position]/sum_num));
		}
		//ͳ�����ݻ���
		fprintf(dataout_ana,"\n");
	}
	fclose(dataout_ana);
	for(int filecount=0;filecount<12;filecount++)
		fclose(datain_ana[filecount]);
	
	//����GNUPLOT��������ͼ��
	system("gnuplot ./*.plt");
	puts("");
	puts("ͼ���������");
	return 0;
}
