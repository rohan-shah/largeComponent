#ifndef SUB_OBSERVATION_VISUALISER_BASE_HEADER_GUARD
#define SUB_OBSERVATION_VISUALISER_BASE_HEADER_GUARD
#include <QFrame>
#include "context.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include "observation.h"
#include <QHBoxLayout>
namespace largeComponent
{
	enum visualisationMode
	{
		Normal_Mode, 
	};
	class subObservationVisualiserBase : public QFrame
	{
		Q_OBJECT
	public:
		subObservationVisualiserBase(const context& contextObj, float pointSize);
		~subObservationVisualiserBase();
		bool eventFilter(QObject* object, QEvent *event);
		void setObservation(const observation& subObs);
	signals:
		void positionChanged(double x, double y);
		void observationLeft();
		void observationRight();
		void observationUp();
		void observationDown();
	private:
		void initialiseControls();
		void addBackgroundRectangle();
		void constructGraphics(const observation& subObs);
		void updateGraphics();
		void constructStandardPoints(const observation& subObs);
		void constructStandardLines(const observation& subObs);

		float pointSize;

		QGraphicsScene* graphicsScene;
		QGraphicsView* graphicsView;
		QHBoxLayout* layout;
		float minX, maxX, minY, maxY;
		const context& contextObj;

		QGraphicsItemGroup* standardPointsItem;
		QGraphicsItemGroup* standardLinesItem;
		QGraphicsRectItem* backgroundItem;
	};
}
#endif
