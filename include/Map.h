//
// Created by Patataa on 11/11/2015.
//

#ifndef SPAGHETTI_MAP_H
#define SPAGHETTI_MAP_H
#include<vector>
#include <stdexcept>
#include <iostream>

template <typename Tiles = unsigned int>
class Map {

    unsigned int m_width, m_height;
    std::vector<Tiles> m_data;
public:


    const std::vector<Tiles> &getM_data() const {
        return m_data;
    }

    Map(
            unsigned int width,
            unsigned int height) :
            m_width(width),
            m_height(height),
            m_data(width * height)
            { }

    Map(
            unsigned int width,
            unsigned int height,
            const std::vector<Tiles> &data) :
            Map(width,height)
    {

        try{
            setData(data);
        }catch (const std::length_error & error) {
            std::cout<<"Invalid argument data"<<std::endl;
            m_data = std::vector<Tiles>(m_width * m_height);
        }

    }

    Tiles &operator()(unsigned int i, unsigned int j) {
        if( i >= m_width)
            throw std::range_error("The first argument is invalid");
        if(j >= m_height)
            throw std::range_error("The second argument is invalid");

        return m_data[j * m_height + i];
    }

    void setData(const std::vector<Tiles> &data) {
        if(data.size() != m_data.size() )
            throw std::length_error("Bad vector data size");
        m_data = data;
    }


};


#endif //SPAGHETTI_MAP_H
