#ifndef ids_h
#define ids_h

// #############
// # BLOCK IDS #
// #############

#define B_AIR           B0000000
#define B_FIRE          B0000001
#define B_DIRT          B0000010
#define B_COBBLE        B0000011
#define B_LAVA0         B0000100
#define B_LAVA1         B0000101
#define B_LAVA2         B0000110
#define B_LAVA3         B0000111
#define B_WATER0        B0001000
#define B_WATER1        B0001001
#define B_WATER2        B0001010
#define B_WATER3        B0001011
#define B_WATER4        B0001100
#define B_WATER5        B0001101
#define B_WATER6        B0001110
#define B_WATER7        B0001111
#define B_FARM0         B0010000
#define B_FARM1         B0010001
#define B_FARM2         B0010010
#define B_FARM3         B0010011
#define B_WHEAT0        B0010100
#define B_WHEAT1        B0010101
#define B_WHEAT2        B0010110
#define B_WHEAT3        B0010111
#define B_CARROT0       B0011000
#define B_CARROT1       B0011001
#define B_CARROT2       B0011010
#define B_CARROT3       B0011011
#define B_POTATO0       B0011100
#define B_POTATO1       B0011101
#define B_POTATO2       B0011110
#define B_POTATO3       B0011111
#define B_COAL_ORE      B0100000
#define B_IRON_ORE      B0100001
#define B_GOLD_ORE      B0100010
#define B_DIA_ORE       B0100011
#define B_DOOR_C        B0100100
#define B_DOOR_O        B0100101
#define B_TRAP_C        B0100110
#define B_TRAP_O        B0100111
#define B_STONE         B0101000
#define B_SNDSTN        B0101001
#define B_GRAVEL        B0101010
#define B_SAND          B0101011
#define B_WOOD          B0101100
#define B_PLANKS        B0101101
#define B_LEAVES        B0101110
#define B_OBSIDIAN      B0101111
#define B_GLASS         B0110000
#define B_BRICKS        B0110001
#define B_WOOL          B0110010
#define B_GOLD_BLOCK    B0110011
#define B_BED           B0110100
#define B_LADDER        B0110101
#define B_TORCH         B0110110
#define B_SPONGE        B0110111
#define B_CRAFT         B0111000
#define B_FURNACE       B0111001
#define B_CHEST         B0111010
#define B_GRASS         B0111011
#define B_SAPLING       B0111100
#define B_FLOWER        B0111101
#define B_TNT_U         B0111110
#define B_TNT_L         B0111111

// #####################
// # CONTROL BLOCK IDS #
// #####################

#define C_LIGHT0      B1000000
#define C_LIGHT1      B1000001
#define C_LIGHT2      B1000010
#define C_LIGHT3      B1000011
#define C_LIGHT4      B1000100
#define C_LIGHT5      B1000101
#define C_LIGHT6      B1000110
#define C_LIGHT7      B1000111
#define C_VOID        B1001000

#define GEN_AIR       B1010000
#define GEN_T_AIR     B1010001
#define GEN_DIRT      B1010010
#define GEN_T_DIRT    B1010011
#define GEN_GRAVEL    B1010100
#define GEN_SAND      B1010110
#define GEN_T_SAND    B1010111
#define GEN_T_COAL    B1011000
#define GEN_T_IRON    B1011001
#define GEN_T_GOLD    B1011010
#define GEN_T_DIA     B1011011
#define GEN_T_WOOD    B1011100
#define GEN_T_LEAVES  B1011101
#define GEN_T_SNDSTN  B1011110
#define GEN_T_GRASS   B1011111

#define COMP_END      B1111101
#define FLAG_4_BYTE   B1111110
#define FLAG_3_BYTE   B1111111

// ############
// # ITEM IDS #
// ############

#define I_NONE                B00000000
#define I_GUNPOWDER           B00000001
#define I_DIRT                B00000010
#define I_COBBLE              B00000011
#define I_IDK                 B00000100
#define I_IDK                 B00000101
#define I_IDK                 B00000110
#define I_IDK                 B00000111
#define I_IDK                 B00001000
#define I_IDK                 B00001001
#define I_IDK                 B00001010
#define I_IDK                 B00001011
#define I_IDK                 B00001100
#define I_IDK                 B00001101
#define I_IDK                 B00001110
#define I_IDK                 B00001111
#define I_IDK                 B00010000
#define I_IDK                 B00010001
#define I_IDK                 B00010010
#define I_IDK                 B00010011
#define I_WHEAT               B00010100
#define I_SEEDS               B00010101
#define I_IDK                 B00010110
#define I_IDK                 B00010111
#define I_CARROT              B00011000
#define I_IDK                 B00011001
#define I_IDK                 B00011010
#define I_IDK                 B00011011
#define I_POTATO              B00011100
#define I_FLESH               B00011101
#define I_IDK                 B00011110
#define I_IDK                 B00011111
#define I_IDK                 B00100000
#define I_IRON_ORE            B00100001
#define I_GOLD_ORE            B00100010
#define I_IDK                 B00100011
#define I_DOOR                B00100100
#define I_IDK                 B00100101
#define I_TRAP                B00100110
#define I_IDK                 B00100111
#define I_STONE               B00101000
#define I_SNDSTN              B00101001
#define I_GRAVEL              B00101010
#define I_SAND                B00101011
#define I_WOOD                B00101100
#define I_PLANKS              B00101101
#define I_IDK                 B00101110
#define I_OBSIDIAN            B00101111
#define I_GLASS               B00110000
#define I_BRICKS              B00110001
#define I_WOOL                B00110010
#define I_GOLD_BLOCK          B00110011
#define I_BED                 B00110100
#define I_LADDER              B00110101
#define I_TORCH               B00110110
#define I_SPONGE              B00110111
#define I_CRAFT               B00111000
#define I_FURNACE             B00111001
#define I_CHEST               B00111010
#define I_IDK                 B00111011
#define I_SAPLING             B00111100
#define I_FLOWER              B00111101
#define I_TNT                 B00111110
#define I_IDK                 B00111111
#define I_SWORD_WOOD          B01000000
#define I_PICK_WOOD           B01000001
#define I_SHOVEL_WOOD         B01000010
#define I_AXE_WOOD            B01000011
#define I_HOE_WOOD            B01000100
#define I_SWORD_STONE         B01000101
#define I_PICK_STONE          B01000110
#define I_SHOVEL_STONE        B01000111
#define I_AXE_STONE           B01001000
#define I_HOE_STONE           B01001001
#define I_SWORD_GOLD          B01001010
#define I_PICK_GOLD           B01001011
#define I_SHOVEL_GOLD         B01001100
#define I_AXE_GOLD            B01001101
#define I_HOE_GOLD            B01001110
#define I_SWORD_IRON          B01001111
#define I_PICK_IRON           B01010000
#define I_SHOVEL_IRON         B01010001
#define I_AXE_IRON            B01010010
#define I_HOE_IRON            B01010011
#define I_SWORD_DIA           B01010100
#define I_PICK_DIA            B01010101
#define I_SHOVEL_DIA          B01010110
#define I_AXE_DIA             B01010111
#define I_HOE_DIA             B01011000
#define I_BOW                 B01011001
#define I_ARROW               B01011010
#define I_FLINT               B01011011
#define I_COAL                B01011100
#define I_GOLD_INGOT          B01011101
#define I_IRON_INGOT          B01011110
#define I_DIAMOND             B01011111
#define I_HELMET_LEATHER      B01100000
#define I_CHESTPLATE_LEATHER  B01100001
#define I_LEG_LEATHER         B01100010
#define I_BOOT_LEATHER        B01100011
#define I_HELMET_GOLD         B01100100
#define I_CHESTPLATE_GOLD     B01100101
#define I_LEG_GOLD            B01100110
#define I_BOOT_GOLD           B01100111
#define I_HELMET_IRON         B01101000
#define I_CHESTPLATE_IRON     B01101001
#define I_LEG_IRON            B01101010
#define I_BOOT_IRON           B01101011
#define I_HELMET_DIA          B01101100
#define I_CHESTPLATE_DIA      B01101101
#define I_LEG_DIA             B01101110
#define I_BOOT_DIA            B001101111
#define I_STICK               B01110000
#define I_LEATHER             B01110001
#define I_STRING              B01110010
#define I_BUCKET_E            B01110011
#define I_BUCKET_W            B01110100
#define I_BUCKET_L            B01110101
#define I_CHICKEN_R           B01110110
#define I_CHICKEN_C           B01110111
#define I_PORK_R              B01111000
#define I_PORK_C              B01111001
#define I_BEEF_R              B01111010
#define I_BEEF_C              B01111011
#define I_F_S                 B01111100
#define I_CHARCOAL            B01111101
#define I_BREAD               B01111110
#define I_FEATHER             B01111111

#endif
