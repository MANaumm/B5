#include "pch.h"
#include "request_queue.h"

using namespace std;

// сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики     
//std::vector<Document> RequestQueue::AddFindRequest(const string& raw_query, DocumentStatus status){
//    return RequestQueue::AddFindRequest (raw_query, [status] (int document_id, DocumentStatus document_status, int rating){    
//        return document_status == status;    
//        });    
//} 
//
//std::vector<Document> RequestQueue::AddFindRequest(const string& raw_query){    
//    std::vector<Document> add_find_request = search_server_.FindTopDocuments(raw_query, DocumentStatus::ACTUAL);    
//
//    AddRequest(add_find_request.size());    
//    return add_find_request;    
//}   

int RequestQueue::GetNoResultRequests() const{    
    return no_result_requests_;            
}    

void RequestQueue::AddRequest(int results_num){    
    ++current_time_;    
    while(!requests_.empty() && min_in_day_ <= current_time_ - requests_.front().timestamp){    
        if(0 == requests_.front().results){    
            --no_result_requests_;    
        }    
        requests_.pop_front();    
    }    
    requests_.push_back({current_time_, results_num});    
    if(0 == results_num){    
        ++no_result_requests_;    
    }    
}    