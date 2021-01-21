/*
 * Copyright (C) 2009 - 2019 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <string.h>

#include "OV7670.h"
#include "chacha20.h"

#include <xil_types.h>

#include "xtime_l.h"

#include "lwip/err.h"
#include "lwip/tcp.h"
#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

#include "auto_contrast_state.h"

int transfer_data() {
	return 0;
}

void print_app_header()
{
#if (LWIP_IPV6==0)
	xil_printf("\n\r\n\r-----lwIP TCP echo server ------\n\r");
#else
	xil_printf("\n\r\n\r-----lwIPv6 TCP echo server ------\n\r");
#endif
	xil_printf("TCP packets sent to port 6001 will be echoed back\n\r");
}

unsigned char *picBuffer[1];
s32 picSize = 640 * 480 * 2;

int sending = 0;
int sent = 0;

u8 key[32] = { 253, 232, 117, 230, 4, 234, 79, 143, 10, 13, 82, 236, 43, 23, 155, 240, 76, 149, 190, 222, 86, 180, 235, 136, 141, 79, 82, 232, 152, 100, 154, 88 };
u8 nonce[12] = { 219, 217, 216, 81, 110, 153, 222, 222, 85, 76, 93, 118 };

err_t send_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{

	int sndbuf;

	if(sending)
	{
		sndbuf = tcp_sndbuf(tpcb);
		if((sent + sndbuf) >= picSize)
		{
			tcp_arg(tpcb, NULL);
			tcp_write(tpcb, *picBuffer + sent, picSize - sent, TCP_WRITE_FLAG_COPY);

			sending = 0;
			free(*picBuffer);
			xil_printf("Picture sent\n");
		}
		else
		{
			tcp_arg(tpcb, NULL);
			tcp_write(tpcb, *picBuffer + sent, sndbuf, TCP_WRITE_FLAG_COPY + TCP_WRITE_FLAG_MORE);
			sent += sndbuf;
		}
	}

	return ERR_OK;
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	if(((unsigned char *)p->payload)[0] == 0x41)
	{

		Camera_determineImageOrientation(((u8 *)p->payload)[3] & 0x01, ((u8 *)p->payload)[3] & 0x02);

		if(((unsigned char *)p->payload)[1])
		{
			Camera_TestBarMode();
		}
		else
		{
			Camera_ImageMode();
		}

		unsigned char acs_code = ((unsigned char *)p->payload)[2];
		auto_contrast_state acs = NO_AUTO_CONTRAST;
		if(acs_code == 1)
		{
			 acs = SOFT_AUTO_CONTRAST;
		}
		else if(acs_code == 2)
		{
			acs = HARD_AUTO_CONTRAST;
		}

		picSize = Camera_getJPEG(picBuffer, acs, ((unsigned char *)p->payload)[4]);

		chacha20_use(*picBuffer, picSize, key, nonce, 0);

		xil_printf("Sending picture, size %d...\n", picSize);

		while(tcp_sndbuf(tpcb) < 7);
		tcp_arg(tpcb, NULL);
		tcp_write(tpcb, &picSize, sizeof(picSize), TCP_WRITE_FLAG_COPY);

		sending = 1;
		sent = 0;
	}

	/* free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}



err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);
	tcp_sent(newpcb, send_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}


int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ANY_TYPE, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}
