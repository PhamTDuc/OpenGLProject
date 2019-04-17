#include "Model.h"
Model::Model(std::string const &path) {
	loadModel(path);
};
void Model::Draw(Shader &shader){
    for(unsigned int i = 0; i < meshes.size(); i++){
    	meshes[i].Draw(shader);
    }
}  

void Model::loadModel(std::string path){
	Assimp::Importer import;
	const aiScene *scene=import.ReadFile(path, aiProcess_Triangulate |aiProcess_FlipUVs| aiProcess_CalcTangentSpace);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout <<"ERROR:ASSIMP::"<<import.GetErrorString()<<std::endl;
		return;
	}
	
	directory=path.substr(0,path.find_last_of('/'));	
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
 }


 Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // process vertex positions, normals and texture coordinates
        Vertex vertex;
        glm::vec3 vector; 
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z; 
		vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;  
		//std::cout << vector.x << ":" << vector.y << ":" << vector.z << std::endl;
	

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.BiTangent = vector;

        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
			//std::cout << vec.x << " " << vec.y<<std::endl;
		}
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);  

        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    } 
    // process material
   if(mesh->mMaterialIndex >= 0)
   {
       aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
       std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                           aiTextureType_DIFFUSE, "diffuse");
       textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
       std::vector<Texture> specularMaps = loadMaterialTextures(material, 
                                           aiTextureType_SPECULAR, "specular");
       textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	   std::vector<Texture> ambientMaps = loadMaterialTextures(material,
		   aiTextureType_AMBIENT, "ambient");
	   textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

	   std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normalMap",false);
	   textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
   } 
    return Mesh(vertices, indices, textures);
} 

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName,bool colorData)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            std::string rel_path(str.C_Str());
            std::string abs_path=(false) ? rel_path : this->directory+"/"+rel_path;
            texture.id = loadTexture(abs_path,colorData);
            texture.typeName = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}  