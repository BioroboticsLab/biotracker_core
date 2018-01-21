#include "TcpListener.h"

#include <sstream>

TcpListener::TcpListener(QObject *parent) : QTcpServer(parent)
{
}

void TcpListener::acceptConnection()
{
	while (this->hasPendingConnections())
	{
		QTcpSocket *socket = this->nextPendingConnection();
		_sockets.push_back(socket);
	}
}

void TcpListener::sendPositionsToSocket(std::string packet)
{

	std::vector<QTcpSocket *>::iterator sock_it = _sockets.begin();
	for (sock_it = _sockets.begin(); sock_it != _sockets.end(); ++sock_it)
		(*sock_it)->write(packet.c_str());
}

std::string TcpListener::sendPositions(
	int frameNo,  
	const std::vector<FishPose>& poses,
	const std::vector<cv::Point2f>& polygon,
	std::chrono::system_clock::time_point ts)
{
	std::stringstream str;
	str << "frame:" << frameNo << ";";

	str << "polygon:" << polygon.size() << ";";
	for (auto vertex = polygon.cbegin(); vertex != polygon.cend(); ++vertex)
	{
		str << vertex->x << "x" << vertex->y << ";";
	}

	int fishCount = poses.size();
	int fi = 0;
	str << "fishcount:" << fishCount << ";";

	for (int i=0; i < poses.size(); i++)
	{

		str << i+1 << "," // the id of the fish
			<< poses[i].position_cm().x << "," // real position in cm - x-coordinate
			<< poses[i].position_cm().y << "," // real position in cm - y-coordinate
			<< poses[i].orientation_rad() << "," // orientation in radian
			<< poses[i].orientation_deg() << "," // orientation in degree
			<< poses[i].width() << ","	// size of the fish blob: width
			<< poses[i].height() << "," // size of the fish blob: height
			<< long(std::chrono::duration_cast<std::chrono::milliseconds>(ts.time_since_epoch()).count()) << "," // the time stamp
			<< "F" << (((i + 1) == poses.size()) ? ";" : "&"); // F: obsolete "isRobofish" flag (F: not a robofish)
	}
	str << "end\n";

	//std::cout << str.str() << std::endl;

#ifdef SAVETRACKINGPACKAGES
	_trackingInfoOutputFile4Simulation << str.str();
#endif
	sendPositionsToSocket(str.str());
	return str.str();
}
