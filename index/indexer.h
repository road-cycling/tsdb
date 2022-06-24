//
// Created by Nathan Kamm on 5/28/22.
//

#ifndef TIMESERIES_DB_INDEXER_H
#define TIMESERIES_DB_INDEXER_H


#include <roaring/roaring.hh>
#include "../service/query_write.pb.h"
#include "indexed_schema.h"

class Indexer {
public:
    Indexer();
    ~Indexer();

    void Index(ts_proto::MetricWriter *metric);
private:

    std::mutex metricSchemaRegistrationLock;

//    std::map<std::string, std::mutex> tagKeyLocks;

    std::map<std::string, std::unique_ptr<indexed_schema>> index;




};

#endif //TIMESERIES_DB_INDEXER_H
