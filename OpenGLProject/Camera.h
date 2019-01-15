#pragma once
#include <glad/glad.h>
#include "glm/ext.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};
class Camera
{private:
	glm::vec3 p_pos;
	glm::vec3 p_euler=glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 p_worldUp=glm::vec3(0.0f,1.0f,0.0f);

public:
	Camera(glm::vec3 pos=glm::vec3(0.0f,0.0f,8.0f)){
		p_pos=pos;
	};
	glm::mat4 getRotation(){
		return glm::eulerAngleYXZ(glm::radians(p_euler.y),glm::radians(p_euler.x),glm::radians(p_euler.z));
	}

	glm::mat4 getViewRotation() {
		return glm::eulerAngleYXZ(glm::radians(-p_euler.y), glm::radians(-p_euler.x), glm::radians(p_euler.z));
	}
	glm::mat4 getView(){
		return getRotation()*glm::lookAt(p_pos, p_pos + glm::vec3(0.0f,0.0f,-1.0f), p_worldUp);
	}

	glm::vec3 getPos(){
		return p_pos;
	}
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true, float MouseSensitivity = 0.2)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		p_euler.y += xoffset;
		p_euler.x -= yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{

			if (p_euler.y > 89.0f)
				p_euler.y = 89.0f;
			if (p_euler.y < -89.0f)
				p_euler.y = -89.0f;
		}
	}

	 void ProcessKeyboard(Camera_Movement direction_enum, float deltaTime,float speed=4)
    {	
    	glm::vec4 direction4f=getViewRotation()*glm::vec4(0.0f,0.0f,-1.0f,1.0f);
        glm::vec3 direction(direction4f);
		glm::vec4 right4f = getViewRotation()*glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec3 right(right4f);
        float velocity = speed * deltaTime;
        if (direction_enum == FORWARD)
            p_pos += direction * velocity;
        if (direction_enum == BACKWARD)
            p_pos -= direction * velocity;
        if (direction_enum == LEFT)
            p_pos -=  right* velocity;
        if (direction_enum == RIGHT)
            p_pos +=  right* velocity;
    }

};

