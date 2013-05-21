#include "TorrentWindow.h"
#include "ui_TorrentWindow.h"

#include <cstdlib>
#include <sstream>


using namespace interface;

TorrentWindow::TorrentWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::TorrentWindow){
    ui->setupUi(this);
    ui->torrentListTreeWidget->setRootIsDecorated(false);
    ui->torrentListTreeWidget->resizeColumnToContents(0);
    ui->torrentListTreeWidget->resizeColumnToContents(4);
    ui->torrentListTreeWidget->resizeColumnToContents(5);

    ui->torrentListTreeWidget->setColumnWidth(1, 200);
    ui->torrentListTreeWidget->setColumnWidth(2, 70);
    ui->torrentListTreeWidget->setColumnWidth(3, 200);

    connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(add_handle()));
}

TorrentWindow::~TorrentWindow(){
    delete ui;
}

void TorrentWindow::start_new_handle(){
    add_row(utils::row("aaaa", "asdasdasd", "asdasda", 67));
    torrent_dialog->close();
}

void TorrentWindow::add_handle(){
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.torrent)"));
    try{
        bencode::provider::stream provider(file_name.toStdWString());
        bencode::parser parser(provider);
        network::bittorrent::file_parser torrent_file(parser.node());

        torrent_dialog = new interface::dialog(this);
        torrent_dialog->comment(torrent_file.comment());
        torrent_dialog->date(torrent_file.creation_date());
        torrent_dialog->name(torrent_file.name());
        torrent_dialog->download_list(torrent_file.files());

        torrent_dialog->show();

        connect(torrent_dialog, SIGNAL(start_signal()), this, SLOT(start_new_handle()));
    }catch(std::exception e){
        QString message = tr("Unable to load \n'") + file_name + tr("': torrent is not valid bencoding");

        message_box = new QMessageBox(this);
        message_box->setText(message);
        message_box->show();
    }
}


void TorrentWindow::add_row(utils::row row){
    QProgressBar* bar = new QProgressBar();

    bar->setValue(row.get_status());
    bar->setStyleSheet("QProgressBar{color: transparent; height:16px; margin:2px;}");

    bar_list.push_back(bar);
    file_list.push_back(row);

    std::stringstream position_flow;
    std::stringstream status_flow;

    position_flow << file_list.size();
    status_flow << row.get_status();

    std::string position(position_flow.str());
    std::string status(status_flow.str());


    QTreeWidgetItem* item = new QTreeWidgetItem(QTreeWidgetItem::UserType);
    item->setText(0, QString(position.c_str()));
    item->setText(1, QString(row.get_file_name().c_str()));
    item->setText(2, QString(row.get_file_size().c_str()));
    item->setText(4, QString(status.c_str()));

    ui->torrentListTreeWidget->addTopLevelItem(item);

    ui->torrentListTreeWidget->setItemWidget(
                ui->torrentListTreeWidget->topLevelItem(file_list.size() - 1),
                3,
                bar);
}
