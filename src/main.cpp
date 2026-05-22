#include <Arduino.h>
#include <TimeLib.h>

#include "utils/logger.h"
#include "network_manager.h"
#include "api_server.h"
#include "services/storage_service.h"
#include "services/device_service.h"
#include "services/mode_service.h"
#include "services/command_service.h"
#include "services/ir_receive_service.h"
#include "services/learn_service.h"
#include "services/ir_send_service.h"
#include "config/system_config.h"
#include "services/scheduler_service.h"
#include "services/alexa_service.h"
#include "led_driver.h"

void setup() {
  // put your setup code here, to run once:
  Logger::begin();
  Logger::info(TAG_BOOT, "Firmware booting: Boot successful");

  // Device Service setup
  if (!StorageService::begin()) {
    Logger::error(TAG_SYSTEM, "Failed to initialize storage");
    ESP.restart();
  }

  // Load devices
  DeviceService::begin();

  // Mode Service setup
  ModeService::begin();

  // IR Receive Service setup
  IRReceiveService::begin();

  // IR Send Service setup
  IRSendService::begin();

  // Learn Service setup
  LearnService::begin();

  // Network setup
  NetworkManager::begin();

  // Time setup
  configTime(0, 0, "pool.ntp.org");
  Logger::info(TAG_TIME, "Time synchronizing");
  while (time(nullptr) < 100000) {
    delay(500);
    Logger::info(TAG_TIME, ".");
  }
  time_t now = time(nullptr);
  Logger::info(TAG_TIME, "Time synchronized: %s", ctime(&now));

  // Api Server setup
  ApiServer::begin();

  // Scheduler Service setup
  SchedulerService::begin();


}

void loop() {
  // put your main code here, to run repeatedly:
  NetworkManager::loop();
  statusLed.update();
  IRReceiveService::loop();
  LearnService::loop();
  SchedulerService::loop();
  AlexaService::loop();
}
