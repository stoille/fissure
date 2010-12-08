#ifndef PickHandler_H
#define PickHandler_H

#include "SimInfo.h"
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgViewer/View>

namespace Fissure
{

	// class to handle events with a pick
	class PickHandler : public osgGA::GUIEventHandler {
	public: 
		
		PickHandler()
		{
		}
		
		~PickHandler() {}
		
		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		
		virtual void pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea);
	};

} //namespace fissure
#endif