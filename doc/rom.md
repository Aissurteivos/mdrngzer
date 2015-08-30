#Rom Layout Information

All memory in this rom is in Little-endian format
##mappa_s.bin 0x003D3A00 - 0x00415D20

###Floor index 0x003D3A22 - 0x003DC620

Each floor in this game has its own 18-byte list that points to other locations in the rom.

|Offset|Value|
|------|:----|
|00-01|Floor terrain Index|
|02-03|Pokemon spawn Index|
|04-05|Trap spawn Index?|
|06-07|Item spawn Index|
|08-09|Kecleon shop Index (same as item)|
|0A-0B|Monster house item Index (same as item)|
|0C-0D|Buried item Index (same as item)|
|0E-0F|Unknown|
|10-11|Unknown|

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
|14|Buried Item density|
|15|Water density|
|16|Darkness level|
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

###Item spawns 0x00409428 to 0x00415404

----To be explained later----  

Category
0x0 = Thrown (Pierce)  
0x1 = Thrown (Rock)  
0x2 = Berries/Seeds/Vitamins  
0x3 = Food/Gummies  
0x4 = Hold  
0x5 = TMS  
0x6 = Orbs  
0x8 = Misc items  
A = Link Box  

"+" means in addition to the range  
"-" means to exclude from the range  

Category 0x0 Items 0x0001 thru 0x0006 + 0x0009  
Category 0x1 Items 0x0007 thru 0x0008 + 0x000A  
Category 0x4 Items 0x000D thru 0x0044  
Category 0x2 Items 0x0045 thru 0x006C , 0x0074 thru 0x0076 - 0x0062  
Category 0x3 Items 0x006D thru 0x0070 , 0x0077 thru 0x0089 + 0x0073  
Category 0x8 Items 0x008B thru 0x00BA - 0x00A6, 0x00AF, 0x00B0, 0x00B1, 0x00B5, 0x00B8, 0x00B9  
Category 0x5 Items 0x00BC thru 0x0124 - 0x00C2, 0x00C6, 0x00CD, 0x00DB, 0x00E0, 0x00E2, 0x00EC, 0x0102, 0x0103  
Category 0x6 Items 0x012D thru 0x0167 - 0x0144, 0x0153, 0x0159, 0x015D, 0x0161  
Category 0xA Items 0x016A  


###Credits

90% of the information in this doc has been gathered from these sources:  
http://projectpokemon.org/wiki/Pok%C3%A9mon_Mystery_Dungeon_Explorers  
http://www.gamefaqs.com/boards/955859-pokemon-mystery-dungeon-explorers-of-sky/51698562
http://www.gamefaqs.com/boards/938930-pokemon-mystery-dungeon-explorers-of-darkness/50597686
http://apointlessplace.net/wms/  
http://apointlessplace.net/wms/data/ - A bunch of helpful lists
