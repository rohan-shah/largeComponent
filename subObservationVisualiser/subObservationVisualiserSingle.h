#ifndef LARGE_COMPONENT_SUB_OBSERVATION_VISUALISER_SINGLE_HEADER_GUARD
#define LARGE_COMPONENT_SUB_OBSERVATION_VISUALISER_SINGLE_HEADER_GUARD
#include <QMainWindow>
#include "context.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QStatusBar>
#include <QFrame>
#include <QHBoxLayout>
#include "observation.h"
#include "subObservationVisualiserBase.h"
#include "subObservationStatusBar.h"
#include "observation.h"
namespace largeComponent
{
	class subObservationVisualiserSingle : public QMainWindow
	{
		Q_OBJECT
	public:
		subObservationVisualiserSingle(const observationWithContext& obsWithContext, float pointSize);
		~subObservationVisualiserSingle();
		bool eventFilter(QObject* object, QEvent *event);
	public slots:
		void positionChanged(double x, double y);
	private:
		subObservationStatusBar* statusBar;
		subObservationVisualiserBase* base;
		const observationWithContext& obsWithContext;
	};
}
#endif
