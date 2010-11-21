#ifndef InitViwer_H
#define InitViewer_H

#include "InitViewerTypes.h"
#include <time.h>
#include <osg/Array>

namespace osgViewer
{
	class Viewer;
}

namespace osg
{
	class StateSet;
	class Geometry;
}

namespace Fissure {
/*
The init viewer reads in model initialization and firing data for the Init model.
It reads in somatype, soma, synapse and firing data in order - then allows one 
to visualize using OSG.

#Format of rows in the data file: 
#First the number of cell types and the identifying letter for each cell type is printed
#In the 2nd part of file, Soma coordinates get printed with: 
#SomaId 	 Cell Type Id 	  	 SomaX	SomaY	SomaZ
#In the 3rd part of file, after a line of -------------   Synapses get printed with: 
#AxonalSomaId 	 DendriticSomaId 	 SynapseX	SynapseY	SynapseZ
#In the 4th part of file, after a line of  --------------   Firing Data gets printed with: 
#Time_step(Cycle_number) 	 AxonalSomaId that fired 	 Cell Type Id
*/
class InitViewer
{
public:
	InitViewer();
	~InitViewer();

	void AddSomaType(char somaType);
	void AddSoma(int somaId, int cellTypeId, int somaX, int somaY, int somaZ);
	void AddSynapse(int axonalSomaId, int dendriticSomaId, int somaX, int somaY, int somaZ);
	void AddFiring(int cycleNum, int axonalSomaId, int cellTypeId);
	osg::Vec4f GetSomaTypeColor(int cellTypeId);
	static osg::StateSet* makeStateSet(float size);
	void StartViewer();
	//timing stuff
	void UpdateFiring(osgViewer::Viewer &viewer);
	void SetNumFiringCycles(int numFiringIntervals) { _numFiringCycles = numFiringIntervals; }
	void SetIntervalPeriod(double intervalPeriod) { _intervalPeriod = intervalPeriod; }
private:
	SomaTypeMap _somaTypes;
	SomaMap _somas;
	SynapseMap _synapses;
	FiringMap _firings;
	//firing stuff
	int _activeFiringCycle;
	int _numFiringCycles;
	time_t _time;
	double _intervalPeriod;
	bool _isFiringActive;
	osg::ref_ptr<osg::Geometry> _somaGeom;
	osg::ref_ptr<osg::Vec4Array> _somaColors;
};

}//namespace Fissure
#endif