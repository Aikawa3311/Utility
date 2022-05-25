/**
 * @brief 三次元配列用クラス
 * @note 参照:[https://github.com/Siv3D/OpenSiv3D/blob/main/Siv3D/include/Siv3D/detail/Grid.ipp]
*/

#include <iostream>
#include <vector>
#include <utility>
#include <tuple>

namespace Utility{

template <typename data_type>
class Grid3D;

/**
 * @brief Grid3Dのoperator[]アクセス用クラス
*/
template <typename data_type>
class Grid3DAccessController{
private:
    Grid3D<data_type> * grid;
    int z_pos;

public:
    Grid3DAccessController(Grid3D<data_type> * grid, int const z_pos)
        : grid(grid),
        z_pos(z_pos){}
    
    /**
     * @brief [y][x]で要素アクセス
    */
    data_type * operator [] (int const y){
        return &(grid->data()[z_pos * (grid->width()) * (grid->height()) + y * grid->width()]);
    }
    data_type const * operator [] (int const y) const {
        return &(grid->data()[z_pos * (grid->width()) * (grid->height()) + y * grid->width()]);
    }
};

/**
 * @brief 三次元配列クラス at(z,y,x)でアクセス
*/
template <typename data_type>
class Grid3D{
public:
    using container_type = std::vector<data_type>;
    using size_type = size_t;

private: 
    container_type m_data;  // [x + y * m_width + z * m_width * m_height]でアクセス
    size_type m_width = 0;  // 横
    size_type m_height = 0; // 縦
    size_type m_depth = 0;  // 奥行

public:
    Grid3D(size_type const m_width = 0, size_type const m_height = 0, size_type const m_depth = 0)
        : m_data(m_width * m_height * m_depth),
        m_width(m_width),
        m_height(m_height),
        m_depth(m_depth){}

    Grid3D(size_type const m_width, size_type const m_height, size_type const m_depth, data_type const & init)
        : m_data(m_width * m_height * m_depth, init),
        m_width(m_width),
        m_height(m_height),
        m_depth(m_depth){}
    
    /**
     * @brief width, height, depthのタプルから構築
    */
    Grid3D(std::tuple<size_type, size_type, size_type> const & size)
        : Grid3D(std::get<0>(size), std::get<1>(size), std::get<2>(size)){}

    /**
     * @brief width, height, depthのタプルから構築
    */
    Grid3D(std::tuple<size_type, size_type, size_type> const & size, data_type const & init)
        : Grid3D(std::get<0>(size), std::get<1>(size), std::get<2>(size), init){}

    /**
     * @brief 配列のクリア
    */
    void clear(){
        m_data.clear();
        m_width = 0;
        m_height = 0;
        m_depth = 0;
    }

    /**
     * @brief 列の挿入
     * @param[in] pos 挿入する場所(挿入する位置がpos(0-indexed)列目になるように)
     * @param[in] init 初期化する値
    */
    void insert_column(int const pos, data_type const & init){
        reserve(m_width+1, m_height, m_depth);

        for(size_type i=0; i<m_depth; ++i){
            for(size_type j=0; j<m_height; ++j){
                m_data.insert(m_data.begin() + pos + j * (m_width + 1) + i * (m_width + 1) * (m_height), init);
            }
        }

        ++m_width;
    }

    /**
     * @brief 行の挿入
     * @param[in] pos 挿入する場所(挿入する位置がpos行目になるように)
     * @param[in] init 初期化する値
    */
    void insert_row(int const pos, data_type const & init){
        reserve(m_width, m_height+1, m_depth);

        for(size_type i=0; i<m_depth; ++i){
            m_data.insert(m_data.begin() + pos*m_width + i * (m_width) * (m_height + 1), m_width, init);
        }
        
        ++m_height;
    }

    /**
     * @brief 奥の挿入
     * @param[in] pos 挿入する場所(挿入する位置がpos奥目になるように)
     * @param[in] init 初期化する値
    */
    void insert_depth(int const pos, data_type const & init){
        reserve(m_width, m_height, m_depth+1);

        m_data.insert(m_data.begin() + pos*m_width*m_height, m_width*m_height, init);
        
        ++m_depth;
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
        insert_row(m_height, init);
    }

    /**
     * @brief 奥を後方に追加
     * @param[in] init 初期化する値
    */
    void push_back_depth(data_type const & init){
        for(size_type i=0; i<m_width*m_height; ++i){
            m_data.push_back(init);
        }
        ++m_depth;
    }

    /**
     * @brief 複数の列を後方に追加
     * @param[in] init 初期化する値
    */
    void push_back_columns(size_type const n, data_type const & init){
        for(size_type i=0; i<n; ++i) push_back_column(init);
    }

    /**
     * @brief 複数の行を後方に追加
     * @param[in] init 初期化する値
    */
    void push_back_rows(size_type const n, data_type const & init){
        for(size_type i=0; i<n; ++i) push_back_row(init);
    }

    /**
     * @brief 複数の奥を後方に追加
     * @param[in] init 初期化する値
    */
    void push_back_depths(size_type const n, data_type const & init){
        for(size_type i=0; i<n; ++i) push_back_depth(init);
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
    void remove_row(size_type const pos){
        // auto it = m_data.begin() + pos * m_width;

        // for(size_type i=0; i<m_depth; ++i){
        //     auto it_begin = it + i * m_width * m_height;
        //     it = m_data.erase(it_begin, it_begin + m_width);
        // }

        int index = 0;

        auto remove_it = std::remove_if(m_data.begin(), m_data.end(), [width = m_width, height = m_height, &pos, &index](data_type const &){
            return ((index++) / width) % height == pos;
        });
        m_data.erase(remove_it, m_data.end());

        --m_height;
    }

    /**
     * @brief 任意の奥の削除
     * @param[in] pos 消したい奥(pos(0-indexed)奥目となるように)
    */
    void remove_depth(int const pos){
        auto it_begin = m_data.begin() + pos * m_width * m_height;
        m_data.erase(it_begin, it_begin + m_width * m_height);
        
        --m_depth;
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
        remove_row(m_height - 1);
    }

    /**
     * @brief 後方の奥の削除
    */
    void pop_back_depth(){
        --m_depth;
        m_data.resize(m_data.size() - m_width * m_height);
    }

    /**
     * @brief 後方の複数の列の削除
    */
    void pop_back_columns(size_type const n){
        for(size_type i=0; i<n; ++i) pop_back_column();
    }

    /**
     * @brief 後方の複数の行の削除
    */
    void pop_back_rows(size_type const n){
        for(size_type i=0; i<n; ++i) pop_back_row();
    }

    /**
     * @brief 後方の複数の奥の削除
    */
    void pop_back_depths(size_type const n){
        for(size_type i=0; i<n; ++i) pop_back_depth();
    }

    /**
     * @brief リザーブ
     * @note m_width, m_height, m_depthは変えない
    */
    void reserve(int const w, int const h, int const d){
        m_data.reserve(w * h * d);
    }

    /**
     * @brief リサイズ
    */
    void resize(size_type const w, size_type const h, size_type const d, data_type const & init){
        // 幅を合わせる
        if(w < m_width){
            pop_back_columns(m_width - w);
        }else if(w > m_width){
            push_back_columns(w - m_width, init);
        }
        // 縦を合わせる
        if(h < m_height){
            pop_back_rows(m_height - h);
        }else if(h > m_height){
            push_back_rows(h - m_height, init);
        }
        // 奥行を合わせる
        if(d < m_depth){
            pop_back_depths(m_depth - d);
        }else if(d > m_depth){
            push_back_depths(d - m_depth, init);
        }
    }

    /**
     * @brief リサイズ
     * @param[in] size (width, height, depth)のタプル
     * @param[in] init 初期化したい値
    */
    void resize(std::tuple<size_type, size_type, size_type> const & size, data_type const & init){
        resize(std::get<0>(size), std::get<1>(size), std::get<2>(size), init);
    }

    /**
     * @brief リサイズ
    */
    void resize(size_type const w, size_type const h, size_type const d){
        resize(w, h, d, data_type{});
    }

    /**
     * @brief リサイズ
    */
    void resize(std::tuple<size_type, size_type, size_type> const & size){
        resize(std::get<0>(size), std::get<1>(size), std::get<2>(size));
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
    data_type & at(int const z, int const y, int const x){
        return m_data.at(x + y*m_width + z*m_width*m_height);
    }

    /**
     * @brief (x,y,z)のタプルにより要素アクセス
     * @param[in] pos (x,y,z)のタプル
    */
    data_type & at(std::tuple<int, int, int> const pos){
        return m_data.at(std::get<0>(pos) + std::get<1>(pos)*m_width + std::get<2>(pos)*m_width*m_height);
    }

    /**
     * @brief 要素アクセス const
    */
    data_type const & at(int const z, int const y, int const x) const {
        return m_data.at(x + y*m_width + z*m_width*m_height);
    }

    /**
     * @brief (x,y,z)のタプルにより要素アクセス const
     * @param[in] pos (x,y,z)のタプル
    */
    data_type const & at(std::tuple<int, int, int> const pos) const {
        return m_data.at(std::get<0>(pos) + std::get<1>(pos)*m_width + std::get<2>(pos)*m_width*m_height);
    }

    /**
     * @brief [z][y][x]で要素アクセス
    */
    Grid3DAccessController<data_type> operator [] (int const z){
        return Grid3DAccessController<data_type>(this, z);
    }
    Grid3DAccessController<data_type> const operator [] (int const z) const {
        return Grid3DAccessController<data_type>(this, z);
    }

    /**
     * @brief (x, y, z)で要素アクセス
     * @param[in] pos (x,y,z)のタプル
    */
    data_type & operator [] (std::tuple<int, int, int> const & pos){
        return operator[](std::get<2>(pos))[std::get<1>(pos)][std::get<0>(pos)];
    }
    data_type const & operator [] (std::tuple<int, int, int> const & pos) const {
        return operator[](std::get<2>(pos))[std::get<1>(pos)][std::get<0>(pos)];
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
     * @brief 奥行方向のサイズを返す
    */
    size_t depth() const {
        return m_depth;
    }

    /**
     * @brief 配列の先頭要素のポインタを返す
    */
    data_type * data(){
        return m_data.data();
    }

    /**
     * @brief 範囲内に収まるかを調べる
     * @return 収まっていたらtrue
    */
    bool in(int const z, int const y, int const x) const {
        return z >= 0 && z < m_depth && y >= 0 && y < m_height && x >= 0 && x < m_width;
    }

    /**
     * @brief (x, y, z)が範囲内に収まるかを調べる
     * @param[in] pos (x, y, z)のタプル
     * @return 収まっていたらtrue
    */
    bool in(std::tuple<int, int, int> const & pos) const {
        return in(std::get<2>(pos), std::get<1>(pos), std::get<0>(pos));
    }

    /**
     * @brief tupleでサイズを返す
     * @return width, height, depthのペア
    */
    std::tuple<size_t, size_t, size_t> size() const {
        return std::make_tuple(m_width, m_height, m_depth);
    }

    /**
     * @brief 出力
    */
    void print() const {
        for(size_type i=0; i<m_height; ++i){
            for(size_type j=0; j<m_depth; ++j){
                std::cout << '[';
                for(size_type k=0; k<m_width; ++k){
                    std::cout << at(j, i, k);
                    std::cout << ((k == m_width - 1) ? "" : " ");
                }
                std::cout << "] ";
            }
            std::cout << std::endl;
        }
    }

    /**
     * @brief サイズの出力
    */
    void print_size() const {
        std::cout << "(width:" << m_width << " height:" << m_height << " depth:" << m_depth << ")" << std::endl;
    }

    /**
     * @brief 各要素への一律な操作
     * @param[in] func z,y,xを引数として受け取る関数
    */
    template <typename Function>
    void foreach(const Function & func){
        for(size_type i=0; i<m_depth; ++i){
            for(size_type j=0; j<m_height; ++j){
                for(size_type k=0; k<m_width; ++k){
                    func(i, j, k);
                }
            }
        }
    }

#ifdef EIGEN_CORE_H

    // Eigen/Coreがincludeされている場合

    /**
     * @brief (x, y, z)で要素アクセス
     * @param[in] pos (x,y,z)の整数ベクトル
    */
    data_type & at(Eigen::Vector3i const & pos){
        return at(pos.z(), pos.y(), pos.x());
    }

    /**
     * @brief (x, y, z)で要素アクセス const
     * @param[in] pos (x,y,z)の整数ベクトル
    */
    data_type const & at(Eigen::Vector3i const & pos) const {
        return at(pos.z(), pos.y(), pos.x());
    }

    /**
     * @brief (x, y, z)で要素アクセス
     * @param[in] pos (x,y,z)の整数ベクトル
    */
    data_type & operator [] (Eigen::Vector3i const & pos){
        return operator[](pos.z())[pos.y()][pos.x()];
    }

    /**
     * @brief (x, y, z)で要素アクセス const
     * @param[in] pos (x,y,z)の整数ベクトル
    */
    data_type const & operator [] (Eigen::Vector3i const & pos) const {
        return operator[](pos.z())[pos.y()][pos.x()];
    }

    /**
     * @brief リサイズ
     * @param[in] size (width, height, depth)の整数ベクトル
     * @param[in] init 初期化したい値
    */
    void resize(Eigen::Vector3i const & size, data_type const & init){
        resize(size.x(), size.y(), size.z(), init);
    }

    /**
     * @brief リサイズ
     * @param[in] size (width, height, depth)の整数ベクトル
    */
    void resize(Eigen::Vector3i const & size){
        resize(size.x(), size.y(), size.z());
    }

    /**
     * @brief (x, y, z)が範囲内に収まるかを調べる
     * @param[in] pos (x, y, z)の整数ベクトル
     * @return 収まっていたらtrue
    */
    bool in(Eigen::Vector3i const & pos) const {
        return in(pos.z(), pos.y(), pos.x());
    }

#endif

};


} // namespace Utility