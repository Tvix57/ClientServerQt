#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QObject>
#include <QByteArray>
#include <QJsonObject>

/**
 * @brief Класс для генерации тестовых данных
 */
class DataGenerator : public QObject
{
    Q_OBJECT

public:
    explicit DataGenerator(QObject *parent = nullptr);

    /**
     * @brief Генерация метрик сети
     * @param clientId ID клиента
     * @return JSON данные
     */
    QByteArray GenerateNetworkMetrics(const QString &clientId);

    /**
     * @brief Генерация статуса устройства
     * @param clientId ID клиента
     * @return JSON данные
     */
    QByteArray GenerateDeviceStatus(const QString &clientId);

    /**
     * @brief Генерация лога
     * @param clientId ID клиента
     * @return JSON данные
     */
    QByteArray GenerateLog(const QString &clientId);

    /**
     * @brief Обновление пороговых значений
     * @param thresholds JSON с порогами
     */
    void UpdateThresholds(const QJsonObject &thresholds);

private:
    QString generateRandomIp() const;
    QString generateRandomLogMessage(int length) const;
    QString generateRandomSeverity() const;
    int determineMessageLength() const;

private:
    QJsonObject m_thresholds;
    int m_messageCounter;
    QStringList m_logTemplates;
    QStringList m_severityLevels;
};

#endif // DATAGENERATOR_H
