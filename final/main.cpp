#include <math.h>
#include <GL/glut.h>
#include "glm.h"
#include "imageloader.h"

int windowWidth = 1000;
int windowHeight = 1000;
float aspect = float(windowWidth) / float(windowHeight);

static int shoulder = 0, shoulder2 = 0, shoulder3 = 0, elbow = 0, fingerBase = 0, fingerUp = 0, rhip = 0, rhip2 = 0, rknee = 0, lknee = 0, lhip = 0, lhip2 = 0;
static int flower=0;
static float x=0;
static int angle=0;
int press=0;
int pressm=0;
//flower
 float q=0.0;
 float w=0.0;
 float e=0.0;
 //robot in z directio
 float r=0.0;

float DRot = 90;
float Zmax, Zmin;
GLMmodel* pmodel;
float VRot =0.0;


GLMmodel* pmodel1 =glmReadOBJ("data/rose+vase.obj");
GLMmodel* pmodel2 =glmReadOBJ("data/table.obj");
GLMmodel* pmodel3 =glmReadOBJ("data/stool.obj");
double eye[] = { .03, .3,.1 };
double center[] = { 0, 0, -2};
double up[] = { 0, 1, 0 };


// RGBA
GLfloat light_ambient2[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse2[] = { 1.0, 0.0, 0.0,1.0 };
GLfloat light_specular2[] = {1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position2[] = {0.5,0.5, 0.5, 1.0 };


// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = {0.5,5.0, 0.0, 1.0 };
GLfloat lightPos1[] = {-0.5,-5.0,-2.0, 1.0 };

// Material Properties
GLfloat mat_amb_diff[] = {0.643, 0.753, 0.934, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = {100.0 };

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
      GLuint textureId;
      glGenTextures(1, &textureId); //Make room for our texture
      glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
      //Map the image to the texture
      glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                               0,                            //0 for now
                               GL_RGB,                       //Format OpenGL uses for image
                               image->width, image->height,  //Width and height
                               0,                            //The border of the image
                               GL_RGB, //GL_RGB, because pixels are stored in RGB format
                               GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                                 //as unsigned numbers
                               image->pixels);               //The actual pixel data
      return textureId; //Returns the id of the texture
}

GLuint _textureId; //The id of the texture
GLuint _textureId1; //The id of the texture


void Draw_cube(GLdouble width, GLdouble height, GLdouble depth) // Draw function
{
   glPushMatrix();
   glScalef(width, height, depth);
   glutWireCube(1.0);
   glPopMatrix();
}
void init()
{
        glEnable(GL_LIGHTING);

        glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
	     GLfloat lightColor2[] = {1.0f, 1.0f,  1.0f, 1.0f };
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
        glLightfv(GL_LIGHT2, GL_POSITION, lightPos1);

        // Enable Depth buffer
        glEnable(GL_DEPTH_TEST);
}

//flower
void drawmodel1(void)
{
		glmUnitize(pmodel1);
		glmFacetNormals(pmodel1);
		glmVertexNormals(pmodel1, 90.0);
		glmScale(pmodel1, .15);
		glmDraw(pmodel1, GLM_SMOOTH | GLM_MATERIAL);
}
//table
void drawmodel2(void)
{
		glmUnitize(pmodel2);
		glmFacetNormals(pmodel2);
		glmVertexNormals(pmodel2, 90.0);
		glmScale(pmodel2, .15);

	glmDraw(pmodel2, GLM_SMOOTH | GLM_MATERIAL);
}
void drawmodel3(void)
{
		glmUnitize(pmodel3);
		glmFacetNormals(pmodel3);
		glmVertexNormals(pmodel3, 90.0);
		glmScale(pmodel3, .15);

	glmDraw(pmodel3, GLM_SMOOTH | GLM_MATERIAL);
}

GLuint startList;

//Initializes 3D rendering
void initRendering() {
     	 Image* image = loadBMP("image2.bmp");
      	_textureId = loadTexture(image);
      	delete image;
       // Turn on the power
        glEnable(GL_LIGHTING);
        // Flip light switch
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        // assign light parameters
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	GLfloat lightColor1[] = {1.0f, 1.0f,  1.0f, 1.0f };
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
        glEnable(GL_NORMALIZE);
        //Enable smooth shading
        glShadeModel(GL_SMOOTH);
        // Enable Depth buffer
        glEnable(GL_DEPTH_TEST);

}



void draw_right_arm(void)
{
   glPushMatrix();

   glTranslatef(-2, 4.5, 0.0);                          //hena x,y
   glRotatef((-(GLfloat)shoulder) - 90, 0.0, 0.0, 1.0); //rotate lebara
   glRotatef(180, 1.0, 0.0, 0.0);
   glTranslatef(1.0, 0.25, 0.0); //hena x,y
   glTranslatef(-1.0, 0.0, 0.5);
   glRotatef(-(GLfloat)shoulder2, 0.0, 1.0, 0.0); //rotate lfoq
   glTranslatef(1.0, 0.0, -0.5);
   glRotatef(-(GLfloat)shoulder3, 1.0, 0.0, 0.0); //rotate hwalen nafso
   //glColor3f(0.0, 1.0, 0.0);
   Draw_cube(2.0f, 0.5f, 1.0f);
   //forearm
   glPushMatrix();
   glTranslatef(1.0, -0.25, 0.0);
   glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
   glTranslatef(1.5, 0.25, 0.0);
   Draw_cube(3.0f, 0.5f, 1.0f);
   //Draw finger flang 1
   glPushMatrix();
   glTranslatef(1.5, 0.25, -0.4); // (b3mqal translation b3d el origin bta3 akher shakl)
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 11
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger 2
   glPushMatrix();
   glTranslatef(1.5, 0.25, -0.2);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 22
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger flang 3
   glPushMatrix();
   glTranslatef(1.5, 0.25, 0.2);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 33
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger 4
   glPushMatrix();
   glTranslatef(1.5, 0.25, 0.4);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f); //law hagarb figer 4 bas ahot pop law harsem finger 44 ashel el pop
                                //Draw finger flang 44
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger flang 5
   glPushMatrix();
   glTranslatef(1.5, -0.25, 0.0);
   glRotatef(-(GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 55
   glPushMatrix();
   glTranslatef(0.25, -0.05, 0.0);
   glRotatef(-(GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
}

void draw_left_arm(void)
{

   glPushMatrix();
   glTranslatef(2, 4.5, 0.0);
   glRotatef(((GLfloat)shoulder) - 90, 0.0, 0.0, 1.0);
   glTranslatef(1.0, 0.25, 0.0);
   glTranslatef(-1.0, 0.0, -0.5);
   glRotatef((GLfloat)shoulder2, 0.0, 1.0, 0.0);
   glTranslatef(1.0, 0.0, 0.5);
   glRotatef((GLfloat)shoulder3, 1.0, 0.0, 0.0);
  // glColor3f(0.0, 1.0, 0.0);
   Draw_cube(2.0f, 0.5f, 1.0f);
   //forearm
   glPushMatrix();
   glTranslatef(1.0, -0.25, 0.0); //y,x
   glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
   glTranslatef(1.5, 0.25, 0.0); //y,x
   Draw_cube(3.0f, 0.5f, 1.0f);
   //Draw finger flang 1
   glPushMatrix();
   glTranslatef(1.5, 0.25, -0.4); // (b3mqal translation b3d el origin bta3 akher shakl)
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 11
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger 2
   glPushMatrix();
   glTranslatef(1.5, 0.25, -0.2);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 22
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger flang 3
   glPushMatrix();
   glTranslatef(1.5, 0.25, 0.2);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 33
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger 4
   glPushMatrix();
   glTranslatef(1.5, 0.25, 0.4);
   glRotatef((GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, -0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f); //law hagarb figer 4 bas ahot pop law harsem finger 44 ashel el pop
                                //Draw finger flang 44
   glPushMatrix();
   glTranslatef(.25, -0.05, 0.0);
   glRotatef((GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   //Draw finger flang 5
   glPushMatrix();
   glTranslatef(1.5, -0.25, 0.0);
   glRotatef(-(GLfloat)fingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   //Draw finger flang 55
   glPushMatrix();
   glTranslatef(0.25, -0.05, 0.0);
   glRotatef(-(GLfloat)fingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.25, 0.05, 0.0);
   Draw_cube(0.5f, 0.1f, 0.1f);
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
}

void draw_right_leg(void)
{
   glPushMatrix();
   glTranslatef(-0.5, -3.25, 0.0);
   glTranslatef(-0.75, 1.75, 0.0);
   glRotatef((GLfloat)rhip, 0.0, 0.0, 1.0);
   glTranslatef(0.5, -1.75, 0.0);
   glTranslatef(0.0, 1.75, -0.5);
   glRotatef((GLfloat)rhip2, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -1.75, .5);
   Draw_cube(1.0f, 3.5f, 1.0f);
   glPushMatrix();
   glTranslatef(0.0, -3.25, 0.0);
   glTranslatef(0.0, 1.75, -0.5);
   glRotatef((GLfloat)rknee, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -1.75, 0.5);
   Draw_cube(1.0f, 3.0f, 1.0f);
   glPushMatrix();
   glTranslatef(-0.05, -2, 0.0);
   glPushMatrix();
   glScalef(1.0, 1.0, 3.0);
   glutSolidCube(1);
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
}

void draw_left_leg(void)
{
   //hena x bel negative nahet el ymen
   glPushMatrix();

   glTranslatef(.75, -3.25, 0.0);
   glTranslatef(.5, 1.75, 0.0);
   glRotatef((GLfloat)lhip, 0.0, 0.0, 1.0);
   glTranslatef(-0.5, -1.75, 0.0);
   glTranslatef(0.0, 1.75, -0.5);
   glRotatef((GLfloat)lhip2, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -1.75, .5);
   Draw_cube(1.0f, 3.5f, 1.0f);
   glPushMatrix();
   glTranslatef(0.0, -3.25, 0.0);
   glTranslatef(0.0, 1.75, -0.5);
   glRotatef((GLfloat)lknee, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -1.75, 0.5);
   Draw_cube(1.0f, 3.0f, 1.0f);
   glPushMatrix();
   glTranslatef(0.0, -2, 0.0);
   glPushMatrix();
   glScalef(1.0, 1.0, 3.0);
   glutSolidCube(1);
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
   glPopMatrix();
}

void screen_menu(int value)
{
	char* name = 0;

	switch (value) {
	case 'a':
		name = "image.bmp";
		break;
	case 's':
		name = "image2.bmp";
		break;
	case 'd':
		name = "image3.bmp";
		break;

	}
	if (name) {
		Image* image = loadBMP(name);
		_textureId=loadTexture(image);
      if (!image) exit(0);

   }
	glutPostRedisplay();

}



void display(void)
{


	glClearColor(0.0, 0.0, 0.0, 0.0);
        // Clear Depth and Color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

        glPushMatrix();
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glPopMatrix();
        //materials properties
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glPushMatrix();
	glTranslatef(0, 0, -1);

	//floor
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBegin(GL_QUADS);

	glNormal3f(0.0,-1.0,0.0);
	glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1,-0.25,2);
        glTexCoord2f(5.0f,  0.0f);
        glVertex3f(1,-0.25,2);
        glTexCoord2f(5.0f,  20.0f);
        glVertex3f(1,-0.25,-2);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f(-1,-0.25,-2);
        glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

//flower
   glPushMatrix();

   glTranslatef(q, w,e);//for interaction
   glTranslatef(-.07,.043,.05);
    glRotatef((GLfloat)flower,0,1,0);
  glTranslatef(.2,0,0);

	glScalef(.5,.5,.5);
   drawmodel1();
	glPopMatrix();

//stool
   glPushMatrix();
   glTranslatef(.16, -.1,-.01);
	glScalef(.5, .5, .5);
   drawmodel3();
	glPopMatrix();


//table
   glPushMatrix();
   glTranslatef(-.3, -.02,-.01);
	glScalef(1.008,1.008,1.008);
   drawmodel2();
	glPopMatrix();



// robot
	glPushMatrix();
    glTranslatef( 0.0, r,0.0);
    glTranslatef( -.06,0,-.58);
	glTranslatef( 0.02,.1,1);
   glTranslatef( 0.01, 0,-.3);
   glScalef(.032, .032, .032);
   glTranslatef( 0, 0.0,0);
   glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)angle, 0.0, 1.0, 0.0);
   glTranslatef( 0, 0.0,0);
   glTranslatef(0.0, 0.0, x);  // for penguin
   glPushMatrix();
   glTranslatef(0.0, 6.0, 0.0);
   glutWireSphere(1, 10, 10);
   glPopMatrix();
   // draw trunck
   glPushMatrix();
   glTranslatef(0.0, 1.5, 0.0);
   /*
   glScalef(3,6,1);
   glutSolidCube(1);
   */
   Draw_cube(3.0f, 6.0f, 01.0f);
   glPopMatrix();

   draw_left_arm();
   draw_right_arm();
   draw_right_leg();
   draw_left_leg();
   glPopMatrix();
   glPushMatrix();
   glTranslatef(-6.5, -7, 1.7);
  // glScalef(3,3,3);
   //glutSolidCube(1.0);
    Draw_cube(3.0f, 3.0f, 3.0f);
   glPopMatrix();
   glPopMatrix();
	glutSwapBuffers();
}

void Timer(int x){
	// Refresh and redraw
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 0);
}




//////////////////////////////?????
void correct()
{
	double speed = 0.001;
	if (eye[0]>0)
	{
		eye[0] -= speed;
		center[0] -= speed;
	}
	else
	{
		eye[0] += speed;
		center[0] += speed;
	}

	if (DRot == 0)
	{
		if ((eye[2] >= -2 && eye[2] <= 2) || eye[2]>0)
		{
			eye[2] -= speed;
			center[2] -= speed;
		}
		else
		{
			eye[2] += speed;
			center[2] += speed;
		}
	}
	else
	{
		if (eye[2]>0)
		{
			eye[2] -= speed;
			center[2] -= speed;
		}
		else
		{
			eye[2] += speed;
			center[2] += speed;
		}
	}

}
void SetBound()
{
	if (DRot == 0 || eye[0]> 0.15 || eye[0]< -0.15)
	{
		if (eye[2] >= -1)
		{
			Zmax = 0.7;
			Zmin = -0.8;
		}
		else
		{
			Zmax = -1.2;
			Zmin = -2.4;
		}
	}
	else
	{
		Zmax = 0.7;
		Zmin = -2.4;
	}
}


void DTimer1(int x){

	DRot -= 1;
	if (DRot == 0)
		return;
	glutPostRedisplay();
	glutTimerFunc(30, DTimer1, 0);


}

void DTimer2(int x){
	DRot += 1;
	if (DRot == 90)
		return;
	glutPostRedisplay();
	glutTimerFunc(30, DTimer2, 0);
}

 // penguin Dance
   int check =1;

  int check2=1;

void timer(int value)
{
   int m=1;
   int l=30;
   int z=20;
  int k=10;
    if (value == 0)
    {

          if (lhip < 50)
      {
         lhip = (lhip + 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer, 0);
      }
       else
       glutTimerFunc(l, timer, 1);
    }


    else if (value == 1)
    {


         if (lhip >= 5)
         {

            lhip = (lhip - 5) % 360;
            glutPostRedisplay();
            glutTimerFunc(l, timer, 1);

         }

        else
         {
            check=check+1;
            if(check<=2)
            glutTimerFunc(l, timer, 0);
            else
            glutTimerFunc(l, timer, 2);

         }

    }


    else if (value == 2)
    {

       if (rhip > -50)
      {
         rhip = (rhip - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer, 2);
      }
       else
       glutTimerFunc(l, timer, 3);
    }

    else if (value == 3)
    {
       if (rhip <= -5)
      {
         rhip = (rhip + 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer, 3);
      }
        else
         {
            check2=check2+1;
            if(check2<=2)
            glutTimerFunc(l, timer, 2);
            else
            glutTimerFunc(k, timer, 4);
         }
    }
// ytharak leodam
      if (value == 4)
    {
       if (lhip2 < 90 && lknee > -90)
      {
         lhip2 = (lhip2 + 5) % 360;
         rhip2 = (rhip2 + 5) % 360;
         lknee = (lknee - 5) % 360;
         rknee = (rknee - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(k, timer, 4);
      }
     else

        glutTimerFunc(k, timer, 5);
     }

    else if (value == 5)
     {
      if (lhip2 >=0  && lknee < 0 )
      {
            x=x-0.1;
            lhip2 = (lhip2 - 5) % 360;
            rhip2 = (rhip2 - 5) % 360;
            lknee = (lknee + 5) % 360;
            rknee = (rknee + 5) % 360;
           glutPostRedisplay();
           glutTimerFunc(k, timer, 5);
        }
         else
         glutTimerFunc(z, timer,6 );
      }

 //ytharak khatwa wahda warah
 if (value == 6)
    {
       if (lhip2 < 90 && lknee > -90)
      {
         lhip2 = (lhip2 + 5) % 360;
         rhip2 = (rhip2 + 5) % 360;
         lknee = (lknee - 5) % 360;
         rknee = (rknee - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(z, timer, 6);
      }
     else

        glutTimerFunc(z, timer, 7);
     }


    else if (value == 7)
     {
      if (lhip2 >= 0  && lknee < 0 )
      {
            x=x+0.1;
            lhip2 = (lhip2 - 5) % 360;
            rhip2 = (rhip2 - 5) % 360;
            lknee = (lknee + 5) % 360;
            rknee = (rknee + 5) % 360;
           glutPostRedisplay();
           glutTimerFunc(z, timer, 7);

      }
       else

        glutTimerFunc(m, timer, 8);
     }
  //ytharak lwarah 3 khatwat
      if (value == 8)
    {
       if (lhip2 < 90 && lknee > -90)
      {
         lhip2 = (lhip2 + 5) % 360;
         rhip2 = (rhip2 + 5) % 360;
         lknee = (lknee - 5) % 360;
         rknee = (rknee - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(m, timer, 8);
      }
     else

        glutTimerFunc(m, timer, 9);
     }


    else if (value == 9)
     {
      if (lhip2 > 0  && lknee < 0 )
      {

        if(x<=5.2)
        {
            x=x+0.1;
            lhip2 = (lhip2 - 5) % 360;
            rhip2 = (rhip2 - 5) % 360;
            lknee = (lknee + 5) % 360;
            rknee = (rknee + 5) % 360;
           glutPostRedisplay();
           glutTimerFunc(m, timer, 9);
        }
      }
      else
         glutTimerFunc(m, timer, 8);

     }


}


void timer4(int value)
{
   int m=1;

         if (value == 0)
    {
       if (lhip2 < 90 && lknee > -90)
      {
         lhip2 = (lhip2 + 5) % 360;
         rhip2 = (rhip2 + 5) % 360;
         lknee = (lknee - 5) % 360;
         rknee = (rknee - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(m, timer4, 0);
      }
     else

        glutTimerFunc(m, timer4, 1);
     }
     else if (value == 1)
     {
      if (lhip2 > 3  && lknee < 3 )
      {

        if(x > .3)
        {
            x=x-0.1;
            lhip2 = (lhip2 - 5) % 360;
            rhip2 = (rhip2 - 5) % 360;
            lknee = (lknee + 5) % 360;
            rknee = (rknee + 5) % 360;
           glutPostRedisplay();
           glutTimerFunc(m, timer4, 1);
        }
      }
          else

        glutTimerFunc(m, timer4, 0);


     }
}

 // Doing activity

void timer2(int value)
{

   int l=50;
    //el goz el awal
    if (value == 1)
    {

      if (  lhip < 60 && shoulder <= 150)
      {
         shoulder = (shoulder + 10) % 360;
         lhip = (lhip + 5) % 360;
         rhip = (rhip - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer2, 1);
      }
       else
       glutTimerFunc(l, timer2, 2);
    }


    else if (value == 2)
    {

      if ( lhip >= 0 && shoulder >= 5 )
      {
         shoulder = (shoulder - 10) % 360;
         lhip = (lhip - 5) % 360;
         rhip = (rhip +5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer2, 2);

      }
           else
       glutTimerFunc(l, timer2, 3);
    }

   // el goz el tany
   else if (value == 3)
    {

      if (lhip2 < 90 && shoulder2 <= 160 )
      {
         shoulder2 = (shoulder2 + 5) % 360;
         lhip2 = (lhip2 + 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer2, 3);

      }
           else
       glutTimerFunc(l, timer2, 4);
    }

   else if (value == 4)
    {

       if (shoulder2 >= 5 && lhip2 >= 0)
      {
         lhip2 = (lhip2 - 5) % 360;
         shoulder2 = (shoulder2 - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer2, 4);

      }
           else
       glutTimerFunc(l, timer2, 5);
    }

       else if (value == 5)
    {

      if (rhip2 < 90 && shoulder2 <= 160 )
      {
         shoulder2 = (shoulder2 + 5) % 360;
         rhip2 = (rhip2 + 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer2, 5);

      }
           else
       glutTimerFunc(l, timer2, 6);
    }

   else if (value == 6)
    {

       if (shoulder2 >= 5 && rhip2 >= 0)
      {
         rhip2 = (rhip2 - 5) % 360;
         shoulder2 = (shoulder2 - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer2, 6);

      }

    }

    }



 // animation with interaction

   void timer3(int value)
{
   int l=70;

    if (value == 1)
    {
       angle=90;
       if ( lknee > -90 && shoulder2 < 90 && shoulder > -30 && fingerBase > -20 )
      {


         if(r == 0 || r >= -6.0)
         {
         lknee = (lknee - 5) % 360;
         rknee = (rknee - 5) % 360;
         shoulder2 = (shoulder2 + 5) % 360;
         shoulder = (shoulder -3) % 360;
         fingerBase = (fingerBase - 1) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer3, 1);
           r=r-.01;
         }
      }


     else

        glutTimerFunc(l, timer3, 2);
     }

    else if (value == 2)
     {
      if ( lknee < 0  && fingerBase > -20)
      {
         fingerBase = (fingerBase - 1) % 360;
         lknee = (lknee + 5) % 360;
         rknee = (rknee + 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer3, 2);
         // w=.3;

         w=w+.01;
         r=r+0.01;
      }
      else

        glutTimerFunc(l, timer3, 3);

     }

   else if (value == 3)
     {
      if ( angle > -60 )
      {
         q=q+.0019;
         e=e-.003;
         angle = (angle - 5) % 360;
         flower = (flower - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer3, 3);
      }
      else

        glutTimerFunc(l, timer3, 4);

     }
 else if (value == 4)
     {
      if ( shoulder2  < 90 && fingerBase > -30)
      {
         w=w+.01;
         q=q-.003;
         fingerBase = (fingerBase - 1) % 360;
         shoulder2= (shoulder2 + 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer3, 4);
      }
      else

        glutTimerFunc(l, timer3, 5);

     }
       else if (value == 5)
     {
      if ( angle <= 0 && fingerBase < 0 && shoulder2  >0 && shoulder <0)
      {

         fingerBase = (fingerBase + 3) % 360;
          shoulder2= (shoulder2 - 9) % 360;
           shoulder = (shoulder +3) % 360;
         angle = (angle + 6) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer3, 5);
      }

     }


}



void Keyboard(unsigned char Key, int x, int y){
	switch (Key)
	{

	case 27:
		exit(0);

		break;

	case ' ':
		if (DRot == 0 || DRot == 90)
		{
			if (DRot)
				DTimer1(0);
			else
				DTimer2(0);
		}
		break;

	case 'p':
   press=press+1;
   if(press == 1 )
   {

         glutTimerFunc(0, timer, 0); //penguin
   }
   else if (press == 2 && pressm==0)
   {
      glutTimerFunc(0, timer4, 0);
      glutTimerFunc(4500, timer, 0);
   }
   else {
      break;
   }
      break;
   case 'l':
       glutTimerFunc(0, timer2, 1);  // Activity

      break;

   case 'm':
      if(press==0 && pressm == 0)
      {
          pressm=1;
       glutTimerFunc(0, timer3, 1);  // Activity
      }
      else if(press != 0 && pressm == 0)
      {
         pressm=1;
         glutTimerFunc(0, timer4, 0);
          glutTimerFunc(2000, timer3, 1);

      }
      else if(pressm != 1)
      {
           break;
      }
      break;

	default:
		break;
	}
}

int main (int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Body");
	initRendering();

	glMatrixMode(GL_PROJECTION);
	gluPerspective(60, aspect, 0.1, 10);



	glutDisplayFunc(display);
   init();
	glutKeyboardFunc(Keyboard);
	Timer(0);
	glutCreateMenu(screen_menu);

	glutAddMenuEntry("Models", 0);
	glutAddMenuEntry("", 0);
	glutAddMenuEntry("wood2", 's');
	glutAddMenuEntry("wood", 'a');
	glutAddMenuEntry("metal", 'd');

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//glutTimerFunc(0,Timer1,0);
	glutMainLoop();
	return 0;

}

