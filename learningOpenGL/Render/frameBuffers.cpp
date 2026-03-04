#include "frameBuffers.h"
#include "config_window.h"
#include "RenderData.h"
#include "Collision/ScreenHit.h"

namespace frameBuff
{

	frameBuffer::frameBuffer() {};
	frameBuffer::frameBuffer(std::string name, typeFrameBuffer type, Assimp_D::structModelName nameAssimp)
	{
		nameFrameBuffer = name;

		switch (type)
		{
		case(typeFrameBuffer::bufferScreen) :
			loadScreenView();
			loadFrameBuffer();
			break;
		case(typeFrameBuffer::bufferAssimp) :
			loadFrameBuffer();
			this->type = type;
			dataBuffer.nameAssimp = nameAssimp;
			break;

		}

	}
	void frameBuffer::loadFrameBuffer()
	{
		////////////////////////// CREACION DEL FRAMEBUFFER

		glGenFramebuffers(1, &dataBuffer.FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, dataBuffer.FBO);

		/////////////////// CREACION DE TEXTURA DEL FRAMEBUFFER

		glGenTextures(1, &dataBuffer.TCB);
		glBindTexture(GL_TEXTURE_2D, dataBuffer.TCB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSettings::screen_w, screenSettings::screen_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dataBuffer.TCB, 0);

		/////////////////CREACION DEL RENDER BUFFER OBJECT
		
		glGenRenderbuffers(1, &dataBuffer.RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, dataBuffer.RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenSettings::screen_w, screenSettings::screen_h);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dataBuffer.RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			SDL_Log("ERROR::FRAMEBUFFER::NOT COMPLETE!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
	void frameBuffer::loadScreenView()
	{

		/*
		std::vector<vertex_Buffer> data_TriangleScreen
		{
			vertex_Buffer(glm::vec2(0.0f, screenSettings::screen_h), glm::vec2(0.0f, 1.0f)),
			vertex_Buffer(glm::vec2(screenSettings::screen_w, screenSettings::screen_h), glm::vec2(1.0f, 1.0f)),
			vertex_Buffer(glm::vec2(screenSettings::screen_w, 0), glm::vec2(1.0f, 0.0f)),
			vertex_Buffer(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),

		};
		*/
		std::vector<vertex_Buffer> data_TriangleScreen
		{
			vertex_Buffer(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
			vertex_Buffer(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
			vertex_Buffer(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
			vertex_Buffer(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
			vertex_Buffer(glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
			vertex_Buffer(glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)),

		};

		std::vector<unsigned int> dataIndices
		{
			0, 1, 3,
			3, 1, 2
		};

		sizeIndices = static_cast<int>(dataIndices.size());

		glGenVertexArrays(1, &dataBuffer.VAO);
		glGenBuffers(1, &dataBuffer.VBO);
		//glGenBuffers(1, &dataBuffer.EBO);

		glBindVertexArray(dataBuffer.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, dataBuffer.VBO);
		glBufferData(GL_ARRAY_BUFFER, data_TriangleScreen.size() * sizeof(vertex_Buffer), &data_TriangleScreen[0], GL_STATIC_DRAW);

		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dataBuffer.EBO);
		//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataIndices.size() * sizeof(unsigned int), &dataIndices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_Buffer), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_Buffer), (void*)offsetof(vertex_Buffer, TexCoord));

		//glBindBuffer(GL_ARRAY_BUFFER,S 0);
		glBindVertexArray(0);

	}
	void frameBuffer::bindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, dataBuffer.FBO);

	}
	void frameBuffer::useFrameBufferScreen()
	{
		RenderData_Set::shader_D["shaderFramebuffer"].use();
		glBindVertexArray(dataBuffer.VAO);
		//glDisable(GL_DEPTH_TEST);
		RenderData_Set::shader_D["shaderFramebuffer"].setInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dataBuffer.TCB);
	//lDrawElements(GL_TRIANGLES, sizeIndices, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void frameBuffer::useFrameBufferModel()
	{

		//glDisable(GL_DEPTH_TEST);

		RenderData_Set::shader_D["shaderFramebuffer"].use();

		////SI EL OBJETO DE TAMA�O ESTA CHICO, AGREGAR LA MATRIX MODEL PARA LA POSICION

		Assimp_D::Mesh* mesh_find {RenderData_Set::ModelsScene_D->out_mesh_fromModel(dataBuffer.nameAssimp.model_ID, dataBuffer.nameAssimp.mesh_ID)}; ///CHECK THIS

		int sizeIndices{};

		if (mesh_find != nullptr) {
			glm::mat4 upModelMesh = mesh_find->MeshCoord.model;
			upModelMesh = glm::translate(upModelMesh, glm::vec3(0.0f, 0.2f, 0.0f));

			RenderData_Set::shader_D["shaderFramebuffer"].transformMat("model", upModelMesh);

			RenderData_Set::shader_D["shaderFramebuffer"].transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
			RenderData_Set::shader_D["shaderFramebuffer"].transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
			sizeIndices = static_cast<int>(mesh_find->indices.size());
			unsigned int& VAO_mesh = mesh_find->outVAO();
			glBindVertexArray(VAO_mesh);
		}

		RenderData_Set::shader_D["shaderFramebuffer"].setInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dataBuffer.TCB);
		glDrawElements(GL_TRIANGLES, sizeIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


	}

	void frameBuffer::clear()
	{
		glDeleteVertexArrays(1, &dataBuffer.VAO);
		glDeleteBuffers(1, &dataBuffer.VBO);
		glDeleteBuffers(1, &dataBuffer.EBO);

		glDeleteRenderbuffers(1, &dataBuffer.RBO);
		glDeleteFramebuffers(1, &dataBuffer.FBO);
	}

}


namespace frameBuff_Obj
{
	void set_PlanarReflection_Dir(uint32_t mesh_ID, camera::camera1& cameraToUpdate) {
		Assimp_D::Mesh* mesh_find {RenderData_Set::ModelsScene_D->out_mesh_fromID(mesh_ID)};  ///CHECK THIS IF I GET ERROR

		if (mesh_find != nullptr)
		{

			AABB::triAABB triNormal
			{
				mesh_find->normalsPos[0],
				mesh_find->normalsPos[1],
				mesh_find->normalsPos[2]

			 };

			AABB::triAABB tri
			{
				mesh_find->verticesPos[0],
				mesh_find->verticesPos[1],
				mesh_find->verticesPos[2]
			  };



			//glm::vec3 normalFace{ opScreenHit::calc_NormalPlane(tri) };
			glm::vec3 normalFace{ opScreenHit::calc_NormalPlane_VertNormal(triNormal, tri) };
			//normalFace = glm::normalize(-normalFace);

			//normalFace = glm::normalize(-(mesh.MeshCoord.posModel - normalFace));

			/*
			glm::vec3 posCamera{ glm::normalize(cameraToUpdate.posCam) };

			glm::vec3 newPos{ glm::cross(posCamera, normalFace) };
			newPos = glm::cross(posCamera, newPos);
			//newPos = cameraToUpdate.posCam + newPos;
			*/
			RenderData_Set::pointUI_D[1].updatePos(normalFace);

			//glm::vec3 cameraTarget{ cameraToUpdate.directionView};
			glm::vec3 cameraTarget{ cameraToUpdate.posCam + cameraToUpdate.directionView };

			glm::mat4 matViewReflect{ reflectionMatrixOP::calcReflectMatrix(cameraToUpdate.posCam, mesh_find->verticesPos[0], normalFace, cameraToUpdate.cameraUp, cameraTarget) };
			glm::mat4 matProjection{ reflectionMatrixOP::calcObliqueProjection(cameraToUpdate.camProjection, matViewReflect, mesh_find->verticesPos[0], normalFace) };

			cameraToUpdate.updateSettingsCam(matViewReflect, matProjection);

			mesh_find = nullptr;
		}

		else if (mesh_find == nullptr)
	    {
			std::string error_findMesh {"ERROR::DOESNT FIND MESH::NOT SET PLANR REFLECTION DIR"};

			register_error_RM::register_error_withSentence(error_findMesh.c_str());
		}

	}
}