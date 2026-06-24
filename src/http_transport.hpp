#pragma once
#include <string>

extern "C" {
#include <curl/curl.h>
}

namespace mlflow {

class HttpTransport {
public:
  explicit HttpTransport(std::string base_url)
      : base_url_(std::move(base_url)) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
  }

  ~HttpTransport() { curl_global_cleanup(); }

  struct Response {
    long status_code;
    std::string body;
  };

  
  struct CurlSession
  {
    CURL* handle = nullptr;
    struct curl_slist* headers = nullptr;
  };

  CurlSession setup_curl_instance(const std::string& endpoint)
  {
    CurlSession session;
    session.handle = curl_easy_init();

    if(session.handle)
    {
      std::string url = base_url_ + "/api/2.0/mlflow" + endpoint;
      session.headers = curl_slist_append(session.headers, "Content-Type: application/json");

      curl_easy_setopt(session.handle, CURLOPT_URL, url.c_str());
      curl_easy_setopt(session.handle, CURLOPT_HTTPHEADER, session.headers);
    }

    return session;
  }

  Response exec_curl(CurlSession session)
  {
    if(!session.handle)
    {
      return {0, "Initialization failed"};
    }

    std::string response_string;
    long response_code = 0;

    auto write_cb = [](char* data, size_t size, size_t nmemb, std::string* writer) -> size_t{
      size_t total_size = size * nmemb;
      writer->append(data, total_size);
      return total_size;
    };

    curl_easy_setopt(session.handle, CURLOPT_WRITEFUNCTION, +write_cb);
    curl_easy_setopt(session.handle, CURLOPT_WRITEDATA, &response_string);

    curl_easy_perform(session.handle);
    curl_easy_getinfo(session.handle, CURLINFO_RESPONSE_CODE, &response_code);

    if(session.headers)
    {
      curl_slist_free_all(session.headers);
    }
    curl_easy_cleanup(session.handle);

    return {response_code, response_string};
  }

  Response post(const std::string &endpoint, const std::string &json_payload) {
    CurlSession session = setup_curl_instance(endpoint);

    curl_easy_setopt(session.handle, CURLOPT_POSTFIELDS, json_payload.c_str());

    return exec_curl(session);
  }

  Response get(const std::string &endpoint, const std::string& json_payload)
  {
    CurlSession session = setup_curl_instance(endpoint);
    curl_easy_setopt(session.handle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(session.handle, CURLOPT_POSTFIELDS, nullptr);
    curl_easy_setopt(session.handle, CURLOPT_POSTFIELDSIZE, 0L);

    return exec_curl(session);
  }

private:
  std::string base_url_;
};

} // namespace mlflow