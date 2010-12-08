#ifndef FissureCommon_H
#define FissureCommon_H

#include "FissureTypes.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/StateSet>
#include <osg/ref_ptr>
#include <osg/Array>
#include <osgText/Text>
#include <osgGA/OrbitManipulator>
#include <time.h>
#include <vector>

using namespace osg;
using namespace osgText;

namespace Fissure{
		
	class SimInfo 
	{
	public:
		static SimInfo *Instance();
		SimInfo();
		~SimInfo();
		
		/* An enum that uses bits to represent active simulation states in one variable */
		enum SimState {
			STOPPED				= 0
			,RUNNING			= 1 
			,SELECTING_SPACE	= 1 << 1
			,FILTER_SOMA		= 1 << 2
			,FILTER_SYNAPSE		= 1 << 3
			,FILTER_LINES		= 1 << 4
			,MOTION_MODEL_ACTIVE= 1 << 5
			,FIRING_ACTIVE		= 1 << 6
			,SHOW_HELP			= 1 << 7
			,SHOW_INDEX			= 1 << 8
			,SHOW_HUD			= 1 << 9
		};
		unsigned gSimState;

		float gCamSpeed;
		float gCamRotSpeed;

		ref_ptr<Group> gRoot;

		unsigned gSelectedSomaId;
		SomaTypeMap gSomaTypeMap;
		SomaMap gSomaMap;
		ref_ptr<Geometry> gSomaGeom;
		ref_ptr<Geode> gSomaGeode;
		ref_ptr<Vec4Array> gSomaColors;

		unsigned gSelectedSynapseId;
		SynapseMap gSynapseMap;
		ref_ptr<Geometry> gSynapseGeom;
		ref_ptr<Geode> gSynapseGeode;
		ref_ptr<Vec4Array> gSynapseColors;

		ref_ptr<Geode> gLinesGeode;
		ref_ptr<Geometry> gLinesGeom;
		ref_ptr<Vec4Array> gLinesColors;

		FiringMap gFiringMap;
		unsigned gFiringTimeElapsed;

		ref_ptr<Vec4Array> gColorsArray;

		ref_ptr<Group> gHUD;
		
		ref_ptr<Geode> gHUD_PickedGeode;
		ref_ptr<Text> gHUD_PickedText;
		ref_ptr<Geode> gHUD_PickedTextGeode;
		
		ref_ptr<Geode> gHUD_HelpGeode;
		ref_ptr<Text> gHUD_HelpText;
		ref_ptr<Geode> gHUD_HelpTextGeode;
		
		ref_ptr<Geode> gHUD_TimeGeode;
		ref_ptr<Text> gHUD_TimeText;
		ref_ptr<Geode> gHUD_TimeTextGeode;
		
		
		std::vector< osg::ref_ptr<osg::Geode> > gHUD_LegendGeodes;
		std::vector< osg::ref_ptr<osgText::Text> > gHUD_LegendTexts;
		
		//orbit motion model stuff
		Vec3d gOrbitCenter;
		ref_ptr<osgGA::OrbitManipulator> gOrbitManipulator;
		
	private:
		static SimInfo* _instance;
	};
	
#define GLOBAL SimInfo::Instance()
}
#endif