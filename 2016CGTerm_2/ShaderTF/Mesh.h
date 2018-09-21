#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <iterator>

#include <gl/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Vertex.h"
#include "Face.h"
#include "Edge.h"

#include "glm\glm.hpp"
#include "glm\ext.hpp"


using namespace std;

#define VERTEX_NORMAL 1
#define FACE_NORMAL 2

//Transformation 
#define AXIS_X		1
#define AXIS_Y		2
#define AXIS_Z		3

struct Material;

class Mesh
{
public:
	Mesh(void);
	Mesh(const Mesh &);
	~Mesh(void);

//Mesh Properties
public:
	int				m_nNumberOfVertex;
	int				m_nNumberOfFace;
	int				m_nNumberOfEdge;
	int				StartFrame;
	int				EndFrame;
	
	glm::vec3			m_fCenterOfMass;
	glm::vec3			m_fDistancetoCOM;
	float			m_fRotateAngle;
//Exception syntex
public:
	bool			m_bIsTextureCoord;
	bool			m_bIsVertexNormal;
	int				m_nMinFaceIdx;

//Mesh Structure : vertex, face, edge, texcoordnate
public:
	GLuint			mTexture;
	
	vector<Vertex>  mVLists;
	vector<Face>	mFaces;
	vector<Edge>	mEdges;
	vector<glm::float2>	mTexCoordinates;
	
	bool			m_bUseMtlLib;
	vector<Material> m_MtlLib;

//Public Function
public:
	bool	LoadMeshFromFile(char* filename);
	bool	LoadMeshFromFile(char* filename, char* texFile);
	bool	LoadMeshFromFile(char* filename, char* mtlFile, bool flag_material);
	void	AddNeighbor();
	void	LaplacianSmoothing();
	void	ComputeVertexNormal();
	void	ComputeFaceNormal();
	void	ComputeEdgeNormal();
	void	RenderTexMesh();
	void	RenderTexMeshWithMaterials(int index);
	void	RenderSolidMesh(int nNormalType);
	void	RenderWireMesh();
	void	RenderVertexNormal();
	void	RenderFaceNormal();
	void	RenderEdgeNormal();

private:
	bool	LoadMaterials(char* filename);
	
	//Transformation function
public:
	void	TranslateMesh(glm::vec3 translate);
	void	RotateMesh(int nAxis, float angle);
	void	ScaleMesh(glm::vec3 scale);

//Inline Min/Max function
public:
	inline int minValue(int a, int b) { return (a > b) ? b : a; }
	inline int maxValue(int a, int b) { return (a < b) ? b : a; }

//Print Function
public:
	void PrintMeshInformation(char* meshName);
};

struct Material {
	string name;
	glm::vec2 faceindex;
	float Ns, Ni;
	float d, Tr;
	float illum;
	glm::vec3 Tf, Ka, Kd, Ks, Ke;

	GLuint map_Ka; //Ambient Texture
	GLuint map_Kd; //Diffuse Relectivity
	GLuint map_d;  //Dissolve
};