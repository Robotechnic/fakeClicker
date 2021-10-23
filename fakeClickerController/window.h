#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QSpinBox>

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

	void on_adress1_editingFinished();
	void on_adress2_editingFinished();
	void on_adress3_editingFinished();

	void on_actionQuit_triggered();

	void on_numberToSend_editingFinished();

	void on_multipleSend_clicked();

	void on_minRandomDelay_valueChanged(int arg1);

	void on_maxRandomDelay_valueChanged(int arg1);

private:
	Ui::Window *ui;

	QList<QSerialPortInfo> infoList;
	QList<QSpinBox *> numberToSend;

	ClickerManager *clicker;
};
#endif // WINDOW_H
