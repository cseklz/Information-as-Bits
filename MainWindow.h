#ifndef INFORMATION_AS_BITS_MAINWINDOW_H
#define INFORMATION_AS_BITS_MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow {
public:
	explicit MainWindow(QWidget *parent = nullptr);
private:
	void createInterface();
};
#endif
