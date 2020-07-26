/*
要求：8k-88k内容复制到168k-248k上，其他数据都不变，
文件总长度256k
文件结构：21字节一行，每行：第5个字节到第20个字节是数据
第一个字节 0x10表示本行数据的长度； 

第二、三字节 0x00 0x08表示本行数据的起始地址； 

第四字节 0x00表示数据类型，数据类型有：0x00、0x01、0x02、0x03、0x04、0x05。 

'00' Data Rrecord：用来记录数据，HEX文件的大部分记录都是数据记录 

'01' End of File Record: 用来标识文件结束，放在文件的最后，标识HEX文件的结尾 

'02' Extended Segment Address Record: 用来标识扩展段地址的记录 

'03' Start Segment Address Record:开始段地址记录 

'04' Extended Linear Address Record: 用来标识扩展线性地址的记录 

'05' Start Linear Address Record:开始线性地址记录 

然后是数据
最后一个字节为校验和 = 0x100-第1到第21个字节的和（去除溢出0x100） 

*/


#include "main.h"
#include <stdio.h>
#include <conio.h>
#include "main.h"
#include <math.h>


unsigned char HextoString(unsigned char number)
{


	if(number>=0 && number<=9)
	{
		number = number + 48;

	}

	if(number>=0xA && number<=0xF)
	{
		number = number + 55;
	}

	/*
	char character;
	switch(number)
	{
		case 0:
			character='0';
		case 1:
			character='1';
		case 2:
			character='2';
		case 3:
			character='3';
		case 4:
			character='4';
		case 5:
			character='5';
		case 6:
			character='6';
		case 7:
			character='7';
		case 8:
			character='8';
		case 9:
			character='9';
		case 0xA:
			character='A';
		case 0xB:
			character='B';
		case 0xC:
			character='C';
		case 0xD:
			character='D';
		case 0xE:
			character='E';
		case 0xF:
			character='F';
		default:
			break;

	}

	return character;
	*/

	return number;

}


unsigned char StringtoHex(unsigned char c)
{
   if((c>='0')&&(c<='9'))
   return c-0x30;
   else if((c>='A')&&(c<='F'))
   return c-'A'+10;
   else if((c>='a')&&(c<='f'))
   return c-'a'+10;
   else 
   return 0x10;
}

//校验位数值
void CheckByte(unsigned char *row)
{
	unsigned char real[20]; 
	unsigned int checksum=0;
	unsigned char j=0;
	for(int i=1;i<40;i++)
	{
		
		real[j]=(row[i]<<4) + row[++i];

		checksum+=real[j];

		j++;
	}

	while(checksum>0x100)
	{
		checksum=checksum&0xFF;
		break;
	}
	checksum = 0x100 - checksum;

	row[41] = checksum>>4;
	row[42] = checksum&0xF;
			
}

void RowProcess(unsigned char *row,unsigned char *realdata,int num)
{
	unsigned char i;

	for(i=1;i<43;i++)
		row[i] = StringtoHex(row[i]);

	// 地址位处理
	row[3] = num>>12;
	row[4] = (num&0xF00)>>8;
	row[5] = (num&0xF0)>>4;
	row[6] = num&0xF;

	// 校验位处理
	CheckByte(row);

	//转为ASCii码
	for(i=1;i<43;i++)
	{
		row[i]=HextoString(row[i]);
	}

	row[0]=58;
	row[43]=10;
/*	//暂不求
	for(i=5;i<21;i++)
	{
		realdata[i]=row[i];
	}
*/

}


unsigned int TempData[462*1024+16*2];

void main(void)
{
	FILE * fInput;
	FILE * fIntemp;
	FILE * fOutput;
	FILE * ftest;
	FILE * fend;

	int backleng;
       
	unsigned char row[44];
    unsigned char realdata[16];

	unsigned char ExpandData[16]={':','0','2','0','0','0','0','0','4','0','0','0','3','F','7',10};
//	unsigned int TempDatas[80*1024];
	unsigned char tempdata3[22542];

	printf("hex文件处理ing! \n");

	if((fInput = fopen("in.hex","r")) == NULL)
	{
		printf("Fail to open file\n");
	}
	else
	{
		printf("in.hex is open\n");
	}

	if((fIntemp = fopen("copy.hex","r")) == NULL)
	{
		printf("Fail to open file\n");
	}
	
	if((fend=fopen("end.hex","r")) == NULL)
	{
		printf("Fail to open endfile!!");
	}

	if(remove("Out.hex") == -1)   
		perror("Could not delete Out.hex ");   
	else   
		printf("Deleted Out.hex success \n");  

	if(fInput != NULL)
		rewind(fInput);
	if(fIntemp != NULL)
		rewind(fIntemp);
	if(fend != NULL)
		rewind(fend);

	fOutput = fopen("Out.hex","w");
    if(fOutput == NULL)
	{
		printf("Could not delete Out.hex");
	}

/*
	ftest = fopen("test.txt","r");
	if(ftest != NULL)
	{
		rewind(ftest);
		unsigned char t[90];
		fread(t,1,90,ftest);
		fwrite(t,1,90,fOutput);

  		if(ftest != NULL)
		{
			fclose(fInput);
			printf("transformation succeed !\n");
		}

		if(fOutput != NULL)
		{
			fclose(fOutput);		
		}
	}

*/
	if(fInput != NULL)
	{
		printf("Encoding\n");

		backleng = hex_length;

		//------------------------------------
		//       读写数据
		fread(TempData,462*1024+16*2,1,fInput);
		fwrite(TempData,462*1024+16*2,1,fOutput);
	
	//	fseek(fIntemp,22528L,SEEK_SET);           //  8*1024/16*44=22528

		int j=0;
	//	for(int i=0;i<80*1024/16*44;)
		
			int addressnum,i=0;

			while(i<66*1024)
			{
				addressnum = 0xA000 + (j++)*16;   
				fread(row,44,1,fIntemp);
				RowProcess(row,realdata,addressnum);                  //处理地址位、校验位 转变成后面168k到248k数据需要的
				fwrite(row,44,1,fOutput);
				i = i + 44;
			}

		//	fread(ExpandData,16,1,fIntemp);
	    	fwrite(ExpandData,16,1,fOutput);                         		 

	
			j=0;
			while(i>=66*1024 && i<225280+16)
			{				
				addressnum =0x0000 + (j++)*16;

				if(i==154*1024)
				{
					unsigned char r[16];
					fread(r,16,1,fIntemp);
				}

				fread(row,44,1,fIntemp);
/*
				if(row[1]='0')
				{
					
					for(int q=0;q<44;q++)
					{
						t[q] = row[q];
					}
					char a=2;
				}
*/
				RowProcess(row,realdata,addressnum);                  //处理地址位、校验位 转变成后面168k到248k数据需要的
				fwrite(row,44,1,fOutput);
				i = i + 44;
			
			}

        
		fread(tempdata3,22539,1,fend);                  //(704-682)*1024+11=22539
		fwrite(tempdata3,22539,1,fOutput);

		printf("Data was transformed!!!!!!!!!!!!!");
     
		//unsigned int totalsize=sizeof(fIntemp);
	}

	printf("all was stored..............................\n");

	if(fend != NULL)
	{
		fclose(fend);
		printf("end data is inputed!");
	}

	if(fIntemp != NULL)
	{
		fclose(fIntemp);
		printf("needful data is inputed! ");
	}

	if(fInput != NULL)
	{
		fclose(fInput);
		printf("transformation succeed !\n");
	}

	if(fOutput != NULL)
	{
		fclose(fOutput);		
	}
	else {
		printf("transformation error !\n");
	}

}


 