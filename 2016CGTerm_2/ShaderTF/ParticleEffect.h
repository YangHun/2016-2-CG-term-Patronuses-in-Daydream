#pragma once

#include "Particle.h"
#include "Interpolator.h"
#include "ParticleEmitter.h"

class Camera;

class ParticleEffect
{
public:

    // A vertex for the particle
    struct Vertex
    {
        Vertex()
            : m_Pos(0)
            , m_Diffuse(0)
            , m_Tex0(0)
        {}

        glm::vec3   m_Pos;      // Vertex position
        glm::vec4   m_Diffuse;  // Diffuse color
        glm::vec2   m_Tex0;     // Texture coordinate
    };
    typedef std::vector<Vertex> VertexBuffer;
    typedef Interpolator<glm::vec4> ColorInterpolator;
	   
	

    ParticleEffect( unsigned int numParticles = 0 );
    virtual ~ParticleEffect();

 //   void SetCamera( Camera* pCamera );
    void SetParticleEmitter( ParticleEmitter* pEmitter );
    void SetColorInterplator( const ColorInterpolator& colors );

	void SetForce( glm:: vec3 vec);

    // Test method to randomize the particles in an interesting way
 //   void RandomizeParticles();
    void EmitParticles();
	
    virtual void Update(float fDeltaTime, float cameraPos[], float cameraRot[],glm::mat4 vlewM, glm::mat4 modelM);
    virtual void Render();

    bool LoadTexture( char* fileName );
    
    // Resize the particle buffer with numParticles
   void Resize( unsigned int numParticles );

protected:
 //   void RandomizeParticle( Particle& particle );
    void EmitParticle( Particle& particle );
	void EmitParticle( Particle& particle, int n );

public :
	bool SortParticles();

public:
    // Build the vertex buffer from the particle buffer
    void BuildVertexBuffer(float rotx, float roty, glm::mat4 modelV);
	void SetParticlePerSec (int n);

public:
    Camera*             m_pCamera;
    ParticleEmitter*    m_pParticleEmitter;
    ColorInterpolator   m_ColorInterpolator;

	GLuint              m_TextureID;
    ParticleBuffer      m_Particles;
    VertexBuffer        m_VertexBuffer;
    glm::mat4x4         m_LocalToWorldMatrix;

    // Apply this force to every particle in the effect
    glm::vec3           m_Force;

	glm::vec3			m_CameraPos;
	glm::vec3			m_Position;

	int					m_ParticlePerSec;
	int					m_numParticles;

	float				m_StartSize;
	float				m_EndSize;
};