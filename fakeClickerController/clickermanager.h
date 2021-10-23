#ifndef CLICKER_MANAGER_H
#define CLICKER_MANAGER_H

#include <QThread>
#include <QObject>
#include <QSerialPort>

#include <QMutex>
#include <QQueue>

#include <QDebug>

typedef int response;

struct responsePacket{
	int adress[3];
	response resp;
};


class ClickerManager : public QThread
{
	Q_OBJECT
public:
	ClickerManager();
	~ClickerManager();

	void run() override;

	bool connectSerial(QSerialPortInfo &info, int baud);
	void disconnectSerial();

	void newResponsePacket(int adress[3], response resp, bool autoStart = true);

	bool isConnected(){return this->clicker.isOpen();};
	void useRandomDelay(bool use){this->randomDelay = use;};

public slots:
	void serialError(QSerialPort::SerialPortError error);
	void readIncoming();

signals:
	void disconnected();

private:
	void sendResponsePacket(const responsePacket &response);
	QSerialPort clicker;

	bool randomDelay;

	QMutex packetLock;
	QQueue<responsePacket> toSend;
};

#endif // CLICKER_MANAGER_H
