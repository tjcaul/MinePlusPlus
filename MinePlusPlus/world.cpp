#include "includes.h"

#define EE_SHUTDOWN_BYTE            0
#define EE_WORLDMAP_START_BYTE      1
#define EE_WORLDMAP_END_BYTE        24
#define EE_VERSION_HIGH_BYTE        25
#define EE_VERSION_LOW_BYTE         26
#define EE_DEFAULT_ICON_BYTE        27

#define EE_WORLD_SIZE_BYTE          32
#define EE_PLAYER_X_HIGH_BYTE       33
#define EE_PLAYER_LOW_X_Y_BYTE      34
#define EE_PLAYER_HEALTH_BYTE       35
#define EE_PLAYER_HUNGER_BYTE       36
#define EE_PLAYER_SPAWNPOINTH_BYTE  37
#define EE_PLAYER_SPAWNPOINTL_BYTE  38
#define EE_PLAYER_RESERVED_BYTE     39
#define EE_WORLD_DATA_START_BYTE    40

worldWidth_t worldWidth;
worldHeight_t worldHeight;

xcoord_t xLimit; //Inclusive. Make negative to use negative x limit.
ycoord_t yLimit; //Inclusive. Bottom limit is 0.

void World::setTickRate (const double tickRateParam) {
  ticksPerSecond = tickRateParam;
  msPerTick = round(1000 / ticksPerSecond);
  com.out.logMultiple("Set Tick Rate: " + String(ticksPerSecond) + " TPS (" + String(msPerTick) + "ms)");
}
void World::setWorldDimensions(const WorldSize sizeParam) {
  size = sizeParam;
  switch (size) {
    case Default:
      worldWidth = 161;
      worldHeight = 33;
      break;
    case Tall:
      worldWidth = 11;
      worldHeight = 11;
      break;
    case Wide:
      worldWidth = 51;
      worldHeight = 11;
      break;
  }
}
void World::start() {
  GLCD.ClearScreen();
  screen.renderWorld();
}
void World::load () {
  /*for (int j = 0; j < 16; j++) {
    for (int i = 0; i < 256; i++) {
    byte testByte = EEPROM.read(i + (j * 256));
    com.out.logChars(String(testByte == i ? testByte : String(testByte) + "  !!!"));
    }
    }*/
#ifdef SAVE_LOAD_LOGGING
  com.out.log(F("Beginning World Loading"));
#endif
  byte worldSizeByte = EEPROM.read(EE_WORLD_SIZE_BYTE);
  byte worldSize = (bitRead(worldSizeByte, 7) * 8) + (bitRead(worldSizeByte, 6) * 4) + (bitRead(worldSizeByte, 5) * 2) + bitRead(worldSizeByte, 4); //shift the bits
  setWorldDimensions(WorldSize(worldSize));
  xLimit = (worldWidth - 1) / 2;
  yLimit = worldHeight - 1;
  block.createBlockDB(worldWidth, worldHeight);
  largeindex_t EEPROMAddress = EE_WORLD_DATA_START_BYTE;
  largeindex_t blockDBAddress = 0;
  while (true) {
    byte byteRead = EEPROM.read(EEPROMAddress);
    if (byteRead == C_3_FLAG) {
      byte numberToLoad = EEPROM.read(EEPROMAddress + 1);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 2);
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.logMultiple("F 3\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
      EEPROMAddress += 3;
      for (index_t i = 0; i < numberToLoad; i++)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == C_4_FLAG) {
      largeindex_t numberToLoad = (EEPROM.read(EEPROMAddress + 1) * 256) + EEPROM.read(EEPROMAddress + 2);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 3);
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.logMultiple("F 4\t\t# " + String(numberToLoad) + "\t\tI " + String(blockToLoad) + "\tB " + String(blockDBAddress) + "-" + String(blockDBAddress + numberToLoad - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
      EEPROMAddress += 4;
      for (largeindex_t i = 0; i < numberToLoad; i++)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == C_END) {
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.log(F("Found COMP_END"));
#endif
      break;
    }
    else {
#ifdef SAVE_LOAD_BYTE_LOGGING
      com.out.logMultiple("\t\t\t\tI " + String(byteRead) + "\tB " + String(blockDBAddress) + "\t\t\tE " + String(EEPROMAddress));
#endif
      blockDB[blockDBAddress] = byteRead;
      EEPROMAddress++;
    }
    blockDBAddress++;
  }
  player.move(EEPROM.read(EE_PLAYER_X_HIGH_BYTE), EEPROM.read(EE_PLAYER_LOW_X_Y_BYTE));
  start();
#ifdef SAVE_LOAD_LOGGING
  com.out.log(F("\tFinished"));
#endif
}
void World::save () {
  /*for (int j = 0; j < 16; j++) {
    for (int i = 0; i < 256; i++)
    EEPROM.update(i + (j * 256), i);
    }*/
  byte worldSizeByte = 0x00;
  bitWrite(worldSizeByte, 7, bitRead(size, 3));
  bitWrite(worldSizeByte, 6, bitRead(size, 2));
  bitWrite(worldSizeByte, 5, bitRead(size, 1));
  bitWrite(worldSizeByte, 4, bitRead(size, 0));
  EEPROM.update(EE_WORLD_SIZE_BYTE, worldSizeByte);
  EEPROM.update(EE_PLAYER_X_HIGH_BYTE, lowByte(player.getCoords().x));
  EEPROM.update(EE_PLAYER_LOW_X_Y_BYTE, player.getCoords().y);
  largeindex_t numberFound = 0;
  largeindex_t EEPROMAddress = EE_WORLD_DATA_START_BYTE;
  id_t currentBlockID = blockDB[0];
  largeindex_t i;
  for (i = 0; i < worldWidth * worldHeight; i++) {
    if (currentBlockID == blockDB[i]) {
      numberFound++;
    } else {
      if (numberFound == 1) {
        EEPROM.update(EEPROMAddress, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.logMultiple("\t\t\t\tI " + String(currentBlockID) + "\tB " + String(i - 1) + "\t\t\tE " + String(EEPROMAddress));
#endif
        EEPROMAddress += 1;
      } else if (numberFound < 256) {
        EEPROM.update(EEPROMAddress, C_3_FLAG);
        EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.logMultiple("F 3\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
        EEPROMAddress += 3;
      } else {
        EEPROM.update(EEPROMAddress, C_4_FLAG);
        EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        com.out.logMultiple("F 4\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
        EEPROMAddress += 4;
      }
      currentBlockID = blockDB[i];
      numberFound = 1;
    }
  }
  if (numberFound == 1) {
    EEPROM.update(EEPROMAddress, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.logMultiple("\t\t\t\tI " + String(currentBlockID) + "\tB " + String(i - 1) + "\t\t\tE " + String(EEPROMAddress));
#endif
    EEPROMAddress++;
  } else if (numberFound < 256) {
    EEPROM.update(EEPROMAddress, C_3_FLAG);
    EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.logMultiple("F 3\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 2));
#endif
    EEPROMAddress += 3;
  } else {
    EEPROM.update(EEPROMAddress, C_4_FLAG);
    EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    com.out.logMultiple("F 4\t\t# " + String(numberFound) + "\t\tI " + String(currentBlockID) + "\tB " + String(i - numberFound) + "-" + String(i - 1) + "\t\tE " + String(EEPROMAddress) + "-" + String(EEPROMAddress + 3));
#endif
    EEPROMAddress += 4;
  }
  EEPROM.update(EEPROMAddress, C_END);
  EEPROM.update(EEPROMAddress + 1, C_END);

}

World world;