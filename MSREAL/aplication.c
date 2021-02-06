#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <iostream>

#define MAX_BRAM_SIZE 8192*4
#define MAX_ASS_SIZE 16384
#define MAX_KERNEL_SIZE 4096*8
#define ASS_SEND 16
#define BLOCK_SIZE 8192*4
#define MMAP

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

string sock_read(int sockfd){
	const int MAX =256;
	char buff[MAX];
	bzero(buff, MAX);

	read(sockfd, buff, sizeof(buff));
	string message(buff);

	return message;

}

int main(int argc, char *argv[])
{

	int sound_array[8192], i =0, j=0;
	int *scrambler_sound_array,*descrambled_sound_array;
	int  *ass;
	int fk, fb, fc, fr;
	int *k,*kk,*g, *b,*bb, *c, *r;
for(int l=0;l<=8191;l++)
{
sound_array[l] = l;

}
//*********************************************sending data to bram0**************************************************************//

fk = open("/dev/bram0", O_RDWR|O_NDELAY);
	if (fk < 0)
	{
		printf("Cannot open /dev/bram0 for write\n");
		return -1;
	}


k=(int*)mmap(0,MAX_KERNEL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fk, 0);
	if (k == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

memcpy(k, sound_array, BLOCK_SIZE);
	munmap(k, BLOCK_SIZE);
	printf("bram0 done\n");
	close(fk);
	if(fk < 0)
	{
		printf("cannot close /dev/bram0 for write\n");
		return -1;
	}
//*******************************************************************************************************************//

//****************************************************setting start register to 1************************************//

int ass_reg[3]={0,1,0};

	fc = open("/dev/scrambler", O_RDWR|O_NDELAY);
	if (c < 0)
	{
		printf("Cannot open /dev/scrambler for write\n");
		return -1;
	}
	c=(int*)mmap(0, MAX_ASS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fc, 0);
	if (c == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(c, ass_reg, ASS_SEND);
	munmap(c, ASS_SEND);
	printf("scrambler done\n");
	close(fc);
	if (fc < 0)
	{
		printf("Cannot close /dev/scrambler for write\n");
		return -1;
	}



//*********************************************************************************************************************//
//*********************************************************clearing start bit******************************************//
int ass_reg1[3]={0,0,0};

        fc = open("/dev/scrambler", O_RDWR|O_NDELAY);
        if (c < 0)
        {
                printf("Cannot open /dev/scrambler for write\n");
                return -1;
        }
        c=(int*)mmap(0, MAX_ASS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fc, 0);
        if (c == NULL ) {
                printf ("\ncouldn't mmap\n");
                return 0;
        }                                                                                                                                                                                                                                                                                                                                                                                                                                     memcpy(c, ass_reg1, ASS_SEND);
        munmap(c, ASS_SEND);
        printf("scrambler done\n");
        close(fc);
        if (fc < 0)
        {
                printf("Cannot close /dev/scrambler for write\n");
                return -1;
        }



//*****************************************copying data from bram1 to scrambler_sound_array array *********************//

fb = open("/dev/bram1", O_RDWR|O_NDELAY);
	if (fb < 0)
	{
		printf("Cannot open /dev/bram1 for write\n");
		return -1;
	}

	scrambler_sound_array = (int *) malloc(MAX_BRAM_SIZE);

	g=(int*)mmap(0,MAX_BRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	if (g == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	
	memcpy(scrambler_sound_array, g, MAX_BRAM_SIZE);

	munmap(g, MAX_BRAM_SIZE);
	printf("bram1 done\n");
	close(fb);
	if(fb < 0)
	{
		printf("cannot close /dev/bram1 for write\n");
		return -1;
	}
//*********************************************************************************************************//


//*************************************sendin scrambled results to file***********************************//
FILE *fm;
	fm =fopen("scrambled.txt","w");
	if (fm==NULL)
	{
		printf("cannot open scrambled.txt\n");
		return -1;
	}

	printf("scrambled opened\n");
	for(i = 0;i<= 8191; i++)//lines-1
	{
		
			fprintf(fm,"%d ",scrambler_sound_array[i]);
			fflush(fm);

		fprintf(fm,"\n");
	}

	fprintf(fm,"\n");
	printf("scrambled sound written\n");

	if(fclose(fm) == EOF)
	{
		printf("cannot close scrambled.txt\n");
		return -1;
	}
//*************************************************************************************************************//
//************************************sending scrambled results to bram0**************************************//

fk = open("/dev/bram0", O_RDWR|O_NDELAY);
	if (fk < 0)
	{
		printf("Cannot open /dev/bram0 for write\n");
		return -1;
	}
	kk=(int*)mmap(0,MAX_KERNEL_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fk, 0);
	if (kk == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(kk, scrambler_sound_array, BLOCK_SIZE);
	munmap(kk, BLOCK_SIZE);
	printf("bram0 done\n");
	close(fk);
	if(fk < 0)
	{
		printf("cannot close /dev/bram_kernel for write\n");
		return -1;
	}
	cout<<"sending scrambled data to bram0"<<endl;
//*********************************************************************************************************************//

//****************************************************setting start register to 1************************************//

int ass_reg2[3]={0,1,0};

        fc = open("/dev/scrambler", O_RDWR|O_NDELAY);
        if (c < 0)
        {
                printf("Cannot open /dev/scrambler for write\n");
                return -1;
        }
        c=(int*)mmap(0, MAX_ASS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fc, 0);
        if (c == NULL ) {
                printf ("\ncouldn't mmap\n");
                return 0;
        }                                                                                                                                                                                                                                                                                                                                                                                                                                     memcpy(c, ass_reg2, ASS_SEND);
        munmap(c, ASS_SEND);
        printf("scrambler done\n");
        close(fc);
        if (fc < 0)
        {
                printf("Cannot close /dev/scrambler for write\n");
                return -1;
        }



//*********************************************************************************************************************//
//*********************************************************clearing start bit******************************************//
int ass_reg3[3]={0,0,0};

	fc = open("/dev/scrambler", O_RDWR|O_NDELAY);
	if (c < 0)
	{
		printf("Cannot open /dev/scrambler for write\n");
		return -1;
	}
	c=(int*)mmap(0, MAX_ASS_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fc, 0);
	if (c == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}

	memcpy(c, ass_reg3, ASS_SEND);
	munmap(c, ASS_SEND);
	printf("scrambling done\n");
	close(fc);
	if (fc < 0)
	{
		printf("Cannot close /dev/scrambler for write\n");
		return -1;
	}
//**********************************************************************************************************************//
//************reading descrambled results from an descrambled_sound_array variable and sending to file******************//
fb = open("/dev/bram1", O_RDWR|O_NDELAY);
	if (fb < 0)
	{
		printf("Cannot open /dev/bram_after_conv for write\n");
		return -1;
	}
	descrambled_sound_array = (int *) malloc(MAX_BRAM_SIZE);
	bb=(int*)mmap(0,MAX_BRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	if (bb == NULL ) {
		printf ("\ncouldn't mmap\n");
		return 0;
	}
	
	memcpy(descrambled_sound_array,bb, MAX_BRAM_SIZE);
	munmap(b, MAX_BRAM_SIZE);
	printf("bram1 done\n");
	close(fb);
	if(fb < 0)
	{
		printf("cannot close /dev/bram1 for write\n");
		return -1;
	}

//**********************************************************************************************************************//

//**********************************sending descrambled results to file************************************************//

FILE *dr;
	dr =fopen("descrambled_sound.txt","w");
	if (dr==NULL)
	{
		printf("cannot open descrambled_sound.txt\n");
		return -1;
	}
	printf("descrambled_sound opened\n");
	for(int ii = 0; ii<=8191; ii++)
	{

			fprintf(dr,"%d ",descrambled_sound_array[ii]);
			fflush(dr);
			fprintf(dr,"\n");
	}
	fprintf(dr,"\n");
	printf("descrambled_sound written\n");

	if(fclose(dr) == EOF)
	{
		printf("cannot close descramled_sound.txt\n");
		return -1;
	}


for (int ll=0;ll<=8192;ll++)

{

if(sound_array[ll] != descrambled_sound_array[ll])
{
printf("simulation failed \n");  
}
else
{
if (ll==8191){
printf("simulation passed \n");
}
}

}


//********************************************************************************************************************//
}
