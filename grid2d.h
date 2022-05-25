/**
 * @brief 二次元配列用クラス
 * @note 参照:[https://github.com/Siv3D/OpenSiv3D/blob/main/Siv3D/include/Siv3D/detail/Grid.ipp]
*/

#include <iostream>
#include <vector>
#include <utility>

namespace Utility{

/**
 * @brief 二次元配列クラス at(y,x)でアクセス
*/
template <typename data_type>
class Grid2D{
public:
    using container_type = std::vector<data_type>;
    using size_type = size_t;

private: 
    container_type m_data;  // [x + y * m_width]でデータへアクセス
    size_type m_width = 0;  // 横
    size_type m_height = 0; // 縦

public:
    Grid2D(size_type const m_width = 0, size_type const m_height = 0)
        : m_data(m_width * m_height),
        m_width(m_width),
        m_height(m_height){}

    Grid2D(size_type const m_width, size_type const m_height, data_type const & init)
        : m_data(m_width * m_height, init),
        m_width(m_width),
        m_height(m_height){}
    
    /**
     * @brief width, heightのペアから構築
    */
    Grid2D(std::pair<size_type, size_type> const & size)
        : m_data(size.first, size.second),
        m_width(size.first),
        m_height(size.second){}

    /**
     * @brief width, heightのペアから構築
    */
    Grid2D(std::pair<size_type, size_type> const & size, data_type const & init)
        : m_data(size.first, size.second, init),
        m_width(size.first),
        m_height(size.second){}

    /**
     * @brief 配列のクリア
    */
    void clear(){
        m_data.clear();
        m_width = 0;
        m_height = 0;
    }

    /**
     * @brief 列の挿入
     * @param[in] pos 挿入する場所(挿入する位置がpos(0-indexed)列目になるように)
     * @param[in] init 初期化する値
    */
    void insert_column(int const pos, data_type const & init){
        reserve(m_width+1, m_height);

        for(size_type i=0; i<m_height; ++i){
            m_data.insert(m_data.begin() + (pos + i * (m_width + 1)), init);
        }
        ++m_width;
    }

    /**
     * @brief 行の挿入
     * @param[in] pos 挿入する場所(挿入する位置がpos行目になるように)
     * @param[in] init 初期化する値
    */
    void insert_row(int const pos, data_type const & init){
        reserve(m_width, m_height+1);

        m_data.insert(m_data.begin() + pos*m_width, m_width, init);
        ++m_height;
    }

    /**
     * @brief 列を後方に追加
     * @param[in] init 初期化する値
    */
    void push_back_column(data_type const & init){
        insert_column(m_width, init);
    }

    /**
     * @brief 行を後方に追加
     * @param[in] init 初期化する値
    */
    void push_back_row(data_type const & init){
        for(size_type i=0; i<m_width; ++i){
            m_data.push_back(init);
        }
        ++m_height;
    }

    /**
     * @brief 任意の列の削除
     * @param[in] pos 消したい列(pos(0-indexed)列目となるように)
    */
    void remove_column(size_type const pos){
        int index = 0;

        auto remove_it = std::remove_if(m_data.begin(), m_data.end(), [width = m_width, &pos, &index](data_type const &){
            return (index++) % width == pos;
        });
        m_data.erase(remove_it, m_data.end());
        --m_width;
    }

    /**
     * @brief 任意の行の削除
     * @param[in] pos 消したい行(pos(0-indexed)行目となるように)
    */
    void remove_row(int const pos){
        m_data.erase(m_data.begin() + pos * m_width, m_data.begin() + (pos + 1) * m_width);
        --m_height;
    }

    /**
     * @brief 後方の列の削除
    */
    void pop_back_column(){
        remove_column(m_width - 1);
    }

    /**
     * @brief 後方の行の削除
    */
    void pop_back_row(){
        --m_height;
        m_data.resize(m_data.size() - m_width);
    }

    /**
     * @brief リザーブ
     * @note m_width, m_heightは変えない
    */
    void reserve(int const w, int const h){
        m_data.reserve(w * h);
    }

    /**
     * @brief リザーブ
     * @note m_width, m_heightは変えない
    */
    void reserve(std::pair<int, int> const & size){
        m_data.reserve(size.first * size.second);
    }

    /**
     * @brief リサイズ
    */
    void resize(size_type const w, size_type const h, data_type const & init){
        // 幅を合わせる
        if(w < m_width){
            for(size_type i=0; i<m_width - w; ++i) pop_back_column();
        }else if(w > m_width){
            for(size_type i=0; i<m_width - w; ++i) push_back_column(init);
        }
        // 縦を合わせる
        if(h < m_height){
            for(size_type i=0; i<m_height - h; ++i) pop_back_row();
        }else if(h > m_height){
            for(size_type i=0; i<m_height - h; ++i) push_back_row(init);
        }
    }

    /**
     * @brief リサイズ
    */
    void resize(std::pair<size_type, size_type> const & size, data_type const & init){
        resize(size.first, size.second, init);
    }

    /**
     * @brief リサイズ
    */
    void resize(size_type const w, size_type const h){
        resize(w, h, data_type{});
    }

    /**
     * @brief リサイズ
    */
    void resize(std::pair<size_type, size_type> const & size){
        resize(size.first, size.second);
    }

    /**
     * @brief 一番最初の要素のlvalue参照
    */
    data_type & front(){
        return m_data.front();
    }
    data_type const & front() const {
        return m_data.front();
    }

    /**
     * @brief 一番最後の要素のlvalue参照
    */
    data_type & back(){
        return m_data.back();
    }
    data_type const & back() const {
        return m_data.back();
    }

    /**
     * @brief 要素アクセス
    */
    data_type & at(int const y, int const x){
        return m_data.at(x + y*m_width);
    }

    /**
     * @brief (x,y)のペアにより要素アクセス
     * @param[in] pos (x,y)のペア
    */
    data_type & at(std::pair<int, int> const pos){
        return m_data.at(pos.first + pos.second*m_width);
    }

    /**
     * @brief 要素アクセス const
    */
    data_type const & at(int const y, int const x) const {
        return m_data.at(x + y*m_width);
    }

    /**
     * @brief (x,y)のペアにより要素アクセス const
     * @param[in] pos (x,y)のペア
    */
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
     * @return width, heightのペア
    */
    std::pair<size_t, size_t> size() const {
        return std::make_pair(m_width, m_height);
    }

    /**
     * @brief 出力
    */
    void print() const {
        for(size_type i=0; i<m_height; ++i){
            for(size_type j=0; j<m_width; ++j){
                std::cout << at(i, j) << ' ';
            }
            std::cout << std::endl;
        }
    }

    /**
     * @brief 各要素への一律な操作
     * @param[in] func y,xを引数として受け取る関数
    */
    template <typename Function>
    void foreach(const Function & func){
        for(size_type i=0; i<m_height; ++i){
            for(size_type j=0; j<m_width; ++j){
                func(i, j);
            }
        }
    }

};


} // namespace Utility