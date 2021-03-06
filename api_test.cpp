//https://github.com/2jerry/cpprestsdk_with_api.git
#include <iostream>
using namespace std;

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
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
	http_client client(U("https://openapi.naver.com/v1"));
	uri_builder builder(L"/search/news");

	const utility::string_t query(U("query=미세먼지&sort=sim&display=100"));
	builder.set_query(query,true);
	http_request req(methods::GET);
	req.set_request_uri(builder.to_string());

	req.headers().add(U("X-Naver-Client-Id"), U("myId"));
	req.headers().add(U("X-Naver-Client-Secret"), U("my_secret"));



	client.request(req).then([=](http_response r) {
		wcout << U("STATUS : ") << r.status_code() << endl;
		wcout << "content-type : " << r.headers().content_type() << endl;

		r.extract_json(true).then([](json::value v) {
			wcout << v.at(U("display")) << endl;
			json::value v2 = v.at(U("items"));
			//wcout << v2 << endl;
			for (int i = 0; i < v2.size(); i++)
			{
				wstring link = v2[i].at(U("link")).as_string();
				if (link.find(L"naver") != -1)
				{
					wcout << v2[i].at(U("title")).as_string() << endl;
					//wcout << link << endl;

					http_client client(utility::conversions::to_string_t(link));

					client.request(U("GET")).then([](http_response resp) {
						wcout << U("STATUS : ") << resp.status_code() << endl;
						wcout << "content-type : " << resp.headers().content_type() << endl;

						resp.extract_string(true).then([](string_t sBoby) {
							wcout << sBoby << endl;
						}).wait();

					}).wait();
				}
			}

			// items -> link 따라가서 -> <div id="articleBodyContents"> 태그 


		}).wait();

	}).wait();
}
