#include <stdio.h>                                                                                                                                                                          
#include <stdlib.h>                                                                                                                                                                         
#include <unistd.h>                                                                                                                                                                                                
#include <sys/socket.h>                                                                                                                                                                                            
#include <arpa/inet.h>                                                                                                                                                                                             
#include <sys/types.h>                                                                                                                                                                                             
#include <string.h>                                                                                                                                                                                                
#include <pthread.h>                                                                                                                                                                                               
#include <semaphore.h>

#define BUFFER_SIZE 1024 * 1024                                                                                                                                                                                  
#define MAX_CLIENTS 5                                                                                                                                                                                                                                                                                                                                                                                         
int client_sockets[MAX_CLIENTS] = {-1}; 
sem_t mutex;

char buffer[BUFFER_SIZE + 200] = {0};

void error_and_exit(const char* error){                                                                                                                                                                            
        perror(error);                                                                                                                                                                                             
        exit(EXIT_FAILURE);                                                                                                                                                                                        
}                                                                                                                                                                                                                  
                                                                                                                                                                                                                   
typedef struct Client{                                                                                                                                                                                             
        int socket;           
		int id;
        char name[100];                                                                                                                                                                                            
        char buffer[BUFFER_SIZE];                                                                                                                                                                                  
} Client;                                                                                                                                                                                                          
                                                                                                                                                                                                                   
                                                                                                                                                                                                                   
void* routine(void* args){                                                                                                                                                                                         
        Client* client = (Client*)(args);                                                                                                                                                                          
        memset(client->name, 0, 100);                                                                                                                                                                              
        memset(client->buffer, 0, BUFFER_SIZE);                                                                                                                                                                    
        read(client->socket, client->name, 100);                                                                                                                                                                   
        printf("Client %s has joined and is using socket %d\n", client->name, client->socket);

        while(1){                                                                                                                                                                                                  
                read(client->socket, client->buffer, BUFFER_SIZE);                                                                                                                                                 
                if(strcmp(client->buffer, "exit\n") == 0){                                                                                                                                                         
                        printf("%s has left\n", client->name);                                                                                                                                                                   
                        break;                                                                                                                                                                                     
                }                                                                                                                                                                                                  
                
				sem_wait(&mutex);

				sprintf(buffer, "%s : %s", client->name, client->buffer);
				for(int ind = 0; ind < MAX_CLIENTS; ind++){
					if(ind != client->id && client_sockets[ind] != -1){
						send(client_sockets[ind], buffer, strlen(buffer), 0);
					}
				}
                
				printf("%s : %s\n", client->name, client->buffer);
				memset(buffer, 0, BUFFER_SIZE + 200);
				
				sem_post(&mutex);

                                                                                                                                                                                                                   
                memset(client->buffer, 0, BUFFER_SIZE);                                                                                                                                                            
                                                                                                                                                                                                                   
//              fgets(client->buffer, BUFFER_SIZE, stdin);                                                                                                                                                         
//              send(client->socket, client->buffer, strlen(client->buffer), 0);                                                                                                                                   
        }                                                                                                                                                                                                          
        close(client->socket);
		client_sockets[client->id] = -1;
		free(client);                                                                                                                                                                                                                 
}                                                                                                                                                                                                                  
                                                                                                                                                                                                                   
                                                                                                                                                                                                                   
int main(int argc, char* argv[]){                                                                                                                                                                                  
                                                                                                                                                                                                                   
        const int PORT = atoi(argv[1] + 1);                                                                                                                                                                        
        const char* name = argv[2];                                                                                                                                                                                
                                                                                                                                                                                                                   
        printf("Serving from PORT %d with servername %s...\n", PORT, name);                                                                                                                                        
                                                                                                                                                                                                                   
        int server_sockfd;                                                                                                                                                                                         
        struct sockaddr_in address;                                                                                                                                                                                
        int address_len = sizeof(address);                                                                                                                                                                         
                                                                                                                                                                                                                   
        if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){                                                                                                                                                 
                error_and_exit("ERROR : Could not create socket\n");                                                                                                                                               
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        address.sin_family = AF_INET;                                                                                                                                                                              
        address.sin_port = htons(PORT);                                                                                                                                                                            
        address.sin_addr.s_addr = INADDR_ANY;                                                                                                                                                                      
                                                                                                                                                                                                                   
        if(bind(server_sockfd, (struct sockaddr*)(&address), (sizeof(address))) < 0){                                                                                                                              
                error_and_exit("ERROR : Bind error\n");                                                                                                                                                            
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        if(listen(server_sockfd, 5) < 0){
                error_and_exit("ERROR : Listen error\n");                                                                                                                                                          
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        pthread_t client_threads[MAX_CLIENTS];                                                                                                                                                                     
        int clients = 0;                                                                                                                                                                                           
        int new_socket;
		sem_init(&mutex, 0, 1);
                                                                                                                                                                                                                   
        while(clients < MAX_CLIENTS){                                                                                                                                                                              
                if((new_socket = accept(server_sockfd, (struct sockaddr*)(&address), (socklen_t*)(&address_len))) < 0){                                                                                            
                        error_and_exit("ERROR : Accept Error\n");                                                                                                                                                  
                }                                                                                                                                                                                                  
                                                                                                                                                                                                                   
                                                                                                                                                                                                                   
                Client* client = (Client*)(malloc(sizeof(Client)));                                                                                                                                                
                client->socket = new_socket;          
				int client_id = clients++;
                
				client_sockets[client_id] = new_socket;                                                                                                                                                              
               	client->id = client_id;
                
				if(pthread_create(&client_threads[client_id], NULL, &routine, (void*)(client)) != 0){                                                                                                              
                        error_and_exit("ERROR : Could not serve client\n");                                                                                                                                        
                }                                                                                                                                                                                                  
                pthread_detach(client_threads[client_id]);

                //int pid = fork();                                                                                                                                                                                
                                                                                                                                                                                                                   
                //if(pid == 0){                                                                                                                                                                                    
                //}                                                                                                                                                                                                

                                                                                                                                                                                                                   
        }                                                                                                                                                                                                          
        close(server_sockfd);                                                                                                                                                                                      
        sem_destroy(&mutex);

        return 0;                                                                                                                                                                                                  
}                                             
