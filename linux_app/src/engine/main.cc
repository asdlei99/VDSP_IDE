#include "vsp_input.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define LEN 10*3*160
using namespace std;

//#define DEBUG
#define PORT 6666

int main(int argc, char* argv[])
{
	char* targetip = "10.99.1.3";
	int sock_fd;
	struct sockaddr_in server_addr;

	FILE* out_file;

	int16_t* buf = (int16_t*)malloc(LEN*2);

	VspInput m_vspInput;

	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("create socket failed\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, targetip, &server_addr.sin_addr)<=0){
		printf("\n Invalid address/Address not support\n");
		exit(1);
	}
#ifdef DEBUG	
	printf("[CLIENT] server add: %s, port: %u\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
#endif

	if(connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		printf("connect to server failed\n");	
		exit(1);
		
	}

#ifdef DEBUG	
	printf("[CLIENT] connected to server %s\n", inet_ntoa(server_addr.sin_addr));

#endif

	out_file = fopen("pcm_out.pcm", "wb");

#ifdef DEBUG	
	printf("vspInput_initialized\n");
#endif


	int i= 300;
	while(i-- >= 0){
		m_vspInput.VspGetData_();

//		copy(m_vspInput.rawMicChannels_[0].begin(),m_vspInput.rawMicChannels_[0].end(),buf); 

		

//		send(sock_fd, buf, LEN*2, 0);
		fwrite(&m_vspInput.rawMicChannels_[0][0], sizeof(int16_t), m_vspInput.rawMicChannels_[0].size(), out_file);

#ifdef DEBUG	
	printf("vspInput_getdata cout:%d, Raw data size:%lu\n", i, m_vspInput.rawMicChannels_[0].size());
#endif
	}

	free(targetip);
	fclose(out_file);
	return 0;
}
