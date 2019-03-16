/*
 Copyright (c) 2013 Randy Gaul http://RandyGaul.net

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef SHAPE_H
#define SHAPE_H

#define MaxPolyVertexCount 64

#include "Clock.h"

struct Body;

struct Shape
{
	virtual ~Shape()
	{
	}

	enum Type
	{
		eCircle, ePoly, eCount, eMark
	};

	virtual Shape *Clone( void ) const = 0;
	virtual void Initialize( void ) = 0;
	virtual void ComputeMass( float density ) = 0;
	virtual void SetOrient( float radians ) = 0;
	virtual void Draw( void ) const = 0;
	virtual Type GetType( void ) const = 0;

	Body *body;

	// For circle shape
	float radius;

	// For Polygon shape
	Mat2 u; // Orientation matrix from model to world
};

struct Circle: public Shape
{
	Circle( float r )
	{
		radius = r;
	}

	Shape *Clone( void ) const
	{
		return new Circle(radius);
	}

	void Initialize( void )
	{
		ComputeMass(1.0f);
	}

	void ComputeMass( float density );

	void SetOrient( float radians )
	{
	}

	void Draw( void ) const;
	Type GetType( void ) const
	{
		return eCircle;
	}
};

struct PolygonShape: public Shape
{
	void Initialize( void )
	{
		ComputeMass(1.0f);
	}

	Shape *Clone( void ) const
	{
		PolygonShape *poly = new PolygonShape();
		poly->u = u;
		for( uint32 i = 0; i < m_vertexCount; ++i ){
			poly->m_vertices[i] = m_vertices[i];
			poly->m_normals[i] = m_normals[i];
		}
		poly->m_vertexCount = m_vertexCount;
		return poly;
	}

	void ComputeMass( float density );

	void SetOrient( float radians )
	{
		u.Set(radians);
	}

	void Draw( void ) const;

	Type GetType( void ) const
	{
		return ePoly;
	}

	// Half width and half height
	void SetBox( float hw, float hh )
	{
		m_vertexCount = 4;
		m_vertices[0].Set(-hw, -hh);
		m_vertices[1].Set(hw, -hh);
		m_vertices[2].Set(hw, hh);
		m_vertices[3].Set(-hw, hh);
		m_normals[0].Set(0.0f, -1.0f);
		m_normals[1].Set(1.0f, 0.0f);
		m_normals[2].Set(0.0f, 1.0f);
		m_normals[3].Set(-1.0f, 0.0f);
	}

	void Set( vec2_ie *vertices, uint32 count )
	{
		// No hulls with less than 3 vertices (ensure actual polygon)
		assert(count > 2 && count <= MaxPolyVertexCount);
		count = std::min((int32) count, MaxPolyVertexCount);

		// Find the right most point on the hull
		int32 rightMost = 0;
		float highestXCoord = vertices[0].x;
		for( uint32 i = 1; i < count; ++i ){
			float x = vertices[i].x;
			if( x > highestXCoord ){
				highestXCoord = x;
				rightMost = i;
			}

			// If matching x then take farthest negative y
			else if( x == highestXCoord )
				if( vertices[i].y < vertices[rightMost].y )
					rightMost = i;
		}

		int32 hull[MaxPolyVertexCount];
		int32 outCount = 0;
		int32 indexHull = rightMost;

		for( ;; ){
			hull[outCount] = indexHull;

			// Search for next index that wraps around the hull
			// by computing cross products to find the most counter-clockwise
			// vertex in the set, given the previos hull index
			int32 nextHullIndex = 0;
			for( int32 i = 1; i < (int32) count; ++i ){
				// Skip if same coordinate as we need three unique
				// points in the set to perform a cross product
				if( nextHullIndex == indexHull ){
					nextHullIndex = i;
					continue;
				}

				// Cross every set of three unique vertices
				// Record each counter clockwise third vertex and add
				// to the output hull
				// See : http://www.oocities.org/pcgpe/math2d.html
				vec2_ie e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
				vec2_ie e2 = vertices[i] - vertices[hull[outCount]];
				float c = Cross(e1, e2);
				if( c < 0.0f )
					nextHullIndex = i;

				// Cross product is zero then e vectors are on same line
				// therefor want to record vertex farthest along that line
				if( c == 0.0f && e2.LenSqr() > e1.LenSqr() )
					nextHullIndex = i;
			}

			++outCount;
			indexHull = nextHullIndex;

			// Conclude algorithm upon wrap-around
			if( nextHullIndex == rightMost ){
				m_vertexCount = outCount;
				break;
			}
		}

		// Copy vertices into shape's vertices
		for( uint32 i = 0; i < m_vertexCount; ++i )
			m_vertices[i] = vertices[hull[i]];

		// Compute face normals
		for( uint32 i1 = 0; i1 < m_vertexCount; ++i1 ){
			uint32 i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
			vec2_ie face = m_vertices[i2] - m_vertices[i1];

			// Ensure no zero-length edges, because that's bad
			assert(face.LenSqr() > EPSILON * EPSILON);

			// Calculate normal with 2D cross product between vector and scalar
			m_normals[i1] = vec2_ie(face.y, -face.x);
			m_normals[i1].Normalize();
		}
	}

	// The extreme point along a direction within a polygon
	vec2_ie GetSupport( const vec2_ie& dir )
	{
		float bestProjection = -FLT_MAX;
		vec2_ie bestVertex;

		for( uint32 i = 0; i < m_vertexCount; ++i ){
			vec2_ie v = m_vertices[i];
			float projection = Dot(v, dir);

			if( projection > bestProjection ){
				bestVertex = v;
				bestProjection = projection;
			}
		}

		return bestVertex;
	}

	uint32 m_vertexCount;
	vec2_ie m_vertices[MaxPolyVertexCount];
	vec2_ie m_normals[MaxPolyVertexCount];
};

//====================================================

// http://gamedev.tutsplus.com/tutorials/implementation/how-to-create-a-custom-2d-physics-engine-the-core-engine/
struct Body
{
	Body( Shape *shape_, uint32 x, uint32 y );

	void ApplyForce( const vec2_ie& f )
	{
		force += f;
	}

	void ApplyImpulse( const vec2_ie& impulse, const vec2_ie& contactVector )
	{
		velocity += im * impulse;
		angularVelocity += iI * Cross(contactVector, impulse);
	}

	void SetStatic( void )
	{
		I = 0.0f;
		iI = 0.0f;
		m = 0.0f;
		im = 0.0f;
	}

	void SetOrient( float radians );

	vec2_ie position;
	vec2_ie velocity;

	float angularVelocity;
	float torque;
	float orient; // radians

	vec2_ie force;

	// Set by shape
	float I;  // moment of inertia
	float iI; // inverse inertia
	float m;  // mass
	float im; // inverse masee

	// http://gamedev.tutsplus.com/tutorials/implementation/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table/
	float staticFriction;
	float dynamicFriction;
	float restitution;

	// Shape interface
	Shape *shape;

	// Store a color in RGB format
	float r, g, b;
};

//========================================================

//glm::vec

struct Mark
{
	Mark( vec2_ie pos )
	{
		this->pos = pos;
	}

	void draw()
	{
		int size = 3;

		glColor3f(1, 1, 0);
		glBegin (GL_LINE_LOOP);
		glVertex2f(pos.x, pos.y - size);
		glVertex2f(pos.x, pos.y + size);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(pos.x - size, pos.y);
		glVertex2f(pos.x + size, pos.y);
		glEnd();
	}

	vec2_ie pos;
};

#endif // SHAPE_H
