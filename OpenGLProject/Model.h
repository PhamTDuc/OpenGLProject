#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "LoadingTexture.h"
#include "Mesh.h"
class Model 
{
    public:
        /*  Functions   */
        Model(const char *path)
        {
            loadModel(path);
        }
        void Draw(Shader &shader);	
        /*  Model Data  */
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded; 
        std::string directory;
        /*  Functions   */
		Model(const std::string &path);
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, 
                                             std::string typeName);
};

