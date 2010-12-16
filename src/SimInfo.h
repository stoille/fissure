#ifndef FissureCommon_H
#define FissureCommon_H

#include "FissureTypes.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/StateSet>
#include <osg/ref_ptr>
#include <osg/Array>
#include <osg/Vec4>
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
		
		unsigned gSimState;
		
		SelectedType gSelectedType;
		
		unsigned gSomaVisibilityState;

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
		
		SomaVisibilityStateMap gSomaVisibilityStateMap;
		
		//orbit motion model stuff
		Vec3d gOrbitCenter;
		ref_ptr<osgGA::OrbitManipulator> gOrbitManipulator;
		
		Vec4 GetSomaTypeColor(int cellTypeId)
		{
			SomaType &st = gSomaTypeMap[cellTypeId];
			return Vec4(st.r,st.g,st.b,st.a);
		}
		
		void SetCenter()
		{
			if(gSelectedType == SOMA)
			{
				Soma& soma = gSomaMap[gSelectedSomaId];
				gOrbitCenter = Vec3d(soma.x,soma.y,soma.z);
			}else if(gSelectedType == SYNAPSE){
				Synapse& synapse = gSynapseMap[gSelectedSynapseId];
				gOrbitCenter = Vec3d(synapse.x,synapse.y,synapse.z);
			}
			gOrbitManipulator->setCenter(gOrbitCenter);
		}
		
		Vec4 DEFAULT_SYNAPSE_COLOR;
		
	private:
		static SimInfo* _instance;
	};
	
#define GLOBAL SimInfo::Instance()
}
#endif