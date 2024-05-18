#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>

template <typename Iterator>
class IteratorRange{
public:
    IteratorRange (){
    }

    IteratorRange (Iterator page_begin, Iterator page_end)
        : begin_(page_begin), end_(page_end){
    }

    auto begin() const{
        return begin_;
    }

    auto end() const{
        return end_;
    }
    size_t size() const{
        return distance(begin_, end_);
    }

private:
    Iterator begin_;
    Iterator end_;
};

template <typename Iterator>
class Paginator{
public:
    //В аргументы передаем начало и конец контейнера, размер страницы
    Paginator (Iterator range_begin, Iterator range_end, size_t page_size)
        : range_begin_(range_begin)
        , range_end_(range_end)
        , page_size_(page_size){
        size_t all_documents = distance(range_begin_, range_end_);

        while(all_documents >= page_size_){
            auto it_page_begin = range_begin;
            auto it_page_end = range_begin + page_size_;
            pages_.push_back({it_page_begin, it_page_end});
            advance(range_begin, page_size);
            all_documents = distance(range_begin, range_end);
        }
        if(distance(range_begin, range_end) > 0){
            pages_.push_back({range_begin, range_end});
        }

    }

    auto begin() const{
        return pages_.begin();
    }

    auto end() const{
        return pages_.end();
    }

    size_t size (){
        return pages_.size();
    }
private:
    Iterator range_begin_;
    Iterator range_end_;
    size_t page_size_;
    std::vector <IteratorRange<Iterator>> pages_;
};


template <typename Container>
auto Paginate(const Container& c, size_t page_size){
    return Paginator(c.begin(), c.end(), page_size);
}

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator> range){
    for(auto i = range.begin(); i != range.end(); ++i){
        out << *i;
    }
    return out;
}