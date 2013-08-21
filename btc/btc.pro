QT += core gui
QT += widgets

CONFIG -= console

TARGET = btc
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += C:\boost\boost_1_53_0
INCLUDEPATH += C:\boost\boost_1_53_0\stage\lib
INCLUDEPATH += C:\Qt\Qt5.0.2\Tools\MinGW\bin
INCLUDEPATH += C:\Qt\Qt5.0.2\Tools\QtCreator\bin

#Win32
LIBS += -LC:\boost\boost_1_53_0\stage\lib \
    -lboost_system-mgw47-mt-1_53 \
    -lboost_filesystem-mgw47-mt-1_53 \
    -lboost_thread-mgw47-mt-1_53 \
    -lws2_32 \
    -lmswsock


#Linux
#INCLUDEPATH += /usr/include/boost
#
#LIBS += -L/usr/bin \
#    -lboost_system \
#    -lboost_filesystem \
#    -lboost_thread

SOURCES += main.cpp \
    bencode/BencodeParser.cpp \
    bencode/consumer/BencodeVectorConsumer.cpp \
    bencode/consumer/BencodeStringConsumer.cpp \
    bencode/consumer/BencodeStreamConsumer.cpp \
    bencode/provider/BencodeVectorProvider.cpp \
    bencode/provider/BencodeStreamProvider.cpp \
    bencode/type/BencodeString.cpp \
    bencode/type/BencodeList.cpp \
    bencode/type/BencodeInteger.cpp \
    bencode/type/BencodeElement.cpp \
    bencode/type/BencodeDictionary.cpp \
    encryption/RandomStringGenerator.cpp \
    encryption/PercentEncoding.cpp \
    encryption/Converter.cpp \
    encryption/sha/SequreHashAlgorithm.cpp \
    encryption/sha/BlockVectorProvider.cpp \
    encryption/sha/BlockStringProvider.cpp \
    encryption/sha/BlockStreamProvider.cpp \
    network/bittorrent/TrackerResponseParser.cpp \
    network/bittorrent/BitTorrentProtocol.cpp \
    network/bittorrent/message/Unchoke.cpp \
    network/bittorrent/message/Request.cpp \
    network/bittorrent/message/Piece.cpp \
    network/bittorrent/message/NotInterested.cpp \
    network/bittorrent/message/KeepAlive.cpp \
    network/bittorrent/message/Interested.cpp \
    network/bittorrent/message/Have.cpp \
    network/bittorrent/message/Handshake.cpp \
    network/bittorrent/message/Choke.cpp \
    network/bittorrent/message/Cancel.cpp \
    network/bittorrent/message/BitField.cpp \
    network/bittorrent/peer_wire/PeerWireProtocol.cpp \
    network/bittorrent/piece/TorrentPiece.cpp \
    network/bittorrent/piece/PieceControl.cpp \
    network/http/HttpProtocol.cpp \
    network/http/HttpParser.cpp \
    network/http/HttpAddressParser.cpp \
    network/tcp/TcpProtocol.cpp \
    network/udp/UdpProtocol.cpp \
    interface/TorrentWindow.cpp \
    interface/DownloadDialog.cpp \
    interface/FileIconProvider.cpp \
    network/bittorrent/io/file.cpp \
    network/bittorrent/io/file_splitter.cpp \
    network/bittorrent/peer_wire/peer.cpp \
    network/bittorrent/peer_wire/parser.cpp \
    network/bittorrent/file_parser.cpp \
    network/bittorrent/peer_wire/incoming_protocol.cpp \
    network/bittorrent/peer_wire/incoming_runner.cpp

HEADERS += \
    network.h \
    encryption.h \
    bencode.h \
    bencode/type.h \
    bencode/provider.h \
    bencode/consumer.h \
    bencode/BencodeParser.h \
    bencode/consumer/BencodeVectorConsumer.h \
    bencode/consumer/BencodeStringConsumer.h \
    bencode/consumer/BencodeStreamConsumer.h \
    bencode/consumer/BencodeConsumerInterface.h \
    bencode/provider/BencodeVectorProvider.h \
    bencode/provider/BencodeStreamProvider.h \
    bencode/provider/BencodeProviderInterface.h \
    bencode/type/BencodeString.h \
    bencode/type/BencodeList.h \
    bencode/type/BencodeInteger.h \
    bencode/type/BencodeElement.h \
    bencode/type/BencodeDictionary.h \
    bencode/type/BencodeComparator.h \
    encryption/sha.h \
    encryption/RandomStringGenerator.h \
    encryption/PercentEncoding.h \
    encryption/Converter.h \
    encryption/sha/SecureHashAlgorithm.h \
    encryption/sha/BlockVectorProvider.h \
    encryption/sha/BlockStringProvider.h \
    encryption/sha/BlockStreamProvider.h \
    encryption/sha/BlockProviderInterface.h \
    network/udp.h \
    network/tcp.h \
    network/ResponsibleInterface.h \
    network/http.h \
    network/bittorrent.h \
    network/bittorrent/TrackerResponseParser.h \
    network/bittorrent/piece.h \
    network/bittorrent/peer_wire.h \
    network/bittorrent/message.h \
    network/bittorrent/io.h \
    network/bittorrent/BitTorrentProtocol.h \
    network/bittorrent/message/Unchoke.h \
    network/bittorrent/message/Request.h \
    network/bittorrent/message/Piece.h \
    network/bittorrent/message/NotInterested.h \
    network/bittorrent/message/MessageInterface.h \
    network/bittorrent/message/KeepAlive.h \
    network/bittorrent/message/Interested.h \
    network/bittorrent/message/Have.h \
    network/bittorrent/message/Handshake.h \
    network/bittorrent/message/Choke.h \
    network/bittorrent/message/Cancel.h \
    network/bittorrent/message/BitField.h \
    network/bittorrent/peer_wire/PeerWireProtocol.h \
    network/bittorrent/piece/TorrentPiece.h \
    network/bittorrent/piece/PieceControl.h \
    network/http/HttpProtocol.h \
    network/http/HttpParser.h \
    network/http/HttpAddressParser.h \
    network/tcp/TcpProtocol.h \
    network/udp/UdpProtocol.h \
    interface.h \
    interface/TorrentWindow.h \
    interface/DownloadDialog.h \
    interface/TorrentUpdater.h \
    interface/TreeWidgetItem.h \
    interface/FileIconProvider.h \
    network/GameOverableInterface.h \
    network/bittorrent/io/basic_file.h \
    network/bittorrent/io/file.h \
    network/bittorrent/io/file_splitter.h \
    network/bittorrent/io/notifiable.h \
    network/bittorrent/peer_wire/peer.h \
    network/bittorrent/peer_wire/parser.h \
    network/bittorrent/basic_parser.h \
    network/bittorrent/file_parser.h \
    network/bittorrent/peer_wire/incoming_communicatable.h \
    network/bittorrent/peer_wire/outcoming_communicatable.h \
    network/bittorrent/core/executable.h \
    network/bittorrent/core.h \
    network/bittorrent/peer_wire/incoming_protocol.h \
    network/bittorrent/peer_wire/incoming_runner.h

FORMS += \
    interface/DownloadDialog.ui \
    interface/TorrentWindow.ui

RESOURCES += \
    interface/icons.qrc

OTHER_FILES +=

