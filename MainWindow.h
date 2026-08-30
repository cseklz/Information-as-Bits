#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QButtonGroup;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;

class MainWindow : public QMainWindow {
	public:
		explicit MainWindow(QWidget* parent = nullptr);

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
		void clearOutputs(); // handle input section (add type checks)
		void setInputMode(InputMode mode);

		void updateInputState(const QString& s);
		void handleInputMode(int id);
		void handleConversion();

		static bool containsOnlyAscii(const QString& s); // either remove or add more for each type

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