#include "pch.h"
#include <iostream>
#include "paginator.h"
//#include "read_input_functions.h"
#include "request_queue.h"
#include "search_server.h"
#include "document.h"

using namespace std; 

int main(){
    SearchServer search_server("and in at"s);
    RequestQueue request_queue(search_server);

    search_server.AddDocument(1, "curly cat curly tail", DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "curly dog and fancy collar", DocumentStatus::ACTUAL, {1, 2, 3});
    search_server.AddDocument(3, "big cat fancy collar ", DocumentStatus::ACTUAL, {1, 2, 8});
    search_server.AddDocument(4, "big dog sparrow Eugene", DocumentStatus::ACTUAL, {1, 3, 2});
    search_server.AddDocument(5, "big dog sparrow Vasiliy", DocumentStatus::ACTUAL, {1, 1, 1});

    
    //const auto search_results1 = search_server.FindTopDocuments("big cat pet"s);
    //int page_size = 1;
    //const auto pages = Paginate(search_results1, page_size);
    //// Выводим найденные документы по страницам
    //for(auto page = pages.begin(); page != pages.end(); ++page){
    //    cout << *page << endl;
    //    cout << "Page break"s << endl;
    //}

    // 1439 запросов с нулевым результатом
    for(int i = 0; i < 1439; ++i){
        request_queue.AddFindRequest("empty request");
    }
    // все еще 1439 запросов с нулевым результатом
    request_queue.AddFindRequest("curly dog");
    // новые сутки, первый запрос удален, 1438 запросов с нулевым результатом
    request_queue.AddFindRequest("big collar");
    // первый запрос удален, 1437 запросов с нулевым результатом
    request_queue.AddFindRequest("sparrow");
    std::cout << "Total empty requests: " << request_queue.GetNoResultRequests() << std::endl;
    return 0;
}