
#ifndef CAFFEINE_UTILITIES_PARAM_INCLUDED
#define CAFFEINE_UTILITIES_PARAM_INCLUDED


#include <Caffeine/Common/Platform.hpp>
#include <Caffeine/Common/Utilities/StringHelpers.hpp>
#include <map>
#include <string>
#include <array>
#include <memory>
#include <ostream>


namespace Caffeine {
namespace Utilities {


/*
	Param
	--
	A tree like structure for passing KV information in a tree.
	All data is held internally as a string, and cast to the various outputs as required.

	Supports XML import but only a subset of XML, attributes are ignored.

	TODO
	-- 
	- JSON/YAML support cos its sooo much nicer to read.
	- XML(JSON/YAML) export out would be nice.
*/
class Param final
{
public:

	enum class FileType
	{
		XML,
		//JSON, // The JSON/YAML lib was being a pain, would like to re-add these.
		//YAML,
	};

	// Ctor
	explicit 					Param(const std::string &name = "ROOT");
	explicit 					Param(const Param *parent, const std::string &name);
	explicit 					Param(const std::string &filename, FileType fileType);
    
    // Copy
                                Param(const Param &other);
    Param&                      operator=(const Param &other);


	// Tree information
	/*
		This is general information about the node that you can use to help.
		getDataAsTree() will give you a string of the layout of the tree.
	*/
	inline std::string 			getName() const                                     { return m_name; }
	inline bool					isRoot() const                                      { return !m_parentNode; }
	inline bool					doesMemberExist(const std::string &str) const       { return m_childNodes.count(str) > 0; }
    inline std::size_t          numberOfMembersOfName(const std::string &str) const { return m_childNodes.count(str); }
	inline std::size_t 			numberOfMembers() const                             { return m_childNodes.size(); }
	std::string					getDataAsTree() const;

	
	// Tree Access
	/*
		This allows for tree access.

		example
		--
		Params params;
		params["node_name"];
		params.getNode("node_name");
	
		// if more than one node with same name exists you can treat as an array.
		params["node_name"][0];
		params.getNode("node_name").getIndex(1);
	*/
	const Param& 				getNode(const std::string &nodeName) const;
	inline const Param& 		operator[](const std::string &nodeName) const       { return getNode(nodeName); }
	inline const Param& 		operator[](const char *nodeName) const       		{ return getNode(nodeName); }

	Param& 						getNode(const std::string &nodeName);
	inline Param& 				operator[](const std::string &nodeName)             { return getNode(nodeName); }
	inline Param& 				operator[](const char *nodeName)             		{ return getNode(nodeName); }

	const Param& 				getIndex(const std::size_t index) const;
	inline const Param& 		operator[](const std::size_t index) const           { return getIndex(index); }
	inline const Param& 		operator[](const int index) const       		    { return getIndex(index); }

	Param& 						getIndex(const std::size_t index);
	inline Param& 				operator[](const std::size_t index)                 { return getIndex(index); }
	inline Param& 				operator[](const int index)                 		{ return getIndex(index); }
	

	// Tree modification
	/*
		This allows you to add and set nodes.

		example
		--
		Param params;
		params.push("foo"); // This results in params["foo"] existing.
		params.push("foo"); // If done twice this will result in params["foo"][0] and params["foo"][1] existing.

		Param otherParams;
		otherParams.push("bar");
		
		params.addNode(otherParams); // Will attach the two params at that point.

		// To asign a value you can use the = operator
		params["foo"][0] = 1.34f;
		params["foo"][1] = "hello";
	*/
	Param&                      push(const std::string &nodeName);
	Param&						addNode(const Param &param);

	inline void 				operator=(const float data) 					{ operator=(std::to_string(data)); }
	inline void 				operator=(const std::array<float, 2> &data)		{ operator=(std::to_string(data.at(0)) + " " + std::to_string(data.at(1))); }
	inline void 				operator=(const std::array<float, 3> &data)		{ operator=(std::to_string(data.at(0)) + " " + std::to_string(data.at(1)) + " " + std::to_string(data.at(2))); }
	inline void 				operator=(const std::array<float, 4> &data)		{ operator=(std::to_string(data.at(0)) + " " + std::to_string(data.at(1)) + " " + std::to_string(data.at(2)) + " " + std::to_string(data.at(3))); }
	inline void 				operator=(const double data)					{ operator=(std::to_string(data)); }
	inline void 				operator=(const uint32_t data)					{ operator=(std::to_string(data)); }
	inline void 				operator=(const int32_t data)					{ operator=(std::to_string(data)); }
	inline void 				operator=(const uint64_t data)					{ operator=(std::to_string(data)); }
	inline void 				operator=(const int64_t data)					{ operator=(std::to_string(data)); }
	inline void 				operator=(const bool data) 						{ data ? operator=("true") : operator=("false"); }
    inline void                 operator=(const char * data)                    { std::string str(data); operator=(str); }
	inline void 				operator=(const std::string &data)				{ m_data = data; }


	// Tree Data
	/*
		These allow you to get access to the data, you can also provide a value if
		no data is found in the tree.

		example
		--
		Param params;
		params["has_value"]    = "foobar";
		std::string hasValue   = params["has_value"].asStdString("foo");
		std::string hasNoValue = params["has_no_value"].asStdString("foo");

		hasValue;   // This contains "foobar"
		hasNoValue; // This contains "foo"

		// asFloat2(), asFloat3(), asFloat4().
		params["numbers"] = "0 1 2 3 4 5"; // can be converted to asFloatX(), but any data passed will be chopped off. so asFloat2 will result in just 0 and 1 returned.
		params["numbers"] = "10"; // can also be converted to asFloatX(), but any spaces missing will be converted to zero.
	*/	
	typedef std::array<float, 2> Float2; // Typedefs required for casting operators
	typedef std::array<float, 3> Float3;
	typedef std::array<float, 4> Float4;

	Float2						asFloat2(const Float2 &defaultData = {{0.f, 0.f}}) const;
	Float3 						asFloat3(const Float3 &defaultData = {{0.f, 0.f, 0.f}}) const;
	Float4 						asFloat4(const Float4 &defaultData = {{0.f, 0.f, 0.f, 0.f}}) const;
    bool                        asBool(const bool defaultData = false) const;
	inline float 				asFloat(const float defaultData = 0.f) const				{ return m_data.empty() ? defaultData : std::stof(m_data);	}
	inline double 				asDouble(const double defaultData = 0.0) const				{ return m_data.empty() ? defaultData : std::stod(m_data);	}
	inline uint32_t				asUInt32(const uint32_t defaultData = 0) const				{ return m_data.empty() ? defaultData : std::stol(m_data);  }
	inline int32_t				asInt32(const int32_t defaultData = 0) const				{ return m_data.empty() ? defaultData : std::stoi(m_data);	}
	inline uint64_t 			asUInt64(const uint64_t defaultData = 0) const				{ return m_data.empty() ? defaultData : std::stoul(m_data); }
	inline int64_t 				asInt64(const int64_t defaultData = 0) const 				{ return m_data.empty() ? defaultData : std::stol(m_data);	}
	inline std::string 			asStdString(const std::string &defaultData = "") const 		{ return m_data.empty() ? defaultData : m_data;				}


	// Casting
	/*
		These are implicit casts to make for some less verbose code.

		example
		--
		Param params;
		params["some_float"] = 1.3f;
		float value = params["some_float"];
	*/
	inline operator 			Float2() const 			{ return asFloat2(); 	}
	inline operator 			Float3() const 			{ return asFloat3(); 	}
	inline operator 			Float4() const 			{ return asFloat4(); 	}
	inline operator 			bool() const 			{ return asBool(); 		}
	inline operator 			float() const 			{ return asFloat(); 	}
	inline operator 			double() const 			{ return asDouble(); 	}
	inline operator 			uint32_t() const 		{ return asUInt32(); 	}
	inline operator 			int32_t() const 		{ return asInt32(); 	}
	inline operator 			uint64_t() const 		{ return asUInt64(); 	}
	inline operator 			int64_t() const 		{ return asInt64(); 	}	
	inline operator 			std::string() const 	{ return asStdString(); }
	
	
private:

	// Stream operator will just ouput data as string.
	/*
		example
		--
		std::cout << params["some_data"] << std::endl;
	*/
	friend std::ostream & operator << (std::ostream &ostr, const Param &param);

protected:

	mutable std::multimap<std::string, std::unique_ptr<Param> > 	m_childNodes;
	const Param * const												m_parentNode;
	std::string														m_name;
	std::string														m_data;
		
}; // class


} // namespace
} // namespace


#endif // include guard
