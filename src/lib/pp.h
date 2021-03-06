#pragma once

#include <boost/preprocessor.hpp>




#define APPLY_TUPLE(MACRO, TUPLE) MACRO TUPLE;
#define APPLY_SEQ(MACRO, SEQ) APPLY_TUPLE(MACRO, BOOST_PP_SEQ_TO_TUPLE(SEQ))

#define TUPLE_TO_SEQ_R(r, _, TUPLE) (BOOST_PP_TUPLE_TO_SEQ(TUPLE))
#define SEQ_OF_TUPLES_TO_SEQ_OF_SEQ(SEQ) BOOST_PP_SEQ_FOR_EACH(TUPLE_TO_SEQ_R, (), SEQ)


#define APPLY_PRODUCT(MACRO, ...) \
    BOOST_PP_SEQ_FOR_EACH_PRODUCT(MACRO, SEQ_OF_TUPLES_TO_SEQ_OF_SEQ(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))



