#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct arg{
	int numThreads;
	int port;
	char * ip;
	int index;
	FILE *fp;
};

pthread_mutex_t lock;


void *run(void * arg){


    int con_fd = 0;
    int ret = 0;
    struct sockaddr_in serv_addr;

//setting local variables from struct

    struct arg * pt = (struct arg *)arg;
    int numThreads = pt->numThreads;
    int port = pt->port;
    char * ip = pt->ip;
    int index = pt->index;
    FILE *fp = pt->fp;
	printf("%d\n", index);

    
int loc = index;

    int bytesReceived = 1;
    char recvBuff[1024];
    char sendIn[5];


//continue loop until no longer getting anything from server

    while(bytesReceived!=0){
//pthread_mutex_lock(&lock);

    con_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (con_fd == -1) {
	perror("Socket Error\n");
	return NULL;
    }

    
    memset(&serv_addr, 0, sizeof(struct sockaddr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);;
    

  
    ret = connect(con_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

    if (ret < 0) {
	perror("Connect error\n");
	return NULL;
    }
//initialize receiving buffer

   	memset(recvBuff, '0', sizeof(recvBuff));
  pthread_mutex_lock(&lock);      
//send to server
	snprintf(sendIn, 128, "%d", loc);
	send(con_fd, sendIn, strlen(sendIn), 0);

//Receive data in chunks of 1024 bytes 
	bytesReceived = recv(con_fd, recvBuff, 1024, 0);

//        pthread_mutex_lock(&lock);
	
//if something was received write in correct spot in file
if(bytesReceived !=0){
	fseek(fp, loc * 1024, SEEK_SET);
      	fwrite(recvBuff, 1,bytesReceived,fp);
}
	loc = loc + numThreads;

        pthread_mutex_unlock(&lock);
   	 
	close(con_fd);

   } 
   


    return NULL;
}

int main(int argc, char ** argv) {

    int numThreads = (argc - 1)/2;

    FILE *fp;
    fp = fopen("output.txt", "a");
    if(NULL == fp)
    {
     	printf("Error opening file");
        return 0;
    }


    pthread_t threads[numThreads];

    int cnt = 0;
    int argScn = 1;
    
    printf("numthreads: %d\n", numThreads);

//make struct to send to run with parameters
    for(int i = 0; i<numThreads; i++){

        struct arg *temp;
	temp = malloc(sizeof(struct arg));
        temp->numThreads = numThreads;
        temp->ip = argv[argScn];
        argScn++;
        temp->port = atoi(argv[argScn]);
        argScn++;
	printf("sending port %d\n", temp->port);
        temp->index = cnt;
        cnt++;
	temp->fp = fp;
	printf("creating thread %d\n", i);
        pthread_create(&threads[i], NULL, run, temp);
	printf("thread created %d\n", i);
    }

    for(int i = 0; i< numThreads; i++){
	
	pthread_join(threads[i], NULL);
    }

}
