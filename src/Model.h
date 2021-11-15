#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Shader.h"
#include "Mesh.h"

class Model {
public:
    Model(char *path);
    void Draw(Shader &shader);
private:
    /* Model Data */
    std::vector<Mesh> meshes;
    std::string directory;

    void LoadModel(const char* path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

    //TODO: std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
