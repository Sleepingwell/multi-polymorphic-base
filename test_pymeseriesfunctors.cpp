#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 70

#include <exception>

#include <boost/mpl/vector.hpp>

#include <GTA/time/time_period.hpp>

#include "result_functor_abstract_base.hpp"
#include "result_functor.hpp"
#include "time_series_types.hpp"


//// A test functor with 0 arg constructor
//template<typename InPoint, typename OutPoint>
//struct TestFunctor0 : PymeseriesFunctorBase<InPoint, OutPoint> {
//    double operator()(InPoint const& inPoint, OutPoint& outPoint, double weight) {
//        printf("%s -> %s\n", typeid(InPoint).name(), typeid(OutPoint).name());
//        return 42.0;
//    }
//};
//typedef make_pymeseries_functor<TestFunctor0>::type test_functor_descriptor0;





// A test functor with 1 arg constructor
template<typename InPoint, typename OutPoint>
struct TestFunctor1 : PymeseriesFunctorBase<InPoint, OutPoint> {
    TestFunctor1(double val) : val_(val) {}
    double operator()(InPoint const& inPoint, OutPoint& outPoint, double weight) {
        printf("%s -> %s with %f\n", typeid(InPoint).name(), typeid(OutPoint).name(), val_);
        return 42.0;
    }
    double val_;
};
typedef make_pymeseries_functor<TestFunctor1, args_<double> >::type test_functor_descriptor1;





// function to excercise it
template<typename FirstType, typename SecondType>
int test(
    typename detail::get_abstract_functor_getter_for_data_and_time_type<PymeseriesFunctorBase, FirstType, SecondType>::type const& functorBase
) {
    typedef dst<FirstType, SecondType, true> DatasetType;
    typedef typename DatasetType::value_type InPoint;
    typedef typename DatasetType::region_timeseries_type OutPoint;
    typedef detail::functor_descriptor_base<PymeseriesFunctorBase, InPoint, OutPoint> FuncDesc;
    typedef typename FuncDesc::abstract_functor_type abstract_functor_type;

    int res(0);

    abstract_functor_type* functor(NULL);
    OutPoint outPoint;

    try {
        functor = static_cast<FuncDesc const&>(functorBase).makeFunctor(InPoint(), OutPoint());
        printf("and the answer is... %f\n", (*functor)(InPoint(), outPoint, 1.0));
    } catch(std::exception const& e) {
        printf("%s\n", e.what());
        res = 1;
    } catch(...) {
        printf("whoops\n");
        res = 1;
    }
    if(functor) {
        delete functor;
    }
    return res;
}





int main(int argc, char* argv[]) {
    //test_functor_descriptor0 bc0;
    //test<double, GT::Time>(bc0);

    test_functor_descriptor1 bc1(1.0);
    test<double, GT::Time>(bc1);

    return 0;
}


//typedef detail::get_point_types_for_data_and_time_type<double, GT::Time>::type point_pair_vector;
//
//typedef boost::mpl::vector<int,float,float,char,int,int,int,double> types;
//typedef boost::mpl::vector<int,float,char,int,double> expected;
//typedef boost::mpl::unique< types, boost::is_same<boost::mpl::_1, boost::mpl::_2> >::type result;
//
// int main() {
//     printf("%s\n\n", typeid(point_pair_vector).name());
//     printf("%s\n\n", typeid(result).name());
//     return 0;
// }
//