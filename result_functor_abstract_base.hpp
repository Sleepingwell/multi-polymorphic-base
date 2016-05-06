#ifndef PYMESERIES_RESULT_FUNCTOR_ABSTRACT_BASE_HEADER_INCLUDED_R64D36GHUS36G54FJ35K4F35H4S3GF14D
#define PYMESERIES_RESULT_FUNCTOR_ABSTRACT_BASE_HEADER_INCLUDED_R64D36GHUS36G54FJ35K4F35H4S3GF14D

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/quote.hpp>

#include <GTA/data_types.h>
#include <GTA/time/time_period.hpp>

#include "multi_bind.hpp"
#include "time_series_types.hpp"



namespace detail {
    // base class for functor descriptors
    template<template<typename, typename> class AbstractFunctor, typename InPoint, typename OutPoint>
    struct functor_descriptor_base {
        typedef AbstractFunctor<InPoint, OutPoint> abstract_functor_type;
        virtual ~functor_descriptor_base(void) {};
        virtual abstract_functor_type* makeFunctor(InPoint const&, OutPoint const&) const = 0;
    };





    template<typename DataType, typename TimePeriodType>
    struct get_point_types_for_data_and_time_type {

        // the dataset types. we deal with the chunked/full datasets together
        // because there is likely (and is at present) same types in the
        // pairs below. It also means we (potentially) have a better shaped
        // inheritance heirachy... that is one that needs shorter sequences
        // of types, which I am close to the limit on anyways.
        typedef dst<DataType, TimePeriodType, false> full_dataset_type;
        typedef typename full_dataset_type::region_timeseries_type full_region_timeseries_type;

        typedef dst<DataType, TimePeriodType, true > chunked_dataset_type;
        typedef typename chunked_dataset_type::region_timeseries_type chunked_region_timeseries_type;

        // all possible in/out points for a given data type and time period type
        typedef boost::mpl::vector6< // order is important (unique depends on it)
            boost::mpl::pair<typename full_dataset_type::reference, full_region_timeseries_type>,
            boost::mpl::pair<typename chunked_dataset_type::reference, chunked_region_timeseries_type>,
            boost::mpl::pair<typename full_dataset_type::const_reference, full_region_timeseries_type>,
            boost::mpl::pair<typename chunked_dataset_type::const_reference, chunked_region_timeseries_type>,
            boost::mpl::pair<typename full_dataset_type::value_type, full_region_timeseries_type>,
            boost::mpl::pair<typename chunked_dataset_type::value_type, chunked_region_timeseries_type>
        > point_types;

        // remove the duplicates
        typedef typename boost::mpl::unique< point_types, boost::is_same<boost::mpl::_1, boost::mpl::_2> >::type type;
    };





    // get a vector of all point pairs we have to deal with.
    template<typename DataTypes, typename TimePeriodTypes>
    struct get_inout_pairs {
        typedef typename multibind::apply_seqences_to_binder<
            multibind::double_bind< boost::mpl::quote2<get_point_types_for_data_and_time_type> >,
            boost::mpl::vector2<DataTypes, TimePeriodTypes>
        >::type type;
    };





    template<
        template<typename, typename> class AbstractFunctor,
        typename InOutPointPairs
    >
    struct get_abstract_functor_getter_for_point_pairs {
        // makes the abstract base class for an in/out point pair
        template<typename PointPair>
        struct make_abstract_func_getter {
            typedef functor_descriptor_base<AbstractFunctor, typename PointPair::first, typename PointPair::second> type;
        };

        // all the point pairs required for a single data type and time period type
        typedef typename boost::mpl::transform<InOutPointPairs, boost::mpl::quote1<make_abstract_func_getter> >::type abstract_getters;

        // the base class for a single data type and time period type
        typedef typename boost::mpl::inherit_linearly<abstract_getters, boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2> >::type type;
    };





    // metafunction to make an abstract base class for a single data type and time period type
    template<
        template<typename, typename> class AbstractFunctor,
        typename DataType,
        typename TimePeriodType
    > struct get_abstract_functor_getter_for_data_and_time_type {
        // the in/out pairs for a single data type and time period type
        typedef typename get_point_types_for_data_and_time_type<DataType, TimePeriodType>::type inout_point_pairs;
        typedef typename get_abstract_functor_getter_for_point_pairs<AbstractFunctor, inout_point_pairs>::type type;
    };
}// end namespace detail





// a vector with an element for each data type. each element is itself
// a vector with an element for each time period type. each elemet of those vectors are
// a pair with the input and output point types
typedef detail::get_inout_pairs<GT::GTDataTypes, GT::GTTemporalPeriodTypes>::type pymeseries_inout_point_pairs;





template<typename InPoint, typename OutPoint>
struct PymeseriesFunctorBase {
    virtual ~PymeseriesFunctorBase(void) {}
    virtual double operator()(InPoint const& inPoint, OutPoint& outPoint, double weight) = 0;//{ throw std::runtime_error("Operation not supported"); }
    virtual void finaliseTimeSeries(OutPoint& outPoint) const { };
};
#endif // PYMESERIES_RESULT_FUNCTOR_ABSTRACT_BASE_HEADER_INCLUDED_R64D36GHUS36G54FJ35K4F35H4S3GF14D
