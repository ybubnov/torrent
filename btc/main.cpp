#include "network.h"
#include "interface.h"
//#include "encryption.h"

int main(int argc, char** argv){
    QApplication a(argc, argv);

    interface::window w;
    w.setWindowTitle("Javelin 1.0 (Beta)");
    w.show();

    return a.exec();
}
