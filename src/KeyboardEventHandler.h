#ifndef KeyboardEventHandler_H
#define KeyboardEventHandler_H
#include <osg/Camera>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>

using namespace osg;
using namespace osgGA;

namespace Fissure
{
	class KeyboardEventHandler : public GUIEventHandler
	{
	public:
	    
		KeyboardEventHandler(ref_ptr<Camera> camera) :
		_camera(camera)
		{}
	    
		virtual bool handle(const GUIEventAdapter& ea, GUIActionAdapter&)
		{
			switch(ea.getEventType())
			{
				case(GUIEventAdapter::KEYDOWN):
				{
					ref_ptr<PositionAttitudeTransform> camXform;
					const Vec3d &camPosition = _camera->asPositionAttitudeTransform()->getPosition();
					switch(ea.getKey())
					{
					case 'l': 
						return true;
					default:
						break;
					}
				}
				default:
					break;
			}
			return false;
		}
	private:
		ref_ptr<Camera> _camera;
	        
	};
}//namespace Fissure
#endif