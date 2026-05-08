#pragma once

// ======================================================
// SYSTEM
// ======================================================

#define TAG_SYSTEM        "SYSTEM"
#define TAG_BOOT          "BOOT"
#define TAG_INIT          "INIT"
#define TAG_SHUTDOWN      "SHUTDOWN"
#define TAG_HEALTH        "HEALTH"
#define TAG_WATCHDOG      "WATCHDOG"
#define TAG_MEMORY        "MEMORY"
#define TAG_TIME          "TIME"
#define TAG_CONFIG        "CONFIG"

// ======================================================
// NETWORKING
// ======================================================

#define TAG_NET           "NET"
#define TAG_WIFI          "WIFI"
#define TAG_WIFI_AP       "WIFI_AP"
#define TAG_WIFI_STA      "WIFI_STA"
#define TAG_WIFI_MGR      "WIFI_MGR"
#define TAG_MDNS          "MDNS"
#define TAG_DNS           "DNS"
#define TAG_HTTP          "HTTP"
#define TAG_API           "API"
#define TAG_AUTH          "AUTH"

// ======================================================
// STORAGE
// ======================================================

#define TAG_FS            "FS"
#define TAG_STORAGE       "STORAGE"
#define TAG_FILE          "FILE"
#define TAG_LITTLEFS      "LITTLEFS"
#define TAG_JSON          "JSON"

// ======================================================
// DEVICE MANAGEMENT
// ======================================================

#define TAG_DEVICE        "DEVICE"
#define TAG_DEVICE_REG    "DEVICE_REG"
#define TAG_DEVICE_CFG    "DEVICE_CFG"
#define TAG_ACTION        "ACTION"
#define TAG_ACTION_REG    "ACTION_REG"
#define TAG_COMMAND       "COMMAND"

// ======================================================
// IR SERVICES
// ======================================================

#define TAG_IR            "IR"
#define TAG_IR_RECV       "IR_RECV"
#define TAG_IR_SEND       "IR_SEND"
#define TAG_IR_RAW        "IR_RAW"
#define TAG_IR_DECODE     "IR_DECODE"
#define TAG_IR_ENCODE     "IR_ENCODE"
#define TAG_IR_QUEUE      "IR_QUEUE"
#define TAG_IR_TIMING     "IR_TIMING"

// ======================================================
// LEARNING MODE
// ======================================================

#define TAG_LEARN         "LEARN"
#define TAG_CAPTURE       "CAPTURE"
#define TAG_MODE          "MODE"

// ======================================================
// SERVICES
// ======================================================

#define TAG_SERVICE       "SERVICE"
#define TAG_NETWORK_SVC   "NETWORK_SVC"
#define TAG_STORAGE_SVC   "STORAGE_SVC"
#define TAG_COMMAND_SVC   "COMMAND_SVC"
#define TAG_DEVICE_SVC    "DEVICE_SVC"
#define TAG_ACTION_SVC    "ACTION_SVC"

// ======================================================
// FUTURE FEATURES
// ======================================================

#define TAG_OTA           "OTA"
#define TAG_SCENE         "SCENE"
#define TAG_AUTOMATION    "AUTOMATION"
#define TAG_SCHEDULER     "SCHEDULER"
#define TAG_ALEXA         "ALEXA"
#define TAG_HOMEKIT       "HOMEKIT"
#define TAG_MQTT          "MQTT"
#define TAG_CLOUD         "CLOUD"
#define TAG_SYNC          "SYNC"
#define TAG_BACKUP        "BACKUP"
#define TAG_RESTORE       "RESTORE"

// ======================================================
// SECURITY
// ======================================================

#define TAG_SECURITY      "SECURITY"
#define TAG_TOKEN         "TOKEN"
#define TAG_ENCRYPTION    "ENCRYPT"
#define TAG_PAIRING       "PAIRING"

// ======================================================
// DEBUGGING
// ======================================================

#define TAG_DEBUG         "DEBUG"
#define TAG_TRACE         "TRACE"
#define TAG_PERF          "PERF"
#define TAG_METRICS       "METRICS"

// ======================================================
// HARDWARE
// ======================================================

#define TAG_GPIO          "GPIO"
#define TAG_LED           "LED"
#define TAG_BUTTON        "BUTTON"
#define TAG_SENSOR        "SENSOR"
#define TAG_POWER         "POWER"

// ======================================================
// APPLICATION
// ======================================================

#define TAG_REMOTE        "REMOTE"
#define TAG_UI            "UI"
#define TAG_RUNTIME       "RUNTIME"
#define TAG_STATE         "STATE"