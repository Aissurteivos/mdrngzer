#Rom Layout Information

All memory in this rom is in Little-endian format
##mappa_s.bin 0x003D3A00 - 0x00415D20

###Floor index 0x003D3A2A - 0x003DC628

Each floor in this game has its own 18-byte list that points to other locations in the rom.

|Offset|Value|
|------|:----|
|00-01|Unknown|
|02-03|Unknown|
|04-05|Unknown|
|06-07|Unknown|
|08-09|Unknown|
|0A-0B|Floor terrain Index|
|0C-0D|Pokemon spawn Index|
|0E-0F|Trap spawn Index?|
|10-11|Item spawn Index|

###Floor terrain data 0x003DC7B0 - 0x003EB1D0
Each floor in this game has 32 byte terrain data entry in a list.
The values are as shown in the table:

|Offset|Value|
|------|:----|
|00|[Floor structure](floorLayouts.md)|
|01|Unknown|
|02|Terrain Appearance|
|03|Music index|
|04|Weather|
|05|Unknown|
|06|Initial Pokemon density|
|07|Kecleon Shop Percentage|
|08|Monster House Percentage|
|09|Flag?|
|0A|Unknown|
|0B|Flag?|
|0C|Rooms with water index|
|0D|Flag?|
|0E|Flag?|
|0F|Item density|
|10|Trap density|
|11|Floor counter|
|12|Event index?|
|13|Unknown|
|14|Buried Item density?|
|15|Water density?|
|16|Darkness level?|
|17|Coin Max, steps of 40|
|18|Unknown|
|19|Unknown|
|1A|Unknown|
|1B|Flag?|
|1C-1D|Enemy IQ|
|1E|Padding|
|1F|Padding|

###Pokemon spawns 0x003EB1D0 to 0x00406E78

Pokemon spawn according to an indexed list in the ROM.  
Each entry in the list is terminated by 8 bytes of zeros, due to being variable legnth.  
Each entry has multiple pokemon in a sub-list.
Each pokemon that spawns in this sub-list has a 8 byte entry.

|Offset|Value|
|------|:----|
|00|Unknown|
|01|Levelx2|
|02-03|Probability of appearing?|
|04-05|Probability of appearing?|
|06-07|Pokemon ID|

Two pokemon IDs have their probablity set to bytes set to 0, and appear in every entry.
Those are Kecleon, and "decoy" being 0x017F, 0x0292 respectively.

Another pokemon that seems to have its probability set to 0 is castform and its forms, which are 0x017B-0x017E.

###Credits

90% of the information in this doc has been gathered from these sources:  
http://projectpokemon.org/wiki/Pok%C3%A9mon_Mystery_Dungeon_Explorers  
http://www.gamefaqs.com/boards/955859-pokemon-mystery-dungeon-explorers-of-sky/51698562
http://www.gamefaqs.com/boards/938930-pokemon-mystery-dungeon-explorers-of-darkness/50597686
http://apointlessplace.net/wms/  
http://apointlessplace.net/wms/data/ - A bunch of helpful lists
