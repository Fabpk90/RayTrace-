//
// Created by fab on 23/02/2020.
//

#include "Vbo.hpp"

Vbo::Vbo(int nbElements, int nbVertices, bool useEbo) : nbVertices(nbVertices), nbElements(nbElements), useEbo(useEbo)
{
    elements = new Element[nbElements];
    elementsValue = nullptr;

    vao = 0;
    vbo = 0;
    sizeOfOneVertex = 0;
}

void Vbo::setElementDescription(int index, Vbo::Element desc)
{
    elements[index] = desc;

    //we reset the count to allow redefining descriptions
    sizeOfOneVertex = 0;

    for (int i = 0; i < nbElements; ++i)
    {
        sizeOfOneVertex += elements[i].NbFloats;
    }
}

void Vbo::createCPUSide()
{
    totalElementSize = 0;

    for (int i = 0; i < nbElements; ++i)
    {
        elements[i].OffsetFloats = totalElementSize;
        totalElementSize += elements[i].NbFloats;
    }

    elementsValue = new float[totalElementSize * nbVertices];

}

void Vbo::setElementData(int dataIndex, int vertexIndex, float f1)
{
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex)] = f1;
}

void Vbo::setElementData(int dataIndex, int vertexIndex, float f1, float f2)
{
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex)] = f1;
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex) + 1] = f2;
}

void Vbo::setElementData(int dataIndex, int vertexIndex, float f1, float f2, float f3)
{
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex)] = f1;
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex) + 1] = f2;
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex) + 2] = f3;
}

void Vbo::setElementData(int dataIndex, int vertexIndex, float f1, float f2, float f3, float f4)
{
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex)] = f1;
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex) + 1] = f2;
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex) + 2] = f3;
    elementsValue[elements[dataIndex].OffsetFloats + (vertexIndex * sizeOfOneVertex) + 3] = f4;
}

void Vbo::createGPUSide()
{
    if (vao != 0)
        glDeleteVertexArrays(1, &vao);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    if (vbo != 0)
        glDeleteBuffers(1, &vbo);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    for (int i = 0; i < nbElements; ++i)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, elements[i].NbFloats, GL_FLOAT, GL_FALSE, (sizeof(float) * sizeOfOneVertex),
                              (void *) (sizeof(float) * elements[i].OffsetFloats));
    }
    glBufferData(GL_ARRAY_BUFFER, totalElementSize * nbVertices * sizeof(float), elementsValue, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Vbo::deleteCPUSide()
{
    delete[] elementsValue;
    elementsValue = nullptr;
}

Vbo::~Vbo()
{
    deleteCPUSide();
    delete[] elements;
}

void Vbo::deleteGPUSide()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Vbo::draw()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDrawArrays(GL_TRIANGLES, 0, nbVertices);
}




