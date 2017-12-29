#include "MeshLoader.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>

Loader::Loader()
{
}
Loader::~Loader()
{

}

void Loader::import(std::string filename)
{
	this->filename = filename.c_str();
}

/*
void Loader::loadData(std::vector<glm::vec3> &positions, std::vector<unsigned int> &faces, std::vector<glm::vec2> &texCoords)
{
#ifdef DEBUG
	std::ofstream indexesdebug;
	indexesdebug.open("debug/indexes.txt");
#endif
	float x, y, z;
	int a1, a2, a3, b1, b2, b3, c1, c2, c3;
	FILE *file = fopen( this->filename, "r" );
	if( file == NULL )
	{
		fprintf(stderr, "Cannot open file %s\n", this->filename );
		return;
	}

	while( 1 )
	{
		char lineHeader[ 128 ];
		int res = fscanf( file, "%s", lineHeader );
		if( res == EOF )
		{
			break;
		}

		//Sommet
		if( strcmp( lineHeader, "v" ) == 0 )
		{
			if( fscanf( file, "%f %f %f", &x, &y, &z ) != EOF )
			{
            	glm::vec3 v = glm::vec3( x, y, z );
				positions.push_back( v );
			}
		}

		//Texture
		if( strcmp( lineHeader, "vt" ) == 0 )
		{
			if( fscanf( file, "%f %f", &x, &y ) != EOF )
			{
				glm::vec2 vt = glm::vec2( x, y );
				texCoords.push_back( vt );
			}
		}

		//Faces
		if( strcmp( lineHeader, "f" ) == 0)
		{
			if (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &a1, &a2, &a3, &b1, &b2, &b3, &c1, &c2, &c3 ) != EOF)
			{
				faces.push_back( a1 );
				faces.push_back( b1 );
				faces.push_back( c1 );
				#ifdef DEBUG
				indexesdebug << a1 << std::endl << b1 << std::endl << c1 << std::endl;
				#endif
			}
		}
	}
	#ifdef DEBUG
	indexesdebug.close();
	#endif
}
*/

void Loader::loadData(std::vector<glm::vec3> &positions, std::vector<unsigned int> &faces, std::vector<glm::vec2> &texCoords2)
{
	#ifdef DEBUG
	std::ofstream verticesdebug;
	verticesdebug.open("debug/vertices.txt");
	std::ofstream indexesdebug;
	indexesdebug.open("debug/indexes.txt");
	#endif

	std::vector< glm::vec2 > texCoords;
	std::vector< unsigned int > texIndexes;

	std::string line;
	std::ifstream file( this->filename );
	assert( file );
	int j = 0;
	while( getline( file, line ) )
	{
		std::vector< std::string > words;
		//boost::split( words, line, [](char c){ return c == ' '; });
		boost::algorithm::split(words, line, boost::is_any_of("\t "), boost::token_compress_on);
		std::vector< unsigned int > toDelete;
		for( unsigned int l = 0; l < words.size(); l++ )
		{
			if( words[ l ].compare( "\r" ) == 0 )
			{
				toDelete.push_back( l );
			}
		}
		std::reverse( toDelete.begin(), toDelete.end() );

		for( unsigned int l = 0; l < toDelete.size(); l ++ )
		{
			words.erase( words.begin() + toDelete[ l ] );
		}

		if( words.size() > 0 )
		{
			//Sommets
			if( words[ 0 ].compare( "v" ) == 0 )
			{
				try
				{
					float x = stof( words[ 1 ] );
					float y = stof( words[ 2 ] );
					float z = stof( words[ 3 ] );
					glm::vec3 v = glm::vec3( x, y, z );
					positions.push_back( v );
					#ifdef DEBUG
					verticesdebug << v.x << ", " << v.y << ", " << v.z << std::endl;
					#endif
				}
				catch( const std::invalid_argument &)
				{
					std::cerr << "Ligne " << j << ": Cannot convert" << words[ 1 ] << " or " << words[ 2 ] << " or " << words[ 3 ] << std::endl; 
				}
			}

			//Coordonnées de texture
			if ( words[ 0 ].compare( "vt" ) == 0 )
			{
				float x = stof( words[ 1 ] );
				float y = stof( words[ 2 ] );
				glm::vec2 vt = glm::vec2( x, y );
				texCoords.push_back( vt );	
			}

			//Faces
			if( words[ 0 ].compare( "f" ) == 0 )
			{
				//Triangles
				if(( words.size() == 4 ) )
				{
					for(unsigned int i = 1; i < words.size(); i++ )
					{
						std::vector< std::string > value;
						boost::split(value, words[ i ], [](char c) { return c == '/'; });
						try
						{
							unsigned int val = stoul( value[ 0 ] ) - 1;
							faces.push_back(val);
							if (value.size() > 1)
							{
								if( !value[ 1 ].empty() )
								{
									unsigned int val2 = stoul( value[ 1 ] ) - 1;
									texIndexes.push_back(val2);
								}
							}
						}
						catch( const std::invalid_argument & )
						{
							std::cerr << "Ligne " << j << ": Conversion de " << value[ 0 ] << "ou" << value[ 1 ] << " impossible" << std::endl;
						}
					}
				}
				if ((words.size() == 5))
				{
					std::vector< unsigned int > face;
					std::vector< unsigned int > texture;
					for (unsigned int i = 1; i < words.size(); i++)
					{
						std::vector< std::string > value;
						boost::split( value, words[ i ], []( char c ) { return c == '/'; });
						try
						{
							unsigned int val = stoul( value[ 0 ] ) - 1;
							face.push_back( val );
							if( value.size() > 1 )
							{
								if ( !value[1].empty() )
								{
									unsigned int val2 = stoul(value[1]) - 1;
									texture.push_back( val2 );
								}
							}
						}
						catch (const std::invalid_argument &)
						{
							std::cerr << "La liste des elements est " << std::endl;
							for(unsigned int k = 0; k < value.size(); k++ )
							{
								std::cerr << "Value " << k << value[ k ] << std::endl;
							}
							std::cerr << "Ligne " << j << ": Conversion de " << value[ 0 ] << " impossible" << std::endl;
						}
					}
					faces.push_back( face[ 0 ] );
					faces.push_back( face[ 1 ] );
					faces.push_back( face[ 2 ] );
					faces.push_back( face[ 0 ] );
					faces.push_back( face[ 2 ] );
					faces.push_back( face[ 3 ] );
					texIndexes.push_back( texture[ 0 ] );
					texIndexes.push_back( texture[ 1 ] );
					texIndexes.push_back( texture[ 2 ] );
					texIndexes.push_back( texture[ 0 ] );
					texIndexes.push_back( texture[ 2 ] );
					texIndexes.push_back( texture[ 3 ] );

					#ifdef DEBUG
					indexesdebug << face[ 0 ] << std::endl
								 << face[ 1 ] << std::endl
								 << face[ 2 ] << std::endl;
					indexesdebug << face[ 0 ] << std::endl
								 << face[ 2 ] << std::endl
								 << face[ 3 ] << std::endl;
					#endif
				}
			}
		}
		j++;
	}
	textureCoordsFill(texCoords2, texCoords, texIndexes );

	#ifdef DEBUG
	verticesdebug.close();
	indexesdebug.close();
	#endif
}

void Loader::textureCoordsFill(std::vector< glm::vec2 > &texCoords2, const std::vector< glm::vec2 > texCoords, const std::vector< unsigned int > texIndexes)
{
	#ifdef DEBUG
	std::ofstream texturesdebug;
	texturesdebug.open("debug/textures.txt");
	#endif
	for ( unsigned int i = 0; i < texIndexes.size(); i++ )
	{
		texCoords2.push_back( texCoords[ texIndexes[ i ] ] );
		#ifdef DEBUG
		texturesdebug << texCoords[ texIndexes[ i ] ].x << "\t" << texCoords[ texIndexes[ i ] ].y << std::endl;
		#endif
	}
	#ifdef DEBUG
	texturesdebug.close();
	#endif
}