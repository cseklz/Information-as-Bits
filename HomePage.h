#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

class HomePage final : public QWidget {
	Q_OBJECT

	public:
		explicit HomePage(QWidget* parent = nullptr);

	signals:
		void conversionRequested();
		void pictureRequested();
};

#endif