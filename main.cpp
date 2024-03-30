// DGNSM 1.5.1 (Also update this on line 131 below and in the .version file)
#include <iostream>
#include <sstream>
#include <fstream>
#include "jj_ini.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <time.h>
#include <vector>
#include <json-c/json.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "jj_system.h"
#include <sys/statvfs.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <cstdlib>
#include "stor.h"


size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
  size_t total_size = size * nmemb;
  output->append(static_cast<char *>(contents), total_size);
  return total_size;
}

std::string sendBitcoinRPCRequest(const char *url, const char *method)
{
  CURL *curl;
  CURLcode res;
  char *response_str = NULL;
  curl = curl_easy_init();
  std::string s;
  struct json_object *request = json_object_new_object();
  json_object_object_add(request, "jsonrpc", json_object_new_string("2.0"));
  json_object_object_add(request, "id", json_object_new_int(1));
  json_object_object_add(request, "method", json_object_new_string(method));
  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_to_json_string(request));
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
  res = curl_easy_perform(curl);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  json_object_put(request);
  return s;
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

    printf("%-15s %-15s\n", "DigiNode Status Monitor", "1.5.1");
    printf("%-15s\n", "----------------------------------");
    if (connection == 0)
    {
      printf("%-15s %-15s\n", "API:", "DISCONNECTED");
    }
    else
    {
      printf("%-15s %-15s\n", "API:", "CONNECTED");
    }
    printf("%-15s %s\n", "TIME:", c);
    printf("%-15s %s\n", "API KEY:", c_dgbn_api);
    printf("%-15s\n", "----------------------------------");
    if (strcmp(cm_enabled, "true") == 0)
    {
      printf("%-15s\n", "RPC: MAINNET");
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
      printf("%-15s\n", "RPC: TESTNET");
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

void *api_hardware(void *arg)
{
  while (run)
  {
    // Hardware api thread
    char url[150];
    sa_sys_rss = getmemrss();
    sa_sys_cpu = getcpu();
    sa_sys_ram_used = getmemramuse();
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
    std::string sub_str;
    char rpcUrl[200];
    sprintf(rpcUrl, "http://%s:%s@%s:%s", cm_rpc_user, cm_rpc_password, cm_rpc_ip, cm_rpc_port);
    std::string str1 = sendBitcoinRPCRequest(rpcUrl, "getmempoolinfo");
    std::string str2 = sendBitcoinRPCRequest(rpcUrl, "getnetworkinfo");
    std::string str3 = sendBitcoinRPCRequest(rpcUrl, "getblockchaininfo");
    std::string result = str1 + str2 + str3;

    std::string textToRemove = "size_";
    size_t found = result.find(textToRemove);
    while (found != std::string::npos)
    {
      result.erase(found, textToRemove.length());
      found = result.find(textToRemove);
    }

    std::string textToRemove2 = "\"result\":{";
    size_t found2 = result.find(textToRemove2);
    while (found2 != std::string::npos)
    {
      result.erase(found2, textToRemove2.length());
      found2 = result.find(textToRemove2);
    }

    char charToRemove = '{';
    result.erase(std::remove_if(result.begin(), result.end(), [charToRemove](char c)
                                { return c == charToRemove; }),
                 result.end());
    char charToRemove2 = '}';
    result.erase(std::remove_if(result.begin(), result.end(), [charToRemove2](char c)
                                { return c == charToRemove2; }),
                 result.end());

    std::vector<std::string> tokens;
    std::stringstream ss(result);
    std::string token;
    while (std::getline(ss, token, ','))
    {
      tokens.push_back(token);
    }
    int u = 1;
    const char *path = "/";
    DiskSpaceInfo data = getDiskSpaceInfo(path);
    double used_space = bytesToGigabytes(data.used_space);
    double total_space = bytesToGigabytes(data.total_space);
    struct json_object *jsonObj = json_object_new_object();
    if (strcmp(cm_output, "true") == 0)
    {  
        // Add value to the JSON object
        json_object_object_add(jsonObj, "DGNSM", json_object_new_int(0001));
        json_object_object_add(jsonObj, "status", json_object_new_int(rpc_main_con));
        json_object_object_add(jsonObj, "cpu", json_object_new_int(sa_sys_cpu));
        json_object_object_add(jsonObj, "ram_used", json_object_new_int(sa_sys_ram_used));
        json_object_object_add(jsonObj, "ram_total", json_object_new_int(sa_sys_ram_total));
        json_object_object_add(jsonObj, "disk_used", json_object_new_int(used_space));
        json_object_object_add(jsonObj, "disk_total", json_object_new_int(total_space));
        json_object_object_add(jsonObj, "uptime", json_object_new_int(m_uptime));
    }

    for (const std::string &s : tokens)
    {
      size_t pos = s.find(':') + 1;

      if (pos != std::string::npos)
      {

        sub_str = s.substr(pos);
        if (s.find("block") == 1)
        {
          if (strcmp(cm_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "block", json_object_new_string(cString));
          }
          m_blocks = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("headers") == 1)
        {
          if (strcmp(cm_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "headers", json_object_new_string(cString));
          }
          m_headers = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("protocolversion") == 1)
        {
          if (strcmp(cm_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "protocolversion", json_object_new_string(cString));
          }
          m_version = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("connections") == 1)
        {
          if (strcmp(cm_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "connections", json_object_new_string(cString));
          }
          m_connections = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("size") == 1)
        {
          if (strcmp(cm_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "transactions", json_object_new_string(cString));
          }
          m_transactions = std::stoi(sub_str);
          u = 2;
        }

        if (u == 2)
        {
          rpc_main_con = 1;
        }
        else
        {
          rpc_main_con = 0;
        }
      }
    }
    if (strcmp(cm_output, "true") == 0)
    {
        //Write the JSON object to a file
        const char *filename = "mainnet.json";
        FILE *file = fopen(filename, "w");
        if (file) {
            fprintf(file, "%s\n", json_object_to_json_string_ext(jsonObj, JSON_C_TO_STRING_PRETTY));
            fclose(file);
        }
    }
        //Release memory
        json_object_put(jsonObj);
    if (rpc_main_con == 1)
    {
      if (m_set_uptime == 0)
      {
        time_t unixTime;
        m_uptime = time(NULL);
        m_set_uptime = 1;
      }
      char url[150];
      sprintf(url, "%s/main/?key=%s&version=%d&connections=%d&transactions=%d&blocks=%d&headers=%d&uptime=%d", api_url, c_dgbn_api, m_version, m_connections, m_transactions, m_blocks, m_headers, m_uptime);
      std::string response = OpenURLWithVariable(url);
      size_t found = response.find("ok");
      if (found != std::string::npos)
      {
        connection = 1;
      }
    }
    sleep(10);
  }
  pthread_exit(NULL);
}

void *api_testnet(void *arg)
{
  while (run)
  {
    std::string sub_str;
    char rpcUrl[200];
    sprintf(rpcUrl, "http://%s:%s@%s:%s", ct_rpc_user, ct_rpc_password, ct_rpc_ip, ct_rpc_port);
    std::string str1 = sendBitcoinRPCRequest(rpcUrl, "getmempoolinfo");
    std::string str2 = sendBitcoinRPCRequest(rpcUrl, "getnetworkinfo");
    std::string str3 = sendBitcoinRPCRequest(rpcUrl, "getblockchaininfo");
    std::string result = str1 + str2 + str3;

    std::string textToRemove = "size_";
    size_t found = result.find(textToRemove);
    while (found != std::string::npos)
    {
      result.erase(found, textToRemove.length());
      found = result.find(textToRemove);
    }

    std::string textToRemove2 = "\"result\":{";
    size_t found2 = result.find(textToRemove2);
    while (found2 != std::string::npos)
    {
      result.erase(found2, textToRemove2.length());
      found2 = result.find(textToRemove2);
    }

    char charToRemove = '{';
    result.erase(std::remove_if(result.begin(), result.end(), [charToRemove](char c)
                                { return c == charToRemove; }),
                 result.end());
    char charToRemove2 = '}';
    result.erase(std::remove_if(result.begin(), result.end(), [charToRemove2](char c)
                                { return c == charToRemove2; }),
                 result.end());

    std::vector<std::string> tokens;
    std::stringstream ss(result);
    std::string token;
    const char *path = "/";
    DiskSpaceInfo data = getDiskSpaceInfo(path);
    double used_space = bytesToGigabytes(data.used_space);
    double total_space = bytesToGigabytes(data.total_space);
    struct json_object *jsonObj = json_object_new_object();
    if (strcmp(ct_output, "true") == 0)
    {
        // Add value to the JSON object
        json_object_object_add(jsonObj, "DGNSM", json_object_new_int(0001));
        json_object_object_add(jsonObj, "status", json_object_new_int(rpc_test_con));
        json_object_object_add(jsonObj, "cpu", json_object_new_int(sa_sys_cpu));
        json_object_object_add(jsonObj, "ram_used", json_object_new_int(sa_sys_ram_used));
        json_object_object_add(jsonObj, "ram_total", json_object_new_int(sa_sys_ram_total));
        json_object_object_add(jsonObj, "disk_used", json_object_new_int(used_space));
        json_object_object_add(jsonObj, "disk_total", json_object_new_int(total_space));
        json_object_object_add(jsonObj, "uptime", json_object_new_int(t_uptime));
    }
    while (std::getline(ss, token, ','))
    {
      tokens.push_back(token);
    }
    int u = 1;
    for (const std::string &s : tokens)
    {
      size_t pos = s.find(':') + 1;
      if (pos != std::string::npos)
      {
        sub_str = s.substr(pos);
        if (s.find("block") == 1)
        {
          if (strcmp(ct_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "block", json_object_new_string(cString));
          }
          t_blocks = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("headers") == 1)
        {
          if (strcmp(ct_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "headers", json_object_new_string(cString));
          }
          t_headers = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("protocolversion") == 1)
        {
          if (strcmp(ct_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "protocolversion", json_object_new_string(cString));
          }
          t_version = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("connections") == 1)
        {
          if (strcmp(ct_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "connections", json_object_new_string(cString));
          }
          t_connections = std::stoi(sub_str);
          u = 2;
        }
        if (s.find("size") == 1)
        {
          if (strcmp(ct_output, "true") == 0)
          {
            // Add value to the JSON object
            const char* cString = sub_str.c_str();
            json_object_object_add(jsonObj, "transactions", json_object_new_string(cString));
          }
          t_transactions = std::stoi(sub_str);
          u = 2;
        }

        if (u == 2)
        {
          rpc_test_con = 1;
        }
        else
        {
          rpc_test_con = 0;
        }
      }
    }
    if (strcmp(ct_output, "true") == 0)
    {
        //Write the JSON object to a file
        const char *filename = "testnet.json";
        FILE *file = fopen(filename, "w");
        if (file) {
            fprintf(file, "%s\n", json_object_to_json_string_ext(jsonObj, JSON_C_TO_STRING_PRETTY));
            fclose(file);
        }

    }
        //Release memory
        json_object_put(jsonObj);
    if (rpc_test_con == 1)
    {
      if (t_set_uptime == 0)
      {
        time_t unixTime;
        t_uptime = time(NULL);
        t_set_uptime = 1;
      }
      char url[150];
      sprintf(url, "%s/test/?key=%s&version=%d&connections=%d&transactions=%d&blocks=%d&headers=%d&uptime=%d", api_url, c_dgbn_api, t_version, t_connections, t_transactions, t_blocks, t_headers, t_uptime);
      std::string response = OpenURLWithVariable(url);
      size_t found = response.find("ok");
      if (found != std::string::npos)
      {
        connection = 1;
      }
    }
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
  const char *homeDir = getenv("HOME");
  std::string sfilePath = std::string(homeDir) + "/.digibyte/dgnsm.config";
  std::ifstream config(sfilePath);
  std::ifstream config2("dgnsm.config");
  std::string filePath;
  if (config.is_open())
  {
    filePath = sfilePath;
    config.close();
  }
  if (config2.is_open())
  {
    filePath = "dgnsm.config";
    config2.close();
  }
  if (filePath == "")
  {
    printf("%-15s\n", "Error loading configuration file: dgnsm.config");
    exit(0);
  }
  JJINI reader(filePath);
  std::string dgbn_api = reader.Get("hardware", "api", "");
  c_dgbn_api = strcpy(new char[dgbn_api.length() + 1], dgbn_api.c_str());

  std::string m_enabled = reader.Get("mainnet", "enabled", "");
  if (m_enabled == "")
  {
    m_enabled = reader.Get("mainnet", "m_enabled", "");
  }
  cm_enabled = strcpy(new char[m_enabled.length() + 1], m_enabled.c_str());

  std::string m_rpc_ip = reader.Get("mainnet", "rpc_ip", "");
  if (m_rpc_ip == "")
  {
    m_rpc_ip = reader.Get("mainnet", "m_rpc_ip", "");
  }
  cm_rpc_ip = strcpy(new char[m_rpc_ip.length() + 1], m_rpc_ip.c_str());

  std::string m_rpc_port = reader.Get("mainnet", "rpc_port", "");
  if (m_rpc_port == "")
  {
    m_rpc_port = reader.Get("mainnet", "m_rpc_port", "");
  }
  cm_rpc_port = strcpy(new char[m_rpc_port.length() + 1], m_rpc_port.c_str());

  std::string m_rpc_user = reader.Get("mainnet", "rpc_user", "");
  if (m_rpc_user == "")
  {
    m_rpc_user = reader.Get("mainnet", "m_rpc_user", "");
  }
  cm_rpc_user = strcpy(new char[m_rpc_user.length() + 1], m_rpc_user.c_str());

  std::string m_rpc_password = reader.Get("mainnet", "rpc_password", "");
  if (m_rpc_password == "")
  {
    m_rpc_password = reader.Get("mainnet", "m_rpc_password", "");
  }
  cm_rpc_password = strcpy(new char[m_rpc_password.length() + 1], m_rpc_password.c_str());

  std::string m_output = reader.Get("mainnet", "json_dump", "");
  if (m_output == "")
  {
    m_output = reader.Get("mainnet", "m_json_dump", "");
  }
  cm_output = strcpy(new char[m_output.length() + 1], m_output.c_str());

  std::string t_enabled = reader.Get("testnet", "enabled", "");
  if (t_enabled == "")
  {
    t_enabled = reader.Get("mainnet", "t_enabled", "");
  }
  ct_enabled = strcpy(new char[t_enabled.length() + 1], t_enabled.c_str());

  std::string t_rpc_ip = reader.Get("testnet", "rpc_ip", "");
  if (t_rpc_ip == "")
  {
    t_rpc_ip = reader.Get("mainnet", "t_rpc_ip", "");
  }
  ct_rpc_ip = strcpy(new char[t_rpc_ip.length() + 1], t_rpc_ip.c_str());

  std::string t_rpc_port = reader.Get("testnet", "rpc_port", "");
  if (t_rpc_port == "")
  {
    t_rpc_port = reader.Get("mainnet", "t_rpc_port", "");
  }
  ct_rpc_port = strcpy(new char[t_rpc_port.length() + 1], t_rpc_port.c_str());

  std::string t_rpc_user = reader.Get("testnet", "rpc_user", "");
  if (t_rpc_user == "")
  {
    t_rpc_user = reader.Get("mainnet", "t_rpc_user", "");
  }
  ct_rpc_user = strcpy(new char[t_rpc_user.length() + 1], t_rpc_user.c_str());

  std::string t_rpc_password = reader.Get("testnet", "rpc_password", "");
  if (t_rpc_password == "")
  {
    t_rpc_password = reader.Get("mainnet", "t_rpc_password", "");
  }
  ct_rpc_password = strcpy(new char[t_rpc_password.length() + 1], t_rpc_password.c_str());

  std::string t_output = reader.Get("testnet", "json_dump", "");
  if (t_output == "")
  {
    t_output = reader.Get("mainnet", "t_json_dump", "");
  }
  ct_output = strcpy(new char[t_output.length() + 1], t_output.c_str());

  if (strcmp(c_dgbn_api, "") == 0)
  {
    printf("%-15s\n", "Error loading api key! dgnsm.config");
    exit(0);
  }
  
  pthread_t ptid0;
  pthread_t ptid2;
  pthread_t ptid3;
  pthread_t ptid4;
  pthread_create(&ptid0, NULL, menu, NULL);
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
