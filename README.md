# DGNSM (DigiNode Status Monitor) 

<!-- sponsors:short -->
<a href="https://github.com/sponsors/DigiNode-Tools"><img src="https://img.shields.io/badge/Sponsor%20DigiNode%20Tools-Click%20to%20support%20me-0066CC?style=for-the-badge&labelColor=002352&logo=data:image/svg%2Bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI%2BPHBhdGggZmlsbD0iI0ZGNEZBMyIgZD0iTTEyIDIxLjM1bC0xLjQ1LTEuMzJDNS40IDE1LjM2IDIgMTIuMjggMiA4LjUgMiA1LjQyIDQuNDIgMyA3LjUgM2MxLjc0IDAgMy40MS44MSA0LjUgMi4wOUMxMy4wOSAzLjgxIDE0Ljc2IDMgMTYuNSAzIDE5LjU4IDMgMjIgNS40MiAyMiA4LjVjMCAzLjc4LTMuNCA2Ljg2LTguNTUgMTEuNTRMMTIgMjEuMzV6Ii8%2BPC9zdmc%2B" alt="Sponsor DigiNode Tools - click to support me"></a>

**Find this useful?** Please consider [sponsoring me on GitHub](https://github.com/sponsors/DigiNode-Tools), so I can devote more time to DigiNode Tools and projects like this one.
<!-- /sponsors:short -->

DGNSM (DigiNode Status Monitor) is a tiny software utility that lets you monitor the status of your DigiByte Node from anywhere via the [diginode.online](https://diginode.online) website.

- Monitor your local DigiByte mainnet and/or testnet node from anywhere
- Displays node block height, peer count, sync status, uptime and more
- Also displays system status - cpu, memory and disk usage
- Anonymous - sign in with [Digi-ID](https://www.digi-id.io/) (No email required)
- Safe - no wallet information ever leaves your DigiByte node and no RPC ports are exposed to the internet

### diginode.online
![diginode.online](https://github.com/Jongjan88/DNSU/assets/125610144/c94a44fb-86ef-49e3-a24c-72c706fa655c)

### DGNSM
![DGNSM](https://github.com/Jongjan88/DNSU/assets/125610144/c7b68bd0-bb6c-43c4-badd-62de66b6a330)


<b>Debian / Ubuntu / ARM</b>

> sudo apt install build-essential git nano libcurl4-openssl-dev libjson-c-dev -y

> git clone https://github.com/DigiNode-Tools/DGNSM/

> cd DGNSM

> make

> mv dgnsm.config.template dgnsm.config

[ Get a free API key at diginode.online ]

> nano dgnsm.config
 
> ./dgnsm
