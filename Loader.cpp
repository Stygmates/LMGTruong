#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Loader.hpp"
#include <vector>
#include <cstdio>
#include <iostream>

void Loader::import( const char* filename )
{
    this->importer = new Assimp::Importer();
    this->scene = this->importer->ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
    if( this->scene == NULL )
    {
        fprintf(stderr, "Erreur: Chargement de l'objet %s impossible.\n", filename);
        exit(1);
    }
}

void Loader::loadData( std::vector< std::vector< glm::vec3 > >& positions )
{
    if(this->scene->HasMeshes())
    {
        positions.resize( this->scene->mNumMeshes );
        for( unsigned int i = 0; i < this->scene->mNumMeshes; i++ )
        {

            aiMesh *mesh = this->scene->mMeshes[ i ];
            positions[ i ].resize( mesh->mNumVertices );
            for( unsigned int j = 0; j < mesh->mNumVertices; j++)
            {
                const aiVector3D& position = mesh->mVertices[ j ];
                glm::vec3 v = glm::vec3( position.x, position.y, position.z );
                glm::normalize(v);
                positions[ i ][ j ] = v; 
            }
        }
    }
}