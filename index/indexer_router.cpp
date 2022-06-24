//
// Created by Nathan Kamm on 5/28/22.
//

#include "indexer_router.h"
#include "../hash/xxhash64.h"


IndexerRouter::IndexerRouter() {


}

void IndexerRouter::EnrichMetric(ts_proto::MetricWriter *metric) {

    XXHash64 metadataHash = XXHash64(0);
    metadataHash.add(metric->namespace_().c_str(), metric->namespace_().length());

    // this is not ideal.? (gmap doesn't guarantee sort post wire
    std::map<std::string, std::string> orderedMap(metric->dimensions().begin(),
                                                  metric->dimensions().end());

    for (auto const& [dimKey, dimValue] : orderedMap) {
        metadataHash.add(dimKey.c_str(), dimKey.length());
        metadataHash.add(dimValue.c_str(), dimValue.length());
    }

    metric->set_metadatauid(metadataHash.hash());

    for (auto const& [metricName, _] : metric->metrics()) {
        XXHash64 metaHashCopy = metadataHash;
        metaHashCopy.add(metricName.c_str(), metricName.length());

        (*metric->mutable_metricuid())[metricName] = metaHashCopy.hash();
    }

}

void IndexerRouter::IngestMetric(ts_proto::MetricWriter *metric) {

    if (!this->namedSpacedIndexes.contains(metric->namespace_())) {
        this->namedSpacedIndexes[metric->namespace_()] = std::make_unique<Indexer>();
    }

    this->namedSpacedIndexes[metric->namespace_()]->Index(metric);


}
