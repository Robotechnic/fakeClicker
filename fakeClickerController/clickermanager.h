#ifndef CLICKER_MANAGER_H
#define CLICKER_MANAGER_H

#include <QThread>
#include <QObject>
#include <QSerialPort>
#include <QRandomGenerator>

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

	void newResponsePacket(int adress[3], response resp);

	bool isConnected(){return this->clicker->isOpen();};
	void useRandomDelay(bool use){this->randomDelay = use;};

	void setMinDelay(int delay){this->minDelay = delay;};
	void setMaxDelay(int delay){this->maxDelay = delay;};

	void setChannel(int channel);

public slots:
	void serialError(QSerialPort::SerialPortError error);
	void readIncoming();

private slots:
	void sendResponsePacket(const responsePacket &response);

signals:
	void disconnected();

	void packetReady(const responsePacket &response);

private:
	QSerialPort *clicker;

	bool randomDelay;
	int minDelay, maxDelay;

	QMutex packetLock;
	QQueue<responsePacket> toSend;
};

#endif // CLICKER_MANAGER_H
