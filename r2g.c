/* Edit by lychee at 2013.3.17 */
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

int IW=640;   /* Input image width */
int IH=480;   /* Input image height */      
int LENGTH=640*480*3;   /* The length of image file in Byte */
int newIW=330;   /* Output image width */
int newIH=330;   /* Output image height */
int HIDIFF=40;   /* The pixel from top image to up edge*/
/*int WIDIFF=37;  */ /* The pixel from detected brim to left edge*/
int WIDIFF=0;   /* The pixel from detected brim to left edge*/

/* Insert into sorcecode to debug */
/*
int debug(int n)
{
	printf("%d\n",n);
	fflush(stdout);
	return 0;
}

*/

/* Detect left brim. Input:1.original img pointer 2.the line to detect. 
   Return:pixel from left image to brim  */
int findBrim(unsigned char *img,int line)
{
	int i,j;
	int cell,cell0;
	int diff;
	int bricount = 0;
	cell0 = (*(img+line*IW) +*(img+line*IW+1)+*(img+line*IW+2))/3;
	for(i=0;i<300;i++){
		cell = (*(img+line*IW+i) +*(img+line*IW+i+1)+*(img+line*IW+i+2))/3;
		diff = cell - cell0;
		if(diff<0)
			bricount++;
		else
			bricount=0;
		
		if(bricount>=5){
			printf("brim=%d\n",i);
			return i;
		}

		cell0=cell;
		/*	
		printf("x=%d \tpixel=%d ",i,*(img+line*IW+i));
		printf("\tdeff=%d\n",diff);
		*/
	}
	return -1;
}



/* Cut out the real fingerprint image */
int crop(unsigned char *img,int brim,unsigned char *newImg)
{
	int i,j;
	for(i=0;i<newIH;i++)
		for(j=0;j<newIW;j++){
			*(newImg+i*newIW+j) = *(img+(i+HIDIFF)*IW+brim+WIDIFF+j);	
		}
	return 0;
}

/* Main */
int main(int argc,char **argv)
{
	
	if(argc!=5){
		printf("Usage:./rgb2grey inputFile outputFile imgW imgH\n");
		return -1;
	}
	
		
	IW=atoi(argv[3]);
	IH=atoi(argv[4]);
	LENGTH=IW*IH*3;
	/*
	IW=640;
	IH=480;
	LENGTH=IW*IH*3;
	*/
	/* 1.Input RGB image*/
	FILE *fp;
	unsigned char *A;
	fp = fopen(infile,"rb");
	if(fp == NULL)
		printf("raw open fail\n");
	A =(unsigned char *) malloc(LENGTH);
	if(fread(A,1,LENGTH,fp)!=LENGTH)
		printf("raw read fail\n");
	fclose(fp);
	
	/* 2.RGB to Grey */
	int i,j,k,n=0;
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char *Grey;
	Grey =(unsigned char *) malloc(IW*IH);

	for(i=0;i<IH;i++)
		for(j=0;j<IW;j++)
			for(k=0;k<3;k++){
				switch(k){
					case 0: R=A[n];break;
					case 1: G=A[n];break;
					case 2: B=A[n];
							*(Grey+i*IW+j)=(R*30+G*59+B*11+50)/100;
							break;
				}
				n++;
			}
	
	/* 3.Detect brim */
	int brim[3];
	brim[1]=findBrim(Grey,40);
	if(brim[1] == -1){
		printf("Detect brim fail\n");
		return -1;
	}

	/* 4.Cut out image */
	unsigned char *newGrey;
	newGrey = (unsigned char *)malloc(newIW*newIH);
	/*crop(Grey,brim[1],newGrey);*/
	crop(Grey,185,newGrey);

	/* 5.Output Greyscale image */
	FILE *fpnewGrey;

	fpnewGrey=fopen(outfile,"w+");
	if(fpnewGrey==NULL)
		printf("fpnewGrey open fail\n");
	
	if(fwrite(newGrey,1,newIW*newIH,fpnewGrey)!=newIW*newIH)
		printf("fpnewGrey write fail\n");
	fclose(fpnewGrey);
	return 0;
}
