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
* PutConfigurationApiImpl.h
*
* 
*/

#ifndef GET_CONFIGURATION_API_IMPL_H_
#define GET_CONFIGURATION_API_IMPL_H_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/optional.h>

#include "ModelBase.h"

#include <PutConfigurationApi.h>

#include <memory>
#include <string>

#include "irods_rest_put_configuration_api_implementation.h"

namespace io::swagger::server::api
{
    using namespace io::swagger::server::model;

    class PutConfigurationApiImpl
        : public io::swagger::server::api::PutConfigurationApi
    {
    public:
        PutConfigurationApiImpl(Pistache::Address addr);

        ~PutConfigurationApiImpl() = default;

        void handler_impl(const Pistache::Rest::Request& request,
                          Pistache::Http::ResponseWriter& response) override;

        irods::rest::put_configuration irods_put_configuration_;
    }; // class PutConfigurationApiImpl
} // namespace io::swagger::server::api

#endif // GET_CONFIGURATION_API_IMPL_H_
