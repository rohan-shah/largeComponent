#ifndef LARGE_COMPONENT_SUBOBSERVATION_STATUS_BAR_HEADER_GUARD
#define LARGE_COMPONENT_SUBOBSERVATION_STATUS_BAR_HEADER_GUARD
#include <QStatusBar>
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <string>
namespace largeComponent
{
	class subObservationStatusBar : public QStatusBar
	{
		Q_OBJECT
	public:
		subObservationStatusBar(std::string text = "");
		void setReduced(bool reduced);
		void setPosition(double x, double y);
	private:
		QLabel* positionLabel;
		QLabel* reducedLabel;
		QFrame* frame;
		QHBoxLayout* layout;
		std::string text;
	};
}
#endif
