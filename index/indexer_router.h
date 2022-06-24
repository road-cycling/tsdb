//
// Created by Nathan Kamm on 5/28/22.
//

#ifndef TIMESERIES_DB_INDEXER_ROUTER_H
#define TIMESERIES_DB_INDEXER_ROUTER_H

#include "../service/query_write.pb.h"
#include "indexer.h"

class IndexerRouter {
public:
    IndexerRouter();

    void EnrichMetric(ts_proto::MetricWriter *metric);
    void IngestMetric(ts_proto::MetricWriter *metric);


private:
    std::map<std::string, std::unique_ptr<Indexer>> namedSpacedIndexes{};
};

#endif //TIMESERIES_DB_INDEXER_ROUTER_H
