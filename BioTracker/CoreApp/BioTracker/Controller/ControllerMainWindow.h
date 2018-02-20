/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef CONTROLLERMAINWINDOW_H
#define CONTROLLERMAINWINDOW_H

#include "Interfaces/IController/IController.h"
#include "QPointer"
#include "QStringListModel"
#include "boost/filesystem.hpp"
#include <vector>
#include "util/types.h"

/**
 * The ControllerMainWindow class controlls the IView class MainWindow.
 * All user interactions done through the MainWindow class are delegated to the controllers of the component that concerns them.
 * Other components can place their IView Widgets on the MainWindow.
 */
class ControllerMainWindow : public IController {
    Q_OBJECT
  public:
    ControllerMainWindow(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    /**
     * Receives the a string containing the video file path from the MainWindow class. The string is then given to the ControllerPlayer class of the MediaPlayer-Component.
     */
    void loadVideo(QString str);
    /**
     * Receives the a string containing the Plugin file path from the MainWindow class. The string is then given to the BioTrackerContext class which will hand it over to the ControllerPlugin class.
     */
    void loadTracker(QString str);
    /**
     * Receives the a string containing the pictures file path from the MainWindow class. The string is then given to the ControllerPlayer class of the MediaPlayer-Component.
     */
    void loadPictures(std::vector<boost::filesystem::path> files);
    /**
     * Receives the a string containing the camera device number from the MainWindow class. The string is then given to the ControllerPlayer class of the MediaPlayer-Component.
     */
    void loadCameraDevice(CameraConfiguration conf);
    /**
     * Receives the command for activating the Tracking in a BioTracker Plugin from the MainWindow class. This command is given to the ControllerPlayer class of the MediaPlayer-component.
     */
    void activeTracking();
    /**
     * Receives the command for deactivating the Tracking in a BioTracker Plugin from the MainWindow class. This command is given to the ControllerPlayer class of the MediaPlayer-component.
     */
    void deactiveTrackring();
    /**
     * Receives a QStringListModel with the names of all currently loades BioTracker Plugins from the ControllerPlugin class.
     */
    void setTrackerList(QStringListModel* trackerList, QString current);
    /**
     * Embeds the Parameter Widget devined in the BioTracker Plugins into the MainWindow Widget.
     */
    void setTrackerParamterWidget(IView* widget);
	void setTrackerElementsWidget(IView *widget);
	void setCoreElementsWidget(IView* widget);
	void setNotificationBrowserWidget(IView * widget);

	void loadTrajectoryFile(std::string file);

    void deactiveTrackingCheckBox();
    void activeTrackingCheckBox();

	//Passes exit command to the GUI context
	void exit();

public:
    signals:
    void emitOnLoadPlugin(const std::string path);
    void emitOnLoadMedia(const std::string path);

    void emitPluginLoaded(const std::string path);
    void emitMediaLoaded(const std::string path);
    void emitTrackLoaded(const std::string path); 

	void emitUndoCommand();
	void emitRedoCommand();
	void emitClearUndoStack();
	void emitShowActionListCommand();

    //view toolbar actions
    void emitAddTrack();
    void emitDeleteSelectedTracks();
    void emitChangeColorBorder();
    void emitChangeColorFill();
    void emitAddLabelAnno();
    void emitAddRectAnno();
    void emitAddArrAnno();
    void emitAddEllAnno();
    void emitDelSelAnno();

	public slots:

    // IController interface
  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;

	private slots:
	void rcvSelectPlugin(QString plugin);
	void receiveCursorPosition(QPoint pos);

private:
	// Internal cleanup callback when a new video or imagestream is loaded.
	void onNewMediumLoaded(const std::string path="");
};

#endif // CONTROLLERMAINWINDOW_H
