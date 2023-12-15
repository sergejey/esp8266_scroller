#include <Arduino.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <mqtt_com.h>
#include <wifi_com.h>
#include <config.h>

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Scrolling parameters
uint8_t frameDelay = 50; // default frame delay value
uint16_t messageReceived = 0;
uint16_t defaultMessageUpdated = 0;

int prevDefaultScreen = 1;

textEffect_t scrollEffect = PA_SCROLL_LEFT;
bool newMessageAvailable = false;

#define BUF_SIZE 512
char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
char defMessage[BUF_SIZE];

String defaultMessage0 = "00:00";
String defaultMessage1 = "";

void setDefaultMessage0(String msg)
{
    defaultMessage0 = msg;
}

void setDefaultMessage1(String msg)
{
    defaultMessage1 = msg;
}

void setDefaultScreen()
{

    defaultMessageUpdated = round(millis() / 1000);
    if (prevDefaultScreen == 1 || defaultMessage1 == "")
    {
        prevDefaultScreen = 0;
        defaultMessage0.toCharArray(defMessage, BUF_SIZE);
    }
    else
    {
        prevDefaultScreen = 1;
        defaultMessage1.toCharArray(defMessage, BUF_SIZE);
    }
    P.displayScroll(defMessage, PA_CENTER, PA_NO_EFFECT, 50);
}

void handleScroller()
{
    if (P.displayAnimate())
    {
        if (messageReceived > 0 && (millis() / 1000 - messageReceived) >= DEFAULT_MESSAGE_MIN_DELAY_SECONDS)
        {
            messageReceived = 0;
            P.displayClear();
            setDefaultScreen();
        }
        if (defaultMessageUpdated > 0 && (millis() / 1000 - defaultMessageUpdated) >= DEFAULT_MESSAGE_UPDATE_SECONDS)
        {
            setDefaultScreen();
        }

        P.displayReset();
    }
}

void refreshMessage()
{
    P.displayClear();
    P.displayScroll(curMessage, PA_LEFT, scrollEffect, frameDelay);
}

String getMessage()
{
    String message = curMessage;
    return message;
}

String getEffect()
{
    String effect = "Unknown";
    if (scrollEffect == PA_NO_EFFECT)
        effect = "PA_NO_EFFECT";
    if (scrollEffect == PA_PRINT)
        effect = "PA_PRINT";
    if (scrollEffect == PA_SCROLL_LEFT)
        effect = "PA_SCROLL_LEFT";
    if (scrollEffect == PA_SCROLL_RIGHT)
        effect = "PA_SCROLL_RIGHT";
    if (scrollEffect == PA_SCROLL_UP)
        effect = "PA_SCROLL_UP";
    if (scrollEffect == PA_SCROLL_DOWN)
        effect = "PA_SCROLL_DOWN";

    if (scrollEffect == PA_SPRITE)
        effect = "PA_SPRITE";
    if (scrollEffect == PA_SLICE)
        effect = "PA_SLICE";
    if (scrollEffect == PA_MESH)
        effect = "PA_MESH";
    if (scrollEffect == PA_FADE)
        effect = "PA_FADE";
    if (scrollEffect == PA_DISSOLVE)
        effect = "PA_DISSOLVE";
    if (scrollEffect == PA_BLINDS)
        effect = "PA_BLINDS";
    if (scrollEffect == PA_RANDOM)
        effect = "PA_RANDOM";
    if (scrollEffect == PA_WIPE)
        effect = "PA_WIPE";
    if (scrollEffect == PA_WIPE_CURSOR)
        effect = "PA_WIPE_CURSOR";
    if (scrollEffect == PA_SCAN_HORIZ)
        effect = "PA_SCAN_HORIZ";
    if (scrollEffect == PA_SCAN_HORIZX)
        effect = "PA_SCAN_HORIZX";
    if (scrollEffect == PA_SCAN_VERT)
        effect = "PA_SCAN_VERT";
    if (scrollEffect == PA_SCAN_VERTX)
        effect = "PA_SCAN_VERTX";
    if (scrollEffect == PA_OPENING)
        effect = "PA_OPENING";
    if (scrollEffect == PA_OPENING_CURSOR)
        effect = "PA_OPENING_CURSOR";
    if (scrollEffect == PA_CLOSING)
        effect = "PA_CLOSING";
    if (scrollEffect == PA_CLOSING_CURSOR)
        effect = "PA_CLOSING_CURSOR";
    if (scrollEffect == PA_SCROLL_UP_LEFT)
        effect = "PA_SCROLL_UP_LEFT";
    if (scrollEffect == PA_SCROLL_UP_RIGHT)
        effect = "PA_SCROLL_UP_RIGHT";
    if (scrollEffect == PA_SCROLL_DOWN_LEFT)
        effect = "PA_SCROLL_DOWN_LEFT";
    if (scrollEffect == PA_SCROLL_DOWN_RIGHT)
        effect = "PA_SCROLL_DOWN_RIGHT";
    if (scrollEffect == PA_GROW_UP)
        effect = "PA_GROW_UP";
    if (scrollEffect == PA_GROW_DOWN)
        effect = "PA_GROW_DOWN";

    return effect;
}

void setEffect(String msg)
{
    if (msg == "PA_NO_EFFECT")
        scrollEffect = PA_NO_EFFECT;
    if (msg == "PA_PRINT")
        scrollEffect = PA_PRINT;
    if (msg == "PA_SCROLL_LEFT")
        scrollEffect = PA_SCROLL_LEFT;
    if (msg == "PA_SCROLL_RIGHT")
        scrollEffect = PA_SCROLL_RIGHT;
    if (msg == "PA_SCROLL_UP")
        scrollEffect = PA_SCROLL_UP;
    if (msg == "PA_SCROLL_DOWN")
        scrollEffect = PA_SCROLL_DOWN;
    if (msg == "PA_SPRITE")
        scrollEffect = PA_SPRITE;
    if (msg == "PA_SLICE")
        scrollEffect = PA_SLICE;
    if (msg == "PA_MESH")
        scrollEffect = PA_MESH;
    if (msg == "PA_FADE")
        scrollEffect = PA_FADE;
    if (msg == "PA_DISSOLVE")
        scrollEffect = PA_DISSOLVE;
    if (msg == "PA_BLINDS")
        scrollEffect = PA_BLINDS;
    if (msg == "PA_RANDOM")
        scrollEffect = PA_RANDOM;
    if (msg == "PA_WIPE")
        scrollEffect = PA_WIPE;
    if (msg == "PA_WIPE_CURSOR")
        scrollEffect = PA_WIPE_CURSOR;
    if (msg == "PA_SCAN_HORIZ")
        scrollEffect = PA_SCAN_HORIZ;
    if (msg == "PA_SCAN_HORIZX")
        scrollEffect = PA_SCAN_HORIZX;
    if (msg == "PA_SCAN_VERT")
        scrollEffect = PA_SCAN_VERT;
    if (msg == "PA_SCAN_VERTX")
        scrollEffect = PA_SCAN_VERTX;
    if (msg == "PA_OPENING")
        scrollEffect = PA_OPENING;
    if (msg == "PA_OPENING_CURSOR")
        scrollEffect = PA_OPENING_CURSOR;
    if (msg == "PA_CLOSING")
        scrollEffect = PA_CLOSING;
    if (msg == "PA_CLOSING_CURSOR")
        scrollEffect = PA_CLOSING_CURSOR;
    if (msg == "PA_SCROLL_UP_LEFT")
        scrollEffect = PA_SCROLL_UP_LEFT;
    if (msg == "PA_SCROLL_UP_RIGHT")
        scrollEffect = PA_SCROLL_UP_RIGHT;
    if (msg == "PA_SCROLL_DOWN_LEFT")
        scrollEffect = PA_SCROLL_DOWN_LEFT;
    if (msg == "PA_SCROLL_DOWN_RIGHT")
        scrollEffect = PA_SCROLL_DOWN_RIGHT;
    if (msg == "PA_GROW_UP")
        scrollEffect = PA_GROW_UP;
    if (msg == "PA_GROW_DOWN")
        scrollEffect = PA_GROW_DOWN;
    publishEvent("effect", getEffect());
    refreshMessage();
}

String getDelay()
{
    String delay = String(frameDelay);
    return delay;
}

void setDelay(String msg)
{
    frameDelay = msg.toInt();
    publishEvent("delay", getDelay());
    refreshMessage();
}

void setMessage(String msg)
{
    messageReceived = round(millis() / 1000);
    defaultMessageUpdated = 0;
    Serial.println("Setting message to: ");
    Serial.println(msg);
    msg.toCharArray(newMessage, BUF_SIZE);
    strcpy(curMessage, newMessage);
    refreshMessage();
}

void startScroller()
{
    curMessage[0] = newMessage[0] = '\0';

    P.begin();
    P.displayClear();
    P.displaySuspend(false);
    P.displayScroll(curMessage, PA_LEFT, scrollEffect, frameDelay);
    sprintf(curMessage, "%03d.%03d.%03d.%03d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
    messageReceived = round(millis() / 1000);
}