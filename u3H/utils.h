#pragma once

#include "qt_Commons.h"

class utils
{
public:
	static QString stringBytetoHumanReadable(QString entry)
	{
		float num = entry.toLongLong();
		QStringList list;
		list << "KB" << "MB" << "GB" << "TB";

		QStringListIterator i(list);
		QString unit("bytes");

		while (num >= 1024.0 && i.hasNext())
		{
			unit = i.next();
			num /= 1024.0;
		}
		return QString().setNum(num, 'f', 2) + " " + unit;
	}

	static bool hasEnding(std::string const &fullString, std::string const &ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}
		else {
			return false;
		}
	}

};