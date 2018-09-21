#include "ParticleEffect.h"



SphereEmitter::SphereEmitter()
: MinimumRadius(0)
, MaximumRadius(1)
, MinInclination(0)
, MaxInclination(180)
, MinAzimuth(0)
, MaxAzimuth(360)
, MinSpeed(10)
, MaxSpeed(20)
, MinLifetime(3)
, MaxLifetime(5)
, Origin(0)
{
	type = EMITTER_SPHERE;
}

// Generate a random number between [0..1)
inline float Random()
{
    return rand() / (float)RAND_MAX;
}

inline float RandRange( float fMin, float fMax )
{
    if ( fMin > fMax ) std::swap( fMin, fMax );
    return ( Random() * ( fMax - fMin ) ) + fMin;
}

inline glm::vec3 RandUnitVec()
{
    float x = ( Random() * 2.0f ) - 1.0f;
    float y = ( Random() * 2.0f ) - 1.0f;
    float z = ( Random() * 2.0f ) - 1.0f;

    return glm::normalize( glm::vec3(x,y,z) );
}

void SphereEmitter::EmitParticle( Particle& particle )
{
    float inclination = glm::radians( RandRange( MinInclination, MaxInclination ) );
    float azimuth = glm::radians( RandRange( MinAzimuth, MaxAzimuth ) );

    float radius = RandRange( MinimumRadius, MaximumRadius );
    float speed = RandRange( MinSpeed, MaxSpeed );
    float lifetime = RandRange( MinLifetime, MaxLifetime );

    float sInclination = sinf( inclination );

    float X = sInclination * cosf( azimuth );
    float Y = sInclination * sinf( azimuth );
    float Z = cosf( inclination );

    glm::vec3 vector( X, Y, Z );

    particle.m_Position = ( vector * radius ) + Origin;
    particle.m_Velocity = vector * speed;

    particle.m_fLifeTime = lifetime;
    particle.m_fAge = 0;
}

ElapsedTime::ElapsedTime( float maxTimeStep /* = 0.03333f */ )
: m_fMaxTimeStep( maxTimeStep )
, m_fPrevious ( std::clock() / (float)CLOCKS_PER_SEC )
{}

float ElapsedTime::GetElapsedTime() const
{
    float fCurrentTime = std::clock() / (float)CLOCKS_PER_SEC;
    float fDeltaTime = fCurrentTime - m_fPrevious;
    m_fPrevious = fCurrentTime;

    // Clamp to the max time step
    fDeltaTime = std::min( fDeltaTime, m_fMaxTimeStep );

    return fDeltaTime;
}



LineEmitter::LineEmitter()
: MinWidth(0)
, MaxWidth(1)
, MinSpeed(1)
, MaxSpeed(5)
, MinLifetime(1)
, MaxLifetime(2)
, Origin(0)
, Direction(0)
{
	type = EMITTER_LINE;
}


void LineEmitter::EmitParticle( Particle& particle )
{
 //   float length = RandRange( MinWidth, MaxWidth );
    
    float lifetime = RandRange( MinLifetime, MaxLifetime );
    float speed = RandRange( MinSpeed, MaxSpeed );

    particle.m_Position = Direction + Origin;
    particle.m_Velocity = glm::normalize(Direction) * speed;

    particle.m_fLifeTime = lifetime;
    particle.m_fAge = 0;
}

void LineEmitter::SetDirection (glm::vec3 vec){
	Direction =  vec;
}

void LineEmitter::SetOrigin (glm::vec3 vec){
	Origin = vec;
}


MeshEmitter::MeshEmitter()
: MinWidth(0)
, MaxWidth(1)
, MinSpeed(1)
, MaxSpeed(5)
, MinLifetime(0.1)
, MaxLifetime(2)
, Origin(0)
, Direction(0)
{
	type = EMITTER_MESH;
}

void MeshEmitter::EmitParticle( Particle& particle){

}
void LineEmitter::EmitParticle( Particle& particle, int i){

}
void SphereEmitter::EmitParticle( Particle& particle, int i){

}

void MeshEmitter::EmitParticle (Particle& particle, int index)
{

	float lifetime = RandRange( MinLifetime, MaxLifetime );
	float speed = RandRange( MinSpeed, MaxSpeed );

//	particle.m_Position = Direction[index] + Origin[index];
	particle.m_Position = Origin[index];
	particle.m_Velocity = glm::normalize(Direction[index]) * speed;

	particle.m_fLifeTime = lifetime;
	particle.m_fAge = 0;
	particle.m_CameraDistance = 0.0f;
}

void MeshEmitter::RegisterMesh (Mesh mesh){

	Origin.clear();
	Direction.clear();

	int v_n = mesh.mVLists.size();
	CenterOfMesh = mesh.m_fCenterOfMass;
	Velocity = glm::vec3 (0.0);

	for (int i = 0; i <v_n; i++){
		Origin.push_back(mesh.mVLists[i].m_fVertexPosition);
	//	Origin[i] += CenterOfMesh;
		Direction.push_back(mesh.mVLists[i].m_fVertexNormal);
		Direction[i] += Velocity;
	}

}


void MeshEmitter::UpdateMesh (Mesh mesh, glm::vec3 velocity){

	Origin.clear();
	Direction.clear();

	int v_n = mesh.mVLists.size();
	CenterOfMesh = mesh.m_fCenterOfMass;
	Velocity = (velocity * -1.0f);

	for (int i = 0; i <v_n; i++){
		Origin.push_back(mesh.mVLists[i].m_fVertexPosition);
		Origin[i] += CenterOfMesh;
		Direction.push_back(mesh.mVLists[i].m_fVertexNormal);
		Direction[i] += Velocity;
	}

}

void MeshEmitter::SetDirection (glm::vec3 vec, int index){
	Direction[index] =  vec;
}

void MeshEmitter::SetOrigin (glm::vec3 vec, int index){
	Origin[index] = vec;
}


SquareEmitter::SquareEmitter()
: MinWidth(0)
, MaxWidth(400)
, MinInclination(0)
, MaxInclination(180)
, MinSpeed(0.1)
, MaxSpeed(1.0)
, MinLifetime(15)
, MaxLifetime(60)
, Origin(-150.0,-5.0,-150.0)
, Direction(0.0, 1.0, 0.0)
{
	type = EMITTER_SQUARE;
}

void SquareEmitter::EmitParticle( Particle& particle )
{
    float x = RandRange( MinWidth, MaxWidth );
    float z = RandRange( MinWidth, MaxWidth );

    float lifetime = RandRange( MinLifetime, MaxLifetime );
    float speed = RandRange( MinSpeed, MaxSpeed );

	glm::vec3 pos = glm::vec3(x, 0.0, z);

    particle.m_Position = pos + Origin;
    particle.m_Velocity = glm::normalize(Direction) * speed;

    particle.m_fLifeTime = lifetime;
    particle.m_fAge = 0;
}

void SquareEmitter::EmitParticle (Particle& particle, int n)
{

}
