//
// Created by Nathan Kamm on 6/18/22.
//

#ifndef TIMESERIES_DB_INDEXED_SCHEMA_H
#define TIMESERIES_DB_INDEXED_SCHEMA_H


#include <roaring/roaring.hh>
#include "../service/query_write.pb.h"

class indexed_schema {
public:
    void Index(ts_proto::MetricWriter *metric);

    void Query(std::map<std::string, std::string> tagQuery);

    int ObtainRowIndex(uint64_t rowIndex);

private:

    int rowIndexIncrementor{0};
    std::map<uint64_t, int> rowIndex;

    std::map<
        std::string, /* tag_K */
        std::map<
            std::string, /* tag_V */
            roaring::Roaring
        >
    > namespacedMetricIndex;

};


#endif //TIMESERIES_DB_INDEXED_SCHEMA_H
