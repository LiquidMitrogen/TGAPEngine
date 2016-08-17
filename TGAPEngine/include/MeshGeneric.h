#ifndef MESHGENERIC_H
#define MESHGENERIC_H

#include "Mesh.h"

namespace engine{
class MeshGeneric : public Mesh
{
    public:
        /** Default constructor */
        MeshGeneric();
        /** Default destructor */
        virtual ~MeshGeneric();
        /** Access vertices
         * \return The current value of vertices
         */
        std::vector<float> getVertices() { return vertices; }
        /** Set vertices
         * \param val New value to set
         */
        void setVertices(std::vector<float> val) { vertices = val; }
        /** Access normals
         * \return The current value of normals
         */
        std::vector<float> getNormals() { return normals; }
        /** Set normals
         * \param val New value to set
         */
        void setNormals(std::vector<float> val) { normals = val; }
        void dupa();
    protected:
    private:
        std::vector<float> vertices; //!< Member variable "vertices"
        std::vector<float> normals; //!< Member variable "normals"
};
}

#endif // MESHGENERIC_H
