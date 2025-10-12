#include "ScreenHit.h"

namespace stencilTest_SelectObj
{

	std::unique_ptr<Assimp::Model> currentModelSelect{ nullptr };
	std::unique_ptr<Assimp::Mesh> currentMeshSelect{ nullptr };
	bool changeStateSelection{ false };
	std::array<selectObjParts, 3> selectedParts
	{
		selectObjParts::triangles,
		selectObjParts::mesh,
		selectObjParts::model
	};

	std::map<std::string, glm::vec3> calculateWorldCoord_WindowPos()
	{
		/*
		float matModelView[16], matProjection[16];

		std::vector<float[16]> matrixCam
		{
			matModelView,
			matProjection

		};

		int seq{};
		int seqArray{};
		for (auto& matCam : matrixCam)
		{
			for (int m = 0; m < 4; m++)
			{
				if (seq == 0)
				{
					matCam[seqArray] = cameras::currentCamera.cam[m].x;
					matCam[seqArray + 1] = cameras::currentCamera.cam[m].y;
					matCam[seqArray + 2] = cameras::currentCamera.cam[m].z;
					matCam[seqArray + 3] = cameras::currentCamera.cam[m].w;
				}

				else if (seq == 1)
				{
					matCam[seqArray] = cameras::currentCamera.camProjection[m].x;
					matCam[seqArray+1] = cameras::currentCamera.camProjection[m].y;
					matCam[seqArray+2] = cameras::currentCamera.camProjection[m].z;
					matCam[seqArray+3] = cameras::currentCamera.camProjection[m].w;

				}
				seqArray += 4;
			}
			seq++;
			seqArray = 0;
		}
	    */
	//   glGetFloatv(cameras::currentCamera.cam, matModleView);

		float xWindow{ static_cast<float>(screenSettings::screen_w) * 0.5f };
		float yWindow{ static_cast<float>(screenSettings::screen_h) * 0.5f };
		glm::vec3 nearWindow{ xWindow, yWindow, 0.0f };
		glm::vec3 farWindow{ xWindow, yWindow, 1.0f };
			//glGetIntegerv(GL_VIEWPORT, viewpo)

	
		std::map<std::string, glm::vec3> ptsPos
		{
			{"nearPoint", glm::vec3(glm::unProject(nearWindow, cameras::currentCamera.cam, cameras::currentCamera.camProjection, viewportOpenGL))},
			{"farPoint", glm::vec3(glm::unProject(farWindow, cameras::currentCamera.cam, cameras::currentCamera.camProjection, viewportOpenGL))}

		};
//glm::vec3 posNearPoint = glm::unProject(nearWindow, cameras::currentCamera.cam, cameras::currentCamera.camProjection, viewportOpenGL);
//glm::vec3 posFarPoint = glm::unProject(farWindow, cameras::currentCamera.cam, cameras::currentCamera.camProjection, viewportOpenGL);

		return ptsPos;
	}

	void calculateIntersect_Objects()
	{
		std::map<std::string, glm::vec3> points_SW3D{ calculateWorldCoord_WindowPos() };

		glm::vec3 rayPointScreen{ glm::normalize(points_SW3D["nearPoint"] - points_SW3D["farPoint"]) };
		std::string modelSelected{};
		int numMeshSelected{};
		bool findTriangle{};
		
		for(auto& dataModels : RenderData_Set::AssimpModel_D)
		{
			std::vector<Assimp::Mesh> meshesModel{ dataModels.second.outMeshes() };
			for (int i = 0; i < static_cast<int>(meshesModel.size()); i++)
			{
				int numberTriangles{ static_cast<int>(meshesModel[i].vertices.size()) / 3 };
				int sumSeq{};
				for (int v = 0; v < numberTriangles; v++)
				{
					float sumAng_AllVertex{};
					//calculate all ang per Vertex
					glm::vec3 posVertex_1{ meshesModel[i].vertices[sumSeq].posicion };
					glm::vec3 posVertex_2{ meshesModel[i].vertices[sumSeq+1].posicion };
					glm::vec3 posVertex_3{ meshesModel[i].vertices[sumSeq+2].posicion };

					posVertex_1 = glm::normalize(points_SW3D["nearPoint"] - posVertex_1);
					posVertex_2 = glm::normalize(points_SW3D["nearPoint"] - posVertex_2);
					posVertex_3 = glm::normalize(points_SW3D["nearPoint"] - posVertex_3);
				
					////Cambiar al metodo del mismo lado
					sumAng_AllVertex += glm::dot(posVertex_1, posVertex_2);
					sumAng_AllVertex += glm::dot(posVertex_2, posVertex_3);
					sumAng_AllVertex += glm::dot(posVertex_3, posVertex_1);

					sumSeq += 3;
				}
				

			}

		
		}

	}

}

void calculateAllScreenHit()
{

}


