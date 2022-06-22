#include "irods_rest_api_base.h"
#include "constants.hpp"

#include <irods/filesystem.hpp>
#include <irods/rodsClient.h>
#include <irods/irods_random.hpp>
#include <irods/rodsErrorTable.h>
#include <irods/irods_at_scope_exit.hpp>
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

    class logical_path_rename : public api_base
    {
    public:
        logical_path_rename()
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

                auto _src = _request.query().get("src").get();
                auto _dst = _request.query().get("dst").get();

                const fs::path src = decode_url(_src);
                const fs::path dst = decode_url(_dst);

                auto conn = get_connection(_request.headers().getRaw("authorization").value());

                fs::client::rename(*conn(), src, dst);

                return std::make_tuple(
                        Pistache::Http::Code::Ok,
			""
                );

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
    }; // class logical_path_rename
} // namespace irods::rest

