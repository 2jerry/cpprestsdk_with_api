//https://github.com/2jerry/cpprestsdk_with_api.git
#include <iostream>
using namespace std;

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "cpprest/streams.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features

using namespace concurrency::streams;

int main()
{
	setlocale(LC_ALL, "kor");
	std::locale::global(std::locale("kor"));
	std::wcout.imbue(std::locale("kor"));
	http_client client(U("https://polling.finance.naver.com/"));
	uri_builder builder(L"api/realtime.nhn");

	const utility::string_t query(U("query=SERVICE_ITEM:950160"));
	builder.set_query(query, true);
	http_request req(methods::GET);
	req.set_request_uri(builder.to_string());

	client.request(req).then([=](http_response r) {
		wcout << U("STATUS : ") << r.status_code() << endl;
		wcout << "content-type : " << r.headers().content_type() << endl;
		r.headers().set_content_type(U("text/plain;charset=UTF-8"));
		wcout << "content-type : " << r.headers().content_type() << endl;

		r.extract_utf8string(true).then([](string v) {
			cout << v << endl;
		}).wait();

	}).wait();
}
