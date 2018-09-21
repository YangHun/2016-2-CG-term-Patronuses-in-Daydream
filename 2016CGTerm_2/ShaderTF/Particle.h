#pragma once

#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <vector>

struct Particle
{
public:
    Particle() 
        : m_Position(0)
        , m_Velocity(0)
        , m_Color(0)
        , m_fRotate(0)
        , m_fAge(0)
        , m_fLifeTime(0)
		, m_CameraDistance(0)
    {}

    glm::vec3   m_Position; // Center point of particle
    glm::vec3   m_Velocity; // Current particle velocity
    glm::vec4   m_Color;    // Particle color
    float       m_fRotate;  // Rotate the particle the center
    float       m_fSize;    // Size of the particle
    float       m_fAge;
    float       m_fLifeTime;
	float		m_CameraDistance; //for Depth Sorting

	bool operator<(Particle& that){
		return this->m_CameraDistance < that.m_CameraDistance;
	}

public : 
	void SetPosition ( glm::vec3 pos){
		m_Position = pos;
	}
};

typedef std::vector<Particle> ParticleBuffer;

// Helper class to count frame time
class ElapsedTime
{
public:
    ElapsedTime( float maxTimeStep = 0.03333f );
    float GetElapsedTime() const;

private:
    float m_fMaxTimeStep;
    mutable float m_fPrevious;
};
