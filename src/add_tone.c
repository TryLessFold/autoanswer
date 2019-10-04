#include <add_tone.h>

pj_status_t add_tone(char *tone_name,
            pj_pool_t *pool,
            pjsua_media_config media_cfg,
            pjsua_conf_port_id *ringback_slot,
            pjmedia_tone_desc *tone)
{
    pj_status_t status;
    pjmedia_port *ringback_port;
    unsigned samples_per_frame;
	pj_str_t name;

	samples_per_frame = media_cfg.audio_frame_ptime *
						media_cfg.clock_rate *
						media_cfg.channel_count / 1000;

	/* Ringback tone (call is ringing) */
	name = pj_str(tone_name);
	status = pjmedia_tonegen_create2(pool, &name,
									 media_cfg.clock_rate,
									 media_cfg.channel_count,
									 samples_per_frame,
									 16, PJMEDIA_TONEGEN_LOOP,
									 &ringback_port);
	if (status != PJ_SUCCESS)
        return status;

    status = pjmedia_tonegen_play(ringback_port, 1, tone,
						 PJMEDIA_TONEGEN_LOOP);
    if (status != PJ_SUCCESS)
        return status;
	status = pjsua_conf_add_port(pool, ringback_port,
	 							 ringback_slot);
	if (status != PJ_SUCCESS)
        return status;
    return PJ_SUCCESS;
}