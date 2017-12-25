#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Loader.hpp"
#include <vector>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <fstream>

void Loader::import( std::string filename )
{
    this->importer = new Assimp::Importer();
    this->scene = this->importer->ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
    assert( this->scene != NULL );
}

void Loader::loadData( std::vector< std::vector< glm::vec3 > >& positions, std::vector< unsigned int>& faces )
{
    #ifdef DEBUG
    std::ofstream verticesdebug;
    verticesdebug.open("debug/vertices.txt");
    std::ofstream indexesdebug;
    indexesdebug.open("debug/indexes.txt");
    #endif
    if( this->scene->HasMeshes() )
    {
        positions.resize( this->scene->mNumMeshes );
        for( unsigned int i = 0; i < this->scene->mNumMeshes; i++ )
        {
            aiMesh *mesh = this->scene->mMeshes[ i ];
            positions[ i ].resize( mesh->mNumVertices );
            #ifdef DEBUG
            verticesdebug << "Nombre de sommets: " << mesh->mNumVertices << std::endl;
            indexesdebug << "Nombre de faces: " << mesh->mNumVertices << std::endl;
            #endif
            for( unsigned int j = 0; j < mesh->mNumVertices; j++ )
            {
                const aiVector3D& position = mesh->mVertices[ j ];
                glm::vec3 v = glm::vec3( position.x, position.y, position.z );
                positions[ i ][ j ] = v;
                #ifdef DEBUG
                verticesdebug << "Sommet " << v.x << ", " << v.y << ", " << v.z << std::endl;
                #endif
            }
            for( unsigned int j = 0; j < mesh->mNumFaces; j++ )
            {
                const aiFace& face = mesh->mFaces[ j ]; 
                assert( face.mNumIndices == 3);
                faces.push_back( face.mIndices[ 0 ] );
                faces.push_back( face.mIndices[ 1 ] );
                faces.push_back( face.mIndices[ 2 ] );
                #ifdef DEBUG
                indexesdebug << "Face " << face.mIndices[ 0 ] << ", " << face.mIndices[ 1 ] << ", " << face.mIndices[ 2 ] << std::endl;
                #endif
            }
        }
        normalize( positions );
    }
    #ifdef DEBUG
    verticesdebug.close();
    indexesdebug.close();
    #endif
}


void Loader::normalize( std::vector< std::vector< glm::vec3 > >& positions )
{
    float minx, miny, minz;
    float maxx, maxy, maxz;
    assert( positions.size() > 0 );
    minx = positions[0][0].x;
    maxx = positions[0][0].x;
    miny = positions[0][0].y;
    maxy = positions[0][0].y;
    minz = positions[0][0].z;
    maxz = positions[0][0].z;
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