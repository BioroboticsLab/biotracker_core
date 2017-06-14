#pragma once

#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>

#include "Model/TrackedComponents/pose/FishPose.h"

#include <vector>
#include <chrono>

class TcpListener : public QTcpServer
{
	Q_OBJECT

public:
	TcpListener(QObject *parent = 0);

public slots:
	void acceptConnection();
	void sendPositionsToSocket(std::string packet);
	std::string TcpListener::sendPositions(int frameNo,
		const std::vector<FishPose>& poses,
		const std::vector<cv::Point2f>& polygon,
		std::chrono::time_point<std::chrono::steady_clock> ts);

private:
	std::vector<QTcpSocket *> _sockets;
};