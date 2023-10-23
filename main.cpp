// ini
#include <iostream>
#include <sstream>
#include "jj_ini.h"
// ini

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <time.h>

// system
#include <sys/resource.h>
#include <sys/time.h>
#include "jj_system.h"
#include <sys/statvfs.h>

// curl
#include <curl/curl.h>

// disk
#include <stdlib.h>
#include <sys/statvfs.h>
typedef struct DiskSpaceInfo
{
  unsigned long long total_space;
  unsigned long long free_space;
  unsigned long long used_space;
} DiskSpaceInfo;
// disk

// Int System
int connection = 0;
int rpc_main_con = 0;
int rpc_test_con = 0;
int sa_sys_cpu = 0;
int sa_sys_rss = 0;
int sa_sys_vsz = 0;
int sa_sys_ram_used = 0;
int sa_sys_ram_total = 0;

// Mainnet

// Testnet

// Int System

// Char
char *c_dgbn_api;
char *cm_enabled;
char *cm_rpc_ip;
char *cm_rpc_port;
char *ct_enabled;
char *ct_rpc_ip;
char *ct_rpc_port;
char api_url[] = "https://digibytenode.com/api";

volatile int run = 1;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
  size_t total_size = size * nmemb;
  output->append(static_cast<char *>(contents), total_size);
  return total_size;
}

std::string OpenURLWithVariable(const std::string &url)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();

  if (curl)
  {

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    std::string response_data;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }
    else
    {
      curl_easy_cleanup(curl);
      return response_data;
    }
    curl_easy_cleanup(curl);
  }

  return "";
}

// disk
double bytesToGigabytes(unsigned long long bytes)
{
  return (double)bytes / 1073741824.0;
}

DiskSpaceInfo getDiskSpaceInfo(const char *path)
{
  DiskSpaceInfo data;
  struct statvfs stats;

  if (statvfs(path, &stats) == 0)
  {
    unsigned long long block_size = stats.f_frsize;
    data.free_space = block_size * stats.f_bfree;
    data.total_space = block_size * stats.f_blocks;
    data.used_space = data.total_space - data.free_space;
  }
  else
  {
    // Handle error
    data.free_space = 0;
    data.total_space = 0;
    data.used_space = 0;
  }

  return data;
}
// disk

void *menu(void *arg)
{
  // menu thread
  while (run)
  {
    char c[256];
    time_t tim = time(NULL);
    struct tm *tmp = localtime(&tim);
    strftime(c, 256, "[%y-%m-%d %H:%M:%S]", tmp);
    printf("\x1b[1;1H");
    printf("\x1b[2J");

    printf("%-15s %-15s\n", "DIGIBYTENODE.COM", "1.0.3.2");
    printf("%-15s\n", "----------------------------------");
    if (connection == 0)
    {
      printf("%-15s %-15s\n", "STATUS:", "DISCONNECTED");
    }
    else
    {
      printf("%-15s %-15s\n", "STATUS:", "CONNECTED");
    }
    printf("%-15s %s\n", "TIME:", c);
    printf("%-15s %s\n", "API KEY:", c_dgbn_api);
    printf("%-15s\n", "----------------------------------");
    if (strcmp(cm_enabled, "true") == 0)
    {
      printf("%-15s\n", "MAINNET RPC");
      if (rpc_main_con == 0)
      {
        printf("%-15s %-15s\n", "STATUS:", "DISCONNECTED");
      }
      else
      {
        printf("%-15s %-15s\n", "STATUS:", "CONNECTED");
      }
      printf("%-15s %s\n", "IP:", cm_rpc_ip);
      printf("%-15s %s\n", "PORT:", cm_rpc_port);
      printf("%-15s\n", "----------------------------------");
    }
    if (strcmp(ct_enabled, "true") == 0)
    {
      printf("%-15s\n", "TESTNET RPC");
      if (rpc_test_con == 0)
      {
        printf("%-15s %-15s\n", "STATUS:", "DISCONNECTED");
      }
      else
      {
        printf("%-15s %-15s\n", "STATUS:", "CONNECTED");
      }
      printf("%-15s %s\n", "IP:", ct_rpc_ip);
      printf("%-15s %s\n", "PORT:", ct_rpc_port);
      printf("%-15s\n", "----------------------------------");
    }
    sleep(1);
  }
  pthread_exit(NULL);
}

void *rpc(void *arg)
{
  // RPC thread
  while (run)
  {

    sleep(5);
  }
  pthread_exit(NULL);
}

void *api_hardware(void *arg)
{
  while (run)
  {
    // Hardware api thread
    char url[150];

    // app ram
    sa_sys_rss = getmemrss();
    // cpu use
    sa_sys_cpu = getcpu();
    // ram use
    sa_sys_ram_used = getmemramuse();
    // ram total
    sa_sys_ram_total = getmemramtotal();

    const char *path = "/";
    DiskSpaceInfo data = getDiskSpaceInfo(path);
    double used_space = bytesToGigabytes(data.used_space);
    double total_space = bytesToGigabytes(data.total_space);
    // double free_space = bytesToGigabytes(data.free_space);

    sprintf(url, "%s/hardware/?key=%s&cpu=%d&mem=%d&memt=%d&lds=%.2f&ldst=%.2f", api_url, c_dgbn_api, sa_sys_cpu, sa_sys_ram_used, sa_sys_ram_total, used_space, total_space);
    std::string response = OpenURLWithVariable(url);

    sleep(5);
  }
  pthread_exit(NULL);
}

void *api_mainnet(void *arg)
{
  while (run)
  {
    // Mainnet api thread

    sleep(10);
  }
  pthread_exit(NULL);
}

void *api_testnet(void *arg)
{
  while (run)
  {
    // Testnet api thread

    sleep(10);
  }
  pthread_exit(NULL);
}

void stop_thread(int signo)
{
  run = 0;
}

int main()
{
  JJINI reader("dgbn.ini");
  // api key
  std::string dgbn_api = reader.Get("hardware", "api", "");
  c_dgbn_api = strcpy(new char[dgbn_api.length() + 1], dgbn_api.c_str());
  // mainnet enabled
  std::string m_enabled = reader.Get("mainnet", "enabled", "");
  cm_enabled = strcpy(new char[m_enabled.length() + 1], m_enabled.c_str());
  // mainnet ip
  std::string m_rpc_ip = reader.Get("mainnet", "rpc_ip", "");
  cm_rpc_ip = strcpy(new char[m_rpc_ip.length() + 1], m_rpc_ip.c_str());
  // mainnet port
  std::string m_rpc_port = reader.Get("mainnet", "rpc_port", "");
  cm_rpc_port = strcpy(new char[m_rpc_port.length() + 1], m_rpc_port.c_str());
  // testnet enabled
  std::string t_enabled = reader.Get("testnet", "enabled", "");
  ct_enabled = strcpy(new char[t_enabled.length() + 1], t_enabled.c_str());
  // testnet ip
  std::string t_rpc_ip = reader.Get("testnet", "rpc_ip", "");
  ct_rpc_ip = strcpy(new char[t_rpc_ip.length() + 1], t_rpc_ip.c_str());
  // testnet port
  std::string t_rpc_port = reader.Get("testnet", "rpc_port", "");
  ct_rpc_port = strcpy(new char[t_rpc_port.length() + 1], t_rpc_port.c_str());

  if (strcmp(c_dgbn_api, "") == 0)
  {
    printf("%-15s\n", "Error loading configuration file: dgbn.ini");
    exit(0);
  }

  pthread_t ptid0;
  pthread_t ptid1;
  pthread_t ptid2;
  pthread_t ptid3;
  pthread_t ptid4;
  pthread_create(&ptid0, NULL, menu, NULL);
  pthread_create(&ptid1, NULL, rpc, NULL);
  pthread_create(&ptid2, NULL, api_hardware, NULL);
  if (strcmp(cm_enabled, "true") == 0)
  {
    pthread_create(&ptid3, NULL, api_mainnet, NULL);
  }
  if (strcmp(ct_enabled, "true") == 0)
  {
    pthread_create(&ptid4, NULL, api_testnet, NULL);
  }
  pthread_join(ptid0, NULL);
  return 0;
}
