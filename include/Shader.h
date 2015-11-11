
#ifndef DEF_SHADER
#define DEF_SHADER
#define GLM_FORCE_RADIANS

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>




//Todo : Refactor this class with a better and cleaner solution
class Shader
{
    public:

    Shader();
    Shader(Shader const &origin);
    Shader(std::string vertexSource, std::string fragmentSource);
    ~Shader();

    Shader& operator=(Shader const &origin);

    bool load();
    bool compile(GLuint &shader, GLenum type, std::string const &fileSource);
    const GLuint & getProgramID() const;


    private:

    GLuint m_vertexID;
    GLuint m_fragmentID;
    GLuint m_programID;

    std::string m_vertexSource;
    std::string m_fragmentSource;
};

#endif
