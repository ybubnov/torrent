#include "DownloadDialog.h"
#include "ui_DownloadDialog.h"

using namespace interface;

DownloadDialog::DownloadDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DownloadDialog){
    ui->setupUi(this);
    ui->downloadListTreeWidget->setColumnWidth(0, 350);

    directory = ui->downloadFodlerLineEdit->text();

    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SIGNAL(cancel_signal()));
    connect(ui->okPushButton, SIGNAL(clicked()), this, SIGNAL(start_signal()));
    connect(ui->browsePushButton, SIGNAL(clicked()), this, SLOT(browse_handle()));
}

void DownloadDialog::comment(std::wstring value){
    ui->torrentCommentLabel->setText(QString::fromStdWString(value));
}

void DownloadDialog::date(std::string value){
    ui->torrentDateLabel->setText(QString::fromStdString(value));
}

void DownloadDialog::name(std::wstring value){
    ui->torrentNameLabel->setText(QString::fromStdWString(value));
}

std::vector<std::wstring> DownloadDialog::split(std::wstring &wstring, wchar_t delimeter){
    std::wstringstream flow(wstring);
    std::vector<std::wstring> elements;
    std::wstring item;

    while(std::getline(flow, item, delimeter)) {
        elements.push_back(item);
    }

    return elements;
}

QString DownloadDialog::size(int64_t length){
    double num = (double)length;
    QStringList list;
    list << tr("kB") << tr("MB") << tr("GB") << tr("TB");

    QStringListIterator i(list);
    QString unit(tr("bytes"));

    while(num >= 1024.0 && i.hasNext()){
        unit = i.next();
        num /= 1024.0;
    }

    return QString().setNum(num, 'f', 2) + " " + unit;
}

QString DownloadDialog::size(std::string& length){
    int64_t ilength;
    std::stringstream flow;
    flow << length;
    flow >> ilength;

    return size(ilength);
}

std::wstring DownloadDialog::get_folder(){
    return directory.toStdWString();
}

void DownloadDialog::browse_handle(){
    directory = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"), QString(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(directory.size()){
        ui->downloadFodlerLineEdit->setText(directory);
    }
}


void DownloadDialog::download_list(std::list<network::bittorrent::download_file> list){
    int64_t total_length = 0;

    for(std::list<network::bittorrent::download_file>::iterator it = list.begin(); it != list.end(); it++){
        network::bittorrent::download_file df = *it;

        total_length += df.length;

        QTreeWidgetItem* item = new QTreeWidgetItem(QTreeWidgetItem::UserType);

        std::vector<std::wstring> path = split(df.path, '\\');

        /*if(path.size() == 1){
            item->setText(0, QString::fromStdWString(df.path));
        }else{
            for(unsigned int i = 1; i < path.size(); i++){
                QList<QTreeWidgetItem*> list = ui->downloadListTreeWidget->findItems(QString::fromStdWString(path[i]));

                if(list.size() != 0){
                    list[0]->
                }else{

                }
            }
        }*/


        item->setText(0, QString::fromStdWString(path[path.size() - 1]));
        item->setText(1, size(df.length));

        ui->downloadListTreeWidget->addTopLevelItem(item);
    }

    if(list.size() == 1){
        ui->torrentSizeLabel->setText(tr("1 file, ") + size(total_length));
    }else{
        ui->torrentSizeLabel->setText(QString::number(list.size()) + tr(" file, ") + size(total_length));
    }


}

DownloadDialog::~DownloadDialog(){
    delete ui;
}
