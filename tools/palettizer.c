#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    printf("%d\n",sizeof(long double));
    if(argc != 3)
    {
        printf("%s\n",argv[0]);
        printf("Error - Command format: <executable> <input bmp> <output text>\n");
        return 0;
    }

    char* buf = (char*)malloc(256*sizeof(char));

    FILE* input = fopen(argv[1],"rb");

    FILE* output = fopen(argv[2],"w");

    snprintf(buf,256,"unsigned char palette[] = {");

    fwrite(buf,strlen(buf),sizeof(char),output);

    fread(buf,54,1,input); //read the bmp headers

    if(buf[0x1C] != 24)
    {
        printf("Wrong bit depth - not 24\n");
        return 0;
    }

    if(!(buf[0x12] == 0 && buf[0x13] == 1 && buf[0x14] == 0 && buf[0x15] == 0))
    {
        printf("Wrong image width - not 256\n");
        return 0;
    }

    unsigned char tmp[4];
    tmp[3]=0;
    
    for(int i=0;i<256;i++)
    {
        fread(tmp,3,1,input);
    
        snprintf(buf,256," %d, %d, %d, %d",tmp[0],tmp[1],tmp[2],tmp[3]);
        fwrite(buf,strlen(buf),sizeof(char),output);
    
        if(i != 255)
            putc(',',output);
    }

    snprintf(buf,256,"};");
    fwrite(buf,strlen(buf),sizeof(char),output);

    fclose(input);
    fclose(output);

    
    
    return 0;
}