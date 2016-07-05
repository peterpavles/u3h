#pragma once

#include "ui_ScreenCapSettings.h"
#include "qt_Commons.h"


class ScreenCapSetting : public QMainWindow
{
	Q_OBJECT

public:
	ScreenCapSetting(QWidget *parent = nullptr);
	~ScreenCapSetting();

private:
	Ui::MainWindow ui;

public slots:
	void on_pushButton_cancel_clicked();
	void on_pushButton_applyOptions_clicked();


signals:
	void ApplyOptions(QString quality, QString scaling, QString refresh);

};