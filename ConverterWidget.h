#ifndef CONVERTERWIDGET_H
#define CONVERTERWIDGET_H

#include <QWidget>

class QButtonGroup;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;

class ConverterWidget : public QWidget {
public:
	explicit ConverterWidget(QWidget* parent = nullptr);

private:
	enum class InputMode {
		ascii,
		decimal,
		binary,
		octal,
		hexadecimal
	};

	void createInterface();

	void updateOutputVisibility();
	void clearOutputs();
	void setInputMode(InputMode mode);

	void updateInputState(const QString& s);
	void handleInputMode(int id);
	void handleConversion();

	static bool containsOnlyAscii(const QString& s);

	QPushButton* convertButton_{nullptr};
	QButtonGroup* inputModeGroup_{nullptr};

	QLineEdit* inputBox_{nullptr};
	QLineEdit* asciiOutput_{nullptr};
	QLineEdit* decimalOutput_{nullptr};
	QLineEdit* binaryOutput_{nullptr};
	QLineEdit* octalOutput_{nullptr};
	QLineEdit* hexadecimalOutput_{nullptr};

	QLabel* asciiOutputLabel_{nullptr};
	QLabel* decimalOutputLabel_{nullptr};
	QLabel* binaryOutputLabel_{nullptr};
	QLabel* octalOutputLabel_{nullptr};
	QLabel* hexadecimalOutputLabel_{nullptr};

	QLabel* inputLabel_{nullptr};
	QLabel* statusLabel_{nullptr};

	InputMode inputMode_{InputMode::ascii};
};

#endif