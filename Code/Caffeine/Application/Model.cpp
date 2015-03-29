
/*
	Model.cpp
	---------
	
	Project: Caffeine
*/

#include <Caffeine/Application/Model.hpp>
#include <sstream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <array>
#include <string>


namespace {


#pragma mark - Helpers

enum class VertexType : char
{
	POSITION,
	UV,
	NORMAL,
};

std::size_t VertexElements[] = {
	3,
	2,
	3,
};

std::size_t VertexSize[] = {
	sizeof(float) * VertexElements[0],
	sizeof(float) * VertexElements[1],
	sizeof(float) * VertexElements[2],
};



# pragma mark - Model Loaders

    bool LoadOBJ(const std::string& filename, std::vector<CaffApp::Mesh> & meshes, CaffApp::AABB &bounds, const bool isGL)
{
	std::ifstream file(filename);
	
	if(file.is_open())
	{
		// Mesh Details
		std::string name = "";
		CaffApp::VertexContainer verts;
		CaffApp::VertexContainer normals;
		CaffApp::VertexContainer uvs;
		CaffApp::IndexContainer  index;
	
		std::string line;
		
		// Loop through contents of the file.
		while(getline(file, line))
		{
			// Split line into an array of strings
			std::istringstream ss(line);
			std::istream_iterator<std::string> begin(ss), end;
			std::vector<std::string>objTokens(begin, end);
			
			std::string type = objTokens[0];
			
			// In Reality the v, vn, vt, and f would happen in order so checking each line for all three
			// is uneccassary, however it makes it a little eaiser to look at, and gives some resiliants
			// to mal-formed .obj files.
			
			// New Object
			if(type == "o")
			{
				//If an additional mesh store previous one.
				if(!index.empty())
				{
					CaffApp::Mesh mesh;
					mesh.setName(name);
					mesh.setVertices(verts);
					mesh.setNormals(normals);
					mesh.setUVs(uvs);
					mesh.setFaceIndex(index);
					
					// Insert into models meshes.
					meshes.push_back(mesh);
				}
			
				name = "";
				
				for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
				{
					name += (*it) + " ";
				}
				
				// Trim last space off.
				name = name.substr(0, name.size() - 1);
			}
			
			// Vertices
			else if(type == "v")
			{
                for(std::size_t i = 1; i < objTokens.size(); ++i)
                {
                    const float value = std::stof((objTokens.at(i)));
                    
                    verts.push_back(value);
                    
                    const uint32_t index = i - 1;

                    if(value > 0)
                    {
                      if (CaffMath::Vector3At(bounds.max, index) < value)
                        {
                            CaffMath::Vector3SetAt(bounds.max, value, index);
                        }
                    }
                    else
                    {
                      if (CaffMath::Vector3At(bounds.min, index) > value)
                        {
                            CaffMath::Vector3SetAt(bounds.min, value, index);
                        }
                    }
                }
                
				//for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
				//{
				//	verts.push_back(::atof((*it).c_str()));
				//}
			}
			
			// Normals
			else if(type == "vn")
			{
				for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
				{
					normals.push_back(std::stof((*it)));
				}
			}
			
			// Texture
			else if(type == "vt")
			{
				for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
				{
					uvs.push_back(std::stof((*it)));
					
					// Adjust the V Coord on OpenGL
					if(isGL && !(uvs.size() % 2))
					{
						uvs.back() = 1.0f - uvs.back();
					}
				}
			}
			
			// Faces / Index
			else if(type == "f")
			{
				// Check to see if there are any empty lists, and add a dummy element to them.
				if(verts.empty())	{ verts.push_back(0.0f);   verts.push_back(0.0f);   verts.push_back(0.0f);	 }
				if(normals.empty()) { normals.push_back(0.0f); normals.push_back(0.0f); normals.push_back(0.0f); }
				if(uvs.empty())		{ uvs.push_back(0.0f);	   uvs.push_back(0.0f);								 }
			
			
				for(auto it = objTokens.begin() + 1; it != objTokens.end(); ++it)
				{
					const std::string indexStr = (*it);
						  std::size_t startLoc = 0;
		
					while(true)
					{
						// Find the next '/' or end of file
						const std::size_t endLoc = indexStr.find("/", startLoc);
						const std::size_t len	 = (std::min(endLoc, indexStr.length()) - startLoc);
						const std::string iStr	 = indexStr.substr(startLoc, len);

						// Did we get a result? len = 0 will lead to missing info
						if(!iStr.empty())
						{
							const unsigned int i = ::atoi(iStr.c_str()) - 1; // .obj starts at 1, array starts at 0.
							
							index.push_back(i);
						}
						// Missing info
						else
						{
							// If it seems we are missing information.
							// Check to see if it was required.
							// If its not insert some blank data.
							index.push_back(0); // This is the dummy element inserted.
						}
						
						
						// End of file.
						if(endLoc == std::string::npos) {
							break;
						}
						
						startLoc = endLoc + 1;
					}
					
				} // While loop
			} // for index loop
		} // while file
		
		
		// Add mesh to the list of meshes.
		if(!index.empty())
		{
			CaffApp::Mesh mesh;
			mesh.setName(name);
			mesh.setVertices(verts);
			mesh.setNormals(normals);
			mesh.setUVs(uvs);
			mesh.setFaceIndex(index);
			
			// Insert into models meshes.
			meshes.push_back(mesh);
		}
		
		return true;
	}

	// Couldn't open the file
	return false;
}


}


# pragma mark - Model Class

namespace Caffeine {
namespace Application {


#define STD_VECTOR_HINT_NUM_OF_MESH_IN_MODEL 4


Model::Model()
: m_meshes()
{
	m_meshes.reserve(STD_VECTOR_HINT_NUM_OF_MESH_IN_MODEL);
}


Model::Model(const std::string &filename)
: Model()
{
  loadModel(filename);
}


bool Model::loadModel(const std::string & filename)
{
	return LoadOBJ(filename, m_meshes, m_bounds, true);
}


bool Model::isMesh(const std::string & mesh) const
{
	for(auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if((*it).getName() == mesh)
		{
			return true;
		}
	}
	
	return false;
}


Mesh & Model::getMesh(const std::size_t mesh) {
	return m_meshes.at(mesh);
}


Mesh & Model::getMesh(const std::string & mesh)
{
	Mesh *meshData = nullptr;
	
	for(auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if((*it).getName() == mesh)
		{
			meshData = &(*it);
		}
	}
	
	assert(meshData);
	return *meshData;
}


} // namespace
} // namespace


# pragma mark - Mesh Class

namespace Caffeine {
namespace Application {


Mesh::Mesh()
: m_name("")
, m_windingOrder(WindingOrder::CCW)
, m_vertices()
, m_verticesIndex()
, m_normals()
, m_normalsIndex()
, m_uvs()
, m_uvsIndex()
, m_index()
{}

/*
bool Mesh::loadMesh(const std::string &filename)
{
	return LoadOBJ(filename, m_vertices, m_normals, m_uvs, m_index);
}
*/


VertexContainer Mesh::getGLVertexBuffer(const WindingOrder windingOrder)
{
	m_windingOrder = windingOrder;

	VertexContainer vertContainer;
	
	// Need to build a GL compatable list of vertex information.
	
	// How many elements are in the index
	unsigned int noElements(0);
	if(!m_vertices.empty()) { noElements++; }
	if(!m_uvs.empty())		{ noElements++; }
	if(!m_normals.empty())	{ noElements++; }
	
	
	std::size_t indexOffset = 0;
	
	
	//for(std::size_t v(0); v < m_vertices.size(); ++v)
	while(indexOffset < m_index.size())
	{
		// Get Vertex.
		if(!m_vertices.empty())
		{
			std::size_t vertPos = m_index.at(indexOffset) * (VertexElements[(std::size_t)VertexType::POSITION]);
		
			for(std::size_t i(0); i < VertexElements[(std::size_t)VertexType::POSITION]; ++i)
			{
				vertContainer.push_back(m_vertices.at(vertPos + i));
			}
			
			++indexOffset;
		}
		
		// Get UV.
		if(!m_uvs.empty())
		{
			std::size_t uvsPos = m_index.at(indexOffset) * (VertexElements[(std::size_t)VertexType::UV]);
		
			for(std::size_t i(0); i < VertexElements[(std::size_t)VertexType::UV]; ++i)
			{
				vertContainer.push_back(m_uvs.at(uvsPos + i));
			}
			
			++indexOffset;
		}
		
		// Get Normal.
		if(!m_normals.empty())
		{
			std::size_t normalPos = m_index.at(indexOffset) * (VertexElements[(std::size_t)VertexType::NORMAL]);
		
			for(std::size_t i(0); i < VertexElements[(std::size_t)VertexType::NORMAL]; ++i)
			{
				vertContainer.push_back(m_normals.at(normalPos + i));
			}
			
			++indexOffset;
		}
	}
	
	
	// Winding Order
	// TODO: Swizzle order
	// FIXME: Move this into the importer.
	if(m_windingOrder == WindingOrder::CCW)
	{
		std::size_t noVerts = vertContainer.size();
		
		for(std::size_t v(0); v < noVerts; ++v)
		{
			
		}
	}
	
	
	m_glVertexBuffer = vertContainer;
	
	return m_glVertexBuffer;
}


IndexContainer Mesh::getGLIndexBuffer()
{
	// TODO: Calc index buffer.
	IndexContainer indexContainer;
	
	return indexContainer;
}


} // namespace
} // namespace
