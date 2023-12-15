# esp8266_scroller

Small project to run messages and scrolling text on matrix modules (MD_MAX72XX) with MQTT based update.

## Usage

Update following lines in config.h file if neccessary:

```c
#define CLK_PIN   D5 // or SCK
#define DATA_PIN  D7 // or MOSI
#define CS_PIN    D8 // or SS

#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW // hardware type
#define MAX_DEVICES 4  // number of segments
```

Power-up device, connect to it (as an Access Point) and setup Wifi.

In web-interface you can setup MQTT settings including:
- MQTT server IP-address (ex: 192.168.1.10)
- Username (optional)
- Password (optional)
- MQTT base topic (ex: devices/)
- Device ID (ex: scrollbox1)

If settings are used as in example, then following MQTT topics are getting available:
- devices/scrollbox1/io/message -- keeps latest alert message sent to device
- devices/scrollbox1/io/message/set -- used to set alert message to device
- devices/scrollbox1/io/delay -- keeps current effect speed
- devices/scrollbox1/io/delay/set -- used to set effect speed
- devices/scrollbox1/io/effect -- keeps current effect
- devices/scrollbox1/io/effect/set -- used to set effect (see below)
- devices/scrollbox1/io/default_message0 -- keeps default message value (device switches back to it after alert message)
- devices/scrollbox1/io/default_message0/set -- used to set default message value
- devices/scrollbox1/io/default_message1 -- keeps 2nd default message value (optional, when set switches between 1st and 2nd message automatically)
- devices/scrollbox1/io/default_message1/set -- used to set 2nd default message value

List of effects avaiable:
- PA_NO_EFFECT
- PA_PRINT
- PA_SCROLL_LEFT
- PA_SCROLL_RIGHT
- PA_SCROLL_UP
- PA_SCROLL_DOWN
- PA_SPRITE
- PA_SLICE
- PA_MESH
- PA_FADE
- PA_DISSOLVE
- PA_BLINDS
- PA_RANDOM
- PA_WIPE
- PA_WIPE_CURSOR
- PA_SCAN_HORIZ
- PA_SCAN_HORIZX
- PA_SCAN_VERT
- PA_SCAN_VERTX
- PA_OPENING
- PA_OPENING_CURSOR
- PA_CLOSING
- PA_CLOSING_CURSOR
- PA_SCROLL_UP_LEFT
- PA_SCROLL_UP_RIGHT
- PA_SCROLL_DOWN_LEFT
- PA_SCROLL_DOWN_RIGHT
- PA_GROW_UP
- PA_GROW_DOWN

Greetings to the authors of [MD_Parola](https://github.com/MajicDesigns/MD_Parola) & [MD_MAX72xx](https://github.com/MajicDesigns/MD_MAX72xx) wonderful libraries!