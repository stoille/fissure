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
			case osgGA::GUIEventAdapter::DOUBLECLICK:
			case osgGA::GUIEventAdapter::PUSH:
			{
				osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
				if (view) pick(view,ea);
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
		if ((GLOBAL->gSimState & RUNNING) && picker->containsIntersections())
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
					//if neuron (soma) selected
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
							//if(GLOBAL->gSelectedSomaId == somaId)
							//	GLOBAL->SetCenter();
							GLOBAL->gSelectedSomaId = somaId;
							GLOBAL->gSelectedType = SOMA;
							gdlist += os.str();
						}
						GLOBAL->gHUD_PickedText->setText(gdlist);
					}
					//if synapse selected
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
							//if(GLOBAL->gSelectedSynapseId == synapseId)
							//	GLOBAL->SetCenter();
							GLOBAL->gSelectedSynapseId = synapseId;
							GLOBAL->gSelectedType = SYNAPSE;
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
							//show all things and reset their types
							//reset visiblity map
							for(SomaVisibilityStateMap::iterator iter = GLOBAL->gSomaVisibilityStateMap.begin();
								iter != GLOBAL->gSomaVisibilityStateMap.end();
								++iter)
							{
								(*iter).second = BODY|LINES|SYN;
							}
							//reset soma colors to normal
							for(unsigned i = 0; i < GLOBAL->gSomaColors->size(); ++i)
							{
								Soma& soma = GLOBAL->gSomaMap[i];
								GLOBAL->gSomaColors->at(i) = GLOBAL->GetSomaTypeColor(soma.cellTypeId);
							}
							//reset synapse colors to normal
							for(unsigned i = 0; i < GLOBAL->gSynapseColors->size(); ++i)
							{
								Synapse& synapse = GLOBAL->gSynapseMap[i];
								GLOBAL->gSynapseColors->at(i) = GLOBAL->DEFAULT_SYNAPSE_COLOR;
							}
							//reset lines to normal
							for(SomaMap::iterator smi = GLOBAL->gSomaMap.begin(); 
								smi != GLOBAL->gSomaMap.end(); 
								++smi)
							{
								SomaMapPair smp = *smi;
								Soma& soma = smp.second;
								//enable axon visiblity
								for (unsigned ai = 0; ai < soma.axons.size(); ++ai) 
								{
									unsigned synapseId = soma.axons[ai];
									Synapse& synapse = GLOBAL->gSynapseMap[synapseId];
									Soma& so = GLOBAL->gSomaMap[synapse.axonalSomaId];
									Vec4 c = GLOBAL->GetSomaTypeColor(so.cellTypeId);
									unsigned li = synapse.id*4; //line index
									//enable/disable line visiblity
									GLOBAL->gLinesColors->at(li) =
										GLOBAL->gLinesColors->at(li+1) = c;
								}
								//enable dendrite visiblity
								for (unsigned ai = 0; ai < soma.dendrites.size(); ++ai) 
								{
									unsigned synapseId = soma.dendrites[ai];
									Synapse& synapse = GLOBAL->gSynapseMap[synapseId];
									Soma& so = GLOBAL->gSomaMap[synapse.axonalSomaId];
									Vec4 c = GLOBAL->GetSomaTypeColor(so.cellTypeId);
									unsigned li = synapse.id*4; //line index
									//enable/disable line visiblity
									GLOBAL->gLinesColors->at(li+2) =
									GLOBAL->gLinesColors->at(li+3) = c;
								}
							}
							
							GLOBAL->gLinesGeom->setColorArray(GLOBAL->gLinesColors);
							GLOBAL->gSynapseGeom->setColorArray(GLOBAL->gSynapseColors);
							GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
						}
						else {
							//otherwise filter by type selected from legend
							unsigned somaTypeId;
							//get soma type id
							for(SomaTypeMap::iterator stmi = GLOBAL->gSomaTypeMap.begin(); 
								stmi != GLOBAL->gSomaTypeMap.end();
								++stmi)
							{
								SomaTypeMapPair stmp = *stmi;
								SomaType& somaType = stmp.second;
								if (somaType.letter == somaTypeLetter) {
									somaTypeId = somaType.id;
									break;
								}
							}
							//toggle the soma state visiblity 
							unsigned &somaVisState = GLOBAL->gSomaVisibilityStateMap[somaTypeId];
							if(somaVisState == NONE)
								somaVisState = BODY | LINES | SYN;
							else if( somaVisState == (BODY|LINES|SYN) )
									somaVisState = BODY | LINES;			
							else if( somaVisState == (BODY|LINES))
									somaVisState = BODY;
							else if( somaVisState == BODY)
								somaVisState = NONE;
							
							//show soma bodies if needed
							for(SomaMap::iterator smi = GLOBAL->gSomaMap.begin(); 
								smi != GLOBAL->gSomaMap.end(); 
								++smi)
							{
								SomaMapPair smp = *smi;
								Soma soma =  smp.second;
								//only change the selected soma types
								if(soma.cellTypeId == somaTypeId)
								{
									//enable/disable soma visiblity
									string somaName = soma.drawable->getName();
									if(somaVisState & BODY)
									{
										if(somaName.substr(0, 2) != "n_")
											soma.drawable->setName("n_"+somaName);
										GLOBAL->gSomaColors->at(soma.id) = GLOBAL->GetSomaTypeColor(soma.cellTypeId);
									}
									else {
										if(somaName.substr(0, 2) == "n_")
											soma.drawable->setName(somaName.substr(2, somaName.size()-2));
										GLOBAL->gSomaColors->at(soma.id) = Vec4(0,0,0,0);
									}
									
									//enable/disable axon visiblity
									for (unsigned ai = 0; ai < soma.axons.size(); ++ai) 
									{
										unsigned synapseId = soma.axons[ai];
										Synapse& synapse = GLOBAL->gSynapseMap[synapseId];
										Soma& so = GLOBAL->gSomaMap[synapse.axonalSomaId];
										Vec4 c = GLOBAL->GetSomaTypeColor(so.cellTypeId);
										unsigned li = synapse.id*4; //line index
										//enable/disable line visiblity
										if(somaVisState & LINES)
											GLOBAL->gLinesColors->at(li) =
												GLOBAL->gLinesColors->at(li+1) = c;
										else GLOBAL->gLinesColors->at(li) =
												GLOBAL->gLinesColors->at(li+1) = Vec4(0,0,0,0);
										//enable/disable synapse visiblity
										string synName = synapse.drawable->getName();
										if(somaVisState & SYN)
										{
											if (synName.substr(0, 2) != "s_") 
												synapse.drawable->setName("s_"+synName);
											GLOBAL->gSynapseColors->at(synapseId) = GLOBAL->DEFAULT_SYNAPSE_COLOR;
										}
										else {
											if (synName.substr(0, 2) == "s_")
												synapse.drawable->setName(synName.substr(2, synName.size()-2));
											GLOBAL->gSynapseColors->at(synapseId) = Vec4(0,0,0,0);
										}
									}
									//enable/disable dendrite visiblity
									for (unsigned ai = 0; ai < soma.dendrites.size(); ++ai) 
									{
										unsigned synapseId = soma.dendrites[ai];
										Synapse& synapse = GLOBAL->gSynapseMap[synapseId];
										Soma& so = GLOBAL->gSomaMap[synapse.dendriticSomaId];
										Vec4 c = GLOBAL->GetSomaTypeColor(so.cellTypeId);
										unsigned li = synapse.id*4; //line index
										//enable/disable line visiblity
										if(somaVisState & LINES)
											GLOBAL->gLinesColors->at(li+2) =
												GLOBAL->gLinesColors->at(li+3) = c;
										else GLOBAL->gLinesColors->at(li+2) =
												GLOBAL->gLinesColors->at(li+3) = Vec4(0,0,0,0);
										//enable/disable synapse visiblity
										string synName = synapse.drawable->getName();
										if(somaVisState & SYN)
										{
											if (synName.substr(0, 2) != "s_") 
												synapse.drawable->setName("s_"+synName);
											GLOBAL->gSynapseColors->at(synapseId) = GLOBAL->DEFAULT_SYNAPSE_COLOR;
										}
										else {
											if (synName.substr(0, 2) == "s_")
												synapse.drawable->setName(synName.substr(2, synName.size()-2));
											GLOBAL->gSynapseColors->at(synapseId) = Vec4(0,0,0,0);
										}
									}
								}
							}
							GLOBAL->gLinesGeom->setColorArray(GLOBAL->gLinesColors);
							GLOBAL->gSynapseGeom->setColorArray(GLOBAL->gSynapseColors);
							GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
						}
					}
					//only show the first hit for now
					break;
				}
			}
		}
	}	
}//namespace Fissure