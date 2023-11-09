#ifndef RemoteXYStream_ArduinoBLE_h
#define RemoteXYStream_ArduinoBLE_h

// #if defined(_ARDUINO_BLE_H_)

#include <ArduinoBLE.h>
#include "RemoteXYStream.h"

#define BLE_ATTRIBUTE_MAX_VALUE_LENGTH 512
#define REMOTEXYCOMM_BLEPERIPHERAL__SEND_BUFFER_SIZE BLE_ATTRIBUTE_MAX_VALUE_LENGTH
#define REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE 1024
#define REMOTEXYCOMM_BLEPERIPHERAL__SERVICE_UUID "0000FFE0-0000-1000-8000-00805F9B34FB"
#define REMOTEXYCOMM_BLEPERIPHERAL__CHARACTERISTIC_UUID "0000FFE1-0000-1000-8000-00805F9B34FB"

class CRemoteXYStream_ArduinoBLE : public CRemoteXYStream {
  public:
    CRemoteXYStream_ArduinoBLE(const char* _bleDeviceName);
    ~CRemoteXYStream_ArduinoBLE();

    /** respects the livecylce of the object */
    static void nonMemberConnectedHandler(BLEDevice central);
    void connectedHandler(BLEDevice central);

    /** respects the livecylce of the object */
    static void nonMemberDisconnectedHandler(BLEDevice central);
    void disconnectedHandler(BLEDevice central);

    /** respects the livecylce of the object */
    static void nonMemberCharacteristicWrittenHandler(BLEDevice central, BLECharacteristic characteristic);
    void characteristicWrittenHandler(BLEDevice central, BLECharacteristic characteristic);

    /** inherited from CRemoteXYStream class */  
    void startWrite (uint16_t len) override;
    void write (uint8_t b) override;
    void handler () override;

  private:
    BLEService *pService;
    BLECharacteristic *pCharacteristic;

    uint8_t sendBuffer[REMOTEXYCOMM_BLEPERIPHERAL__SEND_BUFFER_SIZE];
    uint16_t sendBufferCount;
    uint16_t sendBytesAvailable;

    uint8_t receiveBuffer[REMOTEXYCOMM_BLEPERIPHERAL__RECEIVE_BUFFER_SIZE];
    uint16_t receiveBufferStart;
    uint16_t receiveBufferPos;
    uint16_t receiveBufferCount;
};

#endif  // _BLE_PERIPHERAL_H_
// #endif //RemoteXYStream_BLEPeripheral_h
