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
    this->importer->SetPropertyInteger( AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT );
}

void Loader::loadData(std::vector< glm::vec3 > &positions, std::vector< unsigned int > &faces, std::vector< glm::vec2 > &texCoords )
{
    #ifdef DEBUG
    std::ofstream verticesdebug;
    verticesdebug.open("debug/vertices.txt");
    std::ofstream indexesdebug;
    indexesdebug.open("debug/indexes.txt");
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
            verticesdebug << "Nombre de sommets: " << mesh->mNumVertices << std::endl;
            indexesdebug << "Chargement de " << mesh->mName.C_Str() << std::endl << "Nombre de faces: " << mesh->mNumFaces << std::endl;
            #endif
            for( unsigned int j = 0; j < mesh->mNumVertices; j++ )
            {
                const aiVector3D& position = mesh->mVertices[ j ];
                glm::vec3 v = glm::vec3( position.x, position.y, position.z );
                positions.push_back( v );
                #ifdef DEBUG
                verticesdebug << "\tSommet " << j << ": " << v.x << ", " << v.y << ", " << v.z << std::endl;
                #endif
            }
            for( unsigned int j = 0; j < mesh->mNumFaces; j++ )
            {
                const aiFace& face = mesh->mFaces[ j ]; 
                assert( face.mNumIndices == 3 );
                faces.push_back( face.mIndices[ 0 ] );
                faces.push_back( face.mIndices[ 1 ] );
                faces.push_back( face.mIndices[ 2 ] );
                #ifdef DEBUG
                indexesdebug << "\tFace " << j << ": " << face.mIndices[ 0 ] << ", " << face.mIndices[ 1 ] << ", " << face.mIndices[ 2 ] << std::endl;
                //indexesdebug << face.mIndices[ 0 ] << std::endl << face.mIndices[ 1 ] << std::endl << face.mIndices[ 2 ] << std::endl;
                #endif
            }
            if( mesh->HasTextureCoords( 0 ) )
            {
                texCoords.resize( mesh->mNumVertices );
                for( unsigned int j = 0; j < mesh->mNumVertices; j++ )
                {
                    const aiVector3D& texCoord = mesh->mTextureCoords[ 0 ][ j ];
                    texCoords[ j ] = glm::vec2(texCoord.x, texCoord.y );
                }
            }
        }
        normalize( positions );
    }
    #ifdef DEBUG
    verticesdebug.close();
    indexesdebug.close();
    #endif
}


// void Loader::normalize( std::vector< std::vector< glm::vec3 > >& positions )
// {
//     #ifdef DEBUG
//     std::ofstream normalizeddebug;
//     normalizeddebug.open("debug/normalized.txt");
//     normalizeddebug << "Nombre de sommets: " << positions[0].size() << std::endl;
//     #endif
//     float minx, miny, minz;
//     float maxx, maxy, maxz;
//     assert( positions.size() > 0 );
//     minx = positions[0][0].x;
//     maxx = positions[0][0].x;
//     miny = positions[0][0].y;
//     maxy = positions[0][0].y;
//     minz = positions[0][0].z;
//     maxz = positions[0][0].z;
//     std::vector< std::vector< glm::vec3 > >::iterator it;
// 	for( it = positions.begin(); it != positions.end(); ++it )
// 	{
// 		std::vector< glm::vec3 >::iterator it2;
// 		for( it2 = it->begin(); it2 != it->end(); ++it2 )
// 		{
//             if( minx > it2->x )
//                 minx = it2->x;
//             if( miny > it2->y )
//                 miny = it2->y;
//             if( minz > it2->z )
//                 minz = it2->z;
//             if( maxx < it2->x )
//                 maxx = it2->x;
//             if( maxy < it2->y )
//                 maxy = it2->y;
//             if( maxz < it2->z )
//                 maxz = it2->z;
// 		}
// 	}
//     if( minx == maxx )
//     {
//         std::cout << "xxx" << minx << std::endl;
//         maxx = 1;
//         minx = 0;
//     }
//     if( miny == maxy )
//     {
//         std::cout << "yyy" << miny << std::endl;
//         maxy = 1;
//         miny = 0;
//     }
//     if( minz == maxz )
//     {
//         std::cout << "zzz" << minz << std::endl;
//         maxz = 1;
//         minz = 0;
//     }
//     int j = 0;
//     for( it = positions.begin(); it != positions.end(); ++it )
// 	{
// 		std::vector< glm::vec3 >::iterator it2;
// 		for( it2 = it->begin(); it2 != it->end(); ++it2 )
//         {
//             it2->x = ( it2->x - minx ) / ( maxx - minx);
//             it2->x = ( it2->x * 2 ) - 1;
//             it2->y = ( it2->y - miny ) / ( maxy - miny);
//             it2->y = ( it2->y * 2 ) - 1;
//             it2->z = ( it2->z - minz ) / ( maxz - minz);
//             it2->z = ( it2->z * 2 ) - 1;
//             #ifdef DEBUG
//             normalizeddebug << "\tSommet " << j << ": " << it2->x << ", " << it2->y << ", " << it2->z << std::endl;
//             #endif
//             j++;
//         }
// 	}
// }

void Loader::normalize(std::vector< glm::vec3 > &positions)
{
#ifdef DEBUG
    std::ofstream normalizeddebug;
    normalizeddebug.open("debug/normalized.txt");
    normalizeddebug << "Nombre de sommets: " << positions.size() << std::endl;
#endif
    float minx, miny, minz;
    float maxx, maxy, maxz;
    assert(positions.size() > 0);
    minx = positions[0].x;
    maxx = positions[0].x;
    miny = positions[0].y;
    maxy = positions[0].y;
    minz = positions[0].z;
    maxz = positions[0].z;
    std::vector< glm::vec3 >::iterator it;
    for (it = positions.begin(); it != positions.end(); ++it)
    {
            if (minx > it->x)
                minx = it->x;
            if (miny > it->y)
                miny = it->y;
            if (minz > it->z)
                minz = it->z;
            if (maxx < it->x)
                maxx = it->x;
            if (maxy < it->y)
                maxy = it->y;
            if (maxz < it->z)
                maxz = it->z;
    }
    if (minx == maxx)
    {
        std::cout << "xxx" << minx << std::endl;
        maxx = 1;
        minx = 0;
    }
    if (miny == maxy)
    {
        std::cout << "yyy" << miny << std::endl;
        maxy = 1;
        miny = 0;
    }
    if (minz == maxz)
    {
        std::cout << "zzz" << minz << std::endl;
        maxz = 1;
        minz = 0;
    }
    int j = 0;
    for (it = positions.begin(); it != positions.end(); ++it)
    {
        it->x = (it->x - minx) / (maxx - minx);
        it->x = (it->x * 2) - 1;
        it->y = (it->y - miny) / (maxy - miny);
        it->y = (it->y * 2) - 1;
        it->z = (it->z - minz) / (maxz - minz);
        it->z = (it->z * 2) - 1;
#ifdef DEBUG
        normalizeddebug << "\tSommet " << j << ": " << it->x << ", " << it->y << ", " << it->z << std::endl;
#endif
        j++;
    }
}