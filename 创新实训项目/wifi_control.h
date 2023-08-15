#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_
void ESP8266_ModeInit(void);
void ESP8266_SendCmd(unsigned char *pbuf);
void ESP8266_SendData(unsigned char *pbuf);
#endif