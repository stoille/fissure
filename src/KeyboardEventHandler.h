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
							 osg::Geode* somaGeode,
							 osg::Geode* synapseGeode,
							 osg::Geode* linesGeode,
							 Soma* selectedSoma,
							 Geometry* somaGeom) :
		_fpm(fpm)
		,_moveSpeed(300.0)
		,_rotateSpeed(0.02)
		,_somaGeode(somaGeode)
		,_synapseGeode(synapseGeode)
		,_linesGeode(linesGeode)
		,_selectedSoma(selectedSoma)
		,_somaFiltered(false)
		,_somaGeom(somaGeom)
		{}
	    void ToggleVisibility(osg::Geode* geode);
		void FilterSomas();
		virtual bool handle(const GUIEventAdapter& ea, GUIActionAdapter&);
	private:
		FPSManipulator* _fpm;
		double _moveSpeed;
		double _rotateSpeed;
		osg::Geode* _somaGeode;
		osg::Geode* _synapseGeode;
		osg::Geode* _linesGeode;
		Soma* _selectedSoma;
		Geometry* _somaGeom;
		bool _somaFiltered;
	};
}//namespace Fissure
#endif