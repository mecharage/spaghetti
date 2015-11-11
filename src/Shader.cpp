#include <stdexcept>
#include "Shader.h"

// Constructeurs et Destructeur

Shader::Shader() : m_vertexID(0), m_fragmentID(0), m_programID(0), m_vertexSource(), m_fragmentSource() {
}

Shader::Shader(Shader const &origin) {
  // Copie des fichiers sources

  m_vertexSource = origin.m_vertexSource;
  m_fragmentSource = origin.m_fragmentSource;


  // Chargement du nouveau shader

  load();
}

Shader::Shader(std::string vertexSource, std::string fragmentSource) : m_vertexID(0), m_fragmentID(0), m_programID(0),
m_vertexSource(vertexSource), m_fragmentSource(fragmentSource) {
}

Shader::~Shader() {
  // Destruction du shader

  glDeleteShader(m_vertexID);
  glDeleteShader(m_fragmentID);
  glDeleteProgram(m_programID);
}



Shader& Shader::operator=(Shader const &origin) {
  m_vertexSource = origin.m_vertexSource;
  m_fragmentSource = origin.m_fragmentSource;

  load();

  return *this;
}

bool Shader::load() {
  // Destruction d'un �ventuel ancien Shader

  if (glIsShader(m_vertexID) == GL_TRUE)
	glDeleteShader(m_vertexID);

  if (glIsShader(m_fragmentID) == GL_TRUE)
	glDeleteShader(m_fragmentID);

  if (glIsProgram(m_programID) == GL_TRUE)
	glDeleteProgram(m_programID);


  if (!compile(m_vertexID, GL_VERTEX_SHADER, m_vertexSource))
	throw std::runtime_error("Unable to compile the vertex shader");

  if (!compile(m_fragmentID, GL_FRAGMENT_SHADER, m_fragmentSource))
    throw std::runtime_error("Unable to compile the fragment shader");


  m_programID = glCreateProgram();

  if(!m_programID)
    throw std::runtime_error("An error occurs creating the program object");

  // Association des shaders

  glAttachShader(m_programID, m_vertexID);
  glAttachShader(m_programID, m_fragmentID);


  // Verrouillage des entr�es shader

  glBindAttribLocation(m_programID, 0, "in_Vertex");
  glBindAttribLocation(m_programID, 1, "in_Normals");
  glBindAttribLocation(m_programID, 2, "in_Uv");


  // Linkage du programme

  glLinkProgram(m_programID);


  // V�rification du linkage

  GLint erreurLink(0);
  glGetProgramiv(m_programID, GL_LINK_STATUS, &erreurLink);


  // S'il y a eu une erreur

  if (erreurLink != GL_TRUE) {
	// R�cup�ration de la taille de l'erreur

	GLint tailleErreur(0);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &tailleErreur);


	// Allocation de m�moire

	char *erreur = new char[tailleErreur + 1];


	// R�cup�ration de l'erreur

	glGetShaderInfoLog(m_programID, tailleErreur, &tailleErreur, erreur);
	erreur[tailleErreur] = '\0';


	// Affichage de l'erreur

	std::cout << erreur << std::endl;


	// Lib�ration de la m�moire et retour du bool�en false

	delete[] erreur;
	glDeleteProgram(m_programID);

	return false;
  }


	// Sinon c'est que tout s'est bien pass�

  else
	return true;
}

bool Shader::compile(GLuint &shader, GLenum type, std::string const &fileSource) {
  // Cr�ation du shader

  shader = glCreateShader(type);

  // V�rification du shader

  if (!shader) {
	throw std::runtime_error("Error occurs during the creation of a shader");
  }

  // Flux de lecture
  std::ifstream file(fileSource.c_str());

  // Test d'ouverture

  if (!file) {
    throw std::runtime_error("Cannot open the shader " + fileSource +", the source file relative to the filepath does not exist");
  }
  // Strings permettant de lire le code source
  std::string line;
  std::string codeSource;

  while (getline(file, line))
	codeSource += line + char('\n');
  file.close();


  const GLchar* sourceCodeString = codeSource.c_str();

  // Envoi du code source au shader
  glShaderSource(shader, 1, &sourceCodeString, 0);

  // Compilation du shader
  glCompileShader(shader);

  // V�rification de la compilation
  GLint erreurCompilation(0);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);


  // S'il y a eu une erreur
  if (erreurCompilation != GL_TRUE) {
	// R�cup�ration de la taille de l'erreur
	GLint tailleErreur(0);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);

	// Allocation de m�moire
	char *erreur = new char[tailleErreur + 1];

	// R�cup�ration de l'erreur
	glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
	erreur[tailleErreur] = '\0';

	// Affichage de l'erreur
	std::cout << erreur << std::endl;

	// Lib�ration de la m�moire et retour du bool�en false
	delete[] erreur;
	glDeleteShader(shader);

	return false;
  }
	// Sinon c'est que tout s'est bien pass�

  else
	return true;
}


// Getter

const GLuint & Shader::getProgramID() const {
  return m_programID;
}
