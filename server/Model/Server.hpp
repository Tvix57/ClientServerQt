#ifndef SERVER_HPP
#define SERVER_HPP

#include <QMap>
#include <QByteArray>

class Server {
public:
    Server();
    ~Server();

    void start();
    void stop();

private:
    void listen(int clientSocket);
    void breakConnection(int clientSocket);
    int getClientData();

    QMap<int,QByteArray> clientTable = {};

private:

};
#endif // SERVER_HPP
