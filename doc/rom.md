Floor information (0x003DC7B0-0x003EB200 in ROM)

32 bytes

Byte 01: Floor structure (see floor types)
Byte 02:
Byte 03: Terrain Appearance
Byte 04: Music index
Byte 05: Weather
Byte 06:
Byte 07: Initial Pokemon density
Byte 08: Kecleon Shop Percentage
Byte 09: Monster House Percentage
Byte 10: Flag?
Byte 11:
Byte 12: Flag?
Byte 13: Rooms with water index
Byte 14: Flag?
Byte 15: Flag?
Byte 16: Item density
Byte 17: Trap density
Byte 18: Floor counter
Byte 19: Event index
Byte 20:
Byte 21: Buried Item density
Byte 22: Water density
Byte 23: Darkness
Byte 24: Coin Max, steps of 40
Byte 25:
Byte 26:
Byte 27:
Byte 28: Flag?
Byte 29: Enemy IQ, least sig byte
Byte 30: Enemy IQ, most sig byte
Byte 31: Padding
Byte 32: Padding

Floor Types 

00 = Medium-Large (Biggest: 6 x 4)
01 = Small (Biggest: 2 x 3)
02 = One-Room Monster House
03 = Outer Square (Long hallway around edges of map with 8 rooms inside)
04 = CrossRoads (3 rooms at top and bottom and 2 rooms at left and right side with a string of hallways in the middle of the map connecting the rooms)
05 = Two-Room (One of them has a Monster House)
06 = Line (1 horizontal straight line of 5 rooms)
07 = Cross (5 Rooms in form of Cross; 3 x 3 with Top Left, Top Right, Bottom Left, and Bottom Right Room missing)
08 = Small-Medium (Biggest: 4 x 2)
09 = "Beetle" (1 Giant Room in middle of map with 3 vertical rooms to the left of it and to the right of it)
0A = Outer Rooms (All Rooms at edge of map; Biggest: 6 x 4 with no rooms in the middle)
0B = Small-Medium (Biggest: 3 x 3)
0C = Medium-Large (Biggest: 6 x 4)
0D = Medium-Large (Biggest: 6 x 4)
0E = Medium-Large (Biggest: 6 x 4)
0F = Medium-Large (Biggest: 6 x 4)

