#include "irods_rest_api_base.h"
#include "constants.hpp"

#include <irods/filesystem.hpp>
#include <irods/rodsClient.h>
#include <irods/irods_query.hpp>

#include <pistache/http_headers.h>
#include <pistache/optional.h>
#include <pistache/router.h>

namespace irods::rest
{
    // this is contractually tied directly to the api implementation
#ifndef LOGICAL_PATH_SERVICE_NAME
#define LOGICAL_PATH_SERVICE_NAME
    const std::string service_name{"irods_rest_cpp_logicalpath_server"};
#endif

    class logical_path_bulk : public api_base
    {
    public:
        logical_path_bulk()
            : api_base{service_name}
        {
            info("Endpoint initialized.");
        }

        std::tuple<Pistache::Http::Code, std::string>
        operator()(const Pistache::Rest::Request& _request,
                   Pistache::Http::ResponseWriter& _response)
        {
            namespace fs = irods::experimental::filesystem;

            try {
                const auto _cmds = _request.query().get("cmds").get();
                // parse cmds into a json array
                const auto json_cmds = nlohmann::json::parse(_cmds);
                // iterate over cmds

                  // check if each cmd is valid
                    // we'll say if you elect to do bulk operations, then you have to specify every opt
                  // if not, abort mission
                // execute each one
            }
            catch (const fs::filesystem_error& e) {
                error("Caught exception - [error_code={}] {}", e.code().value(), e.what());
                return make_error_response(e.code().value(), e.what());
            }
            catch (const irods::exception& e) {
                error("Caught exception - [error_code={}] {}", e.code(), e.what());
                return make_error_response(e.code(), e.what());
            }
            catch (const std::exception& e) {
                error("Caught exception - {}", e.what());
                return make_error_response(SYS_INTERNAL_ERR, e.what());
            }
        } // operator()

        bool is_input_valid(const nlohmann::json& json_obj) {
            if ( ! json_obj.is_array() )
            {
                return false;
            }

            for (auto it = json_obj.begin(); it != json_obj.end(); ++it)
            {

            }

            return true
        }
    }; // class logical_path
} // namespace irods::rest

