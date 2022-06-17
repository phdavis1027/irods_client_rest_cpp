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

/*
* QueryApiImpl.h
*
* 
*/

#ifndef QUERY_API_IMPL_H_
#define QUERY_API_IMPL_H_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/optional.h>

#include "ModelBase.h"

#include <QueryApi.h>

#include "Query_results.h"

#include <memory>
#include <string>

#include "irods_rest_query_api_implementation.h"

namespace io::swagger::server::api
{
    using namespace io::swagger::server::model;

    class QueryApiImpl
        : public io::swagger::server::api::QueryApi
    {
    public:
        QueryApiImpl(Pistache::Address addr);

        ~QueryApiImpl() = default;

        void handler_impl(const Pistache::Rest::Request& request,
                          Pistache::Http::ResponseWriter& response) override;

        irods::rest::query irods_query_;
    }; // class QueryApiImpl
} // namespace io::swagger::server::api

#endif // QUERY_API_IMPL_H_