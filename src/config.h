#define ENABLE_OTA 1
#define LEDPIN LED_BUILTIN

#define AUTO_RESTART_SECONDS 86400 // every 24 hours

#define DEFAULT_MESSAGE_MIN_DELAY_SECONDS 5 // minimum delay to switch back to default message
#define DEFAULT_MESSAGE_UPDATE_SECONDS 5    // delay for switching between default message0 and message1

const int EEPROM_MQTT_DEVICE_TITLE = 50;
const int EEPROM_MQTT_USER = 100;
const int EEPROM_MQTT_PASSWORD = 150;
const int EEPROM_MQTT_SERVER = 200;
const int EEPROM_MQTT_BASE = 250;


#define CLK_PIN   D5 // or SCK
#define DATA_PIN  D7 // or MOSI
#define CS_PIN    D8 // or SS

#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW // hardware type
#define MAX_DEVICES 4  // number of segments