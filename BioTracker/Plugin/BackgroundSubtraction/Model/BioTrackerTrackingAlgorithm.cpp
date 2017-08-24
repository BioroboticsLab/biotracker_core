#include "BioTrackerTrackingAlgorithm.h"
#include <future>
#include "TrackedComponents/TrackedComponentFactory.h"
#include <chrono>

#include "Model/TrackedComponents/TrackingRectElement.h"
#include "settings/Settings.h"

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm(IModel *parameter, IModel *trajectory) : _ipp((TrackerParameter*)parameter)
{
	_TrackingParameter = (TrackerParameter*)parameter;
	_TrackedTrajectoryMajor = (TrackedTrajectory*)trajectory;
	_nn2d = std::make_shared<NN2dMapper>(_TrackedTrajectoryMajor);
	BioTracker::Core::Settings *set = _TrackingParameter->getSettings();
	_exporter = 0;

	Rectification::instance().initRecitification(
		_TrackingParameter->getAreaWidth(),
		_TrackingParameter->getAreaHeight(),
		_TrackedTrajectoryMajor);
	_noFish = -1;

	if (_TrackingParameter->getDoNetwork()) {
		_listener = new TcpListener(this);
		_listener->listen(QHostAddress::Any, set->getValueOrDefault<int>(FISHTANKPARAM::FISHTANK_NETWORKING_PORT, 54444));
		QObject::connect(_listener, SIGNAL(newConnection()), _listener, SLOT(acceptConnection()));
	}
}

BioTrackerTrackingAlgorithm::~BioTrackerTrackingAlgorithm()
{
	if (_exporter) {
		_exporter->close();
		delete _exporter;
	}
}

std::vector<FishPose> BioTrackerTrackingAlgorithm::getLastPositionsAsPose() {
	//TODO: This seems kinda fragile: I just assume that the tree has this very certain structure:
	// Trajectory -> M Trajectories -> N TrackedElements
	// For every of M Trajectories grab the last (highest index) of TrackedElements.
	//TODO: If we are tracking somewhere in the middle, this is bad. Do it by id!
	std::vector<FishPose> last;
	for (int i = 0; i < _TrackedTrajectoryMajor->size(); i++) {
		TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(_TrackedTrajectoryMajor->getChild(i));
		if (t) {
			TrackedElement *e = (TrackedElement *)t->getLastChild();
			last.push_back(e->getFishPose());
		}
	}
	return last;
}

void BioTrackerTrackingAlgorithm::resetFishHistory(int noFish) {

	std::vector<FishPose> last;
	for (int i = 0; i < _TrackedTrajectoryMajor->size(); i++) {
		TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(_TrackedTrajectoryMajor->getChild(i));
		if (t) {
			t->clear();
			if (!_TrackedTrajectoryMajor->remove(t)) {
				std::cout << "ERROR: Could not remove trajectory." << std::endl;
			}
			i = -1;
		}
	}

	for (int i = 0; i < noFish; i++) {
		TrackedTrajectory *t = new TrackedTrajectory();
		t->setId(i);
		TrackedElement *e = new TrackedElement(t, "n.a.", i);
		e->setId(i);
		t->add(e);
		_TrackedTrajectoryMajor->add(t);
	}
}

void BioTrackerTrackingAlgorithm::refreshPolygon() {

	std::vector<cv::Point2f> polygon;
	std::vector<cv::Point2f> polygon_cm;

	for (int i = 0; i < _TrackedTrajectoryMajor->size(); i++) {
		TrackingRectElement *te = dynamic_cast<TrackingRectElement *>(_TrackedTrajectoryMajor->getChild(i));
		if (te) {
			polygon.push_back(cv::Point(te->getX(), te->getY()));
		}
	}

	for (auto vertex = polygon.cbegin(); vertex != polygon.cend(); ++vertex)
	{
		polygon_cm.push_back(Rectification::instance().pxToCm(*vertex));
	}

	_polygon_cm = polygon_cm;
}

void BioTrackerTrackingAlgorithm::setDataExporter(IModelDataExporter *exporter) {
	TrackedElement e;
	exporter->open(_TrackedTrajectoryMajor, (IModelTrackedComponent*)&e);
	_exporter = exporter;
}

void BioTrackerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> p_image, uint framenumber)
{
	_ipp.m_TrackingParameter = _TrackingParameter;

	//dont do nothing if we ain't got an image
	if (p_image->empty()) {
		return;
	}

	if (_imageX != p_image->size().width || _imageY != p_image->size().height) {
		_imageX = p_image->size().width;
		_imageY = p_image->size().height;
		Q_EMIT emitDimensionUpdate(_imageX, _imageY);
	}

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	if (!Rectification::instance().isSetup()) {
		Rectification::instance().setupRecitification(100, 100, p_image->size().width, p_image->size().height);
	}

	//The user changed the # of fish. Reset the history and start over!
	if (_noFish != _TrackingParameter->getNoFish()) {
		_noFish = _TrackingParameter->getNoFish(); 
		resetFishHistory(_noFish);
		_nn2d = std::make_shared<NN2dMapper>(_TrackedTrajectoryMajor);
	}	

	//Do the preprocessing
	std::map<std::string, std::shared_ptr<cv::Mat>> images = _ipp.preProcess(p_image);
	std::shared_ptr<cv::Mat> dilated = images.find(std::string("Dilated"))->second;
	std::shared_ptr<cv::Mat> greyMat = images.find(std::string("Greyscale"))->second;
	std::shared_ptr<cv::Mat> sendImage;

	//Find blobs via ellipsefitting
	_bd.setMaxBlobSize(_TrackingParameter->getMaxBlobSize());
	_bd.setMinBlobSize(_TrackingParameter->getMinBlobSize());
	std::vector<BlobPose> blobs = _bd.getPoses(*dilated, *greyMat);

	//This is tricky, as the root node may contain other children than actual trajectories. 
	//Lets denote trajectories as t (they'll be represented by their currently active child), others with x, the maping goes like this:
	// Original:					  t1 x x t2 x t3 x
	// Fishposes for getNewPoses:	  t1     t2   t3
	// Output of getNewPoses:         t1'    t2'  t3'
	// Writing them back to the tree: t1'x x t2'x t3'x
	// The absence or presence of the x should not matter. 
	// However, never switch the position of the trajectories. The NN2d mapper relies on this!
	// If you mess up the order, add or remove some t, then create a new mapper. 
	std::vector<FishPose> fish = getLastPositionsAsPose();
	
	//Find new positions using 2D nearest neighbour
	std::tuple<std::vector<FishPose>, std::vector<float>> poses = _nn2d->getNewPoses(fish, blobs);

	//Insert new poses into data structure
	int trajNumber = 0;
	for (int i = 0; i < _TrackedTrajectoryMajor->size(); i++) {
		TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(_TrackedTrajectoryMajor->getChild(i));
		if (t) {
			TrackedElement *e = new TrackedElement(t, "n.a.", trajNumber);
			e->setFishPose(std::get<0>(poses)[trajNumber]);
			e->setTime(std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count());
			t->add(e);
			_exporter->writeLatest();
			//_ofs << i << "," << std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count() << ","
			//	<< std::get<0>(poses)[trajNumber].position_cm().x << "," << std::get<0>(poses)[trajNumber].position_cm().y << ","
			//	<< std::get<0>(poses)[trajNumber].orientation_deg() << "," << std::get<0>(poses)[trajNumber].orientation_rad() << ",";
			trajNumber++;
		}
	}
	//_ofs << std::endl; //TODO extra module!

	//Send forth new positions to the robotracker, if networking is enabled
	if (_TrackingParameter->getDoNetwork()){ 
		std::vector<FishPose> ps = std::get<0>(poses);
		_listener->sendPositions(framenumber, ps, std::vector<cv::Point2f>(), start);
	}

	//Send forth whatever the user selected
	switch (_TrackingParameter->getSendImage()) {
	case 0: //Send none
		//sendImage = images.find(std::string("Original"))->second;
		//Q_EMIT emitCvMatA(sendImage, QString("Original"));
		Q_EMIT emitChangeDisplayImage("Original");
		break;
	case 1:
		sendImage = images.find(std::string("Binarized"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Binarized"));
		Q_EMIT emitChangeDisplayImage(QString("Binarized"));
		break;
	case 2:
		sendImage = images.find(std::string("Eroded"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Eroded"));
		Q_EMIT emitChangeDisplayImage(QString("Eroded"));
		break;
	case 3:
		sendImage = images.find(std::string("Dilated"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Dilated"));
		Q_EMIT emitChangeDisplayImage(QString("Dilated"));
		break;
	case 4:
		sendImage = images.find(std::string("Foreground"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Foreground"));
		Q_EMIT emitChangeDisplayImage(QString("Foreground"));
		break;
	case 5:
		sendImage = images.find(std::string("Background"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Background"));
		Q_EMIT emitChangeDisplayImage(QString("Background"));
		break;
	}

	//First the user still wants to see the original image, right?
	if (framenumber==1) {
		Q_EMIT emitChangeDisplayImage("Original");
	}

	std::string newSel = _TrackingParameter->getNewSelection();
	//if (newSel !="") {
	//	Q_EMIT emitChangeDisplayImage(QString(newSel.c_str()));
	//	_TrackingParameter->setNewSelection("");
	//}

	Q_EMIT emitTrackingDone(framenumber);
}
