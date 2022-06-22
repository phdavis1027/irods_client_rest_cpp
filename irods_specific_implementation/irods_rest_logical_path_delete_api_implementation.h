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

    class logical_path_delete : public api_base
    {
    public:
        logical_path_delete()
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
                const auto _logical_path = _request.query().get("logical-path").get();
                const auto _no_trash     = _request.query().get("no-trash").getOrElse("0");
                const auto _verbose      = _request.query().get("verbose").getOrElse("0");
                const auto _recursive    = _request.query().get("recursive").getOrElse("0");
                const auto _unregister   = _request.query().get("unregister").getOrElse("0");

                const bool no_trash      = to_bool(_no_trash);
                const bool verbose       = to_bool(_verbose);
                const bool recursive     = to_bool(_recursive);
                const bool unregister    = to_bool(_unregister);

                auto conn = get_connection(_request.headers().getRaw("authorization").value());

                const fs::path logical_path = decode_url( _logical_path );

                fs::extended_remove_options opts {
                    .no_trash      = no_trash,
                    .verbose       = verbose,
                    .progress      = false,
                    .recursive     = recursive,
                    .unregister    = unregister
                };

                bool status  = fs::client::remove(*conn(), logical_path, opts);

                return std::make_tuple(
                  !status ? Pistache::Http::Code::Bad_Request : Pistache::Http::Code::Ok,
		  std::to_string(status)
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
    }; // class logical_path_delete
} // namespace irods::rest

