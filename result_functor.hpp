#ifndef PYMESERIES_RESULT_FUNCTOR_HEADER_INCLUDED_D6GH4S6G4D634D3GS354BN2D165D4
#define PYMESERIES_RESULT_FUNCTOR_HEADER_INCLUDED_D6GH4S6G4D634D3GS354BN2D165D4

#include <boost/mpl/transform.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/lambda.hpp>

#include <boost/fusion/sequence.hpp> // at_c
#include <boost/fusion/container/vector.hpp>

#include "result_functor_abstract_base.hpp"
//#include "multi_bind.hpp"

// while I think the following is valid syntax, it does not work on msvc 2012
//template<
//    typename T0 = boost::fusion::void_,
//    typename T1 = boost::fusion::void_,
//    typename T2 = boost::fusion::void_,
//    typename T3 = boost::fusion::void_,
//    typename T4 = boost::fusion::void_,
//    typename T5 = boost::fusion::void_,
//    typename T6 = boost::fusion::void_,
//    typename T7 = boost::fusion::void_,
//    typename T8 = boost::fusion::void_,
//    typename T9 = boost::fusion::void_
//> using args_ = boost::fusion::vector<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>;
// ... so do this instead
#define args_ boost::fusion::vector


namespace detail {
    // class for wrapping functors.
    template<
        template<typename, typename> class WrappedFunctor,
        typename InPoint,
        typename OutPoint,
        typename Derived,
        typename BaseClass
    >
    struct functor_wrapper : BaseClass {
        virtual WrappedFunctor<InPoint, OutPoint>* makeFunctor(InPoint const&, OutPoint const&) const {
            return static_cast<Derived const*>(this)->template makeFunctor<InPoint, OutPoint>();
        }
    };





    template<
        typename Derived,
        template<typename, typename> class HeldFunctor
    >
    struct make_pymeseries_functor_concrete_base {
        template<template<typename, typename> class AbstractFunctor>
        struct make_base_functor_for_point_pairs {

            // InOutPointPairs is a vector of pairs that are to be
            // included in this part of the class heirachy.
            template<typename InOutPointPairs>
            struct apply {
                struct functor_maker {
                    template<typename InOutPointPair>
                    struct apply {
                        struct type {
                            template<typename Base>
                            struct apply {
                                typedef functor_wrapper<
                                    HeldFunctor,
                                    typename InOutPointPair::first,
                                    typename InOutPointPair::second,
                                    Derived,
                                    Base
                                > type;
                            };
                        };
                    };
                };

                // get the abstract functors
                typedef typename get_abstract_functor_getter_for_point_pairs<AbstractFunctor, InOutPointPairs>::type abstract_base;

                // create a sequence ready for making an abstract heirachy
                typedef typename boost::mpl::transform<InOutPointPairs, functor_maker>::type ready_for_base;

                struct extend {
                    template<typename Base, typename Derived>
                    struct apply { typedef typename boost::mpl::apply<Derived, Base>::type type; };
                };

                typedef typename boost::mpl::accumulate<ready_for_base, abstract_base, extend>::type type;
            };
        };

        typedef typename boost::mpl::transform<
            pymeseries_inout_point_pairs,
            make_base_functor_for_point_pairs<PymeseriesFunctorBase>
        >::type concrete_base_classes;

        typedef typename boost::mpl::inherit_linearly<concrete_base_classes, boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2> >::type type;
    };




    template<
        template<typename, typename> class HeldFunctor,
        typename Args
    >
    struct Functor : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, Args>,
        HeldFunctor
    >::type {
        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>();
        }
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1
    >
    struct Functor<HeldFunctor, args_<T1> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1> >,
        HeldFunctor
    >::type {
        typedef args_<T1> arg_pack_type;

        Functor(
            T1 const& t1
        ) : argPack_(t1) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2
    >
    struct Functor<HeldFunctor, args_<T1, T2> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2
        ) : argPack_(t1, t2) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3
        ) : argPack_(t1, t2, t3) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3,
        typename T4
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3, T4> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3, T4> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3, T4> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3,
            T4 const& t4
        ) : argPack_(t1, t2, t3, t4) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_),
                boost::fusion::at_c<3>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3, T4, T5> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3, T4, T5> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3, T4, T5> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3,
            T4 const& t4,
            T5 const& t5
        ) : argPack_(t1, t2, t3, t4, t5) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_),
                boost::fusion::at_c<3>(argPack_),
                boost::fusion::at_c<4>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5,
        typename T6
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3, T4, T5, T6> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3,
            T4 const& t4,
            T5 const& t5,
            T6 const& t6
        ) : argPack_(t1, t2, t3, t4, t5, t6) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_),
                boost::fusion::at_c<3>(argPack_),
                boost::fusion::at_c<4>(argPack_),
                boost::fusion::at_c<5>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5,
        typename T6,
        typename T7
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3, T4, T5, T6, T7> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3,
            T4 const& t4,
            T5 const& t5,
            T6 const& t6,
            T7 const& t7
        ) : argPack_(t1, t2, t3, t4, t5, t6, t7) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_),
                boost::fusion::at_c<3>(argPack_),
                boost::fusion::at_c<4>(argPack_),
                boost::fusion::at_c<5>(argPack_),
                boost::fusion::at_c<6>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5,
        typename T6,
        typename T7,
        typename T8
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7, T8> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7, T8> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3, T4, T5, T6, T7, T8> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3,
            T4 const& t4,
            T5 const& t5,
            T6 const& t6,
            T7 const& t7,
            T8 const& t8
        ) : argPack_(t1, t2, t3, t4, t5, t6, t7, t8) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_),
                boost::fusion::at_c<3>(argPack_),
                boost::fusion::at_c<4>(argPack_),
                boost::fusion::at_c<5>(argPack_),
                boost::fusion::at_c<6>(argPack_),
                boost::fusion::at_c<7>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5,
        typename T6,
        typename T7,
        typename T8,
        typename T9
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7, T8, T9> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7, T8, T9> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3, T4, T5, T6, T7, T8, T9> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3,
            T4 const& t4,
            T5 const& t5,
            T6 const& t6,
            T7 const& t7,
            T8 const& t8,
            T9 const& t9
        ) : argPack_(t1, t2, t3, t4, t5, t6, t7, t8, t9) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_),
                boost::fusion::at_c<3>(argPack_),
                boost::fusion::at_c<4>(argPack_),
                boost::fusion::at_c<5>(argPack_),
                boost::fusion::at_c<6>(argPack_),
                boost::fusion::at_c<7>(argPack_),
                boost::fusion::at_c<8>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };





    template<
        template<typename, typename> class HeldFunctor,
        typename T1,
        typename T2,
        typename T3,
        typename T4,
        typename T5,
        typename T6,
        typename T7,
        typename T8,
        typename T9,
        typename T10
    >
    struct Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> > : make_pymeseries_functor_concrete_base<
        Functor<HeldFunctor, args_<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> >,
        HeldFunctor
    >::type {
        typedef args_<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> arg_pack_type;

        Functor(
            T1 const& t1,
            T2 const& t2,
            T3 const& t3,
            T4 const& t4,
            T5 const& t5,
            T6 const& t6,
            T7 const& t7,
            T8 const& t8,
            T9 const& t9,
            T10 const& t10
        ) : argPack_(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10) { }

        template<typename InPoint, typename OutPoint>
        HeldFunctor<InPoint, OutPoint>* makeFunctor(void) const {
            return new HeldFunctor<InPoint, OutPoint>(
                boost::fusion::at_c<0>(argPack_),
                boost::fusion::at_c<1>(argPack_),
                boost::fusion::at_c<2>(argPack_),
                boost::fusion::at_c<3>(argPack_),
                boost::fusion::at_c<4>(argPack_),
                boost::fusion::at_c<5>(argPack_),
                boost::fusion::at_c<6>(argPack_),
                boost::fusion::at_c<7>(argPack_),
                boost::fusion::at_c<8>(argPack_),
                boost::fusion::at_c<9>(argPack_)
            );
        }

    private:
        arg_pack_type
            argPack_;
    };
} // end namespace detail





// metafunction to wrap up a functor
template<template<typename, typename> class FunctorToWrap, typename ConstructorArgs=boost::mpl::na >
struct make_pymeseries_functor {
    typedef detail::Functor<FunctorToWrap, ConstructorArgs> type;
};
#endif // PYMESERIES_RESULT_FUNCTOR_HEADER_INCLUDED_D6GH4S6G4D634D3GS354BN2D165D4
