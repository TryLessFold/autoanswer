#ifndef __ADD_TONE__
#define __ADD_TONE__

#define GI_SUCC 0

#include <my_pj.h>

/*
 * This function creates tone slot for conference to ringback_slot.
 */

pj_status_t add_tone(char* tone_name,
            pj_pool_t *pool,
            pjsua_media_config media_cfg,
            pjsua_conf_port_id *ringback_slot,
            pjmedia_tone_desc *tone);

#endif //__ADD_TONE__