//
// Created by Nathan Kamm on 5/28/22.
//

#include "indexer.h"

Indexer::Indexer() {}
Indexer::~Indexer() = default;

void Indexer::Index(ts_proto::MetricWriter *metric) {

//    if ( index. )

    if (!this->index.contains(metric->schema())) {
        this->metricSchemaRegistrationLock.lock();
        if (!this->index.contains(metric->schema())) {
            this->index[metric->schema()] = std::make_unique<indexed_schema>();
        }
        this->metricSchemaRegistrationLock.unlock();
    }


    this->index[metric->schema()]->Index(metric);


}