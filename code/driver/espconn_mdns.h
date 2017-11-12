/**
 * @file espconn_mdns.h
 * @author Lucas
 * @date Nov 5, 2017
 *
 * @brief
 */

#ifndef CODE_DRIVER_ESPCONN_MDNS_H_
#define CODE_DRIVER_ESPCONN_MDNS_H_

#ifndef _MDNS_INFO
#define _MDNS_INFO
struct mdns_info {
	char *host_name;
	char *server_name;
	uint16 server_port;
	unsigned long ipAddr;
	char *txt_data[10];
};
#endif
/******************************************************************************
 * FunctionName : espconn_mdns_init
 * Description  : register a device with mdns
 * Parameters   : ipAddr -- the ip address of device
 * 				  hostname -- the hostname of device
 * Returns      : none
*******************************************************************************/

void espconn_mdns_init(struct mdns_info *info);
/******************************************************************************
 * FunctionName : espconn_mdns_close
 * Description  : close a device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/

void espconn_mdns_close(void);
/******************************************************************************
 * FunctionName : espconn_mdns_server_register
 * Description  : register a device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/
void espconn_mdns_server_register(void);

/******************************************************************************
 * FunctionName : espconn_mdns_server_unregister
 * Description  : unregister a device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/
void espconn_mdns_server_unregister(void);

/******************************************************************************
 * FunctionName : espconn_mdns_get_servername
 * Description  : get server name of device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/

char* espconn_mdns_get_servername(void);
/******************************************************************************
 * FunctionName : espconn_mdns_set_servername
 * Description  : set server name of device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/
void espconn_mdns_set_servername(const char *name);

/******************************************************************************
 * FunctionName : espconn_mdns_set_hostname
 * Description  : set host name of device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/
void espconn_mdns_set_hostname(char *name);

/******************************************************************************
 * FunctionName : espconn_mdns_get_hostname
 * Description  : get host name of device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/
char* espconn_mdns_get_hostname(void);

/******************************************************************************
 * FunctionName : espconn_mdns_disable
 * Description  : disable a device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/
void espconn_mdns_disable(void);

/******************************************************************************
 * FunctionName : espconn_mdns_enable
 * Description  : disable a device with mdns
 * Parameters   : a
 * Returns      : none
*******************************************************************************/
void espconn_mdns_enable(void);

#endif
