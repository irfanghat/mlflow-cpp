#pragma once

#include<string>
#include<vector>
#include<memory>
#include<functional>
#include<algorithm>

extern "C"
{
    #include<curl/curl.h>
}

namespace mlflow
{
    struct AsyncRequest
    {
        CURL* easy_handle = nullptr;
        struct curl_slist* headers = nullptr;
        std::string payload;
        std::string response_body;
        long response_code = 0;
    
        std::function<void(long code, const std::string& body)> on_complete;
    
        ~AsyncRequest()
        {
            if(headers) curl_slist_free_all(headers);
            if(easy_handle) curl_easy_cleanup(easy_handle);
        }
    };
    
    class AsyncHttpTransport
    {
        private:
            CURLM* multi_handle_;
            std::string base_url_;
            std::vector<std::unique_ptr<AsyncRequest>> active_requests_;
    
            std::unique_ptr<AsyncRequest> setup_easy_handle(const std::string& endpoint)
            {
                auto req = std::make_unique<AsyncRequest>();
                req->easy_handle = curl_easy_init();
    
                if(!req->easy_handle) return nullptr;
    
                std::string url = base_url_ + "/api/2.0/mlflow" + endpoint;
                req->headers = curl_slist_append(req->headers, "Content-Type: application/json");
    
                curl_easy_setopt(req->easy_handle, CURLOPT_URL, url.c_str());
                curl_easy_setopt(req->easy_handle, CURLOPT_HTTPHEADER, req->headers);
    
                auto write_cb = [](char* data, size_t size, size_t nmemb, std::string* writer) -> size_t
                {
                    size_t total_size = size * nmemb;
                    writer->append(data, total_size);
                    return total_size;
                };
    
                curl_easy_setopt(req->easy_handle, CURLOPT_WRITEFUNCTION, +write_cb);
                curl_easy_setopt(req->easy_handle, CURLOPT_WRITEDATA, &req->response_body);
                curl_easy_setopt(req->easy_handle, CURLOPT_PRIVATE, req.get());
    
                return req;
            }
    
        public:
            explicit AsyncHttpTransport(std::string base_url) : base_url_(std::move(base_url)) 
            {
                multi_handle_ = curl_multi_init();
            }
    
            ~AsyncHttpTransport()
            {
                active_requests_.clear();
                curl_multi_cleanup(multi_handle_);
            }
    
            void async_post(const std::string& endpoint, const std::string& json_payload, std::function<void(long, const std::string&)> callback)
            {
                auto req = setup_easy_handle(endpoint);
                if(!req) return;
    
                req->payload = json_payload;
                req->on_complete = callback;
    
                curl_easy_setopt(req->easy_handle, CURLOPT_POSTFIELDS, req->payload.c_str());
                curl_multi_add_handle(multi_handle_, req->easy_handle);
                active_requests_.push_back(std::move(req));
            }
    
            void async_get(const std::string& endpoint, std::function<void(long, const std::string&)> callback)
            {
                auto req = setup_easy_handle(endpoint);
                if(!req) return;
    
                req->on_complete = callback;
    
                curl_multi_add_handle(multi_handle_, req->easy_handle);
                active_requests_.push_back(std::move(req));
            }
    
            void process_requests()
            {
                int still_running = 0;
    
                CURLMcode mc = curl_multi_perform(multi_handle_, &still_running);
                if(mc != CURLM_OK) return;
    
                int msgs_left = 0;
                CURLMsg* msg = nullptr;
    
                while((msg = curl_multi_info_read(multi_handle_, &msgs_left)))
                {
                    if(msg->msg == CURLMSG_DONE)
                    {
                        CURL* easy_handle = msg->easy_handle;
    
                        AsyncRequest* req = nullptr;
                        curl_easy_getinfo(easy_handle, CURLINFO_PRIVATE, &req);
    
                        if(req)
                        {
                            curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &req->response_code);
    
                            if(req->on_complete)
                            {
                                req->on_complete(req->response_code, req->response_body);
                            }
    
                            curl_multi_remove_handle(multi_handle_, easy_handle);
    
                            active_requests_.erase(
                                std::remove_if(active_requests_.begin(), active_requests_.end(),
                            [easy_handle](const std::unique_ptr<AsyncRequest>& r)
                            {
                                return r->easy_handle == easy_handle;
                            }),
                            active_requests_.end()
                            );
                        }
                    }
                }
    
                if(still_running > 0)
                {
                    curl_multi_poll(multi_handle_, nullptr, 0, 100, nullptr);
                }
    
            }
    
            bool has_active_requests() const { return !active_requests_.empty(); }
    };
}
