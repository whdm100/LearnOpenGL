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

struct Texture
{
    GLuint id;      // texture id return from LoadTexture(...);
    GLenum type;    // texture type, GL_TEXTURE_2D,GL_TEXTURE_CUBE...
    string map;     // texture_diffuse, texture_specular and so on.
    string path;    // texture file path
    string uname;   // used for glUniform1i call for set shader sampler.
};

class Shader;

class MeshBase
{
public:
    MeshBase();
    virtual ~MeshBase();

    // add textures
    void AddTexture(const Texture &texture);
    void AddTexture(const vector<Texture> &textures);

    // clear textures
    void RemoveTextures();

    // draw current mesh
    void Draw(Shader *shader);
    void DrawInstanced(Shader *shader, GLuint amout);

private:
    void Unload();

protected:
    vector<Texture> _textures;

    bool _hasElemArray;

    GLuint _attrIndex;

    GLuint _vao;
    GLuint _vbo;
    GLuint _ebo;

    GLuint _vertices;
    GLuint _indices;
};

typedef int AttrIDS;

#define AttrNone      0x0
#define AttrPosition  0x1
#define AttrNormal    0x2
#define AttrTangent   0x4
#define AttrDiffuse   0x8
#define AttrSpecular  0x10
#define AttrTex0      0x20
#define AttrTex1      0x40
#define AttrTex2      0x80
#define AttrTex3      0x100
#define AttrTex4      0x200
#define AttrTex5      0x400
#define AttrTex6      0x800
#define AttrTex7      0x1000

// mesh with fixed vertex attribute
class Mesh : public MeshBase
{
public:
    Mesh();
    virtual ~Mesh();

    // load mesh data 
    void Load(AttrIDS ids, const void *vertices, GLsizei vstride, GLuint vcount);
    void Load(AttrIDS ids, const void *indices, GLsizei istride, GLuint icount, const void *vertices, GLsizei vstride, GLuint vcount);

private:
    AttrIDS _attrIDs;
};

// mesh extention with user-defined vertex attribute
class MeshExt : public MeshBase
{
public:
    MeshExt();
    virtual ~MeshExt();

    // load mesh data
    void LoadAttrs(const void *vertices, GLsizei stride, GLuint count);
    void AddAttrs(const void *vertices, GLsizei stride, GLuint count);
    void LoadIndices(const void *indices, GLsizei stride, GLuint count);

private:
    GLuint _vertexSize;
};

// load texture from file
GLuint LoadTexture(const char *path, bool alpha = false);
// faces in order : right, left, top, bottom, front, back
GLuint LoadCubeTexture(const vector<const char*> &faces, bool alpha = false);

class Model
{
public:

    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 texcoord;
    };

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
    void DrawInstanced(Shader *shader, GLuint amout);

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