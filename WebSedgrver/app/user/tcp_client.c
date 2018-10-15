/*
 * client.c
 *
 *  Created on: 2018��3��23��
 *      Author: Leroi
 */
#include "tcp_client.h"

void ICACHE_FLASH_ATTR user_tcp_discon_cb(void *arg){
	os_printf("�Ͽ����ӳɹ���\r\n");
}
void ICACHE_FLASH_ATTR user_tcp_sent_cb(void *arg){
	os_printf("���ͳɹ���\r\n");
}
void ICACHE_FLASH_ATTR user_tcp_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("�յ����ݣ�%s\r\n",pdata);
	os_delay_us(300);
	espconn_disconnect((struct espconn *)arg);
}
void ICACHE_FLASH_ATTR user_tcp_recon_cb(void *arg,sint8 err){
	os_printf("���Ӵ��󣬴������Ϊ %d\r\n",err);
	espconn_connect((struct espconn *)arg);
}
void ICACHE_FLASH_ATTR user_tcp_connect_cb(void *arg){
	os_printf("\r\n ���� \r\n");
	struct espconn *pespconn=arg;
	espconn_regist_recvcb(pespconn,user_tcp_recv_cb);//���ջص�����
	espconn_regist_sentcb(pespconn,user_tcp_sent_cb);//���ͳɹ��Ļص�����
	espconn_regist_disconcb(pespconn,user_tcp_discon_cb);//�����Ļص�����
	espconn_sent(pespconn,"����esp8266",strlen("����esp8266"));
}


void ICACHE_FLASH_ATTR leroi_client_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port){
	os_printf("\r\n ���ò��� \r\n");
	//��������
	user_tcp_conn.type = ESPCONN_TCP;
	user_tcp_conn.state=ESPCONN_NONE;
	user_tcp_conn.proto.tcp=(esp_tcp *)os_zalloc(sizeof(esp_tcp));
	os_memcpy(user_tcp_conn.proto.tcp->local_ip,local_ip,4);
	os_memcpy(user_tcp_conn.proto.tcp->remote_ip,remote_ip,4);
	user_tcp_conn.proto.tcp->local_port=espconn_port();
	user_tcp_conn.proto.tcp->remote_port=remote_port;
	os_printf("\r\n ������� \r\n");
	//ע�����ӻص������������ص�����
	espconn_regist_connectcb(&user_tcp_conn,user_tcp_connect_cb);
	espconn_regist_reconcb(&user_tcp_conn,user_tcp_recon_cb);
	//��������
	espconn_connect(&user_tcp_conn);

}
