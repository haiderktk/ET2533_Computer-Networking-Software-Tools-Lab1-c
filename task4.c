/* 
*/
#include "include.h"
int main(int argc, char *argv[])
{
    //defined header variables
    unsigned char IDtag[4];
    
    //defined fram variables
	   unsigned char frameID[4];
	   uint32_t frameSize;
	   unsigned char frameFlag[2];
    
    if (argc<4)
    {
        printf("Usage: <file.mp3> <title> <artist>\n");
        return 0;
    }
    
    
    char *NewTitle=argv[2];// take new title as an argument
    char *NewArtist=argv[3]; //take new artist as an argument
    int m=sizeof(argv[1]);
    char *buf=(char*)malloc(m*sizeof(char));
    char data;
    if(strcmp(argv[1], "-h")==0)
    {
        printf("<file.mp3>\n");
        return 0;
    }
    unsigned char buffer1[4], buffer2[4], flag[2], frameInfo;
    int i;
    size_t len1,len2;
    FILE * myMP3 = fopen(argv[1], "r+b" );//read from mp3 file
    FILE *myNewMP3= fopen("newFile.mp3","w+");
    if (! myMP3 )
    {
        printf("Unable to get information from MP3 file.\n");
    }
    
    else if(! myNewMP3)
            {
                printf("Unable to get information from NewMP3 file.\n");
            }
    else{
            while((len1=fread(buf,1,sizeof(buf),myMP3))> 0)
            {
                len2=fwrite(buf,1,len1,myNewMP3);
        
            }
        
        //pointer to the file object that identify input stream from the beginning of file
        fseek( myMP3, 10 ,SEEK_SET);
        fseek( myNewMP3, 10 ,SEEK_SET);
        
        while(1)
        {
            bzero(frameID,4);
            if(fread(buffer1,4,1,myMP3)!=1)
            {
                printf("Error! Could NOT read frame ID.\n");
                exit(0);
            }
            strncpy(frameID , buffer1 , 4 );
            fwrite(buffer1, 4,1, myNewMP3);
            
            if ( fread( buffer2 , 4 , 1 , myMP3 ) !=1)
            {
                printf("Error! Could NOt read file frame.\n");
                exit (0);
            }

            frameSize = (buffer2[3] & 0xFF) |
            ((buffer2[2] & 0xFF) << 7 ) |
            ((buffer2[1] & 0xFF) << 14 ) |
            ((buffer2[0] & 0xFF) << 21 );
            
            unsigned int size;
            if (strncmp( frameID , "TPE1" ,  4 ) == 0 )
            {
                size=strlen(NewArtist);
                buffer2[3] = size & 0x7f;
                buffer2[2] = (size >> 7) & 0x7f;
                buffer2[1] = (size >> 14) & 0x7f;
                buffer2[0] = (size >> 21) & 0x7f;
                fwrite(buffer2,4,1,myNewMP3);
            }
            else if( strncmp( frameID , "TIT2" ,  4 ) == 0 )
            {
                size=strlen(NewTitle);
                buffer2[3] = size & 0x7f;
                buffer2[2] = (size >> 7) & 0x7f;
                buffer2[1] = (size >> 14) & 0x7f;
                buffer2[0] = (size >> 21) & 0x7f;
                fwrite(buffer2,4,1,myNewMP3);
                
            }
            else{
                fwrite(buffer2,4,1,myNewMP3);
            }
            if ( fread(flag ,2 , 1 , myMP3) !=1)
            {
                printf("Error!Could NOT read file.\n");
                exit (0);
            }
            
            strncpy( frameFlag , flag , 2 );
            fwrite(flag,2,1,myNewMP3);
            
            
            if ((strncmp( frameID , "TPE1" ,  4 ) == 0 ) ||( strncmp( frameID , "TALB" ,  4 ) == 0 ) ||( strncmp( frameID , "TIT2" ,  4 ) == 0 )||(strncmp ( frameID , "TYER" , 4 ) == 0 )||( strncmp ( frameID , "TCON" ,  4 ) == 0 )||( strncmp ( frameID , "TRCK" ,  4 ) == 0 ))
               {
                
                if (strncmp( frameID , "TPE1" ,  4 ) == 0 )
                {
                    
                    for ( i = 0; i < frameSize; i++ )
                    {
                        frameInfo = fgetc(myMP3);
                    }
                    
                    for ( i = 0; i < size; i++ )
                    {
                        data = NewArtist[i];
                        fwrite(&data,1,1,myNewMP3);
                    }
                }
                
                else if ( strncmp( frameID , "TIT2" ,  4 ) == 0 )
                {
                    
                    for ( i=0 ; i < frameSize; i++ )
                    {
                        frameInfo = fgetc(myMP3);
                    }
                    for ( i=0 ; i < size; i++ )
                    {
                        data = NewTitle[i];
                        fwrite(&data,1,1,myNewMP3);
                    }
                }
                else
                {									
                    for ( i = 0; i < frameSize; i++ )
                    {
                        frameInfo= fgetc(myMP3);
                        fwrite(&frameInfo,1,1,myNewMP3);
                    }							
                }								
                
            }							
            else
            {
                break;
            }
        }
        
        fseek (myMP3 , 0 , SEEK_SET );
        fseek (myNewMP3 , 0 , SEEK_SET );
        while((len2=fread(buf,1, sizeof (buf), myNewMP3)) > 0)
        {
            len1=fwrite(buf,1,len2, myMP3);
        }
        if(remove("newFile.mp3") == 0) { printf("Editing process finished.\n" );}
        else {printf("Create new copy for editing.\n");}
        
        printf("Artist's name replaced with  %s\n",NewArtist);
        printf("Album's Title replaced with  %s\n",NewTitle);
    }
    
    
        fclose(myMP3);
        fclose(myNewMP3);
    
    
return 0;
}

