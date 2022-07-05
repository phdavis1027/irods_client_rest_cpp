#include "irods_rest_api_base.h"
#include "constants.hpp"

#include <irods/filesystem.hpp>
#include <irods/rodsClient.h>
#include <irods/irods_query.hpp>

#include <pistache/http_headers.h>
#include <pistache/optional.h>
#include <pistache/router.h>
namespace fs = irods::experimental::filesystem;
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
            try {
                auto conn = get_connection(_request.headers().getRaw("authorization").value());
                const auto _cmds = _request.query().get("cmds").get();
                // parse cmds into a json array
                const auto json_cmds = nlohmann::json::parse(_cmds);
                if ( is_input_valid(json_cmds, conn) ){
                    std::vector<std::string> failed_cmds;
                    for (auto& cmd_obj : json_cmds){
                        if ( "rename" == cmd_obj["cmd"] ) {
                            fs::client::rename( // this function does not return a status value
                                *conn(),
                                cmd_obj["src"],
                                cmd_obj["dst"]
                            );
                        } else if ( "delete" == cmd_obj["cmd"] ) {
                            fs::extended_remove_options opts {
                                .no_trash   = cmd_obj["opts"]["no-trash"],
                                .verbose    = cmd_obj["opts"]["verbose"],
                                .progress   = false,
                                .recursive  = cmd_obj["opts"]["recursive"],
                                .unregister = cmd_obj["opts"]["unregister"]
                            };

                            if ( ! fs::client::remove(*conn(), cmd_obj["cmd"], opts) ) {
                                failed_cmds.push_back(cmd_obj.dump());
                            }
                        } else {
                            failed_cmds.push_back(cmd_obj.dump());
                        }
                    }
                    nlohmann::json ret(failed_cmds);
                    return std::make_tuple(
                            Pistache::Http::Code::Ok,
                            ret.dump()
                    );
                } else {
                    return std::make_tuple(Pistache::Http::Code::Bad_Request, "Request contained ill-formed commands");
                }
            }
            catch (const fs::filesystem_error& e) {
                error("Caught exception - [error_code={}] {}", e.code().value(), e.what());
                return make_error_response(e.code().value(), e.what());
            }
            catch (const nlohmann::json::exception& e){
                error("Caught exception - [error_code={}] {}", 0, e.what());
                return make_error_response(0, e.what());
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

        bool is_input_valid(const nlohmann::json& json_obj, connection_proxy& conn) {
            if ( ! json_obj.is_array() ){
                return false;
            }

            for (auto& elem : json_obj) {
                if ( "rename" == elem["cmd"] ) {
                    auto args = elem["args"];
                    // check if src and dst exist
                    if ( ! fs::client::exists(*conn(), args["src"]) || ! fs::client::exists(*conn(), args["dst"])) return false;
                } else if ( "delete" == elem["cmd"] ) {
                    auto args = elem["args"];
                    auto opts = elem["opts"];
                    if ( ! fs::client::exists(*conn(), args["logical-path"]) ) return false;
                    if ( ! is_valid_http_bool(opts["unregister"]) ) return false;
                    if ( ! is_valid_http_bool(opts["recursive"]) ) return false;
                    if ( ! is_valid_http_bool(opts["verbose"]) ) return false;
                } else { return false; }
            }

            return true;
        }
    }; // class logical_path
} // namespace irods::rest
