#ifndef IZIPREPROCESSOR_H_
#define IZIPREPROCESSOR_H_

#define _IZI_PP_TUPLE_SIZE_I(e0,e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11,e12,e13,e14,e15,size, ...) size
#define IZI_PP_TUPLE_SIZE(...) _IZI_PP_TUPLE_SIZE_I(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)

#define _IZI_PP_TUPLE_GET_0(a0, ...) a0
#define _IZI_PP_TUPLE_GET_1(a0,a1, ...) a1
#define _IZI_PP_TUPLE_GET_2(a0,a1,a2, ...) a2
#define _IZI_PP_TUPLE_GET_3(a0,a1,a2,a3, ...) a3
#define _IZI_PP_TUPLE_GET_4(a0,a1,a2,a3,a4, ...) a4
#define _IZI_PP_TUPLE_GET_5(a0,a1,a2,a3,a4,a5, ...) a5
#define _IZI_PP_TUPLE_GET_6(a0,a1,a2,a3,a4,a5,a6, ...) a6
#define _IZI_PP_TUPLE_GET_7(a0,a1,a2,a3,a4,a5,a6,a7, ...) a7

#define IZI_PP_TUPLE_GET(tuple, idx) _IZI_PP_TUPLE_GET_ ## idx tuple

#define _IZI_PP_CAT(a,b) a ## b
#define _IZI_PP_UCAT(a,b) a ## _ ## b

#define IZI_PP_CAT(a,b)                _IZI_PP_CAT(a,b)
#define IZI_PP_CAT3(a,b,c)             IZI_PP_CAT(IZI_PP_CAT(a,b),c)
#define IZI_PP_CAT4(a,b,c,d)           IZI_PP_CAT(IZI_PP_CAT3(a,b,c),d)
#define IZI_PP_CAT5(a,b,c,d,e)         IZI_PP_CAT(IZI_PP_CAT4(a,b,c,d),e)
#define IZI_PP_CAT6(a,b,c,d,e,f)       IZI_PP_CAT(IZI_PP_CAT5(a,b,c,d,e),f)
#define IZI_PP_CAT7(a,b,c,d,e,f,g)     IZI_PP_CAT(IZI_PP_CAT6(a,b,c,d,e,f),g)
#define IZI_PP_CAT8(a,b,c,d,e,f,g,h)   IZI_PP_CAT(IZI_PP_CAT7(a,b,c,d,e,f,g),h)
#define IZI_PP_CAT9(a,b,c,d,e,f,g,h,i) IZI_PP_CAT(IZI_PP_CAT8(a,b,c,d,e,f,g,h),i)


#define IZI_PP_UCAT(a,b) _IZI_PP_UCAT(a,b)
#define IZI_PP_UCAT3(a,b,c) IZI_PP_UCAT(IZI_PP_UCAT(a,b),c)
#define IZI_PP_UCAT4(a,b,c,d) IZI_PP_UCAT(IZI_PP_UCAT3(a,b,c),d)
#define IZI_PP_UCAT5(a,b,c,d,e) IZI_PP_UCAT(IZI_PP_UCAT4(a,b,c,d),e)

#define _IZI_PP_CONSUME(x) x
#define IZI_PP_CONSUME(x) _IZI_PP_CONSUME x

#define _IZI_ISR_ID__vector_0     0
#define _IZI_ISR_ID__vector_1     1
#define _IZI_ISR_ID__vector_2     2
#define _IZI_ISR_ID__vector_3     3
#define _IZI_ISR_ID__vector_4     4
#define _IZI_ISR_ID__vector_5     5
#define _IZI_ISR_ID__vector_6     6
#define _IZI_ISR_ID__vector_7     7
#define _IZI_ISR_ID__vector_8     8
#define _IZI_ISR_ID__vector_9     9
#define _IZI_ISR_ID__vector_10    10
#define _IZI_ISR_ID__vector_11    11
#define _IZI_ISR_ID__vector_12    12
#define _IZI_ISR_ID__vector_13    13
#define _IZI_ISR_ID__vector_14    14
#define _IZI_ISR_ID__vector_15    15
#define _IZI_ISR_ID__vector_16    16
#define _IZI_ISR_ID__vector_17    17
#define _IZI_ISR_ID__vector_18    18
#define _IZI_ISR_ID__vector_19    19
#define _IZI_ISR_ID__vector_20    20
#define _IZI_ISR_ID__vector_21    21
#define _IZI_ISR_ID__vector_22    22
#define _IZI_ISR_ID__vector_23    23
#define _IZI_ISR_ID__vector_24    24
#define _IZI_ISR_ID__vector_25    25
#define _IZI_ISR_ID__vector_26    26
#define _IZI_ISR_ID__vector_27    27
#define _IZI_ISR_ID__vector_28    28
#define _IZI_ISR_ID__vector_29    29

#define IZI_ISR_ID(isr) IZI_PP_CAT(_IZI_ISR_ID, isr)
#define IZI_IS_SAME_ISR(isr1,isr2) (IZI_ISR_ID(isr1) == IZI_ISR_ID(isr2))

#endif /* IZIPREPROCESSOR_H_ */
