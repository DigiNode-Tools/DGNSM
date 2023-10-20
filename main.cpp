//ini
#include <iostream>
#include <sstream>
#include "jj_INIReader.h"
//ini

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//system
#include <sys/resource.h>
#include <sys/time.h>
#include "jj_system.h"


int sa_sys_cpu = 0;
int sa_sys_rss = 0;
int sa_sys_vsz = 0;
int sa_sys_ram_used = 0;
int sa_sys_ram_total = 0;



volatile int run = 1;
void *rpc(void *arg) {
  // RPC thread
  int i = 0;
  while (run){
    char c[256];
    time_t tim = time(NULL);
    struct tm *tmp = localtime(&tim);
    strftime(c, 256, "[%y-%m-%d %H:%M:%S]", tmp);

    //app ram
    sa_sys_rss = getmemrss();
    //cpu use
    sa_sys_cpu = getcpu();
    //ram use
    sa_sys_ram_used = getmemramuse();
    //ram total
    sa_sys_ram_total = getmemramtotal();

    printf("\x1b[2K");
    printf("\x1b[u");
    printf("\x1b[s"); 
    printf("RPC CONNECTION %s", c);
    printf(" CPU %i%% - RAM %i/%i MB\n", sa_sys_cpu, sa_sys_ram_used, sa_sys_ram_total);
    
    


    sleep(5);
  }
  pthread_exit(NULL);
}

void *api_hardware(void *arg) {
  while (run){
    // Hardware api thread

    

    sleep(5);
    }
    pthread_exit(NULL);
}

void *api_mainnet(void *arg) {
  while (run){
    // Mainnet api thread
    
    

    sleep(10);
    }
    pthread_exit(NULL);
}


void *api_testnet(void *arg) {
  while (run){
    // Testnet api thread



    sleep(10);
    }
    pthread_exit(NULL);
}


void stop_thread(int signo) {
    run = 0;
}

int main() {

    printf("DIGIBYTENODE.COM CONNECT 1.0.2.1\n");
    printf("\x1b[s");

    pthread_t ptid1;
    pthread_t ptid2;
    pthread_t ptid3;
    pthread_t ptid4;


    pthread_create(&ptid1, NULL, rpc, NULL);
    pthread_create(&ptid2, NULL, api_hardware, NULL);
    pthread_create(&ptid3, NULL, api_mainnet, NULL);
    pthread_create(&ptid4, NULL, api_testnet, NULL);


    pthread_join(ptid1, NULL);


    return 0;
}