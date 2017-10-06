#ifndef COREPARAMETERVIEW_H
#define COREPARAMETERVIEW_H

#include "Interfaces/IView/IViewWidget.h"
//TODO add default parameters accesible via config
//#include "Model/CoreViewParameter.h"

namespace Ui {
	class CoreParameterView;
}

class CoreParameterView : public IViewWidget
{
	Q_OBJECT

public:
	explicit CoreParameterView(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);
	~CoreParameterView();

	QWidget* getTrackerHook();

	private slots:
	void on_checkBoxEnableCoreView_stateChanged(int v);
	void on_comboBoxTracingStyle_currentIndexChanged(const QString & text);
	void on_spinBoxTracingHistoryLength_valueChanged(int i);
	void on_spinBoxTracingSteps_valueChanged(int i);
	void on_pushButtonSelectAll_clicked();
	void on_pushButtonColorChangeBorder_clicked();
	void on_pushButtonColorChangeBrush_clicked();

	void on_lineEditRectWidth_textChanged(QString s);
	void on_lineEditRectHeight_textChanged(QString s);
	void on_checkBoxDisplayTrackingArea_stateChanged(int v);
	void on_checkBoxDisplayRectification_stateChanged(int v);
	void on_checkboxTrackingAreaAsEllipse_stateChanged(int v);
    void on_pushButtonFinalizeExperiment_clicked();

public:
	signals :
            void emitSelectAllTracks();
            void emitViewSwitch(bool lever);
            void emitSelectAll();
			void emitColorChangeBorderAll();
			void emitColorChangeBrushAll();
			void emitTracingHistoryLength(int history);
			void emitTracingStyle(QString style);
			void emitTracingSteps(int steps);

			void emitRectDimensions(double w, double h);
			void emitDisplayTrackingArea(bool b);
			void emitDisplayRectification(bool b);
            void emitTrackingAreaAsEllipse(bool b);

            void emitFinalizeExperiment();
private:
	Ui::CoreParameterView *ui;

	bool m_viewSwitch;

	// IViewWidget interface
	public slots:

	void getNotified();
};

#endif // COREPARAMETERVIEW_H
