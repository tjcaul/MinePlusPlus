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

void World::setTickRate (double tickRateParam) {
  ticksPerSecond = tickRateParam;
  msPerTick = round(1000 / ticksPerSecond);
  cout << prefix << F("Set Tick Rate: ") << ticksPerSecond << F(" TPS (") << msPerTick << F("ms)") << endl;
}
void World::setWorldDimensions(WorldSize sizeParam) {
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
#ifdef SAVE_LOAD_LOGGING
  cout << prefix << F("Beginning World Loading") << endl;
#endif
  byte worldSizeByte = EEPROM.read(EE_WORLD_SIZE_BYTE);
  byte worldSize = (bitRead(worldSizeByte, 7) * 8) + (bitRead(worldSizeByte, 6) * 4) + (bitRead(worldSizeByte, 5) * 2) + bitRead(worldSizeByte, 4); //shift the bits
  setWorldDimensions(WorldSize(worldSize));
  xLimit = (worldWidth - 1) / 2;
  yLimit = worldHeight - 1;
  block.createBlockDB(worldWidth, worldHeight);
  uint16_t EEPROMAddress = EE_WORLD_DATA_START_BYTE;
  uint16_t blockDBAddress = 0;
  while (true) {
    byte byteRead = EEPROM.read(EEPROMAddress);
    if (byteRead == Storage::threeByteFlag) {
      byte numberToLoad = EEPROM.read(EEPROMAddress + 1);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 2);
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << F("F 3\t\t# ") << numberToLoad << F("\t\tI ") << blockToLoad << F("\tB ") << blockDBAddress << '-' << (blockDBAddress + numberToLoad - 1) << F("\t\tE ") << EEPROMAddress << '-' << (EEPROMAddress + 2) << endl;
#endif
      EEPROMAddress += 3;
      for (uint8_t i = 0; i < numberToLoad; ++i)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == Storage::fourByteFlag) {
      uint16_t numberToLoad = (EEPROM.read(EEPROMAddress + 1) * 256) + EEPROM.read(EEPROMAddress + 2);
      byte blockToLoad = EEPROM.read(EEPROMAddress + 3);
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << F("F 4\t\t# ") << numberToLoad << F("\t\tI ") << blockToLoad << F("\tB ") << blockDBAddress << '-' << (blockDBAddress + numberToLoad - 1) << F("\t\tE ") << EEPROMAddress << '-' << (EEPROMAddress + 3) << endl;
#endif
      EEPROMAddress += 4;
      for (uint16_t i = 0; i < numberToLoad; ++i)
        blockDB[blockDBAddress + i] = blockToLoad;
      blockDBAddress += numberToLoad - 1;
    } else if (byteRead == Storage::end) {
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << prefix << F("Found COMP_END") << endl;
#endif
      break;
    }
    else {
#ifdef SAVE_LOAD_BYTE_LOGGING
      cout << F("\t\t\t\tI ") << byteRead << F("\tB ") << blockDBAddress << F("\t\t\tE ") << EEPROMAddress << endl;
#endif
      blockDB[blockDBAddress] = byteRead;
      ++EEPROMAddress;
    }
    ++blockDBAddress;
  }
  player.move(EEPROM.read(EE_PLAYER_X_HIGH_BYTE), EEPROM.read(EE_PLAYER_LOW_X_Y_BYTE));
  start();
#ifdef SAVE_LOAD_LOGGING
  cout << prefix << F("\tFinished") << endl;
#endif
}
void World::save () {
  byte worldSizeByte = 0x00;
  bitWrite(worldSizeByte, 7, bitRead(size, 3));
  bitWrite(worldSizeByte, 6, bitRead(size, 2));
  bitWrite(worldSizeByte, 5, bitRead(size, 1));
  bitWrite(worldSizeByte, 4, bitRead(size, 0));
  EEPROM.update(EE_WORLD_SIZE_BYTE, worldSizeByte);
  EEPROM.update(EE_PLAYER_X_HIGH_BYTE, lowByte(player.getCoords().x));
  EEPROM.update(EE_PLAYER_LOW_X_Y_BYTE, player.getCoords().y);
  uint16_t numberFound = 0;
  uint16_t EEPROMAddress = EE_WORLD_DATA_START_BYTE;
  id_t currentBlockID = blockDB[0];
  uint16_t i;
  for (i = 0; i < worldWidth * worldHeight; ++i) {
    if (currentBlockID == blockDB[i]) {
      ++numberFound;
    } else {
      if (numberFound == 1) {
        EEPROM.update(EEPROMAddress, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        cout << F("\t\t\t\tI ") << currentBlockID << F("\tB ") << i - 1 << F("\t\t\tE ") << EEPROMAddress << endl;
#endif
        EEPROMAddress += 1;
      } else if (numberFound < 256) {
        EEPROM.update(EEPROMAddress, Storage::threeByteFlag);
        EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        cout << F("F 3\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << '-' << i - 1 << F("\t\tE ") << EEPROMAddress << '-' << EEPROMAddress + 2 << endl;
#endif
        EEPROMAddress += 3;
      } else {
        EEPROM.update(EEPROMAddress, Storage::fourByteFlag);
        EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
        EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
        EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
        cout << F("F 4\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << '-' << i - 1 << F("\t\tE ") << EEPROMAddress << '-' << EEPROMAddress + 3 << endl;
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
    cout << F("\t\t\t\tI ") << currentBlockID << F("\tB ") << i - 1 << F("\t\t\tE ") << EEPROMAddress << endl;
#endif
    ++EEPROMAddress;
  } else if (numberFound < 256) {
    EEPROM.update(EEPROMAddress, Storage::threeByteFlag);
    EEPROM.update(EEPROMAddress + 1, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    cout << F("F 3\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << '-' << i - 1 << F("\t\tE ") << EEPROMAddress << '-' << EEPROMAddress + 2 << endl;
#endif
    EEPROMAddress += 3;
  } else {
    EEPROM.update(EEPROMAddress, Storage::fourByteFlag);
    EEPROM.update(EEPROMAddress + 1, highByte(numberFound));
    EEPROM.update(EEPROMAddress + 2, lowByte(numberFound));
    EEPROM.update(EEPROMAddress + 3, currentBlockID);
#ifdef SAVE_LOAD_BYTE_LOGGING
    cout << F("F 4\t\t# ") << numberFound << F("\t\tI ") << currentBlockID << F("\tB ") << i - numberFound << '-' << i - 1 << F("\t\tE ") << EEPROMAddress << '-' << EEPROMAddress + 3 << endl;
#endif
    EEPROMAddress += 4;
  }
  EEPROM.update(EEPROMAddress, Storage::end);
  EEPROM.update(EEPROMAddress + 1, Storage::end);
}
World world;
