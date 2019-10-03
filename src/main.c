#include <my_pj.h>

pj_mutex_t *mtx;
pj_pool_t *pool_mtx;

pjsua_conf_port_id ringback_slot;
pjsua_player_id player_ids[MY_MAX_CALLS];
pjsua_conf_port_id  answer_RBT_slot;
pjsua_conf_port_id  answer_continuous_slot;

/* Adding ports for conference: ringback, players and sounds */
void init_sounds(pj_pool_t *pool,
				pjsua_media_config media_cfg)
{
	pj_status_t status;
	pjmedia_tone_desc tone[1];

	/* Adding ringback tone */
	pj_bzero(&tone, sizeof(tone));
	for (int i = 0; i < RINGBACK_CNT; ++i)
	{
		tone[i].freq1 = RINGBACK_FREQ1;
		tone[i].freq2 = RINGBACK_FREQ2;
		tone[i].on_msec = RINGBACK_ON;
		tone[i].off_msec = RINGBACK_OFF;
	}
	status = add_tone("ringback", pool, media_cfg, &ringback_slot, tone);
	if (status != PJ_SUCCESS)
	{
		error_exit("Doesn't able add tone", status);
	}
	
	/* Adding continuous tone */
	pj_bzero(&tone, sizeof(tone));
	for (int i = 0; i < CONTINUOUS_CNT; ++i)
	{
		tone[i].freq1 = CONTINUOUS_FREQ1;
		tone[i].freq2 = CONTINUOUS_FREQ2;
		tone[i].on_msec = CONTINUOUS_ON;
		tone[i].off_msec = CONTINUOUS_OFF;
	}
	status = add_tone("continuous", pool, media_cfg, &answer_continuous_slot, tone);
	if (status != PJ_SUCCESS)
	{
		error_exit("Doesn't able add tone", status);
	}
	
	/* Adding RBT*/
	pj_bzero(&tone, sizeof(tone));
	for (int i = 0; i < RB_RUS_CNT; ++i)
	{
		tone[i].freq1 = RB_RUS_FREQ1;
		tone[i].freq2 = RB_RUS_FREQ2;
		tone[i].on_msec = RB_RUS_ON;
		tone[i].off_msec = RB_RUS_OFF;
	}
	status = add_tone("RBT", pool, media_cfg, &answer_RBT_slot, tone);
	if (status != PJ_SUCCESS)
	{
		error_exit("Doesn't able add tone", status);
	}
	
	/* Add players for 20 possible callers */
	for (int i = 0; i < MY_MAX_CALLS; i++) {
		const pj_str_t wav_sound = pj_str(WAV_NAME);
		status = pjsua_player_create(&wav_sound , PJMEDIA_FILE_NO_LOOP, &player_ids[i]);
		if (status != PJ_SUCCESS)
		{
			error_exit("Player didn't be created", status);
		}
	}
}

pj_thread_proc *call_acceptance(void *arg)
{
	pjsua_call_info call_info;
	memcpy(&call_info, arg, sizeof(pjsua_call_info));
	//pj_mutex_unlock(mtx);
	int status;
	const int sip_user = atoi(SIP_USER);
	pjsua_conf_connect(ringback_slot, call_info.conf_slot);
	pj_thread_sleep(5000);
	pjsua_conf_disconnect(ringback_slot, call_info.conf_slot);
	char ident[12];
	int i_ident, size_ident;
	size_ident = sizeof(ident);
	get_ident(call_info.local_info.ptr, call_info.local_info.slen, ident, &size_ident);
	i_ident = atoi(ident);
	if (sip_user == i_ident)
	{
		pjsua_conf_connect(answer_continuous_slot, call_info.conf_slot);
	}
	else if (sip_user == (i_ident+1))
	{
		pjsua_conf_connect(pjsua_player_get_conf_port(player_ids[call_info.id]), call_info.conf_slot);
	}
	else
	{
		pjsua_conf_connect(answer_RBT_slot, call_info.conf_slot);
	}
}

/* Callback called by the library upon receiving incoming call */
void on_incoming_call(pjsua_acc_id acc_id,
					  pjsua_call_id call_id,
					  pjsip_rx_data *rdata)
{
	pjsua_call_info ci;
	int size_num, status;
	PJ_UNUSED_ARG(acc_id);
	PJ_UNUSED_ARG(rdata);
	pjsua_call_get_info(call_id, &ci);
	status = get_len_ident(ci.local_info.ptr, ci.local_info.slen, &size_num);
	if (status != GLI_SUCC)
	{
		error_exit("Wrong URL", status);
	}
	char num[size_num];
	get_ident(ci.local_info.ptr, ci.local_info.slen, num, &size_num);
	PJ_LOG(3, (THIS_FILE, "%s Incoming call from %.*s!!",
			   num,
			   (int)ci.remote_info.slen,
			   ci.remote_info.ptr));

	/* Automatically answer incoming calls with 200/OK */
	pjsua_call_answer(call_id, 200, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
void on_call_state(pjsua_call_id call_id,
				   pjsip_event *e)
{
	pjsua_call_info ci;

	PJ_UNUSED_ARG(e);

	pjsua_call_get_info(call_id, &ci);
	PJ_LOG(3, (THIS_FILE, "Call %d state=%.*s", call_id,
			   (int)ci.state_text.slen,
			   ci.state_text.ptr));
}

/* Callback called by the library when call's media state has changed */
void on_call_media_state(pjsua_call_id call_id)
{
	pjsua_call_info call_info;
	pj_pool_t *tmp_pool;
	pj_thread_t *ptr;
	pj_status_t status;
	tmp_pool = pjsua_pool_create("threads_pool", 1000, 1000);
	//pj_mutex_lock(mtx);
	pjsua_call_get_info(call_id, &call_info);
	PJ_LOG(3, (THIS_FILE, "Call_media %d state=%.*s", call_id,
			   (int)call_info.state_text.slen,
			   call_info.state_text.ptr));
	if (call_info.media_status == PJSUA_CALL_MEDIA_ACTIVE)
	{
		status = pj_thread_create(tmp_pool, "call_acceptance", (pj_thread_proc *) &call_acceptance,
						 &call_info, PJ_THREAD_DEFAULT_STACK_SIZE, 0, &ptr);
	//	pj_mutex_lock(mtx);
	//	pj_mutex_unlock(mtx);
		//pjsua_conf_connect(/*pjsua_player_get_conf_port(player_ids[ci.id])*/ answer_continuous_slot, call_info.conf_slot);
	}
	PJ_LOG(3, (THIS_FILE, "Exit MMM"));
}


/*
  * main()
  *
  * argv[1] may contain URL to call.
  */
int main()
{
	pjsua_acc_id acc_id;
	pj_status_t status;
	pjsua_config cfg;
	pjsua_logging_config log_cfg;
	pjsua_media_config media_cfg;
	pj_pool_t *pool;

	/* Create pjsua first! */
	status = pjsua_create();
	if (status != PJ_SUCCESS)
		error_exit("Error in pjsua_create()", status);

	/* Init pjsua */
	pjsua_config_default(&cfg);
	cfg.max_calls = MY_MAX_CALLS;
	cfg.cb.on_incoming_call = &on_incoming_call;
	cfg.cb.on_call_media_state = &on_call_media_state;
	cfg.cb.on_call_state = &on_call_state;

	pjsua_logging_config_default(&log_cfg);
	log_cfg.console_level = 4;

	pjsua_media_config_default(&media_cfg);

	status = pjsua_init(&cfg, &log_cfg, &media_cfg);
	if (status != PJ_SUCCESS)
		error_exit("Error in pjsua_init()", status);

	/* Add UDP transport. */
	{
		pjsua_transport_config cfg;

		pjsua_transport_config_default(&cfg);
		cfg.port = 5078;
		status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
		if (status != PJ_SUCCESS)
			error_exit("Error creating transport", status);
	}

	pool = pjsua_pool_create("tonegen", 1000, 1000);
	init_sounds(pool, media_cfg);
	pool_mtx = pjsua_pool_create("pool_mtx", sizeof(pj_mutex_t *), 2);
	pj_mutex_create(pool_mtx, "mtx", PJ_MUTEX_DEFAULT, &mtx);
	//---------------------------------------------------------------------------------
	/* Initialization is done, now start pjsua */
	status = pjsua_start();
	if (status != PJ_SUCCESS)
		error_exit("Error starting pjsua", status);

	/* Register to SIP server by creating SIP account. */
	for (int i = atoi(SIP_USER); i < (atoi(SIP_USER) + VALUE_DOMAINS); i++)
	{
		pjsua_acc_config cfg;
		char sip_usr[32];
		char sip_url[64];
		snprintf(sip_usr, 32, "%d", i);
		snprintf(sip_url, 64, "sip:%s@%s", sip_usr, SIP_DOMAIN);
		pjsua_acc_config_default(&cfg);
		cfg.id = pj_str(sip_url);
		cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
		cfg.cred_count = 1;
		cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
		cfg.cred_info[0].scheme = pj_str("digest");
		cfg.cred_info[0].username = pj_str(sip_usr);
		cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
		cfg.cred_info[0].data = pj_str(SIP_PASSWD);

		status = pjsua_acc_add(&cfg, PJ_FALSE, &acc_id);
		if (status != PJ_SUCCESS)
			error_exit("Error adding account", status);
	}

	/* Wait until user press "q" to quit. */
	for (;;)
	{
		char option[10];

		puts("Press 'h' to hangup all calls, 'q' to quit");
		if (fgets(option, sizeof(option), stdin) == NULL)
		{
			puts("EOF while reading stdin, will quit now..");
			break;
		}

		if (option[0] == 'q')
			break;

		if (option[0] == 'h')
			pjsua_call_hangup_all();
	}

	/* Destroy pjsua */
	pj_pool_release(pool);
	pjsua_destroy();

	return 0;
}