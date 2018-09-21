#pragma once

#include <Windows.h>
#include <iostream>
#include <gl/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include"ScreenCapture.h"
#include "Shader.h"
#include "Mesh.h"
#include "textfile.h"

#include "Particle.h"
#include "ParticleEffect.h"

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#define SIZE_FOX_ANIMATION 25
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800


using namespace std;

class Engine
{
public:
	Engine(void);
	~Engine(void);

public:
	void SupportVersionCheck();
	void Initialize();
	void Render(glm::mat4 viewM, glm::mat4 modelM, glm::mat4 projM);
	void Update(glm::mat4 viewM, glm::mat4 modelM, glm::mat4 projM);
	void Key(unsigned char key, int x, int y);
	GLuint* LoadTexImage(char* texFile, GLuint *tex);

public:
	//Textures
	/* GL_TEXTURE0 Background Scene Diffuse
	 * GL_TEXTURE1 Background Scene Bumped (필요하면)
	 * GL_TEXTURE2 Animated Mesh Diffuse
	 * GL_TEXTURE3 Animated mesh Bumped (normalMap)
	 * GL_TEXTURE4 Particle Diffuse
	 * GL_TEXTURE5
	*/

	GLuint			mFoxDiffuse[2];
	GLuint			mFoxBumped[2];

	GLuint			mDeerDiffuse;
	GLuint			mDeerBumped;

	GLuint			mGrassDiffuse;
	GLuint			mSkyDiffuse;
	GLuint			mTreeDiffuse[5];

	GLuint			mParticleBokeh;


	
	//Scene Background
#define NUM_GRASS	180 //100
	Mesh			mGrass[NUM_GRASS];
#define NUM_TREE	50 //40
	Mesh			mTree[NUM_TREE];
	Mesh			mPlain;	
	Mesh			mSky;

	//Animals
#define NUM_FOX		10 //3
	glm::vec3		mFoxMeshVelocity[NUM_FOX];
	glm::vec3		mFoxMeshCenterOfMass[NUM_FOX];
	Mesh			mFoxMesh;
	Mesh			mFoxMeshes[NUM_FOX][SIZE_FOX_ANIMATION];

	Mesh			mDeerMesh;

	//ParticleEffect

#define SIZE_FOX_PARTICLE	1553	//fox vList size
//	ParticleEffect	mFoxParticle[SIZE_FOX_PARTICLE]; //for LineEmitter
//	SphereEmitter	mParticleEmitterS;
//	LineEmitter		mFoxParticleEmitterL[SIZE_FOX_PARTICLE];
	MeshEmitter		mFoxParticleEmitterM;

	ParticleEffect	mFoxesParticle[NUM_FOX];
	MeshEmitter		mFoxesParticleEmitterM[NUM_FOX];

	ParticleEffect	mDeerParticle;
	MeshEmitter		mDeerParticleEmitterM;
	
	ParticleEffect	mFieldParticle;
	SquareEmitter	mFieldParticleEmitterS;

		
	//For TF
	GLuint			mironmanDiffuse;
	GLuint			mironmanSpecular;
	


	//Screen Capture
	bool			m_bCapture;
	ScreenCapture	mCapture;

	//Shader
	CShader			m_toonShader;
	CShader			m_impToonShader;
	CShader			m_waveShader;
	CShader			m_lightShader;
	CShader			m_specularMapShader;
	CShader			m_lambertShader;

	CShader			m_TAlightShader;
	CShader			m_AlphaShader;
	CShader			m_ParticleShader;
	CShader			m_TextureShader;
	CShader			m_NormalMapShader;
	CShader			m_PatronusShader;

	float			m_fAngle;
	float			m_fTime;

	void	DeleteShader();
	
public:
	void	SphereScene();
	void	CreateScene();

};


//Camera
extern float					g_Zoom;				// view zoom
extern float					g_Rotate[3];		// x,y,z
extern float					g_Translate[3];		// x,y,z
extern glm::vec3				g_CameraLook;		//Lookat Vector