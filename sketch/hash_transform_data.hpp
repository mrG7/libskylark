#ifndef SKYLARK_HASH_TRANSFORM_DATA_HPP
#define SKYLARK_HASH_TRANSFORM_DATA_HPP

#include <vector>

#include "../base/base.hpp"
#include "../utility/randgen.hpp"

#include "transform_data.hpp"

namespace skylark { namespace sketch {

/**
 * This is the base data class for all the hashing transforms. Essentially, it
 * holds on to a context, and to some random numbers that it has generated
 * both for the scaling factor and for the row/col indices.
 */
template <typename IndexType,
          typename ValueType,
          template <typename> class IdxDistributionType,
          template <typename> class ValueDistribution>
struct hash_transform_data_t : public transform_data_t {
    typedef IndexType index_type;
    typedef ValueType value_type;
    typedef IdxDistributionType<IndexType> idx_distribution_type;
    typedef ValueDistribution<ValueType> value_distribution_type;

    /**
     *  Constructs the data for a hashing sketch.
     *  @param N
     *  @param S
     *  @param context
     */
    hash_transform_data_t (int N, int S, base::context_t context,
                           const std::string type = "")
        : transform_data_t(N, S, context, type) {

    }

    /**
     *  Load a serialized sketch from a file.
     *  @param[in] json_filename
     */
    hash_transform_data_t (const boost::property_tree::ptree &json)
        : transform_data_t(json) {

    }

protected:
    std::vector<index_type> row_idx; /**< precomputed row indices */
    std::vector<value_type> row_value; /**< precomputed scaling factors */

    base::context_t build() {
        base::context_t tmp = transform_data_t::build();

        idx_distribution_type row_idx_distribution(0, _S - 1);
        value_distribution_type row_value_distribution;

        row_idx   = tmp.generate_random_samples_array(
                        _N, row_idx_distribution);
        row_value = tmp.generate_random_samples_array(
                        _N, row_value_distribution);

        return tmp;
    }
};

} } /** namespace skylark::sketch */

#endif /** SKYLARK_HASH_TRANSFORM_DATA_HPP */
