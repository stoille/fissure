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
					case 'N':
						ToggleVisibility(_somaGeode);
						break;
					case 'n':
						ToggleVisibility(_synapseGeode);
						break;
					case 'l':
						ToggleVisibility(_linesGeode);
						break;
					case 'v':
						FilterSynapses();
						break;
					case 'V':
						FilterSomas();
						break;
					//MOTION CONTROLS
					//translations
					case 'w':
					case 'W':
						_fpm->moveForward(_moveSpeed);
						break;
					case 'a':
					case 'A':
						_fpm->moveRight(-_moveSpeed);
						break;
					case 's':
					case 'S':
						_fpm->moveForward(-_moveSpeed);
						break;
					case 'd':
					case 'D':
						_fpm->moveRight(_moveSpeed);
						break;
					case 'e':
					case 'E':
						_fpm->moveUp(_moveSpeed/2);
						break;
					case 'x':
					case 'X':
						_fpm->moveUp(-_moveSpeed/2);
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
					case 'P':
						_rotateSpeed *= 1.5;
						break;
					case 'O':
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
				if(i != _selectedSomaId)
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

	void KeyboardEventHandler::FilterSynapses()
	{
		//fetch the currently selected synapse
		Synapse &synapse = _synapseMap[_selectedSynapseId];

		//fetch the synapse geometry
		ref_ptr<Vec4Array> synapseColors = static_cast<Vec4Array*>(_synapseGeom->getColorArray());
		ref_ptr<Vec4Array> somaColors = static_cast<Vec4Array*>(_somaGeom->getColorArray());
		
		//show only the selected synapse and its connected somas
		if(!_synapseFiltered)
		{
			_synapseFiltered = true;
			for(int i = 0; i < synapseColors->size(); ++i)
				if(i != _selectedSynapseId)
					synapseColors->at(i) = Vec4(synapseColors->at(i).x(),synapseColors->at(i).y(),synapseColors->at(i).z(),0.0);
			for(int i = 0; i < somaColors->size(); ++i)
				if(i != synapse.axonalSomaId && i != synapse.dendriticSomaId)
					somaColors->at(i) = Vec4(somaColors->at(i).x(),somaColors->at(i).y(),somaColors->at(i).z(),0.0);
		}
		else {
			//show all synapses and somas
			_synapseFiltered = false;
			for(int i = 0; i < synapseColors->size(); ++i)
				synapseColors->at(i) = Vec4(synapseColors->at(i).x(),synapseColors->at(i).y(),synapseColors->at(i).z(),1);
			for(int i = 0; i < somaColors->size(); ++i)
				somaColors->at(i) = Vec4(somaColors->at(i).x(),somaColors->at(i).y(),somaColors->at(i).z(),1);
		}
		_synapseGeom->setColorArray(synapseColors);
		_somaGeom->setColorArray(somaColors);
	}
}//namespace Fissure