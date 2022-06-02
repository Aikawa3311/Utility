#include <iostream>
#include <utility>
#include <cassert>

#ifndef UTILITY_POINT2I_H
#define UTILITY_POINT2I_H

namespace Utility{


/**
 * @brief int型の2つの値(x, y)を持つPoint型
*/
class Point2i{
public:
    using value_type = int32_t;

    value_type x;
    value_type y;

    Point2i(value_type const x = 0, value_type const y = 0) : x(x), y(y){}

    /**
     * @brief (x, y)のstd::pairを受け取る
     * @param[in] pos (x, y)のstd::pair
    */
    Point2i(std::pair<value_type, value_type> const pos) : Point2i(pos.first, pos.second){}

    /**
     * @brief x,yをセットする
    */
    void set(value_type const _x, value_type const _y){
        x = _x;
        y = _y;
    }

    /**
     * @brief x,yをセットする
    */
    void set(Point2i const & pos){
        *this = pos;
    }

    /**
     * @brief x,yをセットする
     * @param[in] pos (x, y)を組とするペア
    */
    void set(std::pair<value_type, value_type> const pos){
        set(pos.first, pos.second);
    }

    /**
     * @brief (x, y)のstd::pairを返す
    */
    std::pair<value_type, value_type> pos() const {
        return std::make_pair(x, y);
    }

    /**
     * @brief index番目の要素を受け取る 0でx, 1でy
     * @param[in] index 何番目の要素か
     * @return index=0でx. index=1でyを返す
    */
    value_type elem(int const index) const {
        if(index == 0){
            return x;
        }else if(index == 1){
            return y;
        }else{
            std::cerr << "elem()の引数indexの値が異常です:" << index << std::endl;
            exit(1);
        }
    }

    bool is_zero() const {
        return x == 0 && y == 0;
    }

    Point2i moveBy(Point2i const & p){
        return *this += p;
    }

    Point2i moveBy(value_type const _x, value_type const _y){
        x += _x;
        y += _y;
        return *this;
    }

    Point2i movedBy(Point2i const & p) const {
        Point2i res = *this;
        res.moveBy(p);
        return res;
    }

    Point2i movedBy(value_type const _x, value_type const _y) const {
        Point2i res = *this;
        res.moveBy(_x, _y);
        return res;
    }

    Point2i operator -() const {
        return {-x, -y};
    }

    Point2i operator +=(Point2i const & right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    Point2i operator -=(Point2i const & right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    Point2i operator *=(Point2i const & right) {
        x *= right.x;
        y *= right.y;
        return *this;
    }

    Point2i operator /=(Point2i const & right) {
        x /= right.x;
        y /= right.y;
        return *this;
    }

    Point2i operator *=(value_type const & right) {
        x *= right;
        y *= right;
        return *this;
    }

    Point2i operator /=(value_type const & right) {
        x /= right;
        y /= right;
        return *this;
    }

    Point2i operator *=(float const & right) {
        x *= right;
        y *= right;
        return *this;
    }

    Point2i operator /=(float const & right) {
        x /= right;
        y /= right;
        return *this;
    }

    Point2i operator *=(double const & right) {
        x *= right;
        y *= right;
        return *this;
    }

    Point2i operator /=(double const & right) {
        x /= right;
        y /= right;
        return *this;
    }

    Point2i operator +(Point2i const & right) const {
        Point2i res = *this;
        res += right;
        return res;
    }

    Point2i operator -(Point2i const & right) const {
        Point2i res = *this;
        res -= right;
        return res;
    }

    Point2i operator *(Point2i const & right) const {
        Point2i res = *this;
        res *= right;
        return res;
    }

    Point2i operator /(Point2i const & right) const {
        Point2i res = *this;
        res /= right;
        return res;
    }

    Point2i operator *(value_type const & right) const {
        Point2i res = *this;
        res *= right;
        return res;
    }

    Point2i operator /(value_type const & right) const {
        Point2i res = *this;
        res /= right;
        return res;
    }

    Point2i operator *(float const & right) const {
        Point2i res = *this;
        res *= right;
        return res;
    }

    Point2i operator /(float const & right) const {
        Point2i res = *this;
        res /= right;
        return res;
    }

    Point2i operator *(double const & right) const {
        Point2i res = *this;
        res *= right;
        return res;
    }

    Point2i operator /(double const & right) const {
        Point2i res = *this;
        res /= right;
        return res;
    }

    friend Point2i operator *(value_type const & left, Point2i const & right){
        return right * left;
    }

    friend Point2i operator /(value_type const & left, Point2i const & right){
        return right / left;
    }

    friend Point2i operator *(float const & left, Point2i const & right){
        return right * left;
    }

    friend Point2i operator /(float const & left, Point2i const & right){
        return right / left;
    }

    friend Point2i operator *(double const & left, Point2i const & right){
        return right * left;
    }

    friend Point2i operator /(double const & left, Point2i const & right){
        return right / left;
    }

    bool operator ==(Point2i const & right) const {
        return (x == right.x && y == right.y);
    }

    bool operator !=(Point2i const & right) const {
        return !(*this == right);
    }

    /**
     * @brief xで大小を判定、同一ならyで大小を判定
     * @note std::pairのoperator<を参照
    */
    bool operator <(Point2i const & right) const {
        return x < right.x || (!(right.x < x) && y < right.y);
    }

    /**
     * @brief xで大小を判定、同一ならyで大小を判定
    */
    bool operator <=(Point2i const & right) const {
        return !(right < *this);
    }

    /**
     * @brief xで大小を判定、同一ならyで大小を判定
    */
    bool operator >(Point2i const & right) const {
        return right < *this;
    }

    /**
     * @brief xで大小を判定、同一ならyで大小を判定
    */
    bool operator >=(Point2i const & right) const {
        return !(*this < right);
    }

    friend std::ostream & operator <<(std::ostream & left, Point2i const & right){
        left << right.x << " " << right.y;
        return left;
    }

    friend std::istream & operator >>(std::istream & left, Point2i & right){
        left >> right.x >> right.y;
        return left;
    }
};


} // namespace Utility






#endif // UTILITY_POINT2I_H