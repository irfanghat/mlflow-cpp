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

  Response post(const std::string &endpoint, const std::string &json_payload) {
    CURL *curl = curl_easy_init();
    std::string response_string;
    long response_code = 0;

    if (curl) {
      std::string url = base_url_ + "/api/2.0/mlflow" + endpoint;
      struct curl_slist *headers = nullptr;
      headers = curl_slist_append(headers, "Content-Type: application/json");

      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload.c_str());

      auto write_cb = [](char *data, size_t size, size_t nmemb,
                         std::string *writer) -> size_t {
        writer->append(data, size * nmemb);
        return size * nmemb;
      };
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +write_cb);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

      curl_easy_perform(curl);
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
    }
    return {response_code, response_string};
  }

private:
  std::string base_url_;
};

} // namespace mlflow