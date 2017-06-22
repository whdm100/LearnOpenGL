/**
 * @file: Mesh.h
 *
 * @desc: Wrap class of Assimp Lib.
 *
 * @version: 1.0
 *
 * @date: 2017/06/10
 *
 * @auther:
 *
 */
#ifndef __MESH_H
#define __MESH_H

#include "GLImp.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

enum AttrIDS
{
    AttrNone = 0x0,
    AttrPosition = 0x1,
    AttrNormal = 0x2,
    AttrTangent = 0x4,
    AttrDiffuse = 0x8,
    AttrSpecular = 0x10,
    AttrTex0 = 0x20,
    AttrTex1 = 0x40,
    AttrTex2 = 0x80,
    AttrTex3 = 0x100,
    AttrTex4 = 0x200,
    AttrTex5 = 0x400,
    AttrTex6 = 0x800,
    AttrTex7 = 0x1000
};

struct Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 texcoord;
};

struct Texture
{
    GLuint id;      // texture id return from LoadTexture(...);
    GLenum type;    // texture type, GL_TEXTURE_2D,GL_TEXTURE_CUBE...
    string map;     // texture_diffuse, texture_specular and so on.
    string path;    // texture file path
    string uname;   // used for glUniform1i call for set shader sampler.
};

class Shader;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    // Load mesh data 
    void Load(AttrIDS ids, const void *vertices, GLsizei vstride, GLuint vcount);
    void Load(AttrIDS ids, const GLuint *indices, GLsizei istride, GLuint icount, const void *vertices, GLsizei vstride, GLuint vcount);

    // Add textures
    void AddTexture(const Texture &texture);
    void AddTexture(const vector<Texture> &textures);

    // Clear textures
    void RemoveTextures();

    // Draw current mesh
    void Draw(Shader *shader);

private:

    void Load();
    void Unload();

private:
    vector<Texture> _textures;

    bool _hasElemArray;

    AttrIDS _attrIDs;

    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;

    GLuint _vertices;
    GLuint _indices;
};

GLuint LoadTexture(const char *path, bool alpha = false);
// Load in order : right, left, top, bottom, front, back
GLuint LoadCubeTexture(const vector<const char*> &faces, bool alpha = false);

class Model
{
public:

    Model(GLchar* path)
    {
        LoadModel(path);
    }

    ~Model()
    {
        Unload();
    }

    // Draw current model
    void Draw(Shader *shader);

private:
    vector<Mesh*> _meshes;    
    vector<Texture> _textures;

    string _directory;

    // Load model data base on assimp library.
    void LoadModel(string path);
    void Unload();

    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh *ProcessMesh(aiMesh* mesh, const aiScene* scene);

    vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif // __MESH_H