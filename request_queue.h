#pragma once
#include <vector>
#include <deque>
#include <string>
#include "search_server.h"
#include "document.h"

class RequestQueue{
public:
    RequestQueue(const SearchServer& search_server) : search_server_(search_server){}

    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate){
        std::vector<Document> add_find_request = search_server_.FindTopDocuments(raw_query, document_predicate);
        AddRequest(add_find_request.size());
        
        return add_find_request;
    }

    /*std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);*/

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status){
        return RequestQueue::AddFindRequest (raw_query, [status] (int document_id, DocumentStatus document_status, int rating){
            return document_status == status;
            });
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query){
        std::vector<Document> add_find_request = search_server_.FindTopDocuments(raw_query, DocumentStatus::ACTUAL);

        AddRequest(add_find_request.size());
        return add_find_request;
    }

    int GetNoResultRequests() const;

private:
    struct QueryResult{
        uint64_t timestamp;
        int results;
    };

    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    int no_result_requests_;
    uint64_t current_time_;
    const SearchServer& search_server_;

    void AddRequest(int results_num);
};