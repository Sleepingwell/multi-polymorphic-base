#ifndef PYMESERIES_MULTI_BIND_HEADER_INCLUDED_F6H54F63J43S6G1SF3G1S3GD1HJ3D1
#define PYMESERIES_MULTI_BIND_HEADER_INCLUDED_F6H54F63J43S6G1SF3G1S3GD1HJ3D1

#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/insert_range.hpp>

#include <boost/mpl/bind.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/protect.hpp>

namespace multibind {
    template<typename Applicator>
    struct double_bind {
        template<typename First>
        struct apply {
            struct type {
                template<typename Second>
                struct apply {
                    typedef typename boost::mpl::apply<Applicator, First, Second>::type type;
                };
            };
        };
    };

    template<typename Applicator>
    struct triple_bind {
        template<typename First>
        struct apply {
            struct type {
                template<typename Second>
                struct apply {
                    struct type {
                        template<typename Third>
                        struct apply {
                            typedef typename boost::mpl::apply<Applicator, First, Second, Third>::type type;
                        };
                    };
                };
            };
        };
    };

    template<typename Applicator, int Size, typename State=boost::mpl::vector0<> >
    struct multi_bind {
        template<typename Arg>
        struct apply {
            struct type : multi_bind<Applicator, Size-1, typename boost::mpl::push_back<State, Arg>::type> {};
        };
    };



    template<typename Applicator, typename State>
    struct multi_bind<Applicator, 1, State> {
        template<typename State, typename Arg>
        struct do_apply;

        template<typename Arg>
        struct do_apply<boost::mpl::vector0<>, Arg> {
            typedef typename boost::mpl::apply<Applicator, Arg>::type type;
        };

        template<typename T1, typename Arg>
        struct do_apply<boost::mpl::vector1<T1>, Arg> {
            typedef typename boost::mpl::apply<Applicator, T1, Arg>::type type;
        };

        template<typename T1, typename T2, typename Arg>
        struct do_apply<boost::mpl::vector2<T1, T2>, Arg> {
            typedef typename boost::mpl::apply<Applicator, T1, T2, Arg>::type type;
        };

        template<typename T1, typename T2, typename T3, typename Arg>
        struct do_apply<boost::mpl::vector3<T1, T2, T3>, Arg> {
            typedef typename boost::mpl::apply<Applicator, T1, T2, T3, Arg>::type type;
        };

        template<typename T1, typename T2, typename T3, typename T4, typename Arg>
        struct do_apply<boost::mpl::vector4<T1, T2, T3, T4>, Arg> {
            typedef typename boost::mpl::apply<Applicator, T1, T2, T3, T4, Arg>::type type;
        };

        // by default, boost only provides apply with upto 5 arguments - use apply directly from here on
        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename Arg>
        struct do_apply<boost::mpl::vector5<T1, T2, T3, T4, T5>, Arg> {
            typedef typename Applicator::template apply<T1, T2, T3, T4, T5, Arg>::type type;
        };

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename Arg>
        struct do_apply<boost::mpl::vector6<T1, T2, T3, T4, T5, T6>, Arg> {
            typedef typename Applicator::template apply<T1, T2, T3, T4, T5, T6, Arg>::type type;
        };

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename Arg>
        struct do_apply<boost::mpl::vector7<T1, T2, T3, T4, T5, T6, T7>, Arg> {
            typedef typename Applicator::template apply<T1, T2, T3, T4, T5, T6, T7, Arg>::type type;
        };

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename Arg>
        struct do_apply<boost::mpl::vector8<T1, T2, T3, T4, T5, T6, T7, T8>, Arg> {
            typedef typename Applicator::template apply<T1, T2, T3, T4, T5, T6, T7, T8, Arg>::type type;
        };

        template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename Arg>
        struct do_apply<boost::mpl::vector9<T1, T2, T3, T4, T5, T6, T7, T8, T9>, Arg> {
            typedef typename Applicator::template apply<T1, T2, T3, T4, T5, T6, T7, T8, T9, Arg>::type type;
        };

        template<typename Arg>
        struct apply : do_apply<State, Arg> {};
    };



    template<typename State, typename Sequences>
    struct apply_sequences_to_binder_sequence {
        struct apply_arg_to_binder_sequence {
            template<typename NewState, typename Arg>
            struct apply {
                typedef typename boost::mpl::protect< boost::mpl::bind<boost::mpl::_1, Arg> >::type apply_arg;
                typedef typename boost::mpl::insert_range<
                    NewState,
                    typename boost::mpl::end<NewState>::type,
                    typename boost::mpl::transform<State, apply_arg>::type
                >::type type;
            };
        };

        typedef typename boost::mpl::accumulate<
            typename boost::mpl::front<Sequences>::type,
            boost::mpl::vector0<>,
            apply_arg_to_binder_sequence
        > new_state;

        typedef typename boost::mpl::eval_if<
            boost::mpl::bool_<(boost::mpl::size<Sequences>::value == 1)>,
            new_state,
            apply_sequences_to_binder_sequence<
                typename new_state::type,
                typename boost::mpl::pop_front<Sequences>::type
            >
        >::type type;
    };

    template<typename Binder, typename Sequences>
    struct apply_seqences_to_binder;

    template<typename Applicator, typename Sequences>
    struct apply_seqences_to_binder<double_bind<Applicator>, Sequences>
      : apply_sequences_to_binder_sequence<boost::mpl::vector1<double_bind<Applicator> >, Sequences>::type {
        BOOST_MPL_ASSERT_RELATION( boost::mpl::size<Sequences>::value, <=, 2 );
    };

    template<typename Applicator, typename Sequences>
    struct apply_seqences_to_binder<triple_bind<Applicator>, Sequences>
      : apply_sequences_to_binder_sequence<boost::mpl::vector1<triple_bind<Applicator> >, Sequences>::type {
        BOOST_MPL_ASSERT_RELATION( boost::mpl::size<Sequences>::value, <=, 3 );
    };

    template<typename Applicator, int Size, typename State, typename Sequences>
    struct apply_seqences_to_binder<multi_bind<Applicator, Size, State>, Sequences>
      : apply_sequences_to_binder_sequence<boost::mpl::vector1<multi_bind<Applicator, Size> >, Sequences>::type {
        BOOST_MPL_ASSERT_RELATION( boost::mpl::size<Sequences>::value, >=, Size );
    };
} // end namespace multibind

#endif // PYMESERIES_MULTI_BIND_HEADER_INCLUDED_F6H54F63J43S6G1SF3G1S3GD1HJ3D1
