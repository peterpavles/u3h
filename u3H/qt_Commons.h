#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QSettings>
#include <QMessageBox>
#include <QThread>
#include <QMap>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QEvent>
#include <QCloseEvent>
#include <QTime>
#include <QTimer>
#include <QStyleFactory>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QLabel>

#include <QTreeWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMainWindow>

#include "qt_windows.h"


template <class T> class VPtr
{
public:
	static T* asPtr(QVariant v)
	{
		return  (T *)v.value<void *>();
	}

	static QVariant asQVariant(T* ptr)
	{
		return qVariantFromValue((void *)ptr);
	}
};