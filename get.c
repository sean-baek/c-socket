#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <unistd.h>

//debug define

#define DEBUG_LEVEL_	3
#ifdef  DEBUG_LEVEL_
#define dp(n, fmt, args...)	if (DEBUG_LEVEL_ <= n) fprintf(stderr, "%s:%d,"fmt, __FILE__, __LINE__, ## args)
#define dp0(n, fmt)		if (DEBUG_LEVEL_ <= n) fprintf(stderr, "%s:%d,"fmt, __FILE__, __LINE__)
#define _dp(n, fmt, args...)	if (DEBUG_LEVEL_ <= n) fprintf(stderr, " "fmt, ## args)
#else	/* DEBUG_LEVEL_ */
#define dp(n, fmt, args...)
#define dp0(n, fmt)
#define _dp(n, fmt, args...)
#endif	/* DEBUG_LEVEL_ */

/*IP 정보 가져오기*/
int getIPAddress(char *ip_addr)
{

	int sock;
	struct ifreq ifr;
	struct sockaddr_in *sin;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
	{
		dp(4, "socket");
		return 0;
	}

	strcpy(ifr.ifr_name, "eth0");
	if (ioctl(sock, SIOCGIFADDR, &ifr)< 0)    
	{
		dp(4, "ioctl() - get ip");
		close(sock);
		return 0;
	}
	sin = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy(ip_addr, inet_ntoa(sin->sin_addr));

	close(sock);

	return 1;

}



//Subnetmask 정보 가져오기

int getSubnetMask(char *sub_addr)

{
	int sock;
	struct ifreq ifr;
	struct sockaddr_in *sin;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
	{
		dp(4, "socket");
		return 0;
	}

	strcpy(ifr.ifr_name, "eth0");


	if (ioctl(sock, SIOCGIFNETMASK, &ifr)< 0)    
	{
		dp(4, "ioctl() - get subnet");
		close(sock);
		return 0;
	}

	sin = (struct sockaddr_in*)&ifr.ifr_addr;
	strcpy(sub_addr, inet_ntoa(sin->sin_addr));

	close(sock);
	return 1;
}



//Broadcast 정보 가져오기

int getBroadcastAddress(char *br_addr)

{
	int sock;
	struct ifreq ifr;
	struct sockaddr_in *sin;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) 
	{
		dp(4, "socket");
	
		return 0;
	}

	strcpy(ifr.ifr_name, "eth0");

	if (ioctl(sock, SIOCGIFBRDADDR, &ifr)< 0)    
	{
		dp(4, "ioctl() - get subnet");

		close(sock);

		return 0;
	}

	

	sin = (struct sockaddr_in*)&ifr.ifr_addr;

	strcpy(br_addr, inet_ntoa(sin->sin_addr));

	close(sock);

	return 1;

}



//Mac address 정보 가져오기

int getMacAddress(char *mac)

{

	int sock;

	struct ifreq ifr;

	char mac_adr[18] = {0,};

	

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) 

	{

		dp(4, "socket");

		return 0;

	}



	strcpy(ifr.ifr_name, "eth0");

	if (ioctl(sock, SIOCGIFHWADDR, &ifr)< 0)    

	{

		dp(4, "ioctl() - get mac");

		close(sock);

		return 0;

	}

	

	//convert format ex) 00:00:00:00:00:00

	convrt_mac( ether_ntoa((struct ether_addr *)(ifr.ifr_hwaddr.sa_data)), mac_adr, sizeof(mac_adr) -1 );

	  

	strcpy(mac, mac_adr);

	

	close(sock);

	return 1;

}



//보기 편한 포맷으로 Mac address를 변경해준다. - 함수 출처 : http://hkpco.kr/code/netinfo.c

void convrt_mac(const char *data, char *cvrt_str, int sz)

{

     char buf[128] = {0,};

     char t_buf[8];

     char *stp = strtok( (char *)data , ":" );

     int temp=0;



     do

     {

          memset( t_buf, 0, sizeof(t_buf) );

          sscanf( stp, "%x", &temp );

          snprintf( t_buf, sizeof(t_buf)-1, "%02X", temp );

          strncat( buf, t_buf, sizeof(buf)-1 );

          strncat( buf, ":", sizeof(buf)-1 );

     } while( (stp = strtok( NULL , ":" )) != NULL );



     buf[strlen(buf) -1] = '\0';

     strncpy( cvrt_str, buf, sz );

}





 /*NIC 정보 가져오기

NIC이 'eth0'으로 되어있지 않은경우에 사용했음.
*/

int getNIC() //eth0와 같은 NIC을 가져온다.

{

	int sock;

	struct ifconf ifconf;

	struct ifreq ifr[50];

	int ifs;

	int i;



	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0) 

	{

		dp(4, "socket");

		return 0;

	}



	ifconf.ifc_buf = (char *) ifr;

	ifconf.ifc_len = sizeof ifr;



	if (ioctl(sock, SIOCGIFCONF, &ifconf) == -1) 

	{

		dp(4, "ioctl");

		return 0;

	}



	ifs = ifconf.ifc_len / sizeof(ifr[0]);

	//printf("interfaces = %d:\n", ifs);

	for (i = 0; i < ifs; i++) 

	{

		if(strcmp(ifr[i].ifr_name, "lo") != 0) //'lo'를 제외한 나머지 NIC을 가져온다.

		{

			strcpy(getNIC, ifr[i].ifr_name);

			break;

		}

	}



	close(sock);

	return 1;

}
