#ifndef Vec4Colors_H
#define Vec4Colors_H

#include <osg/Array>
#include <osg/ref_ptr>
using namespace osg;
namespace Fissure
{
	static ref_ptr<Vec4Array> MakeVec4Colors()
	{
		ref_ptr<Vec4Array> colors = new Vec4Array();
		/* grabbed from:
		 http://www.opengl.org/discussion_boards/ubbthreads.php?ubb=showflat&Number=58674&page=2 
		 */
		colors->push_back(Vec4(0.62352f, 0.372549f, 0.623529f, 1.0f));
		colors->push_back(Vec4(0.647059f, 0.164706f, 0.164706f, 1.0f));
		colors->push_back(Vec4(0.372549f, 0.623529f, 0.623529f, 1.0f));
		colors->push_back(Vec4(1.0f, 0.498039f, 0.0f, 1.0f));
		colors->push_back(Vec4(0.258824f, 0.258824f, 0.435294f, 1.0f));
		colors->push_back(Vec4(0.184314f, 0.309804f, 0.184314f, 1.0f));
		colors->push_back(Vec4(0.309804f, 0.309804f, 0.184314f, 1.0f));
		colors->push_back(Vec4(0.6f, 0.196078f, 0.8f, 1.0f));
		colors->push_back(Vec4(0.419608f, 0.137255f, 0.556863f, 1.0f));
		colors->push_back(Vec4(0.184314f, 0.309804f, 0.309804f, 1.0f));
		colors->push_back(Vec4(0.184314f, 0.309804f, 0.309804f, 1.0f));
		colors->push_back(Vec4(0.439216f, 0.576471f, 0.858824f, 1.0f));
		colors->push_back(Vec4(0.556863f, 0.137255f, 0.137255f, 1.0f));
		colors->push_back(Vec4(0.137255f, 0.556863f, 0.137255f, 1.0f));
		colors->push_back(Vec4(0.8f, 0.498039f, 0.196078f, 1.0f));
		colors->push_back(Vec4(0.858824f, 0.858824f, 0.439216f, 1.0f));
		colors->push_back(Vec4(0.576471f, 0.858824f, 0.439216f, 1.0f));
		colors->push_back(Vec4(0.309804f, 0.184314f, 0.184314f, 1.0f));
		colors->push_back(Vec4(0.623529f, 0.623529f, 0.372549f, 1.0f));
		colors->push_back(Vec4(0.74902f, 0.847059f, 0.847059f, 1.0f));
		colors->push_back(Vec4(0.560784f, 0.560784f, 0.737255f, 1.0f));
		colors->push_back(Vec4(0.196078f, 0.8f, 0.196078f, 1.0f));
		colors->push_back(Vec4(0.556863f, 0.137255f, 0.419608f, 1.0f));
		colors->push_back(Vec4(0.196078f, 0.8f, 0.6f, 1.0f));
		colors->push_back(Vec4(0.196078f, 0.196078f, 0.8f, 1.0f));
		colors->push_back(Vec4(0.419608f, 0.556863f, 0.137255f, 1.0f));
		colors->push_back(Vec4(0.917647f, 0.917647f, 0.678431f, 1.0f));
		colors->push_back(Vec4(0.576471f, 0.439216f, 0.858824f, 1.0f));
		colors->push_back(Vec4(0.258824f, 0.435294f, 0.258824f, 1.0f));
		colors->push_back(Vec4(0.498039f, 1.0f, 1.0f, 1.0f));
		colors->push_back(Vec4(0.498039f, 1.0f, 1.0f, 1.0f));
		colors->push_back(Vec4(0.439216f, 0.858824f, 0.858824f, 1.0f));
		colors->push_back(Vec4(0.858824f, 0.439216f, 0.576471f, 1.0f));
		colors->push_back(Vec4(0.184314f, 0.184314f, 0.309804f, 1.0f));
		colors->push_back(Vec4(0.137255f, 0.137255f, 0.556863f, 1.0f));
		colors->push_back(Vec4(0.137255f, 0.137255f, 0.556863f, 1.0f));
		colors->push_back(Vec4(1.0f, 0.5f, 0.0f, 1.0f));
		colors->push_back(Vec4(1.0f, 0.25f, 1.0f, 1.0f));
		colors->push_back(Vec4(0.858824f, 0.439216f, 0.858824f, 1.0f));
		colors->push_back(Vec4(0.560784f, 0.737255f, 0.560784f, 1.0f));
		colors->push_back(Vec4(0.737255f, 0.560784f, 0.560784f, 1.0f));
		colors->push_back(Vec4(0.917647f, 0.678431f, 0.917647f, 1.0f));
		colors->push_back(Vec4(0.435294f, 0.258824f, 0.258824f, 1.0f));
		colors->push_back(Vec4(0.137255f, 0.556863f, 0.419608f, 1.0f));
		colors->push_back(Vec4(0.556863f, 0.419608f, 0.137255f, 1.0f));
		colors->push_back(Vec4(0.196078f, 0.6f, 0.8f, 1.0f));
		colors->push_back(Vec4(0.137255f, 0.419608f, 0.556863f, 1.0f));
		colors->push_back(Vec4(0.858824f, 0.576471f, 0.439216f, 1.0f));
		colors->push_back(Vec4(0.847059f, 0.74902f, 0.847059f, 1.0f));
		colors->push_back(Vec4(0.678431f, 0.917647f, 0.917647f, 1.0f));
		colors->push_back(Vec4(0.309804f, 0.184314f, 0.309804f, 1.0f));
		colors->push_back(Vec4(0.8f, 0.196078f, 0.6f, 1.0f));
		colors->push_back(Vec4(0.847059f, 0.847059f, 0.74902f, 1.0f));
		colors->push_back(Vec4(0.6f, 0.8f, 0.196078f, 1.0f));
		colors->push_back(Vec4(0.22f, 0.69f, 0.87f, 1.0f));
		colors->push_back(Vec4(0.35f, 0.35f, 0.67f, 1.0f));
		colors->push_back(Vec4(0.71f, 0.65f, 0.26f, 1.0f));
		colors->push_back(Vec4(0.72f, 0.45f, 0.20f, 1.0f));
		colors->push_back(Vec4(0.55f, 0.47f, 0.14f, 1.0f));
		colors->push_back(Vec4(0.65f, 0.49f, 0.24f, 1.0f));
		colors->push_back(Vec4(0.90f, 0.91f, 0.98f, 1.0f));
		colors->push_back(Vec4(0.85f, 0.85f, 0.10f, 1.0f));
		colors->push_back(Vec4(0.81f, 0.71f, 0.23f, 1.0f));
		colors->push_back(Vec4(0.82f, 0.57f, 0.46f, 1.0f));
		colors->push_back(Vec4(0.85f, 0.85f, 0.95f, 1.0f));
		colors->push_back(Vec4(1.00f, 0.43f, 0.78f, 1.0f));
		colors->push_back(Vec4(0.53f, 0.12f, 0.47f, 1.0f));
		colors->push_back(Vec4(0.30f, 0.30f, 1.00f, 1.0f));
		colors->push_back(Vec4(0.85f, 0.53f, 0.10f, 1.0f));
		colors->push_back(Vec4(0.89f, 0.47f, 0.20f, 1.0f));
		colors->push_back(Vec4(0.91f, 0.76f, 0.65f, 1.0f));
		colors->push_back(Vec4(0.65f, 0.50f, 0.39f, 1.0f));
		colors->push_back(Vec4(0.52f, 0.37f, 0.26f, 1.0f));
		colors->push_back(Vec4(1.00f, 0.11f, 0.68f, 1.0f));
		colors->push_back(Vec4(0.42f, 0.26f, 0.15f, 1.0f));
		colors->push_back(Vec4(0.36f, 0.20f, 0.09f, 1.0f));
		colors->push_back(Vec4(0.96f, 0.80f, 0.69f, 1.0f));
		colors->push_back(Vec4(0.92f, 0.78f, 0.62f, 1.0f));
		colors->push_back(Vec4(0.00f, 0.00f, 0.61f, 1.0f));
		colors->push_back(Vec4(0.35f, 0.16f, 0.14f, 1.0f));
		colors->push_back(Vec4(0.36f, 0.25f, 0.20f, 1.0f));
		colors->push_back(Vec4(0.59f, 0.41f, 0.31f, 1.0f));
		colors->push_back(Vec4(0.32f, 0.49f, 0.46f, 1.0f));
		colors->push_back(Vec4(0.29f, 0.46f, 0.43f, 1.0f));
		colors->push_back(Vec4(0.52f, 0.39f, 0.39f, 1.0f));
		colors->push_back(Vec4(0.13f, 0.37f, 0.31f, 1.0f));
		colors->push_back(Vec4(0.55f, 0.09f, 0.09f, 1.0f));
		
		return colors;
	}
}
#endif