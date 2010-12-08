#include "PickHandler.h"
#include "SimInfo.h"

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osg/ShapeDrawable>

#include <sstream>

namespace Fissure
{
	
	bool PickHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
	{
		switch(ea.getEventType())
		{
			case(osgGA::GUIEventAdapter::PUSH):
			{
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				if (view) pick(view,ea);
				return false;
			}    
			case(osgGA::GUIEventAdapter::KEYDOWN):
			{
				if (ea.getKey()=='c')
				{        
					osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
					osg::ref_ptr<osgGA::GUIEventAdapter> event = new osgGA::GUIEventAdapter(ea);
					event->setX((ea.getXmin()+ea.getXmax())*0.5);
					event->setY((ea.getYmin()+ea.getYmax())*0.5);
					if (view) pick(view,*event);
				}
				return false;
			}    
			default:
				return false;
		}
	}
	
	void PickHandler::pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea)
	{
		osgUtil::LineSegmentIntersector::Intersections intersections;
		
		std::string gdlist="";
		float x = ea.getX();
		float y = ea.getY();

		osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
		osgUtil::IntersectionVisitor iv(picker.get());
		view->getCamera()->accept(iv);
		if ((GLOBAL->gSimState & SimInfo::RUNNING) && picker->containsIntersections())
		{
			intersections = picker->getIntersections();

			if (view->computeIntersections(x,y,intersections))
			{
				for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
					hitr != intersections.end();
					++hitr)
				{
					//the name of this hit is nothing, ignore it
					if(hitr->drawable->getName().length() == 0)
						continue;
					
					string hitType = hitr->drawable->getName().substr(0,2);
					string hitName = hitr->drawable->getName().substr(2);
					//if neuron selected
					if(hitType == "n_")
					{
						std::stringstream s(hitName);
						int somaId;
						s>>somaId;
						SomaMap::iterator smi = GLOBAL->gSomaMap.find(somaId);
						if(smi != GLOBAL->gSomaMap.end())
						{
							Soma & soma = smi->second;

							std::stringstream os;
							os<<"Soma Id: "<<soma.id<<""<<std::endl;
							os<<"cellTypeId: "<<GLOBAL->gSomaTypeMap[soma.cellTypeId].letter<<std::endl;
							os<<"location ("<< soma.x<<","<<soma.y<<","<<soma.z<<")"<<std::endl;
							GLOBAL->gSelectedSomaId = somaId;
							gdlist += os.str();
						}
						GLOBAL->gHUD_PickedText->setText(gdlist);
					}
					//if soma selected
					else if(hitType == "s_")
					{
						std::stringstream s(hitName);
						int synapseId;
						s>>synapseId;
						SynapseMap::iterator smi = GLOBAL->gSynapseMap.find(synapseId);
						if(smi != GLOBAL->gSynapseMap.end())
						{
							Synapse & synapse = smi->second;
							
							std::stringstream os;
							os<<"Synapse Id: "<<synapse.id<<""<<std::endl;
							os<<"axonalSomaId: "<<synapse.axonalSomaId<<std::endl;
							os<<"dendriticSomaId: "<<synapse.dendriticSomaId<<std::endl;
							os<<"location ("<< synapse.x<<","<<synapse.y<<","<<synapse.z<<")"<<std::endl;
							GLOBAL->gSelectedSynapseId = synapseId;
							gdlist += os.str();
						}
						GLOBAL->gHUD_PickedText->setText(gdlist);
					}
					//if soma legend filter selected
					else if(hitType == "t_")
					{
						stringstream s(hitName);
						char somaTypeLetter;
						s>>somaTypeLetter;
						
						//if the letter is a *, then reselect all the somas
						if (somaTypeLetter == '*') {
							//show all somas
							for(unsigned i = 0; i < GLOBAL->gSomaColors->size(); ++i)
								GLOBAL->gSomaColors->at(i) = Vec4(GLOBAL->gSomaColors->at(i).x(),GLOBAL->gSomaColors->at(i).y(),GLOBAL->gSomaColors->at(i).z(),1);
					
							GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
							GLOBAL->gOrbitCenter = Vec3d(0,0,0);
							GLOBAL->gOrbitManipulator->setCenter(GLOBAL->gOrbitCenter);
						}
						else {
							//otherwise filter by type selected from legend
							unsigned somaTypeId;
							//get soma type id
							for(int i = 0; i < GLOBAL->gSomaTypeMap.size(); ++i)
								if (GLOBAL->gSomaTypeMap[i].letter == somaTypeLetter) {
									somaTypeId = i;
									break;
								}
							//this is for calculating new orbit model center
							Vec3d newCenter(0,0,0);
							unsigned numCounted(0);
							
							//show only the selected soma types
							for(unsigned i = 0; i < GLOBAL->gSomaMap.size(); ++i)
							{
								Soma& soma = GLOBAL->gSomaMap[i];
								if(soma.cellTypeId == somaTypeId)
								{
									newCenter += Vec3d(soma.x,soma.y,soma.z);
									++numCounted;
									GLOBAL->gSomaColors->at(i) = 
									Vec4(GLOBAL->gSomaColors->at(i).x(),GLOBAL->gSomaColors->at(i).y(),GLOBAL->gSomaColors->at(i).z(),1.0);
								}
								else GLOBAL->gSomaColors->at(i) = 
										Vec4(GLOBAL->gSomaColors->at(i).x(),GLOBAL->gSomaColors->at(i).y(),GLOBAL->gSomaColors->at(i).z(),0.0);
								
								GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
								
								//set the new focal center based upon active somas
								GLOBAL->gOrbitCenter = newCenter / numCounted;
								GLOBAL->gOrbitManipulator->setCenter(GLOBAL->gOrbitCenter);
							}
						}
					}
					//only show the first hit for now
					break;
				}
			}
		}
	}	
}//namespace Fissure