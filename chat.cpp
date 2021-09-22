/*******************************************
* Group Name  : XXXXXX

* Member1 Name: XXXXXX
* Member1 SIS ID: XXXXXX
* Member1 Login ID: XXXXXX

* Member2 Name: Zach Walsh
* Member2 SIS ID: 831955300
* Member2 Login ID: zachwals
********************************************/


#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char* argv[]){
  bool server=false;
  int opt;
  if(argc==1){
    server=true;
    printf("Server\n");
  }else if(argc ==2){
    if(strcmp(argv[1],"-h")){
      //Display help
      printf("Help\n");
      return 0;
    }else{
      printf("Bad Arg Help\n");
      return 1;
    }
  }else if(argc != 5){
    printf("Bad Arg\n");
    return 1;
  }else{
    printf("Clinet\n");
  }

if(server){//Server code

}else{//Client code
  while((opt = getopt(argc,argv,"p:s:"))!= -1){
    switch (opt) {
      case 'p':
        //Set port
        printf("set Port\n");
        break;
      case 's':
        //set Server IP
        printf("set IP\n");
        break;
    }
  }
}
  return 0;
}
