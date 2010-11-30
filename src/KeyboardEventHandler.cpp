#include "KeyboardEventHandler.h"
#include "FPSManipulator.h"
#include <osgGA/GUIEventAdapter>
#include <osg/Geometry>

namespace Fissure 
{
	bool KeyboardEventHandler::handle(const GUIEventAdapter& ea, GUIActionAdapter&)
	{
		switch(ea.getEventType())
		{
			case(GUIEventAdapter::KEYDOWN):
			{
				switch(ea.getKey())
				{
					//FILTERING CONTROLS
					case 'n':
						ToggleVisibility(_somaGeode);
						break;
					case 'm':
						ToggleVisibility(_synapseGeode);
						break;
					case 'l':
						ToggleVisibility(_linesGeode);
						break;
					case 'v':
						FilterSomas();
						break;
					//MOTION CONTROLS
					//translations
					case 'w':
						_fpm->moveForward(_moveSpeed);
						break;
					case 'a':
						_fpm->moveRight(-_moveSpeed);
						break;
					case 's':
						_fpm->moveForward(-_moveSpeed);
						break;
					case 'd':
						_fpm->moveRight(_moveSpeed);
						break;
					case 'e':
						_fpm->moveUp(_moveSpeed);
						break;
					case 'x':
						_fpm->moveUp(-_moveSpeed);
						break;
					//rotations
					case osgGA::GUIEventAdapter::KEY_Up:
						_fpm->rotateUp(_rotateSpeed);
						break;
					case osgGA::GUIEventAdapter::KEY_Down:
						_fpm->rotateDown(_rotateSpeed);
						break;
					case osgGA::GUIEventAdapter::KEY_Left:
						_fpm->rotateLeft(_rotateSpeed);
						break;
					case osgGA::GUIEventAdapter::KEY_Right:
						_fpm->rotateRight(_rotateSpeed);
						break;	
					//speed control
					case 'p':
						_moveSpeed *=1.5;
						break;
					case 'o':
						_moveSpeed *= 0.5;
						break;
					case 'u':
						_rotateSpeed *= 1.5;
						break;
					case 'i':
						_rotateSpeed *= 0.5;
						break;
					default:
						break;
				}
			}
			default:
				break;
		}
		return false;
	}
	
	void KeyboardEventHandler::ToggleVisibility(osg::Geode* geode)
	{
		if (geode->getNodeMask()) {
			geode->setNodeMask(0);
		} else {
			geode->setNodeMask(0xFFFFFF);
		}

	}
	
	void KeyboardEventHandler::FilterSomas()
	{
		//fetch the soma geometry
		ref_ptr<Vec4Array> colors = static_cast<Vec4Array*>(_somaGeom->getColorArray());
		
		//show only the selected soma
		if(!_somaFiltered)
		{
			_somaFiltered = true;
			for(int i = 0; i < colors->size(); ++i)
				if(i != _selectedSoma->id)
					colors->at(i) = Vec4(colors->at(i).x(),colors->at(i).y(),colors->at(i).z(),0.0);
		}
		else {
			//show all somas
			_somaFiltered = false;
			for(int i = 0; i < colors->size(); ++i)
				colors->at(i) = Vec4(colors->at(i).x(),colors->at(i).y(),colors->at(i).z(),1);
		}
		_somaGeom->setColorArray(colors);

	}
}//namespace Fissure