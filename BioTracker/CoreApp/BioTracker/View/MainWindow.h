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
#include "SettingsWindow.h"
#include <QCloseEvent>

#include "Utility/SwitchButton.h"

namespace Ui {
class MainWindow;
}

class IController;
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
    void checkTrackerGroupBox();
    void checkMediaGroupBox();
    void activateTracking();

    void deactivateTracking();

Q_SIGNALS:
	void selectPlugin(QString ct);

  private Q_SLOTS:
//menu->File
    void on_actionOpen_Video_triggered();

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

  private:
    Ui::MainWindow* ui;
	GraphicsView *m_graphView;
    QLabel *_cursorPosition;
    int _lastRightPanelWidth;
    int _lastVideoWidgetWidth;
    QLabel* cursorPositionLabel;

	QPointer< CameraDevice > m_CameraDevice;
	QPointer< SettingsWindow > m_SettingsWindow;

	IView *_currentParameterView;
	IView *_currentCoreParameterView;
	QGraphicsObject *_currentElementView;
	QGraphicsObject *_currentCoreView;
    QString _previouslySelectedTracker; 
    SwitchButton* _trackerActivator;

    QGroupBox* _mediaBox;
    QGroupBox* _trackerBox;
};

#endif // BIOTRACKER3MAINWINDOW_H
