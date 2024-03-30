# DGNSU (DigiNode Status Updater) 

DGNSU (DigiNode Status Updater) is a tiny software utility that lets you monitor the status of your DigiByte Node via the [digibytenode.com](https://digibytenode.com) website.
DGNSU runs alongside your DigiByte full node and sends the status to the website making it easy to monitor it from anywhere.

- Monitors your local DigiByte mainnet and/or testnet node
- Includes block height, peer count, sync status, uptime
- Also monitors system usage - cpu, memory and disk space
- Important: No wallet information is included

### digibytenode.com
![digibytenode.com](https://github.com/Jongjan88/DNSU/assets/125610144/c94a44fb-86ef-49e3-a24c-72c706fa655c)

### DGNSU
![DGNSU](https://github.com/Jongjan88/DNSU/assets/125610144/c7b68bd0-bb6c-43c4-badd-62de66b6a330)


<b>Debian / Ubuntu / ARM</b>

> sudo apt install build-essential git nano libcurl4-openssl-dev libjson-c-dev -y

> git clone https://github.com/DigiNode-Tools/DGNSU/

> cd dgnsu

> make

> mv dgnsu.config.template dgnsu.config

[ Get free api key on digibytenode.com ]

> nano dgnsu.config
 
> ./dgnsu
