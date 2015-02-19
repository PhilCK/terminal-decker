
#include <Caffeine/Application/Renderer.hpp>
#include <Caffeine/Common/Utilities/Logging.hpp>
#include <SOIL/SOIL.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

namespace {

	// Usageflags for GL
	const GLenum glUsageFlags[] = {
		GL_STATIC_DRAW,
		GL_DYNAMIC_DRAW,
		GL_STREAM_DRAW,
	};
	
	const GLenum glPrimTypes[] = {
		GL_TRIANGLES,
		GL_TRIANGLE_FAN,
		GL_TRIANGLE_STRIP,
		GL_QUADS,
		GL_LINES,
		GL_LINE_STRIP,
		GL_LINE_LOOP,
		GL_POINTS,
	};
	
	const GLsizei glTypes[] = {
		GL_FLOAT,
		0, // not supported.
		GL_UNSIGNED_BYTE,
	};
	
	
	unsigned int constantTypeSizes[] = {
		sizeof(float),
		sizeof(float) * 2,
		sizeof(float) * 3,
		sizeof(float) * 4,
		sizeof(int),
		sizeof(int) * 2,
		sizeof(int) * 3,
		sizeof(int) * 4,
		sizeof(int),
		sizeof(int) * 2,
		sizeof(int) * 3,
		sizeof(int) * 4,
		sizeof(float) * 4,
		sizeof(float) * 9,
		sizeof(float) * 16,
	};
	
	
	Caffeine::Application::ConstantType getGLConstantType(GLenum type)
	{
			switch (type){
				case GL_FLOAT:          return Caffeine::Application::ConstantType::FLOAT;
				case GL_FLOAT_VEC2_ARB: return Caffeine::Application::ConstantType::VEC2;
				case GL_FLOAT_VEC3_ARB: return Caffeine::Application::ConstantType::VEC3;
				case GL_FLOAT_VEC4_ARB: return Caffeine::Application::ConstantType::VEC4;
				case GL_INT:            return Caffeine::Application::ConstantType::INT;
				case GL_INT_VEC2_ARB:   return Caffeine::Application::ConstantType::IVEC2;
				case GL_INT_VEC3_ARB:   return Caffeine::Application::ConstantType::IVEC3;
				case GL_INT_VEC4_ARB:   return Caffeine::Application::ConstantType::IVEC4;
				case GL_BOOL_ARB:       return Caffeine::Application::ConstantType::BOOL;
				case GL_BOOL_VEC2_ARB:  return Caffeine::Application::ConstantType::BVEC2;
				case GL_BOOL_VEC3_ARB:  return Caffeine::Application::ConstantType::BVEC3;
				case GL_BOOL_VEC4_ARB:  return Caffeine::Application::ConstantType::BVEC4;
				case GL_FLOAT_MAT2_ARB: return Caffeine::Application::ConstantType::MAT2;
				case GL_FLOAT_MAT3_ARB: return Caffeine::Application::ConstantType::MAT3;
				case GL_FLOAT_MAT4_ARB: return Caffeine::Application::ConstantType::MAT4;
			}

		return (Caffeine::Application::ConstantType) -1;
	}
	
	int getSamplerUnit(const Caffeine::Application::Shader * shader, const std::string & constant)
	{
		Caffeine::Application::Sampler *samplers = shader->samplers;
		
		int minSampler = 0;
		int maxSampler = shader->nSamplers - 1;
	
			// Do a quick lookup in the sorted table with a binary search
		while (minSampler <= maxSampler)
		{
			int currSampler = (minSampler + maxSampler) >> 1;
			int res = strcmp(constant.c_str(), samplers[currSampler].name);
			
			if (res == 0)
			{
				return samplers[currSampler].index;
			} else if (res > 0){
				minSampler = currSampler + 1;
			} else {
				maxSampler = currSampler - 1;
			}
		}

		return -1;
	}
	
	int samplerComp(const void *s0, const void *s1){
		return strcmp(((Caffeine::Application::Sampler *) s0)->name, ((Caffeine::Application::Sampler *) s1)->name);
	}

	int constantComp(const void *s0, const void *s1){
		return strcmp(((Caffeine::Application::Constant *) s0)->name, ((Caffeine::Application::Constant *) s1)->name);
	}
	
	
	void *uniformFunc[(int)Caffeine::Application::ConstantType::size];
}


namespace Caffeine {
namespace Application {

#pragma mark - Main Renderer Class

typedef GLvoid (APIENTRY *UNIFORM_FUNC)(GLint location, GLsizei count, const void *value);
typedef GLvoid (APIENTRY *UNIFORM_MAT_FUNC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);


Renderer::Renderer(SDL_Window * sdlWindow)
: m_glContext(nullptr)
, m_fboID(GL_ZERO)
, m_vaoID(GL_ZERO)
	
// Containers
, m_shaders()
, m_vertexFormats()
, m_vertexBuffers()
, m_indexBuffers()

// Selections
, m_selectedShader(NO_SHADER)
, m_selectedVertexFormat(NO_VERTEX_FORMAT)
, m_selectedVertexBuffer(NO_VERTEX_BUFFER)
, m_selectedIndexBuffer(NO_INDEX_BUFFER)
, m_selectedTextures()
	
// Other settings
, m_clearColor()
{
	// Setup SDL Context.
	{
		m_glContext = SDL_GL_CreateContext(sdlWindow);
		SDL_ERROR("Renderer::Renderer() - SDL_GL_CreateContext");
		assert(m_glContext);
		
		SDL_GL_MakeCurrent(sdlWindow, m_glContext);
		SDL_ERROR("Renderer::Renderer() - SDL_GL_MakeCurrent");
	}
	
	// Output the version strings to be sure.	
	const std::string glContext = "OpenGL Context: " + std::string((char *)glGetString(GL_VERSION));
	const std::string glsl	    = "GLSL: " + std::string((char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	CaffUtil::LogInfo(glContext);
	CaffUtil::LogInfo(glsl);
  
	#ifdef CAFF_ON_WINDOWS
	// Initialise Glew
	{
		glewExperimental = GL_TRUE;
		const GLenum glewStatus = glewInit();

    GL_ERROR("glewInit()  [Safe to ignore]");

		if (glewStatus != GLEW_OK)
		{
			const std::string glewError = std::string((char*)glewGetErrorString(glewStatus));
			CaffUtil::LogInfo(glewError);
			// Throw?
		}
	}
	#endif

	// Setup Open GL
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	GL_ERROR("Renderer::Renderer()");
	
	// Containers set size.
	
	uniformFunc[(int)Caffeine::Application::ConstantType::FLOAT] = (void *) glUniform1fv;
	uniformFunc[(int)Caffeine::Application::ConstantType::VEC2]  = (void *) glUniform2fv;
	uniformFunc[(int)Caffeine::Application::ConstantType::VEC3]  = (void *) glUniform3fv;
	uniformFunc[(int)Caffeine::Application::ConstantType::VEC4]  = (void *) glUniform4fv;
	uniformFunc[(int)Caffeine::Application::ConstantType::INT]   = (void *) glUniform1iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::IVEC2] = (void *) glUniform2iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::IVEC3] = (void *) glUniform3iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::IVEC4] = (void *) glUniform4iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::BOOL]  = (void *) glUniform1iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::BVEC2] = (void *) glUniform2iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::BVEC3] = (void *) glUniform3iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::BVEC4] = (void *) glUniform4iv;
	uniformFunc[(int)Caffeine::Application::ConstantType::MAT2]  = (void *) glUniformMatrix2fv;
	uniformFunc[(int)Caffeine::Application::ConstantType::MAT3]  = (void *) glUniformMatrix3fv;
	uniformFunc[(int)Caffeine::Application::ConstantType::MAT4]  = (void *) glUniformMatrix4fv;
	
	// default clear color
	setClearColor(0.f, 0.f, 0.f, 1.f);
	
	// Set the defaults.
	reset();
	apply();
}



Renderer::~Renderer()
{
	if(m_vaoID) { glDeleteVertexArrays(1, &m_vaoID); }
	if(m_fboID) { glDeleteFramebuffers(1, &m_fboID); }
	
	// Last error check just incase.
	GL_ERROR("Renderer::~Renderer()");

	// Destroy SDL.
	SDL_GL_DeleteContext(m_glContext);
	SDL_ERROR("Renderer::~Renderer()");
	
	// TODO: Destroy stuff this isn't :(
}



// *** APPLY INTERNAL STATES *** //
#pragma mark - Internal States

void Renderer::reset()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glUseProgram(NO_SHADER);
	
  GL_ERROR("Reseting")
	
	// Zero out selections
	m_selectedShader		= NO_SHADER;
	m_selectedVertexFormat  = NO_VERTEX_FORMAT;
	m_selectedVertexBuffer  = NO_VERTEX_BUFFER;
	m_selectedIndexBuffer   = NO_INDEX_BUFFER;
	
	// zero m_selectedTextures
	memset(&m_selectedTextures[0], NO_TEXTURE, sizeof(m_selectedTextures));

  setClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
}



void Renderer::apply()
{
	Shader *shader = nullptr;

	if (m_selectedShader == NO_SHADER)
	{
		// TODO: Log Error.
		return;
	}
	else
	{
		shader = m_shaders[m_selectedShader - 1];
		assert(shader);
		if (m_selectedShaderDirty)
		{
			glUseProgram(shader->program);
			GL_ERROR("Failed to use program.");
			m_selectedShaderDirty = false;
		}
		
		// Apply uniforms
		for(unsigned int i = 0; i < shader->nConstants; ++i)
		{
			Constant *uniform = shader->constants + i;
			
			if(uniform->dirty)
			{
				if(uniform->type >= ConstantType::MAT2) {
					((UNIFORM_MAT_FUNC) uniformFunc[(int)uniform->type])(uniform->index, uniform->nElements, GL_FALSE, (float *) uniform->data);
				} else {
					((UNIFORM_FUNC) uniformFunc[(int)uniform->type])(uniform->index, uniform->nElements, (float *) uniform->data);
				}
				
				uniform->dirty = false;
			}
		}
	}
	
	// Apply Textures
	if (m_selectedTextureDirty)
	{
		for(unsigned int i = 0; i < MAX_TEXTURES_UNIT; ++i)
		{
			TextureID texID = m_selectedTextures[i];
		
			if(texID != NO_TEXTURE)
			{
				Texture *tex = m_textures[texID - 1];
			
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, tex->tex);
			
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
				GL_ERROR("Failed binding tex.");
			}

			m_selectedTextureDirty = false;
		}
	}

	// Set Vertex Buffer
	if(true)// (m_selectedVertexBufferDirty && m_selectedVertexBuffer != NO_VERTEX_BUFFER)
	{
		VertexBuffer *buffer = m_vertexBuffers[m_selectedVertexBuffer - 1];
		glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
		GL_ERROR("Failed binding vertex buffer.");
		
		if(m_selectedVertexFormat != NO_VERTEX_FORMAT)
		{
			VertexFormat *vertFormat = m_vertexFormats[m_selectedVertexFormat - 1];
			
			const GLint NOT_IN_USE = -1;
			
			if(vertFormat->dirty)
			{
				for(unsigned int i = 0; i < vertFormat->numberOfAttrs; ++i)
				{
					Attribute * attr = &vertFormat->attrs[i];
					
					GLint index = glGetAttribLocation(shader->program, attr->name.c_str());
					
					if(index != NOT_IN_USE)
					{
						attr->index = index;
						
						glEnableVertexAttribArray(attr->index);
						// index is -1 then not active.
						glVertexAttribPointer(attr->index, attr->size, glTypes[(int)attr->type], GL_FALSE, vertFormat->vertexSize, (void *)attr->pointer);
					}
					
					GL_ERROR("Failed applying attributes.");
				}
				// TODO: Keep dirty until shaders implimented
			}
		}

		m_selectedVertexBufferDirty = false;
	}

	// Set Index Buffer
	if (m_selectedIndexBufferDirty && m_selectedIndexBuffer != NO_INDEX_BUFFER)
  {
		IndexBuffer *buffer = m_indexBuffers[m_selectedIndexBuffer - 1];
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->vbo);
		
		GL_ERROR("Failed binding index buffer.");

		m_selectedIndexBufferDirty = false;
	}
}



void Renderer::setViewPort(const unsigned int width, const unsigned int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}



void Renderer::setClearColor(float const r, float const g, float const b, float const a)
{
  m_clearColor[0] = r;
  m_clearColor[1] = g;
  m_clearColor[2] = b;
  m_clearColor[3] = a;
	
  glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
  GL_ERROR("Failed setting clear color.")
}



void Renderer::clearDefault(const bool backBuffer, const bool depthBuffer) const
{
	glClear( (backBuffer ? GL_COLOR_BUFFER_BIT : 0) | (depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) );
}



// *** SHADERS *** //
#pragma mark - Shaders

ShaderID Renderer::addShader(const std::string & filename)
{
	// read in the file.
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	std::string	  contents;

	if(file.is_open())
	{
		file.seekg(0, std::ios::end);
		contents.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&contents[0], contents.size());
		file.close();
	}
	else
	{
		return NO_SHADER;
	}
	
	// Get the shader name, we use the filename.
	std::string shaderName = "";
	{
		const std::size_t lastSlash = filename.rfind("/");
		const std::size_t length    = filename.size() - lastSlash;
		
		shaderName = filename.substr(lastSlash + 1, length - 1);
	}
	
	// Get the vertex shader text
	std::string vertexShader = "";
	{
		const std::string openVertTag  = "[VERTEXSHADER]";
		const std::string closeVertTag = "[/VERTEXSHADER]";
		
		std::size_t vertStart = contents.find(openVertTag);
		std::size_t vertEnd   = contents.find(closeVertTag);
		
		if((vertStart != std::string::npos) && (vertEnd != std::string::npos))
		{
			vertStart += openVertTag.size();
		
			vertexShader = contents.substr(vertStart, vertEnd - vertStart);
		}
	}
	
	// Get the geo shader text
	std::string geometryShader = "";
	{
		const std::string openGeoTag  = "[GEOMETRYSHADER]";
		const std::string closeGeoTag = "[/GEOMETRYSHADER]";
		
		std::size_t geoStart = contents.find(openGeoTag);
		std::size_t geoEnd   = contents.find(closeGeoTag);
		
		if((geoStart != std::string::npos) && (geoEnd != std::string::npos))
		{
			geoStart += openGeoTag.size();
		
			geometryShader = contents.substr(geoStart, geoEnd - geoStart);
		}
	}
	
	// Get the pixel shader text
	std::string pixelShader	= "";
	{
		const std::string openPixTag  = "[PIXELSHADER]";
		const std::string closePixTag = "[/PIXELSHADER]";
		
		std::size_t pixStart = contents.find(openPixTag);
		std::size_t pixEnd   = contents.find(closePixTag);
		
		if((pixStart != std::string::npos) && (pixEnd != std::string::npos))
		{
			pixStart += openPixTag.size();
		
			pixelShader = contents.substr(pixStart, pixEnd - pixStart);
		}
	}
	
	// Add the shader
	return addShader(vertexShader, geometryShader, pixelShader, shaderName);
}



ShaderID Renderer::addShader(const std::string & vertexShader, const std::string & geometryShader, const std::string & pixelShader, const std::string & shaderName)
{
	Shader *shader = new Shader();
	memset(shader, 0, sizeof(*shader));
	
	// Vertex shader
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	{
		const char * vertSource = vertexShader.c_str();
		glShaderSource(vertShaderID, 1, &vertSource, NULL);
		glCompileShader(vertShaderID);
		
		// Log
		GLint logLength;
		glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &logLength);
		
		//GLchar log[logLength];
		std::vector<GLchar> log;
		log.resize(logLength);
		glGetShaderInfoLog(vertShaderID, logLength, 0, &log[0]);
		
		// Check
		GLint isCompiled;
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &isCompiled);
		
		//if(!log.empty())
		if(logLength > 1)
		{
			std::string msgHeader;
			isCompiled == GL_FALSE ? msgHeader = "Error Compiling Vert Shader: " + shaderName : msgHeader = "Warning Compiling Shader: " + shaderName;
			
			DEBUG_LOG(msgHeader)
			DEBUG_LOG(&log[0]);
		}
		
		if(isCompiled == GL_FALSE) {
			return NO_SHADER;
		}
	}
	
	// Geo shader
	GLuint geoShaderID;
	
	if(geometryShader != "")
	{
		geoShaderID = glCreateShader(GL_GEOMETRY_SHADER);
		const char * geoSource = geometryShader.c_str();
		glShaderSource(geoShaderID, 1, &geoSource, NULL);
		glCompileShader(geoShaderID);
		
		// Log
		GLint logLength;
		glGetShaderiv(geoShaderID, GL_INFO_LOG_LENGTH, &logLength);
		
		//GLchar log[logLength];
		std::vector<GLchar> log;
		log.resize(logLength);
		glGetShaderInfoLog(geoShaderID, logLength, 0, &log[0]);
		
		// Check
		GLint isCompiled;
		glGetShaderiv(geoShaderID, GL_COMPILE_STATUS, &isCompiled);
		
		if(logLength > 1)
		{
			std::string msgHeader;
			isCompiled == GL_FALSE ? msgHeader = "Error Compiling Geo Shader: " + shaderName : msgHeader = "Warning Compiling Shader: " + shaderName;
			
			DEBUG_LOG(msgHeader);
			DEBUG_LOG(&log[0]);
		}
		
		if(isCompiled == GL_FALSE) {
			return NO_SHADER;
		}
	}
	
	// Pixel shader
	GLuint pixelShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	{
		const char * pixelSource = pixelShader.c_str();
		glShaderSource(pixelShaderID, 1, &pixelSource, NULL);
		glCompileShader(pixelShaderID);
		
		// Log
		GLint logLength;
		glGetShaderiv(pixelShaderID, GL_INFO_LOG_LENGTH, &logLength);
		
		//GLchar log[logLength];
		std::vector<GLchar> log;
		log.resize(logLength);
		glGetShaderInfoLog(pixelShaderID, logLength, 0, &log[0]);
		
		// Check
		GLint isCompiled;
		glGetShaderiv(pixelShaderID, GL_COMPILE_STATUS, &isCompiled);
		
		//if(!log.empty())
		if(logLength > 1)
		{
			std::string msgHeader;
			isCompiled == GL_FALSE ? msgHeader = "Error Compiling Frag Shader: " + shaderName : msgHeader = "Warning Compiling Shader: " + shaderName;
			
			DEBUG_LOG(msgHeader)
			DEBUG_LOG(&log[0]);
		}
		
		if(isCompiled == GL_FALSE) {
			return NO_SHADER;
		}
	}

	// Link the program
	GLuint progID = glCreateProgram();
	{
		glAttachShader(progID, vertShaderID);
		if(geometryShader != "") { glAttachShader(progID, geoShaderID); }
		glAttachShader(progID, pixelShaderID);
		glLinkProgram(progID);
		
		// Log
		GLint logLength;
		glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &logLength);
		
		//GLchar log[logLength];
		std::vector<GLchar> log;
		log.resize(logLength);
		glGetProgramInfoLog(progID, logLength, 0, &log[0]);
		
		// Check
		GLint isLinked;
		glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);
		
		//if(!log.empty())
		if(logLength > 1)
		{
			std::string msgHeader;
			isLinked == GL_FALSE ? msgHeader = "Error Linking Shader: " + shaderName : msgHeader = "Warning Linking Shader: " + shaderName;
			
			DEBUG_LOG(msgHeader)
			DEBUG_LOG(&log[0]);
		}
		
		if(isLinked == GL_FALSE) {
			return NO_SHADER;
		}
	}
	
	shader->program			= progID;
	shader->vertexShader	= vertShaderID;
	shader->geometryShader  = geometryShader != "" ? geoShaderID : 0;
	shader->fragmentShader  = pixelShaderID;
	
	glUseProgram(shader->program);
				
	// Get uniforms
	GLint uniformCount, uniLength;
	glGetProgramiv(shader->program, GL_ACTIVE_UNIFORMS,			  &uniformCount);
	glGetProgramiv(shader->program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniLength);
	
		
	Sampler  *samplers = (Sampler *)  malloc(uniformCount * sizeof(Sampler));
	Constant *uniforms = (Constant *) malloc(uniformCount * sizeof(Constant));
		
	
	unsigned int nSamplers = 0;
	unsigned int nUniforms = 0;
	char *name = new char[uniLength];
	
	for(GLint i = 0; i < uniformCount; ++i)
	{
		GLenum type;
		GLint length, size;
		
		glGetActiveUniform(shader->program, i, uniLength, &length, &size, &type, name);
		
		if((type >= GL_SAMPLER_1D) && (type <= GL_SAMPLER_2D_RECT_SHADOW))
		{
			GLint loc = glGetUniformLocation(shader->program, name);
			GL_ERROR("Getting uniform location.");
			
			glUniform1i(loc, nSamplers);
									
			samplers[nSamplers].name = new char[length + 1];
			samplers[nSamplers].index = nSamplers;
			strcpy(samplers[nSamplers].name, name);
			nSamplers++;
		}
		else
		{
			if(strncmp(name, "gl_", 3) != 0)
			{
				uniforms[nUniforms].name = new char[length + 1];
				uniforms[nUniforms].index = glGetUniformLocation(shader->program, name);
				uniforms[nUniforms].type = getGLConstantType(type);
				uniforms[nUniforms].nElements = size;
				strcpy(uniforms[nUniforms].name, name);
				nUniforms++;
			}
		}
		
	}
	
	delete[] name;
	
	glUseProgram(shader->program);
	
	// Shorten arrays
	samplers = (Sampler *) realloc(samplers, nSamplers * sizeof(Sampler));
	uniforms = (Constant *) realloc(uniforms, nUniforms * sizeof(Constant));
	
	qsort(samplers, nSamplers, sizeof(Sampler), samplerComp);
	qsort(uniforms, nUniforms, sizeof(Constant), constantComp);
	
	for(unsigned int i = 0; i < nUniforms; ++i)
	{
		int constantSize = constantTypeSizes[(int)uniforms[i].type] * uniforms[i].nElements;
		uniforms[i].data = new unsigned char[constantSize];
		memset(uniforms[i].data, 0, constantSize);
		uniforms[i].dirty = false;
	}
	
	shader->constants  = uniforms;
	shader->samplers   = samplers;
	shader->nConstants = nUniforms;
	shader->nSamplers  = nSamplers;
	
	m_shaders.push_back(shader);
	
	return m_shaders.size();
}


void Renderer::setShaderRaw(const std::string &constant, const float *data, std::size_t size)
{
	Shader *shader = m_shaders[m_selectedShader - 1];

	int minUniform = 0;
	int maxUniform = shader->nConstants - 1;
	
	Constant *uniforms = shader->constants;
	
	while(minUniform <= maxUniform)
	{
		int currUniform = (minUniform + maxUniform) >> 1;
		int res = strcmp(constant.c_str(), uniforms[currUniform].name);
		
		if(res == 0)
		{
			Constant *uni = uniforms + currUniform;
			
			if(memcmp(uni->data, data, size)) {
				memcpy(uni->data, data, size);
				uni->dirty = true;
			}
			return;
		}
		else if (res > 0){
			minUniform = currUniform + 1;
		}
		else {
			maxUniform = currUniform - 1;
		}
	}
}


//**** TEMP ***** //
GLuint Renderer::getShader() { return m_shaders[m_selectedShader - 1]->program; }


// *** VERTEX FORMAT *** //
#pragma mark - Vertex Format

VertexFormatID Renderer::addVertexFormat(const FormatDesc *desc, std::size_t attrs)
{
	VertexFormat *vertFormat = new VertexFormat;
	memset(vertFormat, 0, sizeof(*vertFormat));

	vertFormat->dirty = true;
	vertFormat->numberOfAttrs = attrs;
	
	for(unsigned int i = 0; i < attrs; ++i)
	{
		vertFormat->attrs[i].name    = desc[i].attrName;
		vertFormat->attrs[i].size	 = desc[i].size;
		vertFormat->attrs[i].type	 = desc[i].type;
		vertFormat->attrs[i].pointer = vertFormat->vertexSize;
		
		// Increase offset
		int fmtSize[] = { sizeof(GLfloat), 0, sizeof(unsigned char) }; // Move this to anon namespace or something.
		
		vertFormat->vertexSize += desc[i].size * fmtSize[(int)desc[i].type];
	}

	m_vertexFormats.push_back(vertFormat);

	return m_vertexFormats.size();
};



// *** VERTEX BUFFER *** //
#pragma mark - Vertex Buffer

VertexID Renderer::addVertexBuffer(const void * data, const std::size_t size, const BufferAccess access)
{
	VertexBuffer * vertexBuffer = new VertexBuffer();
	memset(vertexBuffer, 0, sizeof(*vertexBuffer));
	
	vertexBuffer->size = (GLsizei)size;

	glGenBuffers(1, &vertexBuffer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, glUsageFlags[(int)access]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GL_ERROR("Adding vertex buffer.");

	m_vertexBuffers.push_back(vertexBuffer);
	
	return m_vertexBuffers.size();
}


void Renderer::updateDynamicVertexBuffer(const VertexID id, void * updateData, const unsigned int updateSize)
{
	VertexBuffer * vertexBuffer = m_vertexBuffers[id - 1];

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->vbo);
	
	GLvoid *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	
	memcpy(data, updateData, updateSize);
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
	
	
	GL_ERROR("Updating dynamic vertex buffer.");
}

// *** INDEX BUFFER *** //
#pragma mark - Index Buffer

IndexID Renderer::addIndexBuffer(const void * data, const std::size_t indices, const std::size_t size, const BufferAccess access)
{
	IndexBuffer * indexBuffer = new IndexBuffer();
	indexBuffer->indicies     = indices;
	indexBuffer->size         = size;

	//std::size_t bufferSize = indices * size;
	
	glGenBuffers(1, &indexBuffer->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, glUsageFlags[(int)access]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	GL_ERROR("Adding index buffer.");

	m_indexBuffers.push_back(indexBuffer);
	
	return m_indexBuffers.size();
}



// *** TEXTURES *** //
#pragma mark - Textures

TextureID Renderer::addTexture(const std::string & filename)
{
	Texture *texture = new Texture();
	memset(texture, 0, sizeof(*texture));
	
	glGenTextures(1, &texture->tex);
	glBindTexture(GL_TEXTURE_2D, texture->tex);
	
	// Load Image data
	int width, height;
	unsigned char * image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	
	SOIL_free_image_data(image);
	
	texture->width  = width;
	texture->height = height;
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	GL_ERROR("Adding texture.");
	
	m_textures.push_back(texture);
	
	return m_textures.size();
}


TextureID Renderer::addCubeMapTexture(const std::string &xPos, const std::string &xNeg, const std::string &yPos, const std::string &yNeg, const std::string &zPos, const std::string &zNeg)
{
	Texture *texture = new Texture();
	memset(texture, 0, sizeof(*texture));
	
	// Load Image data
	GLuint cubemap = SOIL_load_OGL_cubemap(xPos.c_str(), xNeg.c_str(), yPos.c_str(), yNeg.c_str(), zPos.c_str(), zNeg.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	
	GL_ERROR("Adding texture.");
	
	m_textures.push_back(texture);
	
	return m_textures.size();
}


void Renderer::setTexture(const std::string & constant, const TextureID id)
{
	int uint = getSamplerUnit(m_shaders[m_selectedShader - 1], constant);
	
	if(uint >= 0) {
		m_selectedTextures[uint] = id;
	}

	m_selectedTextureDirty = true;
}



// *** Render Targets *** //

#pragma mark - Render Targets

TextureID Renderer::addRenderTarget(const uint16_t width, const uint16_t height, const uint16_t depth)
{
	Texture *texture = new Texture();
	memset(texture, 0, sizeof(*texture));
	
	texture->target = GL_TEXTURE_2D;
	
	glGenTextures(1, &texture->tex);
	glBindTexture(GL_TEXTURE_2D, texture->target);
	
	GL_ERROR("Binding new RT texture.");
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(texture->tex, 0);
	
	GL_ERROR("Adding render target.");
	
	m_textures.push_back(texture);
	
	return m_textures.size();
}



void Renderer::setRenderTarget(const TextureID id)
{
	reset();
	apply();
	
	if(!m_fboID)
	{
		glGenFramebuffers(1, &m_fboID);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	
	GL_ERROR("Binding framebuffer.");
}


	
// *** DRAW *** //
#pragma mark - Drawing

void Renderer::drawArray(Primitive const prim, int const firstVert, std::size_t numberOfVerts)
{
	glDrawArrays(glPrimTypes[(int)prim], firstVert, (GLsizei)numberOfVerts);
}



} // namespace
} // namespace
