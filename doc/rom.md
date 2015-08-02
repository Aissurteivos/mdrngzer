#Rom Layout Information

###Floor information
Each floor in this game has a 32 byte entry in a list that starts at 0x003DC7B0 and ends at 0x003EB1D0 in the ROM.
The values that we have figured out are as shown in the table:

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

###Pokemon spawns

Pokemon spawn according to an indexed list in the ROM.  
Each entry in the list is terminated by 8 bytes of zeros.  

Each pokemon that spawns on a floor has a 8 byte entry.

|Offset|Value|
|------|:----|
|00|Unknown|
|01|Levelx2|
|02-03|Probability of appearing?|
|04-05|Probability of appearing?|
|06-07|Pokemon ID|

Two pokemon IDs have their probablity set to bytes set to 0, and appear in every entry.
Those are Kecleon, and "decoy", being 0x017F and 0x0292 respectively.
