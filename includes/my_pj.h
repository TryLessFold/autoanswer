#ifndef __MY_PJ__
#define __MY_PJ__ 

#include <pjsua.h>
#include <pjmedia.h>
#include <pjmedia_audiodev.h>
#include <string.h>

#include <get_ident.h>
#include <get_len_ident.h>
#include <add_tone.h>

#define THIS_FILE "main.c"

#define SIP_DOMAIN "192.168.23.134"
#define SIP_USER "3300"
#define SIP_PASSWD ""
#define VALUE_DOMAINS 3
#define MY_MAX_CALLS 20

#define RINGBACK_FREQ1 440 
#define RINGBACK_FREQ2 480 
#define RINGBACK_ON 2000 
#define RINGBACK_OFF 4000 
#define RINGBACK_CNT 1 

#define CONTINUOUS_FREQ1 425
#define CONTINUOUS_FREQ2 425 
#define CONTINUOUS_ON 1
#define CONTINUOUS_OFF 0 
#define CONTINUOUS_CNT 1 

#define RB_RUS_FREQ1 425 
#define RB_RUS_FREQ2 425 
#define RB_RUS_ON 1000 
#define RB_RUS_OFF 4000 
#define RB_RUS_CNT 1 

#define WAV_NAME "audio.wav"

extern pjsua_conf_port_id ringback_slot;
extern pjsua_player_id player_id[];
extern pjsua_conf_port_id answer_RBT_slot;
extern pjsua_conf_port_id answer_long_beep_slot;

void error_exit(const char *title,
                pj_status_t status);

#endif //__MY_PJ__