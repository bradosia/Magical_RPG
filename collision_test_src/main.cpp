#define M_PI           3.14159265358979323846

#include <math.h>
#include <iostream>
#include <fstream>

//to map image filenames to textureIds
#include <string.h>
#include <map>

/* GLEW
 * The OpenGL Extension Wrangler Library
 */
#define GLEW_STATIC
#include <GL/glew.h>
/* GLFW 3.2.1
 * Open Source, multi-platform library for OpenGL
 */
#include <GLFW/glfw3.h>
//#include <SDL2/SDL_mixer.h>
/* ReactPhysics3D
 * Open-source physics engine
 */
#include <reactphysics3d/reactphysics3d.h>
#include <soil/soil.h>
#include <glm/glm.hpp>

#include "cMesh.h"

class WorldCollisionCallback: public rp3d::CollisionCallback {
public:

	bool boxCollideWithSphere1;
	bool boxCollideWithCylinder;
	bool sphere1CollideWithCylinder;
	bool sphere1CollideWithSphere2;

	rp3d::CollisionBody* boxBody;
	rp3d::CollisionBody* sphere1Body;
	rp3d::CollisionBody* sphere2Body;
	rp3d::CollisionBody* cylinderBody;
	rp3d::CollisionBody* convexBody;

	WorldCollisionCallback() {
	}

	// This method will be called for contact
	virtual void notifyContact(const rp3d::ContactPointInfo& contactPointInfo) {
		std::cout << "shape1->getBody()->getID()="
				<< contactPointInfo.shape1->getBody()->getID()
				<< " shape2->getBody()->getID()="
				<< contactPointInfo.shape2->getBody()->getID() << std::endl;
	}

};

static void glfw_error_callback(int error, const char* description) {
	std::cout << "Error " << error << ": " << description << std::endl;
}

void camera() {
	/*double cx = 0;
	 double cy = 0;
	 double cz = 0;
	 double angley = 0;
	 double anglex = 0;
	 double r = 0.1;
	 // move camera a distance r away from the center
	 glTranslatef(0, 0, -r);

	 // rotate
	 glRotatef(angley, 0, 1, 0);
	 glRotatef(anglex, 1, 0, 0);

	 // move to center of circle
	 glTranslatef(-cx, -cy, -cz);*/

	glViewport(0, 0, 900, 600);
	//glScale(0.1, 0.1, 0.1);
}

static GLuint axes_list;

void axis() {
	/* Create a display list for drawing axes */
	axes_list = glGenLists(1);
	glNewList(axes_list, GL_COMPILE);

	glColor4ub(0, 0, 255, 255);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.75f, 0.25f, 0.0f);
	glVertex3f(0.75f, -0.25f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.75f, 0.0f, 0.25f);
	glVertex3f(0.75f, 0.0f, -0.25f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.75f, 0.25f);
	glVertex3f(0.0f, 0.75f, -0.25f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.25f, 0.75f, 0.0f);
	glVertex3f(-0.25f, 0.75f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.25f, 0.0f, 0.75f);
	glVertex3f(-0.25f, 0.0f, 0.75f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.25f, 0.75f);
	glVertex3f(0.0f, -0.25f, 0.75f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

	glColor4ub(255, 255, 0, 255);
	glRasterPos3f(1.1f, 0.0f, 0.0f);

	glEndList();
}

int main() {
// ---------- Atributes ---------- //

// Physics world
	rp3d::CollisionWorld* mWorld;

// Bodies
	rp3d::CollisionBody* mConeBody;
	rp3d::CollisionBody* mBoxBody;
	rp3d::CollisionBody* mSphere1Body;
	rp3d::CollisionBody* mSphere2Body;
	rp3d::CollisionBody* mCylinderBody;

// Collision shapes
	rp3d::BoxShape* mBoxShape;
	rp3d::ConeShape* mConeShape;
	rp3d::SphereShape* mSphereShape;
	rp3d::CylinderShape* mCylinderShape;

// Proxy shapes
	rp3d::ProxyShape* mBoxProxyShape;
	rp3d::ProxyShape* mSphere1ProxyShape;
	rp3d::ProxyShape* mSphere2ProxyShape;
	rp3d::ProxyShape* mCylinderProxyShape;

// Collision callback class
	WorldCollisionCallback mCollisionCallback;

// Create the world
	mWorld = new rp3d::CollisionWorld();

// Create the bodies
	mConeShape = new rp3d::ConeShape(2, 5); //radius, height
	rp3d::Transform coneTransform(rp3d::Vector3(12.5, 0, 0),
			rp3d::Quaternion(0, 0.5 * M_PI, 0));
	mConeBody = mWorld->createCollisionBody(coneTransform);
	mConeBody->addCollisionShape(mConeShape, rp3d::Transform::identity());
	std::cout << "mConeBody->getID()=" << mConeBody->getID() << std::endl;

	rp3d::Transform boxTransform(rp3d::Vector3(10, 0, 0),
			rp3d::Quaternion::identity());
	mBoxBody = mWorld->createCollisionBody(boxTransform);
	mBoxShape = new rp3d::BoxShape(rp3d::Vector3(3, 3, 3));
	mBoxProxyShape = mBoxBody->addCollisionShape(mBoxShape,
			rp3d::Transform::identity());

	mSphereShape = new rp3d::SphereShape(3.0);
	rp3d::Transform sphere1Transform(rp3d::Vector3(10, 5, 0),
			rp3d::Quaternion::identity());
	mSphere1Body = mWorld->createCollisionBody(sphere1Transform);
	mSphere1ProxyShape = mSphere1Body->addCollisionShape(mSphereShape,
			rp3d::Transform::identity());

	rp3d::Transform sphere2Transform(rp3d::Vector3(30, 10, 10),
			rp3d::Quaternion::identity());
	mSphere2Body = mWorld->createCollisionBody(sphere2Transform);
	mSphere2ProxyShape = mSphere2Body->addCollisionShape(mSphereShape,
			rp3d::Transform::identity());

	mCylinderShape = new rp3d::CylinderShape(1, 2); //radius, height
	rp3d::Transform cylinderTransform(rp3d::Vector3(-0.3, 0, 0),
			rp3d::Quaternion(0, 0 * M_PI, 0));
	mCylinderBody = mWorld->createCollisionBody(cylinderTransform);
	mCylinderProxyShape = mCylinderBody->addCollisionShape(mCylinderShape,
			rp3d::Transform::identity());
	std::cout << "mCylinderBody->getID()=" << mCylinderBody->getID()
			<< std::endl;

	mCollisionCallback.boxBody = mBoxBody;
	mCollisionCallback.sphere1Body = mSphere1Body;
	mCollisionCallback.sphere2Body = mSphere2Body;
	mCollisionCallback.cylinderBody = mCylinderBody;

	/* window */
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	GLFWwindow* window = glfwCreateWindow(900, 600, "hey",
	NULL,
	NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	//GLEW initialization
	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) {
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(
		EXIT_FAILURE);
	}

	cMesh *mesh1 = new cMesh("cone.obj");
	cMeshEntry* meshEntry1 = mesh1->meshEntries.at(0);
	// Vertex and Indices array for the triangle mesh (data in shared and not copied)
	rp3d::TriangleVertexArray* vertexArray = new rp3d::TriangleVertexArray(
			meshEntry1->nbVertices, (void*) meshEntry1->vertices1D,
			sizeof(float), meshEntry1->nbFaces,
			(void*) meshEntry1->faceIndexes1D, sizeof(int),
			rp3d::TriangleVertexArray::VERTEX_FLOAT_TYPE,
			rp3d::TriangleVertexArray::INDEX_INTEGER_TYPE);

	/*for (unsigned int i = 0; i < meshEntry1->nbVertices * 3; i++) {
	 std::cout << "v1D: " << i << " " << meshEntry1->nbVertices << " "
	 << meshEntry1->vertices1D[i] << std::endl;
	 }
	 for (unsigned int i = 0; i < meshEntry1->nbVertices; i++) {
	 std::cout << "v2D: " << meshEntry1->vertices2D[i][0] << ","
	 << meshEntry1->vertices2D[i][1] << ","
	 << meshEntry1->vertices2D[i][2] << std::endl;
	 }
	 for (unsigned int i = 0; i < meshEntry1->nbFaces * 3; i++) {
	 std::cout << "f1D: " << meshEntry1->faceIndexes1D[i] << std::endl;
	 }*/
	// Create the collision shape for the rigid body (convex mesh shape) and
	// do not forget to delete it at the end
	rp3d::ConvexMeshShape* mConvexShape1 = new rp3d::ConvexMeshShape(
			vertexArray, true);
	rp3d::Transform mConvexShapeTransform1(rp3d::Vector3(1, 0, 0),
			rp3d::Quaternion(0.0 * M_PI, 0.0 * M_PI, 1 * M_PI));
	rp3d::CollisionBody* mConvexBody1 = mWorld->createCollisionBody(
			mConvexShapeTransform1);
	rp3d::ProxyShape* mConvexProxyShape1 = mConvexBody1->addCollisionShape(
			mConvexShape1, rp3d::Transform::identity());

	cMesh *mesh2 = new cMesh("cylinder.obj");
	cMeshEntry* meshEntry2 = mesh2->meshEntries.at(0);
	// Vertex and Indices array for the triangle mesh (data in shared and not copied)
	rp3d::TriangleVertexArray* vertexArray2 = new rp3d::TriangleVertexArray(
			meshEntry2->nbVertices, (void*) meshEntry2->vertices1D,
			sizeof(float), meshEntry2->nbFaces,
			(void*) meshEntry2->faceIndexes1D, sizeof(int),
			rp3d::TriangleVertexArray::VERTEX_FLOAT_TYPE,
			rp3d::TriangleVertexArray::INDEX_INTEGER_TYPE);
	rp3d::ConvexMeshShape* mConvexShape2 = new rp3d::ConvexMeshShape(
			vertexArray2, true);
	rp3d::Transform mConvexShapeTransform2(rp3d::Vector3(3.00, 0, 0),
			rp3d::Quaternion(0, 0, 0));
	rp3d::CollisionBody* mConvexBody2 = mWorld->createCollisionBody(
			mConvexShapeTransform2);
	rp3d::ProxyShape* mConvexProxyShape2 = mConvexBody2->addCollisionShape(
			mConvexShape2, rp3d::Transform::identity());

	mWorld->testCollision(&mCollisionCallback);

	axis();

	glm::vec3 vector1 = glm::vec3(0.0f, 1.0f, 0.0f);
	/* load an image file directly as a new OpenGL texture */
	GLuint tex_2d = SOIL_load_OGL_texture("SpiderTex.jpg", SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
					| SOIL_FLAG_COMPRESS_TO_DXT);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		camera();
		glPushMatrix();
		glCallList(axes_list);
		glPopMatrix();

		mesh1->render();
		/*if (keys[1]) { // f1
		 keys[1] = false;
		 KillGLWindow();
		 fullscreen = !fullscreen;
		 if (!CreateGLWindow(windowTitle, 640, 480, 16, fullscreen,
		 window)) {
		 return 0;
		 }
		 }*/

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
