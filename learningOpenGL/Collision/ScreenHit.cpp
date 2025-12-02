#include "ScreenHit.h"
#include "CollisionAABB.h" 

namespace opScreenHit
{
	glm::vec3 calc_NormalPlane(AABB::triAABB vertPlane)
	{

		glm::vec3 arista1{ vertPlane.v2 - vertPlane.v1 };
		//		arista1 = glm::normalize(arista1);
		glm::vec3 arista2{ vertPlane.v3 - vertPlane.v1 };
		//		arista2 = glm::normalize(arista2);

		glm::vec3 dir_N{};

		glm::vec3 dir_N_1{ glm::normalize(glm::cross(arista2, arista1)) };
		glm::vec3 dir_N_2{ glm::normalize(glm::cross(arista1, arista2)) };

		float distDirN_1{ glm::distance(cameras::aerialCamera.posCam, dir_N_1) };
		float distDirN_2{ glm::distance(cameras::aerialCamera.posCam, dir_N_2) };

		if (distDirN_1 < distDirN_2)
		{
			dir_N = dir_N_1;

		}

		else if (distDirN_1 > distDirN_2)
		{
			dir_N = dir_N_2;
		}

		return dir_N;
	}
	glm::vec3 calc_NormalPlane_VertNormal(AABB::triAABB vertN_Plane, AABB::triAABB vertPlane)
	{
		std::vector<glm::vec3> vertN
		{
			vertN_Plane.v1,
			vertN_Plane.v2,
			vertN_Plane.v3
		};

		std::vector<glm::vec3> vert
		{
			vertPlane.v1,
			vertPlane.v2,
			vertPlane.v3
		};


		glm::vec3 posCenterNormal{ transformation_basics::calcCenterGeo(vertN) };
		glm::vec3 posCenterTri{ transformation_basics::calcCenterGeo(vert) };

		glm::vec3 normalPlane{ posCenterTri - posCenterNormal };
		normalPlane *= -1;
		normalPlane = glm::normalize(normalPlane);

		return normalPlane;
	}
	float calc_T(AABB::triAABB vertPlane, glm::vec3 nearPT_screen, glm::vec3 direction_R)
	{
		float t{};
		glm::vec3 N{ opScreenHit::calc_NormalPlane(vertPlane) };
		float denominator{ glm::dot(direction_R, N) };

		if (denominator < 0)
		{
			glm::vec3 planoVec{ vertPlane.v1 - nearPT_screen };
			float numerador{ glm::dot(planoVec, N) };

			t = numerador / denominator;
		}

		return t;
	}
	bool samePlane_Technique(AABB::triAABB tris, glm::vec3 PointCollisicion)
		{
			glm::vec3 r1{ glm::cross(tris.v2 - tris.v1, PointCollisicion - tris.v1) };
			glm::vec3 r2{ glm::cross(tris.v3 - tris.v2, PointCollisicion - tris.v2) };
			glm::vec3 r3{ glm::cross(tris.v1 - tris.v3, PointCollisicion - tris.v3) };

			bool correctCollision{ false };

			if (r1.z > 0 && r2.z > 0 && r3.z > 0)
			{
				correctCollision = true;
			}

			else if (r1.z < 0 && r2.z < 0 && r3.z < 0)
			{
				correctCollision = true;
			}

			return correctCollision;
		}

	
}
namespace ScreenCalc_Hit
{
	std::vector<Assimp_D::structModelName> nameMesh_Hit{};

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

		glm::mat4 camView{ glm::mat4(1.0f) };
		camView = glm::translate(camView, cameras::aerialCamera.directionView);

		std::map<std::string, glm::vec3> ptsPos
		{
			{"nearPoint", glm::vec3(glm::unProject(nearWindow, cameras::aerialCamera.cam, cameras::aerialCamera.camProjection, viewportOpenGL))},
			{"farPoint", glm::vec3(glm::unProject(farWindow, cameras::aerialCamera.cam, cameras::aerialCamera.camProjection, viewportOpenGL))}

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
			std::vector<Assimp_D::Mesh> meshesModel{ dataModels.second.outMeshes() };
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
					
					glm::vec4 posVert_1 = dataModels.second.ModelCoord.lastModel * glm::vec4(posVertex_1, 1.0); //Revisar
					glm::vec4 posVert_2 = dataModels.second.ModelCoord.lastModel * glm::vec4(posVertex_2, 1.0); //Revisar
					glm::vec4 posVert_3 = dataModels.second.ModelCoord.lastModel * glm::vec4(posVertex_3, 1.0); //Revisar

					posVertex_1 = glm::vec3(posVert_1.x, posVert_1.y, posVert_1.z);
					posVertex_2 = glm::vec3(posVert_2.x, posVert_2.y, posVert_2.z);
					posVertex_3 = glm::vec3(posVert_3.x, posVert_3.y, posVert_3.z);
					
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
	bool calc_IntersectTriangleMesh(std::vector<Assimp_D::structModelName>& Mesh, glm::vec3 nearPt, glm::vec3 direction_R)
	{
		bool intersectSuccessful{ false };
		AABB::numberVertSelected numberVert{};
		std::vector<Assimp_D::vertexD> vertex{};
		vertex.resize(3);
		float dist{ 200.0f };
		int meshSelected{};

		for (int m = 0; m < static_cast<int>(Mesh.size()); m++)
		{
			std::vector<Assimp_D::Mesh>& meshes{ RenderData_Set::AssimpModel_D[Mesh[m].nameModel].outMeshes()};

			for (auto& mesh : meshes)
			{
				if (mesh.nameMesh == Mesh[m].nameMesh)
				{

					for (int i = 0; i < static_cast<int>(mesh.verticesPos.size()); i++)
					{
						AABB::triAABB tri
						{
							mesh.verticesPos[i],
							mesh.verticesPos[i + 1],
							mesh.verticesPos[i + 2]

						};

						AABB::triAABB tri_Normal
						{
							mesh.normalsPos[i],
							mesh.normalsPos[i + 1],
							mesh.normalsPos[i + 2]

						};

						float t{ opScreenHit::calc_T(tri, nearPt, direction_R) };

						if (t > 0)
						{
							glm::vec3 pointCollision{ nearPt + t * direction_R };
							bool correctIntersect{ opScreenHit::samePlane_Technique(tri, pointCollision) };

							if (correctIntersect == true)
							{
								float distancePrim{ glm::distance(pointCollision, nearPt) };
								
								if (distancePrim < dist)
								{
									numberVert = AABB::numberVertSelected(i, i + 1, i + 2);

									//vertex.clear();
									vertex[0] = mesh.vertices[i];
									vertex[1] = mesh.vertices[i + 1];
									vertex[2] = mesh.vertices[i + 2];
									dist = distancePrim;

									//data_HitAABB::triangleStencil.texture = mesh.textures;
									//data_HitAABB::triangleStencil.shaderSet = mesh.shaderSet;
									//data_HitAABB::triangleStencil.MeshCoord = mesh.MeshCoord;

									meshSelected = m;

									if (intersectSuccessful == false)
									{ 
										intersectSuccessful = true;
									}
									
									break;
								}

							}

						}

						i += 2;
					}


					break;
				}
			}
		}

		if (intersectSuccessful == true)
		{
		//	std::vector<Assimp::Mesh>& meshes{ RenderData_Set::AssimpModel_D[Mesh[meshSelected].nameModel].outMeshes() };
		//	std::cout << "size_Vertices" << meshes[meshSelected].vertices.size();
		//	std::cout << "size_VerticesPos" << meshes[meshSelected].verticesPos.size();

			if (Mesh[meshSelected].nameMesh == data_HitAABB::selectedObj.first.nameMesh)
			{
				///Aqui nadamas actualizar los vertices y la textura, no el shader

				if (numberVert.v1 != data_HitAABB::selectedObj.second.v1 &&
					numberVert.v2 != data_HitAABB::selectedObj.second.v2 &&
					numberVert.v3 != data_HitAABB::selectedObj.second.v3)
				{
					//	data_HitAABB::selectedTri = std::pair<Assimp::structModelName, AABB::numberVertSelected>(Mesh, AABB::numberVertSelected(i, i + 2, i + 3));

						/*
						std::vector<Assimp::vertexD> vertex
						{
							mesh.vertices[i],
							mesh.vertices[i + 1],
							mesh.vertices[i + 2],
						};
					*/
					//	std::string intersectMesage{ "INTERSECT::" + Mesh.nameModel + "::" + Mesh.nameMesh };
					//	SDL_Log(intersectMesage.c_str());

					data_HitAABB::triangleStencil.setTriangle(vertex);
					data_HitAABB::triangleStencil.insertTriangle();
					data_HitAABB::triangleStencil.shader = RenderData_Set::AssimpModel_D[Mesh[meshSelected].nameModel].outShader();
					data_HitAABB::triangleStencil.name = Mesh[meshSelected];
					//data_HitAABB::triangleStencil.MeshCoord = mesh.MeshCoord;
					data_HitAABB::selectedObj = std::pair<Assimp_D::structModelName, AABB::numberVertSelected>(Mesh[meshSelected], numberVert);
				}


			}

			else if (Mesh[meshSelected].nameMesh != data_HitAABB::selectedObj.first.nameMesh)
			{


				data_HitAABB::triangleStencil.setTriangle(vertex);
				data_HitAABB::triangleStencil.insertTriangle();
			///	data_HitAABB::triangleStencil.texture = mesh.textures;
			//	data_HitAABB::triangleStencil.shaderSet = mesh.shaderSet;
			//	data_HitAABB::triangleStencil.MeshCoord = mesh.MeshCoord;
				data_HitAABB::triangleStencil.name = Mesh[meshSelected];

				//	if (Mesh.nameModel != data_HitAABB::selectedTri.first.nameModel)
				//	{
				data_HitAABB::triangleStencil.shader = RenderData_Set::AssimpModel_D[Mesh[meshSelected].nameModel].outShader();
				//	}

				data_HitAABB::selectedObj = std::pair<Assimp_D::structModelName, AABB::numberVertSelected>(Mesh[meshSelected], numberVert);

			}
		}

		return intersectSuccessful;
	}

	void calc_IntersectAABB()
	{
		std::map<std::string, glm::vec3> coord_PointScreen{ calculateWorldCoord_WindowPos() };
		std::string calc_Direction{ std::to_string(coord_PointScreen["farPoint"].x) + ", " + std::to_string(coord_PointScreen["farPoint"].y) + ", " + std::to_string(coord_PointScreen["farPoint"].z) + ", " };
		//	SDL_Log(calc_Direction.c_str());  ///INVESTIGAR POR QUE LA DIRECION EN (Y) DE farPoint no cambia

		glm::vec3 direction_R{ coord_PointScreen["farPoint"] - coord_PointScreen["nearPoint"] };
		direction_R = glm::normalize(direction_R);
		std::string nameModel_Intersect{};

		std::vector<std::pair<Assimp_D::structModelName, float>> distances_AABB{};
		std::vector< Assimp_D::structModelName> namesIntersectAABB{};


		for (auto& AABB_box : AABB::meshBoundingBox)
		{
			for (auto& AABB_tris : AABB_box.boxTrisAABB)
			{
				float t{ opScreenHit::calc_T(AABB_tris.second, coord_PointScreen["nearPoint"], direction_R) };

				if (t > 0)
				{
					glm::vec3 pointCollision{ coord_PointScreen["nearPoint"] + t * direction_R };

					bool correctIntersect{ opScreenHit::samePlane_Technique(AABB_tris.second, pointCollision) };

					if (correctIntersect == true)
					{
						float dist = glm::distance(pointCollision, coord_PointScreen["nearPoint"]);
						distances_AABB.emplace_back(std::pair<Assimp_D::structModelName, float>(AABB_box.nameAABB, t));
						namesIntersectAABB.emplace_back(AABB_box.nameAABB);
						//SDL_Log(std::string("CORRECT::INTERSECTION---" + AABB_box.nameAABB.nameMesh).c_str());
					}
				}
			}

		}


		if (!namesIntersectAABB.empty())
		{
			///////////////////////////////////////////////////////////////////REVISAR ESTA OPERACION SI SALE EXISTOSA
			//int sizeSelection{ static_cast<int>(distances_AABB.size()) };

			bool correctIntersect_Triangle{ calc_IntersectTriangleMesh(namesIntersectAABB, coord_PointScreen["nearPoint"], direction_R) };

			/*
			int pointerDist{};

			std::vector<std::pair<Assimp::structModelName, float>> distancesInvert{}; /////todos los meshes con los que esta intersectando en el AABB
			//distancesInvert.resize(sizeSelection);

			for (int i = 0; i < static_cast<int>(distances_AABB.size()); i++)
			{
				for (int s = 0; s < static_cast<int>(distances_AABB.size()); s++)
				{
					if (distances_AABB[i].second > distances_AABB[s].second)
					{
						pointerDist += 1;
					}
				}

				distancesInvert.emplace_back(distances_AABB[pointerDist]);
				pointerDist = 0;
			}

			for (auto& intersectMeshes : distancesInvert)
			{

				bool correctIntersect_Triangle{ calc_IntersectTriangleMesh(intersectMeshes.first, coord_PointScreen["nearPoint"], direction_R) };

				SDL_Log(std::to_string(intersectMeshes.second).c_str());
				if (correctIntersect_Triangle == true)
				{
					data_HitAABB::renderSelection = true;
					data_HitAABB::triangleStencil.updateModel();
					break;
				}

			}
		}
		*/

			if (correctIntersect_Triangle == true)
			{
				data_HitAABB::renderSelection = true;
				data_HitAABB::triangleStencil.updateModel();
				///	break;
			}

			else
			{
				data_HitAABB::triangleStencil.destroy();
				data_HitAABB::renderSelection = false;

			}

		}
	}

	void calc_IntersectALL()
	{
		calc_IntersectAABB();
	}
}


