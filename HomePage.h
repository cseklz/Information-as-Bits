#pragma once

#include <QWidget>

class HomePage final : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(QWidget* parent = nullptr);

signals:
    void asciiRequested();
    void baseConverterRequested();
    void pixelsRequested();
};
