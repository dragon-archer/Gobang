#include "pch_gui.h"
#include "window.h"

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	QTranslator translator;
	if(translator.load(":/translations/gobang_zh_CN.qm")) {
		a.installTranslator(&translator);
	}
	Window w;
	w.show();
	return a.exec();
}
