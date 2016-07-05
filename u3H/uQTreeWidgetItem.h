#pragma once 

#include "qt_Commons.h"


#define BytesSizeRole 0x0101

class uQTreeWidgetItem : public QTreeWidgetItem
{



public:
	uQTreeWidgetItem(QTreeWidget* parent=nullptr) : QTreeWidgetItem(parent)
	{
	}
	


private:
	bool operator<(const QTreeWidgetItem &other) const override
	{
		int column = treeWidget()->sortColumn();

		// if column to sort contain numerical value
		QRegExp re("\\d*");  
		if (re.exactMatch(text(column)) && re.exactMatch(other.text(column))) {
			return text(column).toInt() < other.text(column).toInt();
		}

		if (!data(column, BytesSizeRole).isNull() && !other.data(column, BytesSizeRole).isNull()) {
			return data(column, BytesSizeRole).toLongLong() < other.data(column, BytesSizeRole).toLongLong();
		}
			
		return text(column).toLower() < other.text(column).toLower();

	}
};