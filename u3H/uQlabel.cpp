#include "uQlabel.h"

uQLabel::uQLabel(QWidget *parent /* = nullptr */) : QLabel(parent)
{
	this->setMinimumSize(1, 1);
}




void uQLabel::setPixmap(const QPixmap & p)
{
	pix = p;
	QLabel::setPixmap(p);
}


int uQLabel::heightForWidth(int width) const
{
	return ((qreal)pix.height()*width) / pix.width();
}

QSize uQLabel::sizeHint() const 
{
	int w = this->width();
	return QSize(w, heightForWidth(w));
}

void uQLabel::resizeEvent(QResizeEvent * e)
{
	if (!pix.isNull())
	{
		QPixmap pm(pix.width(), pix.height());
		pm = pix.copy(0, 0, pix.width(), pix.height());
		QLabel::setPixmap(pm.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	}
}