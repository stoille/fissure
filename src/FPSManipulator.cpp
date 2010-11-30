#include "FPSManipulator.h"
#include <limits>

namespace Fissure
{
	FPSManipulator::FPSManipulator() : FirstPersonManipulator()
	{
		setAcceleration( 0.25, false );
		setMaxVelocity( 0.25, false );
		setWheelMovement( 0.05, false );
		setAnimationTime( 0.2 );
	}
	
	FPSManipulator::~FPSManipulator()
	{}
	
	void FPSManipulator::moveForward( const double distance )
	{
		FirstPersonManipulator::moveForward(distance);
	}
	
	void FPSManipulator::moveRight( const double distance )
	{
		moveRight( _rotation, distance );
	}
	
	void FPSManipulator::moveRight( const Quat& rotation, const double distance )
	{
		_eye += rotation * Vec3d( distance, 0., 0.);
	}

	void FPSManipulator::moveUp( const double distance )
	{
		moveUp( _rotation, distance );
	}
	
	void FPSManipulator::moveUp( const Quat& rotation, const double distance )
	{
		_eye += rotation * Vec3d( 0., distance, 0.);
	}
	
	void FPSManipulator::rotateUp( const double distance )
	{
		// world up vector
		CoordinateFrame coordinateFrame = getCoordinateFrame( _eye );
		Vec3d localUp = getUpVector( coordinateFrame );
		 rotateYawPitch( _rotation, 0, distance, localUp );
	}
	void FPSManipulator::rotateDown( const double distance )
	{
		// world up vector
		CoordinateFrame coordinateFrame = getCoordinateFrame( _eye );
		Vec3d localUp = getUpVector( coordinateFrame );
		rotateYawPitch( _rotation, 0, -distance, localUp );
	}
	void FPSManipulator::rotateLeft( const double distance)
	{
		// world up vector
		CoordinateFrame coordinateFrame = getCoordinateFrame( _eye );
		Vec3d localUp = getUpVector( coordinateFrame );
		rotateYawPitch( _rotation, -distance, 0, localUp );
	}
	void FPSManipulator::rotateRight( const double distance )
	{
		// world up vector
		CoordinateFrame coordinateFrame = getCoordinateFrame( _eye );
		Vec3d localUp = getUpVector( coordinateFrame );
		rotateYawPitch( _rotation, distance, 0, localUp );
	}
}//namespace Fissure