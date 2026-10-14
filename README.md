# DGNSM (DigiNode Status Monitor) 

<!-- sponsors:short -->
<a href="https://github.com/sponsors/DigiNode-Tools"><img src="https://img.shields.io/badge/Sponsor%20on-GitHub-0066CC?style=for-the-badge&labelColor=002352&logo=githubsponsors&logoColor=EA4AAA" alt="Sponsor DigiNode Tools on GitHub"></a> <a href="https://ko-fi.com/diginode"><img src="https://img.shields.io/badge/Support%20on-Ko--fi-0066CC?style=for-the-badge&labelColor=002352&logo=kofi&logoColor=FF5E5B" alt="Support DigiNode Tools on Ko-fi"></a>

**Find this useful?** Please consider sponsoring me on [GitHub](https://github.com/sponsors/DigiNode-Tools) or [Ko-fi](https://ko-fi.com/diginode), so I can devote more time to DigiNode Tools and projects like this one.
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
