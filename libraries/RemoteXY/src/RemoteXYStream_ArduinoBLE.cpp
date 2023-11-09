// #if defined(_ARDUINO_BLE_H_)

#include "RemoteXYStream_ArduinoBLE.h"
#include <Arduino.h>

#define REMOTEXY__DEBUGLOG

CRemoteXYStream_ArduinoBLE* instance; // for access of member functions from callbacks

CRemoteXYStream_ArduinoBLE::CRemoteXYStream_ArduinoBLE(const char * _bleDeviceName) : CRemoteXYStream() {
  #if defined(REMOTEXY__DEBUGLOG)
    Serial.println("Init Arduino Nano BLE on chip");
  #endif

  receiveBufferCount = 0;        
  receiveBufferStart = 0;
  receiveBufferPos = 0;
  receiveBufferCount = 0;  

  if (!BLE.begin()) {
    #if defined(REMOTEXY__DEBUGLOG)
      Serial.println("Init Ardiono Nano BLE failed!");
    #endif
    while (1);
  }

  pService = new BLEService(REMOTEXYCOMM_BLEPERIPHERAL__SERVICE_UUID);
  pCharacteristic = new BLECharacteristic(REMOTEXYCOMM_BLEPERIPHERAL__CHARACTERISTIC_UUID, BLERead | BLEWriteWithoutResponse | BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
  pCharacteristic->setValue({0}, 1);
  pService->addCharacteristic(*pCharacteristic);
  BLE.addService(*pService);
  BLE.setAdvertisedService(*pService);
  BLE.setLocalName(_bleDeviceName);
  BLE.setDeviceName(_bleDeviceName);
  BLE.setAppearance(0x0080);
  
  instance = this;
  BLE.setEventHandler(BLEConnected, nonMemberConnectedHandler);
  BLE.setEventHandler(BLEDisconnected, nonMemberDisconnectedHandler);
  pCharacteristic->setEventHandler(BLEWritten, nonMemberCharacteristicWrittenHandler);

  BLE.advertise();

  #if defined(REMOTEXY__DEBUGLOG)
    Serial.println("BLE started");
  #endif  
}

CRemoteXYStream_ArduinoBLE::~CRemoteXYStream_ArduinoBLE() {
  instance = NULL;
}

/** respects the livecylce of the object */
void CRemoteXYStream_ArduinoBLE::nonMemberConnectedHandler(BLEDevice central) {
  if (instance != NULL) {
      instance->connectedHandler(central);
  }
}

void CRemoteXYStream_ArduinoBLE::connectedHandler(BLEDevice central) {
  #if defined(REMOTEXY__DEBUGLOG)
    Serial.println("BLE client connected");
  #endif
  receiveBufferStart = 0;
  receiveBufferPos = 0;
  receiveBufferCount = 0;  
}

/** respects the livecylce of the object */
void CRemoteXYStream_ArduinoBLE::nonMemberDisconnectedHandler(BLEDevice central) {
    if (instance != NULL) {
        instance->disconnectedHandler(central);
    }
}

void CRemoteXYStream_ArduinoBLE::disconnectedHandler(BLEDevice central) {
  #if defined(REMOTEXY__DEBUGLOG)
    Serial.println("BLE client disconnected");
  #endif
  receiveBufferCount = 0;
  BLE.advertise();
}

/** respects the livecylce of the object */
void CRemoteXYStream_ArduinoBLE::nonMemberCharacteristicWrittenHandler(BLEDevice central, BLECharacteristic characteristic) {
    if (instance != NULL) {
        instance->characteristicWrittenHandler(central, characteristic);
    }
}

void CRemoteXYStream_ArduinoBLE::characteristicWrittenHandler(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("BLE: characteristicWrittenHandler called");
  for (uint16_t i = 0; i < characteristic.valueSize(); i++) {                
    receiveBuffer[receiveBufferPos++] = characteristic.value()[i];
    if (receiveBufferPos >= REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferPos=0; 
    if (receiveBufferCount < REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferCount++;
    else {
      receiveBufferStart++;
      if (receiveBufferStart >= REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferStart=0;
    }       
  } 
}
  
void CRemoteXYStream_ArduinoBLE::startWrite(uint16_t len) {
  Serial.println("BLE: startWrite called");
  sendBytesAvailable = len;
  sendBufferCount = 0;
}    
    
void CRemoteXYStream_ArduinoBLE::write(uint8_t b) {
  Serial.println("BLE: write called");
  if (pCharacteristic->subscribed() == false) {
    #if defined(REMOTEXY__DEBUGLOG)
      Serial.println("BLE: tx characteristic not subscribed");
    #endif
    return;
  }
  sendBuffer[sendBufferCount++] = b;
  sendBytesAvailable--;
  if ((sendBufferCount == REMOTEXYCOMM_BLEPERIPHERAL__SEND_BUFFER_SIZE) || (sendBytesAvailable == 0)) {
    pCharacteristic->writeValue(sendBuffer, sendBufferCount, false);
    BLE.poll();
    sendBufferCount = 0;
  }
}

void CRemoteXYStream_ArduinoBLE::handler() {
  uint8_t b;
  BLE.poll();
  while (receiveBufferCount > 0) {
    b = receiveBuffer[receiveBufferStart++];
    if (receiveBufferStart >= REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE) receiveBufferStart=0;
    receiveBufferCount--;
    notifyReadByteListener (b);
  }
}

// #endif  // _BLE_PERIPHERAL_H_
