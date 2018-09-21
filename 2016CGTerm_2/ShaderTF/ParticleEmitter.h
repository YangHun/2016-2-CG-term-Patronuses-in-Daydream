#include <vector>

#include "Particle.h"
#include "Mesh.h"

using namespace std;

enum EmitterType {
	EMITTER_NONE,
	EMITTER_SPHERE,
	EMITTER_LINE,
	EMITTER_MESH,
	EMITTER_SQUARE
};

class ParticleEmitter
{
public:
    virtual ~ParticleEmitter() {}
    virtual void EmitParticle( Particle& particle ) = 0;
	virtual void EmitParticle (Particle& particle, int n) = 0;

	EmitterType type;
//    virtual void DebugRender() {}
};

/**
 * ParticleEffect class that demonstrates one possible implementation for
 * billboard particles.
 */

class SphereEmitter : public ParticleEmitter
{
public:
    SphereEmitter();
    virtual void EmitParticle( Particle& particle );
	virtual void EmitParticle (Particle& particle, int n);
//    virtual void DebugRender();

    float MinimumRadius;
    float MaximumRadius;

    float MinInclination;
    float MaxInclination;

    float MinAzimuth;
    float MaxAzimuth;

    float MinSpeed;
    float MaxSpeed;

    float MinLifetime;
    float MaxLifetime;

    glm::vec3   Origin;
    
private: 
//    void RenderSphere( glm::vec4 color, float fRadius );
};

class LineEmitter : public ParticleEmitter {
	public:
    LineEmitter();
    virtual void EmitParticle( Particle& particle );
	virtual void EmitParticle (Particle& particle, int n);
	void SetDirection (glm::vec3 vec);
	void SetOrigin (glm::vec3 vec);
	//    virtual void DebugRender();

    float MinWidth;
    float MaxWidth;

    float MinSpeed;
    float MaxSpeed;

    float MinLifetime;
    float MaxLifetime;

    glm::vec3   Origin;
	glm::vec3	Direction;
};

class MeshEmitter : public ParticleEmitter {
	public:
    MeshEmitter();
	virtual void EmitParticle (Particle& particle);
	virtual void EmitParticle (Particle& particle, int n);
	void RegisterMesh(Mesh mesh);
	void UpdateMesh(Mesh mesh, glm::vec3 velocity);

	//    virtual void DebugRender();

    float MinWidth;
    float MaxWidth;

    float MinSpeed;
    float MaxSpeed;

    float MinLifetime;
    float MaxLifetime;

	vector <glm::vec3>  Origin;
	vector <glm::vec3>	Direction;
	glm::vec3 CenterOfMesh;
	glm::vec3 Velocity;

private: 
	void SetDirection (glm::vec3 vec, int index);
	void SetOrigin (glm::vec3 vec, int index);
	
};

class SquareEmitter : public ParticleEmitter
{
public:
    SquareEmitter();
    virtual void EmitParticle( Particle& particle );
	virtual void EmitParticle (Particle& particle, int n);
//    virtual void DebugRender();

	float MinWidth;
	float MaxWidth;

    float MinInclination;
    float MaxInclination;

    float MinSpeed;
    float MaxSpeed;

    float MinLifetime;
    float MaxLifetime;

    glm::vec3   Origin;
	glm::vec3	Direction;

    
private: 
//    void RenderSphere( glm::vec4 color, float fRadius );
};