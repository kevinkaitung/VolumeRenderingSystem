#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

#include "definition.h"
#include "GLSLSetup.h"
#include "graph.h"
#include "colorMap.h"
#include "colorMap2D.h"
#include "transferFunction.h"

#define ADD_REGION_STATE 0
#define PI 3.14159265358979323846

using namespace std;

float zoomFactor = 1.0;
float xmin = 0.0, xmax = 0.0, ymin = 0.0, ymax = 0.0, zmin = 0.0, zmax = 0.0;
float xminOrigin = -200.0, xmaxOrigin = 200.0, yminOrigin = -200.0, ymaxOrigin = 200.0, zminOrigin = 0.0, zmaxOrigin = 900.0;

float  colors[6][3] = { {0.5,0.5,0.5}, {0.7,0.7,0.7}, {0.7,0.5,0.5},
					 {0.5,0.5,0.5}, {0.5,0.7,0.5}, {0.5,0.5,0.7} };

float Right = 0.0, up = 0.0, zoom = 0.0, pitch = 0.0, roll = 0.0, head = 0.0;

/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * disk;
/*-----Define window size----*/
int width = 1500, height = 600;
int UIWidth = 700, UIHeight = 600;
int DrawWidth = 800, DrawHeight = 600;

/*----Define material properties for cube -----*/
float  mat_ambient[] = { 0.1, 0.3, 0.1, 1.0 };
float  mat_diffuse[] = { 0.1, 0.9, 0.1, 1.0 };
float  mat2_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
float  mat_specular[] = { 0.4, 0.4, 0.4, 1.0 };
float  mat_shininess = 64.0;

float originEye[3] = { 300.0, 300.0, 300.0 };
float eye[3] = { 300.0, 300.0, 300.0 };
float center[3] = { 0.0, 0.0, 0.0 };
float lookup[3] = { 0.0, 1.0, 0.0 };
/*----Define light source properties -------*/
//float  lit_position[] = { eye[0], eye[1], eye[2], 1.0 };
//float  lit_position[] = { 400.0, 400.0, 400.0, 1.0 };
float  origin_lit_direction[] = { 1.0, 1.0, 1.0, 0.0 };
float  lit_direction[] = { 1.0, 1.0, 1.0, 0.0 };
float  lit_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
float  lit_specular[] = { 0.7, 0.7, 0.7, 1.0 };
//float  lit_diffuse[] = { 0.8, 0.4, 0.4, 1.0 };
//float  lit_specular[] = { 0.7, 0.7, 0.7, 1.0 };
//float  lit_cutoff = 60.0;
//float  lit_exponent = 1.0;

//float  lit1_direction[] = { -1.0, -1.0, -1.0, 0.0 };
//float  lit1_position[] = { -0.5, 1.0, -2.0, 1.0 };
//float  lit1_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
//float  lit1_specular[] = { 0.7, 0.7, 0.7, 1.0 };

float  global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

int mode = 0;

int top_menu, transfer_choose_menu, one_dim_color_menu, add_region_menu;
int current_choose_color_line = 0;
int transfer_function_type = 0;
int add_region_state = -1;

int maxCount = 0;

unsigned int texName[4];

extern transferFunction tf;

static colorMap ColorMap(255);
static colorMap2D ColorMap2D;
static int res[3] = { 0 };


/*-----Define a unit box--------*/
/* Vertices of the box */
float  vCoord[][3] = { {0.0, 0.0, 0.0}, {256.0, 0.0, 0.0},
					  {256.0, 256.0, 0.0}, {0.0, 256.0, 0.0},
					  {0.0, 0.0, 256.0}, {256.0, 0.0, 256.0},
					  {256.0, 256.0, 256.0}, {0.0, 256.0, 256.0} };
float  tCoord[][3] = { {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
					  {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
					  {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0},
					  {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0} };
/* face of box, each face composing of 4 vertices */
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
					{4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };

//
float  normals[6][3] = { {0.0,0.0,-1.0}, {0.0,-1.0,0.0}, {1.0,0.0,0.0},
					 {0.0,0.0,1.0}, {0.0,1.0,0.0}, {-1.0,0.0,0.0} };

//float  normals[][3] = { {0.0,0.0,1.0}, {0.0,1.0,0.0}, {-1.0,0.0,0.0},
	//				 {0.0,0.0,-1.0}, {0.0,-1.0,0.0}, {1.0,0.0,0.0} };


/* indices of the box faces */
int    cube[6] = { 0, 1, 2, 3, 4, 5 };

void create_texture();

//graph related init
void  myinit()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);      /*set the background color BLACK */
					 /*Clear the Depth & Color Buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	/*---Create quadratic objects---*/
	if (sphere == NULL) {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
	}
	if (cylind == NULL) {
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
	}
	if (disk == NULL) {
		disk = gluNewQuadric();
		gluQuadricDrawStyle(disk, GLU_FILL);
		gluQuadricNormals(disk, GLU_SMOOTH);
	}
	/*-----Enable face culling -----*/
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
	glEnable(GL_NORMALIZE);   /*Enable mornalization  */

	glEnable(GL_LIGHTING);    /*Enable lighting effects */
	glEnable(GL_LIGHT0);      /*Turn on light0 */
	//glEnable(GL_LIGHT1);      /*Turn on light1 */
	/*-----Define light0 ---------*/
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
	//glLightfv(GL_LIGHT0, GL_POSITION, lit_direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);

	/*-----Define light1 ---------*/
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, lit1_specular);
	//glLightfv(GL_LIGHT1, GL_POSITION, lit1_direction);  /*fixed position in eye space---*/

	/*-----Define some global lighting status -----*/
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

	xmin = xminOrigin;
	xmax = xmaxOrigin;
	ymin = yminOrigin;
	ymax = ymaxOrigin;
	zmin = zminOrigin;
	zmax = zmaxOrigin;

	create_texture();
}

void create_colormap_texture()
{
	glBindTexture(GL_TEXTURE_1D, texName[1]);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	unsigned char* tmpdata = new unsigned char[256 * 4];
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmpdata[4 * i + j] = (unsigned char)ColorMap.rgba[i][j];
		}
	}
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, ColorMap.size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmpdata);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_1D, texName[1]);
	glUniform1i(glGetUniformLocation(ReturnProgramID(0), "T2"), 1);
}

void create_2D_colormap_texture()
{
	//2D color map
	glBindTexture(GL_TEXTURE_2D, texName[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ColorMap2D.maxXSize, ColorMap2D.maxYSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, ColorMap2D.colorMap2DRegions);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texName[3]);
	glUniform1i(glGetUniformLocation(ReturnProgramID(0), "T4"), 3);
}

void create_texture()
{
	glEnable(GL_TEXTURE);

	glGenTextures(4, texName);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	create_colormap_texture();

	glBindTexture(GL_TEXTURE_3D, texName[0]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, res[0], res[1], res[2], 0, GL_RGBA, GL_FLOAT, tf.gradientComponent);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texName[0]);
	glUniform1i(glGetUniformLocation(ReturnProgramID(0), "T1"), 0);

	create_2D_colormap_texture();

	//gradient magnitude
	glBindTexture(GL_TEXTURE_3D, texName[2]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	for (int i = 0; i < res[0] * res[1] * res[2]; i++)
	{
		tf.gradientMagnitude[i] /= tf.diffGradientMag;
	}
	glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, res[0], res[1], res[2], 0, GL_LUMINANCE, GL_FLOAT, tf.gradientMagnitude);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, texName[2]);
	glUniform1i(glGetUniformLocation(ReturnProgramID(0), "T3"), 2);
}

void draw_volume_rendering_cube()
{
	for (int i = 0; i < 6; i++)
	{
		glNormal3fv(normals[i]);
		//glColor3fv(colors[i]);  /* Set color */
		glBegin(GL_POLYGON);  /* Draw the face */
		glMultiTexCoord3fv(GL_TEXTURE0, tCoord[face[i][0]]);
		glVertex3fv(vCoord[face[i][0]]);
		glMultiTexCoord3fv(GL_TEXTURE0, tCoord[face[i][1]]);
		glVertex3fv(vCoord[face[i][1]]);
		glMultiTexCoord3fv(GL_TEXTURE0, tCoord[face[i][2]]);
		glVertex3fv(vCoord[face[i][2]]);
		glMultiTexCoord3fv(GL_TEXTURE0, tCoord[face[i][3]]);
		glVertex3fv(vCoord[face[i][3]]);
		glEnd();
	}
}

void draw_axes()
{
	/*----Draw a white sphere to represent the original-----*/
	glColor3f(0.9, 0.9, 0.9);

	gluSphere(sphere, 2.0,   /* radius=2.0 */
		12,            /* composing of 12 slices*/
		12);           /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
	glColor3f(0.0, 0.95, 0.95);
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
		10.0,              /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */

/* Draw Y axis */
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
	glColor3f(0.95, 0.0, 0.95);
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
		10.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();

	/* Draw X axis */
	glColor3f(0.95, 0.95, 0.0);
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
	gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
		10.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();
	/*-- Restore the original modelview matrix --*/
	glPopMatrix();
}

//畫長條圖的背景網格
void draw_grid()
{
	//set grid color gray
	glColor3f(0.5, 0.5, 0.5);
	glLineWidth(1.0);
	//定義網格大小
	int gridSizeX = 260, gridSizeY = 260;
	//每10單位才畫一次網格線
	for (int i = 0; i <= gridSizeX; i += 10)
	{
		glBegin(GL_LINES);
		glVertex3f(i, 0.0, 0.0);
		glVertex3f(i, gridSizeY - 1.0, 0.0);
		glEnd();
	}
	for (int j = 0; j <= gridSizeY; j += 10)
	{
		glBegin(GL_LINES);
		glVertex3f(0.0, j, 0.0);
		glVertex3f(gridSizeX - 1.0, j, 0.0);
		glEnd();
	}
}

//function value, gradient magnitude值的regular grid圖
void draw_fv_mag_grid()
{
	maxCount = 255;
	for (int i = 0; i < tf.diffFuncValue; i++)
	{
		for (int j = 0; j < tf.diffGradientMag; j++)
		{
			float grayScale = (float)tf.funcValGradientMagCount[i][j] / (float)maxCount;
			if (grayScale > 1.0)
			{
				grayScale = 1.0;
			}
			glColor3f(grayScale, grayScale, grayScale);
			glBegin(GL_POLYGON);
			glVertex3f(i - 0.5, j - 0.5, 0.0);
			glVertex3f(i + 0.5, j - 0.5, 0.0);
			glVertex3f(i + 0.5, j + 0.5, 0.0);
			glVertex3f(i - 0.5, j + 0.5, 0.0);
			glEnd();
		}
	}
}

//畫fv, gradient mag的選定範圍
void draw_regions()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < ColorMap2D.Regions.size(); i++)
	{
		glColor3f(ColorMap2D.Regions[i].r / 255.0, ColorMap2D.Regions[i].g / 255.0, ColorMap2D.Regions[i].b / 255.0);
		glBegin(GL_POLYGON);
		glVertex3f(ColorMap2D.Regions[i].points[0].x, ColorMap2D.Regions[i].points[0].y, 0.0);
		glVertex3f(ColorMap2D.Regions[i].points[1].x, ColorMap2D.Regions[i].points[1].y, 0.0);
		glVertex3f(ColorMap2D.Regions[i].points[2].x, ColorMap2D.Regions[i].points[2].y, 0.0);
		glVertex3f(ColorMap2D.Regions[i].points[3].x, ColorMap2D.Regions[i].points[3].y, 0.0);
		glEnd();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void draw_regions_point()
{
	glPointSize(4.0);
	for (int i = 0; i < ColorMap2D.Regions.size(); i++)
	{
		glColor3f(ColorMap2D.Regions[i].r / 255.0, ColorMap2D.Regions[i].g / 255.0, ColorMap2D.Regions[i].b / 255.0);
		glBegin(GL_POINTS);
		glVertex3f(ColorMap2D.Regions[i].points[0].x, ColorMap2D.Regions[i].points[0].y, 0.0);
		glVertex3f(ColorMap2D.Regions[i].points[1].x, ColorMap2D.Regions[i].points[1].y, 0.0);
		glVertex3f(ColorMap2D.Regions[i].points[2].x, ColorMap2D.Regions[i].points[2].y, 0.0);
		glVertex3f(ColorMap2D.Regions[i].points[3].x, ColorMap2D.Regions[i].points[3].y, 0.0);
		glEnd();
	}
}

//function value值的長條圖
void draw_bar_chart()
{
	maxCount = 255;
	for (int i = 0; i < tf.diffFuncValue; i++)
	{
		float grayScale = (float)tf.funcValCount[i] / maxCount;
		if (grayScale > 1.0)
		{
			grayScale = 1.0;
		}
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON);
		glVertex3f(i - 0.5, 0.0, 0.0);
		glVertex3f(i + 0.5, 0.0, 0.0);
		glVertex3f(i + 0.5, grayScale * 255.0, 0.0);
		glVertex3f(i - 0.5, grayScale * 255.0, 0.0);
		glEnd();
	}
}

void draw_color_line()
{
	glLineWidth(1.0);
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			glColor3f(1.0, 0.0, 0.0);
		}
		else if (i == 1)
		{
			glColor3f(0.0, 1.0, 0.0);
		}
		else if (i == 2)
		{
			glColor3f(0.0, 0.0, 1.0);
		}
		else if (i == 3)
		{
			glColor3f(1.0, 0.0, 1.0);
		}
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < ColorMap.controlPointIndex[i].size(); j++)
		{
			glVertex3f((float)ColorMap.controlPointIndex[i][j], (float)ColorMap.rgba[ColorMap.controlPointIndex[i][j]][i], 0.0);
		}
		glEnd();
	}
}

void draw_sample_point()
{
	glPointSize(5.0);
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			glColor3f(1.0, 0.0, 0.0);
		}
		else if (i == 1)
		{
			glColor3f(0.0, 1.0, 0.0);
		}
		else if (i == 2)
		{
			glColor3f(0.0, 0.0, 1.0);
		}
		else if (i == 3)
		{
			glColor3f(1.0, 0.0, 1.0);
		}
		for (int j = 0; j < ColorMap.controlPointIndex[i].size(); j++)
		{
			glBegin(GL_POINTS);
			glVertex3f((float)ColorMap.controlPointIndex[i][j], (float)ColorMap.rgba[ColorMap.controlPointIndex[i][j]][i], 0.0);
			glEnd();
		}
	}
}

void draw_blend_color()
{
	for (int i = 0; i < ColorMap.size; i++)
	{
		glColor4f(ColorMap.rgba[i][0] / 255.0, ColorMap.rgba[i][1] / 255.0, ColorMap.rgba[i][2] / 255.0, ColorMap.rgba[i][3] / 255.0);
		glBegin(GL_POLYGON);
		glVertex3f(i - 0.5, -20.0, 0.0);
		glVertex3f(i + 0.5, -20.0, 0.0);
		glVertex3f(i + 0.5, -5.0, 0.0);
		glVertex3f(i - 0.5, -5.0, 0.0);
		glEnd();
	}
}

extern unsigned char*** myData;
static int UIViewport[4];
static double UIModelview[16];
static double UIProjection[16];
/*-------------------------------------------------------
 * Display callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{
	/*Clear previous frame and the depth buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//UI control
	//UI panel use default render mode and disable lighting effect
	glUseProgram(0);
	glDisable(GL_LIGHTING);
	glViewport(0, 0, UIWidth, UIHeight);
	make_projection(1);
	make_view(1);
	draw_axes();
	if (transfer_function_type == 0)
	{
		draw_sample_point();
		draw_color_line();
		draw_grid();
		draw_bar_chart();
		draw_blend_color();
	}
	else if (transfer_function_type == 1)
	{
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		glVertex3f(256.0, 144.0, 0.0);
		glEnd();
		draw_regions_point();
		draw_regions();
		draw_fv_mag_grid();
	}

	glGetDoublev(GL_MODELVIEW_MATRIX, UIModelview); //recuperer matrices
	glGetDoublev(GL_PROJECTION_MATRIX, UIProjection); //recuperer matrices
	glGetIntegerv(GL_VIEWPORT, UIViewport);//viewport

	//volume rendering
	glUseProgram(ReturnProgramID(0));

	glUniform1f(glGetUniformLocation(ReturnProgramID(0), "xmax"), res[0]);
	glUniform1f(glGetUniformLocation(ReturnProgramID(0), "ymax"), res[1]);
	glUniform1f(glGetUniformLocation(ReturnProgramID(0), "zmax"), res[2]);
	glUniform3fv(glGetUniformLocation(ReturnProgramID(0), "eye"), 1, eye);
	glUniform1f(glGetUniformLocation(ReturnProgramID(0), "mode"), transfer_function_type);

	glViewport(UIWidth, 0, DrawWidth, DrawHeight);
	make_projection(2);
	make_view(2);
	//glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
	//glBegin(GL_POINTS);
	//glVertex3f(lit_position[0], lit_position[1], lit_position[2]);
	//glEnd();
	//draw_axes();
	glLightfv(GL_LIGHT0, GL_POSITION, lit_direction);
	//glUseProgram(0);
	//glEnable(GL_LIGHTING);
	//-----Define some material properties shared by every one ---
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	//將結果移到正中心點
	glTranslatef(-res[0] / 2, -res[1] / 2, -res[2] / 2);
	draw_volume_rendering_cube();

	/*-------Swap the back buffer to the front --------*/
	glutSwapBuffers();
	return;
}

/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int type)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch (type)
	{
	case 1:
		//for UI viewport
		//利用view volume的比例將高度壓扁(世界坐標系的值仍維持1為間距
		if (transfer_function_type == 0)
		{
			glOrtho(-135.0 * (float)UIWidth / (float)UIHeight, 135.0 * (float)UIWidth / (float)UIHeight, -260,
				260.0,
				0.0, 40.0);
		}
		//2D transfer不須調整view volume比例
		else if (transfer_function_type == 1)
		{
			glOrtho(-135.0 * (float)UIWidth / (float)UIHeight, 135.0 * (float)UIWidth / (float)UIHeight, -100.0,
				100.0,
				0.0, 40.0);
		}
		break;
	case 2:
		//for volume viewport
		glOrtho(xmin * (float)DrawWidth / (float)DrawHeight, xmax * (float)DrawWidth / (float)DrawHeight, ymin, ymax, zmin, zmax);
		break;
	}
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int type)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	switch (type)
	{
	case 1:
		//for UI viewport
		if (transfer_function_type == 0)
		{
			gluLookAt(130, 130, 30.0, 130, 130, 0.0, 0.0, 1.0, 0.0);
		}
		else if (transfer_function_type == 1)
		{
			gluLookAt(130, 65, 30.0, 130, 65, 0.0, 0.0, 1.0, 0.0);
		}
		break;
	case 2:
		//glRotatef(pitch, 1.0, 0.0, 0.0);
		//glRotatef(roll, 0.0, 1.0, 0.0);
		//glRotatef(head, 0.0, 0.0, 1.0);
		//for volume viewport
		gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], lookup[0], lookup[1], lookup[2]);
		break;
	}
}

/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{

	width = w;
	height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width > height)
		glOrtho(xmin, xmax, ymin * (float)height / (float)width,
			ymax * (float)height / (float)width,
			zmin, zmax);
	else
		glOrtho(xmin * (float)width / (float)height,
			xmax * (float)width / (float)height, ymin, ymax,
			zmin, zmax);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void my_keyboard(unsigned char key, int x, int y)
{
	if (key == 'l' || key == 'L') Right += 20.0;
	else if (key == 'r' || key == 'R') Right -= 20.0;
	else if (key == 'u' || key == 'U') up += 20.0;
	else if (key == 'd' || key == 'D') up -= 20.0;
	else if (key == 'n' || key == 'N') zoom -= 20.0;
	else if (key == 'f' || key == 'F') zoom += 20.0;
	else if (key == 'x') pitch += 10.0;
	else if (key == 'X') pitch -= 10.0;
	else if (key == 'y')
	{
		head += 10.0 * PI / 180;
		//Rotate eye position
		eye[0] = cos(head) * originEye[0] + (-sin(head)) * originEye[2];
		eye[1] = originEye[1];
		eye[2] = sin(head) * originEye[0] + cos(head) * originEye[2];
		//Rotate light direction with eye
		lit_direction[0] = cos(head) * origin_lit_direction[0] + (-sin(head)) * origin_lit_direction[2];
		lit_direction[1] = origin_lit_direction[1];
		lit_direction[2] = sin(head) * origin_lit_direction[0] + cos(head) * origin_lit_direction[2];
	}
	else if (key == 'Y')
	{
		head -= 10.0 * PI / 180;
		//Rotate eye position
		eye[0] = cos(head) * originEye[0] + (-sin(head)) * originEye[2];
		eye[1] = originEye[1];
		eye[2] = sin(head) * originEye[0] + cos(head) * originEye[2];
		//Rotate light direction with eye
		lit_direction[0] = cos(head) * origin_lit_direction[0] + (-sin(head)) * origin_lit_direction[2];
		lit_direction[1] = origin_lit_direction[1];
		lit_direction[2] = sin(head) * origin_lit_direction[0] + cos(head) * origin_lit_direction[2];
	}
	else if (key == 'z') roll += 10.0;
	else if (key == 'Z') roll -= 10.0;
	else if (key == 'p') {
		zoomFactor += 0.1;
		xmin = xminOrigin * zoomFactor;
		xmax = xmaxOrigin * zoomFactor;
		ymin = yminOrigin * zoomFactor;
		ymax = ymaxOrigin * zoomFactor;
	}
	else if (key == 'P') {
		zoomFactor -= 0.1;
		xmin = xminOrigin * zoomFactor;
		xmax = xmaxOrigin * zoomFactor;
		ymin = yminOrigin * zoomFactor;
		ymax = ymaxOrigin * zoomFactor;
	}
	else if (key == 'm') {
		mode = 1;
	}
	else if (key == 'M') {
		mode = 0;
	}
	display();
}

void idle()
{
}

//是否調整region中
bool nowAdjustRegion = false;

void my_motion(int x, int y)
{
	double xOut, yOut, zOut;
	gluUnProject(x, UIHeight - y, 0, UIModelview, UIProjection, UIViewport, &xOut, &yOut, &zOut);
	if (nowAdjustRegion)
	{
		//及時計算調整後的region資訊
		ColorMap2D.Regions[ColorMap2D.onAdjustRegionIdx].onAdjustRegionBoundary(xOut, yOut);
	}
	display();
}

void my_mouse(int button, int state, int x, int y)
{
	double xOut, yOut, zOut;
	gluUnProject(x, UIHeight - y, 0, UIModelview, UIProjection, UIViewport, &xOut, &yOut, &zOut);
	if (transfer_function_type == 0)
	{
		if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			cout << "put 1D transfer sample point" << endl;
			ColorMap.userDefineControlPoint(round(xOut), round(yOut), current_choose_color_line);
			create_colormap_texture();
		}
		else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
		{
			cout << "cancel 1D transfer sample point" << endl;
			if (ColorMap.userCancelControlPoint(round(xOut), current_choose_color_line))
			{
				create_colormap_texture();
			}
		}
	}
	else if (transfer_function_type == 1)
	{
		//點擊新增region
		if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && add_region_state == ADD_REGION_STATE)
		{
			cout << "add 2D transfer region" << endl;
			float red, green, blue, alpha;
			cout << "enter r g b a value(0~255):\n";
			cin >> red >> green >> blue >> alpha;
			ColorMap2D.addNewRegion(xOut, yOut, 20.0, 20.0, red, green, blue, alpha);
			//新增完就將狀態更新為idle
			add_region_state = -1;
			ColorMap2D.calculateColorMap2DRegions();
			create_2D_colormap_texture();
		}
		//點擊刪除region
		else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)
		{
			cout << "cancel 2D transfer region" << endl;
			ColorMap2D.deleteRegion(xOut, yOut);
			ColorMap2D.calculateColorMap2DRegions();
			create_2D_colormap_texture();
		}
		//點擊調整region頂點
		else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
		{
			cout << "start adjust 2D transfer region point" << endl;
			//找出調整中的region
			if (ColorMap2D.checkAndFindAdjustRegion(xOut, yOut))
			{
				//找出調整中的region、調整中的點
				if (ColorMap2D.Regions[ColorMap2D.onAdjustRegionIdx].beforeAdjustRegionBoundary(xOut, yOut))
				{
					nowAdjustRegion = true;
				}
			}
		}
		//鬆開調整region頂點
		else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
		{
			cout << "finish adjust 2D transfer region point" << endl;
			nowAdjustRegion = false;
			ColorMap2D.calculateColorMap2DRegions();
			create_2D_colormap_texture();
			for (int i = 0; i < ColorMap2D.Regions.size(); i++)
			{
				cout << "i: " << i << endl;
				cout << "x min: " << ColorMap2D.Regions[i].points[0].x << " x max: " << ColorMap2D.Regions[i].points[2].x << endl;
				cout << "y min: " << ColorMap2D.Regions[i].points[0].y << " y max: " << ColorMap2D.Regions[i].points[2].y << endl;
			}
		}
	}
	display();
}

void one_dim_color_menu_func(int type)
{
	current_choose_color_line = type;
}

void transfer_choose_menu_func(int type)
{
	transfer_function_type = type;
}

void add_region_menu_func(int type)
{
	add_region_state = type;
}

void top_menu_func(int type)
{

}

void mainSetupAndLoop(int argc, char** argv, char* vertShaderFileName, char* fragShaderFileName, int passRes[3])
{
	res[0] = passRes[0];
	res[1] = passRes[1];
	res[2] = passRes[2];
	ColorMap2D.setMaxXYSize(tf.diffFuncValue, tf.diffGradientMag);
	/*-----Initialize the GLUT environment-------*/
	glutInit(&argc, argv);

	/*-----Depth buffer is used, be careful !!!----*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(width, height);
	glutCreateWindow("engine");

	//choose 1D or 2D transfer function
	transfer_choose_menu = glutCreateMenu(transfer_choose_menu_func);
	glutAddMenuEntry("1D transfer", 0);
	glutAddMenuEntry("2D transfer", 1);

	//choose 1D transfer color line
	one_dim_color_menu = glutCreateMenu(one_dim_color_menu_func);
	glutAddMenuEntry("red", 0);
	glutAddMenuEntry("green", 1);
	glutAddMenuEntry("blue", 2);
	glutAddMenuEntry("opacity", 3);

	add_region_menu = glutCreateMenu(add_region_menu_func);
	glutAddMenuEntry("add square region", ADD_REGION_STATE);

	top_menu = glutCreateMenu(top_menu_func);/* Create top menu */
	glutAddSubMenu("transfer function", transfer_choose_menu);    /* add color-menu as a sub-menu */
	glutAddSubMenu("1D transfer color", one_dim_color_menu);
	glutAddSubMenu("2D transfer add region", add_region_menu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);    /* associate top-menu with middle button*/


	//glew initialization
	if (glewInit()) {
		printf("Something went wrong!\n");
		exit(-1);
	}

	myinit();      /*---Initialize other state varibales----*/

	/*----Associate callback func's whith events------*/
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(my_reshape);
	glutKeyboardFunc(my_keyboard);
	glutMotionFunc(my_motion);
	glutMouseFunc(my_mouse);


	//print out OpenGL and GLSL versions
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	//Program objects and shaders must be created after glad has been initiated!!!
	setGLSLshaders(vertShaderFileName, fragShaderFileName);

	glutMainLoop();

}