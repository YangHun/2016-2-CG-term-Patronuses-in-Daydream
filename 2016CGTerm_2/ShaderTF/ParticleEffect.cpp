
#include <string>
#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

#include <gl/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "ParticleEffect.h"

#include "stb_image.h"

ParticleEffect::ParticleEffect( unsigned int numParticles /* = 0 */ )
: m_pCamera( NULL )
, m_pParticleEmitter( NULL )
, m_ColorInterpolator( glm::vec4(1) )
, m_LocalToWorldMatrix(1)
, m_TextureID(0)
, m_Force( 0, -9.81f, 0 ) 
, m_ParticlePerSec(1)
, m_numParticles(numParticles)
, m_CameraPos(0)
, m_StartSize (3.0)
, m_EndSize(0.0)
{
    Resize(numParticles);
	glGenTextures(1,&m_TextureID);
}

ParticleEffect::~ParticleEffect()
{
    if ( m_TextureID != 0 )
    {
        glDeleteTextures( 1, &m_TextureID );
        m_TextureID = 0;
    }
}

void ParticleEffect::Resize( unsigned int numParticles )
{
    m_Particles.resize( numParticles,  Particle() );
    m_VertexBuffer.resize( numParticles * 4, Vertex() );
}


void ParticleEffect::SetParticleEmitter( ParticleEmitter* pEmitter )
{
    m_pParticleEmitter = pEmitter;
}

void ParticleEffect::SetColorInterplator( const ColorInterpolator& colors )
{
    m_ColorInterpolator = colors;
}

void ParticleEffect::SetForce(glm::vec3 vec){
	m_Force = vec;
}

void ParticleEffect::EmitParticle( Particle& particle )
{
	 assert( m_pParticleEmitter != NULL );
    m_pParticleEmitter->EmitParticle( particle );
}

void ParticleEffect::EmitParticle( Particle& particle, int n )
{
	assert( m_pParticleEmitter != NULL );
    m_pParticleEmitter->EmitParticle( particle, n );
}

void ParticleEffect::EmitParticles()
{
	SortParticles();
	if( m_pParticleEmitter->type != EMITTER_MESH){
		for ( unsigned int i = 0; i < m_Particles.size(); ++i )
			{
				EmitParticle(m_Particles[i]);
		}
	}
	else{
		for (int i = 0; i < m_Particles.size(); ++i ) {
			EmitParticle(m_Particles[i], i / m_ParticlePerSec );	
		}
	}
}

bool ParticleEffect::SortParticles(){

	std::sort (&m_Particles[0], &m_Particles[m_numParticles-1]);
	return true;
}

void ParticleEffect::Update(float fDeltaTime, float cameraPos[], float cameraRot[], glm::mat4 viewM, glm::mat4 modelM)
{
	m_CameraPos = glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2]);
	m_CameraPos *= -1;

	glm::vec3 CameraDir = glm::vec3(viewM[2][0], viewM[2][1],viewM[2][2]);

	glm::quat _camerarot = glm::quat(glm::radians (glm::vec3(cameraRot[0],cameraRot[1],0) * -1.0f));
	
	//std::cout << "CameraDir : " << CameraDir.x << " " << CameraDir.y << " " << CameraDir.z << " " <<endl;


	//m_CameraPos = _camerarot * m_CameraPos;
	glm::vec3 CameraLookat = _camerarot * CameraDir;

	//std::cout << "CameraLookat : " << CameraLookat.x << " " << CameraLookat.y << " " << CameraLookat.z << " " <<endl;

    for ( int i = 0; i < (int)m_Particles.size(); i++ ){
        Particle &particle = m_Particles[i];

        particle.m_fAge += fDeltaTime;

        if ( particle.m_fAge > particle.m_fLifeTime )
        {
            if ( m_pParticleEmitter != NULL ){
			
				switch( m_pParticleEmitter->type){
				case EMITTER_NONE:
				case EMITTER_LINE:
				case EMITTER_SPHERE:
				case EMITTER_SQUARE:
					EmitParticle(particle);
					break;
				case EMITTER_MESH:
					EmitParticle(particle, i / m_ParticlePerSec);

					break;
				}
			}
        }
		
        float lifeRatio = glm::saturate(particle.m_fAge / particle.m_fLifeTime);
        particle.m_Velocity += ( m_Force * fDeltaTime );
		particle.m_Position += ( particle.m_Velocity * fDeltaTime);
        particle.m_Color = m_ColorInterpolator.GetValue( lifeRatio );
        particle.m_fRotate = glm::lerp<float>( 0.0f, 180.0f, lifeRatio );
        particle.m_fSize = glm::lerp<float>( m_StartSize, m_EndSize, lifeRatio );

		float x_sum = 0.0f, y_sum = 0.0f, z_sum = 0.0f;
		x_sum = m_CameraPos.x * particle.m_Position.x;
		y_sum = m_CameraPos.y * particle.m_Position.y;
		z_sum = m_CameraPos.z * particle.m_Position.z;
		particle.m_CameraDistance = x_sum + y_sum + z_sum;
	}

	SortParticles();
    BuildVertexBuffer(cameraRot[0], cameraRot[1], viewM);

}

void ParticleEffect::SetParticlePerSec (int n){
	m_ParticlePerSec = n;
}

void ParticleEffect::BuildVertexBuffer(float rotx, float roty, glm::mat4 viewM)
{
    const glm::vec3 X( 0.5, 0, 0 );
    const glm::vec3 Y( 0, 0.5, 0 );
    const glm::vec3 Z( 0, 0 ,1.0 );

    glm::quat cameraRotation;

    cameraRotation = glm::quat( glm::radians( glm::vec3(rotx,roty,0.0f) ) );    
    
    // Make sure the vertex buffer has enough vertices to render the effect
    // If the vertex buffer is already the correct size, no change is made.
    m_VertexBuffer.resize(m_Particles.size() * 4, Vertex() );

    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        Particle& particle = m_Particles[i];
        glm::quat rotation = glm::angleAxis( particle.m_fRotate, Z );

        unsigned int vertexIndex = i * 4;
        Vertex& v0 = m_VertexBuffer[vertexIndex + 0];   // Bottom-left
        Vertex& v1 = m_VertexBuffer[vertexIndex + 1];   // Bottom-right
        Vertex& v2 = m_VertexBuffer[vertexIndex + 2];   // Top-right
        Vertex& v3 = m_VertexBuffer[vertexIndex + 3];   // Top-left

        // Bottom-left
        v0.m_Pos = particle.m_Position + ( rotation * ( -X - Y ) * particle.m_fSize ) * cameraRotation;
        v0.m_Tex0 = glm::vec2( 0, 1 );
        v0.m_Diffuse = particle.m_Color;

        // Bottom-right
        v1.m_Pos = particle.m_Position + ( rotation * ( X - Y ) * particle.m_fSize ) * cameraRotation;
        v1.m_Tex0 = glm::vec2( 1, 1 );
        v1.m_Diffuse = particle.m_Color;

        // Top-right
        v2.m_Pos = particle.m_Position + ( rotation * ( X + Y ) * particle.m_fSize ) * cameraRotation;
        v2.m_Tex0 = glm::vec2( 1, 0 );
        v2.m_Diffuse = particle.m_Color;

        // Top-left
        v3.m_Pos = particle.m_Position + ( rotation * ( -X + Y ) * particle.m_fSize ) * cameraRotation;
        v3.m_Tex0 = glm::vec2( 0, 0 );
        v3.m_Diffuse = particle.m_Color;

		glm::vec3 pos = ( v0.m_Pos + v2.m_Pos ) / 2.0f;

    }    
}


void ParticleEffect::Render()
{
//    glDisable(GL_DEPTH_TEST);           // Disables Depth Testing

    glPushMatrix();
    glMultMatrixf( glm::value_ptr(m_LocalToWorldMatrix) );
   
//	glActiveTexture(GL_TEXTURE0+m_TextureID);
//	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Pos) );
    glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Tex0) );
    glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Diffuse) );

    glDrawArrays( GL_QUADS, 0, m_VertexBuffer.size() );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );	

    glBindTexture( GL_TEXTURE_2D, 0 );
    glPopMatrix();

}


bool ParticleEffect::LoadTexture( char* fileName ){
	FILE *fp = fopen(fileName, "rb");
	if (!fp){ printf("ERROR : No %s.\n fail to bind %d\n", fileName, m_TextureID);  return false; }

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);

	glGenTextures(1, &m_TextureID);

	//bind
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	
	std::cout << m_TextureID; 

    return ( m_TextureID != 0 );
}
