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
                positions[ i ][ j ] = v; 
            }
        }
        normalize( positions );
    }
}


void Loader::normalize( std::vector< std::vector< glm::vec3 > >& positions )
{
    float minx, miny, minz;
    float maxx, maxy, maxz;
    if( positions.size() > 0 )
    {
        if( positions[0].size() > 0 )
        {
            minx = positions[0][0].x;
            maxx = positions[0][0].x;
            miny = positions[0][0].y;
            maxy = positions[0][0].y;
            minz = positions[0][0].z;
            maxz = positions[0][0].z;
        }
        else
        {
            std::cerr << "Erreur lors de la normalisation : vector vide.\n";
        }
    }
    else
    {
        std::cerr << "Erreur lors de la normalisation : vector vide.\n";
    }
    std::vector< std::vector< glm::vec3 > >::iterator it;
	for( it = positions.begin(); it != positions.end(); ++it )
	{
		std::vector< glm::vec3 >::iterator it2;
		for( it2 = it->begin(); it2 != it->end(); ++it2 )
		{
            if( minx > it2->x )
                minx = it2->x;
            if( miny > it2->y )
                miny = it2->y;
            if( minz > it2->z )
                minz = it2->z;
            if( maxx < it2->x )
                maxx = it2->x;
            if( maxy < it2->y )
                maxy = it2->y;
            if( maxz < it2->z )
                maxz = it2->z;
		}
	}
    for( it = positions.begin(); it != positions.end(); ++it )
	{
		std::vector< glm::vec3 >::iterator it2;
		for( it2 = it->begin(); it2 != it->end(); ++it2 )
        {
            it2->x = ( it2->x - minx ) / ( maxx - minx);
            it2->x = ( it2->x * 2 ) - 1;
            it2->y = ( it2->y - miny ) / ( maxy - miny);
            it2->y = ( it2->y * 2 ) - 1;
            it2->z = ( it2->z - minz ) / ( maxz - minz);
            it2->z = ( it2->z * 2 ) - 1;
		}
	}
}