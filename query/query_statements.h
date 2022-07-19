//
// Created by Nathan Kamm on 7/1/22.
//

#ifndef TIMESERIES_DB_QUERY_STATEMENTS_H
#define TIMESERIES_DB_QUERY_STATEMENTS_H

#include "../index/indexed_schema.h"

const auto QS_AND = "qs_and";
const auto QS_OR = "qs_or";

class query {

    virtual roaring::Roaring evaluate(indexed_schema *namespacedIndex) = 0;
    virtual query* getLeft() = 0;
    virtual query* getRight() = 0;

};

class query_holder_base : query {
public:
    query_holder_base(std::string queryTag, std::string queryValue) {
        this->queryValue = queryValue;
        this->queryTag = queryTag;
    }

    roaring::Roaring evaluate(indexed_schema *namespacedIndex) {
        return namespacedIndex->QueryFromAST(this->queryTag, this->queryValue);
    }

    query* getLeft() {
        return nullptr;
    }

    query* getRight() {
        return nullptr;
    }

private:
    std::string queryTag;
    std::string queryValue;

};

class query_statement : query {

    roaring::Roaring evaluate(indexed_schema *namespacedIndex) override {
        if (this->matcherLeft != nullptr && this->matcherRight != nullptr) {
            auto left = this->matcherLeft->evaluate(namespacedIndex);
            auto right = this->matcherRight->evaluate(namespacedIndex);

            if (this->typeOfQuery == QS_AND) {
                return left & right;
            } else if ( this->typeOfQuery == QS_OR ) {
                return left | right;
            } else {
                // panic

            }
        }

        return namespacedIndex->QueryFromAST(this->tagK_matcher, this->tagV_matcher);

    }

    query* getLeft() override {
        return this->matcherLeft;
    }

    query* getRight() override {
        return this->matcherRight;
    }

private:


    [[maybe_unused]] const char*const typeOfQuery{};

    std::string tagK_matcher{""};
    std::string tagV_matcher{""};

    query_statement* matcherLeft{};
    query_statement* matcherRight{};


};


/*

              |
              |
              |
        QEXPR (AND | OR)
         /            \
   "hostname=foo"    fabric="ltx1"


 */

#endif //TIMESERIES_DB_QUERY_STATEMENTS_H
