#include "collisionAABB.h"
//#define ITERATE_MEMBERS(STRUCT) \
 //     for(auto& vert : {STRUCT.v1, STRUCT.v2, STRUCT.v3, STRUCT.v4, STRUCT.v5, STRUCT.v6, STRUCT.v7, STRUCT.v8})
        

namespace AABB
{
	void numberVertSelected::reset()
	{
		v1 = 0;
		v2 = 0;
		v3 = 0;
	}

	BoundingBox::BoundingBox() {};
	BoundingBox::BoundingBox(Assimp_D::structModelName nameAABB, std::vector<glm::vec3> vertAABB, boxAABB box_Coord, std::map<std::string, triAABB> boxTrisAABB)
	{
		loadBoundingBox(nameAABB, vertAABB, box_Coord, boxTrisAABB);
	}
	void BoundingBox::loadBoundingBox(Assimp_D::structModelName nameAABB, std::vector<glm::vec3> vertAABB, boxAABB box_Coord, std::map<std::string, triAABB> boxTrisAABB)
	{
		this->nameAABB = nameAABB;
		this->vertAABB = vertAABB;
		this->box_CoordBase = box_Coord;
		this->boxTrisAABB = boxTrisAABB;
		boxTrisAABB_Base = boxTrisAABB;
		box_CoordActual = box_Coord;

		modelAABB = RenderData_Set::AssimpModel_D[nameAABB.nameModel].ModelCoord.model;

		setUpBox();
	}
	void BoundingBox::setUpBox()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertAABB.size() * sizeof(glm::vec3), &vertAABB[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindVertexArray(0);

	}
	void BoundingBox::draw()
	{
		shaderAABB.use();
		
		std::vector<Assimp_D::Mesh>& meshes_AABB{ RenderData_Set::AssimpModel_D[nameAABB.nameModel].outMeshes() };
		bool notFind{ true };
		for (auto& f_mesh : meshes_AABB)
		{ 
			if (f_mesh.nameMesh == nameAABB.nameMesh)
			{
				shaderAABB.transformMat("model", f_mesh.MeshCoord.model);
				notFind = false;
				break;
			}
		}

		if (notFind == true)
		{
			shaderAABB.transformMat("model", glm::mat4(1.0f));
		}

		//shaderAABB.transformMat("model", matModel);
		shaderAABB.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
		shaderAABB.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);

		glBindVertexArray(VAO); 
		glDrawArrays(GL_LINE_STRIP, 0, vertAABB.size());
	}
	void BoundingBox::destroy()
	{
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	
	}

	void BoundingBox::updatePosicion(glm::mat4& modelMesh)
	{
		if (modelAABB != modelMesh)
		{
			std::tuple<
				std::pair<glm::vec3&, glm::vec3&>,
				std::pair<glm::vec3&, glm::vec3&>,
				std::pair<glm::vec3&, glm::vec3&>,
				std::pair<glm::vec3&, glm::vec3&>,
				std::pair<glm::vec3&, glm::vec3&>,
				std::pair<glm::vec3&, glm::vec3&>,
				std::pair<glm::vec3&, glm::vec3&>,
				std::pair<glm::vec3&, glm::vec3&>> tuple_PosVertex
			{
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v1, box_CoordActual.v1),
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v2, box_CoordActual.v2),
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v3, box_CoordActual.v3),
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v4, box_CoordActual.v4),
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v5, box_CoordActual.v5),
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v6, box_CoordActual.v6),
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v7, box_CoordActual.v7),
			   std::pair<glm::vec3&, glm::vec3&>(box_CoordBase.v8, box_CoordActual.v8)
			};

			auto updateVert = [&](std::pair<glm::vec3&, glm::vec3&> vertex)
				{
					glm::vec4 posMat_AABB(1.0f);
				//	posMat_AABB = RenderData_Set::AssimpModel_D[nameAABB.nameModel].ModelCoord.model * glm::vec4(vertex.first.x, vertex.first.y, vertex.first.z, 1.0f);
					posMat_AABB = modelMesh * glm::vec4(vertex.first.x, vertex.first.y, vertex.first.z, 1.0f);
					vertex.second = glm::vec3(posMat_AABB.x, posMat_AABB.y, posMat_AABB.z);

				};

			std::apply([&](auto&... vert)
				{
					(updateVert(vert), ...); ///FOLD METHOD
				}, tuple_PosVertex);




			
		}
	
	}
	void BoundingBox::updateBoxTrisPos(glm::mat4& modelMesh)
	{
		////////REALIZAR AQUI////////////
		if (modelAABB != modelMesh)
		{
			auto updateVert = [&](std::pair<glm::vec3&, glm::vec3&> box_Coords)
				{
		//	glm::vec4 posMat_AABB(1.0f);
		//	glm::mat4 posMat{ glm::mat4(1.0f) };
				//osMat = glm::translate(posMat, box_Coords.first);
			
				//osMat = modelMesh * posMat;

					glm::vec4 posMat{ modelMesh * glm::vec4(box_Coords.first.x, box_Coords.first.y, box_Coords.first.z, 1.0f) };
		
					//	posMat_AABB = RenderData_Set::AssimpModel_D[nameAABB.nameModel].ModelCoord.model * glm::vec4(box_Coords.first.x, box_Coords.first.y, box_Coords.first.z, 1.0f);
					//	box_Coords.second = glm::vec3(posMat_AABB.x, posMat_AABB.y, posMat_AABB.z);
					box_Coords.second = glm::vec3(posMat.x, posMat.y, posMat.z);
				};


			for (auto& vert : boxTrisAABB)
			{
				std::tuple<
					std::pair<glm::vec3&, glm::vec3&>,
					std::pair<glm::vec3&, glm::vec3&>,
					std::pair<glm::vec3&, glm::vec3&>> trisBox
				{
				std::pair<glm::vec3&, glm::vec3&>(boxTrisAABB_Base[vert.first].v1, vert.second.v1),
				std::pair<glm::vec3&, glm::vec3&>(boxTrisAABB_Base[vert.first].v2, vert.second.v2),
				std::pair<glm::vec3&, glm::vec3&>(boxTrisAABB_Base[vert.first].v3, vert.second.v3),
				};

				std::apply([&](auto&... vertex)
					{
						(updateVert(vertex), ...);
					}, trisBox);
			}

			modelAABB = modelMesh;
		}
	}

	std::vector<BoundingBox> meshBoundingBox{};

	std::pair<std::vector<glm::vec3>, boxAABB> calcBox(glm::vec3 minPos, glm::vec3 maxPos)
	{
		///Vertex max y sus partes
		glm::vec3 vert_1{ maxPos };
		glm::vec3 vert_2{ minPos.x, maxPos.y, maxPos.z };
		glm::vec3 vert_3{ maxPos.x, minPos.y, maxPos.z };
		glm::vec3 vert_4{ maxPos.x, maxPos.y, minPos.z };
	    
		//Vertex min y sus partes
		glm::vec3 vert_5{ minPos };
		glm::vec3 vert_6{ minPos.x, minPos.y, maxPos.z };
		glm::vec3 vert_7{ minPos.x, maxPos.y, minPos.z };
		glm::vec3 vert_8{ maxPos.x, minPos.y, minPos.z };


		std::vector<glm::vec3> boxVec
		{
			vert_1,
			vert_2,
			vert_7,
			vert_4,
			vert_8,
			vert_3,
			vert_6,
			vert_5,
			vert_8,///
			vert_4,
			vert_7,
			vert_5,
			vert_6,
			vert_2,
			vert_1,
			vert_3,
			vert_1,
			vert_4
		};
		boxAABB boxAABB_vertex
		{
			vert_1,
			vert_2,
			vert_3,
			vert_4,
			vert_5,
			vert_6,
			vert_7,
			vert_8

		};

		/*
		std::vector<quadAABB> boxVec
		{
			face1,
			face2,
			face3,
			face4,
			face5,
			face6
		};
		*/

		return std::pair<std::vector<glm::vec3>, boxAABB>(boxVec, boxAABB_vertex);
	}
	std::map<std::string, triAABB> calcTrisAABB(boxAABB vertex)
	{
		triAABB tri_1
		{
			vertex.v1,
			vertex.v2,
			vertex.v3
		};

		triAABB tri_2
		{
			vertex.v2,
			vertex.v3,
			vertex.v6
		};

		triAABB tri_3
		{
			vertex.v2,
			vertex.v5,
			vertex.v6
		};

		triAABB tri_4
		{
			vertex.v2,
			vertex.v5,
			vertex.v7
		};

		triAABB tri_5
		{
			vertex.v5,
			vertex.v8,
			vertex.v7
		};

		triAABB tri_6
		{
			vertex.v4,
			vertex.v8,
			vertex.v7
		};

		triAABB tri_7
		{
			vertex.v4,
			vertex.v1,
			vertex.v8
		};

		triAABB tri_8
		{
			vertex.v3,
			vertex.v8,
			vertex.v1
		};

		triAABB tri_9
		{
			vertex.v1,
			vertex.v4,
			vertex.v7
		};

		triAABB tri_10
		{
			vertex.v2,
			vertex.v1,
			vertex.v7
		};

		triAABB tri_11
		{
			vertex.v8,
			vertex.v3,
			vertex.v5
		};

		triAABB tri_12
		{
			vertex.v6,
			vertex.v3,
			vertex.v5
		};

		std::map<std::string, triAABB> tris
		{
			{"triangle_1", tri_1},
			{"triangle_2", tri_2},
			{"triangle_3", tri_3},
			{"triangle_4", tri_4},
			{"triangle_5", tri_5},
			{"triangle_6", tri_6},
			{"triangle_7", tri_7},
			{"triangle_8", tri_8},
			{"triangle_9", tri_9},
			{"triangle_10", tri_10},
			{"triangle_11", tri_11},
			{"triangle_12", tri_12},
		};

		return tris;
	}
	void create_BoundingBox_Mesh()
	{
		for (auto& Model : RenderData_Set::AssimpModel_D)
		{
			bool existBoundingBox{ false };
			
			for (auto& AABB_box : meshBoundingBox)
			{
				if (AABB_box.nameAABB.nameModel == Model.second.nameModel)
				{
					existBoundingBox = true;
					break;
				}
			}
			
			if (existBoundingBox == false)
			{
				std::vector<Assimp_D::Mesh>& Meshes{ Model.second.outMeshes() };


				for (int i = 0; i < static_cast<int>(Meshes.size()); i++)
				{
					//Calcular el centro de la geometria
					std::vector<glm::vec3> vertex_Tris{};
					for (auto& vert_Array : Meshes[i].vertices)
					{
						vertex_Tris.emplace_back(vert_Array.posicion);

					}

					glm::vec3 centerMesh{ transformation_basics::calcCenterGeo(vertex_Tris) };

					glm::vec3 minPos{ centerMesh };
					glm::vec3 maxPos{ centerMesh };

					for (auto& vertex : Meshes[i].vertices)
					{
						if (vertex.posicion.x < minPos.x)
						{
							minPos.x = vertex.posicion.x;
						}

						if (vertex.posicion.y < minPos.y)
						{
							minPos.y = vertex.posicion.y;
						}

						if (vertex.posicion.z < minPos.z)
						{
							minPos.z = vertex.posicion.z;
						}

						if (vertex.posicion.x > maxPos.x)
						{
							maxPos.x = vertex.posicion.x;
						}

						if (vertex.posicion.y > maxPos.y)
						{
							maxPos.y = vertex.posicion.y;

						}

						if (vertex.posicion.z > maxPos.z)
						{
							maxPos.z = vertex.posicion.z;

						}

					}

					std::pair<std::vector<glm::vec3>, boxAABB> box{ calcBox(minPos, maxPos) };
					std::map<std::string, triAABB> trisAABB{ calcTrisAABB(box.second) }; ////////cambio de inicializacion
					Assimp_D::structModelName name
					{
						Model.second.nameModel,
						Meshes[i].nameMesh, //REVISAR
						false
					};

					meshBoundingBox.emplace_back(BoundingBox(name, box.first, box.second, trisAABB));

				}
			}

		}

	};
	void test_BoundingBos()
	{
		glm::vec3 vert1{ 1.0f, 1.0f, 1.0f };
		glm::vec3 vert2{ -1.0f, 1.0f, 1.0f };
		glm::vec3 vert3{ 1.0f, 1.0f, -1.0f };
		glm::vec3 vert4{ -1.0f, 1.0f, -1.0f };
		glm::vec3 vert5{ 1.0f, -1.0f, 1.0f };
		glm::vec3 vert6{ -1.0f, -1.0f, 1.0f };
		glm::vec3 vert7{ -1.0f, -1.0f, -1.0f };
		glm::vec3 vert8{ 1.0f, -1.0f, -1.0f };

		Assimp_D::structModelName name
		{
			"panchito",
			"panchito_Jr",
			false
		};

		//	meshBoundingBox.emplace_back(BoundingBox(name, vectorAABB));

	};
	void setShader_AABB()
	{
		shading::shader shaderAll_AABB(vShader_AABB_All.c_str(), fShader_AABB_All.c_str());

		for (auto& AABB_box : meshBoundingBox)
		{
			AABB_box.shaderAABB = shaderAll_AABB;
		}

	};
	void updateCoordAABB_All()
	{
		std::string last_Model{};
		std::unique_ptr<std::vector<Assimp_D::Mesh>> currentMeshes{ nullptr };

		for (auto& AABB_box : meshBoundingBox)
		{
			if (AABB_box.nameAABB.nameModel != last_Model)
			{
				currentMeshes.reset();
				currentMeshes = std::make_unique<std::vector<Assimp_D::Mesh>>(RenderData_Set::AssimpModel_D[AABB_box.nameAABB.nameModel].outMeshes());
				last_Model = AABB_box.nameAABB.nameModel;
			}

			for (int i = 0; i < static_cast<int>(currentMeshes->size()); i++)
			{
				if (currentMeshes->at(i).nameMesh == AABB_box.nameAABB.nameMesh)
				{
					AABB_box.updatePosicion(currentMeshes->at(i).MeshCoord.model);
				    AABB_box.updateBoxTrisPos(currentMeshes->at(i).MeshCoord.model);
				}

			}

			//currentMeshes.reset();
			///test 
		}

	}

}

namespace data_HitAABB
{
//	Assimp::structModelName nameModel_Stencil{};
	std::pair<Assimp_D::structModelName, AABB::numberVertSelected> selectedObj{};
	individualComp::singleTriangle triangleStencil{};
	bool renderSelection{};
	bool selectObj{};

	void resetSelectedObj()
	{
		selectedObj.first.reset();
		selectedObj.second.reset();
	}

}
