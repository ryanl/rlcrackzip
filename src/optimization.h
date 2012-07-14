#define LIKELY(x) __builtin_expect(x,true)
#define UNLIKELY(x) __builtin_expect(x,false)
