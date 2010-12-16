#include "KeyboardEventHandler.h"
#include "SimInfo.h"

namespace Fissure 
{
	
	bool KeyboardEventHandler::handle(const GUIEventAdapter& ea, GUIActionAdapter&)
	{
		switch(ea.getEventType())
		{
			case GUIEventAdapter::KEYDOWN :
			{
				//to pull up help menu and pause running state
				if(ea.getKey() == 'h' || ea.getKey() == 'H')
				{
					if(GLOBAL->gSimState & RUNNING)
					{	//show help and pause
						GLOBAL->gHUD_HelpGeode->setNodeMask(0xFFFFFF);
						GLOBAL->gHUD_HelpTextGeode->setNodeMask(0xFFFFFF);
						GLOBAL->gSimState ^= RUNNING;
						GLOBAL->gHUD_TimeText->setText("Firing Paused");
					}
					else {
						GLOBAL->gSimState |= RUNNING;
						GLOBAL->gHUD_HelpGeode->setNodeMask(0);
						GLOBAL->gHUD_HelpTextGeode->setNodeMask(0);
					}
				}
				
				//only accept all other key input if running
				if(GLOBAL->gSimState & RUNNING)
				switch(ea.getKey())
				{
				
					//FILTERING CONTROLS
					case ( 'N' ) :
						ToggleVisibility(GLOBAL->gSomaGeode.get());
						break;
					case ( 'n' ) :
						ToggleVisibility(GLOBAL->gSynapseGeode.get());
						break;
					case ( 'l' ) :
						ToggleVisibility(GLOBAL->gLinesGeode.get());
						break;
					case ( 'v' ) :
						FilterSynapses();
						break;
					case ( 'V' ) :
						FilterSomas();
						break;
					//MOTION CONTROLS
					//translations
					case 'w' :
					case 'W' :
						_fpm->moveForward(GLOBAL->gCamSpeed);
						break;
					case 'a':
					case 'A':
						_fpm->moveRight(-GLOBAL->gCamSpeed);
						break;
					case 's' :
					case 'S' :
						_fpm->moveForward(-GLOBAL->gCamSpeed);
						break;
					case 'd' :
					case 'D' :
						_fpm->moveRight(GLOBAL->gCamSpeed);
						break;
					case 'e' : 
					case 'E' :
						_fpm->moveUp(GLOBAL->gCamSpeed/2);
						break;
					case 'x' : 
					case 'X' :
						_fpm->moveUp(-GLOBAL->gCamSpeed/2);
						break;
					//rotations
					case ( osgGA::GUIEventAdapter::KEY_Up ) :
						_fpm->rotateUp(GLOBAL->gCamRotSpeed);
						break;
					case ( osgGA::GUIEventAdapter::KEY_Down ) :
						_fpm->rotateDown(GLOBAL->gCamRotSpeed);
						break;
					case ( osgGA::GUIEventAdapter::KEY_Left ) :
						_fpm->rotateLeft(GLOBAL->gCamRotSpeed);
						break;	
					case ( osgGA::GUIEventAdapter::KEY_Right ) :
						_fpm->rotateRight(GLOBAL->gCamRotSpeed);
						break;		
					//speed control
					
					case ( '-' ) :
						GLOBAL->gCamSpeed *= 0.5;
						break;
					case ( '=' ) :
						GLOBAL->gCamSpeed *=1.5;
						break;
					case ( '_' ) :
						GLOBAL->gCamRotSpeed *= 0.5;
						break;
					case ( '+' ) :
						GLOBAL->gCamRotSpeed *= 1.5;
						break;
					//firing control
					case 'g' ://start/pause
						if(GLOBAL->gSimState & FIRING_ACTIVE)
						{
							GLOBAL->gSimState ^= FIRING_ACTIVE;
							GLOBAL->gHUD_TimeText->setText("Firing Paused");
						}
						else GLOBAL->gSimState |= FIRING_ACTIVE;
						break;
					case 'G' ://reset
						GLOBAL->gFiringTimeElapsed = 0;
						break;
					case 'c':
						GLOBAL->SetCenter();
						break;
					case 'C':
					case osgGA::GUIEventAdapter::KEY_Space :
						GLOBAL->gOrbitCenter = Vec3d(0,0,0);
						GLOBAL->gOrbitManipulator->setCenter(GLOBAL->gOrbitCenter);
						break;
					default:
						break;
				}
				break;
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
		//show only the selected soma
		if(GLOBAL->gSimState & ~FILTER_SOMA)
		{
			GLOBAL->gSimState |= FILTER_SOMA;
			for(unsigned i = 0; i < GLOBAL->gSomaColors->size(); ++i)
				if(i != GLOBAL->gSelectedSomaId)
					GLOBAL->gSomaColors->at(i) = Vec4(GLOBAL->gSomaColors->at(i).x(),GLOBAL->gSomaColors->at(i).y(),GLOBAL->gSomaColors->at(i).z(),0.0);
		}
		else {
			//show all somas
			GLOBAL->gSimState ^= FILTER_SOMA;
			
			for(unsigned i = 0; i < GLOBAL->gSomaColors->size(); ++i)
				GLOBAL->gSomaColors->at(i) = Vec4(GLOBAL->gSomaColors->at(i).x(),GLOBAL->gSomaColors->at(i).y(),GLOBAL->gSomaColors->at(i).z(),1);
		}
		GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
	}

	void KeyboardEventHandler::FilterSynapses()
	{
		//fetch the currently selected synapse
		Synapse &synapse = GLOBAL->gSynapseMap[GLOBAL->gSelectedSynapseId];

		//show only the selected synapse and its connected somas
		if(GLOBAL->gSimState & FILTER_SYNAPSE)
		{
			//filter the synapses
			GLOBAL->gSimState |= FILTER_SYNAPSE;
			
			for(unsigned i = 0; i < GLOBAL->gSynapseColors->size(); ++i)
				if(i != GLOBAL->gSelectedSynapseId)
					GLOBAL->gSynapseColors->at(i) = Vec4(GLOBAL->gSynapseColors->at(i).x(),GLOBAL->gSynapseColors->at(i).y(),GLOBAL->gSynapseColors->at(i).z(),0.0);
			for(unsigned i = 0; i < GLOBAL->gSomaColors->size(); ++i)
				if(i != synapse.axonalSomaId && i != synapse.dendriticSomaId)
					GLOBAL->gSomaColors->at(i) = Vec4(GLOBAL->gSomaColors->at(i).x(),GLOBAL->gSomaColors->at(i).y(),GLOBAL->gSomaColors->at(i).z(),0.0);
		}
		else {
			//show all synapses
			GLOBAL->gSimState ^= FILTER_SYNAPSE;
			
			for(unsigned i = 0; i < GLOBAL->gSynapseColors->size(); ++i)
				GLOBAL->gSynapseColors->at(i) = Vec4(GLOBAL->gSynapseColors->at(i).x(),GLOBAL->gSynapseColors->at(i).y(),GLOBAL->gSynapseColors->at(i).z(),1);
			for(unsigned i = 0; i < GLOBAL->gSomaColors->size(); ++i)
				GLOBAL->gSomaColors->at(i) = Vec4(GLOBAL->gSomaColors->at(i).x(),GLOBAL->gSomaColors->at(i).y(),GLOBAL->gSomaColors->at(i).z(),1);
		}
		GLOBAL->gSynapseGeom->setColorArray(GLOBAL->gSynapseColors);
		GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
	}
}//namespace Fissure