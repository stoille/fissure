#ifndef FissureViewerTypes_H
#define FissureViewerTypes_H

#include <map>
#include <vector>

using namespace std;

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
	};

	struct SomaType{
		SomaType() {}
		~SomaType() {}
		SomaType(char letter, float r, float g, float b, float a) :
			letter(letter)
			,r(r)
			,g(g)
			,b(b)
			,a(a)
		{}
		
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
	};

	typedef pair<unsigned,SomaType> SomaTypePair;
	typedef map<unsigned,SomaType> SomaTypeMap;

	typedef pair<unsigned,Soma> SomaMapPair;
	typedef map<unsigned,Soma> SomaMap;

	typedef pair<unsigned,Synapse> SynapseMapPair;
	typedef map<unsigned,Synapse> SynapseMap;

	typedef pair<unsigned,Firing> FiringMapPair;
	typedef map<unsigned,Firing> FiringMap;
}//namespace fissure
#endif