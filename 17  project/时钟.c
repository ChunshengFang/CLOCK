#include<reg51.h>
#include <intrins.h> 
#include <stdio.h>
#define _NOP    _nop_();_nop_();_nop_();_nop_();
#define uchar unsigned char
#define uint unsigned int
#define keyinc 0xe7	               //定义键盘位置第四行第一列
#define keydec 0xd7	               //第四行第二列
#define flaginc 0xb7              //第四行第三列
#define flagdec 0x77              //第四行第四列	
uint year;
uchar s,hour,min,sec,week,mon,day,hour2=9,min2=2,sec2=0;
uchar flag=0;
uchar flag1=0;
uchar code table[]={'0','1','2','3','4','5','6','7','8','9',':','-'};
uchar code table1[]={'S','u','n','M','o','n','T','u','e','W','e','d','T','h','u','F','r','i','S','a','t'};
uchar code table2[]={"FCS "};
void delayus();
void display7();
void write_add(uchar address,uchar date);
uchar num;
sbit E=P1^0;	 sbit rw=P1^1;   sbit dt=P1^2;    //定义LCD控制线
sbit sda=P1^6;   sbit scl=P1^7;	                 //定义SDA和SCL信号
sbit ming=P3^7;	  sbit change = P1^4;                               //定义蜂鸣器

/**********延时函数**********/
delay(uint t)			
{
	uint i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<120;j++)
		{;}
	}
	return i;
}

void delayus()	
{
	;
}

/***********写指令于lcd**********/
void write_com(uchar com)		
{
	rw=0;
	dt=0;
	P0=com;
	delay(5);
	E=1;
	delay(5);
	E=0;	
}

/***********写内容于lcd显示**********/
void write_data(uchar date)		
{
	dt=1;
	P0=date;
	delay(5);
	E=1;
	delay(5);
	E=0;
}
/*****************写一个字节函数****************/
void write_byte(uchar date)	
{
	uchar i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;
	    delayus();
		sda=CY;
		delayus();
		scl=1;
		delayus();
	}
	scl=0;
	delayus();
	sda=1;
	delayus();
}

void welcome()
{
    uchar num;
	uchar code tab[]="NICE TO ME YOU";
	uchar code tab1[]="208160117";
	write_com(0x80);
	rw=0;
	E=0;
	write_com(0x38);	 //液晶设置不判忙模式
	write_com(0x0c);	 //开显示 不显示光标
	write_com(0x06);	 //当写一个字符是，地址指针加 1
	write_com(0x01);	 //显示清屏
	write_com(0x80+0x10);  	 //将指令写入缓冲区
     for(num=0;num<14;num++)
   {		      
    write_data(tab[num]);
    delay(10);
   }
    write_com(0x80+0x50); 
   for(num=0;num<9;num++)
   {
    write_data(tab1[num]);
    delay(10);
   }
   for(num=0;num<16;num++)
   {
    write_com(0x18);
    delay(300);
  } 
   delay(1500);
   write_com(0x01);
}
		


 
/*****************初始化****************/
void init()					    
{
	rw=0;
	E=0;
	write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80+0x10);
	welcome();
	
	sda=1;
	_NOP;
	scl=1;
	_NOP;	
	write_add(0x00,0x00);
	write_add(0x01,0x00);
	write_add(0x02,0x50);	        //秒数53秒
	write_add(0x03,0x59);	        //分59分
	write_add(0x04,0x08);           //小时8点
	write_add(0x05,0x25);           //日期25号
	write_add(0x06,0x05);           //星期五
	write_add(0x07,0x05);           //月份5月
	write_add(0x08,0x18);           //年份18年
}
/*****************显示函数****************/  
void display()
{	
	if(flag!=0)
	{
		write_com(0x06);
	
		switch(flag)
		{
			case 1:	write_com(0x80+15);
				write_com(0x0f);delay(800);break;
			case 2:	write_com(0x80+12);
				write_com(0x0f);delay(800);break;
			case 3:	write_com(0x80+9);
				write_com(0x0f);delay(800);break;
			case 4:	write_com(0x80+0x40+14);
				write_com(0x0f);delay(800);break;			
			case 5:	write_com(0x80+0x40+9);
				write_com(0x0f);delay(800);break;
			case 6:	write_com(0x80+0x40+6);
				write_com(0x0f);delay(800);break;
			case 7:	write_com(0x80+0x40+3);
				write_com(0x0f);delay(800);break;				  //定义键盘可改变的数的位置
			default:break;
			
		}
	}
	
	write_com(0x80+0x40);
	write_data(table[2]);		        //在第二行第0个位置显示table中编号为2的字符
	write_data(table[0]);		        //在下一位置显示编号为0的字符
	write_com(0x80+0x42);
	write_data(table[year/16]);	        //在第二行第2个位置显示年份整除16的整数在table中编号的字符
	write_data(table[year%16]);	        //在下一位置显示余数在table中编号的字符
	write_data(table[11]);		        //在下一位置显示table中编号为11的字符
	write_data(table[(mon&0x1f)/16]);
	write_data(table[(mon&0x1f)%16]);
	write_data(table[11]);			     //在下一位置显示table中编号为11的字符
	write_data(table[(day&0x3f)/16]);
	write_data(table[(day&0x3f)%16]);
	write_com(0x80+0x4d);		         //定义星期显示的位置
	for(num=0;num<3;num++)
	{
		write_data(table1[(week&0x07)*3+num]);	
	}
	write_com(0x80+0x08);
	write_data(table[(hour&0x3f)/16]);			//显示在24范围内的小时整除16后的整数作为编号在table中的字符
	write_data(table[(hour&0x3f)%16]);			//显示在24范围内的小时整除16后的余数作为编号在table中的字符
	write_data(table[10]);						//在下一位置显示table中编号为10的字符
	write_data(table[(min&0x7f)/16]);			//显示在60范围内分钟整除16后的整数作为编号在table中的字符
	write_data(table[(min&0x7f)%16]);			//显示在60范围内分钟整除16后的余数作为编号在table中的字符
	write_data(table[10]);						//在下一位置显示table中编号为10的字符
	write_data(table[(sec&0x7f)/16]);			//显示在60范围内秒数整除16后的整数作为编号在table中的字符
	write_data(table[(sec&0x7f)%16]);			 //显示在60范围内秒数整除16后的余数作为编号在table中的字符
	write_com(0x80+0x00);
	write_data('T');
	write_data('I');
	write_data('M');
	write_data('E');	
	write_data(' ');
	write_data(' ');
	write_data(' ');

}

/*****************开始****************/
void start()   
{	
	sda=1;			           	//总线起始
	_NOP;
	_NOP;								                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	scl=1;
	_NOP;				        //发送起始信号
	sda=0;
	_NOP;
}

/*****************停止****************/
void stop()   
{
	sda=0;				 //发送结束条件的信号序列
	_NOP;
	_NOP;
	scl=1;				 //发送结束条件的信号序列
	_NOP;
	sda=1;				 //发送总线结束信号
	_NOP;
}

/*****************应答****************/
void respons() 
{
	uchar i;
	scl=1;
	delayus();
	while((sda==1)&&(i<250))i++;
	scl=0;
	delayus();
}


/*****************读一个字节函数****************/
uchar read_byte() 
{
	uchar i,k;
	scl=0;
	delayus();
	sda=1;			            //数据线准备输入数据，单片机端口输入前为1
	delayus();
	for(i=0;i<8;i++)
	{
		scl=1;
		delayus();	
		k=(k<<1)|sda;
		scl=0;					//时钟线为低电平，准备接收数据
		delayus();	
	}
	return k;
}

/*****************往一个单元里写内容函数****************/
void write_add(uchar address,uchar date) 
{
	start();
	write_byte(0xa2);
	respons();
	write_byte(address);
	respons();
	write_byte(date);
	respons();
	stop();
}

/*****************读一个单元的内容****************/
uchar read_add(uchar address) 
{
	uchar date;
	start();
	write_byte(0xa2);
	respons();
	write_byte(address);
	respons();
	start();
	write_byte(0xa3);
	respons();
	date=read_byte();
	stop();
	return date; 
}
 
/*****************将读出的数写入hour与min中****************/
void read8563()	
{
	sec=read_add(0x02);			 //将读出的数写入秒的寄存器地址02H中
	min=read_add(0x03);			 //将读出的数写入分的寄存器地址03H中
	hour=read_add(0x04);		 //将读出的数写入小时的寄存器地址04H中
	day=read_add(0x05);			 //将读出的数写入天的寄存器地址05H中
	week=read_add(0x06);		 //将读出的数写入星期的寄存器地址06H中
	mon=read_add(0x07);			 //将读出的数写入月份的寄存器地址07H中
	year=read_add(0x08);		 //将读出的数写入年的寄存器地址08H中
}

/******************************************
	 液晶显示器显示闹钟模块
******************************************/

void display7()
{	
	if(flag!=0)
	{
		write_com(0x06);
		switch(flag)
		{
		case 1:	write_com(0x80+0x40+6);
				write_com(0x0f);delay(800);break;
		case 2:	write_com(0x80+0x40+3);
				write_com(0x0f);delay(800);break;
		case 3:	write_com(0x80+0x40+0);
				write_com(0x0f);delay(800);break;
		case 4:	write_com(0x80+13);
				write_com(0x0f);delay(800);break;
		case 5:	write_com(0x80+8);
				write_com(0x0f);delay(800);break;
		case 6:	write_com(0x80+5);
				write_com(0x0f);delay(800);break;
		case 7:	write_com(0x80+2);
				write_com(0x0f);delay(800);break;
		default:break;
		}
	}

	write_com(0x80+0x00);
	write_data(' ');
	write_data('C');
	write_data('L');
	write_data('O');
	write_data('C');
	write_data('K');
	write_data(':');
    write_data(' ');	
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_com(0x80+0x40);
	write_data(' ');
	write_data(' ');
	write_data(table[0]);
	write_data(table[9]);
	write_data(':');
	write_data(table[0]);
	write_data(table[2]);
	write_data(':');
	write_data(table[0]);
	write_data(table[0]);
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
	write_data(' ');
			
	}
	/******************************************
	             闹铃模块
	******************************************/
void clock() 
{
     if(sec2==sec)       //显判断秒的数值是否相等
       if(min2==min)     //是，在判断分是否相等
         if(hour2==hour) //是，再判断时是否相等
           if(sec<6)
	 {
		 ming=0;
		 
     }
	delay(500);
    ming=1;					
}

/*****************按键扫描程序****************/
key()						
{
	uchar temp;
	P2=0xf7;					//P3口连接行列式键盘的第四行,抬高P2的电平
	temp=P2;
	return(temp);
} 

/*****************按键执行子函数****************/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
void key_date()					//按键执行子函数
{
	uchar num;		
	num=key();
	if(num==flaginc)			//功能键的增加,按键后向左移动
	{
		if(flag==8)
		{
			flag=0;				//若已移动7次，则回到起始位置	 
		}
		else {flag++;}			//否则再向左移动一位
	}
	if(num==flagdec)			//功能键减，按键后向右移动
	{
		if(flag==0)				
		{
			flag=7;				//若在起始位置，则到末位
		}
		else {flag--;}			//否则再向右移动一位
	}
	if(num==keyinc)				//参数增加
	{
		uchar ktemp;
		switch(flag)
		{
			case 1:ktemp=0;write_add(0x02,ktemp);break;	//将数写入秒的寄存器02H中
			case 2:ktemp=read_add(0x03)&0x7f; 			//将读出的数写入分的寄存器03H中，范围最大为59
			if(ktemp==0x59)
			{ktemp=0x00;}
			else 
			{
				if(ktemp%16==0x09)
				{ktemp+=0x07;}
				else{ktemp++;}
			}
			write_add(0x03,ktemp);break;				 //将读出的数写入分的寄存器03H中
			case 3:ktemp=read_add(0x04)&0x3f; 			 //将读出的数写入小时的寄存器04H中，范围最大为59
			if(ktemp==0x23)
			{ktemp=0x00;}
			else 
			{
				if(ktemp%16==0x09)
				{ktemp+=0x07;}
				else{ktemp++;}
			}
			write_add(0x04,ktemp);break;				  //将数写入寄存器04H中
			case 4:ktemp=read_add(0x06)&0x07;       	  //将读出的数写入星期的寄存器06H中，有7个
			if(ktemp==6)
			{ktemp=0;}									  //若已为第六个数，则下一个返回第一个
			else {ktemp++;}								  //否则为加一为下一个
			write_add(0x06,ktemp);break;				  //将数写入寄存器06H中
			case 5:ktemp=read_add(0x05)&0x7f;       	  //将读出的数写入天的寄存器05H中，范围最大31
			if(ktemp==0x31)
			{ktemp=0x00;}								  //若已为31，则返回0
			else {if(ktemp%16==0x09){ktemp+=0x07;}ktemp++;}
			write_add(0x05,ktemp);break;				   //将数写入寄存器05H中
			case 6:ktemp=read_add(0x07)&0x7f; 			   //将读出的数写入月份的寄存器07H中，范围最大为12
			if(ktemp==0x12)
			{ktemp=0x00;}								   //若已为12，则返回0
			else {if(ktemp%16==0x09){ktemp+=0x07;}ktemp++;}
			write_add(0x07,ktemp);break;				   //将读出的数写入寄存器07H中
			case 7:ktemp=read_add(0x08);				   //将读出的数写入年的寄存器08H中
			if(ktemp==0x99)
			{ktemp=0x00;}								   //若已为99，则返回0
			else {if(ktemp%16==0x09){ktemp+=0x07;}ktemp++;}
			write_add(0x08,ktemp);break;					//将数写入寄存器08H中
			default:break;
		}
	}
	if(num==keydec)
	{
		uchar dtemp;
		switch(flag)
		{
		case 1:dtemp=0;write_add(0x02,dtemp);break;
		case 2:dtemp=read_add(0x03)&0x7f;
				if(dtemp==0x00){dtemp=0x59;}
				else {if(dtemp%16==0x00){dtemp-=7;}dtemp--;}
				write_add(0x03,dtemp);break;
		case 3:dtemp=read_add(0x04)&0x3f;
				if(dtemp==0x00){dtemp=0x23;}
				else {if(dtemp%16==0x00){dtemp-=7;}dtemp--;}
				write_add(0x04,dtemp);break;
		case 4:dtemp=read_add(0x06)&0x07;
				if(dtemp==0x00){dtemp=0x06;}
				else {dtemp--;}
				write_add(0x06,dtemp);break;
		case 5:dtemp=read_add(0x05)&0x7f;
				if(dtemp==0x00){dtemp=0x31;}
				else {if(dtemp%16==0x00){dtemp-=7;}dtemp--;}
				write_add(0x05,dtemp);break;
		case 6:dtemp=read_add(0x07)&0x7f;
				if(dtemp==0x00){dtemp=0x12;}
				else {if(dtemp%16==0x00){dtemp-=7;}dtemp--;}
				write_add(0x07,dtemp);break;
		case 7:dtemp=read_add(0x08)&0xff;
				if(dtemp==0x00){dtemp=0x99;}
				else {if(dtemp%16==0x00){dtemp-=7;}dtemp--;}
				write_add(0x08,dtemp);break;
		default:break;
		}
		
	}
}

/**************打铃*****************/
void ring()		 
{
   	
    flag1=0;                //是，则标志位，flag1清零
    while(!(flag1==1))      //判断flag1是否到100
   {
      ming=0;                //没有，则，继续驱动蜂鸣器响，时间约为：10s=50ms*200
      key_date();            //调用时间子函数
      display();             //调用显示子函数
	  flag1++;   
   }
   ming=1;                   //关闭蜂鸣器 
}

/**********当时间早于24点且迟于1点时整点报时************/
void baoshi()
 {	
    num=read_add(0x04);
 	if(min==0&&sec<=num&&(hour<0x24)&&(hour>=0x00))		  //当小时为大于等于0点小于24点时整点报时
 	{
 		ming=0;
 		delay(800);
 		ming=1;
 	}
 } 

/**********整点打铃************/
void clock2()                                //打铃的程序
{
  if((sec==0x00)&&(min==0x00))	   ring();	 //当分和秒的位置都是00时，打铃
}
 

	
void main()    //主函数
{
 	init();
 	while(1)
 	{
	   switch((int)change )  //切换显示屏
	   {
	   case 1:
 		   display();
 		   key();		   
 		   key_date();
 		   read8563();
		   clock();
    	   clock2();
		   baoshi();
		   break;
		case 0:
		   display7();
		   key();
 		   key_date();
		   break;

	   }
 }
}

