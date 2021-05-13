#pragma once

#include "../../../dependencies/json-rpc-cxx/include/jsonrpccxx/client.hpp"
#include "../../../dependencies/json-rpc-cxx/include/jsonrpccxx/server.hpp"
#include "../../../dependencies/json-rpc-cxx/include/jsonrpccxx/iclientconnector.hpp"
#include "./rpc_types.h"

using namespace juce;
//using namespace ultralight;

namespace juce_ultralight
{

    class JSConnector : public jsonrpccxx::IClientConnector {
    public:
        explicit JSConnector(jsonrpccxx::JsonRpcServer &server) : server(server) {}
        std::string Send(const std::string &request) override {
            juce::Logger::writeToLog("JSConnector::Send() was called");
            return server.HandleRequest(request); }
    private:
        jsonrpccxx::JsonRpcServer &server;
    };

}
