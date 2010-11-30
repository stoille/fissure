#ifndef KeyboardEventHandler_H
#define KeyboardEventHandler_H
#include <osg/Geode>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include "FPSManipulator.h"
#include "InitViewerTypes.h"

namespace Fissure
{
	class KeyboardEventHandler : public GUIEventHandler
	{
	public:
		KeyboardEventHandler(FPSManipulator* fpm,
			SomaMap& somaMap,
			Geode* somaGeode,
			Geometry* somaGeom,
			int& selectedSomaId,
			SynapseMap& synapseMap,
			Geode* synapseGeode,
			Geometry* synapseGeom,
			int& selectedSynapseId,
			Geode* linesGeode,
			Geometry* linesGeom) :
		_fpm(fpm)
		,_moveSpeed(300.0)
		,_rotateSpeed(0.02)
		,_somaGeode(somaGeode)
		,_synapseGeode(synapseGeode)
		,_somaMap(somaMap)
		,_selectedSomaId(selectedSomaId)
		,_somaGeom(somaGeom)
		,_synapseMap(synapseMap)
		,_selectedSynapseId(selectedSynapseId)
		,_synapseGeom(synapseGeom)

		,_linesGeode(linesGeode)
		,_linesGeom(linesGeom)

		,_somaFiltered(false)
		,_synapseFiltered(false)
		{}
	    void ToggleVisibility(Geode* geode);
		void FilterSomas();
		void FilterSynapses();
		virtual bool handle(const GUIEventAdapter& ea, GUIActionAdapter&);
	private:
		FPSManipulator* _fpm;
		double _moveSpeed;
		double _rotateSpeed;

		SomaMap& _somaMap;
		Geode* _somaGeode;
		Geometry* _somaGeom;
		int &_selectedSomaId;
		bool _somaFiltered;

		SynapseMap& _synapseMap;
		Geode* _synapseGeode;
		Geometry* _synapseGeom;
		int& _selectedSynapseId;
		bool _synapseFiltered;

		Geode* _linesGeode;
		Geometry* _linesGeom;
	};
}//namespace Fissure
#endif