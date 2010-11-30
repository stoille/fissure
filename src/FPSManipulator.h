#ifndef FPSManipulator_H
#define FPSManipulator_H

#include <osgGA/FirstPersonManipulator>
using namespace osg;
using namespace osgGA;

namespace Fissure {

	class FPSManipulator : public FirstPersonManipulator {
	public:
		FPSManipulator();
		~FPSManipulator();
		
		void moveForward( const double distance );

		/// Move camera forward by distance parameter.
		void moveRight( const double distance );
		void moveRight( const Quat& rotation, const double distance );
		void moveUp( const double distance );
		void moveUp( const Quat& rotation, const double distance );
		
		void rotateUp( const double distance );
		void rotateDown( const double distance );
		void rotateLeft( const double distance);
		void rotateRight( const double distance );
	};
}//namespace Fissure

#endif