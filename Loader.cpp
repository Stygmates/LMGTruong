#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Loader.hpp"
#include <vector>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <fstream>

Loader::Loader()
{
}

void Loader::import( std::string filename )
{
    this->importer = new Assimp::Importer();
    this->scene = this->importer->ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
    assert( this->scene != NULL );
    this->importer->SetPropertyInteger( AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
}

void Loader::loadData(std::vector< glm::vec3 > &positions, std::vector< unsigned int > &faces, std::vector< glm::vec2 > &texCoords )
{
    #ifdef DEBUG
    std::ofstream verticesdebug;
    verticesdebug.open("debug/vertices.txt");
    std::ofstream indexesdebug;
    indexesdebug.open("debug/indexes.txt");
    std::ofstream objinfodebug;
    objinfodebug.open("debug/infos.txt");
    objinfodebug << "Number of Animations: " << this->scene->mNumAnimations << std::endl;
    objinfodebug << "Number of cameras: " << this->scene->mNumCameras << std::endl;
    objinfodebug << "Number of lights: " << this->scene->mNumLights << std::endl;
    objinfodebug << "Number of materials: " << this->scene->mNumMaterials << std::endl;
    objinfodebug << "Number of meshes: " << this->scene->mNumMeshes << std::endl;
    objinfodebug << "Number of textures: " << this->scene->mNumTextures << std::endl;
    std::ofstream texturesdebug;
    texturesdebug.open("debug/textures.txt");
    #endif
    if( this->scene->HasMeshes() )
    {
        //positions.resize( this->scene->mNumMeshes );
        for( unsigned int i = 0; i < this->scene->mNumMeshes; i++ )
        {
            aiMesh *mesh = this->scene->mMeshes[ i ];
            //positions[ i ].resize( mesh->mNumVertices );
            #ifdef DEBUG
            //verticesdebug << "Chargement de " << mesh->mName.C_Str() << std::endl << "Nombre de sommets: " << mesh->mNumVertices << std::endl;
            verticesdebug << "Vertices number: " << mesh->mNumVertices << std::endl;
            indexesdebug << "Loading " << mesh->mName.C_Str() << std::endl << "Number of faces: " << mesh->mNumFaces << std::endl;
            objinfodebug << "Mesh named " <<mesh->mName.C_Str() << std::endl;
            #endif
            for( unsigned int j = 0; j < mesh->mNumVertices; j++ )
            {
                const aiVector3D& position = mesh->mVertices[ j ];
                glm::vec3 v = glm::vec3( position.x, position.y, position.z );
                positions.push_back( v );
                #ifdef DEBUG
                verticesdebug << "\tVertice " << j << ": " << v.x << ", " << v.y << ", " << v.z << std::endl;
                #endif
            }
            for( unsigned int j = 0; j < mesh->mNumFaces; j++ )
            {
                const aiFace& face = mesh->mFaces[ j ]; 
                if( face.mNumIndices == 3 )
                {
                    faces.push_back( face.mIndices[ 0 ] );
                    faces.push_back( face.mIndices[ 1 ] );
                    faces.push_back( face.mIndices[ 2 ] );
                    #ifdef DEBUG
                    indexesdebug << "\tFace " << j << ": " << face.mIndices[ 0 ] << ", " << face.mIndices[ 1 ] << ", " << face.mIndices[ 2 ] << std::endl;
                    //indexesdebug << face.mIndices[ 0 ] << std::endl << face.mIndices[ 1 ] << std::endl << face.mIndices[ 2 ] << std::endl;
                    #endif
                }
            }
            if( mesh->HasTextureCoords( 0 ) )
            {
                texCoords.resize( mesh->mNumVertices );
                for( unsigned int j = 0; j < mesh->mNumVertices; j++ )
                {
                    const aiVector3D& texCoord = mesh->mTextureCoords[ 0 ][ j ];
                    texCoords[ j ] = glm::vec2(texCoord.x, texCoord.y );
                    #ifdef DEBUG
                    texturesdebug << texCoord.x << ", " << texCoord.y << std::endl;
                    #endif
                }
            }
        }
    }
    #ifdef DEBUG
    verticesdebug.close();
    indexesdebug.close();
    texturesdebug.close();
    objinfodebug.close();
    #endif
}
