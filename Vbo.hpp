//
// Created by fab on 23/02/2020.
//

#ifndef DUMBERENGINE_VBO_HPP
#define DUMBERENGINE_VBO_HPP

#include <glad/glad.h>

class Vbo
{
public:
    struct Element
    {
        int NbFloats = 0;
        int OffsetFloats = 0;

        Element(int nbFloats)
        {
            NbFloats = nbFloats;
        }

        Element()
        {
            NbFloats = 0;
        }

    };

    Vbo(int nbElements, int nbVertices, bool useEbo = false);

    ~Vbo();

    void setElementDescription(int index, Element desc);

    void setElementData(int dataIndex, int vertexIndex, float f1);

    void setElementData(int dataIndex, int vertexIndex, float f1, float f2);

    void setElementData(int dataIndex, int vertexIndex, float f1, float f2, float f3);

    void setElementData(int dataIndex, int vertexIndex, float f1, float f2, float f3, float f4);

    void createCPUSide();

    void deleteCPUSide();

    void createGPUSide();

    void deleteGPUSide();

    void draw();

private:
    Element *elements;
    int nbElements;
    int nbVertices;

    int totalElementSize;
    int sizeOfOneVertex;

    float *elementsValue;

    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    bool useEbo;
};

#endif //DUMBERENGINE_VBO_HPP
