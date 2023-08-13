#include <stdio.h>                                                                                                                                                                                                 
#include <stdlib.h>                                                                                                                                                                                                
#include <unistd.h>                                                                                                                                                                                                
#include <sys/types.h>                                                                                                                                                                                             
#include <string.h>                                                                                                                                                                                                
                                                                                                                                                                                                                   
                                                                                                                                                                                                                   
enum Role {CLIENT, SERVER, NONE};                                                                                                                                                                                                                                                                                                                                                                        
void error_and_exit(const char* error){                                                                                                                                                                            
        perror(error);                                                                                                                                                                                             
        exit(EXIT_FAILURE);                                                                                                                                                                                        
}                                                                                                                                                                                                                  
                                                                                                                                                                                                                   
int main(int argc, char* argv[]){                                                                                                                                                                                  

        if(argc == 2 && strcmp(argv[1], "-help") == 0){                                                                                                                                                            
                printf("To connect, run : ./connect -[MODE] -[PORT] [NAME]\n");                                                                                                                                    
                printf("For instance to connect as client : ./connect -client -8082 @Jack\n");                                                                                                                     
                printf("For instance to serve as server   : ./connect -server -8082 @Jack\n\n");
                printf("For more help visit : https://github.com/Sly-17/Connect\n\n");
                exit(0);                                                                                                                                                                                           
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        if(argc < 4){                                                                                                                                                                                              
                error_and_exit("Missing necessary arguments - ./connect -mode -port -name\nFor help,  run : ./connect -help\n\n");                                                                                 
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        // Validate arguments                                                                                                                                                                                      
        enum Role role = strcmp(argv[1], "-client") == 0 ? CLIENT : (strcmp(argv[1], "-server") == 0 ? SERVER : NONE);                                                                                             
        if(role == NONE) error_and_exit("ERROR : mode in ./connect -mode -port -name can only be one of `client` or `server`\n\n");                                                                                
                                                                                                                                                                                                                   
        const int PORT = atoi(argv[2] + 1);                                                                                                                                                                        

        if(role == CLIENT){                                                                                                                                                                                        
                execlp("./client", "./client", argv[2], argv[3], NULL);                                                                                                                                                  
        }else if(role == SERVER){                                                                                                                                                                                  
                execlp("./server", "./server", argv[2], argv[3], NULL);                                                                                                                                                  
        }                                                                                                                                                                                                          
                                                                                                                                                                                                                   
        return 0;                                                                                                                                                                                                  
}                                                            
