#define GLEW_STATIC
#include <GL/glew.h>
#include <map>
#include <string>
#include <fstream>

namespace Meadow {
	class Shader;

	class ShaderSource {
		GLuint vertexId, fragmentId;
	public:
		ShaderSource();
		ShaderSource& vertex(const std::string& path);
		ShaderSource& fragment(const std::string& path);

		operator Shader();
	};

	class Shader {
		GLuint id;
		Shader(GLuint id_);

		friend class ShaderSource;
	public:
		Shader(const Shader& other);

		static ShaderSource make();

		void use();
	};
}

static std::map<std::string, GLuint> vertexShaders;
static std::map<std::string, GLuint> fragmentShaders;

static void readShaderSource(const std::string& path, GLchar *&source, GLint &len) {
	std::ifstream t(path);
	t.seekg(0, std::ios::end);
	size_t size = (size_t)t.tellg();

	source = new GLchar[size + 1];

	t.seekg(0, std::ios::beg);
	t.read(source, size);

	source[t.gcount()] = '\0';

	len = (GLint)size;
}

static GLuint compileShader(const std::string& path, GLenum type) {
	GLuint shader = glCreateShader(type);

	GLchar *source;
	GLint len;
	readShaderSource(path, source, len);

	glShaderSource(shader, 1, &source, &len);
	glCompileShader(shader);

	GLint compiled;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		if (log_length > 1) {
			GLchar *log = new GLchar[log_length];
			glGetShaderInfoLog(shader, log_length, NULL, log);
			//debug << "Error compiling shader:\n";
			//debug << log;
			delete log;
		}
	}

	delete source;

	return shader;
}

namespace Meadow {
	ShaderSource::ShaderSource() : vertexId(0), fragmentId(0) { }

	ShaderSource& ShaderSource::vertex(const std::string& path) {
		auto vertexLoc = vertexShaders.find(path);
		if (vertexLoc == vertexShaders.end()) {
			vertexId = compileShader(path, GL_VERTEX_SHADER);
			vertexShaders[path] = vertexId;
		}
		else vertexId = vertexLoc->second;

		return *this;
	}

	ShaderSource& ShaderSource::fragment(const std::string& path) {
		auto fragmentLoc = fragmentShaders.find(path);
		if (fragmentLoc == fragmentShaders.end()) {
			fragmentId = compileShader(path, GL_FRAGMENT_SHADER);
			fragmentShaders[path] = fragmentId;
		}
		else fragmentId = fragmentLoc->second;

		return *this;
	}

	ShaderSource::operator Shader() {
		GLuint id = glCreateProgram();
		if (vertexId) glAttachShader(id, vertexId);
		if (fragmentId) glAttachShader(id, fragmentId);
		glLinkProgram(id);

		GLint linked;
		glGetShaderiv(id, GL_COMPILE_STATUS, &linked);
		if (!linked) {
			GLint log_length = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);

			if (log_length > 1) {
				GLchar *log = new GLchar[log_length];
				glGetShaderInfoLog(id, log_length, NULL, log);
				//debug << "Error compiling shader:\n";
				//debug << log;
				delete log;
			}
		}

		return Shader(id);
	}

	Shader::Shader(GLuint id_) : id(id_) { }
	Shader::Shader(const Shader& other) : id(other.id) { }

	ShaderSource Shader::make() {
		return ShaderSource();
	}

	void Shader::use() {
		glUseProgram(id);
	}
}