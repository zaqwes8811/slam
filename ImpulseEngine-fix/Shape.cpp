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

#include "Precompiled.h"

Body::Body( Shape *shape_, uint32 x, uint32 y ) :
		shape(shape_->Clone())
{
	shape->body = this;
	position.Set((float) x, (float) y);
	velocity.Set(0, 0);
	angularVelocity = 0;
	torque = 0;
	orient = Random(-PI, PI);
	force.Set(0, 0);
	staticFriction = 0.5f;
	dynamicFriction = 0.3f;
	restitution = 0.2f;
	shape->Initialize();
	r = Random(0.2f, 1.0f);
	g = Random(0.2f, 1.0f);
	b = Random(0.2f, 1.0f);
}

void Body::SetOrient( float radians )
{
	orient = radians;
	shape->SetOrient(radians);
}

//=================================================

void PolygonShape::Draw( void ) const
{
	glColor3f(body->r, body->g, body->b);
	glBegin(GL_LINE_LOOP);
	for( uint32 i = 0; i < m_vertexCount; ++i ){
		vec2_ie v = body->position + u * m_vertices[i];
		glVertex2f(v.x, v.y);
	}
	glEnd();
}

void PolygonShape::ComputeMass( float density )
{
	// Calculate centroid and moment of interia
	vec2_ie c(0.0f, 0.0f); // centroid
	float area = 0.0f;
	float I = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	for( uint32 i1 = 0; i1 < m_vertexCount; ++i1 ){
		// Triangle vertices, third vertex implied as (0, 0)
		vec2_ie p1(m_vertices[i1]);
		uint32 i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
		vec2_ie p2(m_vertices[i2]);

		float D = Cross(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;

		// Use area to weight the centroid average, not just vertex position
		c += triangleArea * k_inv3 * (p1 + p2);

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}

	c *= 1.0f / area;

	// Translate vertices to centroid (make the centroid (0, 0)
	// for the polygon in model space)
	// Not really necessary, but I like doing this anyway
	for( uint32 i = 0; i < m_vertexCount; ++i )
		m_vertices[i] -= c;

	body->m = density * area;
	body->im = (body->m) ? 1.0f / body->m : 0.0f;
	body->I = I * density;
	body->iI = body->I ? 1.0f / body->I : 0.0f;
}

//=================================================

void Circle::Draw( void ) const
{
	const uint32 k_segments = 20;

	// Render a circle with a bunch of lines
	glColor3f(body->r, body->g, body->b);
	glBegin(GL_LINE_LOOP);
	float theta = body->orient;
	float inc = PI * 2.0f / (float) k_segments;
	for( uint32 i = 0; i < k_segments; ++i ){
		theta += inc;
		vec2_ie p(std::cos(theta), std::sin(theta));
		p *= radius;
		p += body->position;
		glVertex2f(p.x, p.y);
	}
	glEnd();

	// Render line within circle so orientation is visible
	glBegin(GL_LINE_STRIP);
	vec2_ie r(0, 1.0f);
	float c = std::cos(body->orient);
	float s = std::sin(body->orient);
	r.Set(r.x * c - r.y * s, r.x * s + r.y * c);
	r *= radius;
	r = r + body->position;
	glVertex2f(body->position.x, body->position.y);
	glVertex2f(r.x, r.y);
	glEnd();
}

void Circle::ComputeMass( float density )
{
	body->m = PI * radius * radius * density;
	body->im = (body->m) ? 1.0f / body->m : 0.0f;
	body->I = body->m * radius * radius;
	body->iI = (body->I) ? 1.0f / body->I : 0.0f;
}
