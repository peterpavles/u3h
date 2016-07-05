#pragma once 

#include "qt_Commons.h"


class uQLabel : public QLabel
{

	Q_OBJECT;

public :
	explicit uQLabel(QWidget *parent = nullptr);


	virtual int heightForWidth(int width) const;
	virtual QSize sizeHint() const;

public slots:
	void setPixmap(const QPixmap &);
	void resizeEvent(QResizeEvent *);

private:
	QPixmap pix;

};