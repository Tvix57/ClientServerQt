#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Common {
namespace Constants {

// Network settings
constexpr int DEFAULT_SERVER_PORT = 12345;
constexpr int RECONNECT_INTERVAL_MS = 5000;
constexpr int MIN_SEND_INTERVAL_MS = 10;
constexpr int MAX_SEND_INTERVAL_MS = 100;

// Message sizes
constexpr int SHORT_MESSAGE_MAX = 50;
constexpr int MEDIUM_MESSAGE_MAX = 200;

// Client types
constexpr char CLIENT_TYPE_DEVICE[] = "device";
constexpr char CLIENT_TYPE_GUI[] = "gui";

// Message types
constexpr char MSG_TYPE_HANDSHAKE[] = "handshake";
constexpr char MSG_TYPE_ACK[] = "acknowledgment";
constexpr char MSG_TYPE_COMMAND[] = "command";
constexpr char MSG_TYPE_CONFIG[] = "configuration";
constexpr char MSG_TYPE_NETWORK_METRICS[] = "NetworkMetrics";
constexpr char MSG_TYPE_DEVICE_STATUS[] = "DeviceStatus";
constexpr char MSG_TYPE_LOG[] = "Log";

// Commands
constexpr char CMD_START[] = "start";
constexpr char CMD_STOP[] = "stop";
constexpr char CMD_STATUS[] = "status";

// Severity levels
constexpr char SEVERITY_DEBUG[] = "DEBUG";
constexpr char SEVERITY_INFO[] = "INFO";
constexpr char SEVERITY_WARNING[] = "WARNING";
constexpr char SEVERITY_ERROR[] = "ERROR";
constexpr char SEVERITY_CRITICAL[] = "CRITICAL";

} // namespace Constants
} // namespace Common

#endif // CONSTANTS_H