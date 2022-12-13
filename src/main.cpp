#include <Arduino.h>
#include <ESP32MotorControl.h>
#include "esp_camera.h"
#include <WiFi.h>

// PRESERVE ALWAYS
#define LEFT1 12
#define LEFT2 14
#define RIGHT1 33
#define RIGHT2 32
#define AWAKE 15
#define LIGHT 13

ESP32MotorControl motorControl = ESP32MotorControl(AWAKE);

// Select camera model
#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM


#include "camera_pins.h"

const char *ssid_AP     = "ESP_32_WROVER"; //Enter the router name
const char *password_AP = "16August!"; //Enter the router password

IPAddress local_IP(192,168,1,100);//Set the IP address of ESP32 itself
IPAddress gateway(192,168,1,10);   //Set the gateway of ESP32 itself
IPAddress subnet(255,255,255,0);  //Set the subnet mask for ESP32 itself

camera_config_t config;

void startCameraServer();
void config_init();

void setup() {
    pinMode(AWAKE, OUTPUT);
    pinMode(LEFT1, OUTPUT);
    pinMode(LEFT2, OUTPUT);
    pinMode(RIGHT1, OUTPUT);
    pinMode(RIGHT2, OUTPUT);
    pinMode(LIGHT, OUTPUT);

    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    config_init();
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t * s = esp_camera_sensor_get();
    s->set_vflip(s, 1);        //1-Upside down, 0-No operation
    s->set_hmirror(s, 0);      //1-Reverse left and right, 0-No operation
    s->set_brightness(s, 1);   //up the blightness just a bit
    s->set_saturation(s, -1);  //lower the saturation

    Serial.println("Setting soft-AP configuration ... ");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
    Serial.println("Setting soft-AP ... ");
    boolean result = WiFi.softAP(ssid_AP, password_AP);

    if(result){
        Serial.println("Ready");
        Serial.println(String("Soft-AP IP address = ") + WiFi.softAPIP().toString());
        Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
    }else{
        Serial.println("Failed!");
    }

    Serial.println("");
    Serial.println("WiFi connected");

    startCameraServer();


    motorControl.wake();
    motorControl.attachMotors(LEFT1, LEFT2, RIGHT1, RIGHT2);
    digitalWrite(LIGHT, HIGH);
    delay(30000);
}

void loop() {
    motorControl.motorsSet(50, 50);
    delay(2000);
    motorControl.motorsSet(0, 0);
    delay(2000);
    motorControl.motorsSet(-50, -50);
    delay(2000);
    motorControl.motorsSet(0, 0);
    delay(2000);
}

void config_init() {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.fb_count = 1;
}