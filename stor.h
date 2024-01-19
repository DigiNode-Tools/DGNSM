#ifndef STOR_H
#define STOR_H

// Int System
  int connection = 0;
  int sa_sys_cpu = 0;
  int sa_sys_rss = 0;
  int sa_sys_vsz = 0;
  int sa_sys_ram_used = 0;
  int sa_sys_ram_total = 0;

// Mainnet
  int rpc_main_con = 0;
  int m_version = 0;
  int m_connections = 0;
  int m_transactions = 0;
  int m_blocks = 0;
  int m_headers = 0;
  int m_uptime = 0;
  int m_set_uptime = 0;

// Testnet
  int rpc_test_con = 0;
  int t_version = 0;
  int t_connections = 0;
  int t_transactions = 0;
  int t_blocks = 0;
  int t_headers = 0;
  int t_uptime = 0;
  int t_set_uptime = 0;

// Char
  char *c_dgbn_api;
  char *cm_enabled;
  char *cm_rpc_ip;
  char *cm_rpc_port;
  char *cm_rpc_user;
  char *cm_rpc_password;
  char *cm_output;
  char *ct_enabled;
  char *ct_rpc_ip;
  char *ct_rpc_port;
  char *ct_rpc_user;
  char *ct_rpc_password;
  char *ct_output;

// Api url
  char api_url[] = "https://digibytenode.com/api";

volatile int run = 1;
typedef struct DiskSpaceInfo
{
    unsigned long long total_space;
    unsigned long long free_space;
    unsigned long long used_space;
} DiskSpaceInfo;

#endif // STOR_H
