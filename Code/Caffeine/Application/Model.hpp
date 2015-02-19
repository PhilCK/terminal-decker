/*
	Model.hpp
	---------
	
	Project: Caffeine

*/

#ifndef CAFFEINE_APPLICATION_MESH_INCLUDED
#define CAFFEINE_APPLICATION_MESH_INCLUDED

#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Math/Vector.hpp>
#include <vector>
#include <string>

namespace Caffeine {
namespace Application {


typedef std::vector<float> VertexContainer;
typedef std::vector<unsigned int> IndexContainer;


enum class WindingOrder
{
	CW,
	CCW,
};



class Mesh
{
public:

	explicit					Mesh();
	
	bool						loadMesh(const std::string& filename);
	
	inline VertexContainer		getVertices() const						{ return m_vertices;	  }
	inline IndexContainer		getVerticesIndex() const				{ return m_verticesIndex; }
	
  void						setVertices(VertexContainer verts)		{ m_vertices = verts; }
	void						setVerticesIndex(IndexContainer index)	{ m_verticesIndex = index; }
	
	inline VertexContainer		getNormals() const						{ return m_normals;		 }
	inline IndexContainer		getNormalsIndex() const					{ return m_normalsIndex; }
	
	void						setNormals(VertexContainer normals)		{ m_normals = normals; }
	void						setNormalsIndex(IndexContainer index)	{ m_normalsIndex = index; }

	inline VertexContainer		getUVs() const							{ return m_uvs;		 }
	inline IndexContainer		getUVsIndex() const						{ return m_uvsIndex; }
	
	void						setUVs(VertexContainer uvs)				{ m_uvs = uvs;		  }
	void						setUVsIndex(IndexContainer index)		{ m_uvsIndex = index; }
	
	void						setFaceIndex(IndexContainer index)		{ m_index = index; }
	inline IndexContainer		getFaceIndex() const					{ return m_index;  }
	
	inline std::string			getName() const							{ return m_name; }
	void						setName(const std::string& name)		{ m_name = name; }
	
	
	// ** Open GL Specific: as we can't index normals/uvs individually ** //
	VertexContainer 			getGLVertexBuffer(const WindingOrder windingOrder = WindingOrder::CCW);
	IndexContainer				getGLIndexBuffer(); // do not use //
	inline std::size_t			getGLFaces() const			{ return m_glVertexBuffer.size() / 3; } // TODO: NOPE!
	inline std::size_t			getGLVertices() const		{ return m_glVertexBuffer.size();	  }
	
private:

	std::string					m_name;
	WindingOrder				m_windingOrder;

	VertexContainer				m_vertices;
	IndexContainer				m_verticesIndex;
	
	VertexContainer				m_normals;
	IndexContainer				m_normalsIndex;
	
	VertexContainer				m_uvs;
	IndexContainer				m_uvsIndex;
	
	IndexContainer				m_index;
	
	VertexContainer				m_glVertexBuffer;
	IndexContainer				m_glIndexBuffer;

}; // class

    
struct AABB
{
    CaffMath::Vector3 max;
    CaffMath::Vector3 min;
    
    CaffMath::Vector3 getSize() const
    {
      const float32_t x = CaffMath::Abs(x) + CaffMath::Abs(x);
      const float32_t y = CaffMath::Abs(y) + CaffMath::Abs(y);
      const float32_t z = CaffMath::Abs(z) + CaffMath::Abs(z);
      
      const CaffMath::Vector3 size = CaffMath::Vector3Init(x, y, z);
      
      return size;
    }
    
    CaffMath::Vector3 getCenter() const
    {
        const float32_t x = (CaffMath::Vector3GetX(max) + CaffMath::Vector3GetX(min)) * 0.5f;
        const float32_t y = (CaffMath::Vector3GetY(max) + CaffMath::Vector3GetY(min)) * 0.5f;
        const float32_t z = (CaffMath::Vector3GetZ(max) + CaffMath::Vector3GetZ(min)) * 0.5f;
        
        const CaffMath::Vector3 center = CaffMath::Vector3Init(x, y, z);

        return center;
    }
};

class Model
{

	std::vector<Mesh>           m_meshes;
    AABB                        m_bounds;

public:

	explicit					Model();

	bool						loadModel(const std::string & filename);
	
	inline std::size_t			getNumberOfMeshes() const { return m_meshes.size(); }
	bool						isMesh(const std::string & mesh) const;
	
	Mesh &						getMesh(const std::size_t mesh);
	Mesh &						getMesh(const std::string & mesh);
	
	inline AABB                 getBounds() const { return m_bounds; }


};


} // namespace
} // namespace

#endif // include guard
