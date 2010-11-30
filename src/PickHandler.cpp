#include "PickHandler.h"

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
		if (picker->containsIntersections())
		{
			intersections = picker->getIntersections();

			if (view->computeIntersections(x,y,intersections))
			{
				for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
					hitr != intersections.end();
					++hitr)
				{
					string hitType = hitr->drawable->getName().substr(0,2);
					string hitName = hitr->drawable->getName().substr(2);
					//if neuron selected
					if(hitType == "n_")
					{
						stringstream s(hitName);
						int somaId;
						s>>somaId;
						Soma & soma = _somaMap[somaId];

						std::ostringstream os;
						os<<"Soma Id: "<<soma.id<<""<<std::endl;
						os<<"cellTypeId: "<<soma.cellTypeId<<std::endl;
						os<<"location ("<< soma.x<<","<<soma.y<<","<<soma.z<<")"<<std::endl;
						_selectedSomaId = somaId;
						gdlist += os.str();
					}
					//if soma selected
					else if(hitType == "s_")
					{
						stringstream s(hitName);
						int synapseId;
						s>>synapseId;
						Synapse &synapse = _synapseMap[synapseId];

						std::ostringstream os;
						os<<"Synapse Id: "<<synapse.id<<""<<std::endl;
						os<<"axonalSomaId: "<<synapse.axonalSomaId<<std::endl;
						os<<"dendriticSomaId: "<<synapse.dendriticSomaId<<std::endl;
						os<<"location ("<< synapse.x<<","<<synapse.y<<","<<synapse.z<<")"<<std::endl;
						_selectedSynapseId = synapseId;
						gdlist += os.str();
					}
					//only show the first hit for now
					break;
				}
			}
			setLabel(gdlist);
		}
	}	
}//namespace Fissure