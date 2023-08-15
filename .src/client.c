#include <stdio.h>                                                                                                                                                                                                 
#include <stdlib.h>                                                                                                                                                                                                
#include <unistd.h>                                                                                                                                                                                                
#include <fcntl.h>
#include <sys/socket.h>                                                                                                                                                                                            
#include <arpa/inet.h>                                                                                                                                                                                             
#include <sys/types.h>                                                                                                                                                                                             
#include <string.h>                                                                                                                                                                                                
#include <pthread.h>                                                                                                                                                                                               
                                                                                                                                                                                                                   
#define BUFFER_SIZE 1024 * 1024                                                                                                                                                                                    
                   
enum Status { QUIT, ACTIVE};

enum Status client_status = ACTIVE;

char read_buffer[BUFFER_SIZE] = {0};  


                                                                                                                                                                                                                   
void error_and_exit(const char* error){                                                                                                                                                                            
        perror(error);                                                                                                                                                                                             
        exit(EXIT_FAILURE);                                                                                                                                                                                        
}                                                                                                                                                                                                                  
                                                                                                                                                                                                                   
void* routine(void* args){                                                                                                                                                                                         
        int client_sockfd = *(int*)(args);                                                                                                                                                                         
        while(client_status != QUIT){                                                                                                                                                                                                  
                read(client_sockfd, read_buffer, BUFFER_SIZE);                                                                                                                                                     
                printf("%s", read_buffer);
				memset(read_buffer, 0, BUFFER_SIZE);
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
}              

void remindAboutNotifications(){
		int read_fd = open("./notify.js", O_RDONLY);
		if(read_fd < 0) return;

		printf("Connected!!\nIf you wish to ");
		printf("\033[0;31mreceive notifications (works only on the browser)  on incoming messages");
		printf("\033[0;37m(OPTIONAL) follow below steps:\n");
		printf("1. Open the devtools with Ctrl + Shift + i\n");
		printf("2. Drop the following snippet of code into the console and run it and then immediately close the devtools.\n\n");
		
		char buf;
		while(read(read_fd, &buf, 1)){
			printf("\033[0;32m%c", buf);
		}

		close(read_fd);
		printf("\033[0;37m\n\n");

}
                                                                                                                                                                                                                   
int main(int argc, char* argv[]){                                                                                                                                                                                  
                                                                                                                                                                                                                   
        char buffer[BUFFER_SIZE] = {0};                                                                                                                                                                            
        const int PORT = atoi(argv[1] + 1);                                                                                                                                                                        
        const char* name = argv[2];                                                                                                                                                                                
                                                                                                                                                                                                                   
        printf("Connecting as client to PORT %d with username %s...\n", PORT, name);
                                                                                                                                                                                                                   
        int client_sockfd;                                                                                                                                                                                         
        struct sockaddr_in server_address;                                                                                                                                                                         
                                                                                                                                                                                                                   
        if((client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){                                                                                                                                                 
                error_and_exit("ERROR : Could not create socket\n");                                                                                                                                               
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        server_address.sin_family = AF_INET;                                                                                                                                                                       
        server_address.sin_port = htons(PORT);                                                                                                                                                                     
                                                                                                                                                                                                                   
        if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0){                                                                                                                                        
                error_and_exit("ERROR : Invalid IP format\n");                                                                                                                                                     
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        if(connect(client_sockfd, (struct sockaddr*)(&server_address), sizeof(server_address)) < 0){                                                                                                               
                error_and_exit("ERROR : Could not connect\n");                                                                                                                                                     
        }                                                                           

		remindAboutNotifications();
                                                                                                                                                                                                                   
                                                                                                                                                                                                                   
        sprintf(buffer, "%s", name);                                                                                                                                                                               
        send(client_sockfd, buffer, strlen(buffer), 0);                                                                                                                                                            
                                                                                                                                                                                                                   
        pthread_t client_thread;                                                                                                                                                                                   
                                                                                                                                                                                                                   
        pthread_create(&client_thread, NULL, &routine, (void*)(&client_sockfd));                                                                                                                                   
                                                                                                                                                                                                                   
        while(1){           
                fgets(buffer, BUFFER_SIZE, stdin);                                                                                                                                                                
                send(client_sockfd, buffer, strlen(buffer), 0);                                                                                                                                                    
                if(strcmp(buffer, "exit\n") == 0) break;                                                                                                                                                           
                                                                                                                                                                                                                   
                memset(buffer, 0, BUFFER_SIZE);                                                                                                                                                                    
                                                                                                                                                                                                                   
        }                                                                                                                                                                                                          

		client_status = QUIT;
        pthread_join(client_thread, NULL);                                                                                                                                                                         
        close(client_sockfd);                                                                                                                                                                                      
                                                                                                                                                                                                                   
        return 0;                                                                                                                                                                                                  
}                                            
