
#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "../shader.hpp"

enum light_type {spotlight, ambientlight, pointlight};

class light{

	private:
		glm::vec3 position; // (x,y,z) position
		glm::vec3 direction; // (x,y,z) center of the cone of light. Optional
		
		glm::vec3 amb = {0.25, 0.25, 0.25};
		glm::vec3 dif = {0.5, 0.5, 0.5};
		glm::vec3 spec = {0.75, 0.75, 0.75};

		glm::vec3 factors = {1., 2., 3.};

		float innerCutoff = 25;
	    float outerCutoff = 30;	

		std::shared_ptr<OKengine::shader> shadow_shader; // ponter to the shader
														 // for doing shadows.
		std::shared_ptr<OKengine::shader> regular_shader;											

		light_type type; // type of light
		
		GLuint depthFBO = 0; // buffer for shadow mapping
		GLuint depthTex = 0; // output for shadow map
		// shadow map resolution
		const unsigned int bufferWidth = 1024; 
		const unsigned int bufferHeight = 1024;

		glm::mat4 proj;
		glm::mat4 view;

		bool dirty_ = true;

		void recomputeMatrices();

	public:

		/**
		 * @brief Default constructor, treated as ambientlight at high noon.
		 **/
		light();
		/**
		 * @brief Constructor for a pointlight at a given position.
		 *
		 * @param pos: the position of the point light (x,y,z)
		 **/
		light(glm::vec3 pos);
		/**
		 * @brief Constructor for a spotlight at a given position, pointed at a given direction
		 *
		 * @param pos: the position of the spotlight (x,y,z)
		 * @param dir: the direction the spotlight is pointing in.
		 **/
		light(glm::vec3 pos, glm::vec3 dir);

		/**
		 * @brief A general constructor which assumes nothing about the type of light.
		 *
		 * @param t: the type of light (light_type enum defined above).
		 * @param pos: [optional] position for the light source, defaults to [1,0,0]
		 * @param dir: [optional] direction for the light, defaults to [1,0,0]
		 **/ 
		light(light_type t, glm::vec3 pos=glm::vec3(1.0,0,0), glm::vec3 dir=glm::vec3(1.0,0,0));

		bool operator ==(const light & other);

		/**
		 * @brief Setter for the shadow shader
		 *
		 * @param path: path to the shadow shader, excluding file extension.
		 **/
		void setShadowShader(std::string path);

		/**
		 * @brief Setter for the shadow shader.
		 *
		 * @param shadow: a precompiled shader program for calculating the shadows.
		 **/
		void setShadowShader(std::shared_ptr<OKengine::shader> shadow);

		/**
		 * @brief Setter for the shadow shader
		 *
		 * @param shadow: a referennce to the new shadow shader.
		 **/
		void setShadowShader(const OKengine::shader & shadow);

		/**
		 * @brief Setter for the shadow shader
		 *
		 * @param path: path to the regular shader, excluding file extension.
		 **/
		void setShader(std::string path);

		/**
		 * @brief Setter for the regular shader.
		 *
		 * @param shadow: a precompiled shader program for calculating the shadows.
		 **/
		void setShader(std::shared_ptr<OKengine::shader> shadow);

		/**
		 * @brief Setter for the regular shader
		 *
		 * @param shadow: a referennce to the new shadow shader.
		 **/
		void setShader(const OKengine::shader & shadow);

		/**
		 * @brief mutator for light position
		 *
		 * @param delta: how much to move by in each axis, i.e. pos + (x,y,z)
		 **/
		void moveBy(glm::vec3 delta);

		/**
		 * @brief mutator for the light direction
		 *
		 * @param delta: how much to move the focal point of the cone of light by,
		 * i.e. dir + (x,y,z)
		 **/
		 void tiltBy(glm::vec3 delta);

		 /**
		  * @brief setter for the light position
		  *
		  * @param loc: the (x,y,z) coordinates to set the light's position to.
		  **/
		 void setPosition(glm::vec3 pos);

		 /**
		  * @brief setter for the light direction
		  *
		  * @param loc: the (x,y,z) coordinates to set the focal point of the 
		  * cone of light to.
		  **/
		 void setDirection(glm::vec3 dir);

		 /**
		  * @brief setter for the ambient component of the lighting.
		  *
		  * @param amb: the new value (which shall completely replace the old value)
		  * of the ambient component; glm::vec3 type.
		  **/
		 void setAmbient(glm::vec3 amb);
		 /**
		  * @brief setter for the specular component of the lighting.
		  *
		  * @param spec: the new value (which shall completely replace the old value)
		  * of the specular component; glm::vec3 type.
		  **/
		 void setSpecular(glm::vec3 spec);
		 /**
		  * @brief setter for the diffuse component of the lighting.
		  *
		  * @param dif: the new value (which shall completely replace the old value)
		  * of the diffuse component; glm::vec3 type.
		  **/
		 void setDiffuse(glm::vec3 dif);

		 /**
		  * @brief setter for the attenuation factors (constant, linear, quadratic).
		  *
		  * @param f: vec3 with (constant, linear, quadratic) factors.
		  **/
		 void setFactors(glm::vec3 f);
		 /**
		  * @brief setter for the inner cutoff angle of a spotlight.
		  *
		  * @param c: the inner cutoff angle in degrees.
		  **/
		 void setCutoff(float c);
		 /**
		  * @brief setter for the outer cutoff angle of a spotlight.
		  *
		  * @param c: the outer cutoff angle in degrees.
		  **/
		 void setOuterCutoff(float c);

		 /**
		  * @brief method to bind the relevant uniforms for lighting
		  **/
		 void bind();

		 /**
		  * @brief method to setup for the first pass of rendering (shadowmapping).
		  **/
		 void prepareShadows();

		 /**
		  * @brief method to revert from shadow mapping state
		  **/
		 void cleanupShadows();

		 /**
		  * @brief: checker for whether a shadow shader is held.
		  *
		  * @returns: true if shadow shader is present, false otherwise.
		  **/
		 bool hasShadowShader();

		 //getters
		 light_type getType();
		 glm::vec3 getPosition();
		 glm::vec3 getDirection();
		 OKengine::shader& shadowShader();
		 GLuint getShadowMap();

};
