#include "2D_screenPlayer.h"
#include "Collision/ScreenHit.h"

namespace screenUI
{
	namespace screenWorldPos
	{
		glm::vec3 getScreenPos(float width, float heigth, float distanceScreen)
		{
			glm::vec3 nearPos{ width, heigth, distanceScreen };
			nearPos = glm::unProject(nearPos, cameras::aerialCamera.cam, cameras::aerialCamera.camProjection, viewportOpenGL);
			return nearPos;
		}
	}

	pointerScreen::pointerScreen() {};
	pointerScreen::pointerScreen(const char* vertexPath, const char* fragmentPath, float size, glm::vec3 posPointer, glm::vec3 colorPointer)
	{
		shaderPointer.shaderCreation(vertexPath, fragmentPath);
		sizePoint = size;
		this->posPointer = posPointer;
		this->colorPointer = colorPointer;

		setPointer();
	};
	pointerScreen::pointerScreen(std::string nameShader, float size, glm::vec3 posPointer, glm::vec3 colorPointer)
	{
		this->nameShader = nameShader;
		sizePoint = size;
		this->posPointer = posPointer;
		this->colorPointer = colorPointer;
		setPointer();
	}
	void pointerScreen::setPointer()
	{
		
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &posPointer, GL_STATIC_DRAW);
			
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

			glBindVertexArray(0);

	}

	void pointerScreen::drawPoint()
	{
		shading::shader& shaderPt{ RenderData_Set::shader_D[nameShader] };
		shaderPt.use();
		shaderPt.use();
		shaderPt.transformMat("model", model);
		shaderPt.transformMat("view", cameras::aerialCamera.cam);
		shaderPt.transformMat("projection", cameras::aerialCamera.camProjection);
		shaderPt.setFloat("sizePointer", sizePoint);
		shaderPt.setVec3("PointColor", colorPointer);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	void pointerScreen::updatePoint()
	{
		glm::vec3 posModel{ screenWorldPos::getScreenPos(static_cast<float>(screenSettings::screen_w) * 0.5f, static_cast<float>(screenSettings::screen_h) * 0.5f, 0.8f) };
		//glm::vec3 posNewModel{ cameras::aerialCamera.posCam - (posModel) };
		//posNewModel *= 2.0f;
		//posNewModel = cameras::aerialCamera.posCam - posNewModel;
			//SDL_Log(std::string(std::to_string(posModel.x) + ", " + std::to_string(posModel.y) + ", " + std::to_string(posModel.z)).c_str());

		float rotYaw{ glm::radians(-cameras::aerialCamera.yaw)};
		float rotPitch{ glm::radians(-cameras::aerialCamera.pitch - 90.0f)};

		glm::vec3 posRotPitch{ glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(posPointer)) };

		glm::qua quaYaw{ glm::angleAxis(rotYaw, glm::vec3(0.0f, 1.0f, 0.0f)) };
		glm::qua quaPitch{ glm::angleAxis(rotPitch, glm::normalize(posRotPitch)) };

		glm::mat4 yawMat{ glm::toMat4(quaYaw) };
		glm::mat4 pitchMat{ glm::toMat4(quaPitch) };

		glm::mat4 combineRot{ yawMat * pitchMat };

		glm::mat4 matPos{ glm::mat4(1.0f) };
		//glm::mat4 matTraslation{ glm::mat4(1.0f) };

		//matTraslation = glm::translate(matTraslation, posPointer);
		//matTraslation = matTraslation * combineRot;

	//	glm::vec3 TraslationBack{ matTraslation[3].x, matTraslation[3].y, matTraslation[3].z };
	//	TraslationBack = (-TraslationBack * 2.0f);

		matPos = glm::translate(matPos, cameras::aerialCamera.posCam);
	//	matPos = glm::translate(matPos, TraslationBack);
		matPos = matPos *  combineRot;

	//	glm::vec3 posBack{ posPointer * -1.0f};
    //  matPos = glm::translate(matPos, posBack);

		glm::mat4 translateModelPos{ glm::mat4(1.0f) };
		translateModelPos = glm::translate(translateModelPos, posModel);

		model = translateModelPos;
	//	posPointer = posModel;
		

	}
	void pointerScreen::updatePos(glm::vec3 posPointer)
	{
		glm::mat4 matPos{ glm::mat4(1.0f) };
		matPos = glm::translate(matPos, posPointer);
		
		model = matPos;

	}

}