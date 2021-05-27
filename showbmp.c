#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <sys/mman.h>
#include <math.h>
#include <stdlib.h>

#define LCD_WIDTH 800
#define LCD_HEIGHT 480

#define BIT_3 24
#define BIT_2 16
#define BIT_1 8
#define BIT_0 0

//bmp_path-->bmp图片的路径，x，y--->要显示图片的起始位置
int showbmp(char *bmp_path,int x,int y)
{
	int i,j,k;
	int bmp_w,bmp_h;
	int lcdfd,bmpfd;
	
	//①打开显示屏的设备文件   
	lcdfd=open("/dev/fb0",O_RDWR);
	if(lcdfd==-1)
	{
		printf("open lcd failed\n");
		return -1;
	}
	
	//2、申请内存空间  
	int * lcdmmap=mmap(NULL,LCD_WIDTH*LCD_HEIGHT*4,PROT_READ | PROT_WRITE,MAP_SHARED,lcdfd,0);
	if(lcdmmap==NULL)
	{
		printf("mmap failed\n");
		return -1;
	}
	
	//清屏
	bzero(lcdmmap,LCD_WIDTH*LCD_HEIGHT*4);

	//②打开图片的文件
	bmpfd=open(bmp_path,O_RDWR);
	if(bmpfd==-1)
	{
		printf("open main.bmp failed\n");
		return -1;
	}
	
	lseek(bmpfd,18,SEEK_SET);
	
	//读取图片的宽和高
	read(bmpfd,&bmp_w,4);   
	read(bmpfd,&bmp_h,4);
	printf("bmp_w :%d   bmp_h:%d\n",bmp_w,bmp_h);
	
	if(x+bmp_w > LCD_WIDTH)
	{
		printf("图片显示宽度超出屏幕范围\n");
		return -1;
	}
	
	if(y+bmp_h > LCD_HEIGHT)
	{
		printf("图片显示高度超出屏幕范围\n");
		return -1;
	}
		
	
	//偏移54个字节 头信息
	lseek(bmpfd,54,SEEK_SET);
	
	//③读取图片的内容  800*480*3
	char bmpbuf[bmp_w*bmp_h*3];   //存放bmp的数据
	read(bmpfd,bmpbuf,bmp_w*bmp_h*3);

	int lcdbuf[bmp_w*bmp_h];    
	int tembuf[bmp_w*bmp_h];
	//实现像素的转化----由3字节变成4字节
	for(i=0;i<bmp_w*bmp_h;i++)
	{  
		lcdbuf[i]=(0x00<<BIT_3)|(bmpbuf[3*i+2]<<BIT_2)|(bmpbuf[3*i+1]<<BIT_1)|bmpbuf[3*i]<<BIT_0;
	}
	
	//解决bmp图片镜像问题
	for(i=0;i<bmp_h;i++)
	{
		for(j=0;j<bmp_w;j++)
		{
			tembuf[i*bmp_w+j]=lcdbuf[(bmp_h-1-i)*bmp_w+j]; 		 
		}			   
	}

	//将lcd显示的起始位置偏移到指定的位置
	lcdmmap += y*LCD_WIDTH+x;
	
	//④将读到的内容写到屏幕的设备文件里面
	int r = sqrt(bmp_w*bmp_w + bmp_h*bmp_h);	//求出圆形的直径

	for(k=1;k<r/2+2;k++)         
	{
		for(i=0;i<bmp_h;i++)    
		{
			for(j=0;j<bmp_w;j++)
			{
				if( (j-bmp_w/2)*(j-bmp_w/2)+(i-bmp_h/2)*(i-bmp_h/2)<=k*k )
				{
					*(lcdmmap+i*LCD_WIDTH+j)=tembuf[i*bmp_w+j]; 
				}
			}
		}
	}
	
	//⑤关闭屏幕设备文件
	close(lcdfd);
	//⑥关闭图片文件
	close(bmpfd);
	//解除映射
	munmap(lcdmmap,LCD_WIDTH*LCD_HEIGHT*4);
	return 0;
}


int main(int argc,char **argv)
{
	if(argc != 4)
	{
		printf("传的参数个数错误\n");
		return -1;
	}
		
	showbmp(argv[1],atoi(argv[2]),atoi(argv[3]));
	return 0;
}


