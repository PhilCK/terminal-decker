
#include <Caffeine/Common/Utilities/Param.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <Caffeine/Math/Math.hpp>

#include <tinyxml2/tinyxml2.h>

#include <functional>
#include <algorithm>
#include <vector>
#include <sstream>


namespace
{
	// Using tinyxml2 creates an param tree.
	void LoadXML(const std::string &filename, CaffUtil::Param &paramNode)
	{
        tinyxml2::XMLDocument xmlDoc;
		xmlDoc.LoadFile(filename.c_str());

		if(!xmlDoc.Error())
		{
            tinyxml2::XMLElement *rootElement = xmlDoc.RootElement();
            
            if(rootElement)
            {
				// Depth first search.
                std::function<void(tinyxml2::XMLElement*, CaffUtil::Param&)> ProcessElement = [&](tinyxml2::XMLElement *element, CaffUtil::Param &parent)
                {
                    assert(element);
                    
                    const std::string	 elementName	= element->Name();
					CaffUtil::Param		 &currentNode   = parent.push(elementName);
					tinyxml2::XMLElement *searchElement = element->FirstChildElement();

					// Process children.
                    while(searchElement != nullptr)
                    {
						assert(searchElement);
						
						ProcessElement(searchElement, currentNode);
						
						searchElement = searchElement->NextSiblingElement();
                    }
                    
                    // Process value.
                    if(element->GetText())
                    {
						const std::string value = element->GetText();
						currentNode = value;
                    }
                };
                
				// Get root node.
                ProcessElement(rootElement, paramNode);
            }
		}
		else
		{
			std::string errorMsg1;
			
			if(xmlDoc.GetErrorStr1())
			{
					errorMsg1 = xmlDoc.GetErrorStr1();
			}
		
			CaffUtil::LogWarning("Failed to open xml: " + filename + ", " + errorMsg1);
		}
	}
	
	// Helper method to name the root param
	std::string GetFileTypeString(const CaffUtil::Param::FileType type)
	{
		switch(type)
		{
			case CaffUtil::Param::FileType::XML:
				return "XML";
				break;
			
			default:
				return "FILE";
		}
		
		return "Unkown"; // this is error really.
	}
}



namespace Caffeine {
namespace Utilities {



Param::Param(const std::string &name)
: Param(nullptr, name)
{
}



Param::Param(const Param *parent, const std::string &name)
: m_childNodes()
, m_parentNode(parent)
, m_name(name)
, m_data("")
{
}



Param::Param(const std::string &fileName, FileType fileType)
: Param(nullptr, GetFileTypeString(fileType))
{
    LoadXML(fileName, *this);
}

    

Param::Param(const Param &other)
: m_childNodes()
, m_parentNode(nullptr)
, m_name(other.getName())
, m_data(other.asStdString(""))
{
    // Loop through other and recreate the param tree (Depth first)
    std::function<void(Param&, const Param&)> copyParam = [&](Param &parent, const Param &oParam)
    {
		for(const auto &child : oParam.m_childNodes)
		{
			Param& newNode = parent.push(child.second.get()->getName());
			
			copyParam(newNode, *child.second.get());
		}
		
		parent.m_data = oParam.asStdString();
		parent.m_name = oParam.getName();
    };
    
    copyParam(*this, other);
}


    
Param& Param::operator=(const Param &other)
{
    this->m_childNodes.clear();
    
    // Loop through other and recreate the param tree (Depth first)
    std::function<void(Param&, const Param&)> copyParam = [&](Param &parent, const Param &oParam)
    {
		for(const auto &child : oParam.m_childNodes)
		{
			Param& newNode = parent.push(child.second.get()->getName());
			
			copyParam(newNode, *child.second.get());
		}
		
		parent.m_data = oParam.asStdString();
		parent.m_name = oParam.getName();
    };
    
    copyParam(*this, other);
    
    return *this;
}
    
    

namespace
{
	// Helper method to insert nodes.
	void CreateEmptyNode(std::multimap<std::string, std::unique_ptr<Param> > &container, const Caffeine::Utilities::Param *root, const std::string &str)
	{	
		std::unique_ptr<Param> newNode(new Caffeine::Utilities::Param(root, str));
		container.insert(std::pair<std::string, std::unique_ptr<Param> >(str, std::move(newNode)));
	}
}



const Param& Param::getNode(const std::string &str) const
{
    if(m_childNodes.count(str) > 0)
    {
        return *m_childNodes.find(str)->second.get();
    }

	// If node found.
	CreateEmptyNode(m_childNodes, this, str);

	return getNode(str);
}



Param& Param::getNode(const std::string &str)
{
	return const_cast<Param&>(
			static_cast<const Param&>(*this).getNode(str)
		);
}
    


Param& Param::push(const std::string &str)
{
    const std::size_t numberOfMembers = this->numberOfMembersOfName(str);

	return (*this)[str][numberOfMembers];
}



Param& Param::addNode(const Param &param)
{
	std::unique_ptr<Param> copy(new Caffeine::Utilities::Param(param));
	
	auto it = m_childNodes.insert(std::pair<std::string, std::unique_ptr<Param> >(copy->getName(), std::move(copy)));
	
	return *(it->second.get());
}



const Param& Param::getIndex(const std::size_t index) const
{
	if(m_parentNode)
	{
		// Find the  element of the parent map for your string.
		const std::size_t numOfParams = m_parentNode->m_childNodes.count(m_name);
        
		if(index >= numOfParams)
		{
			// Create empty nodes
			for(std::size_t i = 0; i <= (index - numOfParams); ++i)
			{
				CreateEmptyNode(m_parentNode->m_childNodes, m_parentNode, m_name);
			}
		}

		std::pair< std::map<std::string, std::unique_ptr<Param> >::iterator, std::map<std::string, std::unique_ptr<Param> >::iterator > itRange = m_parentNode->m_childNodes.equal_range(m_name);

		auto it = itRange.first;

		for(std::size_t i = 0; i <= index; ++i)
		{
			if(i < index)
			{
				++it;
			}
			else
			{
				return *(*it).second.get();
			}
		}
		
		assert(false); // this is a big fail.
	}

	assert(false); // Need a root parent.
	return *this;  // Just for warning this should error.
}



Param& Param::getIndex(const std::size_t index)
{
	return const_cast<Param&>(
			static_cast<const Param&>(*this).getIndex(index)
		);
}



std::string Param::getDataAsTree() const
{
	std::stringstream ss;
	
	std::function<void(const CaffUtil::Param&, const uint32_t)> GetTree = [&](const CaffUtil::Param &node, const uint32_t level)
	{
		for(std::size_t t = 0; t < level; ++t)
		{
			ss << "   ";
		}
		
		ss << "[" << node.getName() << "] " << node.asStdString() << "\n";
		
		for(auto &child : node.m_childNodes)
		{
			GetTree(*child.second.get(), level + 1);
		}
	};
		
	GetTree(*this, 0);

	return ss.str();
}



std::array<float, 2> Param::asFloat2(const std::array<float, 2> &defaultData) const
{
	std::array<float, 4> resultArr = asFloat4({{defaultData.at(0), defaultData.at(1), 0.f, 0.f}});
	std::array<float, 2> returnArr {{ resultArr.at(0), resultArr.at(1) }};

	return returnArr;
}



std::array<float, 3> Param::asFloat3(const std::array<float, 3> &defaultData) const
{
	std::array<float, 4> resultArr = asFloat4({{defaultData.at(0), defaultData.at(1), defaultData.at(2), 0.f}});
	std::array<float, 3> returnArr {{ resultArr.at(0), resultArr.at(1), resultArr.at(2) }};

	return returnArr;
}



std::array<float, 4> Param::asFloat4(const std::array<float, 4> &defaultData) const
{
	if(!m_data.empty())
	{
		std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(m_data);

		std::array<float, 4> returnArr = {{0.f, 0.f, 0.f, 0.f}};

		for(std::size_t i = 0; i < CaffMath::Min<std::size_t>(4, tokens.size()); ++i)
		{
			returnArr.at(i) = std::stof(tokens.at(i));
		}

		return returnArr;
	}

	return defaultData;
}



bool Param::asBool(const bool defaultData) const
{
    const std::string currentData = CaffUtil::ToLower(m_data);
    
    if(currentData == "true")
    {
        return true;
    }
    else if(currentData == "false")
    {
        return false;
    }
    
    return defaultData;
}



std::ostream & operator<<(std::ostream &ostr, const Param &param)
{
	return ostr << param.asStdString();
}



} // namespace
} // namespace
