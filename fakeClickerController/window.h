#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QMessageBox>

#include "clickermanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
	Q_OBJECT

public:
	Window(QWidget *parent = nullptr);
	~Window();

	void setConnectedState(bool connected);

private slots:
	void on_updatePort_clicked();

	void on_connect_clicked();

	void serialDisconnected();

	void on_oneSend_clicked();

	void on_randomDelayGroup_toggled(bool arg1);

private:
	Ui::Window *ui;

	QList<QSerialPortInfo> infoList;

	ClickerManager *clicker;
};
#endif // WINDOW_H
