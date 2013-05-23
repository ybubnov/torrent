#include "FileIconProvider.h"

FileIconProvider::FileIconProvider(){

}

FileIconProvider::~FileIconProvider(){

}

QIcon FileIconProvider::icon(QFileIconProvider::IconType type){
    return provider.icon(type);
}

QIcon FileIconProvider::icon(QString path){
    QFile file(path);
    QIcon* ricon;

    if(file.open(QIODevice::ReadWrite)){
        ricon = new QIcon(provider.icon(QFileInfo(file)));
    }

    file.close();
    QFile::remove(path);

    return *ricon;
}
