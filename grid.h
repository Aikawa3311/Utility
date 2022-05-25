#include <iostream>
#include <vector>
#include <utility>

namespace Utility{

/**
 * @brief 二次元配列クラス at(y,x)でアクセス
*/
template <typename data_type>
class Grid2D{
private: 
    std::vector<data_type> m_data;
    size_t m_width;
    size_t m_height;

public:
    Grid2D(size_t const m_width, size_t const m_height)
        : m_width(m_width),
        m_height(m_height),
        m_data(m_width * m_height){}
    

    /**
     * @brief 要素アクセス
    */
    data_type & at(int const y, int const x){
        return m_data.at(x + y*m_width);
    }
    data_type & at(std::pair<int, int> const pos){
        return m_data.at(pos.first + pos.second*m_width);
    }

    /**
     * @brief 要素アクセス const
    */
    data_type const & at(int const y, int const x) const {
        return m_data.at(x + y*m_width);
    }
    data_type const & at(std::pair<int, int> const pos) const {
        return m_data.at(pos.first + pos.second*m_width);
    }

    /**
     * @brief [y][x]で要素アクセス
    */
    data_type * operator [] (int const y){
        return &m_data[y*m_width];
    }
    data_type const * operator [] (int const y) const {
        return &m_data[y*m_width];
    }

    /**
     * @brief 横方向のサイズを返す
    */
    size_t width() const {
        return m_width;
    }

    /**
     * @brief 縦方向のサイズを返す
    */
    size_t height() const {
        return m_height;
    }

    /**
     * @brief pairでサイズを返す
    */
    size_t size() const {
        return std::make_pair(m_width, m_height);
    }
}


} // namespace Utility