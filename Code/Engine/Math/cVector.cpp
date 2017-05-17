// Header Files
//=============

#include "cVector.h"

#include <cassert>
#include <cmath>

// Static Data Initialization
//===========================

namespace
{
	const float s_epsilon = 1.0e-9f;
}

// Interface
//==========

// Addition
eae6320::Math::cVector eae6320::Math::cVector::operator +( const cVector& i_rhs ) const
{
	return cVector( x + i_rhs.x, y + i_rhs.y, z + i_rhs.z );
}
eae6320::Math::cVector& eae6320::Math::cVector::operator +=( const cVector& i_rhs )
{
	x += i_rhs.x;
	y += i_rhs.y;
	z += i_rhs.z;
	return *this;
}

// Subtraction / Negation
eae6320::Math::cVector eae6320::Math::cVector::operator -( const cVector& i_rhs ) const
{
	return cVector( x - i_rhs.x, y - i_rhs.y, z - i_rhs.z );
}
eae6320::Math::cVector& eae6320::Math::cVector::operator -=( const cVector& i_rhs )
{
	x -= i_rhs.x;
	y -= i_rhs.y;
	z -= i_rhs.z;
	return *this;
}
eae6320::Math::cVector eae6320::Math::cVector::operator -() const
{
	return cVector( -x, -y, -z );
}

// Multiplication
eae6320::Math::cVector eae6320::Math::cVector::operator *( const float i_rhs ) const
{
	return cVector( x * i_rhs, y * i_rhs, z * i_rhs );
}
eae6320::Math::cVector& eae6320::Math::cVector::operator *=( const float i_rhs )
{
	x *= i_rhs;
	y *= i_rhs;
	z *= i_rhs;
	return *this;
}
eae6320::Math::cVector operator *( const float i_lhs, const eae6320::Math::cVector& i_rhs )
{
	return i_rhs * i_lhs;
}

// Division
eae6320::Math::cVector eae6320::Math::cVector::operator /( const float i_rhs ) const
{
	assert( std::abs( i_rhs ) > s_epsilon );
	const float rhs_reciprocal = 1.0f / i_rhs;
	return cVector( x * rhs_reciprocal, y * rhs_reciprocal, z * rhs_reciprocal );
}
eae6320::Math::cVector& eae6320::Math::cVector::operator /=( const float i_rhs )
{
	assert( std::abs( i_rhs ) > s_epsilon );
	const float rhs_reciprocal = 1.0f / i_rhs;
	x *= rhs_reciprocal;
	y *= rhs_reciprocal;
	z *= rhs_reciprocal;
	return *this;
}

// Length / Normalization
float eae6320::Math::cVector::GetLength() const
{
	return std::sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
}
float eae6320::Math::cVector::Normalize()
{
	const float length = GetLength();
	assert( length > s_epsilon );
	operator /=( length );
	return length;
}
eae6320::Math::cVector eae6320::Math::cVector::CreateNormalized() const
{
	const float length = GetLength();
	assert( length > s_epsilon );
	const float length_reciprocal = 1.0f / length;
	return cVector( x * length_reciprocal, y * length_reciprocal, z * length_reciprocal );
}

// Products
float eae6320::Math::Dot( const cVector& i_lhs, const cVector& i_rhs )
{
	return ( i_lhs.x * i_rhs.x ) + ( i_lhs.y * i_rhs.y ) + ( i_lhs.z * i_rhs.z );
}
eae6320::Math::cVector eae6320::Math::Cross( const cVector& i_lhs, const cVector& i_rhs )
{
	return cVector(
		( i_lhs.y * i_rhs.z ) - ( i_lhs.z * i_rhs.y ),
		( i_lhs.z * i_rhs.x ) - ( i_lhs.x * i_rhs.z ),
		( i_lhs.x * i_rhs.y ) - ( i_lhs.y * i_rhs.x ) );
}

// Comparison
bool eae6320::Math::cVector::operator ==( const cVector& i_rhs ) const
{
	// Use & rather than && to prevent branches (all three comparisons will be evaluated)
	return ( x == i_rhs.x ) & ( y == i_rhs.y ) & ( z == i_rhs.z );
}
bool eae6320::Math::cVector::operator !=( const cVector& i_rhs ) const
{
	// Use | rather than || to prevent branches (all three comparisons will be evaluated)
	return ( x != i_rhs.x ) | ( y != i_rhs.y ) | ( z != i_rhs.z );
}

// Initialization / Shut Down
//---------------------------

eae6320::Math::cVector::cVector( const float i_x, const float i_y, const float i_z )
	:
	x( i_x ), y( i_y ), z( i_z )
{

}
