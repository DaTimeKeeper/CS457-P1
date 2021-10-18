/*******************************************
* Group Name  : Header
* Member1 Name: Ian Taylor
* Member1 SIS ID: 831856215
* Member1 Login ID: it
* Member2 Name: Zach Walsh
* Member2 SIS ID: 831955300
* Member2 Login ID: zachwals
********************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <iostream>

void *get_in_addr(struct sockaddr *sa);
char *get_host_addr();
int setup_sock_fd(struct addrinfo *server_info);

using namespace std;

bool isNotIP(const char* ip){
  for(size_t i=0;i< strlen(ip);i++){
    if(isdigit(ip[i])||ip[i]=='.'){
    }else{
      return true;
    }
  }
  return false;
}
int main(int argc, char* argv[]){
  bool server=false;
  int opt;
  if(argc==1){
    server=true;
  }else if(argc ==2){
    cout<<"To use client: -p {portNumber} -s {serverIP}\n"<<"To use Server: no args\n"<<"Use server first\n";
    return 0;
  }else if(argc != 5){
    printf("Bad number of Argumnets\n");
    return 1;
  }

if(server){//Server code
  printf("Welcome to Chat!\n");
  //Variable declaration
  char host_port[] = "3790";
  int sock_fd, client_fd;
  struct addrinfo hints, *server_info;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  int rv;
  //Free up hints and then set it up for grabbing the server info.
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  //Grab server info
  if ((rv = getaddrinfo(NULL, host_port, &hints, &server_info)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }
  //Set file descriptor up w/ binding socket sockopt
  sock_fd = setup_sock_fd(server_info);
  if (sock_fd == 1)  {
    fprintf(stderr, "server: failed to bind\n");
    return 1;
  }
  //Free up this structs memory
  freeaddrinfo(server_info);
  //Listen with the sock file descriptor
  if (listen(sock_fd, 1) == -1) {
    perror("listen");
    return 1;
  }
  //Grab the host IP and print it and the port.
	char* host_addr = get_host_addr();	
	printf("Waiting for a connection on %s port %s\n", host_addr, host_port);
  //Accept client connection with client_fd
  sin_size = sizeof their_addr;
  client_fd = accept(sock_fd, (struct sockaddr *)&their_addr, &sin_size);
  if (client_fd == -1) {
      perror("accept");
  }
  printf("Found a friend! You receive first.\n");




  //Loop and variables to receive and send communications
  int received, sent = 1;
  char inc_message[4096];
  bool rec=true;
  while(true) {  
    //Block until message is recv'd
    while(rec) {
      memset(&inc_message, 0, sizeof inc_message);
      received = recv(client_fd, inc_message, 4096, 0);
      if(received > 0) {
        cout<<"Friend: "<<string(inc_message,0,received)<<endl;
        rec=false;
      }
    }
    //Block for server input and then send message
    printf("You: ");
    string buffer;
    getline(cin,buffer);
    if(buffer.length()>140){
      printf("Error: Input too long.\n");
    }else{
      sent = send(client_fd, buffer.c_str(), buffer.size()+1, 0);
      if(sent == 0) {
        printf("Server: message sent is too small.\n");
      }else{
        rec=true;
      }
    }
  }
  
  close(client_fd);
}

else{//Client code
const char* server_addr;
const char* port;
  while((opt = getopt(argc,argv,"p:s:"))!= -1){
    switch (opt) {
      case 'p':
        //Set port
        port=optarg;
        char* t;
        strtol(port,&t,10);
        if(*t){
          printf("Error: Port is not a number\n");
          return 1;
        }
        break;
      case 's':
        //set Server IP
        server_addr=optarg;
        if(isNotIP(server_addr)){
          printf("Error: Server IP is not an IP\n");
          return 1;
        }
        break;
    }
  }
  struct addrinfo hints, *res;
  int sockfd;
  printf("Connecting to server... ");
  memset(&hints, 0, sizeof hints);
  hints.ai_family=AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(server_addr, port, &hints, &res);
  if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol))){
  }else{
    //TODO unable to connect to socket
  }
  if(connect(sockfd, res->ai_addr ,res->ai_addrlen)!=-1){

  }else{
    //TODO return error
  }
  printf("Connected!\n");
  printf("Connected to a friend! You send first.\n");




  while(true){
    char inc_message[4096];
    bool receiving = false;
    string buffer;
    printf("You: ");
    getline(cin,buffer);
    if(buffer.length()>140){
      printf("Error: Input too long.\n");
    }else{
      receiving=true;
      int sent=send(sockfd,buffer.c_str(), buffer.size()+1, 0);
      if(sent<0){
        printf("Client: message sent is too small.\n");
      }
      
    }
    //TODO BLOCK WAIT FOR RECIVE
    while(receiving) {
      memset(&inc_message, 0, sizeof inc_message);
      int received = recv(sockfd, inc_message, sizeof inc_message, 0);
      if(received > 0) {
        cout<<"Friend: "<<string(inc_message, 0, received)<<endl;
        receiving=false;
      }
    }
  }
}
  return 0;
}

int setup_sock_fd(struct addrinfo *server_info) {
  struct addrinfo *ptr;
  int sock_fd;
  int sock_rtn = 1;
  
  //Loop through linked list of interfaces.
  for(ptr = server_info; ptr != NULL; ptr = ptr->ai_next) {
    if ((sock_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == -1) {
      perror("Server socketing error.");
      continue;
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &sock_rtn, sizeof(int)) == -1) {
      perror("Server setsockopt error.");
      return 1;
    }

    if (bind(sock_fd, ptr->ai_addr, ptr->ai_addrlen) == -1) {
      close(sock_fd);
      perror("Server binding error.");
      continue;
    }
    break;
  }
  return sock_fd;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

char* get_host_addr() {
	char name[32];
	
	gethostname(name, sizeof name);
	struct in_addr **addr_list = (struct in_addr **)gethostbyname(name)->h_addr_list;
	return inet_ntoa(*addr_list[0]);
}
