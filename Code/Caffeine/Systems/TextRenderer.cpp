
#include <Caffeine/Systems/TextRenderer.hpp>
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/PropertyManager.hpp>
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Common/Utilities/Directories.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <Caffeine/Components/CameraComponent.hpp>
#include <Caffeine/Math/Math.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <assert.h>
#include <vector>
#include <fstream>
#include <functional>



namespace
{
	// Character vertex stuff.
	GLfloat vertexData[] =
	{
		0.0f,  0.5f, 0.0f,  // Top-left
	};


	GLuint indexData[] =
	{
		0,
	};

	std::array<CaffApp::FormatDesc, 2> vertFmt = {{
		{"inPosition",	CaffApp::AttrFormat::FLOAT, 3},
	}};

	CaffApp::ShaderID		textShaderID;
	CaffApp::VertexID		billboardID;
	CaffApp::IndexID		indexID;
	CaffApp::VertexFormatID vertFmtID;

} // namespace



namespace Caffeine {
namespace Systems {



TextRenderer::TextRenderer(CaffApp::Renderer &renderer, CaffSys::TextureManager &texMgr, CaffSys::ModelManager &modelMgr, CaffSys::CameraManager &camMgr)
: m_renderer(renderer)
, m_textureMgr(texMgr)
, m_modelMgr(modelMgr)
, m_camMgr(camMgr)
, m_fonts()
, m_textToProcess()
{
	const uint32_t bufferReserve(16);
	m_textToProcess.reserve(bufferReserve);

	// Load a default font
	{
		std::ifstream fontDetails(CaffSys::Properties::GetApplicationDir() + "Fonts/dev_font.txt");
		std::string line;
		
		Font font;
		font.maxWidth  = 0;
		font.maxHeight = 0;
		
		while(std::getline(fontDetails, line))
		{
			const std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(line);
			
			enum TokenID { CHAR = 0, ID, X, Y, WIDTH, HEIGHT, XOFFSET, YOFFSET, XADVANCE };
			
			if(!tokens.empty() && tokens.at(CHAR) == "char")
			{
				auto GetValue = [](const std::string &str) -> uint16_t
				{
					return CaffUtil::StringToInt(str.substr(str.find("=") + 1, str.length()));
				};
				
				Font::CharInfo charInfo;
				charInfo.x			= GetValue(tokens.at(X));
				charInfo.y			= GetValue(tokens.at(Y));
				charInfo.width		= GetValue(tokens.at(WIDTH));
				charInfo.height		= GetValue(tokens.at(HEIGHT));
				charInfo.xOffset	= GetValue(tokens.at(XOFFSET));
				charInfo.yOffset	= GetValue(tokens.at(YOFFSET));
				charInfo.xAdvance	= GetValue(tokens.at(XADVANCE));
				
				font.maxWidth  = CaffMath::Max(font.maxWidth,  charInfo.width);
				font.maxHeight = CaffMath::Max(font.maxHeight, charInfo.height);
				
				font.characters[GetValue(tokens.at(ID))] = charInfo;
			}
		}
		
		m_fonts["prop"] = font;
	}

	// Setup Render Stuff
	{
		vertFmtID = m_renderer.addVertexFormat(&vertFmt[0], vertFmt.size());
		assert(vertFmtID);
		
		billboardID = m_renderer.addVertexBuffer(&vertexData[0], sizeof(vertexData), CaffApp::BufferAccess::STATIC);
		assert(billboardID);
		
		indexID = m_renderer.addIndexBuffer(&indexData[0], 6, sizeof(indexData), CaffApp::BufferAccess::STATIC);
		assert(indexID);
		
		textShaderID = m_renderer.addShader(CaffUtil::GetPathDir() + "AppAssets/Shaders/Text.shd");
		assert(textShaderID);
	}
}



void TextRenderer::render2DScreenText(const std::string &text, const CaffMath::Vector2 &position, const std::string &font, const float scale, const bool fixedWidth, const bool processMarkup)
{
	const CaffMath::Vector3 pos = { position.x, position.y, 2.f };

	m_textToProcess.emplace_back();
	TextToRender &renderText = m_textToProcess.back();

	m_textToProcess.back().textToRender = text;
	m_textToProcess.back().renderSpace = TextPosition::SCREEN_SPACE;
	m_textToProcess.back().textType = TextType::TEXT_2D;
	m_textToProcess.back().position = pos;
	m_textToProcess.back().fontName = font;
	m_textToProcess.back().scale = scale;
	m_textToProcess.back().markup = processMarkup;
	m_textToProcess.back().fixedWidth = fixedWidth;
}



void TextRenderer::renderPendingText()
{
	// Uniform names
	const std::string worldMat			= "worldMat";
	const std::string viewMat			= "viewMat";
	const std::string projMat			= "projMat";
	const std::string uniFontMap		= "uniFontMap";
	const std::string uniFontScale		= "uniFontScale";
	const std::string uniMaxScale		= "uniMaxScale";
	const std::string uniAdvance		= "uniAdvance";
	const std::string uniOffset			= "uniOffset";
	const std::string uniFontMapSize	= "uniFontMapSize";
	const std::string uniCharacterSize	= "uniCharacterSize";
	const std::string uniCharacterStart = "uniCharacterStart";

	m_renderer.reset();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_renderer.setShader(textShaderID);
	m_renderer.setVertexFormat(vertFmtID);
	m_renderer.setVertexBuffer(billboardID);
	m_renderer.setIndexBuffer(indexID);

	const CaffComp::CameraComponent &currentCam = m_camMgr.getCurrentCamera();
	const glm::mat4 proj = currentCam.getProjMatrix();
	const glm::mat4 view = currentCam.getViewMatrix();

	m_renderer.setShaderMatrix44f(viewMat, &view[0][0]);
	m_renderer.setShaderMatrix44f(projMat, &proj[0][0]);

	for(auto &text : m_textToProcess)
	{
		// Get/Set the font texture
		{
			const CaffSys::TextureData	*textureData = m_textureMgr.getTextureData("dev_font.png");
			assert(textureData);
			m_renderer.setTexture(uniFontMap, textureData->textureID);

			const glm::vec2 fontMapSize(512.f, 512.f);
			m_renderer.setShader2f(uniFontMapSize, &fontMapSize[0]);
		}		

		// Customise world mat
		{
			// Get the matices
			glm::mat4 world = currentCam.getWorldMatrix();

			if(text.renderSpace == TextPosition::SCREEN_SPACE)
			{
				const glm::vec3 offset(text.position.x, text.position.y, -2.f);
				world = glm::translate(world, offset);
			}
			
			const float fontScale = text.scale * 0.125f;
			world = glm::scale(world, fontScale, fontScale, fontScale);

			m_renderer.setShaderMatrix44f(worldMat, &world[0][0]);
		}
		
		const Font &font = m_fonts.at("prop");

		// Scale
		{
			const glm::vec2 scale(1.f, 1.f);
			m_renderer.setShader2f(uniFontScale, &scale[0]);

			const glm::vec2 maxScale(font.maxWidth, font.maxHeight);
			m_renderer.setShader2f(uniMaxScale, &maxScale[0]);
		}

		glm::vec2 charAdvance;
		charAdvance.x = 0;
		charAdvance.y = 0;
		
		for(std::size_t i = 0; i < text.textToRender.size(); ++i)
		{
			// Current letter we are processing.
			const char current = text.textToRender.at(i);
			
			// If new line move yAdvance
			if(current == '\n')
			{
				charAdvance.y += font.maxHeight;
				charAdvance.x = 0;
				continue;
			}
								
			const uint16_t id = current;
			
			// Catch unknown characters.
			glm::vec2 characterSize;
			glm::vec2 characterStart;
			
			if(font.characters.count(id) > 0)
			{
				const Font::CharInfo &charInfo = font.characters.at(id);

				characterStart.x = charInfo.x;
				characterStart.y = charInfo.y;
				characterSize.x  = charInfo.width;
				characterSize.y  = charInfo.height;
				
				m_renderer.setShader2f(uniAdvance, &charAdvance[0]);
				
				charAdvance.x += text.fixedWidth ? font.maxWidth : charInfo.xAdvance;
				
				const glm::vec2 offset((float)charInfo.xOffset, (float)charInfo.yOffset);
				m_renderer.setShader2f(uniOffset, &offset[0]);
			}
			else
			{
				characterStart.x = 2;
				characterStart.y = 2;
				characterSize.x  = 0;
				characterSize.y  = 0;
				
				m_renderer.setShader2f(uniAdvance, &charAdvance[0]);
				
				const glm::vec2 offset(0.f, 0.f);
				m_renderer.setShader2f(uniOffset, &offset[0]);
			}
			
			m_renderer.setShader2f(uniCharacterSize,	&characterSize[0]);
			m_renderer.setShader2f(uniCharacterStart,	&characterStart[0]);
			
			m_renderer.apply();

			glDrawArrays(GL_POINTS, 0, 1);
		}
	}

	m_textToProcess.clear();
}



} // namespace
} // namespace
