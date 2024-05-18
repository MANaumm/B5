#pragma once
#include <ostream>
#include <string>

struct Document{
    Document(){
        id = 0;
        relevance = 0.0;
        rating = 0;
    }

    Document(int doc_id, double doc_relevance, int doc_rating)
        : id(doc_id), relevance(doc_relevance), rating(doc_rating){
    }

    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus{
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

//Оператор вывода для struct Document
std::ostream& operator << (std::ostream& out, const Document& document);