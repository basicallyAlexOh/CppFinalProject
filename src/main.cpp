#include <QApplication>
#include <QLabel>
#include "core/GUIHandler.h"

int main(int argc, char *argv[])
{

    // TODO: Make this call the GUIHandler 
    QApplication app(argc, argv);

    QLabel label("Hello from Qt!");
    label.resize(200, 40);
    label.show();

    return app.exec();
}
