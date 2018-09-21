#include "Engine.h"
#include <string.h>

#include "stb_image.h"

static float Age = 0;
static int Frame = 0;


Engine::Engine(void)
{
}


Engine::~Engine(void)
{
	DeleteShader();
	mCapture.ReleaseMemory();
}

void Engine::DeleteShader()
{
	
}


void Engine::SupportVersionCheck()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	   /* Problem: glewInit failed, something is seriously wrong. */
		printf(" Load Fail : GLEW Library \n");
		cerr<< glewGetErrorString(err)<<endl;
	}
	else
		printf(" Ready for GLEW Library \n");

	if( glewIsSupported("GL_VERSION_2_0"))
		printf(" Ready for OpenGL 2.0 \n");
	else 
	{
		printf(" OpenGL 2.0 is not supported \n");
		exit(1);
	}
}

void Engine::Initialize()
{
	SupportVersionCheck();

	m_specularMapShader.CreateShader("specular.vert", "specular.frag");
	m_NormalMapShader.CreateShader("normal.vert", "normal.frag");
	m_ParticleShader.CreateShader("Particle.vert", "Particle.frag");

	m_TextureShader.CreateShader("texture.vert","texture.frag");

	m_PatronusShader.CreateShader("patronus.vert", "patronus.frag");

	m_bCapture = false;

	CreateScene();
	
	
}

GLuint* Engine::LoadTexImage(char* texFile, GLuint *tex){

	FILE *fp = fopen(texFile, "rb");
	if (!fp){ printf("ERROR : No %s.\n fail to bind %d\n", texFile, tex);  return false; }

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);
	
	//bind
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	return tex;
}

void Engine::CreateScene()
{

	//Sky

	mSky.LoadMeshFromFile(".\\Meshes\\Sky.obj");
	mSky.TranslateMesh(glm::vec3(0, -5, 0));
	mSky.ScaleMesh(glm::vec3(2.0f, 2.0f, 2.0f));
	
	glGenTextures(1,&mSkyDiffuse);
	glBindTexture(GL_TEXTURE_2D, mSkyDiffuse);
	LoadTexImage(".\\Meshes\\Texture\\Sky.JPG", &mSkyDiffuse);


	//Meshes & Textures

		//Scene Background
#define FIELD_RADIUS  150

	//Grass

	mGrass[0].LoadMeshFromFile(".\\Meshes\\grass1.obj",".\\Meshes\\Texture\\Herbe.jpg");

	for (int i = 0; i < NUM_GRASS; i ++){
		mGrass[i] = Mesh(mGrass[0]);
	}

	for (int i = 0; i < NUM_GRASS; i++){
		int r = (20 + (rand() % (int)(FIELD_RADIUS - 20 + 1)));
		mGrass[i].TranslateMesh( glm::vec3 (-5 +  r * sinf(i * 3.14 * 2.0 / NUM_GRASS), -7.0 , -5 + r * cosf(i * 3.14 * 2.0 / NUM_GRASS )));
	}
	
	glGenTextures(1,&mGrassDiffuse);
	glBindTexture(GL_TEXTURE_2D, mGrassDiffuse);
	LoadTexImage(".\\Meshes\\Texture\\Herbe.jpg", &mGrassDiffuse);

	
	//Trees
	mTree[0].LoadMeshFromFile(".\\Meshes\\DeadTree11.obj",".\\Meshes\\DeadTree11.mtl",true);
	for (int i = 1; i < NUM_TREE; i ++){
		mTree[i] = Mesh(mTree[0]);
	}
	
	for (int i = 0; i < NUM_TREE; i ++){
	
		int r = (rand() % (int)(FIELD_RADIUS - 30 + 1));

		float s = ( 80 + (rand() % (100-80+1)))/100.0f;
		mTree[i].ScaleMesh( glm::vec3(0.2 * s, 0.2 * s, 0.2 * s));
		mTree[i].RotateMesh( AXIS_Y, 33.0 * i);
		mTree[i].TranslateMesh( glm::vec3 (r * sinf(i * 3.14 * 2.0 / NUM_TREE), -20.0 ,r * cosf(i * 3.14 * 2.0 / NUM_TREE )));
	}

	glGenTextures(5, mTreeDiffuse);
	glBindTexture(GL_TEXTURE_2D,mTreeDiffuse[0]);
	LoadTexImage(".\\Meshes\\Texture\\Dead_Tree_StrippedGrey-diff.jpg", &(mTreeDiffuse[0]));
	glBindTexture(GL_TEXTURE_2D,mTreeDiffuse[1]);
	LoadTexImage(".\\Meshes\\Texture\\Dried_Branch_1.tga", &(mTreeDiffuse[1]));
	glBindTexture(GL_TEXTURE_2D,mTreeDiffuse[2]);
	LoadTexImage(".\\Meshes\\Texture\\Dried_Branch_2.tga", &(mTreeDiffuse[2]));
	glBindTexture(GL_TEXTURE_2D,mTreeDiffuse[3]);
	LoadTexImage(".\\Meshes\\Texture\\Dried_Branch_1.tga", &(mTreeDiffuse[3]));
	glBindTexture(GL_TEXTURE_2D,mTreeDiffuse[4]);
	LoadTexImage(".\\Meshes\\Texture\\Dried_Branch_2.tga", &(mTreeDiffuse[4]));

	
	
	//Fox_Animation

	mFoxMesh.LoadMeshFromFile(".\\Meshes\\Run_0.obj",".\\Meshes\\Run_0.mtl",true);
	mFoxMesh.ScaleMesh(glm::vec3(0.05f, 0.05f, 0.05f));

	for (int i = 0; i < NUM_FOX; i++){
		
		int r = FIELD_RADIUS - 10;

		for (int j = 0; j < SIZE_FOX_ANIMATION; j++){
		
			string texname = ".\\Meshes\\Run_";
			char n[2];
			texname.append( (itoa(j,n,10)) );
			texname.append( ".obj");
		
			string mtlname = ".\\Meshes\\Run_";
			mtlname.append( (itoa(j,n,10)) );
			mtlname.append( ".mtl");

			char *tex = new char [ texname.length()+1];
			strcpy(tex,texname.c_str());
			char *mtl = new char [ mtlname.length()+1];
			strcpy(mtl,mtlname.c_str());

			mFoxMeshes[i][j].LoadMeshFromFile(tex, mtl,true);
			mFoxMeshes[i][j].ScaleMesh(glm::vec3(0.05f, 0.05f, 0.05f));
			
			mFoxMeshes[i][j].RotateMesh(AXIS_Y, 180.0f + 36.0f * i);
			mFoxMeshes[i][j].TranslateMesh(glm::vec3(-1.0f + r * sinf(i * 3.14 * 2.0 / NUM_FOX), -5.0 + 4.0f * i, 0.0f + r * cosf(i * 3.14 * 2.0 / NUM_FOX)));

			mFoxMeshes[i][j].StartFrame = 800 + 79 * i;
			mFoxMeshes[i][j].EndFrame = 800 + SIZE_FOX_ANIMATION * 50 + 79 * i;

		}

		mFoxMeshCenterOfMass[i] = mFoxMeshes[i][0].m_fCenterOfMass;
		mFoxMeshVelocity[i] = ( glm::normalize(mFoxMeshCenterOfMass[i]) * -45.0f );
		mFoxMeshVelocity[i].y += 5.0f;

	}

	
	
	glGenTextures(2, mFoxBumped);
	glBindTexture(GL_TEXTURE_2D,mFoxBumped[0]);
	LoadTexImage(".\\Meshes\\Texture\\normal_Fox_0.tga", &(mFoxBumped[0]));

	glBindTexture(GL_TEXTURE_2D,mFoxBumped[1]);
	LoadTexImage(".\\Meshes\\Texture\\normal_Fox_1.tga", &(mFoxBumped[1]));
	
	glGenTextures(2, mFoxDiffuse);
	glBindTexture(GL_TEXTURE_2D,mFoxDiffuse[0]);
	LoadTexImage(".\\Meshes\\Texture\\texture_Fox_0.tga", &(mFoxDiffuse[0]));

	glBindTexture(GL_TEXTURE_2D,mFoxDiffuse[1]);		
	LoadTexImage(".\\Meshes\\Texture\\texture_Fox_1.tga", &(mFoxDiffuse[1]));

	//Dear
	mDeerMesh.LoadMeshFromFile(".\\Meshes\\Deer.obj");
	mDeerMesh.ScaleMesh(glm::vec3(0.3, 0.3, 0.3));
	mDeerMesh.TranslateMesh(glm::vec3(0.0, 0.0, -60.0));
	
	glGenTextures(1, &mDeerDiffuse);
	glBindTexture(GL_TEXTURE_2D,mDeerDiffuse);
	LoadTexImage(".\\Meshes\\Texture\\Deer_body_D.tga", &(mDeerDiffuse));

	glGenTextures(1, &mDeerBumped);
	glBindTexture(GL_TEXTURE_2D,mDeerBumped);
	LoadTexImage(".\\Meshes\\Texture\\Deer_body_N.tga", &(mDeerBumped));

	//Particles


	//Field
	mFieldParticle = ParticleEffect( 300 );

	mFieldParticle.SetParticleEmitter(&mFieldParticleEmitterS);
	mFieldParticle.SetForce ( glm::vec3(0.0f,1.0f, 0.0f));
	mFieldParticle.m_StartSize = 1.0f;
	mFieldParticle.m_EndSize = 10.0f;
//	mDeerParticle.m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

	mFieldParticle.EmitParticles();

	glGenTextures(1, &mParticleBokeh);
	glBindTexture(GL_TEXTURE_2D,mParticleBokeh);
	LoadTexImage(".\\Meshes\\Texture\\Bokeh.PNG", &(mParticleBokeh));
	//LoadTexImage(".\\Meshes\\Texture\\Smoke0391_L.tga", &(mParticleBokeh));


	//Fox_Animation_Particle

#define FOX_PARTICLE_PER_SEC 1

	for(int i = 0 ; i < NUM_FOX; i++){

		mFoxesParticle[i] = ParticleEffect( (int) mFoxMeshes[0][0].mVLists.size() * FOX_PARTICLE_PER_SEC );

		mFoxesParticle[i].SetParticleEmitter(&mFoxesParticleEmitterM[i]);
		mFoxesParticleEmitterM[i].RegisterMesh(mFoxMeshes[0][0]);
		mFoxesParticle[i].SetForce ( glm::vec3(0.0f, -9.8f, 0.0f));
		mFoxesParticle[i].SetParticlePerSec(FOX_PARTICLE_PER_SEC);

		mFoxesParticle[i].m_Position = mFoxMeshes[0][0].m_fCenterOfMass;

		mFoxesParticle[i].EmitParticles();
	}

#define DEER_PARTICLE_PER_SEC 2

	mDeerParticle = ParticleEffect( (int) mDeerMesh.mVLists.size() * DEER_PARTICLE_PER_SEC );

	mDeerParticle.SetParticleEmitter(&mDeerParticleEmitterM);
	mDeerParticleEmitterM.RegisterMesh(mDeerMesh);
	mDeerParticle.SetForce ( glm::vec3(0.0f, 0.0f, 0.0f));
	mDeerParticle.SetParticlePerSec(DEER_PARTICLE_PER_SEC);
//	mDeerParticle.m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

	mDeerParticle.EmitParticles();
		
}


void Engine::Render(glm::mat4 viewM, glm::mat4 modelM, glm::mat4 projM)
{



	glEnable(GL_DEPTH_TEST);

	m_TextureShader.UseProgram();
	
	GLuint tex = m_TextureShader.getLocation("CurrentTex");
	glDisable(GL_LIGHT0);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mSkyDiffuse);
	glUniform1i (tex, 0);

	mSky.RenderTexMesh();
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,0);
	glEnable(GL_LIGHT0);

	//Background Scene
	glDisable(GL_BLEND);

	m_NormalMapShader.UseProgram();
	
	GLuint diffuseTex = m_NormalMapShader.getLocation("CurrentTex");
	GLuint SpecTex = m_NormalMapShader.getLocation("CurrentNMM");
	GLuint UseNormal = m_NormalMapShader.getLocation("useNormalMap");
	
	glUniform1i (UseNormal, false);


	//grass
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,mGrassDiffuse);
	glUniform1i (diffuseTex, 2);

	for (int i = 0; i < NUM_GRASS; i ++){
		
		mGrass[i].RenderTexMesh();
	}

	//trees
	glActiveTexture(GL_TEXTURE3);

	for (int i = 0; i < NUM_TREE; i ++){
		for (int j = 0; j < (int)mTree[0].m_MtlLib.size(); j++){

			glBindTexture(GL_TEXTURE_2D, mTreeDiffuse[j]);
			glUniform1i (diffuseTex, 3);

			mTree[i].RenderTexMeshWithMaterials(j);
		}
	}
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,0);

	//Fox _ Animation
	
	//Meshes
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_COLOR,GL_ONE);


	glUniform1i (UseNormal, true);

	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	m_PatronusShader.UseProgram();

	GLuint cameraPos = m_PatronusShader.getLocation("CameraPos");
	GLuint modelMatrix = m_PatronusShader.getLocation("modelMat");
	GLuint useTex = m_PatronusShader.getLocation("UseTexture");
	GLuint useNMM = m_PatronusShader.getLocation("UseBumped");
	
	diffuseTex = m_PatronusShader.getLocation("CurrentTex");
	SpecTex = m_PatronusShader.getLocation("CurrentNMM");

	float *Mp = glm::value_ptr(modelM);

	glUniform3f(cameraPos, g_Translate[0], g_Translate[1], g_Translate[2] * -1.0f); 
	glUniformMatrix4fv(modelMatrix,1,GL_FALSE,Mp);
	glUniform1i(useTex,true);
	glUniform1i(useNMM,false);


	glUniform1i(useNMM,true);

	//Deer
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,mDeerDiffuse);
	glUniform1i (diffuseTex, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,mDeerBumped);
	glUniform1i (SpecTex, 3);

	mDeerMesh.RenderTexMesh();

	//Fox_Animation
	int index = Frame % SIZE_FOX_ANIMATION;
	
	for (int i = 0; i < NUM_FOX; i++){

		if (mFoxMeshes[i][0].StartFrame <= Frame && mFoxMeshes[i][0].EndFrame > Frame){

			int _index = (Frame - mFoxMeshes[i][0].StartFrame)%SIZE_FOX_ANIMATION;

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D,mFoxDiffuse[0]);
			glUniform1i (diffuseTex, 2);
		
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D,mFoxBumped[0]);
			glUniform1i (SpecTex, 3);

			mFoxMeshes[i][_index].RenderTexMeshWithMaterials(0);
	
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D,0);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D,0);


			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D,mFoxDiffuse[1]);
			glUniform1i (diffuseTex, 2);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D,mFoxBumped[1]);
			glUniform1i (SpecTex, 3);

			mFoxMeshes[i][_index].RenderTexMeshWithMaterials(1);
		}
	}		
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);

	//Particle
	
	glEnable(GL_BLEND);
//	glBlendEquation(GL_FUNC_ADD);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);


	m_ParticleShader.UseProgram();

	GLuint CurrentTex = m_ParticleShader.getLocation("CurrentTex");	
	GLuint input = m_ParticleShader.getLocation("useTex");
	
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,mFoxDiffuse[1]);
	glUniform1i (CurrentTex, 4); //아무거나넣는다 for TexCoord
	glUniform1i (input, false);

	for (int i = 0; i < NUM_FOX; i++){
		
		if (mFoxMeshes[i][0].StartFrame <= Frame && mFoxMeshes[i][0].EndFrame > Frame){

			mFoxesParticle[i].Render();

		}	
	}

	mDeerParticle.Render();

	glDisable(GL_DEPTH_TEST);
	glUniform1i (input, true);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,mParticleBokeh);
	glUniform1i (CurrentTex, 4); //Field Bokeh


	mFieldParticle.Render();
	glEnable(GL_DEPTH_TEST);

	if( m_bCapture == true)
	{
		mCapture.BeginCapture();
	}
	

}

glm::vec3 UpdateMovingMeshPosition(glm::vec3 *curr, glm::vec3 force, float dt){
	
	glm::vec3 dv = force * dt;

	(*curr) += dv;

	return *curr;
}


void Engine::Update(glm::mat4 viewM, glm::mat4 modelM, glm::mat4 projM)
{

	static ElapsedTime elapsedTime;
	
    float fDeltaTime = elapsedTime.GetElapsedTime();
	
	Age += fDeltaTime;
	
	Frame = (int) (Age*80);

	int index = Frame % SIZE_FOX_ANIMATION;
	
	for (int i = 0; i < NUM_FOX; i++){
		if (mFoxMeshes[i][0].StartFrame <= Frame && mFoxMeshes[i][0].EndFrame > Frame){

			int _index = (Frame - mFoxMeshes[i][0].StartFrame)%SIZE_FOX_ANIMATION;

			UpdateMovingMeshPosition(&(mFoxMeshCenterOfMass[i]),mFoxMeshVelocity[i], fDeltaTime);

			mFoxMeshes[i][_index].m_fCenterOfMass = mFoxMeshCenterOfMass[i];

			mFoxesParticleEmitterM[i].UpdateMesh(mFoxMeshes[i][_index], mFoxMeshVelocity[i]);
			mFoxesParticle[i].Update(fDeltaTime,g_Translate, g_Rotate,viewM,modelM);
		}
		else if (mFoxMeshes[i][0].EndFrame <= Frame) {
			mFoxMeshes[i][0].StartFrame += 133;
			mFoxMeshes[i][0].EndFrame += 133;
		}
	}

	mDeerParticle.Update(fDeltaTime,g_Translate,g_Rotate,viewM,modelM);
	
	mFieldParticle.Update(fDeltaTime,g_Translate,g_Rotate,viewM,modelM);
	
	glutPostRedisplay();

}


void Engine::Key(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'c':
			if( m_bCapture == false )
				mCapture.CreateFolder();
			m_bCapture = !m_bCapture;
			break;

	}
}