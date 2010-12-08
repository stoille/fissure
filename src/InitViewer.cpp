#include "InitViewer.h"

#include "SimInfo.h"
#include "KeyboardEventHandler.h"
#include "FPSManipulator.h"
#include "PickHandler.h"

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
#include <osg/ShapeDrawable>


#include <osgDB/ReadFile>

#include <osgGA/StateSetManipulator>
#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/OrbitManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/BlendFunc>

#include "SimInfo.h"
#include "Vec4Colors.h"

using namespace osgGA;

namespace Fissure
{
	const float WIDTH = 1280;
	const float HEIGHT = 1024;
	
	ref_ptr<Vec4Array> colorsArray = MakeVec4Colors();
	const int SOMA_RADIUS = 20;
	const int SYNAPSE_RADIUS = 15;
	const Vec4 DEFAULT_SYNAPSE_COLOR(0,1,0,1);

	class ForceCullCallback : public Drawable::CullCallback 
	{ 
	public: 
        virtual bool cull(NodeVisitor*, Drawable*, State*) const 
        { 
            return true; 
        }
		
	}; 

	InitViewer::InitViewer() : 
		_numFiringCycles(0)
		,_intervalPeriod(0.5)
		,_activeFiringCycle(0)
	{
		time(&_time);
	}
	InitViewer::~InitViewer() 
	{
	}

	void InitViewer::AddSomaType(char somaType)
	{
		Vec4 sc = colorsArray->at( GLOBAL->gSomaTypeMap.size() );
		GLOBAL->gSomaTypeMap[GLOBAL->gSomaTypeMap.size()] = SomaType(somaType,sc.r(),sc.g(),sc.b(),sc.a());
	}

	void InitViewer::AddSoma(int somaId, int cellTypeId, int somaX, int somaY, int somaZ) 
	{
		GLOBAL->gSomaMap[somaId] = Soma(somaId,cellTypeId,somaX,somaY,somaZ);
	}
	
	void InitViewer::AddSynapse(int axonalSomaId, int dendriticSomaId, int somaX, int somaY, int somaZ) 
	{
		GLOBAL->gSynapseMap[GLOBAL->gSynapseMap.size()] = Synapse(GLOBAL->gSynapseMap.size(),axonalSomaId,dendriticSomaId,somaX,somaY,somaZ);
	}

	void InitViewer::AddFiring(int cycleNum, int axonalSomaId, int cellTypeId) 
	{
		GLOBAL->gFiringMap[GLOBAL->gFiringMap.size()] = Firing(GLOBAL->gFiringMap.size(),cycleNum,axonalSomaId,cellTypeId);
	}

	Vec4 InitViewer::GetSomaTypeColor(int cellTypeId)
	{
		SomaType &st =GLOBAL->gSomaTypeMap[cellTypeId];
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
	
	Node* createHUD()
	{
		
		// create the hud. derived from osgHud.cpp
		// adds a set of quads, each in a separate Geode - which can be picked individually
		// eg to be used as a menuing/help system!
		// Can pick texts too!

		Camera* hudCamera = new Camera;
		hudCamera->setReferenceFrame(Transform::ABSOLUTE_RF);
		hudCamera->setProjectionMatrixAsOrtho2D(0,WIDTH,0,HEIGHT);
		hudCamera->setViewMatrix(Matrix::identity());
		hudCamera->setRenderOrder(Camera::POST_RENDER);
		hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
		
		std::string timesFont("fonts/times->ttf");
		
		// turn lighting off for the text and disable depth test to ensure its always ontop.
		Vec3 position(WIDTH - 200,0,0);
		Vec3 dy(0,130,0);
		Vec3 dx(200,0,0);
//BOTTOM RIGHT - picker detail panel
		//background rectangle
		{
			
			GLOBAL->gHUD_PickedGeode = new Geode();
			StateSet* stateset = GLOBAL->gHUD_PickedGeode->getOrCreateStateSet();
			Geometry *quad=new Geometry;
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			Vec3Array* vertices = new Vec3Array(4); // 1 quad
			Vec4Array* colors = new Vec4Array;
			colors = new Vec4Array;
			colors->push_back(Vec4(0.4,0.4,0.4,1.0));
			quad->setColorArray(colors);
			quad->setColorBinding(Geometry::BIND_PER_PRIMITIVE);
			(*vertices)[0]=position;
			(*vertices)[1]=position+dx;
			(*vertices)[2]=position+dx+dy;
			(*vertices)[3]=position+dy;
			quad->setVertexArray(vertices);
			quad->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
			GLOBAL->gHUD_PickedGeode->addDrawable(quad);
			GLOBAL->gHUD->addChild(GLOBAL->gHUD_PickedGeode);
		}  
		
		{ // this displays what has been selected
			GLOBAL->gHUD_PickedTextGeode = new Geode();
			StateSet* stateset = GLOBAL->gHUD_PickedTextGeode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			GLOBAL->gHUD_PickedTextGeode->setName("Picked text label");
			GLOBAL->gHUD_PickedTextGeode->addDrawable( GLOBAL->gHUD_PickedText );
			GLOBAL->gHUD->addChild(GLOBAL->gHUD_PickedTextGeode);
			
			GLOBAL->gHUD_PickedText->setCharacterSize(30.0f);
			GLOBAL->gHUD_PickedText->setFont(timesFont);
			GLOBAL->gHUD_PickedText->setText("Detail Panel");
			GLOBAL->gHUD_PickedText->setPosition(position + Vec3(0,120,0));
			GLOBAL->gHUD_PickedText->setDataVariance(Object::DYNAMIC);

		}    
//CENTER - Help panel
		//background blockout
		{
			position = Vec3(0,0,0);
			dy = Vec3(0.0f,HEIGHT,0.0f);
			dx = Vec3(WIDTH,0.0f,0.0f);
			GLOBAL->gHUD_HelpGeode = new Geode();
			StateSet* stateset = GLOBAL->gHUD_HelpGeode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			stateset->setMode(GL_BLEND, BlendFunc::CONSTANT_ALPHA);
			Vec3Array* vertices = new Vec3Array(4); // 1 quad
			Vec4Array* colors = new Vec4Array;
			colors = new Vec4Array;
			colors->push_back(Vec4(0.4,0.4,0.4,0.4));
			Geometry *quad=new Geometry;
			quad->setColorArray(colors);
			quad->setColorBinding(Geometry::BIND_PER_PRIMITIVE);
			(*vertices)[0]=position;
			(*vertices)[1]=position+dx;
			(*vertices)[2]=position+dx+dy;
			(*vertices)[3]=position+dy;
			quad->setVertexArray(vertices);
			quad->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
			GLOBAL->gHUD_HelpGeode->addDrawable(quad);
			GLOBAL->gHUD_HelpGeode->setNodeMask(0);
			GLOBAL->gHUD->addChild(GLOBAL->gHUD_HelpGeode);
		} 
		//raised help box
		{
			position = Vec3(WIDTH*0.25,HEIGHT*0.15,0);
			dy = Vec3(0.0f,HEIGHT*0.7,0.0f);
			dx = Vec3(WIDTH*0.5,0.0f,0.0f);
			Vec3Array* vertices = new Vec3Array(4); // 1 quad
			Vec4Array* colors = new Vec4Array;
			colors = new Vec4Array;
			colors->push_back(Vec4(0.4,0.4,0.4,1.0));
			Geometry *quad=new Geometry;
			quad->setColorArray(colors);
			quad->setColorBinding(Geometry::BIND_PER_PRIMITIVE);
			(*vertices)[0]=position;
			(*vertices)[1]=position+dx;
			(*vertices)[2]=position+dx+dy;
			(*vertices)[3]=position+dy;
			quad->setVertexArray(vertices);
			quad->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
			GLOBAL->gHUD_HelpGeode->addDrawable(quad);
			GLOBAL->gHUD->addChild(GLOBAL->gHUD_HelpGeode);
		} 
		//help text
		{ // this displays what has been selected
			GLOBAL->gHUD_HelpTextGeode = new Geode();
			StateSet* stateset = GLOBAL->gHUD_HelpTextGeode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			GLOBAL->gHUD_HelpTextGeode->setName("");
			GLOBAL->gHUD_HelpTextGeode->addDrawable( GLOBAL->gHUD_HelpText );
			GLOBAL->gHUD_HelpTextGeode->setNodeMask(0);
			GLOBAL->gHUD->addChild(GLOBAL->gHUD_HelpTextGeode);
			
			GLOBAL->gHUD_HelpText->setCharacterSize(30.0f);
			GLOBAL->gHUD_HelpText->setFont(timesFont);
			GLOBAL->gHUD_HelpText->setText("Fissure is an OSG based visualizer for the INIT / BOSS Brain\n"\
										   "simulation model developed at Stony Brook University. Below\n"\
										   "is a list of keys and their functionality.\n\n"\
										   "Key  -  Description\n"\
										   "------------------------------------------------------------\n"\
										   "1   - Switch to Fly Motion 1:\n"\
										   "        w,a,s,d       -       Forward,Left,Down,Right\n"\
										   "        Arrow Keys       -       Rotation\n"\
										   "        LClick + Drag       -       Looks Around\n"\
										   "-/= -   Increase/Decrease Camera Speed\n"\
										   "_/+ -   Increast/Decrease Camera Rotation Speed\n\n"\
										   "2   -  Switch to Orbit Motion 2:\n"\
										   "        LClick + Drag       -       Spins model about its center\n"\
										   "        RClick + Drag       -       Zoom in and out\n"\
										   "        MClick + Drag       -       Panning\n\n"\
										   "g/G -   Starts/Resets Firing\n"\
										   "n/N -   Toggle Synapse/Soma Visibility\n"\
										   "l   -   Filters Lines Visibility\n"
										   );
										   
			GLOBAL->gHUD_HelpText->setPosition(position + dy + Vec3(10,-30,0));
			GLOBAL->gHUD_HelpText->setDataVariance(Object::DYNAMIC);
			
		}   
//BOTTOM LEFT - soma filter
		position = Vec3(10,10,0);
		dy = Vec3(0,20,0);
		dx = Vec3(20,0,0);
		Vec3 delta(30,0,0); //this is the spacing between tiles
		
		for (int i = 0; i < GLOBAL->gSomaTypeMap.size() - 1; ++i)
		{
			
			SomaType &st = GLOBAL->gSomaTypeMap[i];
			
				
			//color box setup
			Geode* geode = new Geode();
			stringstream s;
			s<<"t_"<<st.letter;
			
			//set states
			StateSet* stateset = geode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			Vec3Array* vertices = new Vec3Array(4); // soma type # quad
			Vec4Array* colors = new Vec4Array;
			//create colored box
			Geometry *quad=new Geometry;
			colors->push_back(Vec4(st.r,st.g,st.b,1));
			quad->setColorArray(colors);
			quad->setColorBinding(Geometry::BIND_PER_PRIMITIVE);
			(*vertices)[0]=position;
			(*vertices)[1]=position+dx;
			(*vertices)[2]=position+dx+dy;
			(*vertices)[3]=position+dy;
			quad->setVertexArray(vertices);
			quad->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
			geode->addDrawable(quad);
			GLOBAL->gHUD_LegendGeodes.push_back(geode);
			GLOBAL->gHUD->addChild(geode);
			
			// text setup
			Text* text = new Text();
			text->setName(s.str());
			geode->addDrawable( text );
			text->setCharacterSize(30.0f);
			text->setFont(timesFont);
			stringstream ss;
			ss<<st.letter;
			text->setText(ss.str());
			text->setPosition(position + Vec3(5,5,0));
			text->setDataVariance(Object::DYNAMIC);
			
			//advance position 
			position += delta;
			
		} 
		//add a box after all of them for select all '*'
		{
			//color box setup
			Geode* geode = new Geode();
			//set states
			StateSet* stateset = geode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			Vec3Array* vertices = new Vec3Array(4); // soma type # quad
			Vec4Array* colors = new Vec4Array;
			//create colored box
			Geometry *quad=new Geometry;
			colors->push_back(Vec4(0,0,0,1));
			quad->setColorArray(colors);
			quad->setColorBinding(Geometry::BIND_PER_PRIMITIVE);
			(*vertices)[0]=position;
			(*vertices)[1]=position+dx;
			(*vertices)[2]=position+dx+dy;
			(*vertices)[3]=position+dy;
			quad->setVertexArray(vertices);
			quad->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
			geode->addDrawable(quad);
			GLOBAL->gHUD_LegendGeodes.push_back(geode);
			GLOBAL->gHUD->addChild(geode);
			
			// text setup
			Text* text = new Text();
			text->setName("t_*");
			geode->addDrawable( text );
			text->setCharacterSize(30.0f);
			text->setFont(timesFont);
			text->setText("*");
			text->setPosition(position + Vec3(5,5,0));
			text->setDataVariance(Object::DYNAMIC);
			
			//advance position 
			position += delta;
			
		} 
//TOP LEFT - firing time
		//background rectangle
		{
			 position = Vec3(0,HEIGHT,0);
			 dy = Vec3(0.0f,-50,0.0f);
			 dx = Vec3(150.0f,0.0f,0.0f);
			GLOBAL->gHUD_TimeGeode = new Geode();
			StateSet* stateset = GLOBAL->gHUD_TimeGeode->getOrCreateStateSet();
			Geometry *quad=new Geometry;
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			Vec3Array* vertices = new Vec3Array(4); // 1 quad
			Vec4Array* colors = new Vec4Array;
			colors = new Vec4Array;
			colors->push_back(Vec4(0.4,0.4,0.4,1.0));
			quad->setColorArray(colors);
			quad->setColorBinding(Geometry::BIND_PER_PRIMITIVE);
			(*vertices)[0]=position;
			(*vertices)[1]=position+dx;
			(*vertices)[2]=position+dx+dy;
			(*vertices)[3]=position+dy;
			quad->setVertexArray(vertices);
			quad->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));
			GLOBAL->gHUD_TimeGeode->addDrawable(quad);
			GLOBAL->gHUD->addChild(GLOBAL->gHUD_TimeGeode);
		} 
		
		{ // this displays what has been selected
			GLOBAL->gHUD_TimeTextGeode = new Geode();
			StateSet* stateset = GLOBAL->gHUD_TimeTextGeode->getOrCreateStateSet();
			stateset->setMode(GL_LIGHTING,StateAttribute::OFF);
			stateset->setMode(GL_DEPTH_TEST,StateAttribute::OFF);
			GLOBAL->gHUD_TimeTextGeode->setName("Time text Label");
			GLOBAL->gHUD_TimeTextGeode->addDrawable( GLOBAL->gHUD_TimeText );
			GLOBAL->gHUD->addChild(GLOBAL->gHUD_TimeTextGeode);
			
			GLOBAL->gHUD_TimeText->setCharacterSize(30.0f);
			GLOBAL->gHUD_TimeText->setFont(timesFont);
			GLOBAL->gHUD_TimeText->setText("Firing Time");
			GLOBAL->gHUD_TimeText->setPosition(position + Vec3(10,-30,0));
			GLOBAL->gHUD_TimeText->setDataVariance(Object::DYNAMIC);
			
		}   
    //finally add the hud geode to the scene
	hudCamera->addChild(GLOBAL->gHUD);
    return hudCamera;

	}

	void InitViewer::StartViewer()
	{
		
		

//SOMA SETUP
		//Create each soma as a point and add to the scene
		ref_ptr<Vec3Array> somaVertices = new Vec3Array();
		ForceCullCallback* cullCB = new ForceCullCallback(); //use this to make soma geometry invisible
		for(SomaMap::iterator smi =GLOBAL->gSomaMap.begin(); smi !=GLOBAL->gSomaMap.end(); ++smi)
		{
			SomaMapPair smp = *smi;
			Soma &soma = smp.second;

			somaVertices->push_back(Vec3(soma.x,soma.y,soma.z) );
			GLOBAL->gSomaColors->push_back(GetSomaTypeColor(soma.cellTypeId));
			
			//for picking
			ShapeDrawable *somaSphereDrawable = new ShapeDrawable(new Sphere(Vec3(soma.x,soma.y,soma.z),SOMA_RADIUS*2));
			somaSphereDrawable->setCullCallback(cullCB); 
			std::stringstream somaId;
			somaId<<soma.id;
			somaSphereDrawable->setName("n_"+somaId.str());
			GLOBAL->gSomaGeode->addDrawable(somaSphereDrawable);
		}
		GLOBAL->gSomaGeom->setVertexArray(somaVertices);
		GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
		GLOBAL->gSomaGeom->setColorBinding(Geometry::BIND_PER_VERTEX);
		GLOBAL->gSomaGeom->addPrimitiveSet(new DrawArrays(PrimitiveSet::POINTS, 0,GLOBAL->gSomaMap.size()));
		GLOBAL->gSomaGeom->setStateSet(makeStateSet(SOMA_RADIUS));
		GLOBAL->gSomaGeode->addDrawable(GLOBAL->gSomaGeom);
		GLOBAL->gRoot->addChild(GLOBAL->gSomaGeode);

//SYNAPSE SETUP
		//Create each synapse as a point and add to the scene
		ref_ptr<Vec3Array> synapseVertices = new Vec3Array();
		for(SynapseMap::iterator smi =GLOBAL->gSynapseMap.begin(); smi !=GLOBAL->gSynapseMap.end(); ++smi)
		{
			SynapseMapPair smp = *smi;
			Synapse &synapse = smp.second;

			synapseVertices->push_back(Vec3(synapse.x,synapse.y,synapse.z) );
			GLOBAL->gSynapseColors->push_back(DEFAULT_SYNAPSE_COLOR);

			//for picking
			ShapeDrawable *synapseSphereDrawable = new ShapeDrawable(new Sphere(Vec3(synapse.x,synapse.y,synapse.z),SOMA_RADIUS*2));
			synapseSphereDrawable->setCullCallback(cullCB); 
			std::stringstream synapseId;
			synapseId<<synapse.id;
			synapseSphereDrawable->setName("s_"+synapseId.str());
			GLOBAL->gSynapseGeode->addDrawable(synapseSphereDrawable);
		}
		GLOBAL->gSynapseGeom->setVertexArray(synapseVertices);
		GLOBAL->gSynapseGeom->setColorArray(GLOBAL->gSynapseColors);
		GLOBAL->gSynapseGeom->setColorBinding(Geometry::BIND_OVERALL);
		GLOBAL->gSynapseGeom->addPrimitiveSet(new DrawArrays(PrimitiveSet::POINTS, 0,GLOBAL->gSynapseMap.size()));
		GLOBAL->gSynapseGeom->setStateSet(makeStateSet(SYNAPSE_RADIUS));
		GLOBAL->gSynapseGeode->addDrawable(GLOBAL->gSynapseGeom);
		GLOBAL->gRoot->addChild(GLOBAL->gSynapseGeode);

//LINES_SETUP
		//setups connection lines between neurons
		GLOBAL->gRoot->addChild(GLOBAL->gLinesGeode);
		//setup the line width of the connections and their state
		ref_ptr<StateSet> ss = GLOBAL->gLinesGeode->getOrCreateStateSet();
		ss->setAttributeAndModes(new LineWidth(1.0));
		ss->setAttributeAndModes(new BlendFunc(BlendFunc::SRC_ALPHA, BlendFunc::CONSTANT_ALPHA));
		ss->setMode( GL_BLEND, StateAttribute::ON );
		GLOBAL->gLinesGeode->setStateSet(ss);
		//create the line/point arrays
		ref_ptr<DrawArrays> synapseLineDrawArray = new DrawArrays(PrimitiveSet::LINES);
		GLOBAL->gLinesGeom->addPrimitiveSet(synapseLineDrawArray);
		ref_ptr<Vec3Array> synapseLines = new Vec3Array();
		GLOBAL->gLinesGeom->setVertexArray(synapseLines);
		//setup the synapse line colors
		GLOBAL->gLinesGeom->setColorArray(GLOBAL->gLinesColors);
		GLOBAL->gLinesGeom->setColorBinding(Geometry::BIND_PER_VERTEX);
		//traverse through all synapses and add their geometry
		for(SynapseMap::iterator sm =GLOBAL->gSynapseMap.begin(); sm !=GLOBAL->gSynapseMap.end(); ++sm)
		{
			SynapseMapPair smp = *sm;
			Synapse &synapse = smp.second;

			//for each synapse add two lines to the related soma
			Soma &aSoma =GLOBAL->gSomaMap[synapse.axonalSomaId],
				 &dSoma =GLOBAL->gSomaMap[synapse.dendriticSomaId];
			synapseLines->push_back(Vec3(synapse.x,synapse.y,synapse.z));
			synapseLines->push_back(Vec3(aSoma.x,aSoma.y,aSoma.z));
			synapseLines->push_back(Vec3(synapse.x,synapse.y,synapse.z));
			synapseLines->push_back(Vec3(dSoma.x,dSoma.y,dSoma.z));

			//add color for each line - colored by soma the axon/dendrite belonGLOBAL->gS to
			GLOBAL->gLinesColors->push_back( GetSomaTypeColor(aSoma.cellTypeId) );
			GLOBAL->gLinesColors->push_back( GetSomaTypeColor(aSoma.cellTypeId) );
			GLOBAL->gLinesColors->push_back( GetSomaTypeColor(dSoma.cellTypeId) );
			GLOBAL->gLinesColors->push_back( GetSomaTypeColor(dSoma.cellTypeId) );
		}
		synapseLineDrawArray->set(PrimitiveSet::LINES, 0, synapseLines->size());
		GLOBAL->gLinesGeode->addDrawable(GLOBAL->gLinesGeom);

//ENGINE STATES AND OPTIMIZATION
		// switch off lighting as we haven't assigned any normals->
		GLOBAL->gRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);

		//optimize before showing to improve performance
		osgUtil::Optimizer optimizer;
		optimizer.optimize(GLOBAL->gRoot);

//SCENE SETUP AND INPUT
		//create the main viewer and set the background color to white
		osgViewer::Viewer viewer;
		viewer.addEventHandler(new osgViewer::WindowSizeHandler());
		//viewer.addEventHandler(new osgViewer::StatsHandler);
		//viewer.addEventHandler(new osgViewer::HelpHandler());
		//viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getStateSet()));
		//Set the scene data and enter the simulation loop.
		viewer.setSceneData( GLOBAL->gRoot );
		ref_ptr<KeySwitchMatrixManipulator> ksm = new KeySwitchMatrixManipulator();
		ref_ptr<FPSManipulator> fpm = new FPSManipulator();
		ksm->addNumberedMatrixManipulator(fpm);
		ksm->addNumberedMatrixManipulator(GLOBAL->gOrbitManipulator);
		viewer.setCameraManipulator(ksm);
		
		KeyboardEventHandler *keh = new KeyboardEventHandler(fpm);
		viewer.addEventHandler(keh);
		
		// add the picker stuff
		PickHandler *ph = new PickHandler();
		GLOBAL->gRoot->addChild(createHUD());
		viewer.addEventHandler(ph);
		
		//run main osg viewer
		viewer.realize();
		//render loop keeps track of firing activity
		while( !viewer.done() )
		{
			viewer.frame();
			//if the simstate is running and the firing is active
			if((GLOBAL->gSimState & SimInfo::RUNNING) && 
			   (GLOBAL->gSimState & SimInfo::FIRING_ACTIVE))
				UpdateFiring(viewer);
		}
	}

	void InitViewer::UpdateFiring(osgViewer::Viewer &viewer)
	{
		//if the time passed since the last update passes the interval period, update
		time_t now;
		time(&now);
		if(difftime(now, _time) >= _intervalPeriod)
		{
			//update the firing time that has been elapsed
			if(GLOBAL->gFiringTimeElapsed > _numFiringCycles || 
			   GLOBAL->gFiringTimeElapsed == 0)
			{
				GLOBAL->gFiringTimeElapsed = 0;
				_activeFiringCycle = 0;
			}
			stringstream tt;
			tt<< "Cycle #"<<GLOBAL->gFiringTimeElapsed<<" / "<<_numFiringCycles;
			GLOBAL->gHUD_TimeText->setText( tt.str() );
				
			
			//traverse through all the firinGLOBAL->gS for each soma and excite (color) its point
			for(FiringMap::iterator fmi = GLOBAL->gFiringMap.begin(); fmi != GLOBAL->gFiringMap.end(); ++fmi)
			{
				FiringMapPair fmp = *fmi;
				Firing &firing = fmp.second;
				//get the excited soma and act on its color
				Soma &soma =GLOBAL->gSomaMap[firing.axonalSomaId];
				//switch the active on and the one just passed off
				if(firing.cycleNum == _activeFiringCycle)
				{
					GLOBAL->gSomaColors->at(soma.id) += Vec4(0.2,0.2,0.2,0.0);
				}
				else if(firing.cycleNum == _activeFiringCycle-1 ){
					GLOBAL->gSomaColors->at(soma.id) -= Vec4(0.2,0.2,0.2,0.0);
				}
			}
			GLOBAL->gSomaGeom->setColorArray(GLOBAL->gSomaColors);
			GLOBAL->gSomaGeom->setColorBinding(Geometry::BIND_PER_VERTEX);
			//update the time and the active firing cycle
			time(&_time);
			_activeFiringCycle += 1 % (_numFiringCycles-1);
			++GLOBAL->gFiringTimeElapsed;
		}
		viewer.requestRedraw();
	}
}//namespace Fissure