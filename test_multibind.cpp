#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 70

#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pair.hpp>

#include "multi_bind.hpp"

using namespace multibind;
using namespace boost;

typedef mpl::vector<double, float> first_types;
typedef mpl::vector<int, long> second_types;
typedef mpl::vector<bool> third_types;





typedef mpl::vector<first_types, second_types> first_and_second_types;
struct make_pair { template<typename First, typename Second> struct apply { typedef mpl::pair<First, Second> type; }; };

typedef double_bind<make_pair> double_binder;
typedef apply_seqences_to_binder<double_binder, first_and_second_types>::type double_bind_target_type;
typedef boost::mpl::vector4<
    boost::mpl::pair<double,int>,
    boost::mpl::pair<float,int>,
    boost::mpl::pair<double,long>,
    boost::mpl::pair<float,long>
> double_bind_target_type_expected;
BOOST_MPL_ASSERT(( is_same<double_bind_target_type, double_bind_target_type_expected> ));

typedef multi_bind<make_pair, 2> multi_binder;
typedef apply_seqences_to_binder<multi_binder, first_and_second_types>::type multi_bind_target_type;
typedef boost::mpl::vector4<
    boost::mpl::pair<double,int>,
    boost::mpl::pair<float,int>,
    boost::mpl::pair<double,long>,
    boost::mpl::pair<float,long>
> multi_bind_target_type_expected;
BOOST_MPL_ASSERT(( is_same<multi_bind_target_type, multi_bind_target_type_expected> ));





typedef mpl::vector<first_types, second_types, third_types> first_and_second_and_third_types;
struct make_triple { template<typename First, typename Second, typename Third> struct apply { typedef mpl::vector3<First, Second, Third> type; }; };

typedef triple_bind<make_triple> triple_binder;
typedef apply_seqences_to_binder<triple_binder, first_and_second_and_third_types>::type triple_bind_target_type_2;
typedef boost::mpl::vector4<
    boost::mpl::vector3<double,int,bool>,
    boost::mpl::vector3<float,int,bool>,
    boost::mpl::vector3<double,long,bool>,
    boost::mpl::vector3<float,long,bool>
> triple_bind_target_type_2_expected;
BOOST_MPL_ASSERT(( is_same<triple_bind_target_type_2, triple_bind_target_type_2_expected> ));

typedef multi_bind<make_triple, 3> multi_binder_3;
typedef apply_seqences_to_binder<multi_binder_3, first_and_second_and_third_types>::type multi_bind_target_type_3;
typedef boost::mpl::vector4<
    boost::mpl::vector3<double,int,bool>,
    boost::mpl::vector3<float,int,bool>,
    boost::mpl::vector3<double,long,bool>,
    boost::mpl::vector3<float,long,bool>
> multi_bind_target_type_3_expected;
BOOST_MPL_ASSERT(( is_same<multi_bind_target_type_3, multi_bind_target_type_3_expected> ));





typedef mpl::vector<
    boost::mpl::vector<
        long double,
        double,
        float,
        //char,
        unsigned char,
        signed char,
        short,
        unsigned short,
        int,
        unsigned int,
        long,
        unsigned long,
        long long,
        unsigned long long
    >,
    mpl::vector<float, double, char, short, int>
> first_and_second_types_2;
typedef apply_seqences_to_binder<multi_binder, first_and_second_types_2>::type multi_bind_target_type_2;





int main(int argc, char* argv[]) {
    printf("%s\n\n", typeid(double_bind_target_type).name());
    printf("%s\n\n", typeid(multi_bind_target_type).name());

    printf("%s\n\n", typeid(triple_bind_target_type_2).name());
    printf("%s\n\n", typeid(multi_bind_target_type_3).name());

    printf("%s\n\n", typeid(multi_bind_target_type_2).name());

    return 0;
}
