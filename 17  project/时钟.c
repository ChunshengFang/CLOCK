#include<reg51.h>
#include <intrins.h> 
#include <stdio.h>
#define _NOP    _nop_();_nop_();_nop_();_nop_();
#define uchar unsigned char
#define uint unsigned int
#define keyinc 0xe7	               //�������λ�õ����е�һ��
#define keydec 0xd7	               //�����еڶ���
#define flaginc 0xb7              //�����е�����
#define flagdec 0x77              //�����е�����	
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
sbit E=P1^0;	 sbit rw=P1^1;   sbit dt=P1^2;    //����LCD������
sbit sda=P1^6;   sbit scl=P1^7;	                 //����SDA��SCL�ź�
sbit ming=P3^7;	  sbit change = P1^4;                               //���������

/**********��ʱ����**********/
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

/***********дָ����lcd**********/
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

/***********д������lcd��ʾ**********/
void write_data(uchar date)		
{
	dt=1;
	P0=date;
	delay(5);
	E=1;
	delay(5);
	E=0;
}
/*****************дһ���ֽں���****************/
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
	write_com(0x38);	 //Һ�����ò���æģʽ
	write_com(0x0c);	 //����ʾ ����ʾ���
	write_com(0x06);	 //��дһ���ַ��ǣ���ַָ��� 1
	write_com(0x01);	 //��ʾ����
	write_com(0x80+0x10);  	 //��ָ��д�뻺����
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
		


 
/*****************��ʼ��****************/
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
	write_add(0x02,0x50);	        //����53��
	write_add(0x03,0x59);	        //��59��
	write_add(0x04,0x08);           //Сʱ8��
	write_add(0x05,0x25);           //����25��
	write_add(0x06,0x05);           //������
	write_add(0x07,0x05);           //�·�5��
	write_add(0x08,0x18);           //���18��
}
/*****************��ʾ����****************/  
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
				write_com(0x0f);delay(800);break;				  //������̿ɸı������λ��
			default:break;
			
		}
	}
	
	write_com(0x80+0x40);
	write_data(table[2]);		        //�ڵڶ��е�0��λ����ʾtable�б��Ϊ2���ַ�
	write_data(table[0]);		        //����һλ����ʾ���Ϊ0���ַ�
	write_com(0x80+0x42);
	write_data(table[year/16]);	        //�ڵڶ��е�2��λ����ʾ�������16��������table�б�ŵ��ַ�
	write_data(table[year%16]);	        //����һλ����ʾ������table�б�ŵ��ַ�
	write_data(table[11]);		        //����һλ����ʾtable�б��Ϊ11���ַ�
	write_data(table[(mon&0x1f)/16]);
	write_data(table[(mon&0x1f)%16]);
	write_data(table[11]);			     //����һλ����ʾtable�б��Ϊ11���ַ�
	write_data(table[(day&0x3f)/16]);
	write_data(table[(day&0x3f)%16]);
	write_com(0x80+0x4d);		         //����������ʾ��λ��
	for(num=0;num<3;num++)
	{
		write_data(table1[(week&0x07)*3+num]);	
	}
	write_com(0x80+0x08);
	write_data(table[(hour&0x3f)/16]);			//��ʾ��24��Χ�ڵ�Сʱ����16���������Ϊ�����table�е��ַ�
	write_data(table[(hour&0x3f)%16]);			//��ʾ��24��Χ�ڵ�Сʱ����16���������Ϊ�����table�е��ַ�
	write_data(table[10]);						//����һλ����ʾtable�б��Ϊ10���ַ�
	write_data(table[(min&0x7f)/16]);			//��ʾ��60��Χ�ڷ�������16���������Ϊ�����table�е��ַ�
	write_data(table[(min&0x7f)%16]);			//��ʾ��60��Χ�ڷ�������16���������Ϊ�����table�е��ַ�
	write_data(table[10]);						//����һλ����ʾtable�б��Ϊ10���ַ�
	write_data(table[(sec&0x7f)/16]);			//��ʾ��60��Χ����������16���������Ϊ�����table�е��ַ�
	write_data(table[(sec&0x7f)%16]);			 //��ʾ��60��Χ����������16���������Ϊ�����table�е��ַ�
	write_com(0x80+0x00);
	write_data('T');
	write_data('I');
	write_data('M');
	write_data('E');	
	write_data(' ');
	write_data(' ');
	write_data(' ');

}

/*****************��ʼ****************/
void start()   
{	
	sda=1;			           	//������ʼ
	_NOP;
	_NOP;								                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	scl=1;
	_NOP;				        //������ʼ�ź�
	sda=0;
	_NOP;
}

/*****************ֹͣ****************/
void stop()   
{
	sda=0;				 //���ͽ����������ź�����
	_NOP;
	_NOP;
	scl=1;				 //���ͽ����������ź�����
	_NOP;
	sda=1;				 //�������߽����ź�
	_NOP;
}

/*****************Ӧ��****************/
void respons() 
{
	uchar i;
	scl=1;
	delayus();
	while((sda==1)&&(i<250))i++;
	scl=0;
	delayus();
}


/*****************��һ���ֽں���****************/
uchar read_byte() 
{
	uchar i,k;
	scl=0;
	delayus();
	sda=1;			            //������׼���������ݣ���Ƭ���˿�����ǰΪ1
	delayus();
	for(i=0;i<8;i++)
	{
		scl=1;
		delayus();	
		k=(k<<1)|sda;
		scl=0;					//ʱ����Ϊ�͵�ƽ��׼����������
		delayus();	
	}
	return k;
}

/*****************��һ����Ԫ��д���ݺ���****************/
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

/*****************��һ����Ԫ������****************/
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
 
/*****************����������д��hour��min��****************/
void read8563()	
{
	sec=read_add(0x02);			 //����������д����ļĴ�����ַ02H��
	min=read_add(0x03);			 //����������д��ֵļĴ�����ַ03H��
	hour=read_add(0x04);		 //����������д��Сʱ�ļĴ�����ַ04H��
	day=read_add(0x05);			 //����������д����ļĴ�����ַ05H��
	week=read_add(0x06);		 //����������д�����ڵļĴ�����ַ06H��
	mon=read_add(0x07);			 //����������д���·ݵļĴ�����ַ07H��
	year=read_add(0x08);		 //����������д����ļĴ�����ַ08H��
}

/******************************************
	 Һ����ʾ����ʾ����ģ��
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
	             ����ģ��
	******************************************/
void clock() 
{
     if(sec2==sec)       //���ж������ֵ�Ƿ����
       if(min2==min)     //�ǣ����жϷ��Ƿ����
         if(hour2==hour) //�ǣ����ж�ʱ�Ƿ����
           if(sec<6)
	 {
		 ming=0;
		 
     }
	delay(500);
    ming=1;					
}

/*****************����ɨ�����****************/
key()						
{
	uchar temp;
	P2=0xf7;					//P3����������ʽ���̵ĵ�����,̧��P2�ĵ�ƽ
	temp=P2;
	return(temp);
} 

/*****************����ִ���Ӻ���****************/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
void key_date()					//����ִ���Ӻ���
{
	uchar num;		
	num=key();
	if(num==flaginc)			//���ܼ�������,�����������ƶ�
	{
		if(flag==8)
		{
			flag=0;				//�����ƶ�7�Σ���ص���ʼλ��	 
		}
		else {flag++;}			//�����������ƶ�һλ
	}
	if(num==flagdec)			//���ܼ����������������ƶ�
	{
		if(flag==0)				
		{
			flag=7;				//������ʼλ�ã���ĩλ
		}
		else {flag--;}			//�����������ƶ�һλ
	}
	if(num==keyinc)				//��������
	{
		uchar ktemp;
		switch(flag)
		{
			case 1:ktemp=0;write_add(0x02,ktemp);break;	//����д����ļĴ���02H��
			case 2:ktemp=read_add(0x03)&0x7f; 			//����������д��ֵļĴ���03H�У���Χ���Ϊ59
			if(ktemp==0x59)
			{ktemp=0x00;}
			else 
			{
				if(ktemp%16==0x09)
				{ktemp+=0x07;}
				else{ktemp++;}
			}
			write_add(0x03,ktemp);break;				 //����������д��ֵļĴ���03H��
			case 3:ktemp=read_add(0x04)&0x3f; 			 //����������д��Сʱ�ļĴ���04H�У���Χ���Ϊ59
			if(ktemp==0x23)
			{ktemp=0x00;}
			else 
			{
				if(ktemp%16==0x09)
				{ktemp+=0x07;}
				else{ktemp++;}
			}
			write_add(0x04,ktemp);break;				  //����д��Ĵ���04H��
			case 4:ktemp=read_add(0x06)&0x07;       	  //����������д�����ڵļĴ���06H�У���7��
			if(ktemp==6)
			{ktemp=0;}									  //����Ϊ��������������һ�����ص�һ��
			else {ktemp++;}								  //����Ϊ��һΪ��һ��
			write_add(0x06,ktemp);break;				  //����д��Ĵ���06H��
			case 5:ktemp=read_add(0x05)&0x7f;       	  //����������д����ļĴ���05H�У���Χ���31
			if(ktemp==0x31)
			{ktemp=0x00;}								  //����Ϊ31���򷵻�0
			else {if(ktemp%16==0x09){ktemp+=0x07;}ktemp++;}
			write_add(0x05,ktemp);break;				   //����д��Ĵ���05H��
			case 6:ktemp=read_add(0x07)&0x7f; 			   //����������д���·ݵļĴ���07H�У���Χ���Ϊ12
			if(ktemp==0x12)
			{ktemp=0x00;}								   //����Ϊ12���򷵻�0
			else {if(ktemp%16==0x09){ktemp+=0x07;}ktemp++;}
			write_add(0x07,ktemp);break;				   //����������д��Ĵ���07H��
			case 7:ktemp=read_add(0x08);				   //����������д����ļĴ���08H��
			if(ktemp==0x99)
			{ktemp=0x00;}								   //����Ϊ99���򷵻�0
			else {if(ktemp%16==0x09){ktemp+=0x07;}ktemp++;}
			write_add(0x08,ktemp);break;					//����д��Ĵ���08H��
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

/**************����*****************/
void ring()		 
{
   	
    flag1=0;                //�ǣ����־λ��flag1����
    while(!(flag1==1))      //�ж�flag1�Ƿ�100
   {
      ming=0;                //û�У��򣬼��������������죬ʱ��ԼΪ��10s=50ms*200
      key_date();            //����ʱ���Ӻ���
      display();             //������ʾ�Ӻ���
	  flag1++;   
   }
   ming=1;                   //�رշ����� 
}

/**********��ʱ������24���ҳ���1��ʱ���㱨ʱ************/
void baoshi()
 {	
    num=read_add(0x04);
 	if(min==0&&sec<=num&&(hour<0x24)&&(hour>=0x00))		  //��СʱΪ���ڵ���0��С��24��ʱ���㱨ʱ
 	{
 		ming=0;
 		delay(800);
 		ming=1;
 	}
 } 

/**********�������************/
void clock2()                                //����ĳ���
{
  if((sec==0x00)&&(min==0x00))	   ring();	 //���ֺ����λ�ö���00ʱ������
}
 

	
void main()    //������
{
 	init();
 	while(1)
 	{
	   switch((int)change )  //�л���ʾ��
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

