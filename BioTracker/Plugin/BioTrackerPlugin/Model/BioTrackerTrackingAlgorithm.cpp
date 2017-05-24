#include "BioTrackerTrackingAlgorithm.h"
#include <future>
#include "TrackedComponents\TrackedComponentFactory.h"
#include <chrono>

#include "Model\TrackedComponents\TrackingRectElement.h"

BioTrackerTrackingAlgorithm::BioTrackerTrackingAlgorithm(IModel *parameter, IModel *trajectory) : _ipp((TrackerParameter*)parameter)
{
	_TrackingParameter = (TrackerParameter*)parameter;
	_TrackedTrajectoryMajor = (TrackedTrajectory*)trajectory;
	_nn2d = std::make_shared<NN2dMapper>(_TrackedTrajectoryMajor);

	_noFish = _TrackingParameter->getNoFish();


	Rectification::instance().initRecitification(80, 80, _TrackedTrajectoryMajor);
	Rectification::instance().setupRecitification(100,100,2040,2040);

	//TODO: put this into a module
	_ofs.open("TrackingOutput.txt", std::ofstream::out);
	for (int i = 0; i < _noFish; i++) {
		_ofs << "ID,time(ns),x,y,ADeg,ARad,";
	}
	_ofs << std::endl;
}
/*
BioTrackerTrackingAlgorithm::~BioTrackerTrackingAlgorithm()
{
	_ofs.close();
}*/

std::vector<FishPose> BioTrackerTrackingAlgorithm::getLastPositionsAsPose() {
	//TODO: This seems kinda fragile: I just assume that the tree has this very certain structure:
	// Trajectory -> M Trajectories -> N TrackedElements
	// For every of M Trajectories grab the last (highest index) of TrackedElements.
	//TODO: If we are tracking somewhere in the middle, this is bad. Do it by id!
	std::vector<FishPose> last;
	for (int i = 0; i < _TrackedTrajectoryMajor->numberOfChildrean(); i++) {
		TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(_TrackedTrajectoryMajor->getChild(i));
		if (t) {
			TrackedElement *e = (TrackedElement *)t->getLastChild();
			last.push_back(e->getFishPose());
		}
	}
	return last;
}

void BioTrackerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> p_image, uint framenumber)
{
	_ipp.m_TrackingParameter = _TrackingParameter;
	auto start = std::chrono::high_resolution_clock::now();

	//The user changed the # of fish. Reset the history and start over!
	if (_noFish != _TrackingParameter->getNoFish()) {
		_noFish = _TrackingParameter->getNoFish(); 
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
	//if (std::get<0>(poses).size() == _TrackedTrajectoryMajor->numberOfChildrean()) { //TODO hardcoded
		for (int i = 0; i < _TrackedTrajectoryMajor->numberOfChildrean(); i++) {
			TrackedTrajectory *t = dynamic_cast<TrackedTrajectory *>(_TrackedTrajectoryMajor->getChild(i));
			if (t) {

				TrackedElement *e = new TrackedElement(t, "n.a.", i);
				e->setFishPose(std::get<0>(poses)[i]);
				t->add(e);

				_ofs << i << "," << std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch()).count()
					<< std::get<0>(poses)[i].position_cm().x << "," << std::get<0>(poses)[i].position_cm().y << ","
					<< std::get<0>(poses)[i].orientation_deg() << "," << std::get<0>(poses)[i].orientation_rad() << ",";

			}
		}
	//}
	//else {
	//	std::cout << "Error: did not track expected size! Size is: " << std::get<1>(poses).size() << std::endl;
	//}
	_ofs << std::endl; //TODO extra module

	//Send forth whatever the user selected
	switch (_TrackingParameter->getSendImage()) {
	case 0: //Send none
		break;
	case 1:
		sendImage = images.find(std::string("Binarized"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Binarized"));
		break;
	case 2:
		sendImage = images.find(std::string("Eroded"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Eroded"));
		break;
	case 3:
		sendImage = images.find(std::string("Dilated"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Dilated"));
		break;
	case 4:
		sendImage = images.find(std::string("Foreground"))->second;
		Q_EMIT emitCvMatA(sendImage, QString("Foreground"));
		break;
	}

	//Draw stuff into the output
	/*std::shared_ptr<cv::Mat> rumgekritzel = std::make_shared<cv::Mat>();
	cvtColor(*dilated, *rumgekritzel, CV_GRAY2BGR);
	for (int i = 0; i < blobs.size(); i++) {
		cv::circle(*rumgekritzel, blobs[i].posPx(), 15, cv::Scalar(0, 255, 0), 3);
	}*/


	Q_EMIT emitTrackingDone();
}
