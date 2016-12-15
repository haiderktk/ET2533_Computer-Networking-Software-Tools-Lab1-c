/* 
    submitted By: hara12
    Assignment task 3 Read ID3v2 tags form the provided mp3 file.
*/
#include "include.h"
int main(int argc, char *argv[])
{
	//defined header variables
    unsigned char IDtag[4];
    char version;
    int revision;
    char flags;
    uint32_t size;
    
    //Defining variables used for breaking down a frame 
	unsigned char frameID[4];
	uint32_t frameSize;
	unsigned char frameFlag[2];

    if (argc<2)
	{	
		printf("Usage: <silence.mp3>\n");
		return 0;	
	}
	if(strcmp(argv[1], "-h")==0)
	{
		printf("Usage: <silence.mp3>\n");
		return 0;	
	}	
    unsigned char buffer1[4], buffer2[4], flag[2], frame_data;
    int i;
    FILE * myMP3 = fopen(argv[1], "rb" );//read from mp3 file
    if (! myMP3 ){
        printf("Unable to read information from the MP3 file.\n"); 
    }
    else {
	   
        fseek( myMP3, 0 ,SEEK_SET);//pointer seeking at the beginning of the file
        if ( fread( (void *) buffer1 , 4 , 1 , myMP3) !=1){
            printf("Error! Unable to read the file\n");
            exit (0);
		}
        
        strncpy( (char *) IDtag , (char *) buffer1 , (size_t) 4);//copying 4 characters from the buffer1 to IDtag
        IDtag[3]='\0';//nulling the garbage value in the tag that is unreadable on the terminal display.
								
        if ( (size_t) fread( (void *) &version ,1 , 1 , myMP3) != 1){
            printf("Error! Unable to read the version.\n");
            exit (0);
        }
        
        if ( fread( (void *) &revision ,1, 1 ,myMP3) != 1){
            printf("Error! Unable to read the revision.\n");
            exit (0);
        }
        
		if ( fread( (void *) &flags , 1 , 1 , myMP3) !=1){
            printf("Error! Unable to read the flags.\n");
            exit (0);
		}
								
        if ( fread( (void *) buffer1 , sizeof(size) , 1 , myMP3) != 1){
            printf("Error! Unable to get size of the file.\n");
            exit (0);
        }

        size = (buffer1[3] & 0xFF) |((buffer1[2] & 0xFF) << 7 ) |((buffer1[1] & 0xFF) << 14 ) |((buffer1[0] & 0xFF) << 21 );//get size of the mp3 file
        
        printf("tagID    >> %s\n",IDtag);
        printf("Version  >> %d\n",version);
        printf("Revision >> %d\n",revision);
        printf("flags    >> %d\n",flags);
        
        fseek (myMP3 , 10 , SEEK_SET );
					 		
        //reading frames using a true loop
        while(1)
        {
            if ( fread( buffer2 ,4 ,1 , myMP3 ) !=1)
            {
                printf("Error! Could NOT read frame IDtag. \n");
                exit (0);
            }
            
            strncpy(frameID , buffer2 , 4 );
            
            
            if ( fread( buffer1 , 4 , 1 , myMP3 ) !=1)
            {
                printf("Error! Could NOT get size of data frame.\n");
                exit (0);
            }
            
            //getting the frame data size
            frameSize = (buffer1[3] & 0xFF) | ((buffer1[2] & 0xFF) << 7 ) | ((buffer1[1] & 0xFF) << 14 ) | ((buffer1[0] & 0xFF) << 21 );
            
            if ( fread(flag ,2 , 1 ,myMP3) !=1)
            {
                printf("Error! Could NOT read flags\n");
                exit (0);
            }
            
            strncpy( frameFlag , flag , 2 );
            
            // # TALB:album name, TCON: content type, TIT2:title , TPE1:lead performer(Artist), TRCK: track No. ,TYER: year
            if ((strncmp( frameID , "TPE1" ,  4 ) == 0 ) ||( strncmp( frameID , "TALB" ,  4 ) == 0 ) ||( strncmp(frameID , "TIT2" ,  4 ) == 0 )||(strncmp ( frameID , "TYER" , 4 ) == 0 )||( strncmp ( frameID , "TCON" ,  4 ) == 0 )||( strncmp ( frameID , "TRCK" ,  4 ) == 0 ))
            {
                //show information of mp3 tag on the screen
                for(i=0;i<4;i++)
                {
                    printf("%c", frameID[i]);
                }
                
                printf("  >>  ");
                
                for ( i = 0; i < frameSize; i++ )
                {
                    frame_data = fgetc(myMP3);
                    printf( "%c" , frame_data );
                    
                }
                printf("\n");
                
            }
            
            else
            {
                break;
            }
        }						
        
        fclose(myMP3);
    }

	return 0;
}
