
static void ap_task(PKT_T *pkt, AP_TASK_T task) {

	switch (task) {
		case AP_INIT:        	//AP初始化
			ap_beacom_timer_start();
			ap_enable_int();
			break;
		case AP_SEND_BEACOM:   //AP发送广播桢
			ap_send_beacom();
			break;
		case AP_RCVD_SYNC:     //AP收到同步请求桢
			//break;
		case AP_SEND_SYNC_ACK: //AP回复同步ACK
			ap_send_nw_info(pkg->id);
			break;
		case AP_RCVD_PULL:     //AP收到PULL请求
			ap_reset_beacom_id(pkt->data.id);
			ap_send_to_eth(pkt->data.id);
			break;
		case AP_SEND_DATA:     //AP发送数据
			if (ap_wakeuped_id(pkt->data.id) == true) {
				ap_set_beacom_id(pkt->data.id);
				ap_send_data(pkt->data.data);
			} else {
				ap_set_wakeup_id(pkt->data.id);
			}
			break;
		case AP_RECV_DATA_ACK: //AP收到数据ACK
			ap_send_to_eth(pkt);
			break;
		case AP_SLEEP:         //AP进入休眠
		default:
			wait_for_int();		
			break;	
	}
}

void ap_main(void) {
	PKT_T eth_pkg;
	UINT32 id;
	PKT_T rf_pkg;
	RING_BUF	task_buf;
	AP_TASK_T task;
	
	ap_init_task_ring_buf(&task_buf);
	ap_register_eth_int(&eth_pkg);
	ap_register_rf_int(&rf_pkg);
	ap_start_beacom_timer();
	
	ap_task(AP_INIT);

	while (1) {
		task = get_task(&task_buf);
		ap_task(task != AP_SEND_DATA ? &rf_pkg : &eth_pkg, task);
	}
}

static void ep_task(PKT *pkt) {
	switch (ep_task) {
		case EP_INIT:          //终端初始化
			break;
		case EP_RECV_BEACOM:   //终端接收广播桢
			break;
		case EP_SEND_SYNC:     //终端发送同步请求
			break;
		case EP_RECV_SYNC_ACK: //终端受到同步ACK
			break;
		case EP_SEND_PULL:     //终端发送PULL请求桢
			break;
		case EP_RECV_DATA:     //终端收到数据
			break;
		case EP_SEND_DATA_ACK: //终端回复数据ACK
			break;
		case EP_SLEEP:          //终端进入休眠	
			break;
		default:
			break;	
	}
}








