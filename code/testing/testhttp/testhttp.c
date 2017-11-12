/**
 * @file testhttptask.c
 * @author Lucas
 * @date Oct 29, 2017
 *
 * @brief
 */
#include <string.h>
#include "testhttp.h"
#include "freertos/FreeRTOS.h"
#include "welcome.html.h"

void httpd_task(void *pvParameters)
{
    struct netconn *client = NULL;
    struct netconn *nc = netconn_new(NETCONN_TCP);
    if (nc == NULL) {
        printf("Failed to allocate socket.\n");
        vTaskDelete(NULL);
    }
    netconn_bind(nc, IP_ADDR_ANY, 80);
    netconn_listen(nc);
    const char *webpage = acWelcomeHtml;

    while (1) {
        err_t err = netconn_accept(nc, &client);
        if (err == ERR_OK) {
            struct netbuf *nb;
            if ((err = netconn_recv(client, &nb)) == ERR_OK) {
                void *data;
                u16_t len;
                netbuf_data(nb, &data, &len);
                printf("Data received: \n%s\n", (char*)data);

                /* check for a GET request */
                if (!strncmp(data, "GET ", 4)) {
                    char uri[16];
                    const int max_uri_len = 16;
                    char *sp1, *sp2;

                    /* extract URI */
                    sp1 = (void*)((uint32)(data) + 4);
                    sp2 = memchr(sp1, ' ', max_uri_len);
                    if (sp2 != NULL)
                    {
						int len = sp2 - sp1;
						memcpy(uri, sp1, len);
						uri[len] = '\0';

						printf("uri: %s\n", uri);
						printf("snprintf ok\n");
                    }
                }
				netconn_write(client, webpage, strlen(webpage), 0);
            }
            netbuf_delete(nb);
        }
        printf("Closing connection\n");
        netconn_close(client);
        netconn_delete(client);
	}
}
