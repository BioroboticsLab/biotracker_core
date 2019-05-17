#ifndef BIOTRACKER3MAINWINDOW_H
#define BIOTRACKER3MAINWINDOW_H

//#include <boost/filesystem.hpp>
#include "Interfaces/IView/IViewMainWindow.h"

// delete
#include "GLVideoView.h"
#include "CameraDevice.h"
#include "QPointer"
#include "QStringListModel"
#include "QGraphicsObject"
#include "View/GraphicsView.h"
#include "util/types.h"
#include "util/camera/base.h"
#include "SettingsWindow.h"
#include <QCloseEvent>
#include "QWizard"

#include "Utility/SwitchButton.h"
#include "Controller/IControllerCfg.h"

namespace Ui {
	class MainWindow;
}

class IController;


/**
* This class inherits IViewMainWindow.
* It is the canvas (GUI) for most of the view of other components.
* It also provides its own fuctionality, like an introduction dialog
*/
class MainWindow : public IViewMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0, IController* controller = 0, IModel* model = 0);
	~MainWindow();

	void addVideoControllWidget(IView* widget);
	void addVideoView(IView* videoView);
	void addTrackerParameterView(IView* parameter);
	void addCoreParameterView(IView* coreParameterView);
	//void addExperimentView(QWidget* experimentWidget);
	void addTrackerElementsView(IView *elemView);
	void addCoreElementsView(IView* coreView);
	void addNotificationBrowser(IView* notificationBrowser);
	void setTrackerList(QStringListModel* trackerList, QString current);
	void setCursorPositionLabel(QPoint pos);
	void setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);


	void activeTrackingCheckBox();
	void deactivateTrackingCheckBox();

	void closeEvent(QCloseEvent *event);

	void setupUpperToolBar();
	void setupVideoToolBar();

	//intro wizard
	void createIntroductionWizard();

	void saveDataToFile();

	void checkTrackerGroupBox();
	void checkMediaGroupBox();
	void activateTracking();

	void deactivateTracking();

	void resetTrackerViews();

Q_SIGNALS:
	void selectPlugin(QString ct);

	private Q_SLOTS:

	void toggleNoShowWiz(bool toggle);

	//menu->File
	void on_actionOpen_Video_triggered();
	
	void on_actionOpen_Video_batch_triggered();

	void on_actionLoad_Tracker_triggered();

	void on_actionOpen_Picture_triggered();

	void on_actionLoad_trackingdata_triggered();

	void on_actionSave_trackingdata_triggered();

	void on_actionQuit_triggered();

	void on_actionOpen_Camera_triggered();

	//menu->Edit

	void on_actionUndo_triggered();

	void on_actionRedo_triggered();

	void on_actionShowActionList_triggered();

	void on_actionSettings_triggered();

	//menu->View
		//menu->View->Toolbars
	void on_actionToggle_menu_toolbar_triggered();
	void on_actionToggle_view_toolbar_triggered();
	void on_actionToggle_compact_menu_toolbar_2_triggered();
	void on_actionToggle_video_toolbar_triggered();
	void on_actionToggle_fullscreen_triggered();

	//menu->Go to
	void on_actionOpen_Plugins_directory_triggered();
	void on_actionOpen_Track_directory_triggered();
	void on_actionOpen_Trial_directory_triggered();
	void on_actionOpen_Screenshot_directory_triggered();
	void on_actionOpen_Videos_directory_triggered();


	//view toolbar actions
	void on_actionAdd_Track_triggered();
	void on_actionDelete_selected_tracks_triggered();
	void on_actionSwap_ID_s_triggered();
	void on_actionSelect_all_triggered();
	void on_actionChange_the_border_color_triggered();
	void on_actionChange_the_fill_color_triggered();
	void on_actionAdd_label_Annotation_triggered();
	void on_actionAdd_rectangular_annotation_triggered();
	void on_actionAdd_arrow_annotation_triggered();
	void on_actionAdd_elliptical_annotation_triggered();
	void on_actionDelete_selected_Annotation_triggered();

	//menu->Help

	void on_actionAbout_triggered();

	void on_actionShortcuts_triggered();

	void on_actionUser_guide_triggered();

	void on_actionShow_introduction_triggered();
	//GUI

	void receiveSelectedCameraDevice(CameraConfiguration conf);

	void on_comboBox_TrackerSelect_currentIndexChanged(QString s);

	void receiveSetTracking(bool toggle);

	//hider
	void on_rightPanelViewControllerButton_clicked();
	void on_bottomPanelViewControllerButton_clicked();
	void on_actionBottom_panel_triggered(bool checked = false);
	void on_actionRight_panel_triggered(bool checked = false);
	//ui signals
	void on_toolBarMenu_visibilityChanged(bool visible);
	void on_toolBarTools_visibilityChanged(bool visible);
	void on_toolBarVideo_visibilityChanged(bool visible);

private:
	Ui::MainWindow* ui;								/**< ui file  */
	GraphicsView *m_graphView;						/**< media panel */
	QLabel *_cursorPosition;						/**< cursor Position in media panel */
	int _lastRightPanelWidth;						/**< lastRightPanelWidth for layouting */
	int _lastVideoWidgetWidth;						/**< lastVideoWidgetWidth for layouting */
	QLabel* cursorPositionLabel;					/**< cursonpositionlabel in status bar  */

	QPointer< CameraDevice > m_CameraDevice;		/**< pointer on camera device  */
	QPointer< SettingsWindow > m_SettingsWindow;	/**< settings window  */

	IView *_currentParameterView;					/**< current tracker paramter view  */
	IView *_currentCoreParameterView;				/**< current core parameter view  */
	QGraphicsObject *_currentElementView;			/**< current visualisation view of tracker  */
	QGraphicsObject *_currentCoreView;				/**< current visualisation view of core app  */
	//QWidget* _currentTrackerP;						/**< current tracker paramter view  */
	QString _previouslySelectedTracker;				/**< previous tracker  */

	SwitchButton* _trackerActivator;				/**< tracking switch  */
	QGroupBox* _mediaBox;							/**< tracker groupbox in menu toolbar  */
	QGroupBox* _trackerBox;							/**< media groupbox in menu toolbar   */

	QWizard* m_introWiz;							/**< introduction  wizard-dialog  */
	Config *_cfg;
};

#endif // BIOTRACKER3MAINWINDOW_H
