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
		Firing(int firingId, int cycleNum, int axonalSomaId, int cellTypeId) :
			id(firingId)
			,cycleNum(cycleNum)
			,axonalSomaId(axonalSomaId)
			,cellTypeId(cellTypeId)
	{}
			int id, cycleNum, axonalSomaId, cellTypeId;
	};
	struct Synapse{
		Synapse() {}
		~Synapse() {}
		Synapse(int synapseId, int axonalSomaId, int dendriticSomaId, int synapseX, int synapseY, int synapseZ) :
			id(synapseId)
			,axonalSomaId(axonalSomaId)
			,dendriticSomaId(dendriticSomaId)
			,x(synapseX)
			,y(synapseY)
			,z(synapseZ)
		{}
		int id, axonalSomaId, dendriticSomaId, x, y, z;
	};

	struct SomaType{
		SomaType() {}
		~SomaType() {}
		SomaType(int cellTypeId, char somaType, float r, float g, float b, float a) :
			cellTypeId(cellTypeId)
			,somaType(somaType)
			,r(r)
			,g(g)
			,b(b)
			,a(a)
		{}
		int cellTypeId;
		char somaType;
		float r,g,b,a;
	};
	struct Soma{
		Soma() {}
		~Soma() {}
		Soma(int somaId, int cellTypeId, int somaX, int somaY, int somaZ) :
			id(somaId)
			,cellTypeId(cellTypeId)
			,x(somaX)
			,y(somaY)
			,z(somaZ) {}
		int id, cellTypeId, x, y, z;
	};

	typedef pair<int,SomaType> SomaTypePair;
	typedef map<int,SomaType> SomaTypeMap;

	typedef pair<int,Soma> SomaMapPair;
	typedef map<int,Soma> SomaMap;

	typedef pair<int,Synapse> SynapseMapPair;
	typedef map<int,Synapse> SynapseMap;

	typedef pair<int,Firing> FiringMapPair;
	typedef map<int,Firing> FiringMap;
}//namespace fissure
#endif