
#ifndef _MAIN_H_ 
#define _MAIN_H_

#include"type.h"
#include "sbc.h"





#define AUDIO_SAMPRATE  		44100			/*  ��Ƶ������  */
#define AUDIO_BITRATE 			384000			/*  ������bit��  */
#define AUDIO_CHANNELS 			2				/*  ������  */
#define AUDIO_BITPERSAMP 		16				/*  ÿ���������ݵ�λ��  */
#define AUDIO_RAWBLOCKSIZE 		(128*4)			/*  ����ǰÿ֡��С  */
#define AUDIO_BLOCKSIZE      	36				/*  �����ÿ֡�Ĵ�С  */


unsigned char  t[44];

#define RAW_FRAME_LENGTH	512		/* ÿ֡������������ݳ��� byte  */

#define hex_length  (352*1024+8*3+6)
#define row_length 43
 

#endif