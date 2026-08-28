#include "MainWindow.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	createInterface();
}

void MainWindow::createInterface() {
	setWindowTitle("Information as Bits");
	resize(800, 600);
}