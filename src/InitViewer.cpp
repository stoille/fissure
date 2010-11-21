#include "InitViewer.h"
#include "Vec4Colors.h"
#include "KeyboardEventHandler.h"

#include <osg/PointSprite>
#include <osg/BlendFunc>
#include <osg/StateAttribute>
#include <osg/Point>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/GLExtensions>
#include <osg/TexEnv>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgUtil/Optimizer>

using namespace osg;

namespace Fissure
{
	ref_ptr<Vec4Array> colorsArray = MakeVec4Colors();
	const int SOMA_RADIUS = 20;
	const int SYNAPSE_RADIUS = 15;
	const Vec4 DEFAULT_SYNAPSE_COLOR(0,1,0,1);

	InitViewer::InitViewer() : 
		_numFiringCycles(0)
		,_intervalPeriod(0.5)
		,_activeFiringCycle(0)
		,_isFiringActive(true)
	{
		time(&_time);
	}
	InitViewer::~InitViewer() 
	{
	}

	void InitViewer::AddSomaType(char somaType)
	{
		Vec4 sc = colorsArray->at(_somaTypes.size());
		_somaTypes[_somaTypes.size()] = SomaType(_somaTypes.size(),somaType,sc.r(),sc.g(),sc.b(),sc.a());
	}

	void InitViewer::AddSoma(int somaId, int cellTypeId, int somaX, int somaY, int somaZ) 
	{
		_somas[somaId] = Soma(somaId,cellTypeId,somaX,somaY,somaZ);
	}
	
	void InitViewer::AddSynapse(int axonalSomaId, int dendriticSomaId, int somaX, int somaY, int somaZ) 
	{
		_synapses[_synapses.size()] = Synapse(_synapses.size(),axonalSomaId,dendriticSomaId,somaX,somaY,somaZ);
	}

	void InitViewer::AddFiring(int cycleNum, int axonalSomaId, int cellTypeId) 
	{
		_firings[_firings.size()] = Firing(_firings.size(),cycleNum,axonalSomaId,cellTypeId);
	}

	osg::Vec4 InitViewer::GetSomaTypeColor(int cellTypeId)
	{
		SomaType &st = _somaTypes[cellTypeId];
		return Vec4(st.r,st.g,st.b,st.a);
	}

	StateSet* InitViewer::makeStateSet(float size)
	{
		StateSet *set = new StateSet();

		/// Setup cool blending
		set->setMode(GL_BLEND, StateAttribute::ON);
		BlendFunc *fn = new BlendFunc();
		fn->setFunction(BlendFunc::SRC_ALPHA, BlendFunc::DST_ALPHA);
		set->setAttributeAndModes(fn, StateAttribute::ON);

		/// Setup the point sprites
		PointSprite *sprite = new PointSprite();
		set->setTextureAttributeAndModes(0, sprite, StateAttribute::ON);

		/// Give some size to the points to be able to see the sprite
		Point *point = new Point();
		point->setSize(size);
		set->setAttribute(point);

		/// Disable depth test to avoid sort problems and Lighting
		set->setMode(GL_DEPTH_TEST, StateAttribute::OFF);
		set->setMode(GL_LIGHTING, StateAttribute::OFF);

		/// The texture for the sprites
		Texture2D *tex = new Texture2D();
		tex->setImage(osgDB::readImageFile("../data/images/particle.rgb"));
		set->setTextureAttributeAndModes(0, tex, StateAttribute::ON);

		return set;
	}

	void InitViewer::StartViewer()
	{
		//create the main viewer and set the background color to white
		osgViewer::Viewer viewer;
		viewer.addEventHandler(new osgViewer::StatsHandler);
		viewer.addEventHandler(new osgViewer::WindowSizeHandler());
		viewer.addEventHandler(new KeyboardEventHandler(viewer.getCamera()));
		viewer.addEventHandler(new osgViewer::HelpHandler());
		viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getStateSet()));
		//viewer.getCamera()->setClearColor(Vec4(126.0/255.0,128.0/255.0,119.0/255.0,1.0));
		ref_ptr<Group> root = new Group();

		//Create each soma as a point and add to the scene
		ref_ptr<Geode> somaGeode = new Geode();
		_somaGeom = new Geometry();
		ref_ptr<Vec3Array> somaVertices = new Vec3Array();
		_somaColors = new Vec4Array();
		for(SomaMap::iterator smi = _somas.begin(); smi != _somas.end(); ++smi)
		{
			SomaMapPair smp = *smi;
			Soma &soma = smp.second;

			somaVertices->push_back(Vec3(soma.x,soma.y,soma.z) );
			_somaColors->push_back(GetSomaTypeColor(soma.cellTypeId));
		}
		_somaGeom->setVertexArray(somaVertices);
		_somaGeom->setColorArray(_somaColors);
		_somaGeom->setColorBinding(Geometry::BIND_PER_VERTEX);
		_somaGeom->addPrimitiveSet(new DrawArrays(PrimitiveSet::POINTS, 0, _somas.size()));
		_somaGeom->setStateSet(makeStateSet(SOMA_RADIUS));
		somaGeode->addDrawable(_somaGeom);
		root->addChild(somaGeode);

		//Create each synapse as a point and add to the scene
		ref_ptr<Geode> synapseGeode = new Geode();
		ref_ptr<Geometry> synapseGeom = new Geometry();
		ref_ptr<Vec3Array> synapseVertices = new Vec3Array();
		ref_ptr<Vec4Array> synapseColors = new Vec4Array();
		for(SynapseMap::iterator smi = _synapses.begin(); smi != _synapses.end(); ++smi)
		{
			SynapseMapPair smp = *smi;
			Synapse &synapse = smp.second;

			synapseVertices->push_back(Vec3(synapse.x,synapse.y,synapse.z) );
			synapseColors->push_back(DEFAULT_SYNAPSE_COLOR);
		}
		synapseGeom->setVertexArray(synapseVertices);
		synapseGeom->setColorArray(synapseColors);
		synapseGeom->setColorBinding(Geometry::BIND_OVERALL);
		synapseGeom->addPrimitiveSet(new DrawArrays(PrimitiveSet::POINTS, 0, _synapses.size()));
		synapseGeom->setStateSet(makeStateSet(SYNAPSE_RADIUS));
		synapseGeode->addDrawable(synapseGeom);
		root->addChild(synapseGeode);

		//setups connection lines between neurons
		ref_ptr<Geometry> synapseLineGeom = new Geometry();
		synapseGeode->addDrawable(synapseLineGeom);
		synapseGeode->addDrawable(synapseLineGeom);
		//setup the line width of the connextions and their state
		ref_ptr<StateSet> ss = synapseGeode->getOrCreateStateSet();
		ss->setAttributeAndModes(new LineWidth(1.0));
		ss->setAttributeAndModes(new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::CONSTANT_ALPHA));
		synapseGeode->setStateSet(ss);
		//create the line/point arrays
		ref_ptr<DrawArrays> synapseLineDrawArray = new DrawArrays(PrimitiveSet::LINES);
		synapseLineGeom->addPrimitiveSet(synapseLineDrawArray);
		ref_ptr<Vec3Array> synapseLines = new Vec3Array();
		synapseLineGeom->setVertexArray(synapseLines);
		//setup the synapse line colors
		ref_ptr<Vec4Array> synapseLineColors = new Vec4Array;
		synapseLineGeom->setColorArray(synapseLineColors);
		synapseLineGeom->setColorBinding(Geometry::BIND_PER_VERTEX);
		//traverse through all synapses and add their geometry
		for(SynapseMap::iterator sm = _synapses.begin(); sm != _synapses.end(); ++sm)
		{
			SynapseMapPair smp = *sm;
			Synapse &synapse = smp.second;

			//for each synapse add two lines to the related soma
			Soma &aSoma = _somas[synapse.axonalSomaId],
				 &dSoma = _somas[synapse.dendriticSomaId];
			synapseLines->push_back(Vec3(synapse.x,synapse.y,synapse.z));
			synapseLines->push_back(Vec3(aSoma.x,aSoma.y,aSoma.z));
			synapseLines->push_back(Vec3(synapse.x,synapse.y,synapse.z));
			synapseLines->push_back(Vec3(dSoma.x,dSoma.y,dSoma.z));

			//add color for each line - colored by soma the axon/dendrite belongs to
			synapseLineColors->push_back( GetSomaTypeColor(aSoma.cellTypeId) );
			synapseLineColors->push_back( GetSomaTypeColor(aSoma.cellTypeId) );
			synapseLineColors->push_back( GetSomaTypeColor(dSoma.cellTypeId) );
			synapseLineColors->push_back( GetSomaTypeColor(dSoma.cellTypeId) );
		}
		synapseLineDrawArray->set(PrimitiveSet::LINES, 0, synapseLines->size());

		// switch off lighting as we haven't assigned any normals.
		root->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);

		//optimize before showing to improve performance
		osgUtil::Optimizer optimizer;
		optimizer.optimize(root);

		//Set the scene data and enter the simulation loop.
		viewer.setSceneData( root );
		//viewer.run();
		viewer.setCameraManipulator(new osgGA::TrackballManipulator());
		viewer.realize();

		while( !viewer.done() )
		{
			viewer.frame();
			if(_isFiringActive)
				UpdateFiring(viewer);
		}
	}

	void InitViewer::UpdateFiring(osgViewer::Viewer &viewer)
	{
		//if the time passed since the last update passes the interval period, update
		time_t now;
		time(&now);
		if(difftime(now, _time) > _intervalPeriod)
		{
			//traverse through all the firings for each soma and excite (color) its point
			for(FiringMap::iterator fmi = _firings.begin(); fmi != _firings.end(); ++fmi)
			{
				FiringMapPair fmp = *fmi;
				Firing &firing = fmp.second;
				//get the excited soma and act on its color
				Soma &soma = _somas[firing.axonalSomaId];
				//switch the active on and the one just passed off
				if(firing.cycleNum == _activeFiringCycle)
				{
					_somaColors->at(soma.id) += Vec4(0.2,0.2,0.2,0.0);
				}
				else if(firing.cycleNum == _activeFiringCycle-1 ){
					_somaColors->at(soma.id) -= Vec4(0.2,0.2,0.2,0.0);
				}
			}
			_somaGeom->setColorArray(_somaColors);
			_somaGeom->setColorBinding(Geometry::BIND_PER_VERTEX);
			//update the time and the active firing cycle
			time(&_time);
			_activeFiringCycle += 1 % (_numFiringCycles-1);
		}
		viewer.requestRedraw();
	}
}//namespace Fissure