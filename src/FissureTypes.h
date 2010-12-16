#ifndef FissureViewerTypes_H
#define FissureViewerTypes_H

#include <map>
#include <vector>
#include <osg/ref_ptr>
#include <osg/Drawable>

using namespace std;
using namespace osg;

namespace Fissure
{
	struct Firing{
		Firing() {}
		~Firing() {}
		Firing(unsigned firingId, unsigned cycleNum, unsigned axonalSomaId, unsigned cellTypeId) :
			id(firingId)
			,cycleNum(cycleNum)
			,axonalSomaId(axonalSomaId)
			,cellTypeId(cellTypeId)
	{}
		unsigned id;
		unsigned cycleNum;
		unsigned axonalSomaId;
		unsigned cellTypeId;
	};
	struct Synapse{
		Synapse() {}
		~Synapse() {}
		Synapse(unsigned synapseId, unsigned axonalSomaId, unsigned dendriticSomaId, float synapseX, float synapseY, float synapseZ) :
			id(synapseId)
			,axonalSomaId(axonalSomaId)
			,dendriticSomaId(dendriticSomaId)
			,x(synapseX)
			,y(synapseY)
			,z(synapseZ)
		{}
		unsigned id;
		unsigned axonalSomaId;
		unsigned dendriticSomaId;
		float x;
		float y;
		float z;
		ref_ptr<Drawable> drawable;
	};

	struct SomaType{
		SomaType() {}
		~SomaType() {}
		SomaType(unsigned somaTypeId, char letter, float r, float g, float b, float a) :
			id(somaTypeId)
			,letter(letter)
			,r(r)
			,g(g)
			,b(b)
			,a(a)
		{}
		unsigned id;
		char letter;
		float r;
		float g;
		float b;
		float a;
	};
	struct Soma{
		Soma() {}
		~Soma() {}
		Soma(unsigned somaId, unsigned cellTypeId, float somaX, float somaY, float somaZ) :
			id(somaId)
			,cellTypeId(cellTypeId)
			,x(somaX)
			,y(somaY)
			,z(somaZ) {}
		unsigned id;
		unsigned cellTypeId;
		float x;
		float y; 
		float z;
		vector<unsigned> axons;
		vector<unsigned> dendrites;
		ref_ptr<Drawable> drawable;
	};
	
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
	
	enum SelectedType {
		NONE_SELECTED,
		SOMA,
		SYNAPSE,
		AXON,
		DENDRITE,
		GUI
	};
	
	enum SomaVisibilityState{
		NONE		= 0
		,BODY		= 1
		,LINES		= 1 << 1
		,SYN		= 1 << 2
	};

	typedef pair<unsigned,SomaType> SomaTypeMapPair;
	typedef map<unsigned,SomaType> SomaTypeMap;

	typedef pair<unsigned,Soma> SomaMapPair;
	typedef map<unsigned,Soma> SomaMap;

	typedef pair<unsigned,Synapse> SynapseMapPair;
	typedef map<unsigned,Synapse> SynapseMap;

	typedef pair<unsigned,Firing> FiringMapPair;
	typedef map<unsigned,Firing> FiringMap;
	
	typedef pair<unsigned,unsigned> SomaVisibilityStatePair;
	typedef map<unsigned,unsigned> SomaVisibilityStateMap;
}//namespace fissure
#endif