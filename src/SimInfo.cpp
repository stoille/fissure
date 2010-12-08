#include "SimInfo.h"

#include "Vec4Colors.h"

namespace Fissure
{
	SimInfo* SimInfo::_instance;
	
	SimInfo::SimInfo() 
	{
		//gamestate
		gSimState = RUNNING | MOTION_MODEL_ACTIVE;
		//camera speeds
		gCamSpeed = 1;
		gCamRotSpeed = 20.0;
		//root scene
		gRoot = new Group();
		//soma info
		gSelectedSomaId = 0;
		gSomaGeom = new Geometry;
		gSomaGeode = new Geode;
		gSomaColors = new Vec4Array;
		//synapse info
		gSelectedSynapseId = 0;
		gSynapseGeom = new Geometry;
		gSynapseGeode = new Geode;
		gSynapseColors = new Vec4Array;
		//synapse connections
		gLinesGeode = new Geode;
		gLinesGeom = new Geometry;
		gLinesColors = new Vec4Array;		
		//hud components
		gHUD = new Group;
		gHUD_PickedGeode = new Geode;
		gHUD_PickedText = new Text;
		gHUD_PickedTextGeode = new Geode;
		gHUD_HelpGeode = new Geode;
		gHUD_HelpText = new Text;
		gHUD_HelpTextGeode = new Geode; 
		gHUD_TimeGeode = new Geode;
		gHUD_TimeText = new Text;
		gHUD_TimeTextGeode = new Geode;
		
		//general color array
		gColorsArray = new Vec4Array;
		
		//time init
		gFiringTimeElapsed = 0;
		
		//center of our orbit model
		gOrbitCenter = Vec3d(0,0,0);
		gOrbitManipulator = new osgGA::OrbitManipulator();
	}
	SimInfo::~SimInfo()
	{
	}
	SimInfo* SimInfo::Instance()
	{
		if(!_instance)
			_instance = new SimInfo();
		return _instance;
	}
}//namespace Fissure