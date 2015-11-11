//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_MAP_H
#define SPAGHETTI_MAP_H
#include<vector>
#include <stdexcept>
#include <iostream>
#include <array>
#include <algorithm>
#include <random>

#include <GL/glew.h>


#include <glm/matrix.hpp>

#include "glk/gl/Texture.h"
#include "glk/gl/util.h"

#include "Type_Declarator.h"

#include "Shader.h"
#include "Object.h"

#include <memory>

extern glm::mat3 g_pvMat;


class Map {

    typedef std::vector<std::unique_ptr<Object>>  Objects_Type;
    unsigned int m_width, m_height;
    std::vector<Tile_Type> m_data;

    Objects_Type m_objects;

	Shader _program;
	GLuint _vbo, _vao;
	GLint _tilemapLoc, _pvmLoc;

	glk::gl::Texture _tex;
public:

    void addObject( std::unique_ptr<Object> object_ptr) {
        m_objects.push_back(std::move(object_ptr));
    }

    template<typename ... Args>
    void eraseObject(Args ... params) {
	    using ex = int[];
	    (void) ex {0, (void(m_objects.erase(params)), 0)...};
    }

    const Objects_Type & getObjects()const {
        return m_objects;
    }


    unsigned int getWidth() const {
        return m_width;
    }

    unsigned int getHeight() const {
        return m_height;
    }

    const std::vector<Tile_Type> &getM_data() const {
        return m_data;
    }

    Map(
            unsigned int width,
            unsigned int height) :
            m_width(width),
            m_height(height),
            m_data(width * height),
	        _program("data/tilemap.vert", "data/tilemap.frag")
            {
	            std::array<glm::vec2, 4> const verts{{{0.0f, 0.0f}, {width, 0.0f},{0.0f, height},{width, height}}};

	            TRY_GL(glGenVertexArrays(1, &_vao));

	            TRY_GL(glBindVertexArray(_vao));
	            TRY_GL(glGenBuffers(1, &_vbo));
	            TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _vbo));

	            TRY_GL(glEnableVertexAttribArray(0u));
	            TRY_GL(glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(0)));

	            TRY_GL(glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec2), verts.data(), GL_STATIC_DRAW));

	            CHECK_GL_ERROR();
	            TRY_GL(_program.load());

	            _pvmLoc = glGetUniformLocation(_program.getProgramID(), "pvmMatrix");
	            _tilemapLoc = glGetUniformLocation(_program.getProgramID(), "tilemap");
	            CHECK_GL_ERROR();

	            _tex.bind();
	            TRY_GL(glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, width, height));
	            _tex.unbind();

	            std::mt19937 rng;
	            rng.seed(1447054199);
	            std::generate(begin(m_data), end(m_data), [&rng] {
		            return !std::uniform_int_distribution<>{0, 1}(rng);
	            });
            }

    Map(
            unsigned int width,
            unsigned int height,
            const std::vector<Tile_Type> &data) :
            Map(width,height)
    {

        try{
            setData(data);
        }catch (const std::length_error & error) {
            std::cout<<"Invalid argument data"<<std::endl;
            m_data = std::vector<Tile_Type>(m_width * m_height);
        }

    }

    Tile_Type &operator()(unsigned int i, unsigned int j) {
        if( i >= m_width)
            throw std::range_error("The first argument is invalid");
        if(j >= m_height)
            throw std::range_error("The second argument is invalid");

        return m_data[j * m_height + i];
    }

    const Tile_Type & operator()(const Coord_type & point)const {
        return (*this)(int(point.x), int(point.y));
    }

    const Tile_Type &operator()(unsigned int i, unsigned int j)const {
        if( i >= m_width)
            throw std::range_error("The first argument is invalid");
        if(j >= m_height)
            throw std::range_error("The second argument is invalid");

        return m_data[j * m_height + i];
    }

    Tile_Type & operator()(const Coord_type & point) {
        return (*this)(point.x, point.y);
    }

    void setData(const std::vector<Tile_Type> &data) {
        if(data.size() != m_data.size() )
            throw std::length_error("Bad vector data size");
        m_data = data;
    }

	void display() const {
		_tex.bind();
		TRY_GL(
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RED_INTEGER, GL_UNSIGNED_INT, m_data.data()));
		_tex.unbind();

		TRY_GL(glUseProgram(_program.getProgramID()));
		TRY_GL(glUniformMatrix3fv(_pvmLoc, 1, GL_FALSE, glm::value_ptr(g_pvMat)));
		TRY_GL(glUniform1i(_tilemapLoc, 0));

		_tex.bind(GL_TEXTURE0);

		TRY_GL(glBindVertexArray(_vao));
		TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
		TRY_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	}

	glk::gl::Texture const &tilemap() const { return _tex; }
};


#endif //SPAGHETTI_MAP_H
