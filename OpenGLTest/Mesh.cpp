#include "stdafx.h"
#include "Mesh.h"
#include "Shader.h"

#include <sstream>
#include <iostream>


MeshBase::MeshBase() :
    _hasElemArray(false),
    _attrIndex(0),
    _vao(0),
    _vbo(0),
    _ebo(0),
    _vertices(0),
    _indices(0)
{
}

MeshBase::~MeshBase()
{
    Unload();
}

void MeshBase::AddTexture(const Texture &texture)
{
    _textures.push_back(texture);
}

void MeshBase::AddTexture(const vector<Texture> &textures)
{
    _textures.assign(textures.begin(), textures.end());
}

void MeshBase::RemoveTextures()
{
    for (auto tex : _textures)
    {
        glDeleteTextures(1, &tex.id);
    }
    _textures.clear();
}

void MeshBase::Unload()
{
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_ebo) glDeleteBuffers(1, &_ebo);
    if (_vao) glDeleteVertexArrays(1, &_vao);

    RemoveTextures();
}

void MeshBase::Draw(Shader *shader)
{
    // Bind appropriate textures
    for (GLuint i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Now set the sampler to the correct texture unit
        shader->SetParamInt(_textures[i].uname.c_str(), i);
        // And finally bind the texture
        glBindTexture(_textures[i].type, _textures[i].id);
    }

    // Draw mesh
    glBindVertexArray(_vao);
    if (_hasElemArray)
        glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, _vertices);
    glBindVertexArray(0);

    // Restore texture to origin state
    for (GLuint i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void MeshBase::DrawInstanced(Shader *shader, GLuint amout)
{
    // Bind appropriate textures
    for (GLuint i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
        // Now set the sampler to the correct texture unit
        shader->SetParamInt(_textures[i].uname.c_str(), i);
        // And finally bind the texture
        glBindTexture(_textures[i].type, _textures[i].id);
    }

    // Draw mesh
    glBindVertexArray(_vao);
    if (_hasElemArray)
        glDrawElementsInstanced(GL_TRIANGLES, _indices, GL_UNSIGNED_INT, 0, amout);
    else
        glDrawArraysInstanced(GL_TRIANGLES, 0, _vertices, amout);
    glBindVertexArray(0);

    // Restore texture to origin state
    for (GLuint i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

Mesh::Mesh() :
    _attrIDs(AttrNone)
{
}

Mesh::~Mesh()
{
}

void Mesh::Load(AttrIDS ids, const void *vertices, GLsizei vstride, GLuint vcount)
{
    if (!_vao) glGenVertexArrays(1, &_vao);
    if (!_vbo) glGenBuffers(1, &_vbo);
    _vertices = vcount;

    GLuint offset = 0;
    _attrIDs = ids;

    glBindVertexArray(_vao);

    // bind vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, vcount * vstride, vertices, GL_STATIC_DRAW);

    const GLuint attrIDSize[] = { 3, 3, 3, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2 };

    for (int i = 0; i != (sizeof(attrIDSize) / sizeof(GLuint)); ++i)
    {
        if (_attrIDs & (1 << i))
        {
            glEnableVertexAttribArray(_attrIndex);
            glVertexAttribPointer(_attrIndex, attrIDSize[i], GL_FLOAT, GL_FALSE, vstride, (const void*)offset);
            ++_attrIndex;
            offset += attrIDSize[i] * sizeof(GL_FLOAT);
        }
    }

    glBindVertexArray(0);
}

void Mesh::Load(AttrIDS ids, const void *indices, GLsizei istride, GLuint icount, const void *vertices, GLsizei vstride, GLuint vcount)
{
    _hasElemArray = true;
    _indices = icount;

    if (!_vao) glGenVertexArrays(1, &_vao);
    if (!_ebo) glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    // bind array elements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * istride, indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    Load(ids, vertices, vstride, vcount);
}

MeshExt::MeshExt() :
    _vertexSize(0)
{
}

MeshExt::~MeshExt()
{
}

void MeshExt::LoadAttrs(const void *vertices, GLsizei stride, GLuint count)
{
    if (!_vao) glGenVertexArrays(1, &_vao);
    if (!_vbo) glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, _vertexSize, count * stride, vertices);
    glBindVertexArray(0);

    _vertexSize += count * stride;
}

void MeshExt::AddAttrs(const void *vertices, GLsizei stride, GLuint count)
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, _vertexSize, count * stride, vertices);
    glBindVertexArray(0);

    _vertexSize += count * stride;
}

void MeshExt::LoadIndices(const void *indices, GLsizei stride, GLuint count)
{
    _hasElemArray = true;
    _indices = count;

    if (!_vao) glGenVertexArrays(1, &_vao);
    if (!_ebo) glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    // bind array elements
    glBindBuffer(GL_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * stride, indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

GLuint LoadTexture(const char *path, bool alpha)
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

GLuint LoadCubeTexture(const vector<const char*> &faces, bool alpha)
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height;
    unsigned char* image;

    for (int i = 0; i < faces.size(); ++i)
    {        
        image = SOIL_load_image(faces[i], &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }

    // Parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

void Model::Draw(Shader *shader)
{
    for (auto iter = _meshes.begin(); iter != _meshes.end(); ++iter)
    {
        (*iter)->Draw(shader);
    }
}

void Model::DrawInstanced(Shader *shader, GLuint amout)
{
    for (auto iter = _meshes.begin(); iter != _meshes.end(); ++iter)
    {
        (*iter)->DrawInstanced(shader, amout);
    }
}

void Model::LoadModel(string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // Check for errors
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        DEBUG_LOG("Error: loadModel failed.\n");
        DEBUG_LOG(importer.GetErrorString());
        return;
    }
    // Retrieve the directory path of the filepath
    _directory = path.substr(0, path.find_last_of('\\')+1);

    // Process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Model::Unload()
{
    for (auto iter = _meshes.begin(); iter != _meshes.end(); ++iter)
        delete (*iter);

    _meshes.clear();
    _textures.clear();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh *Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        Vector3 vector;
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
        // Texture Coordinates
        if (mesh->mTextureCoords[0])
        {
            Vector2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texcoord = vec;
        }
        else
            vertex.texcoord = Vector2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }

    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // Process materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. Diffuse maps
        vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. Specular maps
        vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Return a mesh object created from the extracted mesh data
    Mesh *newMesh = new Mesh();
    int attrIds = AttrPosition | AttrNormal | AttrTex0;
    newMesh->Load((AttrIDS)attrIds, &indices[0], sizeof(GLuint), indices.size(), &vertices[0], sizeof(Vertex), vertices.size());
    newMesh->AddTexture(textures);
    return newMesh;
}

vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;
    int diffuseNr = 0;
    int specularNr = 0;

    for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Retrieve texture number (the N in diffuse_textureN)
        std::stringstream ss;
        string number;
        if (typeName == "texture_diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if (typeName == "texture_specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();
        string uname = typeName + number;

        // Check if texture was loaded before
        GLboolean skip = false;
        for (GLuint j = 0; j < _textures.size(); j++)
        {
            if (!std::strcmp(_textures[j].path.c_str(), str.C_Str()))
            {
                textures.push_back(_textures[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            string fileName = _directory;
            const char *name = str.C_Str();
            while (*name && (*name == '.' || *name == '\\'))
                ++name;
            fileName += name;

            texture.id = LoadTexture(fileName.c_str());
            texture.type = GL_TEXTURE_2D;
            texture.map = typeName;
            texture.path = name;
            texture.uname = uname;
            textures.push_back(texture);

            _textures.push_back(texture);
        }
    }
    return textures;
}