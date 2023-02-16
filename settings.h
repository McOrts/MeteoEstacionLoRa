// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = { 0x70, ?¿, 0x12 };
static uint8_t appEui[] = { 0x9E, ?¿, 0x08 };
static uint8_t appKey[] = { 0xEB, ?¿, 0xF8 };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Sensors params
#define AnemPort GPIO0  // Digital pin connected to the anemomether
volatile int TurnsPulses; // number of turns of the anemomether
#define DHTPIN GPIO3     // Digital pin connected to the DHT22 sensor

// Devise location
const float latitude = 39.573;
const float longitude = 2.732;
const int alt = 20;

// Other params
float SensorId= 10.01;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period

