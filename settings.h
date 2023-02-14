// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x05, 0x99, 0xDA };
static uint8_t appEui[] = { 0x9A, 0xC3, 0x08, 0x90, 0xC8, 0x4F, 0xA4, 0x33 };
static uint8_t appKey[] = { 0x42, 0x0A, 0x60, 0x65, 0x83, 0x98, 0x75, 0x58, 0xEC, 0x46, 0xC7, 0x0A, 0x19, 0xD8, 0x21, 0x28 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Sensors params
#define AnemPort GPIO0
volatile int TurnsPulses; // number of turns of the anemomether

// Devise location
const float latitude = 39.573;
const float longitude = 2.732;
const int alt = 20;

// Other params
float SensorId= 10.01;// Sensor  identifcator number 
const int DutyCycle = 60000; // Transmision and reading period

