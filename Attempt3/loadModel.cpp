#include "loadModel.h"
#include "loadAssets.h"

/*
GLuint shaderProgram;
GLuint vertexAttrib;
GLuint uvAttrib;

// Struct to hold mesh loaded into OpenGL
struct Mesh
{
	unsigned int drawCount = 0;
	unsigned int materialIndex = 0;
	GLuint indexBuffer;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint vertexArrayObject;
	bool hasUvs = false;
};

// Struct to hold material loaded into OpenGL
struct Material
{
	glm::vec3 diffuseColor;
	GLuint diffuseTexture;
	bool hasDiffuseTexture;
};

struct Model
{
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
};

glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 modelScale = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 modelRotation = glm::vec3(0.0f, 0.0f, 0.0f); // euler angles in radians
glm::mat4 modelMatrix;
const std::string modelFile = "models/dragon.obj";
Model* model;


loadModel::loadModel()
{
	shaderProgram = glCreateProgram();

	// Use assimp to load a scene from the model file, and apply some post processing. See http://assimp.sourceforge.net/lib_html/postprocess_8h.html for more information.
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords | aiProcess_TransformUVCoords | aiProcess_OptimizeMeshes | aiProcess_FlipUVs);

	// Store the loaded model in a model struct
	model = new Model();

	if (!scene)
	{
		std::cout << "Error loading model: " << modelFile << std::endl;
	}
	else
	{
		std::cout << "Loaded model file: " << modelFile << std::endl;

		// Use shader
		glUseProgram(shaderProgram);

		// Loop through all the meshes in the scene
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			// Use a MeshStruct to store a mesh
			Mesh* mesh = new Mesh();

			// Vectors to store model data whilst we pass to OpenGL
			std::vector<unsigned int> indices;
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> uvs;

			// Assimp stores indices in faces, these will all be triangles due to the aiProcess_Triangulate flag passed when loading the model
			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			{
				// Push indices back into one long list and count
				indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
				indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
				indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
				mesh->drawCount += 3;
			}

			// Generate index buffer
			glGenBuffers(1, &mesh->indexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->indexBuffer);
			glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// Push vertices and uvs back into their lists
			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
			{
				vertices.push_back(glm::vec3(scene->mMeshes[i]->mVertices[j].x, scene->mMeshes[i]->mVertices[j].y, scene->mMeshes[i]->mVertices[j].z));

				// Check the model has uvs
				if (scene->mMeshes[i]->mTextureCoords[0] != NULL)
					uvs.push_back(glm::vec2(scene->mMeshes[i]->mTextureCoords[0][j].x, scene->mMeshes[i]->mTextureCoords[0][j].y));
			}

			// Generate vertex buffer
			glGenBuffers(1, &mesh->vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

			// Generate uv buffer if we have uvs
			if (uvs.size() > 0)
			{
				mesh->hasUvs = true;
				glGenBuffers(1, &mesh->uvBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->uvBuffer);
				glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
			}

			// Store material index
			mesh->materialIndex = scene->mMeshes[i]->mMaterialIndex;

			// Generate vertex array object
			glGenVertexArrays(1, &mesh->vertexArrayObject);

			// Tell OpenGL how to interpret the model data
			glBindVertexArray(mesh->vertexArrayObject);

			// Vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
			glEnableVertexAttribArray(vertexAttrib);
			glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

			// Uv buffer
			if (mesh->hasUvs)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->uvBuffer);
				glEnableVertexAttribArray(uvAttrib);
				glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}

			// Index buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);

			// Unbind everything
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			// Add to model
			model->meshes.push_back(mesh);
		}

		// Loop through all the material in the scene
		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{
			// Store loaded material in material struct
			Material* material = new Material();

			// Get the diffuse color of the material
			scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, material->diffuseColor);

			std::cout << material->diffuseColor.x << ", " << material->diffuseColor.y << ", " << material->diffuseColor.y << std::endl;

			// Check if the material has a diffuse texture
			if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				// Generate a texture
				glGenTextures(1, &material->diffuseTexture);

				material->hasDiffuseTexture = true;

				// Get texture file path
				aiString filename;
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &filename);

				std::string path = "models/" + std::string(filename.C_Str());

				// Load texture using SDL_image
				SDL_Surface* texture = nullptr;
				texture = IMG_Load(path.c_str());

				if (texture != nullptr)
				{
					glBindTexture(GL_TEXTURE_2D, material->diffuseTexture);

					// Textures have to be passed to OpenGL in the right way depending on format. This is by no means a complete list, and some texture formats might still fail.
					switch (texture->format->format)
					{
					case SDL_PIXELFORMAT_RGB24:
					case SDL_PIXELFORMAT_RGB332:
					case SDL_PIXELFORMAT_RGB444:
					case SDL_PIXELFORMAT_RGB555:
					case SDL_PIXELFORMAT_RGB565:
					case SDL_PIXELFORMAT_RGB888:
						// RGB format
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, texture->w, texture->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);
						break;
					case SDL_PIXELFORMAT_RGBA4444:
					case SDL_PIXELFORMAT_RGBA5551:
					case SDL_PIXELFORMAT_RGBA8888:
						// RGBA format
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
						break;
					case SDL_PIXELFORMAT_BGR24:
					case SDL_PIXELFORMAT_BGR555:
					case SDL_PIXELFORMAT_BGR565:
					case SDL_PIXELFORMAT_BGR888:
						// BGR format
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, texture->w, texture->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texture->pixels);
						break;
					case SDL_PIXELFORMAT_ABGR1555:
					case SDL_PIXELFORMAT_ABGR4444:
					case SDL_PIXELFORMAT_ABGR8888:
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
						break;
					case SDL_PIXELFORMAT_ARGB1555:
					case SDL_PIXELFORMAT_ARGB2101010:
					case SDL_PIXELFORMAT_ARGB4444:
					case SDL_PIXELFORMAT_ARGB8888:
						glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
						break;
					default:
						std::cout << "Unknown texture format: " << SDL_GetPixelFormatName(texture->format->format) << std::endl;
						break;
					}

					// Enable mipmapping
					glGenerateMipmap(GL_TEXTURE_2D);

					// Set texture parameters.
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeat wrapping
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeat wrapping
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear mag filtering
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // trilinear min filtering

					// Unbind the texture
					glBindTexture(GL_TEXTURE_2D, 0);

					std::cout << "Loaded texture: " << path << std::endl;

				}
				else
				{
					std::cout << "Failed to load texture: " << path << std::endl;
				}

				SDL_FreeSurface(texture);
				texture = nullptr;
			}

			// Add to model
			model->materials.push_back(material);
		}

	}

}


loadModel::~loadModel()
{
	//importer.FreeScene();
}
*/