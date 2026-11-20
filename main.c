#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void ReadLineFromNetwork(int sd, char *buf, int size);

int main(int argc, const char * argv[])
{
	char buf[256];
	char line[384];
	int sd;
	size_t n;
	struct sockaddr_in sa;
	struct hostent *ph;
	int quit = 0;
	
	/* Open */
	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sd < 0)
	{
		printf("Error creating socket\n");
		exit(1);
	}
	printf("DEBUG: SD =%d\n", sd);
	
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(25565);
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	connect(sd, (struct sockaddr *)&sa, sizeof(sa));
	
	/* Access */

    ReadLineFromNetwork(sd, buf , 256);
    printf("DEBUG: [%s]\n", buf);

	/* Close */
	close(sd);
	
    return 0;
}

void ReadLineFromNetwork(int sd, char *buf, int size)
{
	size_t n;
	char c;
	int i = 0;
	
    do
    {
        n = recv(sd, &c, 1, 0);
        buf[i] = c;
        if (n > 0 && i > 0 && buf[i] == '\n' && buf[i-1] == '\r')
        {
            buf[i-1] = 0;
            break;
        }
        i++;
    } while (n > 0 && i < (size - 1));
    buf[i] = 0;
}