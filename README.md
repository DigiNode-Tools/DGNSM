# DGNSM (DigiNode Status Monitor) 

DGNSM (DigiNode Status Monitor) is a tiny software utility that lets you monitor the status of your DigiByte Node from anywhere via the [diginode.online](https://diginode.online) website.

- Monitor your local DigiByte mainnet and/or testnet node from anywhere
- Includes block height, peer count, sync status, uptime
- Also monitors system usage - cpu, memory and disk space
- No email required - sign in with [Digi-ID](https://www.digi-id.io/)
- Safe - no wallet information ever leaves your node

### diginode.online
![diginode.online](https://github.com/Jongjan88/DNSU/assets/125610144/c94a44fb-86ef-49e3-a24c-72c706fa655c)

### DGNSM
![DGNSM](https://github.com/Jongjan88/DNSU/assets/125610144/c7b68bd0-bb6c-43c4-badd-62de66b6a330)


<b>Debian / Ubuntu / ARM</b>

> sudo apt install build-essential git nano libcurl4-openssl-dev libjson-c-dev -y

> git clone https://github.com/DigiNode-Tools/DGNSM/

> cd dgnsm

> make

> mv dgnsm.config.template dgnsm.config

[ Get a free API key at diginode.online ]

> nano dgnsm.config
 
> ./dgnsm
