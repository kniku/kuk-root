/********************************************************************************************************
		* PROGRAM      : companylog
		* DATE - TIME  : Sonntag 14 Mai 2006 - 10h18
		* AUTHOR       : Martin Paar ( martin.paar@gmx.at )
			* FILENAME     : EcoSocket.h
			* LICENSE      :
* COMMENTARY   :
********************************************************************************************************/
#ifndef ECO_SOCKET_H
#define ECO_SOCKET_H

#include <QtNetwork/QTcpSocket>
#include <QDataStream>
#include <QObject>
#include <QMessageBox>

class EcoSocket : public QTcpSocket {
Q_OBJECT
};

#endif
