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
    connect(ui->actionStopTorrent, SIGNAL(triggered()), this, SLOT(stop_handle()));
    connect(ui->actionStartTorrent, SIGNAL(triggered()), this, SLOT(start_handle()));
    connect(ui->actionRemoveTorrent, SIGNAL(triggered()), this, SLOT(delete_handle()));
}

TorrentWindow::~TorrentWindow(){
    delete ui;
}

void TorrentWindow::start_new_handle(){
    network::bittorrent::file_parser* torrent_file = parser_list[parser_list.size() - 1];

    add_row(torrent_file->name(), torrent_file->size());
    torrent_dialog->close();

    torrent_updater* subscriber = updater_list[updater_list.size() - 1];
    network::bittorrent::file_parser* parser = parser_list[parser_list.size() - 1];

    network::bittorrent::protocol* protocol =
            new network::bittorrent::protocol(subscriber, *parser, torrent_dialog->get_folder());
    protocol_list.push_back(protocol);

    protocol->yeild();
}

void TorrentWindow::cancel_handle(){
    torrent_dialog->close();

    if(parser_list.size()){
        parser_list.pop_back();
    }
}

void TorrentWindow::delete_handle(){

}

void TorrentWindow::stop_handle(){
    QList<QTreeWidgetItem*> selected = ui->torrentListTreeWidget->selectedItems();
    QString index_str;
    unsigned long index;

    for(QList<QTreeWidgetItem*>::Iterator item_ptr = selected.begin(); item_ptr != selected.end(); item_ptr++){
        index_str = (*item_ptr)->text(0);
        index = index_str.toLong();

        if(index - 1 < protocol_list.size()){
            protocol_list[index - 1]->interrupt();
            (*item_ptr)->setText(5, tr("Stopped"));
        }
    }
}

void TorrentWindow::start_handle(){
    QList<QTreeWidgetItem*> selected = ui->torrentListTreeWidget->selectedItems();
    QString index_str;
    unsigned long index;

    for(QList<QTreeWidgetItem*>::Iterator item_ptr = selected.begin(); item_ptr != selected.end(); item_ptr++){
        index_str = (*item_ptr)->text(0);
        index = index_str.toLong();

        if(index - 1 < protocol_list.size()){
            if(!protocol_list[index - 1]->alive()){
                protocol_list[index - 1]->restart();
                (*item_ptr)->setText(5, tr("Started"));
            }
        }
    }
}

void TorrentWindow::add_handle(){
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.torrent)"));

    if(file_name.size()){
        try{
            bencode::provider::stream provider(file_name.toStdWString());
            bencode::parser parser(provider);
            bencode::element* node = parser.node();
            network::bittorrent::file_parser* torrent_file = new network::bittorrent::file_parser(node);

            parser_list.push_back(torrent_file);

            torrent_dialog = new interface::dialog(this);
            torrent_dialog->comment(torrent_file->comment());
            torrent_dialog->date(torrent_file->creation_date());
            torrent_dialog->name(torrent_file->name());
            torrent_dialog->download_list(torrent_file->files());

            torrent_dialog->show();

            connect(torrent_dialog, SIGNAL(start_signal()), this, SLOT(start_new_handle()));
            connect(torrent_dialog, SIGNAL(cancel_signal()), this, SLOT(cancel_handle()));
        }catch(std::exception e){
            QString message = tr("Unable to load \n'") + file_name + tr("': torrent is not valid bencoding");

            message_box = new QMessageBox(this);
            message_box->setText(message);
            message_box->show();
        }
    }
}


void TorrentWindow::add_row(std::wstring file_name, std::string file_size){
    QProgressBar* bar = new QProgressBar();

    bar->setValue(0);
    bar->setStyleSheet("QProgressBar{color: transparent; height:16px; margin:2px;}");

    bar_list.push_back(bar);

    std::stringstream position_flow;
    position_flow << bar_list.size();
    std::string position(position_flow.str());


    ATreeWidgetItem* item = new ATreeWidgetItem(QTreeWidgetItem::UserType);
    item->setText(0, QString::fromStdString(position));
    item->setText(1, QString::fromStdWString(file_name));
    item->setText(2, DownloadDialog::size(file_size));
    item->setText(4, QString("0 %"));
    item->setText(5, tr("Started"));

    updater_list.push_back(new torrent_updater(bar, item));

    ui->torrentListTreeWidget->addTopLevelItem(item);

    ui->torrentListTreeWidget->setItemWidget(
                ui->torrentListTreeWidget->topLevelItem(bar_list.size() - 1),
                3,
                bar);
}
