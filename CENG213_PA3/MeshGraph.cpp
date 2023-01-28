#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>

MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    vertices.resize(vertexPositions.size());
    for (unsigned i = 0; i < vertexPositions.size(); ++i)
    {
        vertices[i].id=(int) i;
        vertices[i].position3D.x = vertexPositions[i].x;
        vertices[i].position3D.y = vertexPositions[i].y;
        vertices[i].position3D.z = vertexPositions[i].z;
    }
    adjList.resize(vertices.size());
    for (unsigned i = 0; i < edges.size(); i++)
    {
        adjList[(unsigned) edges[i].vertexId0].push_back(&vertices[(unsigned) edges[i].vertexId1]);
        adjList[(unsigned) edges[i].vertexId1].push_back(&vertices[(unsigned) edges[i].vertexId0]);
    }
    for (unsigned i = 0; i < adjList.size(); i++)
    {
        adjList[i].sort();
    }
}

double MeshGraph::AverageDistanceBetweenVertices() const
{
    double totalDistance = 0;
    for (unsigned i = 0; i < adjList.size(); i++)
    {
        for (std::list<Vertex*>::const_iterator j = adjList[i].begin(); j != adjList[i].end(); j++)
        {
            totalDistance += Double3::Distance(vertices[i].position3D,(*j)->position3D);
        }
    }
    return totalDistance/(2*TotalEdgeCount());
}

double MeshGraph::AverageEdgePerVertex() const
{
    return (double) TotalEdgeCount()/TotalVertexCount();
}

int MeshGraph::TotalVertexCount() const
{
    return (int) vertices.size();
}

int MeshGraph::TotalEdgeCount() const
{
    int edgeCount = 0;
    for (unsigned i = 0; i < adjList.size(); i++)
    {
        for (std::list<Vertex*>::const_iterator j = adjList[i].begin(); j != adjList[i].end(); j++)
        {
            edgeCount++;
        }
    }
    return edgeCount/2;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    if (vertices.size()-1 < vertexId)
    {
        return -1;
    }
    int vertexEdgeCount = 0;
    for (std::list<Vertex*>::const_iterator i = adjList[(unsigned) vertexId].begin(); i != adjList[(unsigned) vertexId].end(); i++)
    {
        vertexEdgeCount++;
    }
    return vertexEdgeCount;
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,
                                    int vertexId) const
{
    if (vertices.size()-1 < vertexId)
    {
        outVertexIds.resize(0);
        return;
    }
    outVertexIds.resize((unsigned) VertexEdgeCount(vertexId));
    unsigned index = 0;
    for (std::list<Vertex*>::const_iterator i = adjList[(unsigned) vertexId].begin(); i != adjList[(unsigned) vertexId].end(); i++)
    {
        outVertexIds[index] = (*i)->id;
        index++;
    }
    return;
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    if (vertices.size()-1 < vertexIdFrom || vertices.size()-1 < vertexIdTo)
    {
        outputColorAllVertex.resize(0);
        return;
    }
    outputColorAllVertex.resize(vertices.size());
    for (unsigned i = 0; i < vertices.size(); i++)
    {
        outputColorAllVertex[i].r = outputColorAllVertex[i].g = outputColorAllVertex[i].b = 0;
    }
    if (vertexIdTo == vertexIdFrom)
    {
        outputColorAllVertex[(unsigned) vertexIdFrom].r = color.r;
        outputColorAllVertex[(unsigned) vertexIdFrom].g = color.g;
        outputColorAllVertex[(unsigned) vertexIdFrom].b = color.b;
        return;
    }
    std::vector<double> distance(vertices.size(),INFINITY);
    std::vector<int> previous(vertices.size(),-1);
    ShortestPath(vertexIdFrom, distance, previous);
    if (previous[(unsigned) vertexIdTo] == -1)
    {
        outputColorAllVertex.resize(0);
        return;
    }
    int currentVertexId = vertexIdTo;
    while (currentVertexId != -1)
    {
        outputColorAllVertex[(unsigned ) currentVertexId].r = color.r;
        outputColorAllVertex[(unsigned ) currentVertexId].g = color.g;
        outputColorAllVertex[(unsigned ) currentVertexId].b = color.b;
        currentVertexId = previous[(unsigned) currentVertexId];
    }
    return;
}

void MeshGraph::ShortestPath(int vertexIdStart,std::vector<double> &distance, std::vector<int> &previous) const
{
    BinaryHeap Heap;
    distance[(unsigned) vertexIdStart] = 0;
    for (unsigned i = 0; i < vertices.size(); i++)
    {
        Heap.Add(vertices[i].id,INFINITY);
    }
    Heap.ChangePriority(vertexIdStart,0);
    int currentId;
    double currentDistance;
    std::vector<int> neighbours;
    while(Heap.HeapSize())
    {
        Heap.PopHeap(currentId,currentDistance);
        int neighbourCount = VertexEdgeCount(currentId);
        ImmediateNeighbours(neighbours,currentId);
        for (unsigned i = 0; i < neighbourCount; i++)
        {
            int neighbourId = neighbours[i];
            double distanceToNeighbour = distance[(unsigned) currentId] + Double3::Distance(vertices[(unsigned) currentId].position3D,vertices[(unsigned) neighbourId].position3D);
            if (distanceToNeighbour < distance[(unsigned) neighbourId])
            {
                distance[(unsigned) neighbourId] = distanceToNeighbour;
                previous[(unsigned) neighbourId] = currentId;
                Heap.ChangePriority(neighbourId,distanceToNeighbour);
            }
        }
    }
}

double MeshGraph::FilterFunction(double alpha,double distance, FilterType type) const
{
    if (type == FILTER_GAUSSIAN)
    {
        return std::exp(-std::pow(distance,2)/std::pow(alpha,2));
    }
    return -alpha <= distance && distance <= alpha;
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    if (vertices.size()-1 < vertexId)
    {
        outputColorAllVertex.resize(0);
        return;
    }
    outputColorAllVertex.resize(vertices.size());
    for (unsigned i = 0; i < vertices.size(); i++)
    {
        outputColorAllVertex[i].r = outputColorAllVertex[i].g = outputColorAllVertex[i].b = 0;
    }
    int order = 0;
    std::vector<bool> mark(vertices.size(), false);
    std::vector<double> distance(vertices.size(), INFINITY);
    std::vector<int> depth(vertices.size(),0);
    int currentId;
    double currentDistance;
    std::vector<int> neighbours;
    BinaryHeap Heap;
    Heap.Add(vertexId,order++);
    mark[(unsigned) vertexId] = true;
    distance[(unsigned) vertexId] = 0;
    while (Heap.HeapSize())
    {
        Heap.PopHeap(currentId,currentDistance);
        outputColorAllVertex[(unsigned) currentId].r =(unsigned char) (color.r * FilterFunction(alpha,distance[(unsigned) currentId],type));
        outputColorAllVertex[(unsigned) currentId].g =(unsigned char) (color.g * FilterFunction(alpha,distance[(unsigned) currentId],type));
        outputColorAllVertex[(unsigned) currentId].b =(unsigned char) (color.b * FilterFunction(alpha,distance[(unsigned) currentId],type));
        if (depth[(unsigned) currentId] < maxDepth)
        {
            int neighbourCount = VertexEdgeCount(currentId);
            ImmediateNeighbours(neighbours,currentId);
            for (unsigned i = 0; i < neighbourCount; i++)
            {
                int neighbourId = neighbours[i];
                if (mark[(unsigned) neighbourId])
                {
                    continue;
                }
                mark[(unsigned) neighbourId] = true;
                distance[(unsigned) neighbourId] = distance[(unsigned) currentId] + Double3::Distance(vertices[(unsigned) currentId].position3D,vertices[(unsigned) neighbourId].position3D);
                depth[(unsigned) neighbourId] = depth[(unsigned) currentId] + 1;
                Heap.Add(neighbourId,order++);
            }
        }
    }
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    if (vertices.size()-1 < vertexId)
    {
        outputColorAllVertex.resize(0);
        return;
    }
    outputColorAllVertex.resize(vertices.size());
    for (unsigned i = 0; i < vertices.size(); i++)
    {
        outputColorAllVertex[i].r = outputColorAllVertex[i].g = outputColorAllVertex[i].b = 0;
    }
    int order = 0;
    std::vector<bool> mark(vertices.size(), false);
    std::vector<int> depth(vertices.size(),0);
    int currentId;
    double currentDistance;
    std::vector<int> neighbours;
    BinaryHeap Heap;
    Heap.Add(vertexId,order++);
    mark[(unsigned) vertexId] = true;
    while (Heap.HeapSize())
    {
        Heap.PopHeap(currentId,currentDistance);
        double distance = Double3::Distance(vertices[(unsigned) currentId].position3D,vertices[(unsigned) vertexId].position3D);
        outputColorAllVertex[(unsigned) currentId].r =(unsigned char) (color.r * FilterFunction(alpha,distance,type));
        outputColorAllVertex[(unsigned) currentId].g =(unsigned char) (color.g * FilterFunction(alpha,distance,type));
        outputColorAllVertex[(unsigned) currentId].b =(unsigned char) (color.b * FilterFunction(alpha,distance,type));
        if (depth[(unsigned) currentId] < maxDepth)
        {
            int neighbourCount = VertexEdgeCount(currentId);
            ImmediateNeighbours(neighbours,currentId);
            for (unsigned i = 0; i < neighbourCount; i++)
            {
                int neighbourId = neighbours[i];
                if (mark[(unsigned) neighbourId])
                {
                    continue;
                }
                mark[(unsigned) neighbourId] = true;
                depth[(unsigned) neighbourId] = depth[(unsigned) currentId] + 1;
                Heap.Add(neighbourId,order++);
            }
        }
    }
}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}
