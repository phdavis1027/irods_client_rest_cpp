/**
* iRODS REST API
* This is the iRODS REST API
*
* OpenAPI spec version: 1.0.0
* Contact: info@irods.org
*
* NOTE: This class is auto generated by the swagger code generator program.
* https://github.com/swagger-api/swagger-codegen.git
* Do not edit the class manually.
*/

#include "QueryApi.h"

namespace io {
namespace swagger {
namespace server {
namespace api {

using namespace io::swagger::server::model;

QueryApi::QueryApi(Pistache::Address addr)
    : httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr))
{ };

void QueryApi::init(size_t thr = 2) {
    auto opts = Pistache::Http::Endpoint::options()
        .threads(thr);
    httpEndpoint->init(opts);
    setupRoutes();
}

void QueryApi::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void QueryApi::shutdown() {
    httpEndpoint->shutdown();
}

void QueryApi::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Get(router, base + "/query", Routes::bind(&QueryApi::catalog_query_handler, this));

    // Default handler, called when a route is not found
    router.addCustomHandler(Routes::bind(&QueryApi::query_api_default_handler, this));
}

void QueryApi::catalog_query_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {

    // Getting the query params
    auto queryString = request.query().get("query_string");
    auto queryLimit = request.query().get("query_limit");
    auto rowOffset = request.query().get("row_offset");
    auto queryType = request.query().get("query_type");
    
    try {
      this->catalog_query(request.headers(), request.body(), queryString, queryType, queryLimit, rowOffset, response);
    } catch (std::runtime_error & e) {
      //send a 400 error
      response.send(Pistache::Http::Code::Bad_Request, e.what());
      return;
    }

}

void QueryApi::query_api_default_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}
}
}
}

