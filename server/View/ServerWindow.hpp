#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QObject>
#include <QString>

class ServerWindow : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ isRunning NOTIFY statusChanged FINAL)
    Q_PROPERTY(QString serverStatus READ serverStatus NOTIFY statusChanged FINAL)

public:
    explicit ServerWindow(QObject *parent = nullptr);

    inline bool isRunning() const { return _isRunning; }
    inline QString serverStatus() const { return _serverStatus; }


public slots:
    void startServer();
    void stopServer();

public:
    void addClient();

signals:
    void statusChanged();
    void addRow();

private:
    bool _isRunning = false;
    QString _serverStatus = "Offline";

};

#endif // SERVERWINDOW_H
