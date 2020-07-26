
#ifndef _MAIN_H_ 
#define _MAIN_H_

#include"type.h"
#include "sbc.h"





#define AUDIO_SAMPRATE  		44100			/*  音频采样率  */
#define AUDIO_BITRATE 			384000			/*  编码后的bit率  */
#define AUDIO_CHANNELS 			2				/*  声道数  */
#define AUDIO_BITPERSAMP 		16				/*  每个采样数据的位宽  */
#define AUDIO_RAWBLOCKSIZE 		(128*4)			/*  编码前每帧大小  */
#define AUDIO_BLOCKSIZE      	36				/*  编码后每帧的大小  */


unsigned char  t[44];

#define RAW_FRAME_LENGTH	512		/* 每帧的输入采样数据长度 byte  */

#define hex_length  (352*1024+8*3+6)
#define row_length 43
 

#endif