#ifndef __LOADER_HPP
#define __LOADER_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class Loader{
    public:
        void import( std::string filename );
        void loadData( std::vector< std::vector< glm::vec3 > >& positions, std::vector< unsigned int>& faces);
        void normalize( std::vector< std::vector< glm::vec3 > >& positions );
    private:
        const aiScene *scene;
        Assimp::Importer *importer;
};

#endif