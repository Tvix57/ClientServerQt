#include "common/protocols.h"
#include <QJsonArray>

namespace Common {

// NetworkMetrics implementation
QJsonObject NetworkMetrics::ToJson() const
{
    QJsonObject json;
    json["type"] = "NetworkMetrics";
    json["client_id"] = clientId;
    json["timestamp"] = timestamp;
    json["bandwidth"] = bandwidth;
    json["latency"] = latency;
    json["packet_loss"] = packetLoss;
    
    if (jitter > 0) json["jitter"] = jitter;
    if (!connectionType.isEmpty()) json["connection_type"] = connectionType;
    if (signalStrength != 0) json["signal_strength"] = signalStrength;
    
    return json;
}

NetworkMetrics NetworkMetrics::FromJson(const QJsonObject &json)
{
    NetworkMetrics metrics;
    metrics.clientId = json["client_id"].toString();
    metrics.timestamp = json["timestamp"].toInteger();
    metrics.bandwidth = json["bandwidth"].toDouble();
    metrics.latency = json["latency"].toDouble();
    metrics.packetLoss = json["packet_loss"].toDouble();
    metrics.jitter = json["jitter"].toDouble();
    metrics.connectionType = json["connection_type"].toString();
    metrics.signalStrength = json["signal_strength"].toInt();
    return metrics;
}

// DeviceStatus implementation
QJsonObject DeviceStatus::ToJson() const
{
    QJsonObject json;
    json["type"] = "DeviceStatus";
    json["client_id"] = clientId;
    json["timestamp"] = timestamp;
    json["uptime"] = uptime;
    json["cpu_usage"] = cpuUsage;
    json["memory_usage"] = memoryUsage;
    json["temperature"] = temperature;
    json["ip_address"] = ipAddress;
    
    if (hasWarning) {
        json["warning"] = true;
        json["warning_message"] = warningMessage;
    }
    
    return json;
}

DeviceStatus DeviceStatus::FromJson(const QJsonObject &json)
{
    DeviceStatus status;
    status.clientId = json["client_id"].toString();
    status.timestamp = json["timestamp"].toInteger();
    status.uptime = json["uptime"].toInt();
    status.cpuUsage = json["cpu_usage"].toDouble();
    status.memoryUsage = json["memory_usage"].toDouble();
    status.temperature = json["temperature"].toDouble();
    status.ipAddress = json["ip_address"].toString();
    status.hasWarning = json["warning"].toBool();
    status.warningMessage = json["warning_message"].toString();
    return status;
}

// LogMessage implementation
QJsonObject LogMessage::ToJson() const
{
    QJsonObject json;
    json["type"] = "Log";
    json["client_id"] = clientId;
    json["timestamp"] = timestamp;
    json["severity"] = severity;
    json["message"] = message;
    json["message_id"] = messageId;
    return json;
}

LogMessage LogMessage::FromJson(const QJsonObject &json)
{
    LogMessage log;
    log.clientId = json["client_id"].toString();
    log.timestamp = json["timestamp"].toInteger();
    log.severity = json["severity"].toString();
    log.message = json["message"].toString();
    log.messageId = json["message_id"].toInt();
    return log;
}

// ProtocolHelper implementation
MessageType ProtocolHelper::GetMessageType(const QJsonObject &json)
{
    QString type = json["type"].toString();
    
    if (type == "handshake") return MessageType::Handshake;
    if (type == "acknowledgment") return MessageType::Acknowledgment;
    if (type == "command") return MessageType::Command;
    if (type == "configuration") return MessageType::Configuration;
    if (type == "NetworkMetrics") return MessageType::NetworkMetrics;
    if (type == "DeviceStatus") return MessageType::DeviceStatus;
    if (type == "Log") return MessageType::Log;
    
    return MessageType::Unknown;
}

QString ProtocolHelper::MessageTypeToString(MessageType type)
{
    switch (type) {
        case MessageType::Handshake: return "Handshake";
        case MessageType::Acknowledgment: return "Acknowledgment";
        case MessageType::Command: return "Command";
        case MessageType::Configuration: return "Configuration";
        case MessageType::NetworkMetrics: return "NetworkMetrics";
        case MessageType::DeviceStatus: return "DeviceStatus";
        case MessageType::Log: return "Log";
        default: return "Unknown";
    }
}

QByteArray ProtocolHelper::CreateHandshake(const QString &clientType, const QString &version)
{
    QJsonObject handshake;
    handshake["type"] = "handshake";
    handshake["client_type"] = clientType;
    handshake["version"] = version;
    
    return QJsonDocument(handshake).toJson();
}

QByteArray ProtocolHelper::CreateAcknowledgment(const QString &status, const QString &message)
{
    QJsonObject ack;
    ack["type"] = "acknowledgment";
    ack["status"] = status;
    ack["message"] = message;
    ack["timestamp"] = QDateTime::currentSecsSinceEpoch();
    
    return QJsonDocument(ack).toJson();
}

QByteArray ProtocolHelper::CreateCommand(const QString &command)
{
    QJsonObject cmd;
    cmd["type"] = "command";
    cmd["command"] = command;
    cmd["timestamp"] = QDateTime::currentSecsSinceEpoch();
    
    return QJsonDocument(cmd).toJson();
}

QByteArray ProtocolHelper::CreateConfiguration(const QJsonObject &config)
{
    QJsonObject cfg;
    cfg["type"] = "configuration";
    cfg["config"] = config;
    cfg["timestamp"] = QDateTime::currentSecsSinceEpoch();
    
    return QJsonDocument(cfg).toJson();
}

} // namespace Common