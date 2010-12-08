#ifndef KeyboardEventHandler_H
#define KeyboardEventHandler_H
#include "SimInfo.h"
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include "FPSManipulator.h"

namespace Fissure
{
	class KeyboardEventHandler : public GUIEventHandler
	{
	public:
		KeyboardEventHandler(FPSManipulator* fpm) :
		_fpm(fpm)
		{}
	    void ToggleVisibility(Geode* geode);
		void FilterSomas();
		void FilterSynapses();
		virtual bool handle(const GUIEventAdapter& ea, GUIActionAdapter&);
	private:
		FPSManipulator* _fpm;
	};
}//namespace Fissure
#endif