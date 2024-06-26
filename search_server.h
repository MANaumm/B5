#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "string_processing.h"
#include "document.h"

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double EPSILON = 1e-6;

class SearchServer{
public:
    SearchServer() = default;

    template <typename StringCollection>
    SearchServer(const StringCollection& stop_words){
        for(const std::string& word : stop_words){
            SetStopWords(word);
        }
    }

    SearchServer(const std::string& stop_words)
        :SearchServer(SplitIntoWords(stop_words)){
    }

    void SetStopWords(const std::string& stop_words);

    void AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings);

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const{

        const Query query = ParseQuery(raw_query);

        auto matched_documents = FindAllDocuments(query, document_predicate);
        std::sort(matched_documents.begin(), matched_documents.end(),
            [] (const Document& lhs, const Document& rhs){
                if(abs(lhs.relevance - rhs.relevance) < EPSILON){
                    return lhs.rating > rhs.rating;
                } else{
                    return lhs.relevance > rhs.relevance;
                }
            });
        if(matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT){
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status) const;

    std::vector<Document> FindTopDocuments(const std::string& raw_query) const;

    int GetDocumentCount() const;

    int GetDocumentId(int index) const;

    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const;

private:
    struct DocumentData{
        int rating;
        DocumentStatus status;
    };

    std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_;
    std::vector<int> documents_ids_;
    

    bool IsStopWord(const std::string& word) const;

    static bool IsValidWord(const std::string& word);

    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const;

    int ComputeAverageRating(const std::vector<int>& ratings);

    struct QueryWord{
        std::string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(std::string text) const;

    struct Query{
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    Query ParseQuery(const std::string& text) const;

    double ComputeWordInverseDocumentFreq(const std::string& word) const;

    template <typename Predicate>
    std::vector<Document> FindAllDocuments(const Query& query, Predicate predicat) const{
        std::map<int, double> document_to_relevance;
        for(const std::string& word : query.plus_words){
            if(word_to_document_freqs_.count(word) == 0){
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for(const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)){

                //��������� ��������, �������� �� �� � ���� ����� ���������: id, status, rating
                if(predicat(document_id, documents_.at(document_id).status, documents_.at(document_id).rating) == true){
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for(const std::string& word : query.minus_words){
            if(word_to_document_freqs_.count(word) == 0){
                continue;
            }
            for(const auto& [document_id, _] : word_to_document_freqs_.at(word)){
                document_to_relevance.erase(document_id);
            }
        }

        std::vector<Document> matched_documents;
        for(const auto& [document_id, relevance] : document_to_relevance){
            matched_documents.push_back({document_id, relevance, documents_.at(document_id).rating});
        }

        return matched_documents;
    }
};