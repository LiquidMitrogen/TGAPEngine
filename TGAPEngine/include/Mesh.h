#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include<vector>
namespace engine{
    class Mesh{
    public:
       virtual ~Mesh(){};
       virtual std::vector<float> getVetrices() = 0;
       virtual  std::vector<float> getNormals() = 0;
    };
}



#endif // MESH_H_INCLUDED
