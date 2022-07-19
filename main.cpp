#include <iostream>

#include "service/query_write.pb.h"
#include "index/indexer.h"
#include "query/query_statements.h"

int main() {

    std::vector <ts_proto::MetricWriter*> metrics;

    auto dcs = std::vector<std::string>{"sjc", "sea", "bru", "nyc", "aus"};
    auto intfs = std::vector<std::string>{"eth0", "eth1", "bond0", "bond1", "nic0", "nic1", "peer1"};
    auto hostnames = std::vector<std::string>{"{{dc}}1.corp.github.com", "{{dc}}2.corp.github.com", "{{dc}}3.corp.github.com", "{{dc}}4.corp.github.com"};

    for (const auto& dc : dcs) {
        for (const auto& intf: intfs) {
            for (const auto& hostname: hostnames) {

                ts_proto::MetricWriter* metricGroup = new ts_proto::MetricWriter();
                metricGroup->set_namespace_("panoptes");
                metricGroup->set_schema("network_interface");
                (*metricGroup->mutable_dimensions())["data_center"] = dc;
                (*metricGroup->mutable_dimensions())["network_interface"] = intf;
                (*metricGroup->mutable_dimensions())["hostname"] = hostname;

                metrics.push_back(metricGroup);

            }
        }



    }

    auto indexer = std::make_unique<Indexer>();
    for (auto& metric: metrics) {
        indexer->EnrichMetric(metric);
        indexer->Index(metric);
    }

    std::shared_ptr<indexed_schema> indexedSchema = indexer->getIndexedSchemaForNamespace("network_interface");

    if (indexedSchema == nullptr) {
        std::cout << "nullptr exited" << std::endl;
        exit(1);
    }

    auto queryStatement = std::make_unique<query_holder_base>("network_interface", "eth0");

    auto bitmapResultToMetaDataIndex = queryStatement->evaluate(indexedSchema.get());

    std::cout << "bitmap cardinality " << bitmapResultToMetaDataIndex.cardinality() << std::endl;

    int bytesSaved = bitmapResultToMetaDataIndex.shrinkToFit();

    std::cout << "number of bytes saved: " << bytesSaved << std::endl;

    bitmapResultToMetaDataIndex.rffg

    std::cout << metrics.size() << std::endl;

    return 0;
}

/*

 g++ -std=c++20 main.cpp index/indexer.h index/indexer.cpp index/indexer_router.cpp index/indexer_router.h stream/bitstream.cpp stream/bitstream.h stream/blockbuilder.cpp stream/blockbuilder.h timeseries/timeseries.cpp timeseries/timeseries.h service/query_write.pb.cpp service/query_write.pb.h hash/xxhash64.h index/indexed_schema.cpp index/indexed_schema.h query/query_statements.cpp query/query_statements.h `pkg-config --cflags --libs protobuf` -lroaring


 */