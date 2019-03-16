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
#include "TrafficModelHelper.h"
#include "TrafficModel.h"
#include "ImageZoneDescription.h"

#include <iostream>
#include <mathfu/utilities.h>
#include <mathfu/matrix.h>
#include <mathfu/vector.h>
#include <mathfu/glsl_mappings.h>

#include <memory>
#include <string>
#include <sstream>
#include <iostream>

// https://google.github.io/mathfu/mathfu_guide_matrices.html#mathfu_guide_matrices_declaration

#define ESC_KEY 27

using namespace std;
using namespace mathfu;

static mat2 g_p_mat;
Clock g_clock;
bool g_frameStepping = false;
bool g_canStep = false;
vec2 g_screen;
vec2 g_world;

static int fps = 22;
static TrafficModel* model = new TrafficModelEasyYDim(fps,
        Restrictions::dflt_count_x_gridpoints,
        Restrictions::dflt_count_y_gridpoints, 0xa123);
static BaseScene* g_scene = new SceneNoGravity(1.0f / 60.0f, 10);

//========================================================

void __render_string( int32 x, int32 y, const char *s )
{
	glColor3f(0.5f, 0.5f, 0.9f);
	glRasterPos2i(x, y);
	uint32 l = (uint32) std::strlen(s);
	for( uint32 i = 0; i < l; ++i )
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *(s + i));
}

void render_string( int32 x, int32 y, string s )
{
	__render_string(x, y, s.c_str());
}

void passive_mouse_func( int x, int y )
{
	vec2 point(x, y);
	g_screen = g_p_mat * point;

	// world
	g_world = g_screen;
	mat2 mirror_y(1, 0, 0, -1);
	vec2 v(mirror_y * g_world);

	mat2 rotate_90(0, -1, 1, 0);
	vec2 v1(rotate_90 * v);

	vec2 move_vec(80, 30);

	g_world = v1 + move_vec;
}

void click_mouse_func( int button, int state, int x, int y )
{
	passive_mouse_func(x, y);
}

void draw_cursor()
{
	stringstream ss;
	ss << g_world[0] << "," << g_world(1);
	render_string(g_screen[0], g_screen[1], ss.str());

	Mark m(vec2_ie(30, 80));
	m.draw();
}

void phy_loop_func( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// My event
	draw_cursor();

	// Source event
	static double s_accumulator = 0;
	// Different time mechanisms for Linux and Windows
	s_accumulator += g_clock.Elapsed()
	        / static_cast<double>(boost::chrono::duration_cast<clock_freq>(
	                boost::chrono::seconds(1)).count());
	g_clock.Start();
	s_accumulator = clamp(0.0f, 0.1f, s_accumulator);
	while( s_accumulator >= dt ){
		if( !g_frameStepping )
			g_scene->Step();
		else{
			if( g_canStep ){
				g_scene->Step();
				g_canStep = false;
			}
		}
		s_accumulator -= dt;
	}
	g_clock.Stop();
	g_scene->Render();

	glutSwapBuffers();
}

//========================================================

int main( int argc, char** argv )
{
	float max_img_x_px = 450;
	float max_img_y_px = 800;
	float max_img_x_m = 60;
	float max_img_y_m = 107;

	g_p_mat = mat2(max_img_x_m / max_img_x_px, 0.0f, 0.0f,
	        max_img_y_m / max_img_y_px);

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(450, 800);
	glutCreateWindow("RadarView");
	glutDisplayFunc(phy_loop_func);
	glutMouseFunc(click_mouse_func);
	glutIdleFunc(phy_loop_func);
	glutPassiveMotionFunc(passive_mouse_func);
	glMatrixMode( GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, max_img_x_m, max_img_y_m, 0);
	glMatrixMode( GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	srand(1);
	glutMainLoop();
	return 0;
}
